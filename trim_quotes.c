/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   trim_quotes.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcolucci <dcolucci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/07 19:04:53 by dcolucci          #+#    #+#             */
/*   Updated: 2023/06/10 20:48:10 by dcolucci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_next_index_trim(char *s, int i)
{
	char	q;
	char	*set;

	set = "\'\"";
	while (s[i])
	{
		if (in_set((s[i]), set))
		{
			q = s[i];
			i++;
			while (s[i] && s[i] != q)
				i++;
			return (i);
		}
		else
			break ;
	}
	return (i);
}

char	*delete_quotes_00(char *s, char *join, int *jindex)
{
	int		k;
	char	*tmp;
	int		j;

	j = *jindex;
	if (in_set(s[j], "\"\'"))
	{
		k = ft_next_index_trim(s, j);
		tmp = malloc(sizeof(char) * (k - j));
		ft_strlcpy(tmp, &s[j + 1], k - j);
		join = ft_strjoin_free(join, tmp);
		*jindex = k + 1;
	}
	else
	{
		k = j;
		while (s[k] && !in_set(s[k], "\"\'"))
			k++;
		tmp = malloc(sizeof(char) * (k - j + 1));
		ft_strlcpy(tmp, &s[j], k - j + 1);
		join = ft_strjoin_free(join, tmp);
		*jindex = k;
	}
	return (join);
}

char	*delete_quotes(char *s)
{
	int		j;
	char	*join;

	j = 0;
	join = 0;
	while (s[j])
		join = delete_quotes_00(s, join, &j);
	return (join);
}
/*
	Description:
		The function trim_quotes modify every string of 
		double char pointer exp trimming the quotes at the start
		and at the end of the string.
	Return value:
		It returns the same address of exp but the strings are modified.
*/

char	*ft_safe_quotes(char *cmd)
{
	if (ft_strlen(cmd) == 3 && in_set(cmd[0], "\'\""))
	{
		if (in_set(cmd[1], "<>|"))
			cmd[1] = -cmd[1];
	}
	return (cmd);
}

char	**trim_quotes(char **cmd)
{
	int		i;
	char	*tmp;

	i = -1;
	if (!cmd)
		return (0);
	while (cmd[++i])
	{
		cmd[i] = ft_safe_quotes(cmd[i]);
		tmp = cmd[i];
		cmd[i] = delete_quotes(cmd[i]);
		free(tmp);
	}
	return (cmd);
}
