#include "shunting.h"
#include "tokenizer.h"
#include "stack.h"

static struct token *create_and(void)
{
	struct token *res = malloc(sizeof(struct token));
	res->type = AND;
	res->priority = 1;
	res->arg = NULL;
	res->left = NULL;
	res->right = NULL;
	return res;
}

struct token *shunting(struct tokens *tokens1)
{
    struct stack *operator = NULL;
    struct stack *operand = NULL;
    size_t i = 0;
	struct token *tmp;
	
    while (i < tokens1->len)
    {
		if (i == 0)
		{
			if (tokens1->tok[i]->type > 3)
				operand = stack_push(operand, tokens1->tok[i]);
			else
				operator = stack_push(operator, tokens1->tok[i]);
		}	
		else if (tokens1->tok[i]->type > 3)
		{
			if (tokens1->tok[i - 1]->type < 4) 
				operand = stack_push(operand, tokens1->tok[i]);
			else
			{
				operator = stack_push(operator, create_and());
				operand = stack_push(operand, tokens1->tok[i]);
			}
		}
		else
		{
			tmp = stack_peek(operator);
			if (tmp != NULL && tmp->priority > tokens1->tok[i]->priority)
			{
				tmp->right = stack_peek(operand);
				operand = stack_pop(operand);	
				tmp->left = stack_peek(operand);
				operand = stack_pop(operand);
				operand = stack_push(operand, tmp);
				operator = stack_pop(operator);
				operator = stack_push(operator, tokens1->tok[i]);
			}
			else 
			{
				operator = stack_push(operator, tokens1->tok[i]);
			}
		}
        i++;
    }
	while (stack_peek(operator) != NULL)
	{
		tmp = stack_peek(operator);
		tmp->right = stack_peek(operand);
		operand = stack_pop(operand);	
		tmp->left = stack_peek(operand);
		operand = stack_pop(operand);
		operand = stack_push(operand, tmp);
		operator = stack_pop(operator);
		i++;
	}
	tmp = stack_peek(operand);
	if (operator != NULL)
		free(operator);
	if (operand != NULL)
		free(operand);
    return tmp;
}

