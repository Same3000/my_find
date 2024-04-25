#include "eval.h"

int print_func(struct file *f)
{
	if (f->boolval == 1)
		printf("%s\n", f->path);
	return 1;
}
static int name_func(struct file *f, struct token *t)
{
	if (fnmatch(t->arg[0], f->name, 0) == 0)
	{
		f->boolval = 1;
		return 1;
	}
	f->boolval = 0;
	return 0;
}
static int is_valid(struct token *t)
{
	if (strcmp(t->arg[0], "d") == 0)
		return 1;
	if (strcmp(t->arg[0], "c") == 0)
		return 1;
	if (strcmp(t->arg[0], "b") == 0)
		return 1;
	if (strcmp(t->arg[0], "f") == 0)
		return 1;
	if (strcmp(t->arg[0], "l") == 0)
		return 1;
	if (strcmp(t->arg[0], "p") == 0)
		return 1;
	if (strcmp(t->arg[0], "s") == 0)
		return 1;
	return -1;
}
static int type_func(struct file *f, struct token *t)
{
	if (t->len_arg != 1)
		exit(1);
	if (is_valid(t) == -1)
		exit(1);
	struct stat SBuff;

    stat(f->path, &SBuff);
	if (S_ISDIR(SBuff.st_mode) != 0)
	{
		if (strcmp(t->arg[0], "d") == 0)
		{
			f->boolval = 1;
			return 1;
		}
		f->boolval = 0;
		return 0;
	}
	if (S_ISCHR(SBuff.st_mode) != 0)
	{
		if (strcmp(t->arg[0], "c") == 0)
		{
			f->boolval = 1;
			return 1;
		}
		f->boolval = 0;
		return 0;
	}
	if (S_ISBLK(SBuff.st_mode) != 0)
	{
		if (strcmp(t->arg[0], "b") == 0)
		{
			f->boolval = 1;
			return 1;
		}
		f->boolval = 0;
		return 0;
	}
	if (S_ISREG(SBuff.st_mode) != 0)
	{
		if (strcmp(t->arg[0], "f") == 0)
		{
			f->boolval = 1;
			return 1;
		}
		f->boolval = 0;
		return 0;
	}
	if (S_ISFIFO(SBuff.st_mode) != 0)
	{
		if (strcmp(t->arg[0], "p") == 0)
		{
			f->boolval = 1;
			return 1;
		}
		f->boolval = 0;
		return 0;
	}
	if (S_ISLNK(SBuff.st_mode) != 0)
	{
		if (strcmp(t->arg[0], "l") == 0)
		{
			f->boolval = 1;
			return 1;
		}
		f->boolval = 0;
		return 0;
	}
	if (S_ISSOCK(SBuff.st_mode) != 0)
	{
		if (strcmp(t->arg[0], "s") == 0)
		{
			f->boolval = 1;
			return 1;
		}
		f->boolval = 0;
		return 0;
	}
	f->boolval = 0;
	return 0;
}

int evaluate(struct token *tok, struct file *f)
{
	if (tok == NULL)
		return 0;
	if (tok->type == OR)
		return evaluate(tok->left, f) || evaluate(tok->right, f);
	else if (tok->type == AND)
		return evaluate(tok->left, f) && evaluate(tok->right, f);
	else
	{
		if (tok->type == PRINT)
			return print_func(f);
		if (tok->type == NAME)
			return name_func(f, tok);
		if (tok->type == TYPE)
			return type_func(f, tok);
		/*if (tok->type == EXEC)
			return exec_func(tok);
		if (tok->type == NEWER)
			return newer_func(tok);*/
	}
	return 0;
}
