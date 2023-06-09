/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_print.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcolucci <dcolucci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/12 19:04:05 by dcolucci          #+#    #+#             */
/*   Updated: 2023/05/22 15:29:19 by dcolucci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_arrarr(char **arr)
{
	int	i;

	i = 0;
	if (!arr)
	{
		printf("No arrarr\n");
		return ;
	}
	while (arr[i])
	{
		if (!arr[i])
			printf("Line %d is null\n", i);
		else
			printf("%s\n", arr[i]);
		i++;
	}
}

void	print_node(t_node *node, char **sub_spl)
{
	if (!node)
		return ;
	printf("-------------\n");
	print_arrarr(sub_spl);
	printf("\033[31mNode cmd : %s\n", node->cmds);
	printf("\033[32mNode FULL_CMD :\n");
	print_arrarr(node->full_cmd);
	printf("\033[33mIN_FILE : %d\n", node->infile);
	printf("\033[34mOUT_FILE : %d\n\033[0m", node->outfile);
}
