#include "include/lexer.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

lexer_T* init_lexer(char* contents) {
    lexer_T* lexer = calloc(1, sizeof(struct LEXER_STRUCT));
    lexer->contents = contents;
    lexer->i = 0;
    lexer->c = contents[lexer->i];

    return lexer;
}

void lexer_advance(lexer_T* lexer) {
    // gets the next char in contents if there is one
    if (lexer->c != '\0' && lexer->i < strlen(lexer->contents)) {
        lexer->i += 1;
        lexer->c = lexer->contents[lexer->i];
    }
}

void lexer_skip_whitespace(lexer_T* lexer) {
    // skip white space eg space and 10 is the code for new line
    while (lexer->c == ' ' || lexer->c == 10) {
        lexer_advance(lexer);
    }
}

token_T* lexer_get_next_token(lexer_T* lexer) {
    // goes thru each char in contents and skips white space
    while (lexer->c != '\0' && lexer->i < strlen(lexer->contents)) {
        // skip white space line in lexer_skip_whitespace
        if (lexer->c == ' ' || lexer->c == 10) {
            lexer_skip_whitespace(lexer);
        }

        if (lexer->c == '"') {
            return lexer_collect_string(lexer);
        }

        if (isalnum(lexer->c)) {
            return lexer_collect_ID(lexer);
        }

        switch (lexer->c) {
            case '=': return lexer_advance_with_token(lexer, init_token(TOKEN_EQUALS, lexer_get_current_char_as_string(lexer))); break;
            case ';': return lexer_advance_with_token(lexer, init_token(TOKEN_SEMI, lexer_get_current_char_as_string(lexer))); break;
            case '(': return lexer_advance_with_token(lexer, init_token(TOKEN_LPAREN, lexer_get_current_char_as_string(lexer))); break;
            case ')': return lexer_advance_with_token(lexer, init_token(TOKEN_RPAREN, lexer_get_current_char_as_string(lexer))); break;
        }
    }

    return (void*)0;
}

token_T* lexer_collect_string(lexer_T* lexer) {
    // skip over first "
    lexer_advance(lexer);

    // allocate memory for a string
    char* value = calloc(1, sizeof(char));
    value[0] = '\0';

    while (lexer->c != '"') {
        char* s = lexer_get_current_char_as_string(lexer);
        
        // changing the size of value to fit 1 more char
        value = realloc(value, (strlen(value) + strlen(s) + 1) * sizeof(char));

        // now that value is 1 char longer, we append s
        strcat(value, s);

        // go to next char
        lexer_advance(lexer);
    }

    lexer_advance(lexer);

    // return the token with a string value
    return init_token(TOKEN_STRING, value);
}

token_T* lexer_collect_ID(lexer_T* lexer) {

    // allocate memory for a string to use as ID
    char* value = calloc(1, sizeof(char));
    value[0] = '\0';

    while (isalnum(lexer->c)) {
        char* s = lexer_get_current_char_as_string(lexer);
        
        // changing the size of value to fit 1 more char
        value = realloc(value, (strlen(value) + strlen(s) + 1) * sizeof(char));

        // now that value is 1 char longer, we append s
        strcat(value, s);

        // go to next char
        lexer_advance(lexer);
    }

    // return the token with a string value
    return init_token(TOKEN_ID, value);
}

token_T* lexer_advance_with_token(lexer_T* lexer, token_T* token) {
    lexer_advance(lexer);

    return token;
}

char* lexer_get_current_char_as_string(lexer_T* lexer) {
    // create the string with a size of 2 char
    char* str = calloc(2, sizeof(char));

    // set the first value as the char
    str[0] = lexer->c;

    // set the second value as end string char
    str[1] = '\0';

    return str;
}