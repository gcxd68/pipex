/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_child.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gdosch <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/04 12:33:25 by gdosch            #+#    #+#             */
/*   Updated: 2025/01/04 12:33:27 by gdosch           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static char	*ft_find_cmd_path(char *cmd, char **path)
{
	const size_t	cmd_len = ft_strlen(cmd);
	size_t			path_len;
	char			*full_path;

	if (ft_strchr(cmd, '/'))
	{
		if (access(cmd, F_OK | X_OK) == 0)
			return (ft_strdup(cmd));
		return (NULL);
	}
	while (*path)
	{
		path_len = ft_strlen(*path);
		full_path = malloc(path_len + cmd_len + 2);
		if (!full_path)
			return (NULL);
		ft_memcpy(full_path, *path, path_len);
		full_path[path_len] = '/';
		ft_strlcpy(full_path + path_len + 1, cmd, cmd_len + 1);
		if (access(full_path, F_OK | X_OK) == 0)
			return (full_path);
		free(full_path);
		path++;
	}
	return (NULL);
}

void	ft_child(t_pipex *data, char **env, int curr)
{
	data->curr_cmd = data->cmd[curr];
	if (curr == 0)
		if (dup2(data->io_fd[0], 0) == -1 || dup2(data->pipe_fd[1], 1) == -1)
			ft_cleanup(data, "pipex: dup2 failed", 1);
	if (curr == 1)
		if (dup2(data->pipe_fd[0], 0) == -1
			|| dup2(data->io_fd[1], 1) == -1)
			ft_cleanup(data, "pipex: dup2 failed", 1);
	ft_close_fds(data);
	if (ft_split_args(data, curr) < 0)
		ft_cleanup(data, "pipex: failed to split cmd", 1);
	data->cmd_path = ft_find_cmd_path(data->args[0], data->paths);
	if (!data->cmd_path)
		ft_invalid_cmd(data, curr);
	execve(data->cmd_path, data->args, env);
	ft_cleanup(data, "pipex: execve failed", 1);
}
