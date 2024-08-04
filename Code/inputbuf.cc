#include "inputbuf.h"

using namespace std;

/*
    Constructor when invoked stores all the characters into a list and invokes method to populate the line list
*/
InputBuffer::InputBuffer() {
    char c = getchar();
    while (c != EOF) {
        char_list.push_back(c);
        c = getchar();
    }

    populate_line_list();
    
    // destroy char_list
    char_list.clear();
}

/*
    Skip leading/trailing whitespace
*/
void InputBuffer::skip_spaces() {
    while (buf_index < char_list.size()) {
        if (char_list[buf_index] == ' ' || char_list[buf_index] == '\t') {
            buf_index++;
        }

        else if (char_list[buf_index] == '\n') {
            buf_index++;
            line_no++;
        }

        else
            break;
    }
}

/*
    add_line() does the following:
        1. skip leading whitespace
        2. add the characters and line number in the line_list
        3. skip trailing whitespace
*/
bool InputBuffer::add_line() {
    skip_spaces();
    Line new_line;
    new_line.line_number = line_no;
    
    while (buf_index < char_list.size()) {
        if (char_list[buf_index] == '\n') {
            buf_index++;
            line_no++;
            break;
        }

        new_line.line += char_list[buf_index];
        buf_index++;
    }

    skip_spaces();

    if (new_line.line != "") {
        line_list.push_back(new_line);
        return true;
    }

    return false;
}

/*
    Adds lines into the line_list until there is a line to be added
*/
void InputBuffer::populate_line_list() {
    bool line_added = add_line();
    while (line_added)
        line_added = add_line();
}

/*
    Returns the next available line
*/
Line InputBuffer::get_line() {
    if (line_list.empty()) {
        return Line();
    }
    
    Line output = line_list[0];
    line_list.erase(line_list.begin());

    return output;
}
