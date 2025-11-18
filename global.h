#ifndef GLOBAL_H

#define GLOBAL_H

/**Import files.**/
#include "functions.h"
#include "parsings.h"
#include "main.h"

/* All global variables are declated in main.c file. They are explained in main.c here they are defined so we can use them in other files */
/**extern**/
extern symbols* es_label; /* An array of structures that contains all the labels written in the input.*/
extern unsigned int eu_code[];/* An array containing all the codes of the commands.*/
extern struct cmdStruct es_cmd[];
extern dataMatrix ed_data[];/* An array containing the project data.*/
extern int ei_ic, ei_dc, ei_index, ei_err;/* Counters of commands, labels, and data.*/
extern char row [MAX_ROW];
extern int e_num_row;/* A counter of lines in the code.*/
extern int e_flagValue;/* A flag to mark if there is any entry variables in the file, to know if to build up a file.*/
extern extLabelStruct* e_ext_label;/* Pointer to build the label structure.*/
extern int e_ext_count;/* A counter of the external labels.*/
extern char * regs[];

#endif
