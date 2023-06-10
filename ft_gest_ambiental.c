/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_gest_ambiental.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcolucci <dcolucci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/06 10:13:45 by vguidoni          #+#    #+#             */
/*   Updated: 2023/06/10 20:22:47 by dcolucci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern int	g_status;

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
		join = ft_expander_00(x, envp, join, exp);
		x = k + 1;
		if (!exp[k] || !exp[k + 1])
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
