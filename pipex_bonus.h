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

# include <fcntl.h>
# include <stdlib.h>
# include <stdio.h>
# include <string.h>
# include <unistd.h>
# include <sys/wait.h>

typedef struct s_pipex
{
	pid_t	*pid;
	char	**paths;
	char	**cmd;
	int		io_fd[2];
	int		**pipe_fd;
	int		cmd_ct;
	int		wprot;
	int		status;
}	t_pipex;

void	ft_close_fds(t_pipex *data);
void	ft_child(t_pipex *data, char **env, int *i);
void	ft_cleanup(t_pipex *data, char **args, char *error_msg, int status);

void	*ft_calloc(size_t nmemb, size_t size);
void	ft_free_arr(void **arr);
void	ft_free_int_arr(int **arr, size_t size);
void	*ft_memcpy(void *dest, const void *src, size_t n);
void	*ft_memset(void *s, int c, size_t n);
char	**ft_split(char const *s, char c);
size_t	ft_strlcpy(char *dst, const char *src, size_t size);
size_t	ft_strlen(const char *s);

#endif
