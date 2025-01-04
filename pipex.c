/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gdosch <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/30 12:00:50 by gdosch            #+#    #+#             */
/*   Updated: 2024/12/30 12:00:52 by gdosch           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static void	ft_cleanup_parent(t_pipex *data, char *error_msg)
{
	if (data->paths)
		ft_free_arr((void **)data->paths);
	if (data->pipe_fd[0] != -1)
		close(data->pipe_fd[0]);
	if (data->pipe_fd[1] != -1)
		close(data->pipe_fd[1]);
	if (data->io_fd[0] != -1)
		close(data->io_fd[0]);
	if (data->io_fd[1] != -1)
		close(data->io_fd[1]);
	if (error_msg)
	{
		perror(error_msg);
		exit(EXIT_FAILURE);
	}
}

static char	**ft_get_paths(char **env)
{
	size_t	i;
	char	**paths;

	i = 0;
	if (!env)
		return (NULL);
	while (env[i] && strncmp(env[i], "PATH=", 5) != 0)
		i++;
	if (!env[i])
		return (NULL);
	paths = ft_split(env[i] + 5, ':');
	if (!paths)
		return (NULL);
	return (paths);
}

static void	ft_init_fd(t_pipex *data, int argc, char *argv[])
{
	if (access(argv[1], F_OK) == -1 || access(argv[1], R_OK) == -1)
	{
		perror("Infile does not exist or cannot be read, using /dev/null");
		data->io_fd[0] = open("/dev/null", O_RDONLY);
	}
	else
		data->io_fd[0] = open(argv[1], O_RDONLY);
	if (data->io_fd[0] == -1)
		ft_cleanup_parent(data, "Failed to open infile");
	if (access(argv[argc - 1], F_OK) == 0 && access(argv[argc - 1], W_OK) == -1)
	{
		data->wprot = 1;
		perror("No write permission for outfile, using /dev/null");
		data->io_fd[1] = open("/dev/null", O_WRONLY);
	}
	else
		data->io_fd[1]
			= open(argv[argc - 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (data->io_fd[1] == -1)
		ft_cleanup_parent(data, "Failed to open outfile");
}

static void	ft_pipeline(t_pipex *data, char **env)
{
	if (pipe(data->pipe_fd) == -1)
		ft_cleanup_parent(data, "pipe failed");
	data->paths = ft_get_paths(env);
	if (!data->paths)
		ft_cleanup_parent(data, "Failed to get PATH");
	data->pid[0] = fork();
	if (data->pid[0] == -1)
		ft_cleanup_parent(data, "fork failed");
	if (data->pid[0] == 0)
		exit(ft_first_child(data, env));
	data->pid[1] = fork();
	if (data->pid[1] == -1)
		ft_cleanup_parent(data, "fork failed");
	if (data->pid[1] == 0)
		exit(ft_last_child(data, env));
	ft_cleanup_parent(data, NULL);
}

int	main(int argc, char *argv[], char **env)
{
	t_pipex	data;
	int		i;

	data = (t_pipex){0};
	data.io_fd[0] = -1;
	data.io_fd[1] = -1;
	data.pipe_fd[0] = -1;
	data.pipe_fd[1] = -1;
	if (argc != 5)
		return (ft_putendl_fd("Usage: ./pipex infile cmd1 cmd2 outfile", 2), 0);
	i = -1;
	while (++i < argc - 3)
		data.cmd[i] = argv [i + 2];
	ft_init_fd(&data, argc, argv);
	ft_pipeline(&data, env);
	i = -1;
	while (++i < argc - 3)
		if (waitpid(data.pid[i], &data.status, 0) == -1)
			perror("waitpid failed");
	if (data.wprot == 1)
		return (data.wprot);
	if (WIFEXITED(data.status))
		return (WEXITSTATUS(data.status));
	return (EXIT_FAILURE);
}
