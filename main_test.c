/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_test.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcolucci <dcolucci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/02 19:08:44 by dcolucci          #+#    #+#             */
/*   Updated: 2023/05/24 18:49:17 by dcolucci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(int ac, char **av, char **envp)
{
	char	*input;
	char	**fin;
	t_sh	shell;

	(void)ac;
	(void)av;
	shell.envp = envp;
	printf("%d\n", getpid());
	shell.stdin_fd = dup(STDIN_FILENO);
	while (1)
	{
		input = readline("\033[34mminishell>\033[0m");
		fin = final_split(input, envp);
		if (fin != NULL)
			shell.cmds = *ft_create_cmds(fin);
		ft_exe(&shell, shell.cmds);
		free_arrarr(fin);
		add_history(input);
	}
}
