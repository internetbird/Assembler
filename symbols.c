#include <stdio.h>
#include <stdlib.h>
#include "symbols.h"

typedef struct
{
	SymbolNode *first;
	SymbolNode *last;

} SymbolTable;

SymbolTable assemblerSymbolTable;

void initSymboTable()
{
	assemblerSymbolTable.first = NULL;
	assemblerSymbolTable.last = NULL;
}

Symbol *getSymbol(char *name)
{
	Symbol *symbol = NULL;

	return symbol;
}

int isSymbolExists(char *name)
{
	return 0;
}

int insertSymbol(char name[], SymbolType type, unsigned int value)
{
	Symbol newSymbol;
	SymbolNode *newSymbolNode;

	if(!isSymbolExists(name))
	{

		newSymbolNode = (SymbolNode *)malloc(sizeof(SymbolNode));

	   if(newSymbolNode != NULL)
	   {
		   newSymbol.name = name;
		   newSymbol.type = type;
		   newSymbol.value = value;

		   newSymbolNode->symbol = newSymbol;
		   newSymbolNode->next = NULL;

		   if(assemblerSymbolTable.first == NULL) /* Symbol table is empty */
		   {
			   assemblerSymbolTable.first = newSymbolNode;
			   assemblerSymbolTable.last = newSymbolNode;
		   }else
		   {
			   assemblerSymbolTable.last->next = newSymbolNode;
			   assemblerSymbolTable.last = newSymbolNode;

		   }
	   } else
	   {
		   return 2;
	   }

	} else
	{
		return 1;
	}

	return 0;

}

int insertExternSymbol(char *name)
{
	return 0;

}


