#include "stack.h"

struct stack *stack_push(struct stack *s, struct token *elt)
{
    struct stack *inserted = malloc(sizeof(struct stack));
    inserted->elt = elt;
    if (s == NULL)
        inserted->next = NULL;
    inserted->next = s;
    return inserted;
}

struct stack *stack_pop(struct stack *s)
{
    if (s == NULL)
        return NULL;
    struct stack *newtop = s->next;
    free(s);
    return newtop;
}

struct token *stack_peek(struct stack *s)
{
    if (s == NULL)
        return NULL;
    return s->elt;
}

