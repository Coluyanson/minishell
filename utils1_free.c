/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils1_free.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcolucci <dcolucci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/04 15:41:44 by dcolucci          #+#    #+#             */
/*   Updated: 2023/06/07 16:12:30 by dcolucci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_arrarr(char **mat)
{
	int	i;

	if (!mat)
		return ;
	i = 0;
	while (mat[i])
		free(mat[i++]);
	free(mat);
}

void	ft_free_tnode(t_node *node)
{
	free_arrarr(node->full_cmd);
	ft_safe_free(node->cmds);
	ft_safe_free(node->str_infile);
	ft_safe_free(node->str_outfile);
}

void	ft_free_list(t_list *lst)
{
	t_list	*tmp;

	if (!lst)
		return ;
	while (lst)
	{
		tmp = lst;
		ft_free_tnode((t_node *)(tmp->content));
		ft_safe_free(tmp->content);
		lst = lst->next;
		ft_safe_free(tmp);
	}
}

void	ft_free_shell(t_sh *shell)
{
	free_arrarr(shell->final_split);
	shell->final_split = 0;
	free_arrarr(shell->envp);
	shell->envp = 0;
	if (shell->cmds)
	{
		ft_free_list(*(shell->cmds));
		*(shell->cmds) = 0;
	}
	ft_safe_free(shell->cmds);
	shell->cmds = 0;
}

void	ft_safe_free(void *p)
{
	if (p)
		free(p);
}