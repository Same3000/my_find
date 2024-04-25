#include "tokenizer.h"

static enum type_tok switch_token(char *str)
{
    if (strcmp(str, "-a") == 0)
        return AND;
    if (strcmp(str, "-o") == 0)
        return OR;
    if (strcmp(str, "(") == 0)
        return LEFTPAREN;
    if (strcmp(str, ")") == 0)
        return RIGHTPAREN;
    if (strcmp(str, "-name") == 0)
        return NAME;
    if (strcmp(str, "-print") == 0)
        return PRINT;
    if (strcmp(str, "-exec") == 0)
        return EXEC;
    if (strcmp(str, "-type") == 0)
        return TYPE;
    if (strcmp(str, "-newer") == 0)
        return NEWER;
    if (strcmp(str, "-perm") == 0)
        return PERM;
    if (strcmp(str, "-user") == 0)
        return USER;
    if (strcmp(str, "-group") == 0)
        return GROUP;
    if (strcmp(str, "-d") == 0)
        return D;
    return ERROR;
}
struct token *create_print(size_t argc, char *argv[])
{
    struct token *res = malloc(sizeof(struct token));
    res->type = PRINT;
    res->priority = -1;
    res->left = NULL;
    res->right = NULL;
    size_t i = 1;
    while (i < argc)
    {
        if (argv[i][0] == '-')
            break;
        i++;
    }
    size_t j = 1;
    if (i < argc && i != 1)
    {
        res->arg = malloc(sizeof(char *) * i - 1);
        while (j < i)
        {
            res->arg[j - 1] = malloc(strlen(argv[j]) + 1);
            strcpy(res->arg[j - 1], argv[j]);
            j++;
        }
        j--;
    }
    else
    {
        res->arg = malloc(sizeof(char *) * i);
        res->arg[0] = malloc(2);
        strcpy(res->arg[0], ".");
    }
    res->len_arg = j;
    return res;
}

int my_is_valid_type(struct token *t)
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
static void check_arg(struct token *t)
{
    if (t->type == PRINT || t->type == D)
        return;
    if (t->len_arg != 1)
        errx(1, "Invalid Argument");
    if (t->type == NEWER)
    {
        struct stat s;
        if (stat(t->arg[0], &s) == -1)
            errx(1, "Invalid Argument");
    }
    else if (t->type == TYPE)
    {
        if (my_is_valid_type(t) == -1)
            errx(1, "Invalid Argument");
    }
}

struct token *create_token(char **argv, size_t *i, size_t argc)
{
    struct token *res = malloc(sizeof(struct token));
    res->type = switch_token(argv[*i]);
    if (res->type == PRINT || res->type == D)
    {
        (*i)++;
        if (res->type == PRINT)
        {
            free(res);
            return create_print(argc, argv);
        }
        free(res);
        return NULL;
    }
    res->priority = -1;
    res->left = NULL;
    res->right = NULL;
    if (res->type == ERROR)
        errx(1, "unknown predicate :%s", argv[*i]);
    if (res->type < 4)
    {
        res->arg = NULL;
        if (res->type == 0)
            res->priority = 1;
        else if (res->type == 1)
            res->priority = 0;
        else
            res->priority = 4;
        (*i)++;
        return res;
    }
    (*i)++;
    size_t len = 0;
    while (*i < argc)
    {
        if (res->type == PERM)
            res->type = PERM;
        else if (argv[*i][0] == '-' || argv[*i][0] == ';')
            break;
        else if (argv[*i][0] == ')' || argv[*i][0] == '(')
            break;
        (*i)++;
        len++;
    }
    res->arg = malloc(sizeof(char *) * len);
    res->len_arg = len;
    size_t j = 0;
    while (j < len)
    {
        res->arg[j] = malloc(strlen(argv[(*i - len) + j]) + 1);
        strcpy(res->arg[j], argv[(*i - len) + j]);
        j++;
    }
    check_arg(res);
    return res;
}

struct tokens *tokenizer(size_t argc, char **argv)
{
    struct tokens *res = malloc(sizeof(struct tokens));
    struct token **data = malloc(sizeof(struct token *) * (argc + 1));
    res->tok = data;
    res->depth = 0;
    size_t i = 1;
    while (i < argc)
    {
        if (argv[i][0] == '-')
            break;
        if (argv[i][0] == ')' || argv[i][0] == '(')
            errx(1, "Invalid");
        i++;
    }
    size_t j = 0;
    res->action = 0;
    struct token *tmp;
    while (i < argc)
    {
        tmp = create_token(argv, &i, argc);
        if (tmp == NULL)
        {
            res->depth = 1;
            continue;
        }
        res->tok[j] = tmp;
        if (res->tok[j]->type == PRINT || res->tok[j]->type == EXEC)
            res->action = 1;
        j++;
    }
    res->len = j;
    return res;
}
