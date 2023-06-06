#include "minishell.h"
extern int g_status;
/*	0) ctrl +D
	1) ctrl +\
	2) ctrl +C
	
	0-1 = fanno parte di SIGQUIT
	2  = e' SIGINT
	
	1) caso bash
		0 = esce dalla bash e quindi dal programma
		
		1 = non fa nulla sigaction(SIGQUIT, SIG_IGN)
		
		2 = interrompe la scrittura e va a capo(sotto specifiche)
		
	2) caso haredoc
		0 =  bash: warning: here-document at line 3 delimited by end-of-file (wanted `gfh')
			 ed esce dall'heredoc
		
		1 = non fa nulla sigaction(SIGQUIT, SIG_IGN)
		
		2 = interrompe la scrittura e va a capo(sotto specifiche)*/
void	ft_sigint(int sig)
{
	if(sig == SIGINT)
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
