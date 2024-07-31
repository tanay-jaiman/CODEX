#include "operator_precedence.h"

using namespace std;

#define DEBUG 1

stack_node Stack::peek_terminal() {
    if (stack.back().type == TERMINAL)
        return stack.back();

    else if (stack[stack.size() - 2].type == TERMINAL)
        return stack[stack.size() - 2];

    else {
        return stack_node();
    }
}

stack_node Stack::pop_back() {
    stack_node t = stack.back();
    stack.pop_back();
    return t;
}

void Stack::push_back(Token t) {
    stack_node sn;
    sn.terminal = t;
    sn.type = TERMINAL;

    stack.push_back(sn);
}

void Stack::push_back(Expression* expr) {
    stack_node sn;
    sn.expression = expr;
    sn.type = EXPRESSION;

    stack.push_back(sn);
}

stack_node Stack::back() {
    return stack.back();
}

void OperatorPrecedence::print_table() {
    for (int i = 0; i < table.size(); i++) {
        for (int j = 0; j < table[i].size(); j++) {
            cout << table[i][j] << " : ";
        }

        cout << endl;
    }
}

void OperatorPrecedence::create_table() {
    create_row("01000040");
    create_row("42111114");
    create_row("02220020");
    create_row("02220020");
    create_row("02222220");
    create_row("02222220");
    create_row("04000030");
    create_row("42222224");
}

void OperatorPrecedence::create_row(string row) {
    vector<precedence> table_row;
    for (int i = 0; i < row.length(); i++) {
        char c = row[i];
        table_row.push_back((precedence) atoi(&c));
    }

    table.push_back(table_row);
}

int OperatorPrecedence::get_index(TokenType t) {
    switch(t) {
        case LPAREN:
            return 0;

        case RPAREN:
            return 1;

        case PLUS:
            return 2;

        case MINUS:
            return 3;

        case MULT:
            return 4;

        case DIV:
            return 5;

        case END_OF_FILE:
            return 6;

        case ID:
            return 7;

        case NUMBER:
            return 7;

        case STRING_LITERAL:
            return 7;
    }

    return 8;
}

precedence OperatorPrecedence::get_precedence(TokenType t1, TokenType t2) {
    if (DEBUG) printf("precedence - %d & %d\n", get_index(t1), get_index(t2));
    return table[get_index(t1)][get_index(t2)];
}

stack_node Stack::get(int index) {
    if (index < stack.size())
        return stack[index];
}