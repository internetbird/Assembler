#include <stdio.h>
#include "parser.h"


int main(int argc, char *argv[])
{
	FILE *file;
	Symbol symbol;
	char line[100];
	int i;

	if((file = fopen(argv[1], "r")) != NULL)
	{
		i = 1;

		while(fgets(line,100, file) != NULL)
		{
			printf("line %d:%s\n", i, line);

			if(isLineContainsSymbol(line))
			{

				symbol = getSymbolFromCommandLine(line);


			}

			i++;

		}
	}
	return 0;
}


