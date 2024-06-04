#include "../include/lexer.hpp"
#include <string>
#include <vector>

// Lexer
void Lexer::lexToken() {
  char currentChar = source.at(current);
  this->current++;

  switch (currentChar) {
  case '(':
    addToken(TokenType::OpenParen);
    break;
  case ')':
    addToken(TokenType::CloseParen);
    break;
  case '+':
    addToken(TokenType::Plus);
    break;
  case '-':
    addToken(TokenType::Minus);
    break;
  case '*':
    addToken(TokenType::Star);
    break;
  }
}

void Lexer::addToken(TokenType token) { addToken(token, nullptr); }

void Lexer::addToken(TokenType token, std::string value) {
  this->tokens.push_back(Token{.token = token, .value = value, .line = line});
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
