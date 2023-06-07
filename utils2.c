/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcolucci <dcolucci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/14 21:01:11 by dcolucci          #+#    #+#             */
/*   Updated: 2023/06/06 14:35:01 by dcolucci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_strjoin_free(char *s1, char *s2)
{
	int		size;
	char	*join;

	if (s1 && !s2)
		return (s1);
	if (!s1 && s2)
		return (s2);
	if (!s1 && !s2)
		return (0);
	size = (int)ft_strlen(s1) + ft_strlen(s2);
	join = (char *) malloc (sizeof(char) * (size + 1));
	ft_strlcpy(join, s1, ft_strlen(s1) + 1);
	ft_strlcpy(join + (int)ft_strlen(s1), s2, ft_strlen(s2) + 1);
	free(s1);
	free(s2);
	return (join);
}

int	ft_splitlen(char **split)
{
	int	i;

	i = 0;
	if (!split)
		return (0);
	while (split[i])
		i++;
	return (i);
}

char	**ft_join_split(char **cmd, char **line_spl)
{
	char	**join;
	int		i;
	int		j;

	i = 0;
	j = 0;
	if (!cmd)
		return (copy_arrarr(line_spl));
	while (cmd[i])
		i++;
	while (line_spl[j])
		j++;
	join = (char **) malloc (sizeof(char *) * (i + j + 1));
	i = 0;
	j = 0;
	while (cmd[i])
		join[j++] = ft_strdup(cmd[i++]);
	i = 0;
	while (line_spl[i])
		join[j++] = ft_strdup(line_spl[i++]);
	join[j] = 0;
	return (join);
}

char	**ft_add_to_split(char **spl, char *str)
{
	char	**join;
	int		len;
	int		x;

	x = 0;
	if (!spl)
	{
		join = (char **) malloc (sizeof(char *) * (2));
		join[0] = ft_strdup(str);
		join[1] = 0;
		return (join);
	}
	if (!str)
		return (copy_arrarr(spl));
	len = ft_splitlen(spl);
	join = (char **) malloc (sizeof(char *) * (len + 2));
	while (spl[x])
	{
		join[x] = ft_strdup(spl[x]);
		x++;
	}
	join[x] = ft_strdup(str);
	join[x + 1] = 0;
	return (join);
}

char	**ft_subsplit(char **split, int i, int j)
{
	char	**sub;
	int		k;

	k = 0;
	if (!split || i == j)
		return (0);
	sub = (char **) malloc (sizeof(char *) * (j - i + 1));
	while (i < j && split[i])
		sub[k++] = ft_strdup(split[i++]);
	sub[k] = 0;
	return (sub);
}
