/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcolucci <dcolucci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/24 18:48:20 by dcolucci          #+#    #+#             */
/*   Updated: 2023/05/21 16:54:56 by dcolucci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(int ac, char **av, char **envp)
{

	(void)ac;
	(void)av;
	(void)envp;
	while (1)
	{	
		input = readline("\033[32mminishell>\033[0m");
		print_arrarr(split_quotes(input));
		add_history(input);
		free_arrarr(str);
		free(input);
	}
	str = str;
}
