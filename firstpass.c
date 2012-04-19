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
	int lineContainsSymbol = FALSE, success = TRUE;
	char line[100], *instructionWord;
	char *srcOperandWord1, *srcOperandWord2; /*Two additional words for the source operand to store symbols (if they exists) */
	char *dstOperandWord1, *dstOperandWord2; /*Two additional words for the destination operand to store symbols (if they exists) */
	int srcOperand2IsOffset, dstOperand2IsOffset;
	char *validationMsg, *commandBase2Code;
	char *srcOperand1LinkerChar, *dstOperand1LinkerChar; /*The additional word linker char(a/e/r) */
	char *temp, *insertionResult;
	StatementType type;
	CommandParts parts;

	resetAssemblyCounters();
	initSymboTables();

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
							insertionResult = insertSymbol(parts.symbol, DATA, getDataCounter());

							if(insertionResult != NULL) /*An error occurred*/
							{
								addAssemblerError(insertionResult,lineNum);
								success = FALSE;

							}
						}

						insertDataToMemory(line);

					/*If it's an .extern symbol guidance line insert it to the external symbols list. */
					} else if(type == EXTERNGUIDANCE)
					{
						insertionResult = insertExternSymbolName(parts.externSymbol);

						if(insertionResult != NULL) /*An error occurred*/
						{
							addAssemblerError(insertionResult,lineNum);
							success = FALSE;

						}


					} else if(type == ENTRYGUIDANCE)
					{
						insertionResult = insertEntrySymbol(parts.entrySymbol);

						if(insertionResult != NULL) /*An error occurred*/
						{
							addAssemblerError(insertionResult,lineNum);
							success = FALSE;

						}


					}
					/* It's a command. Insert a command symbol(if exists) and the command to the instruction memory */
					else
					{
						if(lineContainsSymbol)
						{
							insertionResult = insertSymbol(parts.symbol, CODE, getInstructionCounter());

							if(insertionResult != NULL) /*An error occurred*/
							{
								addAssemblerError(insertionResult,lineNum);
								success = FALSE;

							}

						}

						/*Reset the instruction words*/
						instructionWord = getEmptyInstructionWord();
						srcOperandWord1 = NULL;
						srcOperandWord2 = NULL;
						dstOperandWord1 = NULL;
						dstOperandWord2 = NULL;
						srcOperand1LinkerChar = NULL;
						dstOperand1LinkerChar = NULL;
						temp = NULL;
						srcOperand2IsOffset = FALSE;
						dstOperand2IsOffset = FALSE;

						/*Get the command base2 code */
						commandBase2Code = getCommandBase2Code(parts.command);

						/*Copy the command bits to the instruction word*/
						strncpy(instructionWord, commandBase2Code, COMMAND_BITS_LENGTH);

						/*Create the memory words for the source operand*/
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
								val = atoi(extractImmediateAddressingModeValue(parts.sourceOperand));
								srcOperandWord1 = convertBase10toBase2(val);
								srcOperand1LinkerChar = LINKER_ABSOLUTE;

							/*If it's a direct addressing mode, copy the value of the operand to the first operand word*/
							} else if(addressingMode == DIRECT_ADDRESSING_MODE)
							{
								srcOperandWord1 = parts.sourceOperand;


							} else if(addressingMode == INDEX_ADDRESSING_MODE)
							{
								srcOperandWord1 = extractIndexAddressingSymbol(parts.sourceOperand);
								srcOperandWord2 = extractIndexAddressingOffset(parts.sourceOperand);
								srcOperand2IsOffset = TRUE;

							} else if(addressingMode == INDEX2D_ADDRESSING_MODE)
							{
								srcOperandWord1 = extractIndex2dAddressingSymbol(parts.sourceOperand);
								srcOperandWord2 = extractIndex2dAddressingOffset(parts.sourceOperand);

							}

						}

						/*Create the memory words for the destination operand*/
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
							else if(addressingMode == IMMEDIATE_ADDRESSING_MODE)
							{
								val = atoi(extractImmediateAddressingModeValue(parts.destinationOperand));
								dstOperandWord1 = convertBase10toBase2(val);
								dstOperand1LinkerChar = LINKER_ABSOLUTE;

							/*If it's a direct addressing mode, copy the value of the operand to the first operand word*/
							} else if(addressingMode == DIRECT_ADDRESSING_MODE)
							{
								dstOperandWord1 = parts.destinationOperand;


							} else if(addressingMode == INDEX_ADDRESSING_MODE)
							{
								dstOperandWord1 = extractIndexAddressingSymbol(parts.destinationOperand);
								dstOperandWord2 = extractIndexAddressingOffset(parts.destinationOperand);
								dstOperand2IsOffset = TRUE;

							} else if(addressingMode == INDEX2D_ADDRESSING_MODE)
							{
								dstOperandWord1 = extractIndex2dAddressingSymbol(parts.destinationOperand);
								dstOperandWord2 = extractIndex2dAddressingOffset(parts.destinationOperand);

							}
						}

						insertInstructionToMemory(instructionWord, LINKER_ABSOLUTE);

						/* Insert the additional words if needed*/
						if(srcOperandWord1 != NULL)
						{
							insertInstructionToMemory(srcOperandWord1, srcOperand1LinkerChar);
						}

						if(srcOperandWord2 != NULL)
						{
							if(srcOperand2IsOffset) /*Add the symbol offset mark for the second pass*/
							{
								temp = (char*)malloc(2);
								if(temp == NULL) exit(1);

								strcpy(temp,OFFSET_MARK_STRING); /*This mark is used to signal the second pass that the symbol is and offset*/
								srcOperandWord2 = strcat(temp, srcOperandWord2);
							}

							insertInstructionToMemory(srcOperandWord2, NULL);
						}

						/*Add additional words for the destination operand*/
						if(dstOperandWord1 != NULL)
						{
							insertInstructionToMemory(dstOperandWord1, dstOperand1LinkerChar);
						}

						if(dstOperandWord2 != NULL)
						{
							if(dstOperand2IsOffset) /*Add the symbol offset mark*/
							{
								temp = (char*)malloc(2);
								if(temp == NULL) exit(1);

								strcpy(temp,OFFSET_MARK_STRING);
								dstOperandWord2 = strcat(temp, dstOperandWord2);
							}

							insertInstructionToMemory(dstOperandWord2, NULL);
						}

						}
					}

					else /*A validation error */
					{
						addAssemblerError(validationMsg,lineNum);
						success = FALSE;
					}

			}

		 	 lineNum++;
		 }

	return success;
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

