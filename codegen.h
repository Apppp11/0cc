#ifndef CODEGEN_H
#define CODEGEN_H

#include "main.h"
#include "parse.h"
typedef struct Node Node;
Node *create_op_node(NodeKind kind, Node *l_child, Node *r_child);
Node *create_num_node(int val);
Node *expr();
Node *mul();
Node *unary();
Node *primary();
void gen(Node *node);

#endif // !CODEGEN_H
