#ifndef ERROR_H
#define ERROR_H

extern char *user_input;
void error(char *fmt);
void error_at(char *loc, char *fmt, ...);

#endif // !ERROR_H