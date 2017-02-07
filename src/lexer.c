//
// lexer.c
//
// Lexical analyzer, generates symbol table
//
// Created by @avikantz on 01/27/17
//

#define PRINT_CODE NO
#define DEBUG_MODE YES

#include "lexer.h"

BOOL isKeyword (char buffer[BUFFER_SIZE]) {
	int i;
	for (i = 0; i < KEYWORD_COUNT; ++i) {
		if (strcmp(KEYWORDS[i], buffer) == 0)
			return YES;
	}
	return NO;
}

BOOL isDataType (char buffer[BUFFER_SIZE]) {
	int i;
	for (i = 0; i < 7; ++i) {
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

	tnode_p_t token_head = init_tnode("$");

	int scope_flag = 0;

	do {
		
		ch = getc(input);

		if (ch == ' ' || ch == '\t') { // Simply discard the blank space

		} else if (ch == '\n') { // Put the next line on the output buffer
	
			if (PRINT_CODE) printf("\n");
			fprintf(output, "\n");
		
		} else if (ch == '\"' && lch != '\\') { // Wait till the string ends
			
			k = 0;
			buffer[k++] = ch;
			
			do {
				ch = getc(input);
				buffer[k++] = ch;
			} while (ch != '\"');
			
			buffer[k++] = EOS;

			if (PRINT_CODE) printf("<str, %s> ", buffer);
			fprintf(output, "<str, %s> ", buffer);

		} else if (ch == '\'' && lch != '\\') { // Wait till the character stream ends
			
			k = 0;
			
			do {
				ch = getc(input);
				buffer[k++] = ch;
			} while (ch != '\'');
			
			buffer[k-1] = EOS; // Don't want "'" in there.

			if (PRINT_CODE) printf("<char, %s> ", buffer);
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
				
				if (PRINT_CODE) printf("<%s> ", buffer);
				fprintf(output, "<%s> ", buffer);

				char *prevToken = token_head->prev->name;
				if (strcmp(buffer, "if") == 0) {
					if (strcmp(prevToken, "else") == 0) {
						printf("ELSE-IF detected.\n");
					} else {
						printf("IF detected.\n");
					}
				} else if (strcmp(buffer, "else") == 0) {
					printf("ELSE detected.\n");
				}

				// Append the keyword to the token buffer
				insert_token(&token_head, buffer);
				// print_token_buffer(token_head);

			} else {
				// Create an arbitary entry
				// TODO: determine the type of the entry.
				st_entry_t new_entry = new_symbol_table_entry(buffer, TOKEN_TYPE_ID, sizeof(int), TOKEN_SCOPE_LOCAL, 0, " ");

				BOOL inserted;
				st_node_p_t stnode = find_or_insert_st(&symbol_table, new_entry, &inserted);

				if (inserted) {
					// New token is inserted to the symbol table
					char *prevToken = token_head->prev->name; // Get the previous token to determine the type
					if (isKeyword(prevToken)) {
						// Previous token is a keyword, it's a variable declaration, do relevant operations.
						printf("New token <%s, %s> is inserted to ST.\n", buffer, token_head->prev->name);

						// Set size and type for the symbol table entry
						stnode->entry.type = get_token_type(prevToken);
						stnode->entry.size = get_token_size(stnode->entry.type);

						// Set scope
						stnode->entry.scope = (scope_flag == 0) ? TOKEN_SCOPE_GLOBAL : TOKEN_SCOPE_LOCAL; // Don't really see the relevance of it.

					} else {
						// Previous token is not a keyword, prehaps it's a function name ?
					}
				} else {	
					// printf("Token [%s] referenced again.", buffer);
				}

				if (PRINT_CODE) printf("<id, %d> ", stnode->entry.id);
				fprintf(output, "<id, %d> ", stnode->entry.id);

				// Append the token to the token head
				insert_token(&token_head, buffer);
				// print_token_buffer(token_head);
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
			if (PRINT_CODE) printf("<num, %s> ", buffer);
			fprintf(output, "<num, %s> ", buffer);

		} else if (isSpecialSymbol(ch)) {

			if (PRINT_CODE) printf("<%c> ", ch); // Just print the god damn special symbol
			fprintf(output, "<%c> ", ch);

			// Array indices
			if (ch == '[') {
				// Array is declared or referenced
				char *prevToken = token_head->prev->name;
				char *prevPrevToken = token_head->prev->prev->name;
				if (DEBUG_MODE) printf("Prev = %s, PrevPrev = %s\n", prevToken, prevPrevToken);
				if (isDataType(prevPrevToken) && !(isKeyword(prevToken))) {
					// Assume array declarations are of the form <keyword> <identifier> [NUM]
					char buffer[BUFFER_SIZE];
					strcpy(buffer, prevToken);
					st_entry_t new_entry = new_symbol_table_entry(buffer, TOKEN_TYPE_ID, sizeof(int), TOKEN_SCOPE_LOCAL, 0, " ");
					st_node_p_t stnode = find_in_st(&symbol_table, new_entry);
					if (stnode != NULL) {
						stnode->entry.nargs = 9; // Find and get size here.
						if (DEBUG_MODE) printf("Array declaration: %s\n", stnode->entry.name);
					}
				}
			}

			// Count the levels of scope.
			if (ch == '{') {
				scope_flag += 1;
			} else if (ch == '}') {
				scope_flag -= 1;
			}

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
				if (PRINT_CODE) printf("<%s> ", opname);
				fprintf(output, "<%s> ", opname);
				// printf("<%s, %s> ", opname, buffer);
				// fprintf(output, "<%s, %s> ", opname, buffer);
			}

		}

		lch = ch;

	} while (ch != EOF);

	// Print the symbol table for the glory of the Sontaran Empire
	// print_token_buffer(token_head);
	print_symbol_table(symbol_table);

}