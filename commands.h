
#ifndef COMMANDS_H_
#define COMMANDS_H_

#define COMMAND_BITS_LENGTH 4

/* Gets the assembly base2 command code by command name */
char *getCommandBase2Code(char *command_name);

/* Gets the allowed addressing modes for operand 1 of the given assembly command name */
short getOp1AddressingModes(char *command_name);

/* Gets the allowed addressing modes for operand 2 of the given assembly command name */
short getOp2AddressingModes(char *command_name);

int isValidCommand(char *command_name);

#endif
