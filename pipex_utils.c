/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gdosch <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/04 12:33:25 by gdosch            #+#    #+#             */
/*   Updated: 2025/01/04 12:33:27 by gdosch           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static void	ft_close_fds(t_pipex *data)
{
	if (data->pipe_fd[0] != -1)
		close(data->pipe_fd[0]);
	if (data->pipe_fd[1] != -1)
		close(data->pipe_fd[1]);
	if (data->io_fd[0] != -1)
		close(data->io_fd[0]);
	if (data->io_fd[1] != -1)
		close(data->io_fd[1]);
}

void	ft_cleanup(t_pipex *data, char *error_msg, int status)
{
	if (data)
	{
		ft_close_fds(data);
		if (data->paths)
			ft_free_arr((void **)data->paths);
		if (data->args)
			ft_free_arr((void **)data->args);
		if (data->cmd_path)
			free(data->cmd_path);
	}
	if (error_msg)
		perror(error_msg);
	if (status > 0)
		exit(status);
}

static char	*ft_find_cmd_path(char *cmd, char **path)
{
	char	*full_path;
	size_t	i;

	if (cmd[0] == '/' || (cmd[0] == '.' && cmd[1] == '/'))
	{
		if (access(cmd, F_OK | X_OK) == 0)
			return (ft_strdup(cmd));
		return (NULL);
	}
	i = 0;
	while (path[i])
	{
		full_path = malloc(ft_strlen(path[i]) + ft_strlen(cmd) + 2);
		if (!full_path)
			return (NULL);
		ft_memcpy(full_path, path[i], ft_strlen(path[i]));
		full_path[ft_strlen(path[i])] = '/';
		ft_strlcpy(full_path + ft_strlen(path[i]) + 1, cmd, ft_strlen(cmd) + 1);
		if (access(full_path, F_OK | X_OK) == 0)
			return (full_path);
		free(full_path);
		i++;
	}
	return (NULL);
}

void	ft_child(t_pipex *data, char **env, int *i)
{
	if (*i == 0)
		if (dup2(data->io_fd[0], 0) == -1 || dup2(data->pipe_fd[1], 1) == -1)
			ft_cleanup(data, "dup2 failed", 1);
	if (*i == 1)
		if (dup2(data->pipe_fd[0], 0) == -1
			|| dup2(data->io_fd[1], 1) == -1)
			ft_cleanup(data, "dup2 failed", 1);
	ft_close_fds(data);
	if (ft_split_args(&data->args, data->cmd[*i]) < 0)
		ft_cleanup(data, "Failed to split cmd", 1);
	data->cmd_path = ft_find_cmd_path(data->args[0], data->paths);
	if (!data->cmd_path)
		ft_cleanup(data, "Command not found", 127);
	execve(data->cmd_path, data->args, env);
	ft_cleanup(data, "execve failed", 1);
}
