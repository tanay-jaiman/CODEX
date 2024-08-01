#include <iostream>
#include <vector>

using namespace std;

#pragma once

/*
    Stores a line of program code
*/
struct Line {
    string line = "";
    int line_number = -1;
};

/*
    An InputBuffer object splits the program into multiple lines
    and removes any trailing/following whitespaces
    and also removes empty lines
*/
class InputBuffer {
    public:
        Line get_line();
        InputBuffer();

    private:
        vector<Line> line_list;
        vector<char> char_list;
        int line_no = 1;
        
        void skip_spaces();
        bool add_line();
        void populate_line_list();

        int buf_index = 0;
};