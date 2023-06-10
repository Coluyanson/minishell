/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exe_01.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcolucci <dcolucci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/10 20:53:37 by dcolucci          #+#    #+#             */
/*   Updated: 2023/06/10 20:56:57 by dcolucci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern int	g_status;

void	ft_redirection_one(t_node *node)
{
	dup2(node->infile, STDIN_FILENO);
	dup2(node->outfile, STDOUT_FILENO);
}

void	ft_redirection_first(t_node *node, int **fd, int i)
{
	if (ft_out(node))
		dup2(node->outfile, STDOUT_FILENO);
	else
		dup2(fd[i][1], STDOUT_FILENO);
	dup2(node->infile, STDIN_FILENO);
}

void	ft_redirection_last(t_sh *shell, t_node *node, int **fd, int i)
{
	if (ft_in(node))
		dup2(node->infile, STDIN_FILENO);
	else
		dup2(fd[i - 1][0], node->infile);
	dup2(shell->stdout_fd, STDOUT_FILENO);
	dup2(node->outfile, STDOUT_FILENO);
}

void	ft_redirection_middle(t_node *node, int **fd, int i)
{
	if (ft_out(node))
		dup2(node->outfile, STDOUT_FILENO);
	else
		dup2(fd[i][1], node->outfile);
	if (ft_in(node))
		dup2(node->infile, STDIN_FILENO);
	else
		dup2(fd[i - 1][0], node->infile);
}

void	ft_prepare_redirection(t_sh *shell, t_list *cmd, int **fd, int i)
{
	t_node	*node;

	node = (t_node *)cmd->content;
	if (cmd == *(shell->cmds) && !cmd->next)
		ft_redirection_one(node);
	else if (cmd == *(shell->cmds))
		ft_redirection_first(node, fd, i);
	else if (!cmd->next)
		ft_redirection_last(shell, node, fd, i);
	else
		ft_redirection_middle(node, fd, i);
}
