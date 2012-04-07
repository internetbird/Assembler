#include <stdio.h>
#include "output.h"
#include "assembler.h"

int  writeObjFile(char *fileName, char **instructionMemory)
{
	int length, i;

	length = getInstructionCounter();

	for(i=0; i<length ; i++)
	{
		printf("memory line %d:%s", i+1, *(instructionMemory+i));

	}

	return 1;

}
int writeExtFile(char *fileName, char **dataMemory)
{
	return 1;

}
int writeEntFile(char *fileName, char **dataMemory)
{
	return 1;
}

