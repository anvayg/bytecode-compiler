#include "../include/lexer.hpp"
#include "../include/error.hpp"
#include <string>
#include <vector>

// Lexer
void Lexer::lexToken() {
  char currentChar = advance();

  switch (currentChar) {
  // Single character lexemes
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

  // Linebreaks and whitespace
  case ' ':
  case '\r':
  case '\t':
    // Ignore whitespace.
    break;
  case '\n':
    line++;
    break;

  // Comments
  case '/':
    if (match('/')) {
      while (peek() != '\n' && !isAtEnd())
        advance();
    } else {
      Error::report(line, "Division operator currently not supported");
      this->hadError = true;
    }
    break;

  // String literals
  case '"':
    string();
    break;

  default:
    if (isDigit(currentChar)) {
      number();
      break;
    }
  }
}

char Lexer::advance() {
  char currentChar = source.at(current);
  this->current++;
  return currentChar;
}

bool Lexer::match(char expected) {
  if (isAtEnd())
    return false;
  if (source.at(current) != expected)
    return false;

  this->current++;
  return true;
}

char Lexer::peek() {
  if (isAtEnd())
    return '\0';
  else
    return source.at(current);
}

void Lexer::addToken(TokenType token) { addToken(token, nullptr); }

void Lexer::addToken(TokenType token, std::string value) {
  this->tokens.push_back(Token{.token = token, .value = value, .line = line});
}

bool Lexer::isAtEnd() { return current >= source.length(); }

bool Lexer::isDigit(char c) {
  return c >= '0' && c <= '9';
}

void Lexer::string() {
  // Go till end of string
  // Escape sequences not supported
  while (peek() != '"' || !isAtEnd()) {
    if (peek() == '\n') line++;
    advance();
  }

  if (isAtEnd()) {
    Error::report(line, "Unterminated string");
    return;
  }

  // Get string literal
  std::string value = this->source.substr(start + 1, current);
  addToken(TokenType::StrConstant, value);
}

void Lexer::number() {
  // Go till end of number
  while (isDigit(peek()) && !isAtEnd()) advance();

  if (isAtEnd()) {
    Error::report(line, "Lisp program should end with ')'");
    return;
  }

  std::string value = source.substr(start, current);
  addToken(TokenType::Constant, value);
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
