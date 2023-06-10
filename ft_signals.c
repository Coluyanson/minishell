/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_signals.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcolucci <dcolucci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/10 21:03:51 by dcolucci          #+#    #+#             */
/*   Updated: 2023/06/10 21:03:59 by dcolucci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern int	g_status;

void	ft_set_gstatus(int sig)
{
	if (sig == SIGINT)
		g_status = 130;
}

void	ft_sigint(int sig)
{
	if (sig == SIGINT)
	{
		g_status = 130;
		ioctl(STDIN_FILENO, TIOCSTI, "\n");
		rl_on_new_line();
		rl_replace_line("", 0);
	}
}

void	ft_close(int sig)
{
	(void)sig;
	printf("oi\n");
	exit(0);
}

void	ft_gest_sig_bash(void)
{
	signal(SIGINT, ft_sigint);
	signal(SIGQUIT, SIG_IGN);
}
