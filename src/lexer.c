//
// lexer.c
//
// Lexical analyzer, generates symbol table
//
// Created by @avikantz on 01/27/17
//

#include "lexer.h"

BOOL isKeyword (char buffer[BUFFER_SIZE]) {
	int i;
	for (i = 0; i < KEYWORD_COUNT; ++i) {
		if (strcmp(KEYWORDS[i], buffer) == 0)
			return YES;
	}
	return NO;
}



void emit_tokens (char *inname) {

	FILE *input = fopen(inname, "r");

	char outname[128];
	snprintf(outname, 128, "tok_%s", inname);
	FILE *output = fopen(outname, "w+");

	char ch, lch;
	char buffer[BUFFER_SIZE]; int k = 0;

	do {
		
		ch = getc(input);

		if (ch == ' ' || ch == '\t') { // Simply discard the blank space

		} else if (ch == '\n') { // Put the next line on the output buffer
		
			putc(ch, output);
		
		} else if (ch == '\"' && lch != '\\') { // Wait till the string ends
			
			k = 0;
			buffer[k++] = ch;
			
			do {
				ch = getc(input);
				buffer[k++] = ch;
			} while (ch != '\"');
			
			buffer[k++] = EOS;
			printf("STRING: %s\n", buffer);

		} else if (isAlphabet(ch)) { // Take the identifier...

			k = 0;

			while (isValidID(ch)) {
				buffer[k++] = ch;
				ch = getc(input);
				if (k > BUFFER_SIZE) {
					printf("Compiler error (isAlphabet)! %s\n", buffer);
					return;
				}
			}
			buffer[k++] = EOS;

			// Check for keywords here.
			if (isKeyword(buffer)) {
				printf("KEYWRD: %s\n", buffer);
			} else {		
				printf("IDENTI: %s\n", buffer);
			}

		} else if (isDigit(ch)) { // Take the numbers separately

			k = 0;
			while (isNumerical(ch)) {
				buffer[k++] = ch;
				ch = getc(input);
				if (k > BUFFER_SIZE) {
					printf("Compiler error (isDigit)! %s\n", buffer);
					return;
				}
			}
			buffer[k++] = EOS;
			printf("NUMBER: %s\n", buffer);

		}

		lch = ch;

	} while (ch != EOF);

}