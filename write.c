/**Import files.**/
#include "global.h"
#include "parsings.h"
#include "write.h"

/** This function accepts lines of code and writes in a new file the lines of code in the special language of the project (Creates the .ob file). 
    @param file2 - a pointer to the file where the data and code should be written.
	@return void.
*/
void writeObject(FILE* file2)
{ 
	int i;
	fprintf(file2,"	%d %d",ei_ic,ei_dc);/* The function first prints the number of instructions (ei_ic) and the number of data (ei_dc) in separate integers.*/ 
	fputc('\n',file2);
	for(i=0;i<ei_ic;i++)
	{
		int j;
		unsigned short mask=1<<MOV_BIT;/*Writes the code instructions and data to the file, using a special mask to handle bits.*/
 		if(ei_ic<MAX_MEMORY)
			fputc('0',file2);
		fprintf(file2,"%d",i+START_ADDRESSE);/*Each line in the file represents an instruction or a data entry, and starts with a unique identifier for each entry.*/
		fputc('\t',file2);
		for(j=BITS;j>0; j--)/* Move only 14 times because that the data structures are sized at 14 bits.*/
		{
			/*The code instructions are represented by a sequence of dots and slashes, where a dot represents a 0 bit, and a slash represents a 1 bit.*/
			if(!(mask&eu_code[i]))
				fputc('.',file2);
			else                
				fputc('/',file2);
			mask=mask>>1;
		}
		fputc('\n',file2);
	}

	for(i=0;i<ei_dc;i++)
	{
		int j;
		unsigned int mask=1<<MOV_BIT;
		if((i+ei_ic)<MAX_MEMORY)
			fputc('0',file2);
		fprintf(file2,"%d",i+ei_ic+START_ADDRESSE);
		fputc('\t',file2);
		for(j=BITS;j>0; j--)/* Move only 14 times because that the data structures are sized at 14 bits.*/
		{
			if(!(mask&ed_data[i].variable))/* Check by the arrey of data every bite.*/
				fputc('.',file2);
			else
				fputc('/',file2);
			mask=mask>>1;
		}
		fputc('\n',file2);
	}

}

/**
   Writes the entries labels and their addresses to a file (Creates the .ent file).
   @param fp2 A pointer to the output file to write the entries to.
   @return void
*/
void writeEntry(FILE* fp2)
{
	int i;
	for(i=0;i<ei_index;i++)
	{
		if(es_label[i].isExternal==NUM)/* If the label is marked as an entry label write it down in the new file.*/
		{
			fprintf(fp2,"%s",es_label[i].label);/*Write the label name to the output file.*/
 			fputc('\t',fp2);/*Add a tab character as a separator.*/
			fprintf(fp2,"%d",es_label[i].address);/* Write the label address in the memory to the output file.*/
			fputc('\n',fp2);/*Add a new line character to separate entries.*/
		}
	}
}

/**
   this function gets a pointer to a new created file, and write down the all extern labels that are written by the input file (Creates the .ext file).
   @param fp2 The file pointer to write to.
   @return void.
*/
void externalWrite(FILE *fp2)
{
	int i=0;
	for(;i<e_ext_count;i++)
	{
		fprintf(fp2,"%s",e_ext_label[i].label);/* If the label is marked as an extern label write it down in the new file.*/
		fputc('\t',fp2);/*Add a tab character as a separator.*/
		fprintf(fp2,"%d",e_ext_label[i].address+START_ADDRESSE);/* Write the external label to the file.*/
		fputc('\n',fp2);/*Add a new line character to separate entries.*/
	}
}
