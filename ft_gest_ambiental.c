/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_gest_ambiental.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcolucci <dcolucci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/06 10:13:45 by vguidoni          #+#    #+#             */
/*   Updated: 2023/06/06 21:15:59 by dcolucci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern int g_status;

char	*ft_dollar_01(char *dol, int i)
{
	if (!dol)
		return (0);
	if (!dol[i])
		return (ft_strdup("$"));
	return (0);
}

char	*ft_dollar(char *dol, char *separator, char **envp, int *index)
{
	int		i;
	int		j;
	char	*var;
	char	*value;

	i = *index;
	j = 0;
	ft_dollar_01(dol, i);
	if (dol[i] == '?')
	{
		*index = *index + 1;
		return (ft_itoa(g_status));
	}
	else if (in_set(dol[i], separator))
		return (ft_strdup("$"));
	while (dol[i + j] && !in_set(dol[i + j], separator))
		j++;
	var = (char *) malloc (sizeof(char) * (j + 1));
	ft_strlcpy(var, &dol[i], j + 1);
	value = ft_getenv(var, envp);
	free(var);
	*index = *index + j;
	return (value);
}

char	*ft_expand(char **envp, char *to_exp)
{
	int		i;
	int		j;
	char	*expanded;
	char	*tmp_exp;
	char	*tmp;
	char	*dollar;
	char	*separator;

	i = 0;
	j = 0;
	expanded = 0;
	separator = "<>\\/|+-.,;:~{}[]()&%%\"^'#@*$= ";
	while (to_exp[i])
	{
		if (to_exp[i] == '$')
		{
			i = i + 1;
			dollar = ft_dollar(to_exp, separator, envp, &i);
			tmp_exp = expanded;
			expanded = ft_strjoin_null(expanded, dollar);
			ft_safe_free(dollar);
			ft_safe_free(tmp_exp);
		}
		else
		{
			j = 0;
			while (to_exp[i + j] && to_exp[i + j] != '$')
				j++;
			tmp = (char *) malloc (sizeof(char) * (j + 1));
			ft_strlcpy(tmp, &to_exp[i], j + 1);
			tmp_exp = expanded;
			expanded = ft_strjoin_null(expanded, tmp);
			ft_safe_free(tmp);
			ft_safe_free(tmp_exp);
			i = i + j;
		}
	}
	return (expanded);
}

int		ft_next_quote_exp(char *s, int i)
{
	char	q;

	q = 0;
	if (in_set(s[i], "\'\""))
	{
		q = s[i++];
		while (s[i] && s[i] != q)
			i++;
	}
	else
	{
		while (s[i] && !in_set(s[i], "\'\""))
			i++;
		if (in_set(s[i], "\'\""))
			i--;
	}
	return (i);
}

char	*ft_expnder_00(char *exp, char **envp, int x, char *join)
{
	char	*tmp;
	char	*tmp_join;
	char	*expanded;
	int		k;

	k = ft_next_quote_exp(exp, x);
	if (exp[x] == '\'')
	{
		tmp = (char *) malloc (sizeof(char) * (k - x + 2));
		ft_strlcpy(tmp, &exp[x], (k - x + 2));
		tmp_join = join;
		join = ft_strjoin_null(join, tmp);
	}
	else
	{
		tmp = (char *) malloc (sizeof(char) * (k - x + 2));
		ft_strlcpy(tmp, &exp[x], (k - x + 2));
		expanded = ft_expand(envp, tmp);
		tmp_join = join;
		join = ft_strjoin_null(join, expanded);
		ft_safe_free(expanded);
	}
	ft_safe_free(tmp);
	ft_safe_free(tmp_join);
	return (join);
}

char	*ft_expander(char *exp, char **envp)
{
	char	*join;
	int		x;
	int		k;

	x = 0;
	join = 0;
	while (exp[x])
	{
		k = ft_next_quote_exp(exp, x);
		join = ft_expnder_00(exp, envp, x, join);
		x = k + 1;
		if (!exp[k])
			break ;
		if (!exp[k + 1])
			break ;
	}
	return (join);
}

/*
	Description:
		The function ft_gest_ambiental expand every $ variable in every string
		of av starting with double quotes (") or not starting with single quotes
		(');
	Return value:
		It returns a double char pointer with the $variable replaced
		by its corresponding environment variable.
		If no variable is found the $xxx character (not in separators set) 
		are removed from the string.
*/

char	**ft_gest_ambiental(char **spl, char **envp)
{
	int		x;
	char	*tmp;

	x = 0;
	if (spl == NULL)
		return (NULL);
	while (spl[x])
	{
		tmp = spl[x];
		spl[x] = ft_expander(spl[x], envp);
		free(tmp);
		x++;
	}
	return (spl);
}
