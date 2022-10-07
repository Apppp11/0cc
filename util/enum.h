#ifndef ENUM_H
#define ENUM_H
typedef enum
{
    TK_NUM,
    TK_OPERATOR,
    TK_IDENT,
    TK_EOF,
} TokenKind;
typedef enum
{
    ND_ADD, //+
    ND_SUB, //-
    ND_MUL, //*
    ND_DIV, ///
    ND_EQL, //==
    ND_NEQ, //!=
    ND_LES, //<
    ND_LEQ, //<=
    ND_ASN, //=
    ND_NUM,
    ND_LVAR,
} NodeKind;
#endif // !ENUM_H