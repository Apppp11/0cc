#ifndef CODEGEN_H
#define CODEGEN_H

#include "search_token.h"

extern Node *code[100];

Node *create_op_node(NodeKind kind, Node *l_child, Node *r_child);
Node *create_num_node(int val);

void program();
Node *stmt();
Node *expr();
Node *assign();
Node *equality();
Node *relational();
Node *add();
Node *mul();
Node *unary();
Node *primary();
void gen(Node *node);

#endif // !CODEGEN_H
