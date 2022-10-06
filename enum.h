#ifndef ENUM_H
#define ENUM_H
typedef enum
{
    TK_NUM,
    TK_OPERATOR,
    TK_EOF,
} TokenKind;
typedef enum
{
    ND_ADD,
    ND_SUB,
    ND_MUL,
    ND_DIV,
    ND_NUM,
} NodeKind;
#endif // !ENUM_H