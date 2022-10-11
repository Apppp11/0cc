#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include "parse.h"
//トークン列生成

bool find_top(char *top, char *op)
{
    return memcmp(top, op, strlen(op)) == 0;
}
bool is_alnum(char c)
{
    return ('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z' || '0' <= c && c <= '9' || c == '_');
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
        if (find_top(p, "return") && !is_alnum(*(p + strlen("return"))))
        {
            cur_token = create_token(TK_RETURN, cur_token, p, strlen("return"));
            p += strlen("return");
            continue;
        }
        if (find_top(p, "if") && !is_alnum(*(p + strlen("if"))))
        {
            cur_token = create_token(TK_IF, cur_token, p, strlen("if"));
            p += strlen("if");
            continue;
        }
        if (find_top(p, "==") || find_top(p, "!=") || find_top(p, ">=") || find_top(p, "<="))
        {
            cur_token = create_token(TK_OPERATOR, cur_token, p, 2);
            p += 2;
            continue;
        }
        if (find_top(p, "+") || find_top(p, "-") || find_top(p, "*") || find_top(p, "/") || find_top(p, "(") || find_top(p, ")") || find_top(p, "<") || find_top(p, ">") || find_top(p, "=") || find_top(p, ";"))
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
        int lv_size = 0;
        for (;; lv_size++)
        {
            if (!('a' <= *(p + lv_size) && *(p + lv_size) <= 'z'))
                break;
        }
        if (lv_size != 0)
        {
            cur_token = create_token(TK_IDENT, cur_token, p, lv_size);
            p += lv_size;
            continue;
        }

        error_at(token->str, "トークナイズできません");
    }
    create_token(TK_EOF, cur_token, p, 1);
    return head.next_token;
}
