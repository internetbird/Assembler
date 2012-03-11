#include <stdio.h>
#include <string.h>
#include "parser.h"
#include "assembler.h"
#include "symbols.h"
#include "validator.h"
#include "commands.h"

int executeFirstPass(FILE *assemblyFile)
{

	int lineNum = 1, numOfInsertedWords;
	int lineContainsSymbol = 0;
	char line[100], instructionWord[WORD_SIZE];
	char *validationMsg, *commandBase2Code, *operand;
	StatementType type;
	CommandParts parts;

	IC = 0;
	DC = 0;

	while(fgets(line,100, assemblyFile) != NULL )
	{
		 type =  getStatementType(line);

		 if(type != EMPTY && type != COMMENT)
		 {

			/*First we validate the line */
			if((validationMsg = validateCommandLine(line)) == NULL)
			{
				    /*Separate the line to its parts*/
				    parts = parseAssemblyLine(line);

					/* Set a flag that determines if there is a symbol in the currently processed line*/
					lineContainsSymbol = (parts.symbol != NULL);

					/*If it's a data guidance command, insert the data to the data memory and symbol(if exists) */
					if(type == DATAGUIDANCE || type == STRINGGUIDANCE)
					{
						if(lineContainsSymbol)
						{
							insertSymbol(parts.symbol, DATA, DC);
						}

						numOfInsertedWords = insertDataToMemory(line);
						DC += numOfInsertedWords; /* Update the Data Counter */

					/*If it's an .extern symbol guidance line insert it to the external symbols list. */
					} else if(type == ENTRYGUIDANCE || type == EXTERNGUIDANCE)
					{
						if(type == EXTERNGUIDANCE)
						{
							insertExternSymbol(parts.externSymbol);
						}


					/* It's a command. Insert a command symbol(if exists) and the command to the instruction memory */
					} else
					{
						if(lineContainsSymbol)
						{
							insertSymbol(parts.symbol, CODE, IC);
						}

						/*Get the command base2 code */
						commandBase2Code = getCommandBase2Code(parts.command);

						/*Copy the command bits to the instruction word*/
						strncpy(instructionWord, commandBase2Code, COMMAND_BITS_LENGTH);

						/*Copy the source operand addressing mode bits to the instruction word*/
						operand = extractSourceOperand(line);

						/*Copy the source register bits to the instruction word*/

						/*Copy the destination operand addressing mode bits to the instruction word*/
						operand = extractDestinationOperand(line);

						/*Copy the destination register bits to the instruction word*/


						insertInstrunctionToMemory(instructionWord);

					}

					lineNum++;


			} else /*A validation error */
			{
				addAssemblerError(validationMsg,lineNum);
				return 1;
			}
		 }
	}

	return 0;
}
