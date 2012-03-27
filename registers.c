#include <stdio.h>
#include <string.h>
#include "registers.h"

#define NUM_OF_REGISTERS 8

struct assember_register {

	char *name;
	char *base2code;


} registers[NUM_OF_REGISTERS] = {

		{"r0", "000"},
		{"r1", "001"},
		{"r2", "010"},
		{"r3", "011"},
		{"r4", "100"},
		{"r5", "101"},
		{"r6", "110"},
		{"r7", "111"}

};

char *getRegisterBase2Code(char *registerName)
{
	int i;

	for (i=0; i<=NUM_OF_REGISTERS ;i++ )
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



