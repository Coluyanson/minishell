/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils3.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcolucci <dcolucci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/29 16:05:42 by dcolucci          #+#    #+#             */
/*   Updated: 2023/06/02 19:00:03 by dcolucci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


char	*ft_strjoin_null(char *s1, char *s2)
{
	int		size;
	char	*join;

	if (!s1 && !s2)
		return (0);
	if (!s1 && s2)
		return (ft_strdup(s2));
	if (s1 && !s2)
		return (ft_strdup(s1));
	size = ft_strlen(s1) + ft_strlen(s2);
	join = (char *) malloc (sizeof(char) * (size + 1));
	ft_strlcpy(join, s1, ft_strlen(s1) + 1);
	ft_strlcpy(join + ft_strlen(s1), s2, ft_strlen(s2) + 1);
	return (join);
}

int	ft_check_export(char *str, char *env)
{
	int	x;

	x = 0;
	while (env[x] && env[x] != '=')
		x++;
	return (ft_strncmp(str, env, x));
}

char	*ft_truncate_eq(char *str)
{
	int		x;
	char	*trun;

	if (!str)
		return (0);
	if (!ft_strchr(str, '='))
		return (str);
	x = 0;
	while (str[x] && str[x] != '=')
		x++;
	trun = (char *) malloc (sizeof(char) * (x + 1));
	ft_strlcpy(trun, str, x + 1);
	return (trun);
}

char	*ft_getenv(char *var, char **envp)
{
	int		x;
	char	*trun_env;
	char	*env;

	if (!var || !envp)
		return (0);
	x = 0;
	env = 0;
	while (envp[x])
	{
		trun_env = ft_truncate_eq(envp[x]);
		if (!ft_strncmp(trun_env, var, ft_max(ft_strlen(trun_env), ft_strlen(var))))
		{
			if (ft_strchr(envp[x], '='))
				env = ft_strdup(ft_strchr((envp[x]), '=') + 1);
			else
				env = ft_strdup("");
			free(trun_env);
			break ;
		}
		free(trun_env);
		x++;
	}
	return (env);
}

void	ft_setenv(t_sh *shell, char *var, char *value)
{
	int		x;
	char	*trun_env;
	char	*tmp_join;
	char	*join;

	x = 0;
	if (!shell->envp || !var)
		return ;
	while (shell->envp[x])
	{
		trun_env = ft_truncate_eq(shell->envp[x]);
		if (!ft_strncmp(trun_env, var, \
		ft_max(ft_strlen(trun_env), ft_strlen(var))))
		{
			if (value)
			{
				tmp_join = ft_strjoin(var, "=");
				join = ft_strjoin(tmp_join, value);
				shell->envp[x] = join;
			}
			break ;
		}
		x++;
	}
	if (!shell->envp[x])
	{
		if (value)
		{
			tmp_join = ft_strjoin(var, "=");
			join = ft_strjoin(tmp_join, value);
		}
		else
			join = var;
		shell->envp = ft_add_to_split(shell->envp, join);
	}
}

char	*ft_substitute_string(char *str, char *str_in, int index, int size)
{
	char	*tmp;
	char	*tmp2;
	char	*join;

	if (!str_in)
	{
		tmp = malloc(sizeof(char) * (index + 1));
		ft_strlcpy(tmp, str, index + 1);
		tmp2 = ft_strdup(&str[index + size]);
		join = ft_strjoin(tmp, tmp2);
		free(tmp);
		free(tmp2);
	}
	else
	{
		//c i a o " s t r _ i n  "  \0
		//0 1 2 3 4 5 6 7 8 9 10 11 12
		// index = 5
		// size = 6
		/*
			$ P W D
			0 1 2 3
			index = 0
			size = 4
		*/
		tmp = (char *) malloc (sizeof(char) * (ft_strlen(str) - size + ft_strlen(str_in) + 1));
		ft_strlcpy(tmp, str, index + 1);
		ft_strlcpy(tmp + index, str_in, ft_strlen(str_in) + 1);
		ft_strlcpy(tmp + index + ft_strlen(str_in), &str[index + size], INT_MAX);
		join = tmp;
	}
	return (join);
}