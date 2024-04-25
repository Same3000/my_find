#ifndef TOKENIZER_H
#define TOKENIZER_H

#include <err.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

#include "string.h"

enum type_tok
{
    AND,
    OR,
    LEFTPAREN,
    RIGHTPAREN,
    NAME,
    PRINT,
    EXEC,
    TYPE,
    NEWER,
    PERM,
    USER,
    GROUP,
    D,
    ERROR
};

struct tokens
{
    struct token **tok;
    size_t len;
    int action;
    int depth;
};
struct token
{
    char **arg;
    size_t len_arg;
    int priority;
    enum type_tok type;
    struct token *left;
    struct token *right;
};

int my_is_valid_type(struct token *t);
struct token *create_print(size_t argc, char *argv[]);
struct token *create_token(char **argv, size_t *i, size_t argc);

struct tokens *tokenizer(size_t argc, char **argv);

#endif /*TOKENIZER_H */
