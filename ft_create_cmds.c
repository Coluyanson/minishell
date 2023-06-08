/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_create_cmds.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcolucci <dcolucci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/18 19:25:46 by dcolucci          #+#    #+#             */
/*   Updated: 2023/06/08 14:51:38 by dcolucci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern int g_status;

t_list	*ft_new_cmd(char **sub_spl)
{
	t_node	*node;
	t_list	*new;

	if (ft_check_syntax(sub_spl) == -1)
		return (0);
	node = (t_node *) malloc (sizeof(t_node));
	if (!sub_spl)
	{
		g_status = 2;
		ft_putstr_fd("minishell: syntax error near unexpected token `|'\n", \
		STDERR_FILENO);
		free(node);
		return (0);
	}
	node->infile = ft_infile(sub_spl, node);
	if (g_status == 130)
	{
		free(node);
		return (0);
	}
	node->outfile = ft_outfile(sub_spl, node);
	node->full_cmd = ft_full_cmd(sub_spl);
	node->cmds = ft_cmd(sub_spl);
	new = ft_lstnew(node);
	free_arrarr(sub_spl);
	//print_node(node, sub_spl);
	return (new);
}

t_list	**ft_create_cmds(char **final, t_sh *shell)
{
	int		x;
	int		y;
	char	**sub_split;
	t_list	**cmd_lst;
	t_list	*node;

	if (final == NULL)
		return (NULL);
	(void)shell;
	x = 0;
	y = 0;
	cmd_lst = (t_list **) malloc (sizeof(t_list *));
	*cmd_lst = 0;
	while (final[x])
	{
		if (final[x][0] == '|' && ft_strlen(final[x]) == 1)
		{
			sub_split = ft_subsplit(final, y, x);
			node = ft_new_cmd(sub_split);
			if (!node)
			{
				free_arrarr(sub_split);
				if (cmd_lst)
					ft_free_list(*cmd_lst);
				ft_safe_free(cmd_lst);
				return (0);
			}
			ft_lstadd_back(cmd_lst, node);
			y = x + 1;
		}
		x++;
	}
	sub_split = ft_subsplit(final, y, x);
	if (sub_split)
	{
		node = ft_new_cmd(sub_split);
		if (!node)
		{
			free_arrarr(sub_split);
			if (cmd_lst)
				ft_free_list(*cmd_lst);
			ft_safe_free(cmd_lst);
			return (0);
		}
		ft_lstadd_back(cmd_lst, node);
	}
	return (cmd_lst);
}
