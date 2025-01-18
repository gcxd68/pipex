/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gdosch <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/30 12:01:01 by gdosch            #+#    #+#             */
/*   Updated: 2024/12/30 12:01:03 by gdosch           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_BONUS_H
# define PIPEX_BONUS_H

# include "libft/libft.h"
# include <errno.h>
# include <fcntl.h>
# include <stdbool.h>
# include <string.h>
# include <sys/wait.h>

typedef struct s_pipex
{
	char	**args;
	char	**cmd;
	char	**paths;
	int		**pipe_fd;
	char	*def_paths[6];
	char	*cmd_path;
	char	*curr_cmd;
	char	*line;
	pid_t	*pid;
	int		hd_fd[2];
	int		io_fd[2];
	int		cmd_ct;
	int		env;
	int		here_doc;
	int		status;
	int		wprot;
	pid_t	hd_pid;
}	t_pipex;

void	ft_child(t_pipex *data, char **env, int *i);
void	ft_cleanup(t_pipex *data, char *error_msg, int status);
void	ft_here_doc(t_pipex *data, char *limiter);
int		ft_split_args(char ***args, char *cmd);

#endif
