#ifndef SYMBOLS_H_
#define SYMBOLS_H_


#define ERROR_SYMBOL_IS_ALREADY_IN_TABLE 1
#define ERROR_COULD_NOT_ALLOCATE_MEMORY 2

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
int insertSymbol(char *name, SymbolType type, unsigned int value);/* Inserts a new symbol to the symbols table */
int insertExternSymbolName(char *name);
int insertExternSymbolValue(char *name, unsigned int value);
int insertEntrySymbol(char *name);
int isSymbolExists(char *name);
int isExternSymbolExists(char *name);
SymbolNameList getExternSymbolList();
SymbolTable getExternSymbolTable();
SymbolNameList getEntrySymbolList();

#endif /* SYMBOLS_H_ */
