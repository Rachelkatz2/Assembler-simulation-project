/**Import files.**/
#include "global.h"
#include "main.h"
#include "functions.h"
#include "parsings.h"


/**
   Check if a given label is legal.
   A label is considered legal if it meets the following criteria:
   It does not already exist in the es_label array.
   It is not a saved word (a command or a register name).
   It consists only of letters and numbers.
   @param arr a character array representing the label to be checked.
   @param length an integer representing the length of the label array.
   @param file1 a pointer to a FILE object representing the input file being processed.
   @return an integer value of 0 if the label is legal, 1 otherwise.
*/

int isLabelLegal(char arr[],int length,FILE *file1)
{
	int i;
	for(i=0;i<length;i++)/*Check if the label already exists in the es_label array.*/
	{
		if(strcmp(arr,es_label[i].label)==0)/*If the label already exist.*/
		{
			printf("Error:  The label %s is set twice.	Line: %d\n",arr,e_num_row);/*Error message.*/
			ei_err=yes;
			fgets(row,MAX_ROW,file1);
			return 1;
		}
	}
	for(i=0;i<LIMIT_COMMEND;i++)/*Check if the label is a command.*/
	{
		if(strcmp(arr,es_cmd[i].name)==0)/*If the lebal is a command.*/
		{
			printf("Error:  The label %s cant be label because it a saved word (name of a command).	Line: %d\n",arr,e_num_row);/*Error message.*/
			ei_err=yes;
			fgets(row,MAX_ROW,file1);
			return 1;
		}
	}
	for(i=0;i<=LIMIT_REGIST;i++)/*Check if the label is a name of register.*/
	{
		if(strcmp(arr,regs[i])==0)/*If the label name of register.*/
		{
			printf("Error:  The label %s cant be label because it a saved word (name of register).	Line: %d\n",arr,e_num_row);/*Error message.*/
			ei_err=yes;
			fgets(row,MAX_ROW,file1);
			return 1;
		}
	}
	for(i=1;i<strlen(arr);i++)/*Check if the label consists only of letters and numbers.*/
	{
		if(isalnum(arr[i])==0)/*If the label does not consists only of letters and numbers.*/
		{
			printf("Error:  The label %s not only letters and numbers.	Line: %d\n",arr,e_num_row);/*Error message.*/
			ei_err=yes;
			fgets(row,MAX_ROW,file1);
			return 1;
		}
	}
	return 0;
}


/**
   This function reads a line with the .string directive and inserts the data to the data array.
   If the line is invalid, it prints an error message and reads the remaining characters in the line.
   @param file1 - a pointer to the file containing the input code.
   @return void
*/

void checkingString(FILE* file1)
{
	char rg;/*check for spaces before the string.*/
	rg=checkingSpace(file1);
	if(rg!='\"')/*If the data doesn't start with a quotation mark. */
	{
		printf("Error:  The string in the directive .string need to start with \"	Line: %d\n",e_num_row);/*Error message.*/
		fgets(row,MAX_ROW,file1);
		ei_err=yes;
		return;
	}
	rg=fgetc(file1);/*Read the next character from the file and continue reading until the end of the string.*/
	while(rg!='\"')/*As long as he didn't reach the end of the string he read the data.*/
	{
		if(rg=='\n'||!file1)/*If the end of the file or a new line is reached before finding the ending double quote, print an error message and return.*/
		{
			printf("Error:  The string in the directive .string need to finish with \"	Line: %d\n",e_num_row);/*Error message.*/
			ei_err=yes;
			return ;
		}
		ed_data[ei_dc].variable=rg;/*Insert the character to the place in the chart.*/
		ei_dc++;
		rg=fgetc(file1);/*Read the next character from the file.*/
	}
	/*Check if there are any characters after the ending double quote.*/
	rg=checkingSpace(file1);
	ed_data[ei_dc++].variable='\0';/*Insert 0 to the place in the chart after all the string that insert.*/
	if(rg!='\n'&&rg!=EOF)/*If there is more character after the " . */
	{
		printf("Error:  The string in the directive .string need to finish with \"	Line: %d\n",e_num_row);/*Error message.*/
		ei_err=yes;
		fgets(row,MAX_ROW,file1);
		return;
	}
}

/**
   This function reads a line from a .data file and inserts the numbers into the data array. If the line is not written correctly, it prints an error message and reads the rest of the line before exiting the function.
   @param file1 - a pointer to a FILE struct representing the .data file.
   @return void.
*/
void checkingData(FILE *file1)
{
	char rg=checkingSpace(file1);/*Read the first character.*/
	while(rg!='\n'&&rg!=EOF)/*As long as not reached the end of line.*/
	{
		char string[LEN_STR]="\0";/* Initialize string to empty.*/
		int i=0,num=0;
		if(rg==',')/* If there are two consecutive commas.*/
		{
			printf("Error:  The legal order in .data need to be integer and than comma and not two comma in a row.	Line: %d    \n",e_num_row);/*Error message.*/    
			ei_err=yes;
			while(rg!='\n'&&rg!=EOF)/*Read all remaining characters in the line.*/
			rg= fgetc(file1);
			return;
		}

		while(rg!='\n'&&rg!=EOF&&rg!=','&&rg!=' '&&rg!='\t')/*Read a number.*/
		{

			if(rg!='-'&&rg!='+'&&isdigit(rg)==0)/* If the character is not a number.*/
			{
       
				printf("Error:  it is not legal to enter things that not integers in .data.	Line:  %d\n",e_num_row);/*Error message.*/
				ei_err=yes;
				while(rg!='\n'&&rg!=EOF)
					rg=fgetc(file1);
				return;
			}
			if((rg=='-'||rg=='+')&&i!=0)/*if this is not a number*/
			{
				printf("Error:  it is not legal to enter things that not integers in .data.	Line:  %d\n",e_num_row);
				ei_err=yes;
				while(rg!='\n'&&rg!=EOF)/*Read all remaining characters in the line.*/
					rg=fgetc(file1);
				return;
			}
			string[i]=rg;/* Append the character to the string.*/
			i++;
			rg=fgetc(file1);/* Read the next character.*/
		}
		num=atoi(string);/*Convert the string to an integer.*/
		ed_data[ei_dc].variable=num;/* Insert the number to the corresponding position in the data array.*/
		ei_dc++;
		if(rg==' '||rg=='\t')/* Skip any spaces or tabs.*/
			rg=checkingSpace(file1);
		if(rg!=','&&rg!='\n'&&rg!=EOF)/*If their is not comma between the numbers.*/
		{
			printf("Error:  The legal order in .data need to be integer and than comma.	Line:  %d\n",e_num_row);/*Error message.*/
			ei_err=yes;
			while(rg!='\n'&&file1)
				rg=fgetc(file1);
			return;
		}
		if(rg==',')/*If there is a comma after the number.*/
		{
			rg=checkingSpace(file1);/*Skip any spaces or tabs/*/
			if(rg=='\n'||!file1)/*If the data section ends with a comma.*/
			{
				printf("error:   In .data not need to finish with ,	Line:  %d\n",e_num_row);/*Error message.*/
				ei_err=yes;
			}
		}
	}
}



/**
   Finds the index number of a command within the es_cmd array.
   @param arr: An array of characters representing the command name to be searched for.
   @return The index number of the matching command, or -1 if the command name is not found.
*/

int whichCode(char arr[])
{
	int i;
	for(i=0;i<LIMIT_COMMEND;i++)
	{
	if(strcmp(es_cmd[i].name,arr)==0)
		return i;
	}
	return -1;
}



/**
   This function reads spaces and tabs from a file until it reaches a character that is not a space or a tab.
   It then returns the first non-space/non-tab character encountered.
   @param file1 A pointer to a file opened in read mode.
   @return The first non-space/non-tab character encountered in the file.
*/
char checkingSpace(FILE* file1)
{
	char rg;
	rg=fgetc(file1);
	while(rg==' '||rg=='\t')
		rg=fgetc(file1);
	return rg;
}


/**
   This function checks what kind of method is written in a line of code and returns an integer representing the type of method.
   If the function reaches the end of the line or there is no character, it returns -1.
   @param file1 A pointer to a file opened in read mode.
   @return 0 if immediate, 1 if label, 3 if register, and -1 if the function reaches the end of the line or there is no character.
*/
int checkingMethod(FILE* file1)
{
	char rg;
	rg= checkingSpace(file1);
	if(rg=='\n'||rg==EOF)/*If he come to the end of the line.*/
		return -1;
	if(rg=='#')/*If the method is immediate, the function returns 0.*/
	{
		while(rg!=' '&&rg!=','&&rg!='\t'&& rg!='\n'&&file1)
		{
			rg=fgetc(file1);
		}
		return IMMEDIATE;
	}
	if(rg=='r')/*If it start with r check if it register.*/
	{
		rg=fgetc(file1);
		if(isdigit(rg))/*If the after character is number.*/
		{
			int number;
			number=rg-'0';
			if(number>=0&&number<=LIMIT_REGIST)/*Check if it one of the register.*/
			{
				rg=fgetc(file1); 
				if(rg!=' '&&rg!=','&&rg!='\t'&& rg!='\n'&&file1&&rg!=CLOSE_BRACKETS)/*If it not register.*/
				{
					while(rg!=' '&&rg!=','&&rg!='\t'&& rg!='\n'&&file1&&rg!=CLOSE_BRACKETS)
						rg=fgetc(file1);
					return LABEL;
				}
				return REGISTER;/*If it register.*/
			}
		}
	}
	while(rg!=' '&&rg!=','&&rg!='\t'&& rg!='\n'&&file1)/*If not register or immidiate it register.*/
	{
		rg=fgetc(file1);
	}
	return LABEL;
}

/**
   This function receives a pointer to a file and checks the first character in the current line to determine its type:
   label, register, immediate, jump or an empty line. It returns an integer representing the type of the line.
   If the function reaches the end of the line or there are no characters, it returns -1.
   @param file1 A pointer to a file opened in read mode.
   @return An integer representing the type of the current line in the file: LABEL (1) if the line is a label.
                                                                             REGISTER (2) if the line is a register.
                                                                             IMMEDIATE (3) if the line is immediate.
                                                                             JUMP (4) if the line is a jump with parameters.
   -1 if the function reaches the end of the line or there are no characters.
*/
int checkingMethod1(FILE* file1)
{
	char rg;
	rg= checkingSpace(file1);/* Get the first non-whitespace character in the line. */
	if(rg=='\n'||!file1)/*If the function reaches the end of the line or there are no characters, it returns -1.*/
		return -1;
	if(rg=='#')/*If the method is immediate, the function returns 0.*/
	{
		while(rg!=' '&&rg!=','&&rg!='\t'&& rg!='\n'&&file1)
		{
			rg=fgetc(file1);/* Move to the end of the immediate value. */
		}
		return IMMEDIATE;
	}
	if(rg=='r')/*If it start with r check if it register.*/
	{  
		rg=fgetc(file1);/* Move to the end of the immediate value. */
		if(isdigit(rg))/*If the after character is number.*/
		{
			int number=rg-'0';
			if(number>=0&&number<=LIMIT_REGIST)/*Check if it one of the register.*/
			{
				rg=fgetc(file1); 
				if(rg==' '||rg==','||rg=='\t'|| rg=='\n'||!file1)
				{
					return REGISTER;
				}
			}
		}
		fseek(file1,-1,SEEK_CUR);/*Move the file pointer back to the beginning of the register. */
	}
	/* If the line is not a label, register or immediate, check if it's a jump with parameters or a label. */
	while(rg!=' '&&rg!=','&&rg!='\t'&& rg!='\n'&&file1&&rg!=OPEN_BRACKETS)
	{
 		rg=fgetc(file1);/*Move to the end of the current word. */
	}
	if(rg== OPEN_BRACKETS)/*If it come to ( it jump with parameters method.*/
		return JUMP;
	return LABEL;/* Otherwise, it's a label. */
}
    
/**
   @brief This function inserts an immediate value into the array command at a specified location.
   @param file2 - A pointer to the file we're currently reading from.
   @param nameParam - A string containing a number and a location to insert it in the array.
   @param location - The location in the array to insert the number into.
   @return void.
*/

void checkImmediate(FILE* file2,char nameParam[],int location)
{
	int number;
	addressing ad;
 
	number=atoi(nameParam+1);/*Move the data from string to number.*/
	ad.u.kind=ABSOLUTE;
	ad.u.num=number;
	eu_code[location]=ad.fullReg;
}

/**

   Inserts a label into the appropriate location in the command array.
   @param file2 - A pointer to the output file.
   @param nameParam - A string containing the label name to be inserted.
   @param location - The location where the label is to be inserted in the command array.
   @return - void.
   @errors - If the label is not found in the label table, an error message is printed to the console and the program exits.
*/

void checkingMethodLabel(FILE* file2,char nameParam[],int location)
{
	int i,flag=no;/*The flag shows if the label is in the label table.*/
	addressing ad;
	for(i=0;i<ei_index&&flag==no;i++)/*Transfers the entire table of labels as long as the label is not found.*/
	{
		if(strcmp(nameParam,es_label[i].label)==0)/*if the label found.*/
		{
			flag=yes;
			if(es_label[i].isExternal==1)/*If the label is external, add it to the external diagram.*/
			{
				ad.u.kind=EXTERNAL;
				ad.u.num=0;
    
				if(e_ext_count==0)/*If by now there is no external label, build a new external chart.*/
				{
					e_ext_label=(extLabelStruct*)malloc(sizeof(extLabelStruct));
					if(!e_ext_label)
					{
						printf("Error: Memory allocation failed.	Line: %d\n", e_num_row);/*Error message.*/
						exit(0);
					}
				}
				else/*Adding to the extearnal chart another data.*/
				{
					e_ext_label=(extLabelStruct*)realloc(e_ext_label,sizeof(extLabelStruct)*(e_ext_count+1));
					if(!e_ext_label)
					{
						printf("Error: Memory allocation failed.	Line: %d\n", e_num_row);/*Error message.*/
						exit(0);
					}
				}
				strcpy(e_ext_label[e_ext_count].label,nameParam);/*Add Label's name to the external data.*/
				e_ext_label[e_ext_count].address=location;/*Add Label's location to the external data.*/
				e_ext_count++;
			}
			else/*If it not external updating the address of the label and the is relocatable.*/
			{
				ad.u.num=es_label[i].address;
				ad.u.kind=RELOCATABLE;
			}
			eu_code[location]=ad.fullReg;
		}
	}
	if (flag==no)/*If the label is not in the label chart.*/
	{
		printf("Error:  The variable %s wasn't defined as a label.	Line: %d\n",nameParam,e_num_row);/*Error message.*/
		ei_err=yes;
		checkingMethod(file2);
	}
}

/**   
   this function getting a string that include register and place to insert and return what a register number.
   @param file2 A pointer to a file.
   @param nameParam A string that includes register and place to insert.
   @return int The register number extracted from the name parameter, or -1 if no valid register number was found.
*/

int checkingMethodRegister(FILE* file2,char nameParam[])
{
	int i;
	for(i=LOW_LIMIT;i<HIGHT_LIMIT ;i++)
	{
		if(nameParam[1]==i)
		{
			return i-'0';
		}
	}
	return -1;
}

/**
   This function dynamically allocates memory for a new string with an added ".am" extension.
   It first allocates memory for the new string, extending the size of the original string by the maximum length of the extension.
   If the dynamic allocation fails, the function prints an error message to the standard output and exits with an error code.
   Then, the function copies the original string to the new string using the strcpy function and adds the ".am" extension using the strcat function.
   Finally, the function returns a pointer to the new string.
   Dynamically allocates memory for a new string with an added extension and returns a pointer to it.
   @param original A pointer to the original string.
   @return char* A pointer to the new string with an added extension, or NULL if dynamic allocation failed.
*/

char *creating_file(char *original)
{
	char *different = (char *)malloc(strlen(original) + MAX_EXTENSION_LENGTH); /* Extends the string to max. */
	if (different == NULL)
	{
		fprintf(stdout, "Dynamic allocation error."); /* Catching errors. */
		exit(ERROR);
	}
	strcpy(different, original); 
	strcat(different, ".am");    
	return different;
}

/**
   This function copies a word from the given row into the given word buffer. The word is copied until its end or until the maximum line length (LINE_LEN) is reached. If the given word or row pointers are NULL, the function returns without performing any action.
   Copies a word from a given row into a given word buffer
   @param word A buffer to store the copied word
   @param row The row containing the word to be copied
   @return void
*/
void copying_word(char *word, char *row)
{
	int i = 0;
	if (word == NULL || row == NULL)
		return;

	while (i < LINE_LEN && !isspace(row[i]) && row[i] != '\0') /* Copying word until its end to *word. */
	{
		word[i] = row[i];
		i++;
	}
	word[i] = '\0';
}

/**
    skip to the next non spaces to.
    @param line the line we are working on.
    @return char* the pointer to the char that starts next word.
 */
char *next_word(char *str)
{
	if (str == NULL)
		return NULL;
	while (!isspace(*str) && !end_of_row(str))
		str++; /* Skip rest of characters in the current line (until a space). */
	str = skip_white_characters(str); /* Skip spaces */
	if (end_of_row(str))
		return NULL;
	return str;
}

/**
   Skips any white space characters at the beginning of the string and returns
   a pointer to the first non-white space character.
   @param ch - a character pointer to a string.
   @return a pointer to the first non-white space character in the string.
   If the input pointer is NULL, returns NULL.
*/
char *skip_white_characters(char *ch)
{
	if (ch == NULL)
		return NULL;
	while (isspace(*ch)) /* Continue the loop if the character is a whitespace. */
		ch++;
	return ch;
}

/**
   Checks if the given character pointer should be disregarded.
   A character pointer is considered to be disregarded if it points to a semicolon
   (';') or a null terminator ('\0') or a newline character ('\n') after skipping
   any white space characters at the beginning of the string.
   @param row - a character pointer to a row.
   @return 1 if the row should be disregarded, 0 otherwise.
*/
int disregard(char *row)
{
	row = skip_white_characters(row);
	return *row == ';' || *row == '\0' || *row == '\n';
}


/**
   Checks if the given character pointer points to the end of a row.
   A row is considered to be ended if the pointer is NULL or points to
   a null terminator ('\0') or a newline character ('\n').
   @param row - a character pointer to a row.
   @return 1 if the row has ended, 0 otherwise.
*/
int end_of_row(char *row)
{
	return row == NULL || *row == '\0' || *row == '\n';
}
