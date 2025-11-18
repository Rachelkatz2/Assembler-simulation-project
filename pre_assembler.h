
#ifndef PRE_ASSEMBLER_H
#define PRE_ASSEMBLER_H

/**define**/
/* Since there are no details we will assume these constants for maximum macro size. */
#define MCR_ROWS 10
#define NAME_LEN 30
#define ARR_LEN 4

/**Structures**/

/*Define a linked list of macros and a pointer to that list. Assume that the length of the max macro name is equal to the length of the max label.*/
typedef struct structMacros * mcrPtr;
typedef struct structMacros {
	char name[NAME_LEN]; /* Mcr name */
	char substance[MCR_ROWS *LINE_LEN]; /* Save the line substance of the mcr. */
	mcrPtr next; /* A pointer to the next label in the list. */
} Macros;


/**The function signatures of pre_assembler.h**/
void reading_line(char *row);
void pre_assembler(FILE *file, char *file_name);
boolean is_label(char *word);
void isMcr(char *word, char *row);
void addMcr(mcrPtr * macroTable, char * macroName);
void addLine (char * row, char * word);
mcrPtr checkMcr(mcrPtr macroTable ,char * word);
void free_mcr(mcrPtr * macroTable);

#endif
