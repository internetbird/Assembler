#ifndef SYMBOLS_H_
#define SYMBOLS_H_

#define MAX_SYMBOL_NAME_SIZE 30
#define BASE_2_VALUE_SIZE 8
#define SYMBOL_END_CHAR ':'

typedef enum
{
	CODE,
	DATA

} SymbolType;

typedef struct
{
	char name[MAX_SYMBOL_NAME_SIZE];
	unsigned int value;
	SymbolType type;

} Symbol;


typedef struct symbolNode
{
	Symbol symbol;
	struct symbolNode *next;

} SymbolNode; /* A node in a symbol list */


void initSymboTable();
Symbol *getSymbol(char *name);
int insertSymbol(char name[], SymbolType type, unsigned int value);/* Inserts a new symbol to the symbols table */
int insertExternSymbol(char *name);
int isSymbolExists(char *name);


#endif /* SYMBOLS_H_ */
