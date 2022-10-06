#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include "parse.h"
#include "main.h"
//トークン列生成
bool consume_operator(char op)
{
    if (token->kind != TK_OPERATOR || token->str[0] != op)
        return false;
    token = token->next_token;
    return true;
}

void expect_operator(char op)
{
    if (token->kind != TK_OPERATOR || token->str[0] != op)
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

Token *create_token(TokenKind kind, Token *cur, char *str)
{
    Token *tok = calloc(1, sizeof(Token));
    tok->kind = kind;
    tok->str = str;
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

        if (*p == '+' || *p == '-' || *p == '*' || *p == '/' || *p == '(' || *p == ')')
        {
            cur_token = create_token(TK_OPERATOR, cur_token, p++);
            continue;
        }
        if (isdigit(*p))
        {
            cur_token = create_token(TK_NUM, cur_token, p);
            cur_token->val = strtol(p, &p, 10);
            continue;
        }

        error_at(token->str, "トークナイズできません");
    }
    create_token(TK_EOF, cur_token, p);
    return head.next_token;
}
