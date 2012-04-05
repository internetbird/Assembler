#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "parser.h"
#include "assembler.h"
#include "symbols.h"
#include "validator.h"
#include "commands.h"
#include "registers.h"
#include "secondpass.h"
#include "firstpass.h"

char * word;

/*Executes the second pass - replacing all the sybols with their correct values */
int executeSecondPass()
{

	int i = 0, noErrorsFound = 1;
	unsigned int instructionMemoryLength;
	char *currInstructionWord ;
	Symbol *currSymbol;

	instructionMemoryLength = getInstructionCounterOffset();

	while(i < instructionMemoryLength && noErrorsFound)
	{

		currInstructionWord = getInstructionMemoryWord(i);

		/*If it's a symbol that needs to be replaced with the actual symbol value*/
		if (isalpha(currInstructionWord[0]))
		{
			if(isSymbolExists(currInstructionWord))
			{
				currSymbol = getSymbol(currInstructionWord);
				setInstructionMemoryWord(i, convertBase10toBase2(currSymbol->value));

			} else /*Report symbol not found error */
			{

				addAssemblerError(SYMBOL_NOT_VALID, i);
				noErrorsFound = 0;

			}
		}
		else if (currInstructionWord[0] == OFFSET_MARK_LABEL)
		{
			word = strtok(currInstructionWord, OFFSET_MARK_STRING);
			if(isSymbolExists(word))
			{
				currSymbol = getSymbol(word);
				setInstructionMemoryWord(i, convertBase10toBase2(currSymbol->value-(i+1)*IC_STARTUP_VALUE));
			} else
			{
				addAssemblerError(SYMBOL_NOT_VALID, i);
				noErrorsFound = 0;

			}
		}

		i++;
	}

	return noErrorsFound;
}


