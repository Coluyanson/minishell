/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_gest_ambiental_00.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcolucci <dcolucci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/10 20:21:18 by dcolucci          #+#    #+#             */
/*   Updated: 2023/06/10 20:24:33 by dcolucci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern int	g_status;

char	*ft_dollar(char *dol, char *separator, char **envp, int *index)
{
	int		i;
	int		j;
	char	*var;
	char	*value;

	i = *index;
	j = 0;
	if (!dol)
		return (0);
	if (dol[i] == '?')
	{
		*index = *index + 1;
		return (ft_itoa(g_status));
	}
	else if (in_set(dol[i], separator) || !dol[i])
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

char	*ft_expand_no_dollar(int *index, char *to_exp, char *expanded)
{
	int		i;
	int		j;
	char	*tmp;
	char	*tmp_exp;

	j = 0;
	i = *index;
	while (to_exp[i + j] && to_exp[i + j] != '$')
		j++;
	tmp = (char *) malloc (sizeof(char) * (j + 1));
	ft_strlcpy(tmp, &to_exp[i], j + 1);
	tmp_exp = expanded;
	expanded = ft_strjoin_null(expanded, tmp);
	ft_safe_free(tmp);
	ft_safe_free(tmp_exp);
	*index = *index + j;
	return (expanded);
}

char	*ft_expand(char **envp, char *to_exp)
{
	int		i;
	char	*expanded;
	char	*tmp_exp;
	char	*dollar;
	char	*separator;

	i = 0;
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
			expanded = ft_expand_no_dollar(&i, to_exp, expanded);
	}
	return (expanded);
}

int	ft_next_quote_exp(char *s, int i)
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

char	*ft_expander_00(int x, char **envp, char *join, char *exp)
{
	char	*tmp;
	char	*tmp_join;
	char	*expanded;
	int		k;

	k = ft_next_quote_exp(exp, x);
	tmp = (char *) malloc (sizeof(char) * (k - x + 2));
	ft_strlcpy(tmp, &exp[x], (k - x + 2));
	if (exp[x] == '\'')
	{
		tmp_join = join;
		join = ft_strjoin_null(join, tmp);
	}
	else
	{
		expanded = ft_expand(envp, tmp);
		tmp_join = join;
		join = ft_strjoin_null(join, expanded);
		ft_safe_free(expanded);
	}
	ft_safe_free(tmp);
	ft_safe_free(tmp_join);
	return (join);
}
