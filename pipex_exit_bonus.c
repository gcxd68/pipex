/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_exit_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gdosch <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/26 14:57:56 by gdosch            #+#    #+#             */
/*   Updated: 2025/01/26 14:57:58 by gdosch           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

void	ft_close_fds(t_pipex *data)
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
	if (err_msg)
		perror(err_msg);
	if (status > 0 || data->hd_pid == 0)
		exit(status);
}
