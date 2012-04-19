

#ifndef OUTPUT_H_
#define OUTPUT_H_

#define FILE_CREATED_SUCCESSFULY 0
#define FILE_COULD_NOT_BE_CREATED 1
#define FILE_SKIPPED_NO_RELEVANT_DATA 2


/*Creates and writes the given instruction memory content to the .ob file*/
int  writeObjFile(char *fileName, char **instructionMemory, char **dataMemory);

int writeExtFile(char *fileName);
int writeEntFile(char *fileName);


#endif
