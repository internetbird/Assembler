#ifndef SYMBOLS_H_
#define SYMBOLS_H_


#define ERROR_SYMBOL_ALREADY_EXISTS "Symbol already exists. "
#define ERROR_COULD_NOT_ALLOCATE_MEMORY "Could not allocate symbol memory."

#define BASE_2_VALUE_SIZE 8

typedef enum
{
	UNDEFINED,
	CODE,
	DATA

} SymbolType;

typedef struct
{
	char *name;
	unsigned int value;
	SymbolType type;

} Symbol;


typedef struct symbolNode
{
	Symbol symbol;
	struct symbolNode *next;

} SymbolNode; /* A node in a symbol list */

typedef struct symbolNameNode
{
	char *name;
	struct symbolNameNode *next;

} SymbolNameNode;

typedef struct
{
	SymbolNameNode *first;
	SymbolNameNode *last;

} SymbolNameList;

typedef struct
{
	SymbolNode *first;
	SymbolNode *last;

} SymbolTable;

void initSymboTables();
Symbol *getSymbol(char *name);

/*Inserts a symbol with the given name, type  and value to the assembler symbol table*/
/*Returns NULL if the insertion was successful and an error message otherwise*/
char *insertSymbol(char *name, SymbolType type, unsigned int value);

/*Inserts an external symbol name to the external symbols list */
/*Returns NULL if the insertion was successful and an error message otherwise*/
char *insertExternSymbolName(char *name);

/*Inserts an external symbol name and value to the external symbols table(used when creating the .ex file)*/
/*Returns NULL if the insertion was successful and an error message otherwise*/
char *insertExternSymbolValue(char *name, unsigned int value);

/*Inserts an entry symbol name to the entry symbol table */
/*Returns NULL if the insertion was successful and an error message otherwise*/
char *insertEntrySymbol(char *name);
int isSymbolExists(char *name);
int isExternSymbolExists(char *name);
SymbolNameList getExternSymbolList();
SymbolTable getExternSymbolTable();
SymbolNameList getEntrySymbolList();

#endif /* SYMBOLS_H_ */
