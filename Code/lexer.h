#include "inputbuf.h"

using namespace std;

#ifndef LEXER_H
#define LEXER_H

enum TokenType {PLUS, MINUS, MULT, DIV, EQUAL, SEMICOLON, LPAREN, RPAREN, LBRACE, RBRACE, LANG, RANG, EXCLAMATION, QUOTES, DOT, PROCLAIM, ANNOUNCE, ENGRAVE, CONSIDER, OTHERWISE, WHILST, THOU, SHALT, NOT, PASS, ID, NUMBER, STRING_LITERAL, ERROR, END_OF_FILE};


class Token {
    public:
        Token() {
            this->token_type = ERROR;
            this->line_number = -1;
            this->lexeme = "";
        }

        void printToken();
        TokenType token_type;
        int line_number;
        string lexeme;
};

class LexicalAnalyzer {
    public:
        LexicalAnalyzer() {
            tokenify_program();
        }

        Token get_token();
        Token peek_token(int how_far);

    private:
        InputBuffer input_buffer;
        vector<Token> token_list;
        int token_index = 0;

        void tokenify_program();
        void tokenify_line(Line);

        int get_token_main(Line line, int index);
        Token scan_id_or_keyword(Line line, int index);
        Token scan_number(Line line, int index);
        Token scan_string(Line line, int index);
        Token scan_symbol(Line line, int index);
        int next_scan_index(string lexeme);
};

#endif