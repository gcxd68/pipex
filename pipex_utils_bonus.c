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
		while (++i < data->cmd_ct - 1 && data->pipe_fd[i])
		{
			if (data->pipe_fd[i][0] != -1)
				close(data->pipe_fd[i][0]);
			if (data->pipe_fd[i][1] != -1)
				close(data->pipe_fd[i][1]);
			ft_memset(data->pipe_fd[i], -1, sizeof(int) * 2);
		}
	}
	if (data->here_doc)
	{
		i = -1;
		while (++i < 2 && data->hd_fd[i] != -1)
			close(data->hd_fd[i]);
		ft_memset(data->hd_fd, -1, sizeof(data->hd_fd));
	}
	i = -1;
	while (++i < 2 && data->io_fd[i] != -1)
		close(data->io_fd[i]);
	ft_memset(data->io_fd, -1, sizeof(data->io_fd));
}

void	ft_cleanup(t_pipex *data, char *err_msg, int status)
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
		if (data->pid && (status > 0 || data->hd_pid == 0))
			free(data->pid);
	}
	if (err_msg && status == 127 && errno == ENOENT)
		ft_fprintf(2, "%s: %s\n", data->curr_cmd, err_msg);
	else if (err_msg && status == 127)
		ft_fprintf(2, "%s: %s: %s\n", data->curr_cmd, err_msg, strerror(errno));
	else if (err_msg)
		perror(err_msg);
	if (status > 0 || data->hd_pid == 0)
		exit(status);
}

static char	*ft_find_cmd_path(char *cmd, char **path)
{
	const size_t	cmd_len = ft_strlen(cmd);
	char			*full_path;
	size_t			i;

	if (ft_strchr(cmd, '/'))
	{
		if (access(cmd, F_OK | X_OK) == 0)
			return (ft_strdup(cmd));
		return (NULL);
	}
	i = 0;
	while (path[i])
	{
		full_path = malloc(ft_strlen(path[i]) + cmd_len + 2);
		if (!full_path)
			return (NULL);
		ft_memcpy(full_path, path[i], ft_strlen(path[i]));
		full_path[ft_strlen(path[i])] = '/';
		ft_strlcpy(full_path + ft_strlen(path[i]) + 1, cmd, cmd_len + 1);
		if (access(full_path, F_OK | X_OK) == 0)
			return (full_path);
		free(full_path);
		i++;
	}
	return (NULL);
}

void	ft_here_doc(t_pipex *data, char *limiter, size_t limiter_len)
{
	if (pipe(data->hd_fd) == -1)
		ft_cleanup(data, "pipex: here_doc pipe failed", 1);
	data->hd_pid = fork();
	if (data->hd_pid == -1)
		ft_cleanup(data, "pipex: here_doc fork failed", 1);
	if (data->hd_pid == 0)
	{
		while (1)
		{
			write(1, "> ", 2);
			data->line = get_next_line(0);
			if (!data->line || (ft_strncmp(data->line, limiter,
						limiter_len) == 0 && data->line[limiter_len] == '\n'))
			{
				free(data->line);
				break ;
			}
			write(data->hd_fd[1], data->line, ft_strlen(data->line));
			free(data->line);
		}
		ft_cleanup(data, NULL, 0);
	}
	close(data->hd_fd[1]);
	waitpid(data->hd_pid, NULL, 0);
	data->io_fd[0] = data->hd_fd[0];
}

void	ft_child(t_pipex *data, char **env, int *i)
{
	data->curr_cmd = data->cmd[*i];
	if (*i == 0)
		if (dup2(data->io_fd[0], 0) == -1 || dup2(data->pipe_fd[0][1], 1) == -1)
			ft_cleanup(data, "pipex: dup2 failed", 1);
	if (*i == data->cmd_ct - 1)
		if (dup2(data->pipe_fd[data->cmd_ct - 2][0], 0) == -1
			|| dup2(data->io_fd[1], 1) == -1)
			ft_cleanup(data, "pipex: dup2 failed", 1);
	if (*i > 0 && *i < data->cmd_ct - 1)
		if (dup2(data->pipe_fd[*i - 1][0], 0) == -1
			|| dup2(data->pipe_fd[*i][1], 1) == -1)
			ft_cleanup(data, "pipex: dup2 failed", 1);
	ft_close_fds(data);
	if (ft_split_args(&data->args, data->curr_cmd) < 0)
		ft_cleanup(data, "pipex: failed to split cmd", 1);
	data->cmd_path = ft_find_cmd_path(data->args[0], data->paths);
	if (!data->cmd_path)
	{
		if (errno == ENOENT && ((data->in_err && *i == 0)
				|| (data->out_err && *i == data->cmd_ct - 1)))
			ft_cleanup(data, NULL, 127);
		ft_cleanup(data, "command not found", 127);
	}
	execve(data->cmd_path, data->args, env);
	ft_cleanup(data, "pipex: execve failed", 1);
}
