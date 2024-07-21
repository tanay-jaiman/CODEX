#include "lexer.h"
#include <cctype>

#define DEBUG 0

using namespace std;

string to_lower(string s) {
    string output = "";
    for (int i = 0; i < s.length(); i++) {
        output += tolower(s[i]);
    }

    return output;
}

void Token::printToken() {
    printf("Lexeme: %s, Type: %d, Line: %d\n", lexeme.c_str(), token_type, line_number);
}

void LexicalAnalyzer::tokenify_program() {
    Line current_line = input_buffer.get_line();
    while (current_line.line_number != -1) {
        tokenify_line(current_line);
        current_line = input_buffer.get_line();
    }
}

void LexicalAnalyzer::tokenify_line(Line current_line) {
    int index = 0;
    while (index < current_line.line.length()) {
        index = get_token_main(current_line, index);
    }
}

int LexicalAnalyzer::get_token_main(Line current_line, int index) {
    string line = current_line.line;
    Token t;

    Token quote = Token();
    quote.lexeme = "\"";
    quote.line_number = current_line.line_number;
    quote.token_type = QUOTES;

    if (DEBUG) printf("Start of get_token_main()\n");

    if (line[index] == ' ') {
        if (DEBUG) printf("Space encountered.\n");
        return index + 1;
    }

    else if (line[index] == '\"') {
        if (DEBUG) printf("String encountered.\n");
        t = scan_string(current_line, index);
        token_list.push_back(quote);
        token_list.push_back(t);
        if (t.token_type == STRING_LITERAL)
            token_list.push_back(quote);
        return index + t.lexeme.length() + 2;
    }

    else if (isalpha(line[index])) {
        if (DEBUG) printf("ID or Keyword encountered.\n");
        t = scan_id_or_keyword(current_line, index);
        token_list.push_back(t);
        return index + t.lexeme.length();
    }

    else if (isdigit(line[index])) {
        if (DEBUG) printf("Number encountered.\n");
        t = scan_number(current_line, index);
        token_list.push_back(t);
        return index + t.lexeme.length();
    }

    else {
        if (DEBUG) printf("Symbol encountered.\n");
        t = scan_symbol(current_line, index);
        token_list.push_back(t);
        return index + 1;
    }
}

Token LexicalAnalyzer::scan_string(Line current_line, int index) {
    string line = current_line.line;
    Token output = Token();
    output.line_number = current_line.line_number;

    if (line[index] == '\"') {
        // token_list.push_back(quote);
        index++;
    }

    else return output;

    while (line[index] != '\"' && index < line.length()) {
        output.lexeme += line[index];
        index++;
    }

    if (index >= line.length()) {
        output.token_type = ERROR;
        return output;
    }

    else if (line[index] == '\"') {
        // token_list.push_back(quote);
        output.token_type = STRING_LITERAL;
        index++;
    }

    return output;
}

Token LexicalAnalyzer::scan_id_or_keyword(Line current_line, int index) {
    string line = current_line.line;
    Token output = Token();
    output.line_number = current_line.line_number;

    while (isalpha(line[index])) {
        output.lexeme += line[index];
        index++;
    }

    if (to_lower(output.lexeme) == "proclaim") {
        output.token_type = PROCLAIM;
    }

    else if (to_lower(output.lexeme) == "engrave") {
        output.token_type = ENGRAVE;
    }

    else if (to_lower(output.lexeme) == "announce") {
        output.token_type = ANNOUNCE;
    }

    else if (to_lower(output.lexeme) == "consider") {
        output.token_type = CONSIDER;
    }

    else if (to_lower(output.lexeme) == "otherwise") {
        output.token_type = OTHERWISE;
    }

    else if (to_lower(output.lexeme) == "whilst") {
        output.token_type = WHILST;
    }

    else if (to_lower(output.lexeme) == "thou") {
        output.token_type = THOU;
    }

    else if (to_lower(output.lexeme) == "shalt") {
        output.token_type = SHALT;
    }

    else if (to_lower(output.lexeme) == "not") {
        output.token_type = NOT;
    }

    else if (to_lower(output.lexeme) == "pass") {
        output.token_type = PASS;
    }

    else {
        output.token_type = ID;
    }

    return output;
}

Token LexicalAnalyzer::scan_number(Line current_line, int index) {
    string line = current_line.line;
    Token output = Token();
    output.line_number = current_line.line_number;

    if (isdigit(line[index])) {
        output.token_type = NUMBER;
    }

    else return output;

    while (isdigit(line[index])) {
        output.lexeme += line[index];
        index++;
    }

    return output;
}

Token LexicalAnalyzer::scan_symbol(Line current_line, int index) {
    string line = current_line.line;
    Token output = Token();
    output.line_number = current_line.line_number;

    switch (line[index]) {
        case '=':
            output.token_type = EQUAL;
            break;

        case '+':
            output.token_type = PLUS;
            break;

        case '-':
            output.token_type = MINUS;
            break;

        case '*':
            output.token_type = MULT;
            break;

        case '/':
            output.token_type = DIV;
            break;

        case ';':
            output.token_type = SEMICOLON;
            break;

        case '(':
            output.token_type = LPAREN;
            break;

        case ')':
            output.token_type = RPAREN;
            break;

        case '{':
            output.token_type = LBRACE;
            break;

        case '}':
            output.token_type = RBRACE;
            break;

        case '<':
            output.token_type = LANG;
            break;

        case '>':
            output.token_type = RANG;
            break;

        case '!':
            output.token_type = EXCLAMATION;
            break;

        case '\"':
            output.token_type = QUOTES;
            break;

        case '.':
            output.token_type = DOT;
            break;

        default:
            return output;
    }

    output.lexeme = line[index];
    return output;
}

Token LexicalAnalyzer::get_token() {
    Token output = Token();
    output.token_type = END_OF_FILE;
    if (token_index < token_list.size()) {
        output = token_list[token_index];
        token_index++;
    }

    return output;
}

int main() {
    LexicalAnalyzer lexer = LexicalAnalyzer();
    Token t = lexer.get_token();

    while (t.token_type != END_OF_FILE) {
        t.printToken();
        t = lexer.get_token();
    }

    return 0;
}