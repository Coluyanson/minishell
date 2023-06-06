/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_cmd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcolucci <dcolucci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/26 15:07:32 by dcolucci          #+#    #+#             */
/*   Updated: 2023/05/26 18:55:06 by dcolucci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_in(t_node *node)
{
	if (node->infile != STDIN_FILENO)
		return (1);
	return (0);
}

int	ft_out(t_node *node)
{
	if (node->outfile != STDOUT_FILENO)
		return (1);
	return (0);
}