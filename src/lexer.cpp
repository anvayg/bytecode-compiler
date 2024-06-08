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
    } else if (isAlpha(currentChar)) {
      identifier();
    } else {
      Error::report(line, "Unexpected character");
      this->hadError = true;
    }
    break;
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

void Lexer::addToken(TokenType token) { addToken(token, ""); }

void Lexer::addToken(TokenType token, std::string value) {
  Token t = Token{.token = token, .value = value, .line = line};
  this->tokens.push_back(t);
}

bool Lexer::isAtEnd() { return current >= source.length(); }

bool Lexer::isDigit(char c) { return c >= '0' && c <= '9'; }

bool Lexer::isAlpha(char c) {
  return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c == '_');
}

bool Lexer::isAlphaNumeric(char c) { return (isAlpha(c) || isDigit(c)); }

void Lexer::string() {
  // Go till end of string
  // Escape sequences not supported
  while (peek() != '"' && !isAtEnd()) {
    if (peek() == '\n') line++;
    advance();
  }

  if (isAtEnd()) {
    Error::report(line, "Unterminated string");
    this->hadError=true;
    return;
  }

  // Closing "
  advance();

  // Get string literal
  std::string value = this->source.substr(start + 1, current - 1);
  addToken(TokenType::StrConstant, value);
}

void Lexer::number() {
  // Go till end of number
  while (isDigit(peek()) && !isAtEnd())
    advance();

  if (isAtEnd()) {
    Error::report(line, "Lisp program should end with ')'");
    this->hadError=true;
    return;
  }

  std::string value = source.substr(start, current);
  addToken(TokenType::Constant, value);
}

void Lexer::identifier() {
  while (isAlphaNumeric(peek()) && !isAtEnd()) advance();

  if (isAtEnd()) {
    Error::report(line, "Lisp program should end with ')'");
    this->hadError=true;
    return;
  }

  // Check for reserved keywords
  std::string text = source.substr(start, current);
  if (text == "lambda") {
    addToken(TokenType::Lambda, text);
  } else {
    addToken(TokenType::Identifier, text);
  }
}

Lexer::Lexer(std::string source) {
  this->source = source;
}

std::vector<Token> Lexer::lex() {
  while (current < source.length()) {
    // At beginning of next lexeme
    start = current;
    Lexer::lexToken();
  }
  
  tokens.push_back(
      Token{.token = TokenType::Eof, .value = "", .line = line});
  return tokens;
}

bool Lexer::lexError() {
  return this->hadError;
}
