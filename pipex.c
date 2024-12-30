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

static void	ft_free_arr(char **split_ret)
{
	size_t	i;

	i = 0;
	while (split_ret[i])
	{
		free(split_ret[i]);
		split_ret[i] = 0;
		i++;
	}
	free(split_ret);
	split_ret = 0;
}

int	main(int argc, char *argv[])
{
	pid_t	pid1
	pid_t	pid2;
	char	*infile;
	char	*cmd1;
	char	*cmd2;
	char	*outfile;
	int		fd_in;
	int		fd_out;
	int		pipe_fd[2];

	infile = argv[1];
	cmd1 = argv[2];
	cmd2 = argv[3];
	outfile = argv[4];

	if (access(infile, F_OK) == -1)
	{
	    perror("infile does not exist");
	    exit(-1);
	}
	fd_in = open(infile, O_RDONLY);

	if (fd_in == -1)
	{
    	perror("Failed to open infile");
    	exit(-1);
	}

	if (pipe(pipe_fd) == -1)
	{
		perror("pipe failed");
		exit (-1);
	}

	// Premier fork
	args = ft_split(cmd1, ' ');
	if (!args)
		exit(-1);
    pid1 = fork();
    if (pid1 == -1) {
        perror("fork failed");
        exit(EXIT_FAILURE);
    }

    if (pid1 == 0)  // Processus enfant 1
    {
        close(pipe_fd[0]);  // Ne pas lire depuis le pipe dans ce processus
        dup2(fd_in, 0);      // Rediriger l'entrée depuis le fichier d'entrée
        dup2(pipe_fd[1], 1); // Rediriger la sortie vers le pipe
        close(pipe_fd[1]);   // Fermer le descripteur après duplication
        close(fd_in);        // Fermer le fichier d'entrée
		// Libérer la mémoire avant d'exécuter execve
		ft_free_arr(args);
        // Exécuter la première commande
        execve(args[0], args, NULL); // Remplacer le processus par "grep"
        perror("execve failed");  // Si execve échoue
        exit(-1);
    }

    ft_free_arr(args);
    args = 0;

	fd_out = open(outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd_out == -1)
	{
        perror("Failed to open outfile");
        exit(EXIT_FAILURE);
    }

    // Deuxième fork
    args = ft_split(cmd2, ' ');
	if (!args)
		exit(-1);
    pid2 = fork();
    if (pid2 == -1)
    {
        perror("fork failed");
        exit(EXIT_FAILURE);
    }

    if (pid2 == 0)  // Processus enfant 2
    {
        close(pipe_fd[1]);  // Ne pas écrire dans le pipe
        dup2(pipe_fd[0], 0); // Rediriger l'entrée depuis le pipe
        dup2(fd_out, 1);     // Rediriger la sortie vers le fichier de sortie
        close(pipe_fd[0]);   // Fermer le descripteur après duplication
        close(fd_out);       // Fermer le fichier de sortie
		// Libérer la mémoire avant d'exécuter execve
		ft_free_arr(args);
        // Exécuter la deuxième commande
        execve(args[0], args, NULL); // Remplacer le processus par "wc"
        perror("execve failed");  // Si execve échoue
        exit(-1);
    }

    ft_free_arr(args);
    args = 0;

    // Processus parent
    close(pipe_fd[0]); // Fermer les descripteurs dans le parent
    close(pipe_fd[1]);

    // Attendre que les deux enfants se terminent
    waitpid(pid1, NULL, 0);  // Attendre le premier enfant
    waitpid(pid2, NULL, 0);  // Attendre le deuxième enfant

    // Fermer les fichiers dans le parent
    close(fd_in);
    close(fd_out);

    return 0;
}
