#include "instruction.h"
#include "operator_precedence.h"

#ifndef PARSER_H
#define PARSER_H

#pragma once

using namespace std;

/* Parser class : Responsible for consuming tokens stored by LexicalAnalyzer and parse the program to store execution information like variables declared, creating abstract syntax trees of expressions. */
class Parser {
    private:
        LexicalAnalyzer lexer;
        Token expect(TokenType);

        Program program = Program();
        OperatorPrecedence table = OperatorPrecedence();

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

        Variable parse_id(bool being_declared, bool constant);

        Condition parse_condition();
        boolean_operator parse_condition_operator();

        stack_node reduce_candidate(vector<stack_node>);
};

#endif