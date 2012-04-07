#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include "assembler.h"
#include "registers.h"
#include "parser.h"
#include "symbols.h"
#include "commands.h"

#define HASH_KEY '#'
#define HASH_STRING "#"
#define INDEX_ADDRESSING_STRING_1 "["
#define INDEX_ADDRESSING_STRING_2 "%"
#define INDEX_ADDRESSING_END_CHAR ']'
#define INDEX2D_ADDRESSING_STRING "[]"
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

	parts = parseAssemblyLine(line);
	StatementType type = getStatementType(line);

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
				return 0;
			}
		}

	} else
	{
		return 0;

	}

	return 1;

}

int isValidIntegetList(char *list)
{
	char *currData;
	int isValid = 1;

	if(list == NULL) return 0;

	if ((currData = extractFirstGuidanceData(list)) != NULL)
	{
		if (isInteger(currData))
		{
			while((currData = extractNextGuidanceData()) != NULL)
			{
				if(!isInteger(currData)) isValid = 0;
			}
		}
		else
		{
			isValid = 0;
		}
	} else /*list is empty */
	{
		isValid = 0;
	}
	return isValid;
}

int isInteger(char *value)
{
	while (*value !='\0')
	{
		if(isdigit(*value)|| *value == '+' || *value == '-') /*spaces have already been removed*/
		{
			value++;

		} else
		{
			return 0;

		}

	}
	return 1;
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

	if(!isValidSymbol(parts.symbol))
	{
		result = SYMBOL_NOT_VALID;

	}

	return result;

}

char *validateDataGuidance(CommandParts parts)
{
	char *result = NULL;

	if(!isValidIntegetList(extractGuidanceData(parts.data)))
	{
		result = DATA_NOT_AN_INTEGER;

	}

	return result;

}

char *validateStringGuidance(CommandParts parts)
{
	char *result = NULL;

	if(extractGuidanceString(parts.data) == NULL)
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

	} else /*There are allowed addressing modes and the operand is not empty */
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
	char *tmp = strdup(operand);
	char *result = NULL;

	if ((strchr(tmp,HASH_KEY))!= NULL)
	{
		strtok(tmp,HASH_STRING);
		if (isInteger(tmp) == 0)
		result = NOT_A_VALID_IMMEDIATE_ADDRESSING_MODE;
	}
	else
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
  char *result = NULL;
  char *temp = strdup(operand), *temp2;

  /*Extract the first symbol */
  strtok(temp, INDEX_ADDRESSING_STRING_1);

  if (isValidSymbol(temp))
  {
	/*Extract and validate the second symbol */
	temp = strtok(NULL,INDEX_ADDRESSING_STRING_2);

	temp2 = strchr(temp, INDEX_ADDRESSING_END_CHAR);

	if(temp2 == NULL) /*No end char */
	{
		result = NOT_A_VALID_INDEX_ADDRESSING_MODE;
	}

	else
	{
	    *temp2 = '\0'; /*Replace the ']' with '\0' to terminate the second symbol string */

		if (temp != NULL && isValidSymbol(temp)==0)
		{
			result = NOT_A_VALID_INDEX_ADDRESSING_MODE;
		}
	}
  } else
  {
	  result = NOT_A_VALID_INDEX_ADDRESSING_MODE;
  }

  return result;

}
char *validateIndex2dAddressingMode(char *operand)
{
	char *result = NULL;
	char *temp = strdup(operand);

	strtok(temp,INDEX2D_ADDRESSING_STRING);

	if (temp != NULL && isValidSymbol(temp)!=0)
	{
		if(strtok(NULL,INDEX2D_ADDRESSING_STRING) !=NULL && isValidSymbol(temp) !=0)
		{
			if(strtok(NULL,INDEX2D_ADDRESSING_STRING) == NULL || isRegister(temp)==0)
			{
				result = NOT_A_VALID_INDEX2D_ADDRESSING_MODE;
			}

		} else result = NOT_A_VALID_INDEX2D_ADDRESSING_MODE;

	}
	else result = NOT_A_VALID_INDEX2D_ADDRESSING_MODE;

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






