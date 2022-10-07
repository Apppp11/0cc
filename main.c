#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>

#include "main.h"

//変数宣言
Token *token;
char *user_input;

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