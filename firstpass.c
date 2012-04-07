#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "parser.h"
#include "assembler.h"
#include "symbols.h"
#include "validator.h"
#include "commands.h"
#include "registers.h"

char *getEmptyInstructionWord();

int executeFirstPass(FILE *assemblyFile)
{

	int lineNum = 1, val, addressingMode;
	int lineContainsSymbol = 0;
	char line[100], *instructionWord, *operandWord1, *operandWord2;
	char *validationMsg, *commandBase2Code;
	StatementType type;
	CommandParts parts;

	resetAssemblyCounters();

	while(fgets(line,MAX_LINE_LENGTH, assemblyFile) != NULL )
	{
		 type =  getStatementType(line);

		 if(type != EMPTY && type != COMMENT)
		 {
			/*First we validate the line */
			if((validationMsg = validateAssemblyLine(line)) == NULL)
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
							insertSymbol(parts.symbol, DATA, getDataCounter());
						}

						insertDataToMemory(line);


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
							insertSymbol(parts.symbol, CODE, getInstructionCounter());
						}

						/*Initialize the instruction words*/
						instructionWord = getEmptyInstructionWord();
						operandWord1 = NULL;
						operandWord2 = NULL;


						/*Get the command base2 code */
						commandBase2Code = getCommandBase2Code(parts.command);

						/*Copy the command bits to the instruction word*/
						strncpy(instructionWord, commandBase2Code, COMMAND_BITS_LENGTH);


						if(parts.sourceOperand != NULL)
						{
							addressingMode = parseAddressingMode(parts.sourceOperand);

							/*Copy the source operand addressing mode bits to the instruction word*/
							strncpy(instructionWord + SOURCE_ADDRESING_MODE_OFFSET,
									getAddressingModeBits(addressingMode) ,
									ADDRESSING_MODE_BITS_LENGTH);

							/*If it's a register addressing mode, copy the source register bits to the instruction word*/
							if(addressingMode == REGISTER_ADDRESSING_MODE)
							{
								strncpy(instructionWord + SOURCE_REGISTER_OFFSET,
										getRegisterBase2Code(parts.sourceOperand),
										REGISTER_BITS_LENGTH);

							/*If it's an immediate addressing mode, copy the value of the operand to the first operand word*/
							} else if(addressingMode == IMMEDIATE_ADDRESSING_MODE)
							{
								val = extractImmediateAddressingModeValue(parts.sourceOperand);
								operandWord1 = convertBase10toBase2(val);

							/*If it's a direct addressing mode, copy the value of the operand to the first operand word*/
							} else if(addressingMode == DIRECT_ADDRESSING_MODE)
							{
								operandWord1 = parts.sourceOperand;


							} else if(addressingMode == INDEX_ADDRESSING_MODE)
							{
								operandWord1 = extractIndexAddressingSybol(parts.sourceOperand);

							} else if(addressingMode == INDEX2D_ADDRESSING_MODE)
							{
								operandWord1 = NULL;

							}

						}

						if(parts.destinationOperand != NULL)
						{
							addressingMode = parseAddressingMode(parts.destinationOperand);

							/*Copy the destination operand addressing mode bits to the instruction word*/
							strncpy(instructionWord + DESTINATION_ADDRESING_MODE_OFFSET,
												getAddressingModeBits(addressingMode) ,
																ADDRESSING_MODE_BITS_LENGTH);

							/*If it's a register addressing mode, copy the destination register bits to the instruction word*/
							if(addressingMode == REGISTER_ADDRESSING_MODE)
							{
								strncpy(instructionWord + DESTINATION_REGISTER_OFFSET,
										getRegisterBase2Code(parts.destinationOperand),
										REGISTER_BITS_LENGTH);
							}

							/*If it's an immediate addressing mode, copy the value of the operand to the first operand word*/
							} else if(addressingMode == IMMEDIATE_ADDRESSING_MODE)
							{
								val = extractImmediateAddressingModeValue(parts.destinationOperand);
								operandWord1 = convertBase10toBase2(val);

							/*If it's a direct addressing mode, copy the value of the operand to the first operand word*/
							} else if(addressingMode == DIRECT_ADDRESSING_MODE)
							{
								operandWord1 = parts.destinationOperand;


							} else if(addressingMode == INDEX_ADDRESSING_MODE)
							{
								operandWord1 = extractIndexAddressingSybol(parts.destinationOperand);
								operandWord2 = extractIndexAddressingOffset(parts.destinationOperand);

							} else if(addressingMode == INDEX2D_ADDRESSING_MODE)
							{
								operandWord1 = extractIndex2dAddressingSybol(parts.destinationOperand);
								operandWord2 = extractIndex2dAddressingOffset(parts.destinationOperand);

							}
						}

						insertInstructionToMemory(instructionWord);

						/* Insert the additional words if needed*/
						if(operandWord1 != NULL)
						{
							insertInstructionToMemory(operandWord1);
						}

						if(operandWord2 != NULL)
						{
							insertInstructionToMemory(operandWord2);
						}

					} else /*A validation error */
					{
						addAssemblerError(validationMsg,lineNum);
						return 0;
					}

			}

		 	 lineNum++;
		 }

	return 1;
}

char *getEmptyInstructionWord()
{
	char *word;
	int i;

	word = (char *)malloc(WORD_SIZE+1);
	if(word == NULL) exit(1);

	for(i=0; i<WORD_SIZE; i++)
	{
		word[i] = '0';

	}
	word[WORD_SIZE] = '\0';

	return word;

}

