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

static void	ft_get_paths(t_pipex *data, char **env)
{
	size_t	i;

	if (!env || !*env)
	{
		data->paths = malloc(sizeof(char *) * 6);
		if (!data->paths)
			ft_cleanup(data, "Failed to split PATH", NULL, 1);
		data->paths[0] = ft_strdup("/usr/local/bin");
		data->paths[1] = ft_strdup("/usr/bin");
		data->paths[2] = ft_strdup("/bin");
		data->paths[3] = ft_strdup("/usr/sbin");
		data->paths[4] = ft_strdup("/sbin");
		data->paths[5] = NULL;
		i = -1;
		while (++i < 5)
			if (!data->paths[i])
				ft_cleanup(data, "Failed to get PATH", NULL, 1);
		return ;
	}
	while (env && *env && ft_strncmp(*env, "PATH=", 5) != 0)
		env++;
	data->paths = ft_split(*env + 5, ':');
	if (!data->paths)
		ft_cleanup(data, "Failed to get PATH", NULL, 1);
}

static void	ft_init_io(t_pipex *data, char *infile, char *outfile)
{
	ft_memset(data->io_fd, -1, sizeof(data->io_fd));
	if (access(outfile, F_OK) == 0 && access(outfile, W_OK) == -1)
	{
		data->wprot = 1;
		data->io_fd[1] = open("/dev/null", O_WRONLY);
	}
	else
		data->io_fd[1] = open(outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (data->io_fd[1] == -1)
		ft_cleanup(data, "Failed to open outfile", NULL, 1);
	if (access(infile, F_OK | R_OK) == -1)
	{
		perror("pipex: line 1: input");
		data->io_fd[0] = open("/dev/null", O_RDONLY);
	}
	else
		data->io_fd[0] = open(infile, O_RDONLY);
	if (data->io_fd[0] == -1)
		ft_cleanup(data, "Failed to open infile", NULL, 1);
}

static void	ft_pipeline(t_pipex *data, char *argv[], char **env)
{
	int	i;

	i = -1;
	while (++i < 2)
		data->cmd[i] = argv[i + 2];
	ft_get_paths(data, env);
	ft_memset(data->pipe_fd, -1, sizeof(data->pipe_fd));
	if (pipe(data->pipe_fd) == -1)
		ft_cleanup(data, "pipe failed", NULL, 1);
	i = -1;
	while (++i < 2)
	{
		data->pid[i] = fork();
		if (data->pid[i] == -1)
			ft_cleanup(data, "fork failed", NULL, 1);
		if (data->pid[i] == 0)
			ft_child(data, env, &i);
	}
	ft_cleanup(data, NULL, NULL, 0);
	i = -1;
	while (++i < 2)
		if (waitpid(data->pid[i], &data->status, 0) == -1)
			ft_cleanup(NULL, "waitpid failed", NULL, 1);
}

int	main(int argc, char *argv[], char **env)
{
	t_pipex	data;

	if (argc != 5)
		return (write(2, "Usage: ./pipex file1 cmd1 cmd2 file2\n", 37), 1);
	data = (t_pipex){0};
	ft_init_io(&data, argv[1], argv[4]);
	ft_pipeline(&data, argv, env);
	if (data.wprot == 1)
		return (ft_fprintf(2, "pipex: outfile: %s\n",
				strerror(EACCES)), data.wprot);
	if (WIFEXITED(data.status))
		return (WEXITSTATUS(data.status));
	return (1);
}
