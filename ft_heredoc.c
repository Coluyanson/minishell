/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_heredoc.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcolucci <dcolucci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/21 16:23:36 by dcolucci          #+#    #+#             */
/*   Updated: 2023/05/22 15:56:10 by dcolucci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_heredoc(char *delimiter)
{
	char	*inp_heredoc;
	int		fd;
	(void)delimiter;
	signal(SIGQUIT,SIG_IGN);
	printf("%s\n", delimiter);
	fd = open("./tmp/heredoc", O_CREAT | O_TRUNC | O_WRONLY, S_IRWXU, 0666);
	while (1)
	{
		
		inp_heredoc = readline("\033[33m🤟🍀🤟>\033[0m");
		if(!inp_heredoc)
		{
			printf("\n");
			break;
		}
		if (!ft_strncmp(inp_heredoc, delimiter, \
		ft_max(ft_strlen(inp_heredoc), ft_strlen(delimiter))))
			break ;
		ft_putstr_fd(inp_heredoc, fd);
		ft_putstr_fd("\n", fd);
	}
	
	//ft_gest_sig_bash;
	return (fd);
}
