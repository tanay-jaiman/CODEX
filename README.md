# CODEX

'CODEX' is a project aimed at creating a fun scripting language that performs basic computations and instructions. It is inspired by the medieval tongue with a neat Witcher 3 reference.

## Features and Drawbacks (0.2.0)

1. **CODEX** is an interpreted, high-level, & weakly-typed scripting language. It takes syntactic inspiration from C++.
2. Two data types are supported that can be stored inside variables - floating point numerals (any integer declared is automatically type casted into a float), and strings.
3. Both of the above-mentioned data types are declared implicitly with automatic type casting from float to string in that specified order.
4. Booleans are used to resolve conditions but cannot be used as a data type.
5. Errors : 
    -   **Syntax errors** work like any language, if the syntax is violated then, an error is thrown.
    -   **Type errors** are thrown only in the case of subtraction, multiplication, or division between strings and numbers. In case of addition or boolean comparison between different types, float is casted into string and passed through a `strcmp`. 
    -   **Declaration errors** arise in one of the following cases :
        1. A variable is used but was never declared
        2. A variable is being declared that has already been declared (note that assignment is not the same as declaration)
        3. A variable was declared as a constant(`engrave`) but an attempt to assign it to another value was made.

6. Conditional statements are handled using "consider and otherwise," which is the CODEX counterpart of if-else. Note that there is no existing counterpart of "else if" or "elif."
7. Looping through statements is handled by "whilst," which is the CODEX counterpart of a while loop. Note that there is no existing counterpart of a "for loop."
8. Comments are similar to how they would be declared in C++ with the use of `//`. There is, however, no support for multi-line comments.
9. Currently, there is no support for arrays/lists, structs, enums, or functions.

## Syntax

1. **Assignment:**
    ```plaintext
    proclaim <variable_name> = <variable_value> ;
    ```

2. **Constant Assignment:**
    ```plaintext
    engrave <variable_name> = <variable_value> ;
    ```

3. **Print Statement:**
    ```plaintext
    announce <string_to_be_printed> | <variable_to_be_printed> ;
    ```

4. **If-Else Statement:**
    ```plaintext
    consider (<condition>) {
        <statements>
    }
    otherwise {
        <statements>
    }
    ```

5. **Break Out of Body:**
    ```plaintext
    thou shalt not pass ;
    ```

6. **While Loop:**
    ```plaintext
    whilst (<condition>) {
        <statements>
    }
    ```

## Tokens

```plaintext
PLUS                -> +
MINUS               -> -
MULT                -> *
DIV                 -> /
EQUAL               -> =
SEMICOLON           -> ;
LPAREN              -> (
RPAREN              -> )
LBRACE              -> {
RBRACE              -> }
LANG                -> <
RANG                -> >
EXCLAMATION         -> !
QUOTES              -> "
DOT                 -> .
PROCLAIM            -> proclaim
ANNOUNCE            -> announce
ENGRAVE             -> engrave
CONSIDER            -> consider
OTHERWISE           -> otherwise
WHILST              -> whilst
THOU                -> thou
SHALT               -> shalt
NOT                 -> not
PASS                -> pass
```

# GRAMMAR

``` plaintext
<program>         -> <statement_list>
<statement_list>  -> <statement> | <statement> <statement_list>
<statement>       -> <assign_statement> | <print_statement> | <if_statement> | <break_statement> | <while_loop> | <comment>

<assign_statement>      -> <variable_assignment> | <constant_assignment>
<variable_assignment>   -> PROCLAIM <ID> EQUAL <expression> SEMICOLON | <ID> EQUAL <expression> SEMICOLON
<constant_assignment>   -> ENGRAVE <ID> EQUAL <expression> SEMICOLON
<ID>                    -> <letter> | <letter> <char_list>

<expression>            -> <expression> PLUS <expression> | <expression> MINUS <expression> | <expression> MULT <expression> | <expression> DIV <expression> | LPAREN <expression> RPAREN | <literal>

<literal>               -> <string> | <number> | <ID>

<print_statement>       -> ANNOUNCE <string_list> SEMICOLON
<string_list>           -> <string> | <string> PLUS <string_list>
<string>                -> <ID> | <string_literal>
<char_list>             -> <char> | <char> <char_list>
<string_literal>        -> QUOTES <words> QUOTES

<letter>                -> A | B | ... | Z
<number>                -> <number> <number> | <number> DOT <number> | 0 | 1 | ... | 9
<char>                  -> A | B | ... | Z | 0 | 1 | ... | 9 | _

<condition>             -> <expression> <condition_operator> <expression>
<condition_operator>    -> EQUAL EQUAL | LANG | RANG | LANG EQUAL | RANG EQUAL | EXCLAMATION EQUAL

<if_statement>          -> CONSIDER LPAREN <condition> RPAREN LBRACE <statement_list> RBRACE |  CONSIDER LPAREN <condition> RPAREN LBRACE <statement_list> RBRACE OTHERWISE RPAREN LBRACE <statement_list> RBRACE

<while_statement>       -> WHILST LPAREN <condition> RPAREN LBRACE <statement_list> RBRACE

<break_statement>       -> THOU SHALT NOT PASS SEMICOLON

<comment>               -> DIV DIV <char_list>
```
# REPORT BUGS / GIVE FEEDBACK

To report any bugs or to leave a comment/feedback, you may send me an email at tanayjaiman@outlook.com or drop a message on my linkedin - linkedin.com/in/tanayjaiman