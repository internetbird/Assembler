#ifndef VALIDATOR_H_
#define VALIDATOR_H_

#define MAX_BYTES 80
#define INVALID_NUM "invalid number of operands"
#define NO_STRING "no string was received in .string line"
#define WRONG_ADDRESSING "the addressing method is incompatible with the command"
#define TOO_LONG "the command line is longer than 80 characters"
#define NOT_A_NUMBER "there is a parameter that is not an integer"

char *validateAssemblyLine(char *line);

#endif
