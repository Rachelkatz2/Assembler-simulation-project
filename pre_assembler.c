

/*
This code file describes a script that replaces the macros with the original code that was included in the macro. An action will be triggered before the first conversion is performed.
*/

#include "main.h"
#include "pre_assembler.h"
#include "functions.h"

boolean is_mcr;/*Determines whether the current line is a macro definition.*/
mcrPtr mcr_node;/*A pointer to the current macro node.*/
mcrPtr pointer;/*Pointer to the current position in the file being pre-assembled.*/
FILE *mcrFile;/*A pointer to the file containing the macros*/
int row_counter;/*A counter for the current row being read from the file.*/
char *new_name_of_file;/*A string containing the new name of the file being pre-assembled.*/
/*
This function handles the pre-assembler algorithm, which is used to spread macros.
@param file A pointer to the file to be pre-assembled.
@param file_name The name of the file to be opened.
@return void
 */
void pre_assembler(FILE *file, char *file_name)
{
	char row[LINE_LEN];
	is_mcr = FALSE;/*A boolean variable indicating whether the current line is a macro definition. */
	row_counter = 1; /* A counter for the current row being read from the file*/
	new_name_of_file = creating_file(file_name);/*A string containing the new name of the file being pre-assembled.*/
	mcrFile = fopen(new_name_of_file, "w");/* A pointer to the file containing the macros*/
	mcr_node = NULL;/*A pointer to the current macro node.*/

	while (fgets(row, LINE_LEN, file) != NULL) /* Read the lines until the end of the file. */
	{
		if (!disregard(row)) /* Ignore line if it's blank or ; */
			reading_line(row);
		row_counter++;
	}

	free_mcr(&mcr_node);/*Freeing the allocated memory.*/
	fclose(mcrFile);/*Closes the file containing the macros. */
}


/**
 * Reads a line of code and checks if it contains a macro or a label.
 * If it's a macro, the line will be added to the macro table until endmacro.
 * If it's not a macro, it will be added to the line table.
 * 
 * @param row The row of code to read.
 */
void reading_line(char *row)
{

	char word[LINE_LEN];

	char *total_line = row;
	row = skip_white_characters(row);/*Skips all leading white characters  from the input row.*/
	copying_word(word, row);/* Check if the first word is a label, if so, check the next word. */

	if (is_label(word))/* Check next word for macro. */
	{
		row = next_word(row);/*Moves the row pointer to the next word in the input row.*/
		copying_word(word, row);/*Copies the next word .*/
	}

	if (is_mcr)/* Checks if the current input row is part of a macro definition.*/
	{ 
		if (!strcmp(word, "endmcr"))
		{
			is_mcr = FALSE;
			return;
		}
		addLine(total_line, word);
	}
	else
	{ 
		isMcr(word, row);/*Checks if the current input row contains a macro definition.*/
		if (!is_mcr)
		{
			addLine(total_line, word);
		}
	}
}
/**
 * Determines whether a given word is a label.
 * A label is defined as a word that ends with a colon.
 * 
 * @param word A string representing the word to check.
 * @return TRUE if the word is a label, FALSE otherwise.
 */
boolean is_label(char *word)
{
	int word_row = strlen(word);
	if (word[word_row - 1] == ':')
		return TRUE;
	else
		return FALSE;
}
/**
 * Checks if the word is a macro or not.
 * 
 * @param word current word.
 * @param row The pointer to a starting position within the row.
 */
void isMcr(char *word, char *row)
{
	if (!strcmp(word, "mcr"))/* enter if macro */
    { 
		int i;
		is_mcr = TRUE;
		/* Gets the macro name and enters it into the macro table. */
		row = next_word(row);
		copying_word(word, row);
		/* Check if macro name is legit and not directive or command name */
		for (i = 0; i < CMD_LIST_LEN; i++)
		{
			if (!strcmp(word, commands[i]))
			{
				printf("Error: The macro name '%s' at line %d is invalid because it is a command name\n", word, row_counter);
				remove(new_name_of_file);
				exit(1);
			}
		}
		for (i = 0; i < ARR_LEN; i++)
		{
			if (!strcmp(word, directives[i]))
			{
				printf("Error: The macro name '%s' at line %d is invalid because it is a directive name\n", word, row_counter);
				remove(new_name_of_file);
				exit(1);
			}
		}
		if ((pointer = checkMcr(mcr_node, word)) != NULL)
		{
			printf("Error: The macro name '%s' at line %d is invalid because this macro is already defined\n", word, row_counter);
			remove(new_name_of_file);
			exit(1);
		}
		addMcr(&mcr_node, word);
	}
}

/* The function adds a new macro to the macro table.
 *
 * @param macroTable A pointer to the pointer of the macro table.
 * @param macroName A string representing the name of the new macro.
 */
void addMcr(mcrPtr *macroTable, char *macroName)
{

	mcrPtr ptr1, ptr2;
	ptr1 = (mcrPtr)malloc(sizeof(Macros));/*Allocating memory to a new macro*/
	if (!ptr1)
	{
		fprintf(stderr, "Dynamic allocation error."); /* catching errors */
		exit(ERROR);
	}
	/* Save the new macro as a node in our table */
	strcpy(ptr1->name, macroName);
	if (*macroTable == NULL) /* if table empty */
		*macroTable = ptr1;
	else
	{ /* add the new macro as the last node in table */
		ptr2 = *macroTable;
		*macroTable = ptr1;
		ptr1->next = ptr2;
	}
}

/* Creating a new line in the new file without macros.

 *@param row A pointer to the string containing the line to be added.
 *@param word A pointer to the string containing the current word in the row.
*/
void addLine(char *row, char *word)
{
	if (is_mcr)
	{
		strcat(mcr_node->substance, row);
	}
	else
	{
	/* Add a line to its type, whether it's a macro name or a regular line of code. */
		if ((pointer = checkMcr(mcr_node, word)) != NULL)
		{
			fputs(pointer->substance, mcrFile);
		}
		else
		{
			fputs(row, mcrFile);
		}
	}
}
/**
 * @brief the function checkes if the macro already exists
 * 
 * @param macroTable macro table to check against
 * @param word current macro
 * @return A pointer to the macro node if found, otherwise NULL.
 */
mcrPtr checkMcr(mcrPtr macroTable, char *word)
{
    
	mcrPtr ptr1;
	ptr1 = macroTable;
	while (ptr1 != NULL)
	{
	if (!strcmp(ptr1->name, word))
		return ptr1;
		ptr1 = ptr1->next;
	}

	return NULL;
}

/* Freeing the memory we allocated to the macro list.
 *@param macroTable A pointer to the head of the macro table to free.
 * @return void
*/
void free_mcr(mcrPtr *macroTable)
{

	mcrPtr p;
	while (*macroTable)
	{
		p = *macroTable;
		*macroTable = (*macroTable)->next;
		free(p);
	}
}
