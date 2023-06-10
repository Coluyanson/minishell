/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_builtins_01.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcolucci <dcolucci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/10 20:40:55 by dcolucci          #+#    #+#             */
/*   Updated: 2023/06/10 20:43:25 by dcolucci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern int	g_status;

static void	ft_set_export(t_sh *sh, char *arg)
{
	char	*var;
	char	*value;

	if (ft_strchr(arg, '='))
	{
		var = ft_truncate_eq(arg);
		value = ft_strdup(ft_strchr(arg, '=') + 1);
		ft_setenv(sh, var, value);
		ft_safe_free(var);
		ft_safe_free(value);
	}
	else
		ft_setenv(sh, arg, 0);
}

int	ft_export(t_node *node, t_sh *sh)
{
	int		x;

	x = 0;
	g_status = 0;
	if (!node->full_cmd[1])
	{
		while (sh->envp[x])
		{
			ft_putstr_fd(sh->envp[x++], STDOUT_FILENO);
			ft_putchar_fd('\n', STDOUT_FILENO);
		}
		return (1);
	}
	x = 1;
	while (node->full_cmd[x])
	{
		ft_set_export(sh, node->full_cmd[x++]);
	}
	return (1);
}
