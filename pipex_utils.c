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
