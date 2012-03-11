/**************************************************************/
/* parser.c                                                   */
/* this file contains assembler parsing utility  functions    */
/**************************************************************/
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "parser.h"
#include "symbols.h"
#include "registers.h"
#include "commands.h"
#include "assembler.h"


/* Extracts a symbol name from the given line     */
/* The function assumes that the line starts with */
/* a valid symbol.                                */
char *extractSymbolName(char *line);
char *extractExternSymbolName(char *line);
char *extractCommandName(char *line);
char *extractSourceOperand(char *line);
char *extractDestinationOperand(char *line);
CommandParts getEmptyCommandParts();
int isLineContainsSymbol(char *line);
char *getSubstringAfterSymbol(char *line);
char *getSubstringAfterCommand(char *line);
char *convertTo2sComplement(char base2Positive[]);
int sourceOperandExists(char *line);


/* Returns the type of assembly statement         */
/* that the given line corresponds to.            */

StatementType getStatementType(char *line)
{

	char *test = NULL;

	if(sscanf(line, "%s", test) == 0) /* No non white characters */
	{
		return EMPTY;

	} else if(*line == ';')
	{
		return COMMENT;

	} else if(strstr(line, DATA_GUIDANCE) != NULL)
	{
		return DATAGUIDANCE;

	} else if(strstr(line, STRING_GUIDANCE) != NULL){

		return STRINGGUIDANCE;
	}
	else if(strstr(line, ENTRY_GUIDANCE) != NULL){

		return ENTRYGUIDANCE;
	}
	else if(strstr(line, EXTERN_GUIDANCE) != NULL)
	{
		return EXTERNGUIDANCE;
	}

	return COMMAND;
}

/* This function takes a single assembly command line and separates it to its parts */

CommandParts parseAssemblyLine(char *line)
{
	CommandParts parts;
	StatementType type;

	parts = getEmptyCommandParts();
	type = getStatementType(line);

	/*Set the symbol part, if it exists */
	if(isLineContainsSymbol(line))
	{
		parts.symbol = extractSymbolName(line);
		line = getSubstringAfterSymbol(line);

	}

	/*Set all the parts for the given statement type */

	switch(type)
	{
		case COMMAND:
			parts.command = extractCommandName(line);
			line = getSubstringAfterCommand(line);
			if(line != NULL) /*There are operands*/
				{
					if(sourceOperandExists(line))
					{
						parts.sourceOperand = extractSourceOperand(line);
					}

					parts.destinationOperand = extractDestinationOperand(line);

				}

			break;

		case EXTERNGUIDANCE:
			parts.externSymbol = extractExternSymbolName(line);
			break;

		case DATAGUIDANCE:
			parts.data = extractGuidanceData(line);
			break;

		case STRINGGUIDANCE:
			parts.data = extractGuidanceString(line);
			break;
		default:
			break;

	}



}

CommandParts getEmptyCommandParts()
{
	CommandParts parts;

	parts.symbol = NULL;
	parts.command = NULL;
	parts.sourceOperand = NULL;
	parts.destinationOperand = NULL;
	parts.externSymbol = NULL;
	parts.data = NULL;

	return parts;

}


/*Returns a substring of the line without the symbol part */
/*The function assumes that the line has a symbol */

char *getSubstringAfterSymbol(char *line)
{
	return strchr(line, SYMBOL_END_CHAR) + 1;


}

/*Returns a substring of the line without the command part */
/*The function assumes that the line has a command */

char *getSubstringAfterCommand(char *line)
{
	int i=0;

	/*Trim the starting spaces */
	while(isspace(line[i])) i++;

	/*If the line ends after the command, return NULL */
	if(line[i] == '\n' || line[i] == EOF) return NULL;

	/* Skip the command part */
	while(isalpha(line[i]) || line[i] == '.') i++;


	return line+i;


}



/* Extracts a symbol name from the given line     */
/* The function assumes that the line starts with */
/* a valid symbol.                                */

char *extractSymbolName(char *line)
{
	int i = 0;
	char *symbolName;

	while(line[i] != SYMBOL_END_CHAR)
	{
			i++;
	}


	strncpy(symbolName, line, i);
	symbolName[i] = '\0';

	return symbolName;
}

/* Extracts the data numbers part from the given command line */
/* the function assumes that the line is a valid data guidance line */

char *extractGuidanceData(char *line)
{
	char *guidanceData;

	guidanceData = strstr(line, DATA_GUIDANCE);

	/* Skip all characters until we reach a number */
	while((*guidanceData) != '+' &&
			(*guidanceData) != '-' &&
			!isdigit(*guidanceData))
	{
		guidanceData++;
	}

	return guidanceData;

}

/*Extracts the first number string in the given data string */
char *extractFirstGuidanceData(char *data)
{
	return strtok(data, ',');
}

/*Extracts the next number string in the given data string. */
/*Returns NULL if there are no more values to return */
char *extractNextGuidanceData()
{
	return strtok(NULL, ',');

}

char *extractGuidanceString(char *line)
{
	char *guidanceString, *stringStart, *stringEnd;

	stringStart = strchr(line, '"');
	stringEnd = strrchr(line, '"');

	strncpy(guidanceString, stringStart+1, stringEnd - stringStart);

	/*Add 0 to the end of the string */
	*(guidanceString + stringEnd - stringStart) = 0;

	return guidanceString;
}

/*Extracts the source operand string from a line that contains two operands */
/*Assumes the given line contains two operand */

char *extractSourceOperand(char *line)
{
	char *operand, *operandEnd;
	int i=0;

	/*Skip white spaces*/
	while (isspace(line[i])) i++;

	/*Find the end of the source operand*/
	operandEnd =  strchr(line, ',');

	strncpy(operand, line+i, operandEnd - line - i);

	/*Set the end of the operand string */
	operand[operandEnd - line - i] = '\0';


	return operand;

}

/*Extracts the destination operand from a line that contains one or two operands */

char *extractDestinationOperand(char *line)
{
	char *operand;
	char *startOperand;

	if((startOperand = strchr(line, ',')) != NULL) /*If there are 2 operands, extract the second*/
	{

		sscanf(startOperand+1, "%s", operand);
	}
	 else /*There is only one operand, extract it. */
	{
		sscanf(line, "%s", operand);

	}


	return operand;
}


/*Extracts the command name from the given line*/
/*The function assumes that the command is the first word of the line*/

char *extractCommandName(char *line)
{
	char *commandName;

	sscanf(line, "%s", commandName);

	return commandName;


}

/* Checks if the line contains a symbol      */

int isLineContainsSymbol(char *line)
{
	return strchr(line, SYMBOL_END_CHAR) != NULL;


}

/* Checks if the line contains a symbol      */

/*

int isLineContainsSymbol(char *line)
{
	int i=1, length;

	length = strlen(line);

	if(isalpha(line[0]))
	{
		while(i < MAX_SYMBOL_NAME_SIZE &&
			  line[i] != SYMBOL_END_CHAR)
		{
			if(isalpha(line[i]) || isdigit(line[i]))
			{
				i++;

			} else
			{
				return 0;
			}
		}

		if(i < MAX_SYMBOL_NAME_SIZE)
		{
			return 1;

		} else
		{
			return 0;
		}

	} else
	{
		return 0;

	}

}

*/


/* Validates a given assembly command line        */
/* returns NULL if the command line is valid      */
/* or an error message otherwise.                 */

char *validateCommandLine(char *line)
{
	return NULL;

}

/* Returns the base2 value of the addressing mode for the given operand*/

const char *parseAddressingMode(char *operand)
{
	if(isRegister(operand))
	{
		return REGISTER_ADDRESSING_MODE;

	} else if(operand[0] == '#')
	{
		return IMMEDIATE_ADDRESSING_MODE;

	} else if(strstr(operand, "[%") != NULL)
	{
		return INDEX_ADDRESSING_MODE;

	} else if(strstr(operand,"[" ) != NULL)
	{
		return INDEX2D_ADDRESSING_MODE;
	}

	return DIRECT_ADDRESSING_MODE;
}

const char *parseOperand(char *operand)
{
	if(isRegister(operand))
	{
		return getRegisterBase2Code(operand);
	}

	else return NOT_REGISTER_OPERAND;

}
const char *parseCommand(char *command)
{
	return getCommandBase2Code(command);

}




char *convertBase10toBase2(int base10)
{
	char base2Word[WORD_SIZE], temp;
	int i, isNegative;

	if(base10 < 0)
	{
		isNegative = 1;
		base10 = -base10;

	}

	/* Calculate the positive value base 2 value */

	for(i=0; i<WORD_SIZE; i++)
	{
		if(i%2 == 0)
		{
			base2Word[i] = '0';
		} else
		{
		    base2Word[i] = '1';
		}

		i = i/2;
	}

	if(isNegative)
	{
		base2Word = convertTo2sComplement(base2Word);
	}

	/*Reverse the string (MSB is first)*/
	for(i=0; i<(WORD_SIZE/2); i++)
	{
		temp = base2Word[i];
		base2Word[i] = base2Word[WORD_SIZE-i-1];
		base2Word[WORD_SIZE-i-1] = temp;

	}

	return base2Word;
}

/* Converts a positive base 2 value to its 2's complement negative value*/

char *convertTo2sComplement(char base2Positive[])
{
	char base2Negative[WORD_SIZE];
	int i;

	/*First switch the bits sign */
	for(i=0; i<WORD_SIZE; i++)
	{
		if(base2Positive[i] == '1')
		{
			base2Negative[i] = '0';

		} else
		{
			base2Negative[i] = '1';

		}
	}

	/* Then, add 1 to the result */
	for(i=0; i<WORD_SIZE; i++)
	{
		if(base2Negative[i] == '1')
		{
			base2Negative[i] == '0';

		} else /* equals '0' */
		{
			base2Negative[i] == '1';
			break;

		}
	}

	return base2Negative;

}

/*Extracts the ".extern" symbol name*/
/*The function assumes the given line is an extern guidance statement*/

char *extractExternSymbolName(char *line)
{
	char *symbol ,*temp;

	temp = strstr(line, EXTERN_GUIDANCE);
	temp = (temp + strlen(EXTERN_GUIDANCE));

	sscanf(temp , "%s", symbol);

	return symbol;

}

int sourceOperandExists(char *line)
{

}







