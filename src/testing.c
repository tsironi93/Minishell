
#include "../minishell.h"

const char *get_token_type(t_token_type type) {
    switch (type) {
		case COMMAND		:return "TOKEN_COMMAND";
		case DOUBLE_QUOTES	:return "TOKEN_DOUBLE_QUOTES";
		case SINGLE_QUOTES	:return "TOKEN_SINGLE_QUOTES";
		case PIPE			:return "TOKEN_PIPE";
        case REDIRECT_INP	:return "TOKEN_REDIRECT_INPUT";
		case REDIRECT_OUT	:return "TOKEN_REDIRECT_OUTPUT";
		case FILENAME_INP	:return "TOKEN_INPUT_FILENAME";
        case FILENAME_OUT	:return "TOKEN_OUTPUT_FILENAME";
		case ARGS			:return "TOKEN_OPTIONS";
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
}
