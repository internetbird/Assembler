#include "output.h"
#include "symbols.h"
#include "assembler.h"
#include "parser.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define EIGHT_BIT_OFFSET 8
#define FOUR_BIT_OFFSET 12


/*Creates and writes the given instruction memory content to the .ob file*/
int writeObjFile(char *filename, char **instructionMemory, char **dataMemory)
{
	char *fname ;
	FILE *objfile;
	int i,instructionLength, dataLength;

	fname = strdup(filename);
	strcat(fname, ".ob");
	objfile = fopen(fname, "w");

	instructionLength = getInstructionCounter();
	dataLength = getDataCounter();


	printf("Printing obj file...\n");

	if (objfile !=NULL)
	{
		/*Write the instruction and data length header*/

		fprintf(objfile, "%s %s\n", convertBase10toBase2(instructionLength) + EIGHT_BIT_OFFSET,convertBase10toBase2(dataLength) + FOUR_BIT_OFFSET);
		printf("%s %s\n", convertBase10toBase2(instructionLength) + EIGHT_BIT_OFFSET,convertBase10toBase2(dataLength) + FOUR_BIT_OFFSET);

		/*Write the instruction section*/
		for(i=0; i<instructionLength; i++)
		{
			fprintf(objfile, "%s %.16s", convertBase10toBase2(i+IC_STARTUP_VALUE) + EIGHT_BIT_OFFSET, instructionMemory[i]);
			printf("%s %.16s", convertBase10toBase2(i+IC_STARTUP_VALUE) + EIGHT_BIT_OFFSET, instructionMemory[i]);

			fprintf(objfile, " %c\n", instructionMemory[i][WORD_SIZE]);
			printf(" %c\n", instructionMemory[i][WORD_SIZE]);

		}

		/*Write the data section*/
		for(i=0; i<dataLength; i++)
		{
		    fprintf(objfile, "%s %s\n", convertBase10toBase2(i+instructionLength+ IC_STARTUP_VALUE) + EIGHT_BIT_OFFSET , dataMemory[i]);
		    printf("%s %s\n", convertBase10toBase2(i+instructionLength+ IC_STARTUP_VALUE) + EIGHT_BIT_OFFSET, dataMemory[i]);

		}

		fclose (objfile);

		return FILE_CREATED_SUCCESSFULY;
	}

	return FILE_COULD_NOT_BE_CREATED;
}


int writeExtFile(char *filename)
{
	char *fname;
	FILE *extfile ;
	SymbolNode *current;
	SymbolTable extSymbolTable;

	fname = strdup(filename);
	strcat(fname, ".ext");
	extfile = fopen(fname, "w");
	extSymbolTable = getExternSymbolTable();
	current = extSymbolTable.first;

	if (extfile != NULL)
	{
		while (current != NULL)
		{
			fprintf(extfile, "%s %s\n", current->symbol.name , convertBase10toBase2(current->symbol.value) + EIGHT_BIT_OFFSET);
			current = current->next;
		}

		fclose(extfile);

		return FILE_CREATED_SUCCESSFULY;
	}
	return FILE_COULD_NOT_BE_CREATED;
}


int writeEntFile(char *filename)
{
	char *fname;
	SymbolNameList entNameList;
	SymbolNameNode *current;
	Symbol *currentSymbol;
	FILE *entfile;

	fname = strdup(filename);
	strcat(fname, ".ent");
	entfile = fopen(fname, "w");
	entNameList = getEntrySymbolList();
	current = entNameList.first;

	if (entfile != NULL)
	{
		while (current != NULL)
		{
			currentSymbol = getSymbol(current->name);

			if(currentSymbol != NULL)
			{
				fprintf(entfile, "%s %s\n", current->name, convertBase10toBase2(currentSymbol->value)+ EIGHT_BIT_OFFSET);
				current = current->next;

			} else /*The entry symbol does not exist in the symbol table*/
			{
				return FILE_COULD_NOT_BE_CREATED;
			}
		}

		fclose(entfile);

		return FILE_CREATED_SUCCESSFULY;
	}

	return FILE_COULD_NOT_BE_CREATED;
}


