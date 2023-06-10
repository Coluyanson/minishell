/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_quotes.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcolucci <dcolucci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/02 18:38:52 by dcolucci          #+#    #+#             */
/*   Updated: 2023/06/10 20:35:41 by dcolucci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern int	g_status;

/*
It finds next index after the first quote. 
If there is no matching quote it returns -1; 
*/

int	ft_next_index_quote(char *s, int i)
{
	char	q;
	char	*set;

	set = "\'\"";
	while (s[i])
	{
		if (in_set(s[i], set))
		{	
			q = s[i++];
			while (s[i] && s[i] != q)
				i++;
			if (!s[i])
				return (-1);
			else
				i++;
		}
		while (s[i] && !in_set(s[i], set) && s[i] != ' ')
			i++;
		if (s[i] == ' ')
			break ;
	}
	return (i);
}

/*Nelle parentesi chiuse posso non devo interpretare nulla.
Mi fermo quando, dopo aver trovato un numero pari di parentesi,
trovo un carattere tra >, >>, <, <<, | oppure spazio??. (un trattino - ?)
Finche' non trovo uno di questi devo considerare la stringa ancora non terminata.

CORREZIONE : 
Considero una stringa ogni volta che si chiude una parentesi
*/

int	count_strings(char *s)
{
	int		strings;
	int		i;

	i = 0;
	strings = 0;
	while (s[i])
	{
		if (s[i] != ' ')
		{
			i = ft_next_index_quote(s, i);
			strings++;
		}
		else
			while (s[i] && s[i] == ' ')
				i++;
	}
	return (strings);
}

int	fill_line(char **line, char *s, int j)
{
	int	k;

	k = ft_next_index_quote(s, j);
	*line = (char *) malloc (sizeof(char) * (k - j + 1));
	ft_strlcpy(*line, &s[j], k - j + 1);
	return (k);
}

char	**fill_split(char **split, char *s, int strings)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (i < strings && s[j])
	{
		if (s[j] == ' ')
			while (s[j] && s[j] == ' ')
				j++;
		else
			j = fill_line(&split[i++], s, j);
	}
	split[i] = 0;
	return (split);
}

/*
	Description:
		The split_quotes function takes a string s and splits it into
		multiple strings separated by spaces; strings in double and single
		closed quotes are considered a single string.
	Return value:
		It returns a NULL-terminated double char pointer with all the splitted strings.
		When quotes are not closed or when s is NULL it returns a NULL pointer.
*/

char	**split_quotes(char *s)
{
	char	**split;
	int		strings;

	split = 0;
	if (!s || ft_check_matched_quotes(s))
		return (0);
	strings = count_strings(s);
	if (strings == 0)
		return (0);
	split = (char **) malloc(sizeof(char *) * (strings + 1));
	split = fill_split(split, s, strings);
	return (split);
}
