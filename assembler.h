#ifndef ASSEMBLER_H_
#define ASSEMBLER_H_

unsigned int IC; /* The instruction counter */
unsigned int DC; /* The data counter */

/* Assembler error messages */
#define COMMAND_NOT_VALID "The command is not valid."
#define SYMBOL_NOT_VALID "Symbol not vallid."

/* Assembler constants */
#define IC_STARTUP_VALUE 100
#define WORD_SIZE 16
#define DATA_MEMORY_SIZE 10000
#define INSTRUCTION_MEMORY_SIZE 10000
#define MAX_SYMBOL_LENGTH 30
#define SYMBOL_END_CHAR ':'

/* Assembler keywords */
#define DATA_GUIDANCE ".data"
#define STRING_GUIDANCE ".string"
#define ENTRY_GUIDANCE ".entry"
#define EXTERN_GUIDANCE ".extern"


int insertDataToMemory(char *line); /* Inserts a string or number data to the data memory . Returns the number of words that where inserted. */
void insertInstructionToMemory(char *word); /*Inserts a single word to the instruction memory */
void addAssemblerError(const char *errorMessage, int lineNumber);

#endif
