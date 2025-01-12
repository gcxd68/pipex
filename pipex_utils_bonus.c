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

static void	ft_close_fds(t_pipex *data)
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
	if (data->here_doc)
	{
		if (data->hd_fd[0] != -1)
			close(data->hd_fd[0]);
		if (data->hd_fd[1] != -1)
			close(data->hd_fd[1]);
	}
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
		if (data->pipe_fd)
			ft_free_int_arr(data->pipe_fd, data->cmd_ct - 1);
		if (data->paths)
			ft_free_arr((void **)data->paths);
		if (data->args)
			ft_free_arr((void **)data->args);
		if (data->cmd)
			free(data->cmd);
		if (data->cmd_path)
			free(data->cmd_path);
		if (data->pid && status > 0)
			free(data->pid);
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

void	ft_here_doc(t_pipex *data, char *limiter)
{
	data->hd_pid = fork();
	if (data->hd_pid == -1)
		ft_cleanup(data, "Failed to fork for here_doc", 1);
	if (data->hd_pid == 0)
	{
		while (1)
		{
			write(1, "heredoc> ", 9);
			data->line = get_next_line(0);
			if (!data->line)
				break ;
			if (ft_strncmp(data->line, limiter, ft_strlen(limiter)) == 0
				&& data->line[ft_strlen(limiter)] == '\n')
			{
				free(data->line);
				break ;
			}
			write(data->hd_fd[1], data->line, ft_strlen(data->line));
			free(data->line);
		}
		ft_close_fds(data);
		exit(0);
	}
	close(data->hd_fd[1]);
	waitpid(data->hd_pid, NULL, 0);
}

void	ft_child(t_pipex *data, char **env, int *i)
{
	if (*i == 0)
		if (dup2(data->io_fd[0], 0) == -1 || dup2(data->pipe_fd[0][1], 1) == -1)
			ft_cleanup(data, "dup2 failed", 1);
	if (*i == data->cmd_ct - 1)
		if (dup2(data->pipe_fd[data->cmd_ct - 2][0], 0) == -1
			|| dup2(data->io_fd[1], 1) == -1)
			ft_cleanup(data, "dup2 failed", 1);
	if (*i > 0 && *i < data->cmd_ct - 1)
		if (dup2(data->pipe_fd[*i - 1][0], 0) == -1
			|| dup2(data->pipe_fd[*i][1], 1) == -1)
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
