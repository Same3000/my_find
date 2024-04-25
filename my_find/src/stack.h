#ifndef STACK_H
#define STACK_H

#include <stdlib.h>
#include <stddef.h>

struct stack
{
    struct token *elt;
    struct stack *next;
};

struct stack *stack_push(struct stack *s, struct token *elt);
struct stack *stack_pop(struct stack *s);
struct token *stack_peek(struct stack *s);

#endif /* STACK_H */
