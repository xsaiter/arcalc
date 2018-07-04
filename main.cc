/*
 * recursive descent parsing
 *
 * expr -> term {+term}
 * term -> factor {*factor}
 * factor -> num | (expr)
*/

#include <iostream>
#include <string>
#include <memory>
#include <cctype>

enum class tok_s { NUM, ADD, MUL, OPEN, CLOSE, END };

int expr();
int term();
int factor();

tok_s cur_tok;
long cur_num;
int cur_pos = 0;
std::string input;

bool is_digit(char c) { return std::isdigit(c); }

void read_next_tok() {
  int i = cur_pos;

  char c = input[i++];

  if (c == '(') {
    cur_tok = tok_s::OPEN;
  } else if (c == ')') {
    cur_tok = tok_s::CLOSE;
  } else if (c == '+') {
    cur_tok = tok_s::ADD;
  } else if (c == '*') {
    cur_tok = tok_s::MUL;
  } else if (is_digit(c)) {
    std::string s;
    s += c;
    while (is_digit(input[i])) {
      s += c;
      ++i;
    }
    cur_num = std::atoi(s.c_str());
    cur_tok = tok_s::NUM;
  }

  cur_pos = i;
}

int expr() {
  int res = term();
  while (cur_tok == tok_s::ADD) {
    read_next_tok();
    res += term();
  }
  return res;
}

int term() {
  int res = factor();
  while (cur_tok == tok_s::MUL) {
    read_next_tok();
    res *= factor();
  }
  return res;
}

int factor() {
  int res = 0;
  switch (cur_tok) {
  case tok_s::NUM:
    res = cur_num;
    read_next_tok();
    break;
  case tok_s::OPEN:
    read_next_tok();
    res = expr();
    if (cur_tok == tok_s::CLOSE) {
      read_next_tok();
    } else {
      throw "no close parentheses found";
    }
    break;
  default:
    throw "unexpected token";
  }
  return res;
}

int main(int argc, char *argv[]) {
  input = "(1+2)*3";
  read_next_tok();
  int res = expr();
  std::cout << "result: " << res << std::endl;
}
