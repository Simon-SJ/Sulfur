#include "include/parser.h"
#include <stdio.h>
#include <string.h>

parser_T* init_parser(lexer_T* lexer) {
    parser_T* parser = calloc(1, sizeof(struct PARSER_STRUCT));
    parser->lexer = lexer;
    parser->current_token = lexer_get_next_token(lexer);

    return parser;
}

void parser_eat(parser_T* parser, int token_type) {

    // if current token is equals to expected token, then go next
    if (parser->current_token->type == token_type) {
        parser->current_token = lexer_get_next_token(parser->lexer);
    } else {
        // else throw an error
        printf(
            "Unexpected token! `%s`, with type %d", 
            parser->current_token->value, 
            parser->current_token->type
        );
        exit(1);
    }
}

AST_T* parser_parse(parser_T* parser) {
    return parser_parse_statements(parser);
}

AST_T* parser_parse_statement(parser_T* parser) {
    switch (parser->current_token->type) {
        case TOKEN_ID: return parser_parse_id(parser); break;
    }
}

AST_T* parser_parse_statements(parser_T* parser) {
    // creating a compund type ast node
    AST_T* compound = init_ast(AST_COMPOUND);

    // init the size
    compound->compound_value = calloc(1, sizeof(struct AST_STRUCT*));

    // parse the first statement and adding it to our ast node list
    AST_T* ast_statement = parser_parse_statement(parser);
    compound->compound_value[0] = ast_statement;
    compound->compound_size += 1;

    // while a simi colon is expected do
    while (parser->current_token->type == TOKEN_SEMI) {
        
        // check it the right id
        parser_eat(parser, TOKEN_SEMI);

        // parse the next statement and add it to the end of the ast node list
        AST_T* ast_statement = parser_parse_statement(parser);
        compound->compound_size += 1;
        compound->compound_value = realloc(
            compound->compound_value, 
            compound->compound_size * sizeof(struct AST_STRUCT*)
        );
        compound->compound_value[compound->compound_size-1] = ast_statement;
    }

    return compound;
} 

AST_T* parser_parse_expr(parser_T* parser) {
    switch (parser->current_token->type) {
        case TOKEN_STRING: return parser_parse_string(parser);
    }
}

AST_T* parser_parse_factor(parser_T* parser) {

}

AST_T* parser_parse_term(parser_T* parser) {

}

AST_T* parser_parse_function_call(parser_T* parser) {
    
}

AST_T* parser_parse_variable_definition(parser_T* parser) {
    // first thing is "var" so we use eat on it
    parser_eat(parser, TOKEN_ID);
    
    // now we get the "name"
    char* variable_definition_variable_name = parser->current_token->value;
    parser_eat(parser, TOKEN_ID);

    // we dont need "=" for anything so check its there and skip
    parser_eat(parser, TOKEN_EQUALS);

    // we get the variable value
    AST_T* variable_definition_value = parser_parse_expr(parser);

    // create a new variable def
    AST_T* variable_definition = init_ast(AST_VARIABLE_DEFINITION);

    // populize the new var with our data
    variable_definition->variable_definition_variable_name = variable_definition_variable_name;
    variable_definition->variable_definition_value = variable_definition_value;

    return variable_definition;
}

AST_T* parser_parse_variable(parser_T* parser) {
    char* token_value = parser->current_token->value;

    // first one is gonna be a var name or function name
    parser_eat(parser, TOKEN_ID); 

    if (parser->current_token->type == TOKEN_LPAREN) {
        return parser_parse_function_call(parser);
    }

    AST_T* ast_variable = init_ast(AST_VARIABLE);
    ast_variable->variable_name = token_value;

    return ast_variable;
}

AST_T* parser_parse_string(parser_T* parser) {
    AST_T* ast_string = init_ast(AST_STRING);
    ast_string->string_value = parser->current_token->value;

    parser_eat(parser, TOKEN_STRING);

    return ast_string;
}

AST_T* parser_parse_id(parser_T* parser) {
    if (strcmp(parser->current_token->value, "var") == 0) {
        return parser_parse_variable_definition(parser);
    } else {
        return parser_parse_variable(parser);
    }
}
