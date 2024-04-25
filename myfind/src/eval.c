#include "eval.h"

int print_func(struct file *f)
{
    if (f->boolval == 1)
        printf("%s\n", f->path);
    else if (f->boolval == -1)
    {
        fprintf(stderr, "myfind: Error printfunc\n");
    }
    return 1;
}
static int name_func(struct file *f, struct token *t)
{
    if (t->len_arg != 1)
        errx(1, "Invalid");
    if (fnmatch(t->arg[0], f->name, 0) == 0)
    {
        f->boolval = 1;
        return 1;
    }
    f->boolval = 0;
    return 0;
}
static int type_func(struct file *f, struct token *t)
{
    if (t->len_arg != 1)
        errx(1, "Invalid");
    struct stat SBuff;

    if (lstat(f->path, &SBuff) == -1)
        errx(1, "Invalid");
    if (S_ISDIR(SBuff.st_mode) != 0 && strcmp(t->arg[0], "d") == 0)
    {
        f->boolval = 1;
        return 1;
    }
    if (S_ISCHR(SBuff.st_mode) != 0 && strcmp(t->arg[0], "c") == 0)
    {
        f->boolval = 1;
        return 1;
    }
    if (S_ISBLK(SBuff.st_mode) != 0 && strcmp(t->arg[0], "b") == 0)
    {
        f->boolval = 1;
        return 1;
    }
    if (S_ISREG(SBuff.st_mode) != 0 && strcmp(t->arg[0], "f") == 0)
    {
        f->boolval = 1;
        return 1;
    }
    if (S_ISFIFO(SBuff.st_mode) != 0 && strcmp(t->arg[0], "p") == 0)
    {
        f->boolval = 1;
        return 1;
    }
    if (S_ISLNK(SBuff.st_mode) != 0 && strcmp(t->arg[0], "l") == 0)
    {
        f->boolval = 1;
        return 1;
    }
    if (S_ISSOCK(SBuff.st_mode) != 0 && strcmp(t->arg[0], "s") == 0)
    {
        f->boolval = 1;
        return 1;
    }
    f->boolval = 0;
    return 0;
}

int newer_func(struct file *f, struct token *t)
{
    if (t->len_arg != 1)
        errx(1, "Invalid");
    struct stat Stok;
    struct stat Sfile;
    if (lstat(f->path, &Sfile) == -1)
        errx(1, "Invalid");
    if (lstat(t->arg[0], &Stok) == -1)
        errx(1, "Invalid");
    if (Sfile.st_mtim.tv_sec > Stok.st_mtim.tv_sec)
    {
        f->boolval = 1;
        return 1;
    }
    else if (Sfile.st_mtim.tv_sec == Stok.st_mtim.tv_sec)
    {
        if (Sfile.st_mtim.tv_nsec > Stok.st_mtim.tv_nsec)
        {
            f->boolval = 1;
            return 1;
        }
    }
    f->boolval = 0;
    return 0;
}

static int convert(int number, int base)
{
    if (number == 0 || base == 10)
        return number;
    return (number % base) + 10 * convert(number / base, base);
}
static void is_valid_perm(long test)
{
    if (test % 10 > 7)
        errx(1, "invalid mode");
    test /= 10;
    if (test % 10 > 7)
        errx(1, "invalid mode");
    test /= 10;
    if (test % 10 > 7)
        errx(1, "invalid mode");
}
static int aux_perm1(int test, int ref)
{
    for (size_t i = 0; i < 3; i++)
    {
        if (test % 10 == 0 && ref % 10 == 1)
            return 0;
        test /= 10;
        ref /= 10;
    }
    return 1;
}
static int aux_perm2(int test, int ref)
{
    for (size_t i = 0; i < 3; i++)
    {
        if (test % 10 == 1 && ref % 10 == 1)
            return 1;
        test /= 10;
        ref /= 10;
    }
    return 0;
}
static int aux_boolval(struct file *f, int ret)
{
    f->boolval = ret;
    return ret;
}
static int perm_func(struct file *f, struct token *tok)
{
    struct stat Sfile;
    lstat(f->path, &Sfile);
    int mode = Sfile.st_mode;
    char *ptr;
    long tok_mod;
    tok_mod = strtol(&tok->arg[0][1], &ptr, 10);
    is_valid_perm(tok_mod);
    mode = convert(mode, 8) % 1000;
    int tmp = mode;
    int tmp_tok = tok_mod;

    int perm1 = aux_perm1(convert(tmp % 10, 2), convert(tmp_tok % 10, 2));
    int per11 = aux_perm2(convert(tmp % 10, 2), convert(tmp_tok % 10, 2));
    tmp /= 10;
    tmp_tok /= 10;

    int perm2 = aux_perm1(convert(tmp % 10, 2), convert(tmp_tok % 10, 2));
    int per22 = aux_perm2(convert(tmp % 10, 2), convert(tmp_tok % 10, 2));
    tmp /= 10;
    tmp_tok /= 10;

    int perm3 = aux_perm1(convert(tmp, 2), convert(tmp_tok, 2));
    int per33 = aux_perm2(convert(tmp, 2), convert(tmp_tok, 2));

    if (tok->len_arg != 1)
        errx(1, "Invalid");
    if (tok->arg[0][0] == '-')
    {
        return ((perm1 == 1 && perm2 == 1) && perm3 == 1) ? aux_boolval(f, 1)
                                                          : aux_boolval(f, 0);
    }
    if (tok->arg[0][0] == '/')
    {
        return (per11 == 1 || per22 == 1) || per33 == 1 ? aux_boolval(f, 1)
                                                        : aux_boolval(f, 0);
    }
    tok_mod = strtol(tok->arg[0], &ptr, 10);
    return (mode == tok_mod) ? aux_boolval(f, 1) : aux_boolval(f, 0);
}
static int user_func(struct file *f, struct token *t)
{
    struct passwd *tok_id = getpwnam(t->arg[0]);
    if (tok_id == NULL)
        errx(1, "Invalid");
    struct stat s;
    if (lstat(f->path, &s) == -1)
        errx(1, "Invalid");
    struct passwd *file_id = getpwuid(s.st_uid);
    if (strcmp(tok_id->pw_name, file_id->pw_name) == 0
        && tok_id->pw_uid == file_id->pw_uid)
    {
        f->boolval = 1;
        return 1;
    }
    f->boolval = 0;
    return 0;
}
static int groupe_func(struct file *f, struct token *t)
{
    struct group *tok_id = getgrnam(t->arg[0]);
    if (tok_id == NULL)
        errx(1, "Invalid");
    struct stat s;
    if (lstat(f->path, &s) == -1)
        errx(1, "Invalid");
    struct group *file_id = getgrgid(s.st_gid);
    if (strcmp(tok_id->gr_name, file_id->gr_name) == 0
        && tok_id->gr_gid == file_id->gr_gid)
    {
        f->boolval = 1;
        return 1;
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
        if (tok->type == NEWER)
            return newer_func(f, tok);
        if (tok->type == PERM)
            return perm_func(f, tok);
        if (tok->type == USER)
            return user_func(f, tok);
        if (tok->type == GROUP)
            return groupe_func(f, tok);
    }
    return 0;
}
