/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   final_split.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcolucci <dcolucci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/09 19:56:18 by dcolucci          #+#    #+#             */
/*   Updated: 2023/06/05 19:09:09 by dcolucci         ###   ########.fr       */
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
extern int g_status;

char	**final_split(char *input, char **envp)
{
	char	**final;

	final = split_quotes(input);
/* 	printf("QUOTES\n");
	printf("===============\n");
	print_arrarr(final);
	printf("===============\n"); */
	final = ft_gest_ambiental(final, envp);
/*  	printf("EXP\n");
	printf("===============\n");
	print_arrarr(final);
	printf("===============\n"); */
	g_status = 0;
	final = split_cmd(final);
/* 	printf("CMD\n");
	print_arrarr(final); */
	final = trim_quotes(final);
 /* 	printf("TRIM\n");
	printf("===============\n");
	print_arrarr(final);
	printf("===============\n"); */
	return (final);
}
