#include "instruction.h"

#pragma once

struct type_error
{
    int line_number;
};

class Execute
{
public:
    void execute_program();
    Execute(Program p) { program = p; }

private:
    Program program;
    vector<InstructionNode*> break_instructions;
    InstructionNode *program_counter = nullptr;

    void type_check();
    void throw_type_error(type_error);

    variable_value resolve_expression(Expression *);

    bool resolve_condition_helper(variable_value, variable_value, boolean_operator);
    bool resolve_condition(Condition);

    void execute_instruction_node(InstructionNode *);
    void execute_assign_instruction(assign_instruction);
    void execute_print_instruction(print_instruction);
    void execute_cjmp_instruction(cjmp_instruction);
    void execute_jmp_instruction(jmp_instruction);
    void execute_break_instruction();
};