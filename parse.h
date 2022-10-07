#ifndef PARSE_H
#define PARSE_H

#include "util/enum.h"
#include "util/struct.h"
#include "error.h"
extern Token *token;
bool search_top(char *top, char *op);
Token *create_token(TokenKind kind, Token *cur, char *str, int len);
Token *tokenize(char *p);
#endif // !PARSE_H