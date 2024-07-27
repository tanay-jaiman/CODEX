#include "lexer.h"

using namespace std;

#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#pragma once

/* Allowed variable types */
enum variable_type {
    NUMBER_VAR,
    STRING_VAR, 
    ERROR_VAR,
    UNDEFINED_VAR
};

using variable_value = variant<string, float>; // Variable can store either a string or a float

struct Variable {
    bool constant = false;
    variable_type type = ERROR_VAR;
    Token token;
    variable_value value;
};

enum instruction_type {
    ASSIGN_INSTRUCTION,
    CJMP_INSTRUCTION,
    JMP_INSTRUCTION,
    PRINT_INSTRUCTION,
    NOOP_INSTRUCTION,
    BREAK_INSTRUCTION
};

enum boolean_operator {
    EQUAL_TO,
    GREATER_THAN,
    LESSER_THAN,
    GREATER_OR_EQUAL,
    LESSER_OR_EQUAL,
    UNEQUAL
};

enum expression_type {
    NUM_EXPR, STRING_EXPR, VAR_EXPR, OP_EXPR, ERROR_EXPR
};

// using expr_value = variant<Token, float>;

struct Expression {
    expression_type type;
    Token value;
    Expression * left   = nullptr;
    Expression * right  = nullptr;
};

struct Condition {
    boolean_operator oper;
    Expression * left   = nullptr;
    Expression * right  = nullptr;
};

struct InstructionNode;

struct assign_instruction {
    Variable var;
    Expression * expression;
};

struct print_instruction {
    Expression * expression;
};

struct cjmp_instruction {
    Condition condition;
    InstructionNode * instruction;
};

struct jmp_instruction {
    InstructionNode * instruction;
};

using instruction_data = variant<assign_instruction, print_instruction, cjmp_instruction, jmp_instruction>;

struct InstructionNode {
    instruction_type type;
    InstructionNode * next = nullptr;
    instruction_data instruction;

    InstructionNode() = default;
    ~InstructionNode() = default;

    void set_assign_instruction(const Variable &var, Expression * expression) {
        type = ASSIGN_INSTRUCTION;
        instruction = assign_instruction{var, expression};
    }

    void set_print_instruction(Expression * expression) {
        type = PRINT_INSTRUCTION;
        instruction = print_instruction{expression};
    }

    void set_cjmp_instruction(const Condition condition, InstructionNode * ins) {
        type = CJMP_INSTRUCTION;
        instruction = cjmp_instruction{condition, ins};
    }

    void set_jmp_instruction(InstructionNode * ins) {
        type = JMP_INSTRUCTION;
        instruction = jmp_instruction{ins};
    }
};

struct TypeError {
    int line_number;
    expression_type left;
    expression_type right;
};

class Program {
    public:
        InstructionNode * create_instruction();
        InstructionNode * create_instruction(instruction_type);

        Expression * create_expression();
        Expression * create_expression(expression_type type, Token value);

        Condition create_condition();
        Condition create_condition(boolean_operator);

        void destroy_all_instructions();
        void destroy_all_expressions();

        void add_instruction(InstructionNode *);

        void add_variable(Variable);
        Variable find_variable(string);

        void add_type_error(TypeError);

    private:
        vector<InstructionNode*> list_of_all_instructions;

        vector<Expression*> list_of_all_expressions;

        vector<Variable> list_of_variables;

        vector<TypeError> list_of_type_errors;

        InstructionNode * head = nullptr;
};

#endif