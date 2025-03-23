
#include "../minishell.h"
#include <stdio.h>

const char *get_token_type(t_token_type type) {
    switch (type) {
		case COMMAND				:return "TOKEN_COMMAND";
		case DOUBLE_QUOTES			:return "TOKEN_DOUBLE_QUOTES";
		case SINGLE_QUOTES			:return "TOKEN_SINGLE_QUOTES";
		case PIPE					:return "TOKEN_PIPE";
        case REDIRECT_INP			:return "TOKEN_REDIRECT_INPUT";
		case REDIRECT_OUT			:return "TOKEN_REDIRECT_OUTPUT";
		case FILENAME_INP			:return "TOKEN_INPUT_FILENAME";
        case FILENAME_OUT			:return "TOKEN_OUTPUT_FILENAME";
		case ARGS					:return "TOKEN_OPTIONS";
		case HERE_DOC				:return "HERE_DOC";
		case APPEND					:return "APPEND";
		case APPEND_FILENAME_OUT	:return "APPEND_FILENAME_OUT";
		case HERE_DOC_OPT			:return "HERE_DOC_OPT";
		default				:return "UNKNOWN_TOKEN";

	}
}

void	print_tokens(t_token **token)
{
	t_token	*temp;

	if (!token || !*token)
		return ;
	temp = *token;
	while (temp)
	{
		printf("'%s'\t\t----->\t\t%s \n", temp->value, get_token_type(temp->type));
		temp = temp->next;
	}
	printf("\n\n\n\n\n");
}

void	p(void)
{
	printf("\n\n\t-------->DEBUG<-------\t\n\n");
}

void	print_linked(t_env **ll)
{
	t_env	*temp;

	temp = *ll;
	while (temp)
	{
		printf("%s\n", temp->str);
		temp = temp->next;
	}
}
