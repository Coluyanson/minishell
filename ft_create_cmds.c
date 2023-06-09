/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_create_cmds.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcolucci <dcolucci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/18 19:25:46 by dcolucci          #+#    #+#             */
/*   Updated: 2023/06/09 16:12:15 by dcolucci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern int g_status;

char	*ft_restore_neg(char *s)
{
	int	i;

	i = 0;
	if (!s)
		return (0);
	while (s[i])
	{
		if (s[i] < 0)
			s[i] = -s[i];
		i++;
	}
	return (s);
}

void	ft_restore_neg_arrarr(char **arrarr)
{
	int		x;

	x = 0;
	if (!arrarr)
		return ;
	while (arrarr[x])
		ft_restore_neg(arrarr[x++]);
}

t_list	*ft_new_cmd(char **sub_spl)
{
	t_node	*node;
	t_list	*new;

	if (ft_check_syntax(sub_spl) == -1)
		return (0);
	if (!sub_spl)
	{
		g_status = 2;
		ft_putstr_fd("minishell: syntax error near unexpected token `|'\n", \
		STDERR_FILENO);
		return (0);
	}
	node = (t_node *) malloc (sizeof(t_node));
	node->infile = ft_infile(sub_spl, node);
	if (g_status == 130)
	{
		free(node);
		return (0);
	}
	node->outfile = ft_outfile(sub_spl, node);
	node->full_cmd = ft_full_cmd(sub_spl);
	ft_restore_neg_arrarr(node->full_cmd);
	node->cmds = ft_cmd(sub_spl);
	ft_restore_neg(node->cmds);
	new = ft_lstnew(node);
	return (new);
}

t_list	*ft_add_command(t_list **cmd_lst, char **final, int x, int y)
{
	t_list	*node_lst;
	char	**sub_split;

	node_lst = 0;
	sub_split = ft_subsplit(final, y, x);
	node_lst = ft_new_cmd(sub_split);
	if (!node_lst)
	{
		free_arrarr(sub_split);
		if (cmd_lst)
			ft_free_list(*cmd_lst);
		ft_safe_free(cmd_lst);
		return (0);
	}
	ft_lstadd_back(cmd_lst, node_lst);
	free_arrarr(sub_split);
	return (node_lst);
}

t_list	**ft_create_cmds(char **final, t_sh *shell)
{
	int		x;
	int		y;
	t_list	**cmd_lst;

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
			if (!(ft_add_command(cmd_lst, final, x, y)))
				return (0);
			y = x + 1;
		}
		x++;
	}
	if (!(ft_add_command(cmd_lst, final, x, y)))
		return (0);
	return (cmd_lst);
}
