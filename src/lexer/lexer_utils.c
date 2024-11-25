#include "../minishell.h"

void	free_split_res(char **split_res)
{
	int i;

	i = 0;
	while (split_res[i])
	{
		free(split_res[i]);
		i++;
	}
	free(split_res);
}

char *add_spaces_around_redirection(const char *cmdline) 
{
    size_t len = ft_strlen(cmdline);
    size_t new_len = len * 3;
    char *new_cmdline = malloc(new_len + 1);
    if (!new_cmdline) return NULL;

    size_t j = 0;
    int in_quotes = 0;

    for (size_t i = 0; i < len; i++) {
        if (cmdline[i] == '"' || cmdline[i] == '\'' ) {
            in_quotes = !in_quotes;
            new_cmdline[j++] = cmdline[i];
        } else if (!in_quotes && (cmdline[i] == '|' || cmdline[i] == '<' || cmdline[i] == '>' || (cmdline[i] == '>' && cmdline[i+1] == '>'))) {
            if (i > 0 && !isspace(cmdline[i-1])) {
                new_cmdline[j++] = ' ';
            }
            new_cmdline[j++] = cmdline[i];
            if (cmdline[i] == '>' && cmdline[i+1] == '>') {
                new_cmdline[j++] = cmdline[++i];
            }
			if (cmdline[i] == '<' && cmdline[i+1] == '<') {
                new_cmdline[j++] = cmdline[++i];
            }
            if (i < len - 1 && !isspace(cmdline[i+1])) {
                new_cmdline[j++] = ' ';
            }
        } else {
            new_cmdline[j++] = cmdline[i];
        }
    }
    new_cmdline[j] = '\0';
    return new_cmdline;
}