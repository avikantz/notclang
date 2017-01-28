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

#pragma mark - Symbol table entries

st_entry_t new_symbol_table_entry (int id, char name[128], token_type_t type, size_t size, token_scope_t scope, int nargs, char return_type[128]) {
	st_entry_t entry;
	entry.id = id;
	strcpy(entry.name, name);
	entry.type = type;
	entry.size = size;
	entry.scope = scope;
	entry.nargs = nargs;
	entry.args = (st_entry_t *)calloc(10, sizeof(st_entry_t));
	return entry;
}