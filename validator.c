#define _GNU_SOURCE /*For using strdup without compiler warnings*/
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include "assembler.h"
#include "registers.h"
#include "parser.h"
#include "symbols.h"
#include "commands.h"

#define DATA_NOT_AN_INTEGER "One of the data parameters is not an integer."
#define NO__GUIDANCE_STRING "There is no guidance string."
#define COMMAND_DOES_NOT_SUPPORT_OPERAND "The command does not support this operand."
#define ADDRESSING_MODE_NOT_ALLOWED "The addressing mode is not allowed."
#define NOT_A_VALID_COMMAND_NAME "The command name is not valid."
#define COMMAND_REQUIRES_OPERAND "The command requires an operand."
#define NOT_A_VALID_IMMEDIATE_ADDRESSING_MODE "The operand is not a valid immediate addressing mode."
#define NOT_A_VALID_DIRECT_ADDRESSING_MODE "The operand is not a valid direct addressing mode."
#define NOT_A_VALID_INDEX_ADDRESSING_MODE "The operand is not a valid index addressing mode."
#define NOT_A_VALID_INDEX2D_ADDRESSING_MODE "The operand is not a valid index 2d addressing mode."
#define NOT_A_VALID_REGISTER_ADDRESSING_MODE "The operand is not a valid register addressing mode."


int isValidSymbol(char *symbol);
int isValidIntegetList(char *list);
int isInteger(char *value);
int isValidCommandName(char *commandName);

char *validateExternGuidance(CommandParts parts);
char *validateEntryGuidance(CommandParts parts);
char *validateDataGuidance(CommandParts parts);
char *validateStringGuidance(CommandParts parts);
char *validateCommandOperands(CommandParts parts);

char *validateCommandOperand(char *operand, short allowedAddressingModes);
/*Validates the the operand string is a valid operand in the given addressing mode */
char *validateAddressingMode(int addressingMode, char *operand);
char *validateImmediateAddressingMode(char *operand);
char *validateDirectAddressingMode(char *operand);
char *validateIndexAddressingMode(char *operand);
char *validateIndex2dAddressingMode(char *operand);
char *validateRegisterAddressingMode(char *operand);


/* Validates a given assembly command line        */
/* returns NULL if the command line is valid      */
/* or an error message otherwise.                 */
char *validateAssemblyLine(char *line)
{
	CommandParts parts; /* Unfilled fields must be NULL in order for the function to work */
	char *validationResult = NULL;
	char *lineToValidate;
	StatementType type;

	lineToValidate = strdup(line); /*We work on a copy to keep the original line unchanged*/

	parts = parseAssemblyLine(lineToValidate);
	type = getStatementType(lineToValidate);

	switch (type){

		case EXTERNGUIDANCE:

			validationResult = validateExternGuidance(parts);
			break;

		case ENTRYGUIDANCE:

			validationResult = validateEntryGuidance(parts);
			break;

		case DATAGUIDANCE:

			validationResult = validateDataGuidance(parts);
			break;

		case  STRINGGUIDANCE:

			validationResult =  validateStringGuidance(parts);
			break;

		case COMMAND:

			if(!isValidCommandName(parts.command))
			{
				validationResult = NOT_A_VALID_COMMAND_NAME;
			}
			else /* A valid command name */
			{
				validationResult = validateCommandOperands(parts);
			}

			break;

		case COMMENT:
		default:
			break;


		}

	return validationResult;

}


/*Checks if the given string is a valid symbol */

int isValidSymbol(char *symbol)
{
	int i=1, length;

	length = strlen(symbol);

	if (isRegister(symbol)) return 0; /*symbol cannot be a register*/

	if (isValidCommandName(symbol)) return 0; /* symbol cannot be a command*/

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
				return FALSE;
			}
		}

	} else
	{
		return FALSE;

	}

	return TRUE;

}

int isValidIntegetList(char *list)
{
	char *currData;
	int isValid = TRUE;

	if(list == NULL) return 0;

	if ((currData = extractFirstGuidanceData(list)) != NULL)
	{
		if (isInteger(currData))
		{
			while((currData = extractNextGuidanceData()) != NULL)
			{
				if(!isInteger(currData)) isValid = FALSE;
			}
		}
		else
		{
			isValid = FALSE;
		}
	} else /*list is empty */
	{
		isValid = FALSE;
	}
	return isValid;
}

int isInteger(char *value)
{
	/*Skip leading spaces*/
	while(isspace(*value)) value++;

	while (*value !='\0' && *value != '\n' && *value != EOF)
	{
		if(isdigit(*value)|| *value == '+' || *value == '-')
		{
			value++;

		} else
		{
			return FALSE;

		}

	}
	return TRUE;
}


int isValidCommandName(char *commandName)
{
	Command command;

	command = getCommandByCommandName(commandName);

	return command.name != NULL;
}

char *validateExternGuidance(CommandParts parts)
{
	char *result = NULL;

	if(!isValidSymbol(parts.externSymbol))
	{
		result =  SYMBOL_NOT_VALID;

	}
	return result;

}
char *validateEntryGuidance(CommandParts parts)
{
	char *result = NULL;

	if(!isValidSymbol(parts.entrySymbol))
	{
		result = SYMBOL_NOT_VALID;

	}

	return result;

}

char *validateDataGuidance(CommandParts parts)
{
	char *result = NULL;

	if(!isValidIntegetList(parts.data))
	{
		result = DATA_NOT_AN_INTEGER;

	}

	return result;

}

char *validateStringGuidance(CommandParts parts)
{
	char *result = NULL;

	if(parts.data == NULL)
	{
		result = NO__GUIDANCE_STRING;
	}

	return result;
}

char *validateCommandOperands(CommandParts parts)
{
	char *result = NULL;

	Command command;
	command = getCommandByCommandName(parts.command);

	/*Validate the source operand*/


	result = validateCommandOperand(parts.sourceOperand, command.srcOperandAddressingModes);


	if(result == NULL) /*Validate the destination operand */
	{
		result = validateCommandOperand(parts.destinationOperand, command.dstOperandAddressingModes);
	}

	return result;
}


char *validateCommandOperand(char *operand, short allowedAddressingModes)
{
	char *result = NULL;
	int addressingMode;

	if(allowedAddressingModes == NO_OPERAND && operand != NULL) /*No addressing modes are allowed the the operand is not empty*/
	{
		result = COMMAND_DOES_NOT_SUPPORT_OPERAND;

	} else if(allowedAddressingModes != NO_OPERAND && operand == NULL) /*The command requires an operand and the operand is empty*/
	{
		result = COMMAND_REQUIRES_OPERAND;

	} else /*There are allowed addressing modes*/
	{
		if(operand != NULL)
		{
			addressingMode = parseAddressingMode(operand);

			result = validateAddressingMode(addressingMode, operand);

			if(result == NULL) /*Operand has a valid addressing mode form */
			{
				/*Check that the operand addressing mode is allowed for the given command */
				if(!(allowedAddressingModes & convertToAddressingModeFlag(addressingMode)))
				{
					result = ADDRESSING_MODE_NOT_ALLOWED;

				}

			}
		}

	}

	return result;
}

/*Validates the the operand string is a valid operand in the given addressing mode */

char *validateAddressingMode(int addressingMode, char *operand)
{
	char *result = NULL;

	switch(addressingMode)
	{
		case IMMEDIATE_ADDRESSING_MODE:
			result = validateImmediateAddressingMode(operand);
			break;

		case DIRECT_ADDRESSING_MODE:
			result = validateDirectAddressingMode(operand);
			break;

		case INDEX_ADDRESSING_MODE:
			result = validateIndexAddressingMode(operand);
			break;

		case INDEX2D_ADDRESSING_MODE:
			result = validateIndex2dAddressingMode(operand);
			break;

		case REGISTER_ADDRESSING_MODE:
			result = validateRegisterAddressingMode(operand);
			break;

		default:
			break;
	}

	return result;
}

char *validateImmediateAddressingMode(char *operand)
{

	char *result = NULL;

	if (!isInteger(extractImmediateAddressingModeValue(operand)))
	{
		result = NOT_A_VALID_IMMEDIATE_ADDRESSING_MODE;
	}

	return result;
}
char *validateDirectAddressingMode(char *operand)
{
    char *result = NULL;

	if (isValidSymbol(operand)== 0)
	{
		result = NOT_A_VALID_DIRECT_ADDRESSING_MODE;
	}

	return result;
}


char *validateIndexAddressingMode(char *operand)
{
  char *symbol, *offset, *result;

  result = NULL;

  symbol = extractIndexAddressingOffset(operand);
  offset = extractIndexAddressingOffset(operand);

  if (!isValidSymbol(symbol) || !isValidSymbol(offset))
  {
	  result = NOT_A_VALID_INDEX_ADDRESSING_MODE;
  }

  return result;

}
char *validateIndex2dAddressingMode(char *operand)
{
	char *symbol, *offset1, *offset2, *result;

	result = NULL;

	symbol = extractIndex2dAddressingSymbol(operand);
	offset1 = extractIndex2dAddressingOffset(operand);
	offset2 = extractIndex2dAddressingRegister(operand);


	if (!isValidSymbol(symbol) || !isValidSymbol(offset1) || !isRegister(offset2))
	{
		result = NOT_A_VALID_INDEX2D_ADDRESSING_MODE;
	}

	return result;

}
char *validateRegisterAddressingMode(char *operand)
{
	char *result = NULL;

	if (!isRegister(operand))
	{
		result = NOT_A_VALID_REGISTER_ADDRESSING_MODE;
	}

	return result;
}







