#ifndef EVAL_H
#define EVAL_H
#define _DEFAULT_SOURCE
#include <err.h>
#include <fnmatch.h>
#include <grp.h>
#include <pwd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>

#include "simple_ls.h"
#include "tokenizer.h"

// int is_valid_perm(struct token *t);
int evaluate(struct token *tok, struct file *f);
int print_func(struct file *f);
#endif /* EVAL_H */
