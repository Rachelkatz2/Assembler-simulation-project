
#include "global.h"
#include "functions.h"
#include "parsings.h"
/**

Parses the input file in the second iteration in order to classify each line according to its type.
The function reads each line from the file and identifies whether it is a label, directive or code line,
and performs the relevant actions accordingly.
@param file2 A pointer to the input file in the second iteration.
@return void
*/
void parsing2(FILE *file2)
{
	int i;
	char array[MAX_WORD];
	char row [MAX_ROW];
	char rg=fgetc(file2);/*Read a single character to detect if it reaches the end of the file.*/
	e_num_row=0;
	ei_ic=0;
	ei_err=0;
	e_ext_count=0;
	e_flagValue=0;/* A flag to recognize if there is an entry label in the file, in order to create an entry file.*/
	while(rg!=EOF)/* The function run untill it ends. */
	{
		int flagEntry=0;/* Ask if the label is an entry label.*/
		fseek(file2,-1,SEEK_CUR);/*Go back one step, to read the whole word.*/
		e_num_row++;
		fscanf(file2,"%s",array);/* Read the first word in the line, in order to associate th eline to the type. */
		if(array[0]==';')/* If the line is meaningless or empty, continue.*/
		{
			fgets(row,MAX_ROW,file2);
			rg=fgetc(file2);
			continue;
		}

		if(array[strlen(array)-1]==':')/* If the last sign is colon, it is a label.*/
		{
			fscanf(file2,"%s",array);
		}
		if(strcmp(array,".string")==0||strcmp(array,".data")==0||strcmp(array,".extern")==0)/* If the label is the kind of data or string or extern continue readig*/
		{
			fgets(row,MAX_ROW,file2);
			rg=fgetc(file2);
			continue;
		}
		if(strcmp(array,".entry")==0)/* If the label is the kind of entry, continue reading the data afterwords*/
		{
			e_flagValue=1;
			fscanf(file2,"%s",array);
			i=0;
			while(!flagEntry&&i!=ei_index)
			{
				if(strcmp(es_label[i].label,array)==0)/* Check if the label is set.*/
				{
					es_label[i].isExternal=NUM;
					flagEntry=1;
				}
				i++;
			}
			if(!flagEntry)/* If the label is not defined it is an error.*/
			{
				printf("Error: The label %s is not defined, but it be used as an entry directive. Line: %d\n",array,e_num_row);/*Error message.*/
				ei_err=yes;
			}
			rg=fgetc(file2);
			continue;
		}
		code(file2,array);/* Go to function code.*/
		rg=fgetc(file2);
	}
}

/**
   This function gets lines in the file thar are not labels and try to check if they are commands,
   and encode them as the method of the parameters.
   @param file2 A pointer to the input file in the second iteration.
   @param  arr Lines in the file that are not labels.
   @return void.
*/

void code(FILE* file2,char arr[])
{
	char rg;
	int place;
	int j=0;
	cpuOpcode c;/* Building a data structure to insert the data of the command.*/
	char nameParameter[MAX_WORD];/* A buffer to put in the name of the parameters.*/
	place= whichCode(arr);/*Send to the function whichCode to define what is the command.*/
	if(place==-1) /* If the function did not recognize the command, the line is a different type of line.*/
	{
		fgets(row,MAX_ROW,file2);
		return;
	}
	c.u.opcode=place;/* The opcode of the commmand is as the plase thet the function returned.*/
	c.u.era=ABSOLUTE;/* The command type is absolute*/
	if(!(place==ACT_JMP||place==ACT_BNE||place==ACT_JSR))/* If the commands doesn't use method of addressing #2, go in. */
	{
		int registParameter1,registParameter2;/*If the method of addressing will be register, he creats parameters. */
		int parameter1, parameter2;/* The parameters of the command.*/
		c.u.parameter1=0;
		c.u.parameter2=0;
		parameter1= checkingMethod(file2);/* Check if the method is valid, and which method of addressing it use.*/
		if(parameter1==-1)/* If there is no parameters.*/
		{
			if(es_cmd[place].oper==0)/* If the command doesn't need 	parameters.*/
			{
				c.u.opDest=0;
				c.u.opSource=0;
				eu_code[ei_ic]=c.fullReg;/* Fill in the struct and send th the arrey of codes.*/
				ei_ic++;
			}
			return;
		}
		fseek(file2,-NUM,SEEK_CUR);/* Go back two steps in order to read the all parameter. */
		rg=fgetc(file2);

		while(rg!=' '&&rg!='\t')/* Go back in order to put to recognize the name of the parameter.*/
		{
			fseek(file2,-NUM,SEEK_CUR);
			rg=fgetc(file2);
		}
		rg=fgetc(file2);
		while(rg!=' '&&rg!='\t'&&rg!='\n'&&rg!=','&&file2)/* Put the parameter as a string in order to recognize his type.*/
		{
			nameParameter[j]=rg;
			j++;
			rg=fgetc(file2);
		}
		nameParameter[j]='\0';
		if(rg==' '||rg=='\t')
			rg=checkingSpace(file2);
		if(parameter1==IMMEDIATE)/* Check the type of the first parameter, and send to the function that handles it.*/ 
			checkImmediate(file2,nameParameter,ei_ic+1);
		if(parameter1==LABEL)/* Check what the type of the first parameter, and send to the function that handle with it (checkMethodLabel).*/ 
			checkingMethodLabel(file2,nameParameter,ei_ic+1);
		if(parameter1==REGISTER)/* Check what the type of the first parameter, and send to the function that handle with it (checkMethodRegister).*/ 
		{
			registParameter1=checkingMethodRegister(file2,nameParameter);
		}
		if(rg=='\n'||!file2)/* Take one step back if it comes to an end.*/
			fseek(file2,-1,SEEK_CUR);
		parameter2= checkingMethod(file2);/* Go to next and check what the second parameter type is.*/

		if(parameter2==-1)/* If there isn't another parameter, fill in the data as command with one parameter*/
		{
			if(es_cmd[place].oper==1)/* Only if the function really needs one parameter enter the data.*/
			{
				c.u.opDest=parameter1;
				c.u.opSource=0;
				if(parameter1==REGISTER)/* If the parameter type is a register, put it in the register data.*/
				{
					regist r;
					r.u.destination=registParameter1;
					r.u.source=0;
					r.u.kind=ABSOLUTE;
					eu_code[ei_ic+1]=r.fullReg;
				}
				eu_code[ei_ic]=c.fullReg;/* Fill in the data in the arrey of codes. */ 
				ei_ic+=NUM;
			}
			return;
		}
		fseek(file2,-NUM,SEEK_CUR);/* Go back tow steps in order to read the all parameter*/
		rg=fgetc(file2);

		while(rg!=' '&&rg!='\t'&&rg!=',')/* Go back in order to read the whole word at once.*/
		{
			fseek(file2,-NUM,SEEK_CUR);
			rg=fgetc(file2);
		}
		rg=fgetc(file2);
		j=0;
		while(rg!=' '&&rg!='\t'&&rg!='\n'&&rg!=','&&file2)/* Read the second parameter again in order to define his kind.*/
		{
			nameParameter[j]=rg;
			j++;
			rg=fgetc(file2);
		}
		nameParameter[j]='\0';
		if(es_cmd[place].oper==NUM)/* Only if the command really needs two parameters check the second parameter.*/
		{
			if(parameter2==IMMEDIATE) /* Check the type of the second parameter, and send to the function that handles it.*/ 
				checkImmediate(file2,nameParameter,ei_ic+NUM);
			if(parameter2==LABEL) /* check what the type of the second parameter, and send to the function that handle with it.*/ 
			{
				checkingMethodLabel(file2,nameParameter,ei_ic+NUM);
			}
			if(parameter2==REGISTER) /* check what the type of the second parameter, and send to the function that handle with it.*/ 
			{
				registParameter2=checkingMethodRegister(file2,nameParameter);
			}
			c.u.opDest=parameter2; /* Fill in the data of the second parameter.*/
			c.u.opSource=parameter1;
			eu_code[ei_ic]=c.fullReg;
			if(parameter1==REGISTER||parameter2==REGISTER)/* If the parameter is a register finisher to build the structure of the register.*/
			{
				regist r;
				r.u.kind=ABSOLUTE;
				if(parameter1==REGISTER&&parameter2==REGISTER)/* If both parameters are registers fill in both parameters in one struct*/
				{
					r.u.destination=registParameter2;
					r.u.source=registParameter1;
					eu_code[ei_ic+1]=r.fullReg;
					ei_ic+=NUM;
				}
				else
				{
					if(parameter1==REGISTER)/* if only one of the parameters is registers, handle it.*/
					{
						r.u.destination=0;
						r.u.source=registParameter1;
						eu_code[ei_ic+1]=r.fullReg;
					}
					else
					{
						r.u.destination=registParameter2;
						r.u.source=0;
						eu_code[ei_ic+NUM]=r.fullReg;
					}
					ei_ic+=THREE_IC;
				}
			}
			else
				ei_ic+=THREE_IC;
		}
	}


	else/* If the kind of the command is from the addressing method that use the function jump. */

	{
		int registParameter1,registParameter2;
		int parameter1, parameter2;
		parameter1=checkingMethod1(file2);/* Send the first word to the function that checks this type of command.*/
		if(parameter1==-1)/* If there is no parameter continue to the next line. */
			return;
		if(parameter1!=NUM)
		{
			c.u.parameter1=0;
			c.u.parameter2=0;
			fseek(file2,-NUM,SEEK_CUR);/* Go back tow steps in order to read the all parameter*/
			rg=fgetc(file2);

			while(rg!=' '&&rg!='\t')/* Go back in order to read the all parameter.*/
			{
				fseek(file2,-NUM,SEEK_CUR);
				rg=fgetc(file2);
			}
			rg=fgetc(file2);
			while(rg!=' '&&rg!='\t'&&rg!='\n'&&rg!=','&&file2)/* Read the all parameter to identify the parameter type.*/
			{
				nameParameter[j]=rg;
				j++;
				rg=fgetc(file2);
			}
			nameParameter[j]='\0';
			if(parameter1==IMMEDIATE)/* Check the type of the first parameter, and send to the function that handles it.*/ 
				checkImmediate(file2,nameParameter,ei_ic+1);
			if(parameter1==LABEL)
			{
				checkingMethodLabel(file2,nameParameter,ei_ic+1);
			}
			if(parameter1==REGISTER)/* If the kind of the parameter is register put in the data in an appropriate data structure.*/

			{
				regist r;
				registParameter1=checkingMethodRegister(file2,nameParameter);
				r.u.destination=registParameter1;
				r.u.source=0;
				r.u.kind=ABSOLUTE;
				eu_code[ei_ic+1]=r.fullReg;
			}
			if(rg=='\n'||!file2)
 				fseek(file2,-1,SEEK_CUR);
			parameter2= checkingMethod(file2);/* If the function reaches the end of the line or the end of the file, read the entire line and insert the data.*/
			c.u.era=ABSOLUTE;
			c.u.opDest=parameter1;
			c.u.opSource=0;
			eu_code[ei_ic]=c.fullReg;
			ei_ic+=NUM;
		}
		else
		{
			fseek(file2,-NUM,SEEK_CUR);/* Go back tow steps in order to read the all parameter*/
			rg=fgetc(file2);

			while(rg!=' '&&rg!='\t')/* Go back in order to read the all parameter.*/
			{
				fseek(file2,-NUM,SEEK_CUR);
				rg=fgetc(file2);
			}
			rg=fgetc(file2);
			while(rg!=OPEN_BRACKETS)/* While the next character is not parentheses read the word.*/
			{
				nameParameter[j]=rg;
				j++;
				rg=fgetc(file2);
			}
			nameParameter[j]='\0';
			checkingMethodLabel(file2,nameParameter,ei_ic+1);/*After the command should be alabel check if it is valid.*/
			parameter1=checkingMethod(file2);/* Send the first parameter in parentheses.*/
			fseek(file2,-NUM,SEEK_CUR);/* Go back tow steps in order to read the all parameter*/
			rg=fgetc(file2);

			while(rg!=OPEN_BRACKETS)
			{
				fseek(file2,-NUM,SEEK_CUR);
				rg=fgetc(file2);
			}
			rg=fgetc(file2);
			j=0;
			while(rg!=','&&rg!=' '&&rg!='\n'&&rg!='\t'&&file2)/* Go back in order to read the all word.*/
			{
				nameParameter[j]=rg;
				j++;
				rg=fgetc(file2);
			}
			nameParameter[j]='\0';
			if(parameter1==IMMEDIATE)/* check what the type of the first parameter, and send to the function that handle with it.*/ 
				checkImmediate(file2,nameParameter,ei_ic+NUM);
			if(parameter1==LABEL)/* check what the type of the first parameter, and send to the function that handle with it.*/ 
			{
				checkingMethodLabel(file2,nameParameter,ei_ic+NUM);
			}
			if(parameter1==REGISTER)/* check what the type of the first parameter, and send to the function that handle with it.*/ 
			{
				registParameter1=checkingMethodRegister(file2,nameParameter);
			}
			parameter2= checkingMethod(file2);
			fseek(file2,-NUM,SEEK_CUR);/* go back 2 steps in order to read the all parameter*/
			rg=fgetc(file2);
			j=0;
			while(rg!=' '&&rg!='\t'&&rg!=',')
			{
				fseek(file2,-NUM,SEEK_CUR);
				rg=fgetc(file2);
			}
			rg=fgetc(file2);
			while(rg!=CLOSE_BRACKETS&&rg!=' '&&rg!='\t'&&rg!='\n'&&file2)/* go back in order to read the all word.*/
			{
				nameParameter[j]=rg;
				j++;
				rg=fgetc(file2);
			}
			nameParameter[j]='\0';
			if(parameter2==IMMEDIATE)/* Check the type of the first parameter, and send to the function that handles it.*/ 
				checkImmediate(file2,nameParameter,ei_ic+THREE_IC);
			if(parameter2==LABEL)/* Check the type of the first parameter, and send to the function that handles it.*/ 
			{
				checkingMethodLabel(file2,nameParameter,ei_ic+THREE_IC);
			}
			if(parameter2==REGISTER)/* Check the type of the first parameter, and send to the function that handles it.*/ 
			{
				registParameter2=checkingMethodRegister(file2,nameParameter);
			}
			c.u.parameter1=parameter1;/* Fill in the last data in the structure.*/
			c.u.parameter2=parameter2;
			c.u.opDest=JUMP;
			c.u.opSource=0;
			eu_code[ei_ic]=c.fullReg;
			if(parameter1==REGISTER||parameter2==REGISTER)/*  If one of the parameter is a register, handle with it.*/
			{
				regist r;
				r.u.kind=ABSOLUTE;
				if(parameter1==REGISTER&&parameter2==REGISTER)/* If both of the parameters is register put in both parameter in one struct.*/
				{
					r.u.destination=registParameter2;
					r.u.source=registParameter1;
					eu_code[ei_ic+NUM]=r.fullReg;
					ei_ic+=THREE_IC;
				}
				else
				{
				if(parameter1==REGISTER)/* If one of the parameters is a register, fill in the last data in the struct.*/
				{
					r.u.destination=0;
					r.u.source=registParameter1;
					eu_code[ei_ic+NUM]=r.fullReg;
				}
				else
				{
					r.u.destination=registParameter2;
					r.u.source=0;
					eu_code[ei_ic+THREE_IC]=r.fullReg;
				}
				ei_ic+=FOUR_IC;
			}
		}
		else
			ei_ic+=FOUR_IC;
		}
	}
}

