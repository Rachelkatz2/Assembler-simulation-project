

#ifndef FUNCTIONS_H

#define FUNCTIONS_H

/**Import file.**/
#include "main.h"

/**define**/
#define MAX_EXTENSION_LENGTH 5
#define LEN_STR 10
#define LIMIT_REGIST 7
#define LIMIT_COMMEND 16
#define LOW_LIMIT 48
#define HIGHT_LIMIT 56

/**struct**/
typedef union 
{
	struct
	{
		unsigned int kind : 2;
		unsigned int num : 12;
	}u;
	unsigned int fullReg;
}addressing;

/**Signatures of the functions in functions.c .**/
int isLabelLegal(char arr[],int length,FILE * file1);
void checkingData(FILE * file1);
int checkingMethod(FILE* file1);
int checkingMethod1(FILE* file1);
void checkingString(FILE* file1);
char checkingSpace(FILE* file1);
int whichCode(char arr[]);
void checkImmediate(FILE* file2,char nameParam[],int location);
void checkingMethodLabel(FILE* file2,char nameParam[],int location);
int checkingMethodRegister(FILE* file2,char nameParam[]);
char *creating_file(char *original);
void copying_word(char *word, char *row);
char *skip_white_characters(char *ch);
int disregard(char *row);
int end_of_row(char *row);
char *next_word(char *str);

#endif

