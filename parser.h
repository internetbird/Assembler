#include "symbols.h"
#include "assembler.h"

#ifndef PARSER_H_
#define PARSER_H_

#define IMMEDIATE_ADDRESSING_MODE "000"
#define DIRECT_ADDRESSING_MODE "001"
#define INDEX_ADDRESSING_MODE "010"
#define INDEX2D_ADDRESSING_MODE "011"
#define REGISTER_ADDRESSING_MODE "100"

#define NOT_REGISTER_OPERAND "000"


typedef enum statement_type
{
	EMPTY,
	COMMENT,
	DATAGUIDANCE,
	STRINGGUIDANCE,
	ENTRYGUIDANCE,
	EXTERNGUIDANCE,
	COMMAND

} StatementType;

typedef struct
{
	char *symbol;
	char *command;
	char *sourceOperand;
	char *destinationOperand;
	char *externSymbol;
	char *data;

} CommandParts;

/*

typedef enum addressing_modes
{
	IMMEDIATE,
	DIRECT,
	INDEX,
	INDEX2D,
	REGISTER

} AddressingMode;
*/

StatementType getStatementType(char *line);
CommandParts parseAssemblyLine(char *line);

/* Returns the base2 value of the addressing mode for the given operand*/
/* The function assumes a valid operand */
char *parseAddressingMode(char *operand);

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
