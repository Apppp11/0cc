#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>

#include "main.h"

//変数宣言
Token *token;     //トークン列
char *user_input; //入力文字列
LVar *locals;     //ローカル変数列

Node *code[100]; //構文木

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        error("引数の個数が正しくないです\n");
        return 1;
    }
    //トークン列作成
    user_input = argv[1];
    token = tokenize(argv[1]);

    printf(".intel_syntax noprefix\n");
    printf(".globl main\n");
    printf("main:\n");

    //変数領域確保
    printf("    push rbp\n");
    printf("    mov rbp, rsp\n");
    printf("    sub rsp, 208\n");

    //構文木作成
    program();
    int i = 0;
    while (code[i] != NULL)
    {
        gen(code[i++]);
        printf("    pop rax\n");
    }

    printf("    mov rsp, rbp\n");
    printf("    pop rbp\n");

    printf("    ret\n");
    return 0;
}