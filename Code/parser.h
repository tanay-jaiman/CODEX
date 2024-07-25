#include "instruction.h"

#ifndef PARSER_H
#define PARSER_H

#pragma once

using namespace std;

class Parser {
    private:
        LexicalAnalyzer lexer;
        Token expect(TokenType);

        Program program = Program();

    public:
        void syntax_error(int line_number);
        void throw_declaration_error(int type, Token token);

        void parse_program();
        void parse_statement_list();
        void parse_statement();
        void parse_assign_statement();
        void parse_print_statement();
        void parse_if_statement();
        void parse_while_statement();
        void parse_break_statement();
        
        bool comment_found();
        void skip_comment();

        Expression * parse_expression();

        Token parse_number();
        Token parse_string();

        // parse_id is responsible for adding the variables
        Variable parse_id(bool variable_exists, bool constant);

        Condition parse_condition();
        boolean_operator parse_condition_operator();
};

#endif