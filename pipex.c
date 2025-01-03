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

static void	ft_free_arr(char **arr)
{
	size_t	i;

	if (!arr)
		return ;
	i = 0;
	while (arr[i])
	{
		free(arr[i]);
		arr[i] = 0;
		i++;
	}
	free(arr);
}

static void	ft_close_fds(int io_fd[2], int pipe_fd[2])
{
	if (pipe_fd[0] != -1)
		close(pipe_fd[0]);
	if (pipe_fd[1] != -1)
		close(pipe_fd[1]);
	if (io_fd[0] != -1)
		close(io_fd[0]);
	if (io_fd[1] != -1)
		close(io_fd[1]);
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

static char	*ft_find_cmd_path(char *cmd, char **path)
{
	char	*full_path;
	size_t	len_path;
	size_t	len_cmd;
	size_t	i;

	i = 0;
	len_cmd = ft_strlen(cmd);
	while (path[i])
	{
		len_path = ft_strlen(path[i]);
		full_path = malloc(len_path + len_cmd + 2);
		if (!full_path)
			return (NULL);
		ft_memcpy(full_path, path[i], len_path);
		full_path[len_path] = '/';
		ft_strlcpy(full_path + len_path + 1, cmd, len_cmd + 1);
		if (access(full_path, X_OK) == 0)
			return (full_path);
		free(full_path);
		i++;
	}
	return (NULL);
}

static int	ft_first_child(char *cmd[2], int pipe_fd[2], int std_fd[2], char **paths)
{
	char	**args;
	char	*cmd_path;

	args = ft_split(cmd[0], ' ');
	if (!args)
		return (perror("Failed to split cmd1"), -1);
	close(pipe_fd[0]); // Ne pas lire depuis le pipe dans ce processus
	if (dup2(std_fd[0], 0) == -1 || dup2(pipe_fd[1], 1) == -1) // Rediriger l'entrée depuis le fichier d'entrée et la sortie vers le pipe
		return (perror("dup2 failed"), ft_free_arr(args), -1);
	close(pipe_fd[1]); // Fermer le descripteur après duplication
	close(std_fd[0]); // Fermer le fichier d'entrée
	cmd_path = ft_find_cmd_path(args[0], paths);
	if (!cmd_path)
		return (perror("Command not found"), ft_free_arr(args), -1);
	execve(cmd_path, args, NULL);
	return (perror("execve failed"), ft_free_arr(args), free(cmd_path), -1);
}

static int	ft_last_child(char *cmd[2], int pipe_fd[2], int std_fd[2], char **paths)
{
	char	**args;
	char	*cmd_path;

	args = ft_split(cmd[1], ' ');
	if (!args)
		return (perror("Failed to split cmd1"), -1);
	close(pipe_fd[1]); // Ne pas écrire dans le pipe
	if (dup2(pipe_fd[0], 0) == -1 || dup2(std_fd[1], 1) == -1) // Rediriger l'entrée depuis le pipe et la sortie vers le fichier de sortie
		return (perror("dup2 failed"), ft_free_arr(args), -1);
	close(pipe_fd[0]); // Fermer le descripteur après duplication
	close(std_fd[1]); // Fermer le fichier de sortie
	cmd_path = ft_find_cmd_path(args[0], paths);
	if (!cmd_path)
		return (perror("Command not found"), ft_free_arr(args), -1);
	execve(cmd_path, args, NULL);
	return (perror("execve failed"), ft_free_arr(args), free(cmd_path), -1);
}

int	main(int argc, char *argv[], char **env)
{
	pid_t	pid[2];
	char	**paths;
	char	*cmd[2];
	int		io_fd[2];
	int		pipe_fd[2];
	int		i;

	if (argc < 5)
		return (perror("Usage: ./pipex infile cmd1 cmd2 outfile"), -1);
	i = -1;
	while (++i < argc - 3)
		cmd[i] = argv [i + 2];
	if (access(argv[1], F_OK) == -1)
		return (perror("infile does not exist"), -1);
	io_fd[0] = open(argv[1], O_RDONLY);
	if (io_fd[0] == -1)
		return (perror("Failed to open infile"), -1);
	if (access(argv[argc - 1], F_OK) == 0 && access(argv[argc - 1], W_OK) == -1)
		return (ft_close_fds(io_fd, pipe_fd), perror("No write permission for outfile"), -1);
	io_fd[1] = open(argv[argc - 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (io_fd[1] == -1)
		return (ft_close_fds(io_fd, pipe_fd), perror("Failed to open outfile"), -1);
	if (pipe(pipe_fd) == -1)
		return (ft_close_fds(io_fd, pipe_fd), perror("pipe failed"), -1);
	paths = ft_get_paths(env);
	if (!paths)
		return (ft_close_fds(io_fd, pipe_fd), perror("Failed to get PATH"), -1);
	pid[0] = fork();
	if (pid[0] == -1)
		return (ft_free_arr(paths), ft_close_fds(io_fd, pipe_fd), perror("fork failed"), -1);
	if (pid[0] == 0)
		exit(ft_first_child(cmd, pipe_fd, io_fd, paths));
	pid[1] = fork();
	if (pid[1] == -1)
		return (ft_free_arr(paths), ft_close_fds(io_fd, pipe_fd), perror("fork failed"), -1);
	if (pid[1] == 0)
		exit(ft_last_child(cmd, pipe_fd, io_fd, paths));
	ft_close_fds(io_fd, pipe_fd);
	i = -1;
	while (++i < argc - 3)
		waitpid(pid[i], NULL, 0);
	return (ft_free_arr(paths), 0);
}
