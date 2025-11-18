
#include "functions.h"
#include "parsings.h"
#include "main.h"
#include "global.h"
int flag_label;/* Check if the variable is indeed a label.*/
/* this project get the file that the user enter and read it, while reading the function counter how many place the file need for the data and for the commands.
  @brief Parses an assembly file and extracts the relevant data to assemble it.
  The function reads the assembly file line by line and extracts the relevant data,
  such as the instructions, the labels and the data sections. It saves this information
  into the appropriate data structures, such as the symbol table and the code and data
  arrays. The function also performs various error checks along the way to ensure the
  validity of the input file.
  @param file1 A pointer to the input assembly file.
  @return void
*/
void parsing(FILE *file1)
{
 char array [MAX_WORD];
 int directiveSentence;/* A flag indicating whether the row is a directive Sentence.*/
 char rg=fgetc(file1);
 e_num_row=0;/* A counter of the number of rows.*/
 flag_label=0;
 ei_ic=START_ADDRESS;/* counter of code.*/
 ei_dc=0,/* counter of data.*/
 ei_index=0;
 ei_err=0;
  while(rg!=EOF)/* A loop that ran until the end of the file was reached.*/
  {
    fseek(file1,-1,SEEK_CUR);/* Go back one line to read the whole word.*/
    fscanf(file1,"%s",array);/* read the first word.*/
    flag_label=0;
    e_num_row++;
    if(array[0]==';')/* In case the line is empty, read to the end of the line and go back.*/
    { 
      fgets(row,MAX_ROW,file1);
      rg=fgetc(file1);
      continue;
    }
 
   if(array[strlen(array)-1]==':')/* If the first word is a label.*/
   { 
      char string[MAX_WORD];
      strncpy(string,array,strlen(array)-1);/* Inserts a string into an array to test it.*/
      string[strlen(array)-1]='\0';
      flag_label=yes;/* Lighting the flag of labels.*/
      if(!(isalpha(array[0])))/* If the first character is not a letter.*/
      {
       printf("Error:  Labels must start with a letter. %s does not start with a letter	Line: %d\n",array,e_num_row);/*Error message.*/
       ei_err=yes;
       fgets(row,MAX_ROW,file1);
       rg=fgetc(file1);
       continue;
      }
      
      if(isLabelLegal(string,ei_index,file1))/* Sends to a function that checks whether the label is set correctly.*/
      {
       rg=fgetc(file1);
       continue;
      }
      if(ei_index==0)/*If no code has been written before, build a place of memory.*/
      {
       es_label=(symbols*)malloc(sizeof(symbols));
       if(!es_label)
       {
        printf("Error: Memory allocation failed.	Line: %d\n", e_num_row);
        exit(0);
       }
      }
      else
      {
       es_label=(symbols*)realloc(es_label,(ei_index+1)*sizeof(symbols));/*Building a place for the next label. */
       if(!es_label)
       {
        printf("Error: Memory allocation failed.	Line: %d\n", e_num_row);
        exit(0);
       }
      }
      strncpy(es_label[ei_index].label,array,strlen(array)-1);
      es_label[ei_index].label[strlen(array)-1]='\0';
      fscanf(file1,"%s",array);/* Read the next word.*/
   }
      
   directiveSentence= checkingDirective(array, file1);/* Sending to a function that performs a line check. and sees whether the line is a prompt or not.*/
   if(directiveSentence!=1)
    checkingCode(array,file1);/* Send to a function that checks if it is code.*/
   if(flag_label==1 ) /* If it is labels advance the counter.*/ 
    ei_index++;
   rg=fgetc(file1);
  }
betweenfun();/* Send to the function betweenfun before going over the second time.*/
}
/**

This function checks if a given word is a directive keyword, and performs the appropriate actions.
If the keyword is ".data" or ".string", it puts the appropriate data in the label structure.
If the keyword is ".data", it sends the file pointer to a function that reads data.
If the keyword is ".string", it sends the file pointer to a function that reads strings.
If the keyword is ".extern" or ".entry", it checks if the data after it is legal.
If the keyword is ".extern", it checks if the label is legal, and puts it in the label array.
If the keyword is ".entry", it sets the flag_label to no, since there is no label for an entry directive.
@param arr A character array representing the word to be checked.
@param file1 A file pointer to the file being parsed.
@return Returns 1 if the word is a directive keyword and the appropriate actions were taken, otherwise returns 0.
*/
int checkingDirective(char arr[],FILE *file1)
{ 
	if(strcmp(arr,".string")==0||strcmp(arr,".data")==0)/* If the word is data or string, put in the right data in the label structure.*/
	{
		if(flag_label)
		{
			es_label[ei_index].address=ei_dc;
			es_label[ei_index].isCode=no;
			es_label[ei_index].isExternal=no;
		}
		if(strcmp(arr,".data")==0)/* If is data, send to a function that read data.*/
			checkingData(file1);
     
			if(strcmp(arr,".string")==0)/* If it is a string, send to the function that read strings.*/
			checkingString(file1);
		return 1;
	}
	if(strcmp(arr,".extern")==0||strcmp(arr,".entry")==0)/* If the word is entry, or extern Check if the following data is valid.*/
	{
		char breaking [MAX_WORD];
		char rg= checkingSpace(file1);
		if(rg=='\n'||rg==EOF)/* If after entry or extern the file or line ends, it is an error.*/
		{
			printf("Error: After %s has to be one parameter	Line: %d\n",arr,e_num_row);/*Error message.*/
			ei_err=yes;
			breaking[0]='\0';
			return 1;
		}
		fseek(file1,-1,SEEK_CUR);
		fscanf(file1,"%s",breaking);/* Read the next word.*/
		rg= checkingSpace(file1);
		if(rg!='\n'&&(file1))/* If after the second word the line didn't finish it's an error.*/
		{  
			fgets(row,MAX_ROW,file1);
			printf("Error: After %s has to be just one parameter	Line: %d\n",arr,e_num_row);/*Error message.*/
			ei_err=yes;
			return 1;
		}     
		if(strcmp(arr,".extern")==0)   /* If the type is extern' check if it's lawful.*/
		{
			if(!(isalpha(breaking[0])))/* If the first letter is not an alphabet letter it's an error.*/ 
			{
				printf("Error:  The label has to start with a letter ,and %s doesn't start with a letter.	Line: %d\n",breaking,e_num_row);/*Error message.*/
				ei_err=yes;
				fgets(row,MAX_ROW,file1);
				return 1;
			}
			if(isLabelLegal(breaking,ei_index,file1))/* Check if the label is lawful.*/
			return 1;
			if(ei_index==0)
			{
				es_label=(symbols*)malloc(sizeof(symbols));/* Build up a place for the labels.*/
				if(!es_label)
				{
					printf("Error: Memory allocation failed.	Line: %d\n", e_num_row);/*Error message.*/
					exit(0);
				}
			}
			else/* If there is place for labeles, continue.*/
			{
				es_label=(symbols*)realloc(es_label,sizeof(symbols)*(ei_index+1));
				if(!es_label)
				{
					printf("Error: Memory allocation failed.	Line: %d\n", e_num_row);/*Error message.*/
					exit(0);
				}
			}
			strcpy(es_label[ei_index].label,breaking);/* Pyt in the data in the arrey.*/
			es_label[ei_index].label[strlen(breaking)]='\0';
			es_label[ei_index].address=0;
			es_label[ei_index].isExternal=yes;
			es_label[ei_index].isCode=0;
			flag_label=yes;
		}
		if(flag_label==yes&&strcmp(arr,".entry")==0)/* If his type is entry there isn't a label now.*/
		{
			strcpy(es_label[ei_index].label,"\0");
			flag_label=no;
		}
		return 1;
	}
	return 0;
}



/**
these funtion gets the first word of the line, and the file to read, and check if the code is legal. 
@param arr  The first word of the line.
@param file1  file pointer to the file being parsed.
@return void.
*/
void checkingCode(char arr[],FILE *file1)  
{
	int place;
	place= whichCode(arr);/* Checks the command location.*/
	if(place==-1)
	{ 
		printf("Error: The command has no meaning.	Line: %d \n",e_num_row);/*Error message.*/
		ei_err=yes;
		fgets(row,MAX_ROW,file1);
		return;
	}
	if(flag_label)/* If the code is a label.*/
	{
		es_label[ei_index].address=ei_ic;
		es_label[ei_index].isCode=yes;
		es_label[ei_index].isExternal=no; 
	}
	ei_ic++;
	if(!(place==ACT_JMP||place==ACT_BNE||place==ACT_JSR))/* If the command is not from the type of method of addressing #2.*/

	{
		char rg;
		int counter=0;
		int parameter1,parameter2;
		parameter1= checkingMethod(file1);/* Reads the first parameter and checks its type.*/
		fseek(file1,-1,SEEK_CUR);
		rg=checkingSpace(file1);
		if(parameter1!=-1)/* If there is one parameter and then empty space. the number of parameters is one.*/
		{
			if(rg=='\n'||!file1)
				counter=1;
			else
			{ 
				if(rg!=',')/* If there isn't a comma between the parameters it is an error.*/
				{
					printf("Error: There is not comma between the paramters of the command %s.	Line: %d\n",arr,e_num_row);/*Error message.*/
					ei_err=yes; 
					fgets(row,MAX_ROW,file1);
					return;
				}
				else
				{
					parameter2= checkingMethod(file1);/* Pass the second parameter to the test function.*/
					if(parameter2==-1)/* If there is no parameter after the comma, report an error.*/
					{
						printf("Error: There is not any paramater after the comma %s.	Line: %d\n",arr,e_num_row);/*Error message.*/
						ei_err=yes; 
						return;
					}
					counter=NUM;
					fseek(file1,-1,SEEK_CUR);
					rg=checkingSpace(file1);
					if(rg!='\n'&&rg!=EOF)/* If there is more parameters counter them.*/
					{
						checkingMethod(file1);
						counter++;
					}
				}
			}
		}
       
		if(counter>es_cmd[place].oper)/* If the parameters are more than it has to be itis an error.*/
		{
			printf("Error: Too much paremetrs for the command %s.	Line: %d\n",es_cmd[place].name,e_num_row);/*Error message.*/
			ei_err=yes;
			return;
		}
		if(counter<es_cmd[place].oper)/* If there are fewer parameters than there should be, this is an error.*/
		{
			printf("Error: Too little paremetrs for the command %s.	Line: %d \n",es_cmd[place].name,e_num_row);/*Error message.*/
			ei_err=yes;
			return;
		}
		if(counter==1)
		{
			if(es_cmd[place].permit[1][parameter1]==0)/* If the command cann't use the type of a spesific parameter it is an error.*/
			{
				printf("Error: The method is not available for the command %s.	Line: %d\n",es_cmd[place].name,e_num_row);/*Error message.*/
				ei_err=yes;
				return;
			}
		}
		if(counter==NUM)
		{
			if(es_cmd[place].permit[0][parameter1]==0||es_cmd[place].permit[1][parameter2]==0)/* If the command cann't use the type of a spesific parameter it is an error.*/
			{
				printf("Error: The method is not available for the command %s.	Line: %d\n",es_cmd[place].name,e_num_row);/*Error message.*/
				ei_err=yes;
				return;
			}
		}
		if(counter==1)/* Procede the counter of code according to the parameters.*/
			ei_ic++; 
		if(counter==NUM)
		{
			if(parameter1==REGISTER&&parameter2==REGISTER)
				ei_ic++;
			else
				ei_ic+=NUM;
		}
	}
	else/* If the commands are from the type of "jmp"*/
	{ 
 		char rg;
		int parameter=checkingMethod1(file1);/* Send the first parameter for checking*/
		if(parameter==-1)/* If thre aren't parameters it is an error.*/
		{
			printf("Error: Too little paremetrs for the command %s.	Line: %d\n",es_cmd[place].name,e_num_row);/*Error message.*/
			ei_err=yes;
			return;
		}
		fseek(file1,-1,SEEK_CUR);
		rg=checkingSpace(file1);
		if(rg!='\n'&&file1&&rg!=OPEN_BRACKETS)/* If after the first word there isn't brackets or end of line' it is an error.*/
		{
			fgets(row,MAX_ROW,file1);
			printf("Error: Too much paremetrs for the command %s.	Line: %d\n",es_cmd[place].name,e_num_row);/*Error message.*/
			ei_err=yes;
			return;
		}
		if(es_cmd[place].permit[1][parameter]==0)/* If the command cann't use the type of a spesific parameter it is an error.*/
		{
			printf("Error: The method is not available for the command %s.	Line: %d\n",es_cmd[place].name,e_num_row);/*Error message.*/
			ei_err=yes;
			return;
		}
		ei_ic++;
		if(parameter==NUM) /* If it use method of addressing #2.*/
		{
			int parameter1,parameter2;/*Build up two parameters for the type of command.*/
			parameter1=checkingMethod(file1);/* Send to a function to check the first parameter.*/
			fseek(file1,-1,SEEK_CUR);
			rg=fgetc(file1);
			if(parameter1==-1)/* If there isn't any parameter it's an error.*/
			{
				printf("Error: There is no paramters in the command %s.	Line: %d\n",es_cmd[place].name,e_num_row);/*Error message.*/
				ei_err=yes;
				return;
			}
			if(rg!=',')/* If after the first parameter there isn't a comma it is an error.*/
			{
				fgets(row,MAX_ROW,file1);
				printf("Error: The paramters of the command %s are not legal.	Line: %d\n",es_cmd[place].name,e_num_row);/*Error message.*/
				ei_err=yes;
				return;
			}
			parameter2=checkingMethod(file1);/* Send the second parameter for checking.*/
			if(parameter2==-1)/* If there is just one parameter it's an error.*/
			{
				printf("Error: There is just one paramter in the command %s.	Line: %d\n",es_cmd[place].name,e_num_row);/*Error message.*/
				ei_err=yes;
				return;
			}
			if(parameter2==REGISTER)/*Just by a rregister go back one step because it read less than othher kinds.*/
				fseek(file1,-1,SEEK_CUR);
			else
				fseek(file1,-NUM,SEEK_CUR);
			rg=fgetc(file1);
			if(rg!=CLOSE_BRACKETS)/* If there aren't brackets after the parameters, it's an error.*/
			{
				printf("Error: Missing brackets in command %s.	Line: %d\n",es_cmd[place].name,e_num_row);/*Error message.*/
				ei_err=yes;
				if(rg!='\n'&&file1)
					fgets(row,MAX_ROW,file1);
				return ;
			}
			if(parameter1==REGISTER&&parameter2==REGISTER)/* Procede the counter according to the type of the parameters.*/
				ei_ic++;
			else
				ei_ic+=NUM;
		}
	}
}


