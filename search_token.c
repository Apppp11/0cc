#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include "search_token.h"
bool consume_operator(char *op)
{
    if (token->kind != TK_OPERATOR || token->len != strlen(op) || memcmp(token->str, op, token->len))
        return false;
    token = token->next_token;
    return true;
}
Token *consume_ident()
{
    if (token->kind != TK_IDENT)
        return NULL;
    Token *ret = token;
    token = token->next_token;
    return ret;
}

void expect_operator(char *op)
{
    if (token->kind != TK_OPERATOR || token->len != strlen(op) || memcmp(token->str, op, token->len))
        error_at(token->str, "'%c'ではありません", op);
    token = token->next_token;
}

int expect_number()
{
    if (token->kind != TK_NUM)
        error_at(token->str, "数ではありません");
    int val = token->val;
    token = token->next_token;
    return val;
}

bool at_eof()
{
    return token->kind == TK_EOF;
}
