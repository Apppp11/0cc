#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include "parse.h"
#include "main.h"
//トークン列生成

bool find_top(char *top, char *op)
{
    return memcmp(top, op, strlen(op)) == 0;
}
bool consume_operator(char *op)
{
    if (token->kind != TK_OPERATOR || token->len != strlen(op) || memcmp(token->str, op, token->len))
        return false;
    token = token->next_token;
    return true;
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

Token *create_token(TokenKind kind, Token *cur, char *str, int len)
{
    Token *tok = calloc(1, sizeof(Token));
    tok->kind = kind;
    tok->str = str;
    tok->len = len;
    cur->next_token = tok;
    return tok;
}
Token *tokenize(char *p)
{
    Token head;
    head.next_token = NULL;
    Token *cur_token = &head;

    while (*p)
    {
        if (isspace(*p))
        {
            p++;
            continue;
        }
        if (find_top(p, "==") || find_top(p, "!=") || find_top(p, ">=") || find_top(p, "<="))
        {
            cur_token = create_token(TK_OPERATOR, cur_token, p, 2);
            p += 2;
            continue;
        }
        if (find_top(p, "+") || find_top(p, "-") || find_top(p, "*") || find_top(p, "/") || find_top(p, "(") || find_top(p, ")") || find_top(p, "<") || find_top(p, ">"))
        {
            cur_token = create_token(TK_OPERATOR, cur_token, p++, 1);
            continue;
        }
        if (isdigit(*p))
        {
            cur_token = create_token(TK_NUM, cur_token, p, 1);
            cur_token->val = strtol(p, &p, 10);
            continue;
        }

        error_at(token->str, "トークナイズできません");
    }
    create_token(TK_EOF, cur_token, p, 1);
    return head.next_token;
}
