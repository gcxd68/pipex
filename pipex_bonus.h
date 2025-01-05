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

# include <ctype.h>
# include <errno.h>
# include <fcntl.h>
# include <limits.h>
# include <stddef.h>
# include <stdint.h>
# include <stdlib.h>
# include <stdarg.h>
# include <stdio.h>
# include <string.h>
# include <strings.h>
# include <time.h>
# include <unistd.h>
# include <bsd/string.h>
# include <sys/types.h>
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
int		ft_child(t_pipex *data, char **env, int *i);
void	ft_cleanup_parent(t_pipex *data, char *error_msg);

void	*ft_calloc(size_t nmemb, size_t size);
void	ft_free_arr(void **arr);
void	*ft_memcpy(void *dest, const void *src, size_t n);
void	*ft_memset(void *s, int c, size_t n);
int		ft_printf(const char *format, ...)
		__attribute__((format(printf, 1, 2)));
char	**ft_split(char const *s, char c);
size_t	ft_strlcpy(char *dst, const char *src, size_t size);
size_t	ft_strlen(const char *s);

#endif
