#include <stdio.h>
#include "os_midi.h"
#include "bit99_midi.h"
#include "ansi_codes.h"

enum {false=0, true} sysex_EOX_received;
#define EOX 0xF7

FILE *fout=NULL;

/** Set a file for the output of Sysex

*/
void bit99_set_output_file(FILE *f)
{
    fout=f;
}

/** Dump a single program via Sysex
    @param program the program to be dumped in the range [1, 99].
*/
int bit99_program_dump(cr_model cr, unsigned int program)
{
    unsigned char msg[] = {
        0xF0,   // Sysex start
        0x25,   // Manifacturer ID (Crumar)
        0x20,   // Device ID + channel (0x10 for the BIT 01)
        0x09,   // Request program dump
        0x20,   // Identifies model and channel (0x10 for the BIT 01)
        0x00,   // This is the byte to change for the program 0-98
        0xF7};
    if(cr==BIT01) {
        msg[2]=0x10;
        msg[4]=0x10;
    }
    if (program >99) {
        fprintf(stderr, "This is a program for a Bit 99, not a Bit %d!\n",
            program);
        fprintf(stderr, "The program number should be between 1 and 99\n");
        return 1;
    } else if (program==0) {
        fprintf(stderr, "The program number should be between 1 and 99\n");
    }
    msg[5]=program-1;
    sysex_EOX_received=false;

    printf("P%2d ",program);
    fflush(stdout);
    midi_send(msg, sizeof(msg));
    while(sysex_EOX_received==false) {
        // wait for EOX
        midi_sleep_10ms();
    }
    // Ensure that there is a pause. The BIT 99 does not like to receive
    // Sysex requests too frequently.
    midi_sleep_10ms();
    printf("R ");
    fflush(stdout);
    return 0;
}

/** Dump all programs
*/
int bit99_program_dump_all(cr_model m)
{
    printf("Dumping programs 1 to 99:\n");
    for(int i=1; i<100; ++i)
        if (bit99_program_dump(m, i)) return 1;
    printf("\n");
    return 0;
}

static void bit99_callback(unsigned char *data, int len)
{
    if(fout!=NULL) {
        fwrite(data, sizeof(unsigned char), len, fout);
    }
    if(fout==NULL) {
        printf(C_SAVE);
        //printf(C_POSITION("24","1"));
        last_line();
        printf("%s MIDI RECEIVED DATA: %s ", C_REVERSE, C_NORMAL);
    }
    for (int i = 0; i < len; ++i) {
        if(fout==NULL) printf("0x%X ", data[i]);
        if(data[i]==EOX) {
            sysex_EOX_received=true;
        }
    }

    if(fout==NULL)
        printf(C_RESTORE);
    fflush(stdout);
}

int bit99_send_file(char *fn)
{

    FILE *fin=fopen(fn, "r");
    int ch;

    if(fin==NULL) {
        fprintf(stderr, "Could not open file %s", fn);
        return 1;
    }

    // Reading file character by character
    while ((ch = fgetc(fin)) != EOF) {
        if (midi_send((unsigned char *)&ch, 1))
            return 1;
        if(ch==0xF7) {
            midi_sleep_100ms();
            printf(".");
            fflush(stdout);
        }
    }
    fclose(fin);
    printf("\n");
    return 0;
}

void bit99_set_callback(void)
{
    midi_set_user_callback(bit99_callback);
}