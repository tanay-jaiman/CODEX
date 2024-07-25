#include "parser.h"

using namespace std;

#define DEBUG 1

void Parser::syntax_error(int line_number) {
    printf("Syntax Error found on line number : %d\n", line_number);
    exit(1);
}

Token Parser::expect(TokenType t) {
    if (lexer.peek_token(0).token_type != t) 
        syntax_error(lexer.peek_token(0).line_number);

    Token a = lexer.get_token();

    if (comment_found())
        skip_comment();

    return a;
}

bool Parser::comment_found() {
    if (lexer.peek_token(0).token_type == DIV && lexer.peek_token(1).token_type == DIV)
        return true;

    return false;
}

void Parser::skip_comment() {
    if (DEBUG) printf("Comment found.\n");
    Token t = lexer.get_token();
    while (lexer.peek_token(0).line_number == t.line_number && lexer.peek_token(0).token_type != END_OF_FILE) {
        lexer.get_token();
    }

    if (comment_found())
        skip_comment();
}

void Parser::parse_program() {
    if (comment_found())
        skip_comment();
    parse_statement_list();
    expect(END_OF_FILE);

    if (DEBUG) printf("NO SYNTAX ERRORS FOUND.\n");
}

void Parser::parse_statement_list() {
    parse_statement();

    switch (lexer.peek_token(0).token_type) {
        case PROCLAIM:
            parse_statement_list();
            break;

        case ENGRAVE:
            parse_statement_list();
            break;

        case ANNOUNCE:
            parse_statement_list();
            break;

        case CONSIDER:
            parse_statement_list();
            break;

        case WHILST:
            parse_statement_list();
            break;

        case THOU:
            parse_statement_list();
            break;

        case ID:
            parse_statement_list();
            break;
    }
}

void Parser::parse_statement() {
   switch (lexer.peek_token(0).token_type) {
        case PROCLAIM:
            parse_assign_statement();
            break;

        case ENGRAVE:
            parse_assign_statement();
            break;

        case ANNOUNCE:
            parse_print_statement();
            break;

        case CONSIDER:
            parse_if_statement();
            break;

        case WHILST:
            parse_while_statement();
            break;

        case THOU:
            parse_break_statement();
            break;

        case END_OF_FILE:
            return;

        case ID:
            parse_assign_statement();
            break;

        default:
            syntax_error(lexer.peek_token(0).line_number);
    }

    if (comment_found())
        skip_comment();
}

void Parser::parse_assign_statement() {
    if (DEBUG) printf("Assign statement parsing started\n");
    bool variable_exists = false;
    bool constant = false;
    if (lexer.peek_token(0).token_type == PROCLAIM)
        expect (PROCLAIM);

    else if (lexer.peek_token(0).token_type == ENGRAVE) {
        expect (ENGRAVE);
        constant = true;
    }

    else 
        variable_exists = true;

    Variable var = parse_id(variable_exists, constant); 

    expect (EQUAL);

    Expression * expression = parse_expression(); 

    // Add the instruction
    InstructionNode * instruction = program.create_instruction(ASSIGN_INSTRUCTION);
    instruction->set_assign_instruction(var, expression);
    program.add_instruction(instruction);

    expect (SEMICOLON);

    // skip comments
    if (comment_found())
        skip_comment();

    if (DEBUG) printf("Assign statement parsing complete.\n");
}

// print_statement := ANNOUNCE <expression> SEMICOLON
void Parser::parse_print_statement() {
    if (DEBUG) printf("Print statement parsing started.\n");

    expect(ANNOUNCE);
    Expression * expression = parse_expression();

    InstructionNode * instruction = program.create_instruction(PRINT_INSTRUCTION);
    instruction->set_print_instruction(expression);
    program.add_instruction(instruction);

    expect(SEMICOLON);

    if (DEBUG) printf("Print statement parsing complete.\n");
}

/*
    CONSIDER (<CONDITION>) { <STATEMENT_LIST> } [OTHERWISE { <STATEMENT_LIST> }]
*/
void Parser::parse_if_statement() {
    if (DEBUG) printf("If statement parsing started.\n");

    expect(CONSIDER);
    expect(LPAREN);

    InstructionNode * end_of_if = program.create_instruction(NOOP_INSTRUCTION);
    
    Condition condition = parse_condition();
    InstructionNode * condition_instruction = program.create_instruction(CJMP_INSTRUCTION);
    condition_instruction->set_cjmp_instruction(condition, end_of_if);
    program.add_instruction(condition_instruction);

    expect(RPAREN);
    expect(LBRACE);

    parse_statement_list();

    InstructionNode * end_of_consider = program.create_instruction(JMP_INSTRUCTION);
    end_of_consider->set_jmp_instruction(end_of_if);

    expect(RBRACE);

    if (lexer.peek_token(0).token_type == OTHERWISE) {
        expect(OTHERWISE);
        expect(LBRACE);

        InstructionNode * start_of_otherwise = program.create_instruction(NOOP_INSTRUCTION);
        program.add_instruction(start_of_otherwise);

        condition_instruction->set_cjmp_instruction(condition, start_of_otherwise);

        parse_statement_list();

        expect(RBRACE);
    }

    program.add_instruction(end_of_if);
    if (DEBUG) printf("If statement parsing complete.\n");
}

/*
    WHILST (<CONDITION>) { <STATEMENT_LIST> }
*/
void Parser::parse_while_statement() {
    if (DEBUG) printf("While parsing started.\n");
    expect(WHILST);

    expect(LPAREN);

    InstructionNode * end_of_while = program.create_instruction(NOOP_INSTRUCTION);

    Condition condition = parse_condition();
    InstructionNode * condition_instruction = program.create_instruction(CJMP_INSTRUCTION);
    condition_instruction->set_cjmp_instruction(condition, end_of_while);

    program.add_instruction(condition_instruction);

    expect(RPAREN);

    expect(LBRACE);

    parse_statement_list();

    expect(RBRACE);

    if (DEBUG) printf("While parsing completed.\n");

    program.add_instruction(end_of_while);
    if (DEBUG) printf("While parsing completed.\n");
}

/*
    THOU SHALT NOT PASS SEMICOLON
*/
void Parser::parse_break_statement() {
    expect(THOU);
    expect(SHALT);
    expect(NOT);
    expect(PASS);
    expect(SEMICOLON);

    InstructionNode * break_statement = program.create_instruction(BREAK_INSTRUCTION);
    program.add_instruction(break_statement);
}

/* <EXPRESSION> <BOOLEAN_OPERATOR> <EXPRESSION> */
Condition Parser::parse_condition() {
    Condition output = Condition();
    output.left = parse_expression();
    output.oper = parse_condition_operator();
    output.right = parse_expression();
}

boolean_operator Parser::parse_condition_operator() {
    TokenType t = lexer.peek_token(0).token_type;

    switch (t) {
        case EQUAL:
            expect (EQUAL);
            expect (EQUAL);
            return EQUAL_TO;

        case LANG:
            expect (LANG);

            if (lexer.peek_token(0).token_type == EQUAL) {
                expect(EQUAL);
                return LESSER_OR_EQUAL;
            }

            return LESSER_THAN;

        case RANG:
            expect(RANG);

            if (lexer.peek_token(0).token_type == EQUAL) {
                expect (EQUAL);
                return GREATER_OR_EQUAL;
            }

            return GREATER_THAN;

        case EXCLAMATION:
            expect(EXCLAMATION);
            expect(EQUAL);

            return UNEQUAL;
    }
}

Variable Program::find_variable(string var_name) {
    Variable output = Variable();
    
    for (int i = 0; i < list_of_variables.size(); i++) {
        if (list_of_variables[i].token.lexeme == var_name) {
            output = list_of_variables[i];
        }
    }

    return output;
}

void Program::add_variable(Variable var) {
    list_of_variables.push_back(var);
}

void Parser::throw_declaration_error(int type, Token token) {
    if (type == 1) {
        Variable var = program.find_variable(token.lexeme);
        printf("Variable %s already declared on line number: %d!\n", token.lexeme.c_str(), var.token.line_number);
    }

    if (type == 2) {
        printf("Variable %s used on line number %d was never declared!\n", token.lexeme.c_str(), token.line_number);
    }

    exit(1);
}

/*
    - If a variable is being declared and it doesn't exist, create it
    - If a variable is being declared and it already exists, error (T1)
    - If a variable is being accessed and it exists, return it
    - If a variable is being accessed and it doens't exist, error (T2)

    TODO:
        Currently error throwing gives no information about where the error occurred or how it can be fixed. That needs to be fixed for appropriate debug information. Top priority before Operator Precedence and parse_expression is started. 

        This would probably require optimizing instruction.h and making the structures more robust.

        The whole scenario regarding Variables and literals is a bit dodgy. Also needs to be decided if variable assignment is calculated at runtime or parse time?
*/
Variable Parser::parse_id(bool variable_exists, bool constant) {
    if (DEBUG) printf("ID parsing started.\n");
    Token t = expect(ID);
    Variable var = program.find_variable(t.lexeme);;

    if (variable_exists) {
        if (var.type == ERROR_VAR) {
            throw_declaration_error(2, t);
        }
    }

    else {
        if (var.type != ERROR_VAR) {
            throw_declaration_error(1, t);
        }

        else {
            var = {constant, UNDEFINED_VAR, t, ""};
            program.add_variable(var);
        }
    }

    if (DEBUG) printf("ID parsing completed.\n");
    return var;
}

Token Parser::parse_number() {
    return expect(NUMBER);
}

Token Parser::parse_string() {
    if (DEBUG) printf("String parsing started.\n");

    expect(QUOTES);
    Token t = expect(STRING_LITERAL);
    expect(QUOTES);

    if (DEBUG) printf("String parsing completed.\n");

    return t;
}

// temporary implementation of parse_expression
Expression * Parser::parse_expression() {
    if (DEBUG) printf("Expression parsing started.\n");
    Token t = lexer.peek_token(0);
    Expression * output;

    switch (t.token_type) {
        case LPAREN:
            expect(LPAREN);
            output = parse_expression();
            expect(RPAREN);
            break;

        case QUOTES:
            parse_string();
            break;

        case ID:
            parse_id(true, false);
            break;

        case NUMBER:
            parse_number();
            break;
    }

    t = lexer.peek_token(0);

    if (t.token_type == LANG || t.token_type == RANG || t.token_type == EQUAL || t.token_type == EXCLAMATION || t.token_type == SEMICOLON || t.token_type == RPAREN) {
        if (DEBUG) printf("Expression parsing completed.\n");
        return output;
    }

    else {
        lexer.get_token();
        parse_expression();
    }

    if (DEBUG) printf("Expression parsing completed.\n");
    return output;
}

int main() {
    Parser parser;

    if (DEBUG) printf("Starting Parsing.\n");

    parser.parse_program();

    if (DEBUG) printf("Parsing complete.\n");
    return 0;
}