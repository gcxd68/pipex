/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_splitter.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gdosch <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/12 19:27:58 by gdosch            #+#    #+#             */
/*   Updated: 2025/01/12 19:27:59 by gdosch           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static int	ft_add_arg(char *cmd_idx, char ***args, int arg_ct, int len)
{
	char	*arg;
	int		i;
	int		j;

	arg = malloc(len + 1);
	if (!arg)
		return (-1);
	i = 0;
	j = 0;
	while (i < len)
	{
		if (cmd_idx[i] == '\\' && cmd_idx[i + 1]
			&& (cmd_idx[i + 1] == '"' || cmd_idx[i + 1] == '\''
				|| cmd_idx[i + 1] == '\\'))
			i++;
		arg[j++] = cmd_idx[i++];
	}
	arg[j] = '\0';
	*args = ft_realloc(*args, sizeof(char *) * (arg_ct + 2));
	if (!*args)
		return (free(arg), -1);
	(*args)[arg_ct] = arg;
	(*args)[arg_ct + 1] = NULL;
	return (0);
}

static void	ft_is_quote(char *cmd, int i, bool quote[3])
{
	if (quote[0] && cmd[i] == '\'' && cmd[i - 1] != '\\')
	{
		quote[0] = !quote[0];
		return ;
	}
	if (quote[1] && cmd[i] == '\"' && cmd[i - 1] != '\\')
	{
		quote[1] = !quote[1];
		return ;
	}
	if (cmd[i] == '\'' && (i == 0 || cmd[i - 1] != '\\'))
		quote[0] = !quote[0];
	else if (cmd[i] == '\"' && (i == 0 || cmd[i - 1] != '\\'))
		quote[1] = !quote[1];
}

static int	ft_valid_cmd(char ***args, char *cmd)
{
	if (access(cmd, F_OK) == 0)
	{
		*args = malloc(sizeof(char *) * 2);
		if (!*args)
			return (-1);
		(*args)[0] = ft_strdup(cmd);
		if (!(*args)[0])
			return (-1);
		(*args)[1] = NULL;
	}
	return (0);
}

static int	ft_split_core(char ***args, char *cmd)
{
	bool	quote[3];
	int		arg_ct;
	int		start_idx;
	int		i;

	ft_memset(quote, 0, sizeof(quote));
	arg_ct = 0;
	i = -1;
	while (cmd[++i])
	{
		if (cmd[i] == ' ')
			continue ;
		start_idx = i;
		while (cmd[i] && (cmd[i] != ' ' || quote[0] || quote[1]))
			ft_is_quote(cmd, i++, quote);
		if (cmd[i - 1] == '\'' || cmd[i - 1] == '\"')
			quote[2] = !quote[2];
		if (i > start_idx)
			if (ft_add_arg(&cmd[start_idx + quote[2]], args,
					arg_ct++, i - start_idx - 2 * quote[2]) < 0)
				return (-1);
		if (!cmd[i])
			break ;
	}
	return (0);
}

int	ft_split_args(char ***args, char *cmd)
{
	if (access(cmd, F_OK) == 0)
		return (ft_valid_cmd(args, cmd));
	if (ft_split_core(args, cmd) < 0)
		return (-1);
	return (0);
}
