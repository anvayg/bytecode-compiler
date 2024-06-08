#include <iostream>
#include <string>
#include <vector>

enum class TokenType {
  // Tokens which will have contents (literals)
  Constant,
  StrConstant,
  Identifier,

  // Keywords
  Lambda,

  // Punctuation
  OpenParen,
  CloseParen,
  Plus,
  Minus,
  Star,

  Eof
};

struct Token {
  TokenType token;
  std::string value;
  int line;
};

// Overload << operator for TokenType
inline std::ostream &operator<<(std::ostream &os, const TokenType &type) {
  switch (type) {
  case TokenType::Constant:
    return os << "Constant";
  case TokenType::StrConstant:
    return os << "StrConstant";
  case TokenType::Identifier:
    return os << "Identifier";
  case TokenType::Lambda:
    return os << "Lambda";
  case TokenType::OpenParen:
    return os << "OpenParen";
  case TokenType::CloseParen:
    return os << "CloseParen";
  case TokenType::Plus:
    return os << "Plus";
  case TokenType::Minus:
    return os << "Minus";
  case TokenType::Star:
    return os << "Star";
  case TokenType::Eof:
    return os << "Eof";
  default:
    return os << "Unknown";
  }
}

// Overload << operator for Token
inline std::ostream &operator<<(std::ostream &os, const Token &token) {
  os << "Token(" << token.token << ", \"" << token.value << "\", " << token.line
     << ")";
  return os;
}

// Function to print a vector of Tokens
inline void printTokens(const std::vector<Token> &tokens) {
  for (const auto &token : tokens) {
    std::cout << token << std::endl;
  }
}

class Lexer {
private:
  int start = 0;
  int current = 0;
  int line = 1;
  std::string source;
  std::vector<Token> tokens;
  bool hadError = false;

  void lexToken();
  char advance();
  bool match(char expected);
  char peek();
  void addToken(TokenType t);
  void addToken(TokenType t, std::string value);
  bool isAtEnd();
  bool isDigit(char c);
  bool isAlpha(char c);
  bool isAlphaNumeric(char c);
  void string();
  void number();
  void identifier();

public:
  Lexer(std::string source);
  std::vector<Token> lex();
  bool lexError();
};