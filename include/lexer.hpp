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
  void string();
  void number();

public:
  Lexer(std::string source);
  std::vector<Token> lex();
};