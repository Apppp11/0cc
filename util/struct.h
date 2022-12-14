#ifndef STRUCT_H
#define STRUCT_H
//トークン列用構造体
#include "enum.h"
typedef struct Token Token;
struct Token
{
    TokenKind kind;
    Token *next_token;
    int val;
    char *str;
    int len;
};
typedef struct Node Node;
//抽象構文木用構造体
struct Node
{
    NodeKind kind;
    Node *l_child;
    Node *r_child;
    int val;    //定数用
    int offset; //変数用
    Node *cond;
    Node *then;
    Node *els;
};

//ローカル変数名保持用
typedef struct LVar LVar;
struct LVar
{
    LVar *nextLVar;
    char *name;
    int len;
    int offset;
};

#endif // !STRUCT_H
