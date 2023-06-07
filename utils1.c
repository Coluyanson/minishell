/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils1.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcolucci <dcolucci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/02 15:16:15 by dcolucci          #+#    #+#             */
/*   Updated: 2023/05/12 20:05:33 by dcolucci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**copy_arrarr(char **arr)
{
	int		i;
	char	**cpy;

	i = 0;
	if (!arr)
		return (0);
	while (arr[i])
		i++;
	if (i == 0)
		return (0);
	cpy = (char **) calloc(sizeof(char *) , (i + 1));
	i = 0;
	while (arr[i])
	{
		cpy[i] = ft_strdup(arr[i]);
		i++;
	}
	cpy[i] = 0;
	return (cpy);
}

int	in_set(char c, char *s)
{
	int	i;

	i = 0;
	while (s[i])
	{
		if (s[i] == c)
			return (1);
		i++;
	}
	return (0);
}

char	*find_next_char(char *str, char *set, int i)
{
	if (!str || !set)
		return (0);
	if ((ft_strlen(str) - 1 < (size_t)i) || !ft_strlen(set))
		return (0);
	while (str[i])
	{
		if (in_set(str[i], set))
			return (&str[i]);
		i++;
	}
	return (0);
}

int	ft_max(int i, int j)
{
	if (i > j)
		return (i);
	return (j);
}

int	compare_env(char *env, char *av, int k, int i)
{
	int	x;

	x = 0;
	while (env[x] && env[x] != '=')
		x++;
	return (ft_strncmp(env, av, ft_max(k - i - 1, x)));
}
