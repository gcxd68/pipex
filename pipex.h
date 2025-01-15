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

# include <errno.h>
# include <fcntl.h>
# include <stdbool.h>
# include <stdlib.h>
# include <stdio.h>
# include <string.h>
# include <unistd.h>
# include <sys/wait.h>

typedef struct s_pipex
{
	char	**args;
	char	**paths;
	char	*cmd[2];
	char	*cmd_path;
	int		io_fd[2];
	int		pipe_fd[2];
	pid_t	pid[2];
	int		status;
	int		wprot;
}	t_pipex;

void	ft_child(t_pipex *data, char **env, int *i);
void	ft_cleanup(t_pipex *data, char *error_msg, char *cmd, int status);
int		ft_split_args(char ***args, char *cmd);

int		ft_fprintf(int fd, const char *format, ...)
		__attribute__((format(printf, 2, 3)));
void	ft_free_arr(void **arr);
void	*ft_memcpy(void *dest, const void *src, size_t n);
void	*ft_memset(void *s, int c, size_t n);
void	*ft_realloc(void *ptr, size_t old_size, size_t new_size);
char	**ft_split(char const *s, char c);
char	*ft_strchr(const char *s, int c);
char	*ft_strdup(char *s);
size_t	ft_strlcpy(char *dst, const char *src, size_t size);
size_t	ft_strlen(const char *s);
int		ft_strncmp(const char *s1, const char *s2, size_t n);

#endif
