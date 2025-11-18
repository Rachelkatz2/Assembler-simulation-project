#ifndef WRITE_H

#define WRITE_H

/**define**/
#define START_ADDRESSE 100
#define MAX_MEMORY 900
#define MOV_BIT 13
#define BITS 14
/**The function signatures of white.h**/
void externalWrite(FILE * file2);
void writeEntry(FILE* file2);
void writeObject(FILE* file2);

#endif
