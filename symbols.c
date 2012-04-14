#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symbols.h"


SymbolTable assemblerSymbolTable;
SymbolNameList entrySymbolList;
SymbolNameList externSymbolList;
SymbolTable externSymbolTable;


void initSymboTables()
{
	assemblerSymbolTable.first = NULL;
	assemblerSymbolTable.last = NULL;

	entrySymbolList.first = NULL;
	entrySymbolList.last = NULL;

	externSymbolList.first = NULL;
	externSymbolList.last = NULL;

	externSymbolTable.first = NULL;
	externSymbolTable.last = NULL;

}


Symbol *getSymbol(char *name)
{
	Symbol *symbol = NULL;
	SymbolNode *current;

	current = assemblerSymbolTable.first;

	while(current != NULL)
	{
		if(strcmp(current->symbol.name, name) == 0)
		{
			symbol = &(current->symbol);
			break;
		}

		current = current->next;
	}

	return symbol;
}

int isSymbolExists(char *name)
{
	SymbolNode *current;
	int exists = 0;

	current = assemblerSymbolTable.first;

	while(current != NULL)
	{
		if(strcmp(current->symbol.name, name) == 0)
		{
			exists = 1;
			break;
		}

		current = current->next;
	}

	return exists;
}

int isExternSymbolExists(char *name)
{
	SymbolNameNode *current;
	int exists = 0;

	current = externSymbolList.first;

	while(current != NULL)
	{
		if(strcmp(current->name, name) == 0)
		{
			exists = 1;
			break;
		}

		current = current->next;
	}

	return exists;

}

int isEntrySymbolExists(char *name)
{
	SymbolNameNode *current;
	int exists = 0;

	current = entrySymbolList.first;

	while(current != NULL)
	{
		if(strcmp(current->name, name) == 0)
		{
			exists = 1;
			break;
		}

		current = current->next;
	}

	return exists;

}

int insertSymbol(char *name, SymbolType type, unsigned int value)
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
		   return ERROR_COULD_NOT_ALLOCATE_MEMORY;
	   }

	} else
	{
		return ERROR_SYMBOL_IS_ALREADY_IN_TABLE;
	}

	return 0;

}

int insertExternSymbolName(char *name)
{

	SymbolNameNode *newNode;

	if(!isExternSymbolExists(name))
	{

		newNode = (SymbolNameNode *)malloc(sizeof(SymbolNameNode));

	   if(newNode != NULL)
	   {

		   newNode->name = name;
		   newNode->next = NULL;

		   if(externSymbolList.first == NULL) /* Entries list is empty */
		   {
			   externSymbolList.first = newNode;
			   externSymbolList.last = newNode;
		   }else
		   {
			   externSymbolList.last->next = newNode;
			   externSymbolList.last = newNode;

		   }
	   } else
	   {
		   return ERROR_COULD_NOT_ALLOCATE_MEMORY;
	   }

	} else
	{
		return ERROR_SYMBOL_IS_ALREADY_IN_TABLE;
	}

	return 0;

}

int insertExternSymbolValue(char *name, unsigned int value)
{
	Symbol newSymbol;
	SymbolNode *newSymbolNode;

	newSymbolNode = (SymbolNode *)malloc(sizeof(SymbolNode));

   if(newSymbolNode != NULL)
   {
	   newSymbol.name = name;
	   newSymbol.type = UNDEFINED;
	   newSymbol.value = value;

	   newSymbolNode->symbol = newSymbol;
	   newSymbolNode->next = NULL;

	   if(externSymbolTable.first == NULL) /* Symbol table is empty */
	   {
		   externSymbolTable.first = newSymbolNode;
		   externSymbolTable.last = newSymbolNode;
	   }else
	   {
		   externSymbolTable.last->next = newSymbolNode;
		   externSymbolTable.last = newSymbolNode;

	   }

   	 } else
   	 {
		   return ERROR_COULD_NOT_ALLOCATE_MEMORY;
	 }

	return 0;

}

int insertEntrySymbol(char *name)
{

	SymbolNameNode *newNode;

	if(!isEntrySymbolExists(name))
	{

		newNode = (SymbolNameNode *)malloc(sizeof(SymbolNameNode));

	   if(newNode != NULL)
	   {

		   newNode->name = name;
		   newNode->next = NULL;

		   if(entrySymbolList.first == NULL) /* Entries list is empty */
		   {
			   entrySymbolList.first = newNode;
			   entrySymbolList.last = newNode;
		   }else
		   {
			   entrySymbolList.last->next = newNode;
			   entrySymbolList.last = newNode;

		   }
	   } else
	   {
		   return ERROR_COULD_NOT_ALLOCATE_MEMORY;
	   }

	} else
	{
		return ERROR_SYMBOL_IS_ALREADY_IN_TABLE;
	}

	return 0;

}

SymbolNameList getExternSymbolList()
{
	return externSymbolList;
}

SymbolNameList getEntrySymbolList()
{
	return entrySymbolList;
}

SymbolTable getExternSymbolTable()
{
	return externSymbolTable;
}




