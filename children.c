/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   children.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gdosch <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/04 12:33:25 by gdosch            #+#    #+#             */
/*   Updated: 2025/01/04 12:33:27 by gdosch           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static int	ft_cleanup_child(char **args, char *cmd_path, char *error_msg)
{
	if (args)
		ft_free_arr((void **)args);
	if (cmd_path)
		free(cmd_path);
	if (error_msg)
	{
		perror(error_msg);
		return (-1);
	}
	return (0);
}

static char	*ft_find_cmd_path(char *cmd, char **path)
{
	char	*full_path;
	size_t	len_path;
	size_t	len_cmd;
	size_t	i;

	i = 0;
	len_cmd = ft_strlen(cmd);
	while (path[i])
	{
		len_path = ft_strlen(path[i]);
		full_path = malloc(len_path + len_cmd + 2);
		if (!full_path)
			return (NULL);
		ft_memcpy(full_path, path[i], len_path);
		full_path[len_path] = '/';
		ft_strlcpy(full_path + len_path + 1, cmd, len_cmd + 1);
		if (access(full_path, X_OK) == 0)
			return (full_path);
		free(full_path);
		i++;
	}
	return (NULL);
}

void	ft_first_child(t_pipex *data, char **env)
{
	char	**args;
	char	*cmd_path;

	args = ft_split(data->cmd[0], ' ');
	if (!args)
		exit(ft_cleanup_child(NULL, NULL, "Failed to split cmd1"));
	close(data->pipe_fd[0]);
	if (dup2(data->io_fd[0], 0) == -1 || dup2(data->pipe_fd[1], 1) == -1)
		exit(ft_cleanup_child(args, NULL, "dup2 failed"));
	close(data->pipe_fd[1]);
	close(data->io_fd[0]);
	cmd_path = ft_find_cmd_path(args[0], data->paths);
	if (!cmd_path)
		exit(ft_cleanup_child(args, NULL, "Command not found"));
	execve(cmd_path, args, env);
	exit(ft_cleanup_child(args, cmd_path, "execve failed"));
}

void	ft_last_child(t_pipex *data, char **env)
{
	char	**args;
	char	*cmd_path;

	args = ft_split(data->cmd[1], ' ');
	if (!args)
		exit(ft_cleanup_child(NULL, NULL, "Failed to split cmd1"));
	close(data->pipe_fd[1]);
	if (dup2(data->pipe_fd[0], 0) == -1 || dup2(data->io_fd[1], 1) == -1)
		exit(ft_cleanup_child(args, NULL, "dup2 failed"));
	close(data->pipe_fd[0]);
	close(data->io_fd[1]);
	cmd_path = ft_find_cmd_path(args[0], data->paths);
	if (!cmd_path)
		exit(ft_cleanup_child(args, NULL, "Command not found"));
	execve(cmd_path, args, env);
	exit(ft_cleanup_child(args, cmd_path, "execve failed"));
}
