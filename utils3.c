/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils3.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcolucci <dcolucci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/29 16:05:42 by dcolucci          #+#    #+#             */
/*   Updated: 2023/06/10 20:47:53 by dcolucci         ###   ########.fr       */
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
		return (ft_strdup(str));
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
		if (!ft_strncmp(trun_env, var, \
		ft_max(ft_strlen(trun_env), ft_strlen(var))))
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
