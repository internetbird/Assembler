/******************************************************************************/
/* Assembler project for course 20465                                         */
/* Programmed by : Itay Parsiado (ID:062862495) & Shai Aharoni (ID:025332990) */
/******************************************************************************/

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
#define LINKER_ABSOLUTE "a"
#define LINKER_RELOCATABLE "r"
#define LINKER_EXTERNAL "e"
#define OFFSET_MARK_LABEL '*' /*A character added to the beginning of a symbol to mark that it is an offset*/
#define OFFSET_MARK_STRING "*"

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

#define TRUE 1
#define FALSE 0

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
void insertInstructionToMemory(char *word, char *linkerChar); /*Inserts a single word to the instruction memory */
void addAssemblerError(const char *errorMessage, int lineNumber);
void resetAssemblyCounters();
unsigned int getInstructionCounter();
unsigned int getDataCounter();
char *getInstructionMemoryWord(unsigned int index);
void setInstructionMemoryWord(unsigned int index, char *value, char* linkerChar);

#endif
