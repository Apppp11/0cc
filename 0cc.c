#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>

//トークン列用構造体
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

//抽象構文木用構造体
typedef enum
{
    ND_ADD,
    ND_SUB,
    ND_MUL,
    ND_DIV,
    ND_NUM,
} NodeKind;

typedef struct Node Node;
struct Node
{
    NodeKind kind;
    Node *l_child;
    Node *r_child;
    int val;
};

//変数宣言
Token *token;
char *user_input;

//エラー出力
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

//構文木生成
Node *create_op_node(NodeKind kind, Node *l_child, Node *r_child);
Node *create_num_node(int val);
Node *expr();
Node *mul();
Node *primary();

Node *create_op_node(NodeKind kind, Node *l_child, Node *r_child)
{
    Node *node = calloc(1, sizeof(Node));
    node->kind = kind;
    node->l_child = l_child;
    node->r_child = r_child;
    return node;
}

Node *create_num_node(int val)
{
    Node *node = calloc(1, sizeof(Node));
    node->kind = ND_NUM;
    node->val = val;
    return node;
}

Node *expr()
{
    Node *node = mul();
    while (1)
    {

        if (consume_operator('+'))
        {
            node = create_op_node(ND_ADD, node, mul());
        }
        else if (consume_operator('-'))
        {
            node = create_op_node(ND_SUB, node, mul());
        }
        else
        {
            return node;
        }
    }
}
Node *mul()
{
    Node *node = primary();
    while (1)
    {
        if (consume_operator('*'))
        {
            node = create_op_node(ND_MUL, node, primary());
        }
        else if (consume_operator('/'))
        {
            node = create_op_node(ND_DIV, node, primary());
        }
        else
        {
            return node;
        }
    }
}
Node *primary()
{
    if (consume_operator('('))
    {
        Node *node = expr();
        expect_operator(')');
        return node;
    }
    return create_num_node(expect_number());
}

//アセンブリ生成
void gen(Node *node)
{
    if (node == NULL)
        exit(1);
    if (node->kind == ND_NUM)
    {
        printf("    push %d\n", node->val);
        return;
    }

    //演算子の場合
    gen(node->l_child);
    gen(node->r_child);
    printf("    pop rdi\n");
    printf("    pop rax\n");
    switch (node->kind)
    {
    case ND_ADD:
        printf("    add rax, rdi\n");
        break;
    case ND_SUB:
        printf("    sub rax,rdi\n");
        break;
    case ND_MUL:
        printf("    imul rax, rdi\n");
        break;
    case ND_DIV:
        printf("    cqo\n");
        printf("    idiv rdi\n");
        break;
    }
    printf("    push rax\n");
}
int main(int argc, char **argv)
{
    if (argc != 2)
    {
        printf("引数の個数が正しくないです\n");
        return 1;
    }
    //トークン列作成
    user_input = argv[1];
    token = tokenize(argv[1]);
    printf(".intel_syntax noprefix\n");
    printf(".globl main\n");
    printf("main:\n");

    //構文木作成
    Node *node = expr();
    gen(node);
    printf("    pop rax\n");
    printf("    ret\n");
    return 0;
}