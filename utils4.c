/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils4.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcolucci <dcolucci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/10 20:35:17 by dcolucci          #+#    #+#             */
/*   Updated: 2023/06/10 20:50:32 by dcolucci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern int	g_status;

char	*ft_restore_neg(char *s)
{
	int	i;

	i = 0;
	if (!s)
		return (0);
	while (s[i])
	{
		if (s[i] < 0)
			s[i] = -s[i];
		i++;
	}
	return (s);
}

int	ft_check_matched_quotes(char *s)
{
	int	i;

	i = 0;
	while (s[i])
	{
		i = ft_next_index_quote(s, i);
		if (i == -1)
		{
			g_status = 2;
			ft_putstr_fd("\033[31;5;107mError : unmatched quotes\n\033[0m", \
			STDERR_FILENO);
			return (1);
		}
		if (!s[i])
			break ;
		i++;
	}
	return (0);
}

void	ft_setenv_00(char *value, int x, char *var, t_sh *shell)
{
	if (value)
	{
		if (value)
		{
			shell->tmp_join = ft_strjoin(var, "=");
			shell->join = ft_strjoin(shell->tmp_join, value);
			free(shell->tmp_join);
			shell->tmp_env = shell->envp[x];
			shell->envp[x] = shell->join;
			free(shell->tmp_env);
		}
	}
	x = x;
}

void	ft_setenv_01(char *value, char *var, t_sh *shell)
{
	if (value)
	{
		shell->tmp_join = ft_strjoin(var, "=");
		shell->join = ft_strjoin(shell->tmp_join, value);
		free(shell->tmp_join);
	}
	else
		shell->join = var;
	shell->tmp = shell->envp;
	shell->envp = ft_add_to_split(shell->envp, shell->join);
}

void	ft_setenv(t_sh *shell, char *var, char *value)
{
	int		x;
	char	*trun_env;

	x = -1;
	if (!shell->envp || !var)
		return ;
	while (shell->envp[++x])
	{
		trun_env = ft_truncate_eq(shell->envp[x]);
		if (!ft_strncmp(trun_env, var, \
		ft_max(ft_strlen(trun_env), ft_strlen(var))))
		{
			ft_setenv_00(value, x, var, shell);
			free(trun_env);
			break ;
		}
		free(trun_env);
	}
	if (!shell->envp[x])
	{
		ft_setenv_01(value, var, shell);
		if (value)
			free(shell->join);
		free_arrarr(shell->tmp);
	}
}
