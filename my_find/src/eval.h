#ifndef EVAL_H
#define EVAL_H
#define _DEFAULT_SOURCE
#include "simple_ls.h"
#include "tokenizer.h"
#include <fnmatch.h>
#include <sys/stat.h>

int evaluate(struct token *tok, struct file *f);
int print_func(struct file *f);
#endif /* EVAL_H */
