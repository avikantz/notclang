//
// util.h
//
// Utility functions
//
// Created by @avikantz on 01/26/17
//

#ifndef IMPORT_STUFF
#define IMPORT_STUFF
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#endif

#ifndef KEYWORD_COUNT
#define KEYWORD_COUNT 32
extern const char *KEYWORDS[KEYWORD_COUNT];
#endif

typedef enum { NO, YES } BOOL;

// Checks if a character is a digit (duh)
BOOL isDigit (char);

// Checks for digit, '.', or 'E'/'e'
BOOL isNumerical (char);

// Checks if a character is an alphabet (duh)
BOOL isAlphabet (char);

// Checks if a character is a valid identifier character (letter, digit, or _)
BOOL isValidID (char);

// Checks if a character is a uppercase
BOOL isLowerCase (char);

// Checks if a character is a lowercase
BOOL isUpperCase (char);

typedef enum token_type {
	TOKEN_TYPE_INT,
	TOKEN_TYPE_CHAR,
	TOKEN_TYPE_FLOAT,
	TOKEN_TYPE_DOUBLE,
	TOKEN_TYPE_ID,
	TOKEN_TYPE_FUNC
} token_type_t;

typedef enum token_scope {
	TOKEN_SCOPE_LOCAL,
	TOKEN_SCOPE_GLOBAL
} token_scope_t;

typedef struct st_entry {
	int id;
	char name[128];
	token_type_t type;
	size_t size;
	token_scope_t scope;
	int nargs;
	char return_type[128];
	struct st_entry *args;
} st_entry_t;

// Retruns a new symbol table entry
st_entry_t new_symbol_table_entry (int id, char name[128], token_type_t type, size_t size, token_scope_t scope, int nargs, char return_type[128]);


