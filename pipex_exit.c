/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_exit.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gdosch <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/26 14:57:20 by gdosch            #+#    #+#             */
/*   Updated: 2025/01/26 14:57:22 by gdosch           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	ft_close_fds(t_pipex *data)
{
	if (data->pipe_fd[0] != -1)
		close(data->pipe_fd[0]);
	if (data->pipe_fd[1] != -1)
		close(data->pipe_fd[1]);
	ft_memset(data->pipe_fd, -1, sizeof(data->pipe_fd));
	if (data->io_fd[0] != -1)
		close(data->io_fd[0]);
	if (data->io_fd[1] != -1)
		close(data->io_fd[1]);
	ft_memset(data->io_fd, -1, sizeof(data->io_fd));
}

void	ft_cleanup(t_pipex *data, char *err_msg, int status)
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
	if (err_msg)
		perror(err_msg);
	if (status > 0)
		exit(status);
}

int	ft_is_executable(t_pipex *data, int curr)
{
	if (access(data->cmd[curr], F_OK | X_OK) == 0)
	{
		data->args = ft_calloc(2, sizeof(char *));
		if (!data->args)
			return (-1);
		data->args[0] = ft_strdup(data->cmd[curr]);
		if (!data->args[0])
			return (free(data->args), data->args = 0, -1);
	}
	else if (access(data->cmd[curr], F_OK) == 0)
	{
		ft_dprintf(2, "pipex: %s: %s\n", data->cmd[curr], strerror(errno));
		ft_cleanup(data, NULL, 126);
	}
	return (0);
}

void	ft_is_no_directory(t_pipex *data)
{
	int	fd;

	fd = open(data->args[0], O_DIRECTORY);
	if (fd == -1)
	{
		if (errno == ENOTDIR)
			return ;
		else if (errno != ENOENT && errno != EACCES)
			ft_cleanup(data, "pipex: open failed", 1);
	}
	else
	{
		close(fd);
		ft_dprintf(2, "pipex: %s: %s\n", data->args[0], strerror(EISDIR));
		ft_cleanup(data, NULL, 126);
	}
}

void	ft_invalid_cmd(t_pipex *data, int curr)
{
	if (errno == EACCES)
	{
		ft_dprintf(2, "pipex: %s: %s\n", data->args[0], strerror(errno));
		ft_cleanup(data, NULL, 126);
	}
	else if (errno == ENOENT)
	{
		if (!data->args || !data->args[0] || *data->args[0] == '\0')
			ft_dprintf(2, "pipex: %s: %s\n", "''", strerror(errno));
		else if (!(data->in_err && curr == 0) && !(data->out_err && curr == 1))
		{
			if (ft_strchr(data->args[0], '/'))
				ft_dprintf(2, "pipex: %s: %s\n",
					data->args[0], strerror(errno));
			else
				ft_dprintf(2, "%s: %s\n", data->args[0], "command not found");
		}
		ft_cleanup(data, NULL, 127);
	}
	else
	{
		ft_dprintf(2, "%s: cmd failure: %s\n", data->args[0], strerror(errno));
		ft_cleanup(data, NULL, 1);
	}
}
