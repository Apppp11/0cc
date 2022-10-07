#ifndef SEARCH_TOKEN
#define SEARCH_TOKEN

#include "util/enum.h"
#include "util/struct.h"
#include "error.h"

extern Token *token;
bool consume_operator(char *op);
Token *consume_ident();
void expect_operator(char *op);
int expect_number();
bool at_eof();
#endif // !SEARCH_TOKEN