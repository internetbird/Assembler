#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"
#include "firstpass.h"
#include "secondpass.h"
#include "assembler.h"

char *DataMemory[DATA_MEMORY_SIZE];
char *InstructionMemory[INSTRUCTION_MEMORY_SIZE];

unsigned int dataMemoryPosition;
unsigned int instructionMemoryPosition;

void insertIntToDataMemory(int value);

int main(int argc, char *argv[])
{
	FILE *file;

	IC = IC_STARTUP_VALUE;
	dataMemoryPosition = 0;
	instructionMemoryPosition = 0;

	if((file = fopen(argv[1], "r")) != NULL)
	{
		if(executeFirstPass(file) != 0)
		{
		   printf("Terminating assembler execution...\n");
		   return 1;
		}
	}
	return 0;
}


/* Inserts a data or string to the data memory image. */
/* The function assumes that the line contains a data guidance command. */
/* Returns the number of inserted words. */

int insertDataToMemory(char *line)
{
	StatementType type;
	char *data;
	int insertedWords = 0, i;

	type = getStatementType(line);

	if(type == DATAGUIDANCE)
	{

		data = extractGuidanceData(line);
		data = extractFirstGuidanceData(data);

		while(data  != NULL)
		{
			insertIntToDataMemory(atoi(data));
			insertedWords++;

			data = extractNextGuidanceData();
		}


	} else /*String guidance */
	{
		data = extractGuidanceString(line);

		for(i=0; i<strlen(data); i++)
		{
			insertIntToDataMemory(data[i]);
			insertedWords++;
		}

		/* Add "zero" word */
		insertIntToDataMemory(0);
		insertedWords++;


	}

	return insertedWords;

}

void insertInstructionToMemory(char *word)
{
	InstructionMemory[instructionMemoryPosition] = word;
	instructionMemoryPosition ++;

}

/* Inserts an integer value to the data memory*/

void insertIntToDataMemory(int value)
{
	char *word;

	word = convertBase10toBase2(value);
	DataMemory[dataMemoryPosition] = word;

	dataMemoryPosition++;

}

void addAssemblerError(const char *errorMessage, int lineNumber)
{

	fprintf(stderr, "Error at line %d:%s",lineNumber, errorMessage );

}



