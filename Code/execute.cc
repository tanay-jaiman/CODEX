#include "execute.h"
#include <format>

#define DEBUG 0
#define VAR_DEBUG 0

void Execute::execute_program()
{
    if (DEBUG) printf("starting program execution\n");
    program_counter = program.get_head();
    while (program_counter)
    {
        if (break_instructions.size() > 0 && program_counter == break_instructions.back())
            break_instructions.pop_back();
        execute_instruction_node(program_counter);
    }
    if (DEBUG) printf("completed program execution\n");
}

void Execute::execute_instruction_node(InstructionNode *ins)
{
    instruction_data instruction = ins->instruction;
    switch (ins->type)
    {
    case ASSIGN_INSTRUCTION:
        execute_assign_instruction(*get_if<assign_instruction>(&instruction));
        break;

    case CJMP_INSTRUCTION:
        execute_cjmp_instruction(*get_if<cjmp_instruction>(&instruction));
        break;

    case JMP_INSTRUCTION:
        execute_jmp_instruction(*get_if<jmp_instruction>(&instruction));
        break;

    case PRINT_INSTRUCTION:
        execute_print_instruction(*get_if<print_instruction>(&instruction));
        break;

    case BREAK_INSTRUCTION:
        execute_break_instruction();
        break;

    case NOOP_INSTRUCTION:
        program_counter = ins->next;
        return;
    }
}

void Execute::execute_assign_instruction(assign_instruction assign)
{
    Variable var = program.find_variable(assign.var.token.lexeme);
    // If variable is undefined then give it a value
    if (DEBUG) printf("starting assign instruction execution\n");
    if (var.type == UNDEFINED_VAR || var.constant == false)
    {
        var.value = resolve_expression(assign.expression);

        if (holds_alternative<string>(assign.var.value))
        {
            var.type = STRING_VAR;
        }

        else
        {
            var.type = NUMBER_VAR;
        }

        program.list_of_variables[program.get_variable_index(var.token.lexeme)] = var;
    }

    // else if variable is initialized (not undefined) and a constant, throw error
    else
    {
        printf("Constant variable \"%s\" already engraved on line number : %d\n", assign.var.token.lexeme.c_str(), assign.var.token.line_number);
        exit(1);
    }

    program_counter = program_counter->next;
    if (DEBUG) printf("completed assign instruction execution\n");
}

void Execute::execute_cjmp_instruction(cjmp_instruction cjmp)
{
    if (DEBUG) printf("starting cjmp execution\n");

    bool condition_result = resolve_condition(cjmp.condition);
    if (cjmp.is_while)
        break_instructions.push_back(cjmp.instruction);

    if (condition_result == false)
        program_counter = cjmp.instruction;

    else
        program_counter = program_counter->next;
    
    if (DEBUG) printf("completed cjmp execution\n");
}

void Execute::execute_jmp_instruction(jmp_instruction jmp)
{
    program_counter = jmp.instruction;
}

void Execute::execute_break_instruction() {
    if (break_instructions.size() > 0) {
        program_counter = break_instructions.back();
        break_instructions.pop_back();
    }
}

void Execute::execute_print_instruction(print_instruction print) {
    if (DEBUG) printf("starting print execution\n");
    auto value = resolve_expression(print.expression);

    if (holds_alternative<string>(value)) {
        printf("%s\n", get_if<string>(&value)->c_str());
    }

    else if (holds_alternative<float>(value)) {
        printf("%.2f\n", get<float>(value));
    }

    program_counter = program_counter->next;
    if (DEBUG) printf("complete print execution\n");
}

bool Execute::resolve_condition_helper(variable_value left, variable_value right, boolean_operator op) {
    if (holds_alternative<string>(left) && holds_alternative<string>(right)) {
        switch (op) {
            case EQUAL_TO:
                return strcmp(get<string>(left).c_str(), get<string>(right).c_str()) == 0;

            case UNEQUAL:
                return strcmp(get<string>(left).c_str(), get<string>(right).c_str()) != 0; 

            case GREATER_THAN:
                return strcmp(get<string>(left).c_str(), get<string>(right).c_str()) > 0;

            case LESSER_THAN:
                return strcmp(get<string>(left).c_str(), get<string>(right).c_str()) < 0;

            case GREATER_OR_EQUAL:
                return strcmp(get<string>(left).c_str(), get<string>(right).c_str()) >= 0;

            case LESSER_OR_EQUAL:
                return strcmp(get<string>(left).c_str(), get<string>(right).c_str()) <= 0;
        }
    }

    if (holds_alternative<float>(left) && holds_alternative<float>(right)) {
        switch(op) {
            case EQUAL_TO:
                return get<float>(left) == get<float>(right);
            
            case UNEQUAL:
                return get<float>(left) != get<float>(right);

            case GREATER_THAN:
                return get<float>(left) > get<float>(right);

            case LESSER_THAN:
                return get<float>(left) < get<float>(right);

            case GREATER_OR_EQUAL:
                return get<float>(left) >= get<float>(right);

            case LESSER_OR_EQUAL:
                return get<float>(left) <= get<float>(right);
        }
    }
}

bool Execute::resolve_condition(Condition condition) {
    if (DEBUG) printf("starting resolve condition\n");
    auto left_val = resolve_expression(condition.left);
    // if (VAR_DEBUG) printf("%s\n", get<string>(left_val).c_str());
    auto right_val = resolve_expression(condition.right);
    // if (VAR_DEBUG) printf("%s\n", get<string>(right_val).c_str());

    if (holds_alternative<string>(left_val) && holds_alternative<float>(right_val)) {
        right_val = to_string(get<float>(right_val));
    }

    else if (holds_alternative<float>(left_val) && holds_alternative<string>(right_val)) {
        left_val = to_string(get<float>(left_val));
    }

    return resolve_condition_helper(left_val, right_val, condition.oper);

    if (DEBUG) printf("completed resolve condition.\n");
}

variable_value Execute::resolve_expression(Expression * expr) {
    if (DEBUG) printf("starting resolve expression\n");
    variable_value output;
    if (expr->left == nullptr && expr->right == nullptr) {
        if (expr->type == STRING_EXPR) {
            output = expr->value.lexeme;
        }

        else if (expr->type == NUM_EXPR) {
            output = (float) atof(expr->value.lexeme.c_str());
            // if (VAR_DEBUG) printf("%f\n", get<float>(output));
        }

        else if (expr->type == VAR_EXPR) {
            output = program.find_variable(expr->value.lexeme).value;
            // if (VAR_DEBUG) printf("%s\n", get<string>(output).c_str());
        }

        // if (DEBUG) printf("%s\n", get<string>(output).c_str());
        if (DEBUG) printf("completed resolve expression\n");

        return output;
    }

    if (expr->type == OP_EXPR) {
        auto left = resolve_expression(expr->left);
        if (DEBUG) printf("%s\n", get<string>(left).c_str());
        auto right = resolve_expression(expr->right);
        if (DEBUG) printf("%s\n", get<string>(right).c_str());
        switch(expr->value.token_type) {
            // addition is allowed between all. string + number => string 
            case PLUS:
                if (holds_alternative<string>(left) && holds_alternative<float>(right))
                    right = to_string(get<float>(right));

                else if (holds_alternative<float>(left) && holds_alternative<string>(right)) {
                    left = to_string(get<float>(left));
                }

                if (holds_alternative<string>(left) && holds_alternative<string>(right))
                    output = get<string>(left) + get<string>(right);

                else if (holds_alternative<float>(left) && holds_alternative<float>(right))
                    output = get<float>(left) + get<float>(right);

                break;

            case MINUS:
                if (holds_alternative<float>(left) && holds_alternative<float>(right))
                    output = get<float>(left) - get<float>(right);

                else 
                    throw_type_error({expr->value.line_number});

                break;

            case MULT:
                if (holds_alternative<float>(left) && holds_alternative<float>(right))
                    output = get<float>(left) * get<float>(right);

                else 
                    throw_type_error({expr->value.line_number});

                break;

            case DIV:
                if (holds_alternative<float>(left) && holds_alternative<float>(right))
                    output = get<float>(left) / get<float>(right);

                else 
                    throw_type_error({expr->value.line_number});

                break;
        }
    }

    return output;
    if (DEBUG) printf("completed resolve expression\n");
}

void Execute::throw_type_error(type_error err) {
    printf("Type error on line number: %d\n", err.line_number);
    exit(1);
}