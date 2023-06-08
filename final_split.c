/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   final_split.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcolucci <dcolucci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/09 19:56:18 by dcolucci          #+#    #+#             */
/*   Updated: 2023/06/08 15:45:27 by dcolucci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
	Description:
		The funcion final_split create a double char pointer splitting
		the input string as the BASH shell would have done before analyzing
		the commands.
	Return value:
		The function returns the double char pointer with every string processed
		before analyzing commands.
*/
extern int	g_status;

char	**final_split(char *input, char **envp)
{
	char	**final;

	final = split_quotes(input);
	final = ft_gest_ambiental(final, envp);
	g_status = 0;
	final = split_cmd(final);
	final = trim_quotes(final);
	return (final);
}
