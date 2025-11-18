#ifndef PARSINGS_H

#define PARSINGS_H

/**Import file.**/
#include "main.h"


#define ACT_JMP 9
#define ACT_BNE 10
#define ACT_JSR 13
#define NUM 2
#define OPEN_BRACKETS 40
#define CLOSE_BRACKETS 41
#define THREE_IC 3
#define FOUR_IC 4
/**enum**/
typedef enum{IMMEDIATE=0,LABEL,JUMP,REGISTER}address;
typedef enum {ABSOLUTE=0,EXTERNAL,RELOCATABLE} codeEnum;
enum {no=0,yes};

/**Structures**/
typedef struct{
	char label [MAX_WORD];/*Represents the name of the corresponding symbol. The field is represented by an array of characters with a length of MAX_WORD.*/
	int address;/*Represents the address of the symbol within the code file.*/
	int isExternal;/* Indicates the type of the symbol. The value of the field is set to 0 if it is not external, 1 if it is external, and 2 if it is internal.*/
	int isCode;/*indicates whether the symbol is a code label or a data label. The value of the field is set to 1 if it is a code label and 0 if it is a data label.*/
}symbols;

typedef union 
{
	struct
	{
		unsigned int kind : 2; /*Represents the type of message transmitted through the computer system.*/
		unsigned int destination : 6;/*Represents the destination address for the transmitted message.*/
		unsigned int source : 6;/*Represents the source address for the transmitted message.*/
	}u;
	unsigned int fullReg;
}regist;

typedef union
{
	struct
	{	
		unsigned int era : 2;/*A 2-bit field representing the addressing mode of the operands.*/
		unsigned int opDest : 2;/*A  2-bit field representing the addressing mode of the destination operand.*/
		unsigned int opSource : 2;/*A 2-bit field representing the addressing mode of the source operand.*/
		unsigned int opcode : 4;/*A  4-bit field representing the opcode of the instruction.*/
		unsigned int parameter2 : 2;/*A 2-bit field representing the addressing mode of the second parameter.*/
		unsigned int parameter1 : 2;/*A  2-bit field representing the addressing mode of the first parameter.*/
		unsigned int unused : 18;/*A n 18-bit field that is not currently used.*/
	} u;
	unsigned int fullReg;
} cpuOpcode;


/**The function signatures of pre_assembler.h**/
void parsing(FILE *file1);
int checkingDirective(char arr[],FILE *file1);
void checkingCode(char arr[],FILE *file1);
void parsing2(FILE * file2);
void code(FILE* file2,char arr[]);
void betweenfun();

#endif
