/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_test.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcolucci <dcolucci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/02 19:08:44 by dcolucci          #+#    #+#             */
/*   Updated: 2023/05/22 15:33:26 by dcolucci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	chek;

int	main(int ac, char **av, char **envp)
{
	char	*input;
	char	**fin;
	t_sh	shell;
	int sig;

	(void)ac;
	(void)av;
	shell.envp = envp;
	sig = 0;
	ft_gest_sig_bash(sig);
	while (1)
	{
		input = readline("\033[34mminishell>\033[0m");
		if(!input)
			break ;
		fin = final_split(input, envp);
		if (fin != NULL)
			ft_exe(*(ft_create_cmds(fin)), &shell);
		free_arrarr(fin);
		add_history(input);
	}
	printf("\033[6;31mexit💀\n\033[0m");
}
