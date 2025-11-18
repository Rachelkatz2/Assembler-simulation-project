/*Rachel Klar 325415867 
  Gitty Assaf 214182420*/

/**Import files.**/
#include "main.h"
#include "pre_assembler.h"
#include "parsings.h"
#include "write.h"


/* Global variables are defined here: */
symbols* es_label=NULL;/*The array of labels includes all the labels, identified by their address, indicating whether they are input or external, and whether they are defined through the command line*/
int e_flagValue=0;/*This variable indicates the number of label entries present.*/
char row [MAX_ROW];/* string that can contain a line*/
int e_num_row;/*Display the current line number.*/
unsigned int eu_code[RAM_MACHINE]; /* array to store machine code */
 dataMatrix ed_data[RAM_MACHINE]; /* array to store machine data (numbers and strings) */
int ei_ic=START_ADDRESS, ei_dc=0; /*The Instruction Counter and Data Counter are two variables used to track the execution of a program. The value of the Instruction Counter is set to 100 at the beginning because the code starts at 100.*/
int ei_err; /* have we reached an error in file */
 int ei_index=0;/* counter of labels*/
extLabelStruct* e_ext_label=NULL;/*The external label array stores the label name and its line number where it is used.*/
int e_ext_count=0;/* This variable indicates the number of times the external label is used.*/
char *regs[]={"r0","r1","r2","r3","r4","r5","r6","r7"};/* A array of all the names of register*/
struct cmdStruct es_cmd[] = {
/* The command table contains a list of all commands, including their corresponding string, code, number of arguments, and permissible parameters for the source and destination. The table also includes four permissible parameters, namely immediate, label, jump with parameters, and register, which are represented by 1 and 0 indicating permitted and not permitted, respectively. */
	{"mov", 0, 2, {{1,1,0,1}, {0,1,0,1}}},
	{"cmp", 1, 2, {{1,1,0,1}, {1,1,0,1}}}, 
	{"add", 2, 2, {{1,1,0,1}, {0,1,0,1}}}, 
	{"sub", 3, 2, {{1,1,0,1}, {0,1,0,1}}}, 
	{"not", 4, 2, {{0,0,0,0}, {0,1,0,1}}}, 
	{"clr", 5, 2, {{0,0,0,0}, {0,1,0,1}}}, 
	{"lea", 6, 2, {{0,1,0,0}, {0,1,0,1}}}, 
	{"inc", 7, 1, {{0,0,0,0}, {0,1,0,1}}}, 
	{"dec", 8, 1, {{0,0,0,0}, {0,1,0,1}}}, 
	{"jmp", 9, 1, {{0,0,0,0}, {0,1,1,1}}}, 
	{"bne", 10, 1, {{0,0,0,0}, {0,1,1,1}}}, 
	{"red", 11, 1, {{0,0,0,0}, {0,1,0,1}}}, 
	{"prn", 12, 1, {{0,0,0,0}, {1,1,0,1}}}, 
	{"jsr", 13, 1, {{0,0,0,0}, {0,1,1,1}}}, 
	{"rts", 14, 0, {{0,0,0,0}, {0,0,0,0}}}, 
	{"stop", 15, 0, {{0,0,0,0}, {0,0,0,0}}}
};

const char* directives[] = {".data", ".string", ".entry", ".extern" };

const char *commands[] = {"mov", "cmp", "add", "sub", "not", "clr", "lea", "inc", "dec", "jmp", "bne", "red", "prn", "jsr", "rts", "stop"};



int main (int argc, char *argv[]) 
{
	FILE *file1, *file2; /* file1 for read file2 for write */
	int i;
	long size;  /*the position in the file*/
	char nameOfFile[NUM_OF_FILE][MAX_LINE_LEN]; /* array to store up to 5 file names */
	if (argc == 1) 
	{
		fprintf (stderr, "No input file.\n");
		return 1;
	}
	for (i=1;i<argc;i++) /* for all files in command line */
	{
		strcpy (&nameOfFile[ARR_AS][0],*(argv+i)); 
		strcat (&nameOfFile[ARR_AS][0], EXT_READ); /* append .as to file name */
		strcpy (&nameOfFile[ARR_AM][0],*(argv+i));
		strcat (&nameOfFile[ARR_AM][0], EXT_MCR); /* append .am to file name */
		strcpy (&nameOfFile[ARR_OB][0],*(argv+i));
		strcat (&nameOfFile[ARR_OB][0], EXT_OBJECT); /* append .ob to file name */
		strcpy (&nameOfFile[ARR_EXT][0],*(argv+i));
		strcat (&nameOfFile[ARR_EXT][0], EXT_EXTERN); /* append .ext to file name */
		strcpy (&nameOfFile[ARR_ENT][0],*(argv+i));
		strcat (&nameOfFile[ARR_ENT][0], EXT_ENTRY); /* append .ent to file name */
		printf ("Input filename: %s\n", &nameOfFile[ARR_AS][0]);
		printf ("Macro filename: %s\n", &nameOfFile[ARR_AM][0]);
		printf ("Object filename: %s\n", &nameOfFile[ARR_OB][0]);
		printf ("Extern filename: %s\n", &nameOfFile[ARR_EXT][0]);
		printf ("Entry filename: %s\n", &nameOfFile[ARR_ENT][0]);
		
           	

		if ((file1 = fopen(&nameOfFile[ARR_AS][0], "r")) == NULL)/* open .as file for read */
		{ 
			fprintf (stderr, "can't open %s\n", &nameOfFile[ARR_AS][0]);
			return 1; 
		}
		else
		{
			fseek (file1, 0, SEEK_END);
			size = ftell(file1); 
			if (0 == size) /*checks if the file is empty*/
			{
				fprintf(stderr, "Error: File is empty.\n");
				fclose(file1);
				return 1;
			}  
			else
			{
				rewind(file1);
			}
			pre_assembler(file1, argv[i]);/* Making the macros */
			fclose(file1);
		}
		

		if ((file1 = fopen(&nameOfFile[ARR_AM][0], "r")) == NULL)/* open .am file for read */
		{ 
			fprintf (stderr, "can't open %s\n", &nameOfFile[ARR_AM][0]);
			return 1; 
		} 
		else
		{
			parsing (file1); /* parse the file */
			fclose(file1);
		}
		if ((file1 = fopen(&nameOfFile[ARR_AM][0], "r")) == NULL) /* Open .am file for read. */
		{
			fprintf (stderr, "can't open %s\n", &nameOfFile[ARR_AM][0]);
			return 1; 
		} 

		if (ei_err)  /* If we reached an error exit. */
			continue;
		parsing2(file1);
		if (ei_err)  /* If we reached an error exit. */
			continue;
		if ((file2 = fopen(&nameOfFile[ARR_OB][0], "w")) ==NULL) { /* open .ob file to write */
			fprintf (stderr, "can't open %s\n", &nameOfFile[ARR_OB][0]);
		return 1; 
	} 
	writeObject(file2); /* Write .ob file content. */
	if(e_ext_count!=0)/* If there is a command that was used with an external label. */
	{
		if ((file2 = fopen(&nameOfFile[ARR_EXT][0], "w")) ==NULL) /* open .ext file to write */
		{
			fprintf (stderr, "can't open %s\n", &nameOfFile[ARR_EXT][0]);
			return 1; 
		} 
		externalWrite(file2); /* Write .ext file content. */
	}
	if(e_flagValue!=0)/* If there is a entries label. */
	{
		if ((file2 = fopen(&nameOfFile[ARR_ENT][0], "w")) == NULL) /* Open .ent file to write. */
		{
			fprintf (stderr, "can't open %s\n", &nameOfFile[ARR_ENT][0]);
			return 1; 
		} 
		writeEntry(file2); /* Write .ent file content. */
	}
	if(e_ext_count!=0)/* Release the external chart. */
		free(e_ext_label);
	if(es_label!=0)/*release the label chart*/
		free(es_label);
	}
	return 0;
}


