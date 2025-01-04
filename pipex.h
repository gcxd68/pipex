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
	pid_t	pid[2];
	char	**paths;
	char	*cmd[2];
	int		io_fd[2];
	int		pipe_fd[2];
}	t_pipex;


void	ft_first_child(t_pipex *data, char **env);
void	ft_last_child(t_pipex *data, char **env);

void	ft_free_arr(void **arr);
void	*ft_memcpy(void *dest, const void *src, size_t n);
char	**ft_split(char const *s, char c);
size_t	ft_strlcpy(char *dst, const char *src, size_t size);
size_t	ft_strlen(const char *s);

#endif
