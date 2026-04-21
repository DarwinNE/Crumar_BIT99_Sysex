#include <stdio.h>
#include <strings.h>
#include <ctype.h>

#include "bit99_midi.h"
#include "bit99.h"
#include "os_midi.h"
#include "ansi_codes.h"

#include <CoreFoundation/CFRunLoop.h>

// ****************** Default configuration *********************

cr_model model=BIT99; // BIT99 or BIT01
int destination=-1; // -1 means pick up the last MIDI interface
bool interpret=true;

// **************************************************************




#define BUFFER_SIZE 1024
char buffer[BUFFER_SIZE];
char base_file_name[BUFFER_SIZE];
char full_file_name[BUFFER_SIZE];

FILE *fout;


int check_file_name(int pr)
{
    if (strcmp(base_file_name, "")==0) {
        printf("Enter a file name, first.\n");
        return 1;
    }
    // We already ensured that the place for pr number and .syx exists.
    if(pr>0 && pr<100) {
        sprintf(full_file_name, "%s_pr%d.syx",base_file_name,pr);
    } else {
        sprintf(full_file_name, "%s_all.syx",base_file_name);
    }
    printf("Full file name: %s\n", full_file_name);
    fout = fopen(full_file_name, "w");
    return 0;
}

void press_a_key(void)
{
    printf("\nPRESS A KEY TO CONTINUE\n");
    for(int i=0; i<5;++i) midi_sleep_100ms();
    wait_for_key();
}

void strip_cr(char *s)
{
    while(*s!='\0') {
        if(*s=='\n')
            *s='\0';
        ++s;
    }
}

void main_menu(void)
{
    char choice;
    int exit_loop=0;
    int pr;

menu:
    printf(C_CLEAR);
    printf(C_REVERSE);
    printf("**************************************************************\n");
    printf("  CRUMAR BIT99/BIT01 toolkit, by Davide Bucci, v. 1.0, 2026   \n");
    printf("**************************************************************\n");
    printf(C_NORMAL);
    printf("\n   %sMAIN MENU%s\n\n", C_BOLD, C_NORMAL);
    printf("    %s 1 %s - Set model.................: %s%s%s \n",
        C_REVERSE, C_NORMAL,
        C_BOLD, model==BIT01?"BIT01":"BIT99", C_NORMAL);
    printf("    %s 2 %s - Set the base file name....: %s%s%s\n",
        C_REVERSE, C_NORMAL,
        C_BOLD, base_file_name, C_NORMAL);
    printf("    %s 3 %s - Interpret received Sysex..: %s%s%s \n",
        C_REVERSE, C_NORMAL,
        C_BOLD, interpret?"Yes":"No", C_NORMAL);
    printf("    %s 4 %s - Receive a single program.\n", C_REVERSE, C_NORMAL);
    printf("    %s 5 %s - Receive all programs.\n", C_REVERSE, C_NORMAL);
    printf("    %s 6 %s - Send a file.\n", C_REVERSE, C_NORMAL);
    printf("    %s 7 %s - Interpret a file.\n", C_REVERSE, C_NORMAL);
    printf("\n");
    printf("    %s M %s - Set MIDI out..............: ", C_REVERSE, C_NORMAL);
    if(midi_get_description_destination(destination,buffer,sizeof(buffer))
        !=NULL)
    {
        printf("%s\n", buffer);
    } else {
        printf("(none)\n");
    }
    printf("\n");
    printf("    %s Q %s - Exit\n",C_REVERSE, C_NORMAL);
choose:
    printf("\nChoose an option: ");
    while (exit_loop==0) {
        fgets(buffer, BUFFER_SIZE-1, stdin);
        strip_cr(buffer);
        switch(tolower(buffer[0])) {
            case 'm':
                printf("%sAvailable destinations:%s\n", C_BOLD, C_NORMAL);
                for(int i=0; i<midi_get_number_of_destinations();++i) {
                    printf("%d - %s\n", i,
                        midi_get_description_destination(i, buffer,
                        sizeof(buffer)));
                }
                printf("\nChoose the destination: ");
                fgets(buffer, BUFFER_SIZE-20, stdin);
                strip_cr(buffer);

                sscanf(buffer,"%d", &pr);
                if(midi_set_destination(pr)==0) {
                    destination = pr;
                }
                goto menu;
            case '1':
                if (model==BIT01) model=BIT99; else model=BIT01;
                goto menu;
                break;
            case '2':
                printf("Enter the base file name (it will be completed with "
                    " the .syx extension): ");
                fgets(base_file_name, BUFFER_SIZE-20, stdin);
                strip_cr(base_file_name);
                goto menu;
                break;

            case '3':
                if (interpret) interpret=false; else interpret=true;
                goto menu;
                break;
            case '4':
                if (strcmp(base_file_name, "")==0) {
                    printf("Enter a file name, first.\n");
                    press_a_key();
                    goto menu;
                    break;
                }
                printf("Which program to dump (1-99, other to abort)? ");
                scanf("%d",&pr);
                getchar();
                if(pr<1 || pr>99) {
                    printf("Invalid program, abort");
                    goto choose;
                }

                if(check_file_name(pr)==0) {
                    bit99_set_output_file(fout);
                    bit99_program_dump(model, pr);
                    fclose(fout);
                    fout=NULL;
                    bit99_set_output_file(NULL);
                    if (interpret)
                        bit99_sysex(full_file_name);
                    press_a_key();
                    goto menu;
                } else {
                    goto choose;
                }
                break;
            case '5':
                if(check_file_name(-1)==0) {
                    bit99_set_output_file(fout);
                    bit99_program_dump_all(model);
                    fclose(fout);
                    fout=NULL;
                    bit99_set_output_file(NULL);
                    if (interpret)
                        bit99_sysex(full_file_name);
                    press_a_key();
                    goto menu;
                } else {
                    goto choose;
                }
                break;
            case '6':
                if(strcmp(full_file_name,"")==0) {
                    printf("Enter full file name with extension: ");
                    fgets(full_file_name, BUFFER_SIZE-1, stdin);
                    strip_cr(full_file_name);
                } else {
                    printf("Enter full file name with extension or ");
                    printf("type return for %s\n", full_file_name);
                    fgets(buffer, BUFFER_SIZE-1, stdin);
                    if(strcmp(buffer, "\n")!=0)
                        strcpy(full_file_name, buffer);
                }
                bit99_send_file(full_file_name);
                press_a_key();
                goto menu;
                break;
            case '7':
                if(strcmp(full_file_name,"")==0) {
                    printf("Enter full file name with extension: ");
                    fgets(full_file_name, BUFFER_SIZE-1, stdin);
                    strip_cr(full_file_name);
                } else {
                    printf("Enter full file name with extension or ");
                    printf("type return for %s\n", full_file_name);
                    fgets(buffer, BUFFER_SIZE-1, stdin);
                    if(strcmp(buffer, "\n")!=0)
                        strcpy(full_file_name, buffer);
                }
                bit99_sysex(full_file_name);
                press_a_key();
                goto menu;
                break;
            case 'q':
                exit_loop=1;
                break;
            default:
                goto choose;
        }
    }
    printf(C_CLEAR);
}

int main(int argc, char** argv)
{
    midi_enumerate();
    midi_init_in(1,0);
    if(destination ==-1) {
        destination = midi_get_number_of_destinations()-1;
    }
    midi_init_out(destination,0);
    bit99_set_callback();
    main_menu();
    return 0;
}