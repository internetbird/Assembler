
#ifndef COMMANDS_H_
#define COMMANDS_H_

#define COMMAND_BITS_LENGTH 4
#define COMMAND_CHAR_LENGTH 3

#define ADR_MODE_IMD_FLAG 01
#define ADR_MODE_DIRECT_FLAG 02
#define ADR_MODE_INDEX_FLAG 04
#define ADR_MODE_INDEX2D_FLAG 010
#define ADR_MODE_REGISTER_FLAG 0100

#define NO_OPERAND 0

typedef struct command {

	char *name;
	char *base2code;
	short srcOperandAddressingModes;
	short dstOperandAddressingModes;


} Command;

/* Gets the assembly base2 command code by command name */
char *getCommandBase2Code(char *command_name);

/* Gets the allowed addressing modes for operand 1 of the given assembly command name */
short getSrcOperandAddressingModes(char *command_name);

/* Gets the allowed addressing modes for operand 2 of the given assembly command name */
short getDstOperandAddressingModes(char *command_name);

/* Gets the command by command name */
/* If the command name is not valid the function returns an empty command (name = NULL) */
Command getCommandByCommandName(char *command_name);

/*Converts an addressing mode number to the addressing mode flag*/
short convertToAddressingModeFlag(short adderssingMode);

/*Converts the addressing mode integer value to the binary string */
char *getAddressingModeBits(int addressingMode);

#endif
