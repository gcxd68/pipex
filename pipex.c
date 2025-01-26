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
	const char	*def_paths = "/usr/local/bin:/usr/bin:/bin:/usr/sbin:/sbin";

	if (env && *env)
	{
		while (ft_strncmp(*env, "PATH=", 5) != 0)
			env++;
		data->paths = ft_split(*env + 5, ':');
	}
	else
		data->paths = ft_split(def_paths, ':');
	if (!data->paths || !data->paths[0])
		ft_cleanup(data, "pipex: failed to split PATH", 1);
}

static void	ft_init_io(t_pipex *data, char *infile, char *outfile)
{
	ft_memset(data->io_fd, -1, sizeof(data->io_fd));
	if (access(outfile, F_OK) == 0 && access(outfile, W_OK) == -1)
	{
		data->out_err = errno;
		data->io_fd[1] = open("/dev/null", O_WRONLY);
	}
	else
		data->io_fd[1] = open(outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (data->io_fd[1] == -1)
		ft_cleanup(data, "pipex : failed to open file2", 1);
	if (access(infile, F_OK | R_OK) == -1)
	{
		data->in_err = errno;
		data->io_fd[0] = open("/dev/null", O_RDONLY);
	}
	else
		data->io_fd[0] = open(infile, O_RDONLY);
	if (data->io_fd[0] == -1)
		ft_cleanup(data, "pipex: failed to open file1", 1);
}

static void	ft_pipeline(t_pipex *data, char *argv[], char **env)
{
	int	i;

	data->cmd[0] = argv[2];
	data->cmd[1] = argv[3];
	ft_get_paths(data, env);
	ft_memset(data->pipe_fd, -1, sizeof(data->pipe_fd));
	if (pipe(data->pipe_fd) == -1)
		ft_cleanup(data, "pipex: pipe failed", 1);
	ft_memset(data->pid, -1, sizeof(data->pid));
	if (data->in_err)
		ft_fprintf(2, "pipex: %s: %s\n", argv[1], strerror(data->in_err));
	if (data->out_err)
		ft_fprintf(2, "pipex: %s: %s\n", argv[4], strerror(data->out_err));
	i = -1;
	while (++i < 2)
	{
		data->pid[i] = fork();
		if (data->pid[i] == -1)
			ft_cleanup(data, "pipex: fork failed", 1);
		if (data->pid[i] == 0)
			ft_child(data, env, i);
	}
	ft_cleanup(data, NULL, 0);
}

int	main(int argc, char *argv[], char **env)
{
	t_pipex	data;
	int		status;

	if (argc != 5)
		return (write(2, "Usage: ./pipex file1 cmd1 cmd2 file2\n", 37), 2);
	data = (t_pipex){0};
	ft_init_io(&data, argv[1], argv[4]);
	ft_pipeline(&data, argv, env);
	if (waitpid(data.pid[0], &status, 0) == -1
		|| waitpid(data.pid[1], &status, 0) == -1)
		ft_cleanup(NULL, "pipex: waitpid failed", 1);
	if (!data.out_err && WIFEXITED(status))
		return (WEXITSTATUS(status));
	return (1);
}
