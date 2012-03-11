#include <stdio.h>
#include <string.h>
#include "registers.h"

#define NUM_OF_REGISTERS 8

struct assember_register {

	char *name;
	char *base2code;


} registers[NUM_OF_REGISTERS] = {

		{"r0", "0000"},
		{"r1", "0001"},
		{"r2", "0010"},
		{"r3", "0011"},
		{"r4", "0100"},
		{"r5", "0101"},
		{"r6", "0110"},
		{"r7", "0111"}

};

char *getRegisterBase2Code(char *registerName)
{
	int i;

	for (i=0; i++; i<=NUM_OF_REGISTERS)
	{
		if(strcmp(registers[i].name, registerName) == 0)
		{
			return registers[i].base2code;
		}
	}

	return NULL;

}

int isRegister(char *name)
{
	if(getRegisterBase2Code(name) == NULL)
	{
		return 0;

	} else
	{
		return 1;
	}

}



