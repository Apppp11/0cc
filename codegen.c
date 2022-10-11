#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include "codegen.h"

int if_idx = 0;

//構文木生成
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

Node *create_lvar_node(char *name, int len)
{
    Node *node = calloc(1, sizeof(Node));
    node->kind = ND_LVAR;
    LVar *lvar = find_lvar(name, len);
    if (lvar)
    {
        node->offset = lvar->offset;
    }
    else
    {
        lvar = calloc(1, sizeof(LVar));
        lvar->nextLVar = locals;
        lvar->name = name;
        lvar->len = len;
        if (locals)
            lvar->offset = locals->offset + 8;
        else
            lvar->offset = 0;
        node->offset = lvar->offset;
        locals = lvar;
    }
    return node;
}

Node *create_node(NodeKind nk)
{
    Node *node = calloc(1, sizeof(Node));
    node->kind = nk;
    return node;
}
void program()
{
    int i = 0;
    while (!at_eof())
    {
        code[i++] = stmt();
    }
    code[i] = NULL;
}
Node *stmt()
{
    Node *node;
    if (consume_token(TK_RETURN))
    {
        node = create_node(ND_RETURN);
        node->l_child = expr();
        expect_operator(";");
    }
    else if (consume_token(TK_IF))
    {
        node = create_node(ND_IF);
        expect_operator("(");
        node->l_child = expr();
        expect_operator(")");
        node->r_child = stmt();
    }
    else
    {
        node = expr();
        expect_operator(";");
    }
    return node;
}
Node *expr()
{
    return assign();
}
Node *assign()
{
    Node *node = equality();
    if (consume_operator("="))
    {
        node = create_op_node(ND_ASN, node, assign());
    }
    return node;
}
Node *equality()
{
    Node *node = relational();
    while (1)
    {
        if (consume_operator("=="))
        {
            node = create_op_node(ND_EQL, node, relational());
        }
        else if (consume_operator("!="))
        {
            node = create_op_node(ND_NEQ, node, relational());
        }
        else
            return node;
    }
}
Node *relational()
{
    Node *node = add();
    while (1)
    {
        if (consume_operator("<="))
        {
            node = create_op_node(ND_LEQ, node, add());
        }
        else if (consume_operator(">="))
        {
            node = create_op_node(ND_LEQ, add(), node);
        }
        else if (consume_operator(">"))
        {
            node = create_op_node(ND_LES, add(), node);
        }
        else if (consume_operator("<"))
        {
            node = create_op_node(ND_LES, node, add());
        }
        else
            return node;
    }
}
Node *add()
{
    Node *node = mul();
    while (1)
    {

        if (consume_operator("+"))
        {
            node = create_op_node(ND_ADD, node, mul());
        }
        else if (consume_operator("-"))
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
    Node *node = unary();
    while (1)
    {
        if (consume_operator("*"))
        {
            node = create_op_node(ND_MUL, node, unary());
        }
        else if (consume_operator("/"))
        {
            node = create_op_node(ND_DIV, node, unary());
        }
        else
        {
            return node;
        }
    }
}

Node *unary()
{
    if (consume_operator("+"))
    {
        return primary();
    }
    else if (consume_operator("-"))
    {
        return create_op_node(ND_SUB, create_num_node(0), primary());
    }
    return primary();
}
Node *primary()
{
    if (consume_operator("("))
    {
        Node *node = expr();
        expect_operator(")");
        return node;
    }
    Token *token = consume_ident();
    if (token)
    {
        return create_lvar_node(token->str, token->len);
    }
    return create_num_node(expect_number());
}

//左辺値の評価
void gen_lval(Node *node)
{
    if (node->kind != ND_LVAR)
    {
        error("変数ではない左辺値が存在します");
    }
    printf("    mov rax, rbp\n");
    printf("    sub rax, %d\n", node->offset);
    printf("    push rax\n");
}

//アセンブリ生成
void gen(Node *node)
{
    switch (node->kind)
    {
    case ND_RETURN:
        gen(node->l_child);
        printf("    pop rax\n");
        printf("    mov rsp, rbp\n");
        printf("    pop rbp\n");
        printf("    ret\n");
        return;
        break;
    case ND_IF:
        gen(node->l_child);
        printf("    pop rax\n");
        printf("    cmp rax, 0\n");
        printf("    je .Lend%d\n", if_idx);
        gen(node->r_child);
        printf(".Lend%d:\n", if_idx);
        if_idx++;
        return;
        break;
    case ND_NUM:
        printf("    push %d\n", node->val);
        return;
        break;
    case ND_LVAR:
        gen_lval(node);
        printf("    pop rax\n");
        printf("    mov rax, [rax]\n");
        printf("    push rax\n");
        return;
        break;
    case ND_ASN:
        gen_lval(node->l_child);
        gen(node->r_child);
        printf("    pop rdi\n");
        printf("    pop rax\n");
        printf("    mov [rax], rdi\n");
        printf("    push rdi\n");
        return;
        break;
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
    case ND_EQL:
        printf("    cmp rax,rdi\n");
        printf("    sete al\n");
        printf("    movzb rax,al\n");
        break;
    case ND_NEQ:
        printf("    cmp rax,rdi\n");
        printf("    setne al\n");
        printf("    movzb rax,al\n");
        break;
    case ND_LES:
        printf("    cmp rax,rdi\n");
        printf("    setl al\n");
        printf("    movzb rax,al\n");
        break;
    case ND_LEQ:
        printf("    cmp rax,rdi\n");
        printf("    setle al\n");
        printf("    movzb rax,al\n");
        break;
    }
    printf("    push rax\n");
}