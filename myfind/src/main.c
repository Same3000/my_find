#include "eval.h"
#include "file.h"
#include "shunting.h"
#include "simple_ls.h"
#include "tokenizer.h"

static void free_arg(struct token *t)
{
    if (t->arg == NULL)
        return;
    for (size_t i = 0; i < t->len_arg; i++)
    {
        free(t->arg[i]);
    }
    free(t->arg);
}
static void free_token(struct token *t)
{
    if (t == NULL)
        return;
    free_token(t->left);
    free_token(t->right);
    free_arg(t);
    free(t);
}
/*static char *get_enum(enum type_tok tipe)
{
    switch (tipe)
   {
      case OR: return "OR";
      case AND: return "AND";
      case EXEC: return "EXEC";
      case LEFTPAREN: return "LEFTPAREN";
      case RIGHTPAREN: return "RIGHTPAREN";
      case TYPE: return "TYPE";
      case NAME: return "NAME";
      case NEWER: return "NEWER";
      default: return "AND";

   }
    return "???";
}

static void print_rec(struct token *t)
{
    if (t != NULL)
    {
        printf("his name is %s\n", get_enum(t->type));
        print_rec(t->left);
        print_rec(t->right);
    }
}*/

int main(int argc, char *argv[])
{
    struct tokens *res = tokenizer(argc, argv);
    struct files *list = files_init(8, res->depth);
    int ret = 0;
    int i = 1;
    ret = my_ls(argc, argv, &i, list);
    if (ret == 1 && i < argc)
        my_ls(argc, argv, &i, list);
    struct token *finaltok = shunting(res);
    for (size_t i = 0; i < list->size; i++)
    {
        evaluate(finaltok, list->data[i]);
        if (res->action == 0)
            print_func(list->data[i]);
    }
    // print_rec(finaltok);
    free(res->tok);
    free_files(list);
    free(res);
    free_token(finaltok);
    return ret;
}
