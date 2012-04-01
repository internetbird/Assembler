#include <stdio.h>
#include <string.h>
#include "commands.h"
#include "assembler.h"

#define NUM_OF_COMMANDS 16

#define ADR_MODE_IMD_BITS "000"
#define ADR_MODE_DIRECT_BITS "001"
#define ADR_MODE_INDEX_BITS "010"
#define ADR_MODE_INDEX2D_BITS "011"
#define ADR_MODE_REGISTER_BITS "100"

#define ALL_ADR_MODES ADR_MODE_IMD_FLAG | ADR_MODE_DIRECT_FLAG | ADR_MODE_INDEX_FLAG | ADR_MODE_INDEX2D_FLAG | ADR_MODE_REGISTER_FLAG
#define ADR_MODES_1_TO_4 ADR_MODE_DIRECT_FLAG | ADR_MODE_INDEX_FLAG | ADR_MODE_INDEX2D_FLAG | ADR_MODE_REGISTER_FLAG

/* Definition of all the assembly command names, base2 values and allowed addressing modes */

Command commands[NUM_OF_COMMANDS] = {

		{"mov", "0000", ALL_ADR_MODES , ADR_MODES_1_TO_4  },
		{"cmp", "0001", ALL_ADR_MODES, ALL_ADR_MODES},
		{"add", "0010", ALL_ADR_MODES, ADR_MODES_1_TO_4},
		{"sub", "0011", ALL_ADR_MODES, ADR_MODES_1_TO_4},
		{"not", "0100", NO_OPERAND, ADR_MODE_INDEX_FLAG | ADR_MODE_INDEX2D_FLAG | ADR_MODE_REGISTER_FLAG},
		{"clr", "0101", NO_OPERAND, ADR_MODES_1_TO_4},
		{"lea", "0110", ADR_MODE_DIRECT_FLAG | ADR_MODE_INDEX_FLAG | ADR_MODE_INDEX2D_FLAG, ADR_MODES_1_TO_4},
		{"inc", "0111", NO_OPERAND, ADR_MODES_1_TO_4},
		{"dec", "1000", NO_OPERAND, ADR_MODES_1_TO_4},
		{"jmp", "1001", NO_OPERAND, ADR_MODES_1_TO_4},
		{"bne", "1010", NO_OPERAND, ADR_MODES_1_TO_4},
		{"red", "1011", NO_OPERAND, ADR_MODES_1_TO_4},
		{"prn", "1100", NO_OPERAND, ALL_ADR_MODES},
		{"jsr", "1101", NO_OPERAND, ADR_MODE_DIRECT_FLAG},
		{"rts", "1110", NO_OPERAND, NO_OPERAND},
		{"stop", "1111", NO_OPERAND, NO_OPERAND}

};


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

short getSrcOperandAddressingModes(char *command_name)
{
	Command command;

	command = getCommandByCommandName(command_name);

	return command.srcOperandAddressingModes;
}

/* Gets the allowed addressing modes for operand 2 of the given assembly command name */
/* The function assumes that the command name is valid */

short getDstOperandAddressingModes(char *command_name)
{
	Command command;

	command = getCommandByCommandName(command_name);

	return command.dstOperandAddressingModes;
}

/* Gets the command by command name */
/* If the command name is not valid the function returns an empty command (name = NULL) */

Command getCommandByCommandName(char *command_name)
{
	int i;
	Command command;

	command.name = NULL; /*Initialize the name for validation purposes, this is how we know that the command name is not valid*/

	for (i=0;i<=NUM_OF_COMMANDS; i++ )
	{
		if(strcmp(commands[i].name, command_name) == 0)
		{
			command = commands[i];
		}
	}

  return  command;
}

/*Converts an addressing mode number to the addressing mode flag*/
short convertToAddressingModeFlag(short adderssingMode)
{

	short flag;

	switch(adderssingMode)
	{
		case IMMEDIATE_ADDRESSING_MODE:
			flag = ADR_MODE_IMD_FLAG;
			break;

		case DIRECT_ADDRESSING_MODE:
			flag = ADR_MODE_DIRECT_FLAG;
			break;

		case INDEX_ADDRESSING_MODE:
			flag = ADR_MODE_INDEX_FLAG;
			break;

		case INDEX2D_ADDRESSING_MODE:
			flag = ADR_MODE_INDEX_FLAG;
			break;

		case REGISTER_ADDRESSING_MODE:
			flag = ADR_MODE_REGISTER_FLAG;
			break;

		default:
			flag = NO_OPERAND;
			break;

	}

	return flag;
}

/*Converts the addressing mode integer value to the binary string */
char *getAddressingModeBits(int addressingMode)
{
	char *bits;

	switch(addressingMode)
	{
		case IMMEDIATE_ADDRESSING_MODE:
			bits = ADR_MODE_IMD_BITS;
			break;

		case DIRECT_ADDRESSING_MODE:
			bits = ADR_MODE_DIRECT_BITS;
			break;

		case INDEX_ADDRESSING_MODE:
			bits = ADR_MODE_INDEX_BITS;
			break;

		case INDEX2D_ADDRESSING_MODE:
			bits = ADR_MODE_INDEX2D_BITS;
			break;

		case REGISTER_ADDRESSING_MODE:
			bits = ADR_MODE_REGISTER_BITS;
			break;

		default:
			break;

	}

	return bits;


}






