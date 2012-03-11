#include <ctype.h>
#include "assembler.h"
#include "parser.h"

bool isValidSymbol(char *symbol);

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



bool isValidSymbol(char *symbol)
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

}

