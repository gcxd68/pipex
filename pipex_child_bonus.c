/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_child_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gdosch <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/04 12:33:25 by gdosch            #+#    #+#             */
/*   Updated: 2025/01/04 12:33:27 by gdosch           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

static int	ft_is_executable(t_pipex *data, int curr)
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

static void	ft_is_no_directory(t_pipex *data)
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

static int	ft_find_cmd_path(t_pipex *data, char *cmd, char **path)
{
	const size_t	cmd_len = ft_strlen(cmd);
	size_t			path_len;
	char			*full_path;

	if (ft_strchr(cmd, '/'))
	{
		if (access(cmd, F_OK | X_OK) == 0)
			return (data->cmd_path = ft_strdup(cmd), 0);
		return (-1);
	}
	while (*path)
	{
		path_len = ft_strlen(*path);
		full_path = malloc(path_len + cmd_len + 2);
		if (!full_path)
			return (-1);
		ft_memcpy(full_path, *path, path_len);
		full_path[path_len] = '/';
		ft_strlcpy(full_path + path_len + 1, cmd, cmd_len + 1);
		if (access(full_path, F_OK | X_OK) == 0)
			return (data->cmd_path = full_path, 0);
		free(full_path);
		path++;
	}
	return (-1);
}

static void	ft_invalid_cmd(t_pipex *data, int curr)
{
	if (errno == EACCES)
	{
		ft_dprintf(2, "pipex: %s: %s\n", data->args[0], strerror(errno));
		ft_cleanup(data, NULL, 126);
	}
	else if (errno == ENOENT)
	{
		if (!(data->in_err && curr == 0) && !(data->out_err && curr == 1))
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

void	ft_child(t_pipex *data, char **env, int curr)
{
	if (!data->cmd[curr] || *data->cmd[curr] == '\0')
	{
		write(2, "'': command not found\n", 22);
		ft_cleanup(data, NULL, 127);
	}
	if (curr == 0)
		if (dup2(data->io_fd[0], 0) == -1 || dup2(data->pipe_fd[0][1], 1) == -1)
			ft_cleanup(data, "pipex: dup2 failed", 1);
	if (curr == data->cmd_ct - 1)
		if (dup2(data->pipe_fd[data->cmd_ct - 2][0], 0) == -1
			|| dup2(data->io_fd[1], 1) == -1)
			ft_cleanup(data, "pipex: dup2 failed", 1);
	if (curr > 0 && curr < data->cmd_ct - 1)
		if (dup2(data->pipe_fd[curr - 1][0], 0) == -1
			|| dup2(data->pipe_fd[curr][1], 1) == -1)
			ft_cleanup(data, "pipex: dup2 failed", 1);
	ft_close_fds(data);
	if (ft_is_executable(data, curr) < 0
		|| ft_split_args(&data->args, data->cmd[curr]) < 0)
		ft_cleanup(data, "pipex: failed to split cmd", 1);
	ft_is_no_directory(data);
	if (ft_find_cmd_path(data, data->args[0], data->paths) < 0)
		ft_invalid_cmd(data, curr);
	execve(data->cmd_path, data->args, env);
	ft_cleanup(data, "pipex: execve failed", 1);
}
