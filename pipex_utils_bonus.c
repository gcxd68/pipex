/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gdosch <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/04 12:33:25 by gdosch            #+#    #+#             */
/*   Updated: 2025/01/04 12:33:27 by gdosch           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

void	ft_close_fds(t_pipex *data)
{
	int	i;

	if (data->pipe_fd)
	{
		i = -1;
		while (++i < data->cmd_ct - 1)
		{
			if (data->pipe_fd[i][0] != -1)
				close(data->pipe_fd[i][0]);
			if (data->pipe_fd[i][1] != -1)
				close(data->pipe_fd[i][1]);
		}
	}
	if (data->io_fd[0] != -1)
		close(data->io_fd[0]);
	if (data->io_fd[1] != -1)
		close(data->io_fd[1]);
}

void	ft_cleanup_parent(t_pipex *data, char *error_msg)
{
	ft_close_fds(data);
	if (data->pipe_fd)
		ft_free_int_arr(data->pipe_fd, data->cmd_ct - 1);
	if (data->cmd)
		free(data->cmd);
	if (data->paths)
		ft_free_arr((void **)data->paths);
	if (error_msg)
	{
		if (data->pid)
			free(data->pid);
		perror(error_msg);
		exit(EXIT_FAILURE);
	}
}

static int	ft_cleanup_child(char **args, char **paths, char *error_msg)
{
	if (args)
		ft_free_arr((void **)args);
	if (paths)
		ft_free_arr((void **)paths);
	if (error_msg)
	{
		perror(error_msg);
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
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

int	ft_child(t_pipex *data, char **env, int *i)
{
	char	**args;
	char	*cmd_path;

	args = ft_split(data->cmd[*i], ' ');
	if (!args)
		return (ft_cleanup_child(NULL, NULL, "Failed to split cmd"));
	if (*i == 0)
		if (dup2(data->io_fd[0], 0) == -1 || dup2(data->pipe_fd[0][1], 1) == -1)
			return (ft_cleanup_child(args, NULL, "dup2 failed"));
	if (*i == data->cmd_ct - 1)
		if (dup2(data->pipe_fd[data->cmd_ct - 2][0], 0) == -1
			|| dup2(data->io_fd[1], 1) == -1)
			return (ft_cleanup_child(args, NULL, "dup2 failed"));
	if (*i > 0 && *i < data->cmd_ct - 1)
		if (dup2(data->pipe_fd[*i - 1][0], 0) == -1
			|| dup2(data->pipe_fd[*i][1], 1) == -1)
			return (ft_cleanup_child(args, NULL, "dup2 failed"));
	ft_close_fds(data);
	cmd_path = ft_find_cmd_path(args[0], data->paths);
	if (!cmd_path)
		return (ft_cleanup_child(args, data->paths, "Command not found"), 127);
	execve(cmd_path, args, env);
	return (free(cmd_path), ft_cleanup_child(args, NULL, "execve failed"));
}
