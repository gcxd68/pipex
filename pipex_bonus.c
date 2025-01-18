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

static void	ft_get_paths(t_pipex *data, char **env)
{
	if (!env || !*env)
	{
		data->def_paths[0] = "/usr/local/bin";
		data->def_paths[1] = "/usr/bin";
		data->def_paths[2] = "/bin";
		data->def_paths[3] = "/usr/sbin";
		data->def_paths[4] = "/sbin";
		data->paths = data->def_paths;
		return ;
	}
	data->env = 1;
	while (ft_strncmp(*env, "PATH=", 5) != 0)
		env++;
	data->paths = ft_split(*env + 5, ':');
	if (!data->paths)
		ft_cleanup(data, "pipex: failed to get PATH", 1);
}

static void	ft_init_io(t_pipex *data, char *infile, char *outfile)
{
	ft_memset(data->io_fd, -1, sizeof(data->io_fd));
	if (access(outfile, F_OK) == 0 && access(outfile, W_OK) == -1)
	{
		data->wprot = 1;
		data->io_fd[1] = open("/dev/null", O_WRONLY);
	}
	else if (data->here_doc)
		data->io_fd[1] = open(outfile, O_WRONLY | O_CREAT | O_APPEND, 0644);
	else
		data->io_fd[1] = open(outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (data->io_fd[1] == -1)
		ft_cleanup(data, "pipex: failed to open outfile", 1);
	if (data->here_doc)
		return ;
	if (access(infile, F_OK | R_OK) == -1)
	{
		perror("pipex: line 1: input");
		data->io_fd[0] = open("/dev/null", O_RDONLY);
	}
	else
		data->io_fd[0] = open(infile, O_RDONLY);
	if (data->io_fd[0] == -1)
		ft_cleanup(data, "pipex: failed to open infile", 1);
}

static void	ft_init_data(t_pipex *data, char *argv[])
{
	int	i;

	data->pid = malloc(sizeof(pid_t) * data->cmd_ct);
	if (!data->pid)
		ft_cleanup(data, "pipex: pid array failure", 1);
	data->cmd = ft_calloc(data->cmd_ct + 1, sizeof(char *));
	if (!data->cmd)
		ft_cleanup(data, "pipex: cmd array failure", 1);
	i = -1;
	while (++i < data->cmd_ct)
		data->cmd[i] = argv[i + 2 + data->here_doc];
	data->pipe_fd = malloc(sizeof(int *) * (data->cmd_ct - 1));
	if (!data->pipe_fd)
		ft_cleanup(data, "pipex: pipe array failure", 1);
	i = -1;
	while (++i < data->cmd_ct - 1)
	{
		data->pipe_fd[i] = malloc(sizeof(int) * 2);
		if (!data->pipe_fd[i])
			ft_cleanup(data, "pipex: pipe array failure", 1);
		ft_memset(data->pipe_fd[i], -1, sizeof(int) * 2);
	}
	ft_memset(data->hd_fd, -1, sizeof(data->hd_fd));
}

static void	ft_pipeline(t_pipex *data, char *argv[], char **env)
{
	int	i;

	ft_get_paths(data, env);
	if (data->here_doc)
	{
		if (pipe(data->hd_fd) == -1)
			ft_cleanup(data, "pipex: here_doc pipe failed", 1);
		ft_here_doc(data, argv[2]);
		data->io_fd[0] = data->hd_fd[0];
	}
	i = -1;
	while (++i < data->cmd_ct - 1)
		if (pipe(data->pipe_fd[i]) == -1)
			ft_cleanup(data, "pipex: pipe failed", 1);
	i = -1;
	while (++i < data->cmd_ct)
	{
		data->pid[i] = fork();
		if (data->pid[i] == -1)
			ft_cleanup(data, "pipex: fork failed", 1);
		data->curr_cmd = data->cmd[i];
		if (data->pid[i] == 0)
			ft_child(data, env, &i);
	}
	ft_cleanup(data, NULL, 0);
}

int	main(int argc, char *argv[], char **env)
{
	t_pipex	data;
	int		i;

	data = (t_pipex){0};
	data.here_doc = (argc > 1 && !ft_strncmp(argv[1], "here_doc", 8));
	data.cmd_ct = argc - 3 - data.here_doc;
	if (data.cmd_ct < 2)
		return (write(2, "Usage:\n./pipex file1 cmd1 ... cmdn file2\n"
				"./pipex here_doc LIMITER cmd1 ... cmdn file\n", 85), 1);
	ft_init_io(&data, argv[1], argv[data.cmd_ct + 2 + data.here_doc]);
	ft_init_data(&data, argv);
	ft_pipeline(&data, argv, env);
	i = -1;
	while (++i < data.cmd_ct)
		if (waitpid(data.pid[i], &data.status, 0) == -1)
			ft_cleanup(NULL, "pipex: waitpid failed", 1);
	if (data.pid)
		free(data.pid);
	if (data.wprot == 1)
		return (ft_fprintf(2, "pipex: outfile: %s\n",
				strerror(EACCES)), data.wprot);
	if (WIFEXITED(data.status))
		return (WEXITSTATUS(data.status));
	return (1);
}
