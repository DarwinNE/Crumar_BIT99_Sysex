#ifndef __BIT99_MIDI__
#define __BIT99_MIDI__

typedef enum {BIT01, BIT99} cr_model;

int bit99_program_dump(cr_model, unsigned int program);
int bit99_program_dump_all(cr_model);
void bit99_set_callback(void);
void bit99_set_output_file(FILE *f);
int bit99_send_file(char *fn);

#endif