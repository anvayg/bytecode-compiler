#include "../include/lexer.hpp"
#include <string>
#include <vector>

// Lexer
void Lexer::lexToken() {
    char currentChar = source.at(current);
    this->current++;

    switch (currentChar) {
        case '(':
        case ')':
    }
}

void Lexer::addToken(TokenType token) {
    
}

std::vector<Token> Lexer::lex() {
  while (current < source.length()) {
    // At beginning of next lexeme
    start = current;
    Lexer::lexToken();
  }

  tokens.push_back(
      Token{.token = TokenType::Eof, .value = nullptr, .line = line});
  return tokens;
}
