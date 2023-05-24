#include "minishell.h"
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
		chek = 0;
		if(sig == SIGINT)
		{
			ioctl(STDIN_FILENO, TIOCSTI, "\n");
			chek = 1;
			rl_replace_line("", 0);
			rl_on_new_line();
		}
}

void	ft_gest_sig_bash(int sig)
{
	//struct sigaction
	signal(SIGINT, ft_sigint);
	signal(SIGQUIT, SIG_IGN);
	sig = sig;
}
