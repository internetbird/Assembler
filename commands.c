#include <stdio.h>
#include <string.h>
#include "commands.h"

#define NUM_OF_COMMANDS 16

#define ADR_MODE_0 01
#define ADR_MODE_1 02
#define ADR_MODE_2 04
#define ADR_MODE_3 010
#define ADR_MODE_4 0100

#define ALL_ADR_MODES ADR_MODE_0 | ADR_MODE_1 | ADR_MODE_2 | ADR_MODE_3 | ADR_MODE_4
#define ADR_MODES_1_TO_4 ADR_MODE_1 | ADR_MODE_2 | ADR_MODE_3 | ADR_MODE_4
#define NO_OPERAND 0

/* Definition of all the assembly command names, base2 values and allowed addressing modes */


typedef struct command {

	char *name;
	char *base2code;
	short Op1AddressingModes;
	short Op2AddressingModes;


} Command;

Command commands[NUM_OF_COMMANDS] = {

		{"mov", "0000", ALL_ADR_MODES , ADR_MODES_1_TO_4  },
		{"cmp", "0001", ALL_ADR_MODES, ALL_ADR_MODES},
		{"add", "0010", ALL_ADR_MODES, ADR_MODES_1_TO_4},
		{"sub", "0011", ALL_ADR_MODES, ADR_MODES_1_TO_4},
		{"not", "0100", NO_OPERAND, ADR_MODE_2 | ADR_MODE_3 | ADR_MODE_4},
		{"clr", "0101", NO_OPERAND, ADR_MODES_1_TO_4},
		{"lea", "0110", ADR_MODE_1 | ADR_MODE_2 | ADR_MODE_3, ADR_MODES_1_TO_4},
		{"inc", "0111", NO_OPERAND, ADR_MODES_1_TO_4},
		{"dec", "1000", NO_OPERAND, ADR_MODES_1_TO_4},
		{"jmp", "1001", NO_OPERAND, ADR_MODES_1_TO_4},
		{"bne", "1010", NO_OPERAND, ADR_MODES_1_TO_4},
		{"red", "1011", NO_OPERAND, ADR_MODES_1_TO_4},
		{"prn", "1100", NO_OPERAND, ALL_ADR_MODES},
		{"jsr", "1101", NO_OPERAND, ADR_MODE_1},
		{"rts", "1110", NO_OPERAND, NO_OPERAND},
		{"stop", "1111", NO_OPERAND, NO_OPERAND}

};

Command getCommandByCommandName(char *command_name);

/* Gets the assembly base2 command code by command name */
/* The function assumes that the command name is valid */

char *getCommandBase2Code(char *command_name)
{
	Command the_command;

	the_command = getCommandByCommandName(command_name);

	return the_command.base2code;


}

/* Gets the allowed addressing modes for operand 1 of the given assembly command name */
/* The function assumes that the command name is valid */

short getOp1AddressingModes(char *command_name)
{
	Command the_command;

	the_command = getCommandByCommandName(command_name);

	return the_command.Op1AddressingModes;


}

/* Gets the allowed addressing modes for operand 2 of the given assembly command name */
/* The function assumes that the command name is valid */

short getOp2AddressingModes(char *command_name)
{
	Command the_command;

	the_command = getCommandByCommandName(command_name);

	return the_command.Op2AddressingModes;

}

/* Gets the command by command name */
/* The function assumes that the command name is valid */

Command getCommandByCommandName(char *command_name)
{
	int i;

	for (i=0;i<=NUM_OF_COMMANDS; i++ )
	{
		if(strcmp(commands[i].name, command_name) == 0)
		{
			return commands[i];
		}
	}


  return  commands[0];

}

int isValidCommand(char *command_name)
{

	int valid = 0, i;

	for (i=0;i<=NUM_OF_COMMANDS; i++ )
	{
		if(strcmp(commands[i].name, command_name) == 0)
		{
			valid = 1;
		}
	}

	return valid;
}



