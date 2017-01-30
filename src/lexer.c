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

	st_node_p_t symbol_table;

	do {
		
		ch = getc(input);

		if (ch == ' ' || ch == '\t') { // Simply discard the blank space

		} else if (ch == '\n') { // Put the next line on the output buffer
	
			printf("\n");
			fprintf(output, "\n");
		
		} else if (ch == '\"' && lch != '\\') { // Wait till the string ends
			
			k = 0;
			buffer[k++] = ch;
			
			do {
				ch = getc(input);
				buffer[k++] = ch;
			} while (ch != '\"');
			
			buffer[k++] = EOS;

			printf("<str, %s> ", buffer);
			fprintf(output, "<str, %s> ", buffer);

		} else if (ch == '\'' && lch != '\\') { // Wait till the character stream ends
			
			k = 0;
			
			do {
				ch = getc(input);
				buffer[k++] = ch;
			} while (ch != '\'');
			
			buffer[k-1] = EOS; // Don't want "'" in there.

			printf("<char, %s> ", buffer);
			fprintf(output, "<char, %s> ", buffer);

		} else if (isAlphabet(ch)) { // Take the identifier...

			k = 0;

			while (isValidID(ch)) {
				buffer[k++] = ch;
				ch = getc(input);
				if (k > BUFFER_SIZE) {
					printf("Error: Buffer overflow (isAlphabet)! %s\n", buffer);
					return;
				}
			}
			ungetc(ch, input); // Put the last character back in the buffer, cause the while loop takes one more
			buffer[k++] = EOS;

			// Check for keywords here.
			if (isKeyword(buffer)) {
				
				printf("<%s> ", buffer);
				fprintf(output, "<%s> ", buffer);

			} else {
				// Create an arbitary entry
				// TODO: determine the type of the entry.
				st_entry_t new_entry = new_symbol_table_entry(buffer, TOKEN_TYPE_ID, sizeof(int), TOKEN_SCOPE_LOCAL, 0, " ");

				BOOL inserted;
				st_node_p_t stnode = find_or_insert_st(&symbol_table, new_entry, &inserted);
				// if (inserted) {
				// Do something if inserted.
				// } else {
				// Do something of already present node.
				// }

				printf("<id, %d> ", stnode->entry.id);
				fprintf(output, "<id, %d> ", stnode->entry.id);
			}

		} else if (isDigit(ch)) { // Take the numbers separately

			k = 0;
			while (isNumerical(ch)) {
				buffer[k++] = ch;
				ch = getc(input);
				if (k > BUFFER_SIZE) {
					printf("Error: Buffer overflow (isDigit): %s\n", buffer);
					return;
				}
			}
			ungetc(ch, input); // Put the last character back in the buffer, cause the while loop takes one more
			buffer[k++] = EOS;
			printf("<num, %s> ", buffer);
			fprintf(output, "<num, %s> ", buffer);

		} else if (isSpecialSymbol(ch)) {

			printf("<%c> ", ch); // Just print the god damn special symbol
			fprintf(output, "<%c> ", ch);

		} else if (isOperator(ch)) {

			k = 0;
			while (isOperator(ch) || ch == ' ' || ch == '\t') { // To detect terms like "< =" 
				if (isOperator(ch)) {
					buffer[k++] = ch;
				}
				ch = getc(input);
			}
			buffer[k++] = EOS;
			ungetc(ch, input); // Put the last character back in the buffer, cause the while loop takes one more

			char *opname = operator_name(buffer);
			if (opname != NULL) {
				printf("<%s> ", opname);
				fprintf(output, "<%s> ", opname);
				// printf("<%s, %s> ", opname, buffer);
				// fprintf(output, "<%s, %s> ", opname, buffer);
			}

		}

		lch = ch;

	} while (ch != EOF);

}