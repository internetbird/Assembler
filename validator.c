#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "assembler.h"
#include "parser.h"

int isValidSymbol(char *symbol);

/* Validates a given assembly command line        */
/* returns NULL if the command line is valid      */
/* or an error message otherwise.                 */
char *validateCommandLine(char *line)
{

	CommandParts parts;
	parts = parseAssemblyLine(line);

	if(parts.symbol != NULL && !isValidSymbol(parts.symbol))
	{
		return SYMBOL_NOT_VALID;
	}

	return NULL;

}



int isValidSymbol(char *symbol)
{
	int i=1, length;

	length = strlen(symbol);

	if(length > MAX_SYMBOL_LENGTH) return 0;

	if(isalpha(symbol[0]))
	{
		while(i < length)
		{
			if(isalpha(symbol[i]) || isdigit(symbol[i]))
			{
				i++;

			} else
			{
				return 0;
			}
		}

	} else
	{
		return 0;

	}

	return 1;

}

