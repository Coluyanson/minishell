/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils1_free.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcolucci <dcolucci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/04 15:41:44 by dcolucci          #+#    #+#             */
/*   Updated: 2023/06/06 16:34:55 by dcolucci         ###   ########.fr       */
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
	if (node->full_cmd)
		free_arrarr(node->full_cmd);
	if (node->cmds)
		free(node->cmds);
	if (node->str_infile)
		free(node->str_infile);
	if (node->str_outfile)
		free(node->str_outfile);
}

void	ft_free_list(t_list *lst)
{
	t_list	*tmp;

	if (!lst)
		return ;
	while (lst)
	{
		tmp = lst;
		ft_free_tnode((t_node * )(tmp->content));
		free(tmp->content);
		lst = lst->next;
		free(tmp);
	}
}

void	ft_free_shell(t_sh *shell)
{
	free_arrarr(shell->envp);
	ft_free_list(*(shell->cmds));
}

void	ft_safe_free(void *p)
{
	if (p)
		free(p);
}