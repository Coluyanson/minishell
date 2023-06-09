/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_check_syntax.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcolucci <dcolucci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/15 14:56:10 by dcolucci          #+#    #+#             */
/*   Updated: 2023/06/09 14:17:23 by dcolucci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_count_redirection(char **sub_cmd, char redir)
{
	int		x;
	int		count;

	x = 0;
	count = 1;
	while (sub_cmd[x])
	{
		if (sub_cmd[x][0] == redir && ft_strlen(sub_cmd[x]) == 1)
			count++;
		else if (sub_cmd[x][0] != redir && in_set(sub_cmd[x][0], "<>") \
			&& ft_strlen(sub_cmd[x]) == 1)
			return (INT_MAX);
		else
			return (count);
		x++;
	}
	return (count);
}

int	ft_check_syntax(char **sub_cmd)
{
	int		x;
	int		count;

	x = 0;
	count = 0;
	if (!sub_cmd)
		return (0);
	while (sub_cmd[x])
	{
		if (in_set(sub_cmd[x][0], "<>") && ft_strlen(sub_cmd[x]) == 1)
		{
			count = ft_count_redirection(&sub_cmd[x + 1], sub_cmd[x][0]);
			if (count > 2 || !sub_cmd[x + 1])
			{
				ft_putstr_fd("\033[31mSytax error using < and >\n\033[0m", \
				STDERR_FILENO);
				return (-1);
			}
			else
				x = x + count;
		}
		else
			x++;
	}
	return (0);
}
