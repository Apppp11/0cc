#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include "codegen.h"

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
    Node *node = unary();
    while (1)
    {
        if (consume_operator('*'))
        {
            node = create_op_node(ND_MUL, node, unary());
        }
        else if (consume_operator('/'))
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
    if (consume_operator('+'))
    {
        return primary();
    }
    else if (consume_operator('-'))
    {
        return create_op_node(ND_SUB, create_num_node(0), primary());
    }
    return primary();
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