/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: itsiros <itsiros@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 15:36:48 by itsiros           #+#    #+#             */
/*   Updated: 2025/03/28 19:58:57 by itsiros          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static t_token *search_next_command(t_token **token)
{
    t_token *temp = *token;
    
    // Find next PIPE
    while (temp && temp->type != PIPE)
        temp = temp->next;
    
    // If PIPE found, move to next COMMAND
    if (temp && temp->type == PIPE)
        temp = temp->next;
    
    // Return the COMMAND token or NULL
    return (temp && temp->type == COMMAND) ? temp : NULL;
}

static void redirect_pipe(int fd[][2], int num_pipes, int process_index, t_data *data, t_token **token)
{
    t_token *temp = *token;
    
    // Navigate to correct command
    for (int j = 0; j < process_index; j++)
        temp = search_next_command(&temp);
    
    // Input redirection
    if (process_index > 0)
    {
        dup2(fd[process_index - 1][0], STDIN_FILENO);
        close(fd[process_index - 1][0]);
    }
    
    // Output redirection
    if (process_index < num_pipes)
    {
        dup2(fd[process_index][1], STDOUT_FILENO);
        close(fd[process_index][1]);
    }
    
    // Close all pipe file descriptors
    for (int j = 0; j < num_pipes; j++)
    {
        close(fd[j][0]);
        close(fd[j][1]);
    }
    
    // Execute command
    try_to_exec(data, &temp, data->env_paths);
}

void handle_pipeline(t_token **token, t_data *data, int num_pipes)
{
    int fd[64][2];
    pid_t pid[65];
    int i;
    
    // Create pipes
    for (i = 0; i < num_pipes; i++)
    {
        if (pipe(fd[i]) == -1)
        {
            perror("pipe");
            return;
        }
    }
    
    // Fork processes
    for (i = 0; i < num_pipes + 1; i++)
    {
        pid[i] = fork();
        
        if (pid[i] < 0)
        {
            perror("fork");
            return;
        }
        
        if (pid[i] == 0)  // Child process
        {
            redirect_pipe(fd, num_pipes, i, data, token);
            exit(1);  // Ensure child exits if exec fails
        }
    }
    
    // Close all pipes in parent
    for (i = 0; i < num_pipes; i++)
    {
        close(fd[i][0]);
        close(fd[i][1]);
    }
    
    // Wait for all child processes
    for (i = 0; i < num_pipes + 1; i++)
        waitpid(pid[i], NULL, 0);
}
