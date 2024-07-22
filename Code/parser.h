#include "lexer.h"

using namespace std;

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

class Parser {
    private:
        LexicalAnalyzer lexer;
        Token expect(TokenType);

    public:
        void syntax_error(int line_number);

        void parse_program();
        void parse_statement_list();
        void parse_statement();
        void parse_assign_statement();
        void parse_print_statement();
};