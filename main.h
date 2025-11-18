#ifndef MAIN_H
#define MAIN_H

/**Import libraries.**/
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <math.h>



/** Defined variables.**/
#define EXT_READ ".as" /* extention for read assembly file */
#define EXT_MCR ".am"  /* extention of write macro file */
#define EXT_OBJECT ".ob" /* extention of write object file */
#define EXT_EXTERN ".ext" /* extention of write external file */
#define EXT_ENTRY ".ent" /* extention of write entry file */
/** Length Constants **/
#define LINE_LEN 82  /* Line max size is 80 , extra 2 bits space for \n or \0 */
#define LABEL_LEN 30
#define CMD_LIST_LEN 16
#define RAM_MACHINE 2000
#define ERROR 1
#define MAX_ROW 80
#define START_ADDRESS 100  /* Offset for CODE start address */
#define RAM_MACHINE 2000
#define MAX_WORD 31
#define ARR_AS 0
#define ARR_AM 1
#define ARR_OB 2
#define ARR_EXT 3
#define ARR_ENT 4
#define NUM_OF_FILE 5
#define MAX_LINE_LEN 50


/**enum**/
typedef enum boolean {FALSE,TRUE}boolean;/*Boolean definition.*/
enum commands {MOV, CMP, ADD, SUB, NOT, CLR, LEA,  INC, DEC, JMP, BNE, RED, PRN, JSR, RTS, STOP, UNKNOWN_COMMAND};/*enum to save the actions.*/


/**struct**/
typedef struct {
	char label[MAX_WORD];/*Represents a label identifier with a maximum length of MAX_WORD.*/
	short address;/*Represents a short length memory address.*/
}extLabelStruct;

typedef struct data{
	int variable : 14;/*Represents a data matrix of variables of size 14 bits each.*/
}dataMatrix;

struct cmdStruct { /* stores commands such as mov, stop, inc etc. */
	char name[5]; /* Four chars + '0' */
	int code; /* code between 0 and 15 */
	int oper; /* How many operands for this command (0-2) */
	int permit[2][4]; /* Table of 2*4 permittable source/destinations operands: immediate, direct, indirect, register. */
};

/**extern**/
extern const char *commands[];
extern const char *directives[];


#endif
