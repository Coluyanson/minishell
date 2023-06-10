/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_create_node_00.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcolucci <dcolucci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/10 20:26:50 by dcolucci          #+#    #+#             */
/*   Updated: 2023/06/10 21:03:37 by dcolucci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_cmd(char **sub_cmd)
{
	int		i;
	char	*cmd;

	i = 0;
	cmd = 0;
	while (sub_cmd[i])
	{
		if (!in_set(sub_cmd[i][0], "<>|") || \
		(in_set(sub_cmd[i][0], "<>|") && ft_strlen(sub_cmd[i]) != 1))
		{
			cmd = (ft_strdup(sub_cmd[i]));
			return (cmd);
		}
		else
		{
			while (sub_cmd[i] && in_set(sub_cmd[i][0], "<>"))
				i++;
			if (!sub_cmd[i])
				break ;
			else
				i++;
		}
	}
	return (cmd);
}

char	**ft_full_cmd(char **sub_cmd)
{
	int		i;
	char	**full_cmd;
	char	**tmp;

	i = 0;
	full_cmd = 0;
	while (sub_cmd[i])
	{
		if (!in_set(sub_cmd[i][0], "<>|") || \
		(in_set(sub_cmd[i][0], "<>|") && ft_strlen(sub_cmd[i]) != 1))
		{
			tmp = full_cmd;
			full_cmd = ft_add_to_split(full_cmd, sub_cmd[i++]);
			free_arrarr(tmp);
		}
		else
		{
			while (sub_cmd[i] && in_set(sub_cmd[i][0], "<>"))
				i++;
			if (!sub_cmd[i++])
				break ;
		}
	}
	return (full_cmd);
}
