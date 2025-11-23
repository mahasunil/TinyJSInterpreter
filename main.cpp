#include <iostream>
#include <vector>
#include <cctype>
#include <string>
#include <memory>

using namespace std;

enum class TokenType
{
    PLUS,
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

struct Token
{
    TokenType type;
    string value;
};

class Lexer
{
public:
    Lexer(const string &str) : text(str), pos(0) { advance(); };

    vector<Token> tokenize()
    {
        vector<Token> tokens;
        while (currentChar!='\0')
        {
            if (isspace(currentChar))
                advance();
            else if (isdigit(currentChar))
            {
                tokens.push_back(number());
            }
            else if (isalpha(currentChar))
            {
                tokens.push_back(identifier());
            }
            else if (currentChar == '+')
            {
                tokens.push_back({TokenType::PLUS, "+"});
                advance();
            }
            else if (currentChar == '-')
            {
                tokens.push_back({TokenType::MINUS, "-"});
                advance();
            }
            else if (currentChar == '*')
            {
                tokens.push_back({TokenType::MUL, "*"});
                advance();
            }
            else if (currentChar == '/')
            {
                tokens.push_back({TokenType::DIV, "/"});
                advance();
            }
            else if (currentChar == '(')
            {
                tokens.push_back({TokenType::LPAREN, "("});
                advance();
            }
            else if (currentChar == ')')
            {
                tokens.push_back({TokenType::RPAREN, ")"});
                advance();
            }
            else if (currentChar == ';')
            {
                tokens.push_back({TokenType::SEMICOLON, ";"});
                advance();
            }
            else if (currentChar == '=')
            {
                tokens.push_back({TokenType::ASSIGN, "="});
                advance();
            }
            else
            {
                cout << "Unknown character...." << endl;
                advance();
            }
        }
        tokens.push_back({TokenType::END, ""});
        return tokens;
    }

private:
    string text;
    int pos;
    char currentChar;

    void advance()
    {
        if (pos < text.size())
        {
            currentChar = text[pos++];
        }
        else
        {
            currentChar = '\0';
        }
    }

    Token number()
    {
        string result;
        while (isdigit(currentChar))
        {
            result += currentChar;
            advance();
        }
        return {TokenType::NUMBER, result};
    }

    Token identifier()
    {
        string result;
        while (isalnum(currentChar))
        {
            result += currentChar;
            advance();
        }
        if (result == "let")
        {
            return {TokenType::LET, result};
        }
        if (result == "print")
        {
            return {TokenType::PRINT, result};
        }
        return {TokenType::IDENTIFIER, result};
    }
};