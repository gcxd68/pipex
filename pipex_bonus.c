/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gdosch <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/30 12:00:50 by gdosch            #+#    #+#             */
/*   Updated: 2024/12/30 12:00:52 by gdosch           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

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

static void	ft_init_io(t_pipex *data, char *argv[])
{
	ft_memset(data->io_fd, -1, sizeof(data->io_fd));
	if (access(argv[1], F_OK) == -1 || access(argv[1], R_OK) == -1)
	{
		perror("Infile does not exist or cannot be read, using /dev/null");
		data->io_fd[0] = open("/dev/null", O_RDONLY);
	}
	else
		data->io_fd[0] = open(argv[1], O_RDONLY);
	if (data->io_fd[0] == -1)
		ft_cleanup_parent(data, "Failed to open infile");
	if (access(argv[data->cmd_ct + 2], F_OK) == 0
		&& access(argv[data->cmd_ct + 2], W_OK) == -1)
	{
		data->wprot = 1;
		perror("No write permission for outfile, using /dev/null");
		data->io_fd[1] = open("/dev/null", O_WRONLY);
	}
	else
		data->io_fd[1]
			= open(argv[data->cmd_ct + 2], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (data->io_fd[1] == -1)
		ft_cleanup_parent(data, "Failed to open outfile");
}

static void	ft_init_data(t_pipex *data, char *argv[])
{
	int	i;

	data->pid = malloc(sizeof(pid_t) * data->cmd_ct);
	if (!data->pid)
		ft_cleanup_parent(data, "Failed to allocate memory for pids");
	data->cmd = ft_calloc(data->cmd_ct + 1, sizeof(char *));
	if (!data->cmd)
		ft_cleanup_parent(data, "Failed to allocate memory for cmds");
	i = -1;
	while (++i < data->cmd_ct)
		data->cmd[i] = argv [i + 2];
	data->pipe_fd = malloc(sizeof(int *) * (data->cmd_ct - 1));
	if (!data->pipe_fd)
		ft_cleanup_parent(data, "Failed to allocate memory for pipes");
	i = -1;
	while (++i < data->cmd_ct - 1)
	{
		data->pipe_fd[i] = malloc(sizeof(int) * 2);
		if (!data->pipe_fd[i])
			ft_cleanup_parent(data, "Failed to allocate memory for a pipe");
		ft_memset(data->pipe_fd[i], -1, sizeof(int) * 2);
	}
}

static void	ft_pipeline(t_pipex *data, char **env)
{
	int	i;

	data->paths = ft_get_paths(env);
	if (!data->paths)
		ft_cleanup_parent(data, "Failed to get PATH");
	i = -1;
	while (++i < data->cmd_ct - 1)
		if (pipe(data->pipe_fd[i]) == -1)
			ft_cleanup_parent(data, "pipe failed");
	i = -1;
	while (++i < data->cmd_ct)
	{
		data->pid[i] = fork();
		if (data->pid[i] == -1)
			ft_cleanup_parent(data, "fork failed");
		if (data->pid[i] == 0)
			exit(ft_child(data, env, &i));
	}
	ft_cleanup_parent(data, NULL);
}

int	main(int argc, char *argv[], char **env)
{
	t_pipex	data;
	int		i;

	data = (t_pipex){0};
	data.cmd_ct = argc - 3;
	if (data.cmd_ct < 2)
		return (write(2, "Usage: ./pipex file1 cmd1 ... cmdn file2\n", 41), 0);
	ft_init_io(&data, argv);
	ft_init_data(&data, argv);
	ft_pipeline(&data, env);
	i = -1;
	while (++i < data.cmd_ct)
		if (waitpid(data.pid[i], &data.status, 0) == -1)
			perror("waitpid failed");
	if (data.pid)
		free(data.pid);
	if (data.wprot == 1)
		return (data.wprot);
	if (WIFEXITED(data.status))
		return (WEXITSTATUS(data.status));
	return (EXIT_FAILURE);
}
