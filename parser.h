#include "symbols.h"
#include "assembler.h"

#ifndef PARSER_H_
#define PARSER_H_

#define NOT_REGISTER_OPERAND "000"

typedef struct
{
	char *symbol;
	char *command;
	char *sourceOperand;
	char *destinationOperand;
	char *externSymbol;
	char *data;

} CommandParts;


StatementType getStatementType(char *line);
CommandParts parseAssemblyLine(char *line);

/* Returns the integer value of the addressing mode for the given operand*/
/* The function assumes a valid operand */
int parseAddressingMode(char *operand);

const char *parseOperand(char *operand);
const char *parseCommand(char *command);

char *extractFirstGuidanceData(char *data);
/*Extracts the next number string in the data string that was initialized in the extractFirstGuidanceData method. */
/*Returns NULL if there are no more values to return */
char *extractNextGuidanceData();
char *extractGuidanceData(char *line);
char *extractGuidanceString(char *line);
int extractImmediateAddressingModeValue(char *operand);

/*Extracts the symbol part from a valid index addressing operand */
char *extractIndexAddressingSybol(char *operand);
/*Extracts the offset part from a valid index addressing operand */
char *extractIndexAddressingOffset(char *operand);
char *extractIndex2dAddressingSybol(char *operand);
char *extractIndex2dAddressingOffset(char *operand);

int isSymbolGuidanceCommand(char *line);

/*Returns a base 2 assembly word of the given base 10 value*/
char *convertBase10toBase2(int base10);

#endif
