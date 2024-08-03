#include "execute.h"

#pragma once

enum precedence {LESS_PRECEDENCE, EQUAL_PRECEDENCE, MORE_PRECEDENCE, ACCEPT_PRECEDENCE, ERROR_PRECEDENCE};

enum stack_node_type {TERMINAL, EXPRESSION, ERROR_STACK_NODE};

struct stack_node {
    stack_node_type type = ERROR_STACK_NODE;
    Token terminal;
    Expression * expression = nullptr;
};

/* Stack Class : Contains stack that stores terminals/expressions for operator precedence operations */
class Stack {
    public:
        Stack() {
            stack_node dollar;
            dollar.terminal.token_type = END_OF_FILE;
            dollar.type = TERMINAL;

            stack.push_back(dollar);
        }

        stack_node peek_terminal();
        void push_back(Token);
        void push_back(Expression*);
        void push_back(stack_node);

        stack_node pop_back();
        stack_node back();
        stack_node get(int index);

    private:
        vector<stack_node> stack;
};

/* OperatorPrecedence Class : Responsible for creating operator precedence table and getting precedence values. */
class OperatorPrecedence {
    private:
        void create_table();
        void create_row(string);
        int get_index(TokenType);

        vector<vector<precedence>> table;

    public:
        OperatorPrecedence() {
            create_table();
        }

        precedence get_precedence(TokenType, TokenType);
};