#include "instruction.h"

using namespace std;

/* ----- CREATE INSTRUCTION ----- */
InstructionNode * Program::create_instruction()
{
    InstructionNode *output = (InstructionNode *)malloc(sizeof(InstructionNode));

    list_of_all_instructions.push_back(output);

    return output;
}

InstructionNode * Program::create_instruction(instruction_type type)
{
    InstructionNode *output = (InstructionNode *)malloc(sizeof(InstructionNode));

    output->type = type;

    list_of_all_instructions.push_back(output);

    return output;
}
/* ----- CREATE INSTRUCTION ----- */

/* ----- CREATE EXPRESSION ----- */
Expression * Program::create_expression()
{
    Expression *expression = (Expression *)malloc(sizeof(Expression));

    list_of_all_expressions.push_back(expression);

    return expression;
}

Expression * Program::create_expression(expression_type type, Token value) {
    Expression * expression = (Expression *) malloc(sizeof(Expression));

    (*expression).type = type;
    (*expression).value = value;
    list_of_all_expressions.push_back(expression);

    return expression;
}
/* ----- CREATE EXPRESSION ----- */

/* ----- CREATE CONDITION ------ */
Condition Program::create_condition() {
    return Condition();
}

Condition Program::create_condition(boolean_operator op) {
    Condition output = Condition();
    output.oper = op;

    return output;
}
/* ----- CREATE CONDITION ----- */

/* ----- DESTROY POINTERS ------ */
void Program::destroy_all_instructions() {
    for (int i = 0; i < list_of_all_instructions.size(); i++) {
        delete(list_of_all_instructions[i]);
    }
}

void Program::destroy_all_expressions() {
    for (int i = 0; i < list_of_all_expressions.size(); i++) {
        delete(list_of_all_expressions[i]);
    }
}
/* ----- DESTROY POINTERS ------ */

void Program::add_instruction(InstructionNode * instruction) {
    if (!head) {
        head = instruction;
        return;
    }

    InstructionNode * iter = head;
    while (iter->next != nullptr) {
        iter = iter->next;
    }

    iter->next = instruction;
}