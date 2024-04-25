#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "expression.h"
int check_children(struct my_expr *expr)
{
    if (expr->data.children.right == NULL || expr->data.children.left == NULL)
        exit(1);
    return 0;
}
int check_div(int b)
{
    if (b == 0)
    {
        fprintf(stderr, "DIV 0");
        exit(1);
    }
    return 0;
}
int eval_expr(struct my_expr *expr)
{
    if (expr->type == 5)
        return expr->data.value;
    switch (expr->type)
    {
    case 0:
        check_children(expr);
        return eval_expr(expr->data.children.left)
            + eval_expr(expr->data.children.right);
    case 1:
        check_children(expr);
        return eval_expr(expr->data.children.left)
            - eval_expr(expr->data.children.right);
    case 2:
        check_children(expr);
        return eval_expr(expr->data.children.left)
            * eval_expr(expr->data.children.right);
    case 3:
        check_div(eval_expr(expr->data.children.right));
        return eval_expr(expr->data.children.left)
            / eval_expr(expr->data.children.right);
    case 4:
        if (expr->data.children.right != NULL)
            exit(1);
        return -1 * eval_expr(expr->data.children.left);
    default:
        return 0;
    }
    return 0;
}
