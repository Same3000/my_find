#ifndef TOKENIZER_H
#define TOKENIZER_H

#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include "string.h"
#include <err.h>

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
	ERROR
};

struct tokens
{
    struct token **tok;
    size_t len;
	int action;
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
struct token *create_print(size_t argc, char *argv[]);
struct token *create_token(char **argv ,size_t *i, size_t argc);

struct tokens *tokenizer(size_t argc, char **argv);

#endif /*TOKENIZER_H */
