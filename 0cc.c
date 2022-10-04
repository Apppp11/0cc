#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>

typedef enum
{
    TK_NUM,
    TK_OPERATOR,
    TK_EOF,
} TokenKind;
typedef struct Token Token;

struct Token
{
    TokenKind kind;
    Token *next_token;
    int val;
    char *str;
};

Token *token;
char *user_input;

void error_at(char *loc, char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);

    int pos = loc - user_input;
    fprintf(stderr, "%s\n", user_input);
    fprintf(stderr, "%*s", pos, " ");
    fprintf(stderr, "^ ");
    vfprintf(stderr, fmt, ap);
    fprintf(stderr, "\n");
    exit(1);
}

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

        if (*p == '+' || *p == '-')
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
int main(int argc, char **argv)
{
    if (argc != 2)
    {
        printf("引数の個数が正しくないです\n");
        return 1;
    }
    user_input = argv[1];
    token = tokenize(argv[1]);
    printf(".intel_syntax noprefix\n");
    printf(".globl main\n");
    printf("main:\n");
    printf("    mov rax, %d\n", expect_number());

    while (!at_eof())
    {
        if (consume_operator('+'))
        {
            printf("    add rax, %d\n", expect_number());
        }
        else
        {
            expect_operator('-');
            printf("    sub rax, %d\n", expect_number());
        }
    }
    printf("    ret\n");
    return 0;
}