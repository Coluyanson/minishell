/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exe_02.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcolucci <dcolucci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/10 20:53:40 by dcolucci          #+#    #+#             */
/*   Updated: 2023/06/10 21:01:13 by dcolucci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern int	g_status;

int	**ft_pipe_array(int n_pipes)
{
	int		**fd;
	int		i;

	i = 0;
	fd = 0;
	if (n_pipes <= 0)
		return (0);
	fd = (int **) malloc (sizeof(int *) * (n_pipes));
	while (i < n_pipes)
	{
		fd[i] = (int *) malloc (sizeof(int) * 2);
		if (pipe(fd[i]) == -1)
		{
			ft_putstr_fd("\033[33mERROR CREATING PIPE\n\033", STDERR_FILENO);
			return (0);
		}
		i++;
	}
	return (fd);
}

void	ft_close_array_pipes(int size, int **fd)
{
	int		i;

	i = 0;
	if (size > 1)
	{
		while (i < size - 1)
		{
			close(fd[i][1]);
			close(fd[i][0]);
			free(fd[i++]);
		}
		free(fd);
	}
}
