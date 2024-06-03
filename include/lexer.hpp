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

  void lexToken();
  void addToken(TokenType t);
  void addToken(TokenType t, std::string value);

public:
  Lexer(std::string source);
  std::vector<Token> lex();
};