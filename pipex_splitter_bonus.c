/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_splitter_bonus.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gdosch <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/12 13:43:37 by gdosch            #+#    #+#             */
/*   Updated: 2025/01/12 13:43:39 by gdosch           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

static int	ft_add_arg(char *cmd_idx, char ***args, int arg_ct, int arg_len)
{
	char	*arg;

	arg = malloc(arg_len + 1);
	if (!arg)
		return (-1);
	ft_strlcpy(arg, cmd_idx, arg_len + 1);
	*args = ft_realloc(*args, sizeof(char *) * (arg_ct + 2));
	if (!*args)
		return (free(arg), -1);
	(*args)[arg_ct] = arg;
	(*args)[arg_ct + 1] = NULL;
	return (0);
}

static void	ft_is_quote(char c, bool quote[2])
{
	if (c == '\'' && !quote[1])
		quote[0] = !quote[0];
	else if (c == '\"' && !quote[0])
		quote[1] = !quote[1];
}

int	ft_split_args(char ***args, char *cmd)
{
	bool	quote[3];
	int		arg_ct;
	int		start_idx;
	int		i;

	arg_ct = 0;
	i = -1;
	while (cmd[++i])
	{
		ft_memset(quote, 0, sizeof(quote));
		if (cmd[i] == ' ')
			continue ;
		start_idx = i;
		while (cmd[i] && (cmd[i] != ' ' || quote[0] || quote[1]))
			ft_is_quote(cmd[i++], quote);
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
