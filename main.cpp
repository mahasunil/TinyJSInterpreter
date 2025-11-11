#include<iostream>
#include<vector>
#include<cctype>
#include<string>
#include<memory>

using namespace std;

enum class TokenType{
    PlUS,
    MINUS,
    MUL,
    DIV,
    RPAREN,
    LPAREN,
    ASSIGN,
    LET,
    PRINT,
    IDENTIFIER,
    SEMICOLON,
    NUMBER,
    END
};