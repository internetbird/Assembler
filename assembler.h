#ifndef ASSEMBLER_H_
#define ASSEMBLER_H_


/* Assembler error messages */
#define COMMAND_NOT_VALID "The command is not valid."
#define SYMBOL_NOT_VALID "Symbol not valid."

/* Assembler constants */
#define IC_STARTUP_VALUE 100
#define WORD_SIZE 16
#define DATA_MEMORY_SIZE 10000
#define INSTRUCTION_MEMORY_SIZE 10000
#define MAX_SYMBOL_LENGTH 30
#define SYMBOL_END_CHAR ':'
#define MAX_LINE_LENGTH 80
#define ASSEMBLY_FILE_EXT ".as"

/* Addressing Modes */
#define IMMEDIATE_ADDRESSING_MODE 0
#define DIRECT_ADDRESSING_MODE 1
#define INDEX_ADDRESSING_MODE 2
#define INDEX2D_ADDRESSING_MODE 3
#define REGISTER_ADDRESSING_MODE 4

/* Assembly command parts */
#define SOURCE_ADDRESING_MODE_OFFSET 4
#define SOURCE_REGISTER_OFFSET 7
#define DESTINATION_ADDRESING_MODE_OFFSET 10
#define DESTINATION_REGISTER_OFFSET 13
#define ADDRESSING_MODE_BITS_LENGTH 3
#define REGISTER_BITS_LENGTH 3

#define NOT_REGISTER_OPERAND "000"

/* Assembler keywords */
#define DATA_GUIDANCE ".data"
#define STRING_GUIDANCE ".string"
#define ENTRY_GUIDANCE ".entry"
#define EXTERN_GUIDANCE ".extern"

typedef enum statement_type
{
	EMPTY,
	COMMENT,
	DATAGUIDANCE,
	STRINGGUIDANCE,
	ENTRYGUIDANCE,
	EXTERNGUIDANCE,
	COMMAND

} StatementType;


void insertDataToMemory(char *line); /* Inserts a string or number data to the data memory . Returns the number of words that where inserted. */
void insertInstructionToMemory(char *word); /*Inserts a single word to the instruction memory */
void addAssemblerError(const char *errorMessage, int lineNumber);
void resetAssemblyCounters();

/*Returns the absolute value of the instruction counter */
unsigned int getInstructionCounter();
/*Returns the value of the instruction counter relative to the memory beginning address*/
unsigned int getInstructionCounterOffset();
unsigned int getDataCounter();
char *getInstructionMemoryWord(unsigned int index);
void setInstructionMemoryWord(unsigned int index, char *value);




#endif
