/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_test.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcolucci <dcolucci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/02 19:08:44 by dcolucci          #+#    #+#             */
/*   Updated: 2023/06/07 18:47:49 by dcolucci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
int g_status;

void	ft_init_shell(t_sh *shell)
{
	shell->cmds = 0;
	shell->envp = 0;
	shell->final_split = 0;
}

char	*ft_create_prompt(t_sh shell)
{
	char	*tmp1;
	char	*tmp2;
	char	*tmp3;
	char	*tmp4;

	tmp1 = ft_getenv("USER", shell.envp);
	tmp2 = "\033[0m@\033[31mminishell>\033[0m";
	tmp4 = ft_strjoin_null("\033[32m", tmp1);
	tmp3 = ft_strjoin_null(tmp4, tmp2);
	free(tmp1);
	free(tmp4);
	return (tmp3);
}

int	main(int ac, char **av, char **envp)
{
	char	*input;
	char	*prompt;
	char	**fin;
	t_sh	shell;

	(void)ac;
	(void)av;
	ft_init_shell(&shell);
	shell.stdin_fd = dup(STDIN_FILENO);
	shell.stdout_fd = dup(STDOUT_FILENO);
	shell.envp = copy_arrarr(envp);
	ft_gest_sig_bash();
	(void)fin;
	while (1)
	{
		prompt = ft_create_prompt(shell);
		input = readline(prompt);
		if (!input)
		{
			ft_putstr_fd("\033[32mexit\033[0m \033👨‍💻", shell.stdout_fd);
			ft_putchar_fd('\n', shell.stdout_fd);
			ft_free_shell(&shell);
			free(prompt);
			break ;
		}
		if (ft_strlen(input))
		{
			shell.final_split = final_split(input, shell.envp);
			shell.cmds = ft_create_cmds(shell.final_split, &shell);
			if (shell.cmds)
			{
				ft_exe(&shell, *(shell.cmds));
				ft_free_list(*(shell.cmds));
				*(shell.cmds) = 0;
				free(shell.cmds);
				shell.cmds = 0;
			}
			free_arrarr(shell.final_split);
			shell.final_split = 0;
			ft_safe_free(prompt);
			add_history(input);
		}
	}
	unlink("heredoc");
}
