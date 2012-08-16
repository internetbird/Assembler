/******************************************************************************/
/* Assembler project for course 20465                                         */
/* Programmed by : Itay Parsiado (ID:062862495) & Shai Aharoni (ID:025332990) */
/******************************************************************************/

#define _GNU_SOURCE /*For using strdup without compiler warnings*/
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

#define NO_ASSEMBER_ARGUMENTS_ERROR "No assembly files supplied. Usage is: Assembler [file1] [file2] ...\n"
#define FILE_NAME_NOT_VALID "Assembly file:%s is not valid\n"
#define FIRST_PASS_FAILED "Executing first pass on file:%s failed.\n"
#define SECOND_PASS_FAILED "Executing second pass on file:%s failed.\n"
#define OB_FILE_CREATION_FAILED "Creating .ob file for assembly file:%s, failed\n"
#define ENT_FILE_CREATION_FAILED "Creating .ent file for assembly file:%s, failed\n"
#define EXT_FILE_CREATION_FAILED "Creating .ext file for assembly file:%s, failed\n"

#define ASSEMBLER_EXECUTION_FAILED 1 /*General failure exit code */

char *DataMemory[DATA_MEMORY_SIZE];
char *InstructionMemory[INSTRUCTION_MEMORY_SIZE+1]; /*Last char is for the linker*/

void insertIntToDataMemory(int value);

int main(int argc, char *argv[])
{
	FILE *file;
	char *assemblyFileName, *fileNameWithoutExtension;
	int i, fileCreateResult;

	if(argc < 2) fprintf(stderr, NO_ASSEMBER_ARGUMENTS_ERROR);

	for(i = 1; i<argc ; i++) /*Iterate over all the assembly files */
	{
		fileNameWithoutExtension = strdup(argv[i]); /*We need to keep the name for the output files*/
		assemblyFileName = (char *)malloc(strlen(fileNameWithoutExtension) + strlen(ASSEMBLY_FILE_EXT) + 1); /*Allocate memory for the full assembly file name*/

		if(assemblyFileName == NULL) /*Memory could not be allocated*/
		{
			exit(ASSEMBLER_EXECUTION_FAILED);
		}

		/*Build the full assembly file name*/
		assemblyFileName =  strdup(argv[i]);
		strcat(assemblyFileName, ASSEMBLY_FILE_EXT);

		if((file = fopen(assemblyFileName, "r")) != NULL)
		{
			if(!executeFirstPass(file))
			{
			   fprintf(stderr, FIRST_PASS_FAILED, assemblyFileName);
			   continue;
			}

			if(!executeSecondPass())
			{
				fprintf(stderr, SECOND_PASS_FAILED, assemblyFileName);
				continue;
			}

			fileCreateResult = writeObjFile(fileNameWithoutExtension, InstructionMemory, DataMemory);

			if(fileCreateResult == FILE_COULD_NOT_BE_CREATED)
			{
				fprintf(stderr, OB_FILE_CREATION_FAILED, assemblyFileName);
			}

			fileCreateResult = writeExtFile(fileNameWithoutExtension);

			if(fileCreateResult == FILE_COULD_NOT_BE_CREATED)
			{
				fprintf(stderr, EXT_FILE_CREATION_FAILED, assemblyFileName);
			}

			fileCreateResult = writeEntFile(fileNameWithoutExtension);

			if(fileCreateResult == FILE_COULD_NOT_BE_CREATED)
			{
				fprintf(stderr, ENT_FILE_CREATION_FAILED, assemblyFileName);
			}


		} else
		{
			fprintf(stderr, FILE_NAME_NOT_VALID, assemblyFileName);
			exit(ASSEMBLER_EXECUTION_FAILED);
		}

		free(assemblyFileName);
	}
	return 0;
}


/* Inserts a data or string to the data memory image. */
/* The function assumes that the line contains a data guidance command. */

void insertDataToMemory(char *line)
{
	StatementType type;
	char *data;
	int i;

	type = getStatementType(line);

	if(type == DATAGUIDANCE)
	{

		data = extractGuidanceData(line);
		data = extractFirstGuidanceData(data);

		while(data  != NULL)
		{
			insertIntToDataMemory(atoi(data));
			DC++;

			data = extractNextGuidanceData();
		}


	} else /*String guidance */
	{
		data = extractGuidanceString(line);

		for(i=0; i<strlen(data); i++)
		{
			insertIntToDataMemory(data[i]);
			DC++;
		}

		/* Add "zero" word */
		insertIntToDataMemory(0);
		DC++;

	}

}

void insertInstructionToMemory(char *word, char *linkerChar)
{

	if(linkerChar != NULL)
	{
		InstructionMemory[IC] = strcat(word, linkerChar);

	} else
	{
		InstructionMemory[IC] = word;
	}

	IC++;

}

/* Inserts an integer value to the data memory*/

void insertIntToDataMemory(int value)
{
	char *word;

	word = convertBase10toBase2(value);
	DataMemory[DC] = word;

}

void addAssemblerError(const char *errorMessage, int lineNumber)
{

	fprintf(stderr, "Error at line %d:%s\n",lineNumber, errorMessage );
}


unsigned int getInstructionCounter()
{
	return  IC;

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

void setInstructionMemoryWord(unsigned int index, char *value, char *linkerChar)
{
	InstructionMemory[index] = strcat(value, linkerChar);
}





