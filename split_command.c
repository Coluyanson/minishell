/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_command.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcolucci <dcolucci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/07 20:12:34 by dcolucci          #+#    #+#             */
/*   Updated: 2023/06/06 12:56:50 by dcolucci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_next_index_cmd(char *s, char *set, int i)
{
	char	q;
	char	*set_q;

	set_q = "'\"\"";
	while (s[i])
	{
		if (in_set(s[i], set_q))
		{
			q = s[i++];
			while (s[i] && s[i] != q)
				i++;
			if (s[i] == q)
				i++;
		}
		while (s[i] && !in_set(s[i], set_q) && !in_set(s[i], set))
			i++;
		if (in_set(s[i], set))
			return (i);
	}
	return (i);
}

int	count_sep_cmd(char *s, char *set)
{
	int	count;
	int	i;

	i = 0;
	count = 0;
	while (s[i])
	{
		if (!in_set(s[i], set))
			count++;
		i = ft_next_index_cmd(s, set, i);
		if (in_set(s[i], set))
		{
			i++;
			count++;
		}
	}
	return (count);
}

void	ft_fill_nano(char **nano, char *exp, char *set, int sep)
{
	int	i;
	int	j;
	int	k;

	i = 0;
	k = 0;
	j = ft_next_index_cmd(exp, set, i);
	while (exp[i] && k < sep)
	{
		if (in_set(exp[i], set))
		{
			nano[k] = (char *) malloc (sizeof(char) * 2);
			ft_strlcpy(nano[k++], &exp[i++], 2);
			j = ft_next_index_cmd(exp, set, i);
		}
		else
		{
			nano[k] = (char *) malloc (sizeof(char) * (j - i + 1));
			ft_strlcpy(nano[k++], &exp[i], (j - i + 1));
			i = j;
		}
	}
	nano[k] = 0;
}

char	**nano_split_cmd(char *exp)
{
	int		sep;
	int		index_cmd;
	char	**nano;

	index_cmd = ft_next_index_cmd(exp, "<>|", 0);
	if (!exp[index_cmd])
	{
		nano = (char **) malloc (sizeof (char *) * (2));
		nano[0] = ft_strdup(exp);
		nano[1] = 0;
		return (nano);
	}
	sep = count_sep_cmd(exp, "<>|");
	nano = (char **) malloc (sizeof(char *) * (sep + 1));
	ft_fill_nano(nano, exp, "<>|", sep);
	return (nano);
}

/*
	Description :
		split_cmd function split every string of the exp double 
		char pointer with the separators set equals to "<>|".
	Return Value :
		It returns a double char pointer where every char in separators set "<>|"
		are splitted in single strings.
*/

char	**split_cmd(char **exp)
{
	char	**split_cmd;
	char	**line_spl;
	char	**tmp;
	char	**tmp_spl;
	int		x;

	x = 0;
	split_cmd = 0;
	if (exp == NULL)
		return (NULL);
	while (exp[x])
	{
		line_spl = nano_split_cmd(exp[x]);
		if (!line_spl)
			return (0);
		tmp = line_spl;
		tmp_spl = split_cmd;
		split_cmd = ft_join_split(split_cmd, line_spl);
		free_arrarr(tmp);
		free_arrarr(tmp_spl);
		x++;
	}
	free_arrarr(exp);
	return (split_cmd);
}
