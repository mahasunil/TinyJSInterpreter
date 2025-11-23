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
        while (currentChar != '\0')
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

struct ASTNode
{
    virtual ~ASTNode() {};
};

struct NumberNode : ASTNode
{
    int value;
    NumberNode(int v) : value(v) {};
};
struct BinOpNode : ASTNode
{
    shared_ptr<ASTNode> left;
    TokenType op;
    shared_ptr<ASTNode> right;
    BinOpNode(shared_ptr<ASTNode> l, TokenType o, shared_ptr<ASTNode> r) : left(l), op(o), right(r) {};
};

struct VarAccessNode : ASTNode
{
    string name;
    VarAccessNode(const string &n) : name(n) {};
};

struct VarAssignNode : ASTNode
{
    string name;
    shared_ptr<ASTNode> value;
    VarAssignNode(const string &n, shared_ptr<ASTNode> v) : name(n), value(v) {};
};

class Parser
{
public:
    Parser(const vector<Token> &tokens) : tokens(tokens), pos(0) {};
    shared_ptr<ASTNode> parse()
    {
        return expr();
    }

private:
    vector<Token> tokens;
    size_t pos;

    Token currentToken()
    {
        if (pos < tokens.size())
        {
            return tokens[pos];
        }
        return {TokenType::END, ""};
    }

    void advance()
    {
        if (pos < tokens.size())
            pos++;
    }

    shared_ptr<ASTNode> factor()
    {
        Token tok = currentToken();
        if (tok.type == TokenType::NUMBER)
        {
            advance();
            return make_shared<NumberNode>(stoi(tok.value));
        }
        else if (tok.type == TokenType::IDENTIFIER)
        {
            advance();
            return make_shared<VarAccessNode>(tok.value);
        }
        else if (tok.type == TokenType::LPAREN)
        {
            advance();
            auto node = expr();
            if (currentToken().type == TokenType::RPAREN)
                advance();
            return node;
        }
        return nullptr;
    }

    shared_ptr<ASTNode> term()
    {
        auto node = factor();
        while (currentToken().type == TokenType::MUL || currentToken().type == TokenType::DIV)
        {
            TokenType op = currentToken().type;
            advance();
            node = make_shared<BinOpNode>(node, op, factor());
        }
        return node;
    }

    shared_ptr<ASTNode> expr()
    {
        auto node = term();
        while (currentToken().type == TokenType::PLUS || currentToken().type == TokenType::MINUS)
        {
            TokenType op = currentToken().type;
            advance();
            node = make_shared<BinOpNode>(node, op, term());
        }
        return node;
    }
};