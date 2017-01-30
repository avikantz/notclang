// 
// util.c
//
// Utility functions
// 
// Created by @avikantz on 01/26/17
//

#include "util.h"

const char *KEYWORDS[KEYWORD_COUNT] = {
	"auto", 	"double",	"int",		"struct",	"break",	"else",		"long",		"switch",	
	"case",		"enum",		"register",	"char",		"typedef",	"extern",	"return",	"union",	
	"continue",	"for",		"signed",	"void",		"do",		"if",		"static",	"while",	
	"default",	"goto",		"sizeof",	"volatile",	"const",	"float",	"short",	"unsigned"
};

#pragma mark - Utility functions

BOOL isDigit (char ch) {
	return (ch >= '0' && ch <= '9');
}

BOOL isNumerical (char ch) {
	return (isDigit(ch) || ch == '.' || ch == 'e' || ch == 'E');
}

BOOL isAlphabet (char ch) {
	return ((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z'));
}

BOOL isValidID (char ch) {
	return (isDigit(ch) || isAlphabet(ch) || ch == '_');
}

BOOL isLowerCase (char ch) {
	return (ch >= 'a' || ch <= 'z'); 
}

BOOL isUpperCase (char ch) {
	return (ch >= 'A' && ch <= 'Z'); 
}

const char *SPECIALSYMBOLS = "[]{}(),;:.#";

BOOL isSpecialSymbol (char ch) {
	return (strchr(SPECIALSYMBOLS, ch) != NULL);
}

const char *OPERATORS = "+-*/%<>=!&|^~";

BOOL isOperator (char ch) {
	return (strchr(OPERATORS, ch) != NULL);
}

const char *OPERATOR_NAMES[52] = {
	"+", 	"ADD",
	"-",	"SUB",
	"*",	"MUL",
	"/",	"DIV",
	"%",	"MOD",
	"+=",	"ADD ASS",
	"-=",	"SUB ASS",
	"*=",	"MUL ASS",
	"/=",	"DIV ASS",
	"%=",	"MOD ASS",
	"=",	"ASS",
	">",	"GT",
	"<",	"LT",
	"!",	"NOT",
	">=",	"GE",
	"<=",	"LE",
	"==",	"EQ",
	"!=",	"NE",
	"&",	"BAND",
	"|",	"BOR",
	"~",	"BNOT",
	"^",	"BXOR",
	">>",	"BRS",
	"<<",	"BLS",
	"&&",	"LAND",
	"||",	"LOR"
};

char *operator_name (char *op) {
	int i;
	char *opname = (char *)malloc(10 * sizeof(char));
	for (i = 0; i < 2 * 26; i += 2) {
		if (strcmp(OPERATOR_NAMES[i], op) == 0) {
			strcpy(opname, OPERATOR_NAMES[i+1]);
			return opname;
		}
	}
	return NULL;
}

#pragma mark - Symbol table entries

st_entry_t new_symbol_table_entry (char name[128], token_type_t type, size_t size, token_scope_t scope, int nargs, char return_type[128]) {
	st_entry_t entry;
	strcpy(entry.name, name);
	entry.type = type;
	entry.size = size;
	entry.scope = scope;
	entry.nargs = nargs;
	strcpy(entry.return_type, return_type);
	return entry;
}

st_node_p_t init_st_node (st_entry_t entry) {
	st_node_p_t node = (st_node_p_t)malloc(sizeof(st_node_t));
	node->entry = entry;
	node->next = NULL;
	return node;
}

// Finds a symbol table entry by its name, inserts it otherwise
st_node_p_t find_or_insert_st (st_node_p_t *head, st_entry_t entry, BOOL *inserted) {
	if (*head == NULL) {
		*head = init_st_node(entry);
		(*head)->entry.id = 1;
		*inserted = YES;
		return *head; // Only one node, currently inserted.
	}
	int id = 1;
	st_node_p_t temp = *head;
	while (temp->next != NULL) {
		if (strcmp(temp->entry.name, entry.name) == 0) {
			*inserted = NO;
			return temp;
		}
		temp = temp->next;
		id = temp->entry.id;
	}
	temp->next = init_st_node(entry);
	temp->next->entry.id = id + 1;
	*inserted = YES;
	return temp->next;
}


void print_symbol_table (st_node_p_t head) {

	if (head == NULL) {
		return;
	}

	// Finish this...

}
