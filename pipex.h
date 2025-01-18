/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gdosch <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/30 12:01:01 by gdosch            #+#    #+#             */
/*   Updated: 2024/12/30 12:01:03 by gdosch           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include "libft/libft.h"
# include <errno.h>
# include <fcntl.h>
# include <stdbool.h>
# include <string.h>
# include <sys/wait.h>

typedef struct s_pipex
{
	char	**args;
	char	**paths;
	char	*cmd[2];
	char	*def_paths[6];
	char	*cmd_path;
	char	*curr_cmd;
	int		io_fd[2];
	int		pipe_fd[2];
	pid_t	pid[2];
	int		env;
	int		status;
	int		wprot;
}	t_pipex;

void	ft_child(t_pipex *data, char **env, int *i);
void	ft_cleanup(t_pipex *data, char *error_msg, int status);
int		ft_split_args(char ***args, char *cmd);

#endif
