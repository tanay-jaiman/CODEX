#include "lexer.h"

using namespace std;

#ifndef INSTRUCTION_H
#define INSTRUCTION_H

enum variable_type {
    NUMBER_VAR,
    STRING_VAR
};

struct Variable {
    bool constant = false;
    variable_type type;
    string name;
    string value;
};

enum instruction_type {
    ASSIGN_INSTRUCTION,
    CJMP_INSTRUCTION,
    JMP_INSTRUCTION,
    PRINT_INSTRUCTION,
    NOOP_INSTRUCTION
};

enum boolean_operator {
    EQUAL_TO,
    GREATER_THAN,
    LESSER_THAN,
    GREATER_OR_EQUAL,
    LESSER_OR_EQUAL,
    UNEQUAL
};

struct Expression {
    Token value;
    Expression * left   = nullptr;
    Expression * right  = nullptr;
};

struct Condition {
    boolean_operator oper;
    Expression * left   = nullptr;
    Expression * right  = nullptr;
};

struct assign_instruction {
    Variable var;
    Expression * expression;
};

struct print_instruction {
    vector<string> to_print;
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

    void set_print_instruction(const vector<string> to_print) {
        type = PRINT_INSTRUCTION;
        instruction = print_instruction{to_print};
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

class Program {
    public:
        InstructionNode * create_instruction();
        InstructionNode * create_instruction(instruction_type);

        Expression * create_expression();
        Expression * create_expression(Token value);

        Condition create_condition();
        Condition create_condition(boolean_operator);

        void destroy_all_instructions();
        void destroy_all_expressions();

        void add_instruction(InstructionNode *);

    private:
        vector<InstructionNode*> list_of_all_instructions;
        vector<Expression*> list_of_all_expressions;

        InstructionNode * head = nullptr;
};

#endif