#include "include/lexer.h"
#include <stdio.h>

int main(int argc, char* argv[]) {
    //printf("Hello World!\n");

    lexer_T* lexer = init_lexer(
        "var name = \"Fin S. Ikke\";\n"
        "print(name);\n"
    );

    token_T* token = (void*)0;

    while ((token = lexer_get_next_token(lexer)) != (void*)0) {
        printf("TOKEN(%d, %s)\n", token->type, token->value);
    }

    return 0;
}