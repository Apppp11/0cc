#ifndef PARSE_H
#define PARSE_H

#include "main.h"
typedef struct Token Token;
bool consume_operator(char op);
void expect_operator(char op);
int expect_number();
bool at_eof();
Token *create_token(TokenKind kind, Token *cur, char *str);
Token *tokenize(char *p);

#endif // !PARSE_H