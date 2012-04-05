#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"
#include "firstpass.h"
#include "secondpass.h"
#include "assembler.h"
#include "output.h"

unsigned int IC; /* The instruction counter */
unsigned int DC; /* The data counter */

#define NO_ASSEMBER_ARGUMENTS_ERROR "No assembly files supplied. Usage is: Assembler [file1] [file2] ..."
#define FILE_NAME_NOT_VALID "Assembly file:%s is not valid"
#define FIRST_PASS_FAILED "Executing first pass on file:%s failed. Terminating assembler execution."
#define SECOND_PASS_FAILED "Executing second pass on file:%s failed. Terminating assembler execution."

char *DataMemory[DATA_MEMORY_SIZE];
char *InstructionMemory[INSTRUCTION_MEMORY_SIZE];

void insertIntToDataMemory(int value);

int main(int argc, char *argv[])
{
	FILE *file;
	char *assemblyFileName;
	int i;

	if(argc < 2) fprintf(stderr, NO_ASSEMBER_ARGUMENTS_ERROR);

	for(i = 1; i<argc ; i++) /*Iterate over all the assembly files */
	{
		assemblyFileName = strcat(argv[i], ASSEMBLY_FILE_EXT);

		if((file = fopen(assemblyFileName, "r")) != NULL)
		{
			if(!executeFirstPass(file))
			{
			   printf(FIRST_PASS_FAILED, assemblyFileName);
			   exit(1);
			}

			if(!executeSecondPass())
			{
				printf(SECOND_PASS_FAILED, assemblyFileName);
				exit(1);
			}

			writeObjFile(argv[i], InstructionMemory);
			writeExtFile(argv[i], DataMemory);
			writeEntFile(argv[i], DataMemory);


		} else
		{
			fprintf(stderr, FILE_NAME_NOT_VALID, assemblyFileName);
			exit(1);
		}
	}
	return 0;
}


/* Inserts a data or string to the data memory image. */
/* The function assumes that the line contains a data guidance command. */


void insertDataToMemory(char *line)
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

	DC = DC + insertedWords;

}

void insertInstructionToMemory(char *word)
{
	InstructionMemory[IC] = word;
	IC++;

}

/* Inserts an integer value to the data memory*/

void insertIntToDataMemory(int value)
{
	char *word;

	word = convertBase10toBase2(value);
	DataMemory[DC] = word;

	DC++;

}

void addAssemblerError(const char *errorMessage, int lineNumber)
{

	fprintf(stderr, "Error at line %d:%s",lineNumber, errorMessage );

}

/*Returns the absolute value of the instruction counter */
unsigned int getInstructionCounter()
{
	return  IC + IC_STARTUP_VALUE;

}

/*Returns the value of the instruction counter relative to the memory beginning address*/
unsigned int getInstructionCounterOffset()
{
	return IC;
}


unsigned int getDataCounter()
{
	return DC;

}

void resetAssemblyCounters()
{
	IC = 0;
	DC = 0;

}

char *getInstructionMemoryWord(unsigned int index)
{
	return InstructionMemory[index];

}
void setInstructionMemoryWord(unsigned int index, char *value)
{
	InstructionMemory[index] = value;

}




