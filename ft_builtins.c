/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_builtins.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcolucci <dcolucci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/27 17:18:16 by dcolucci          #+#    #+#             */
/*   Updated: 2023/06/10 20:41:29 by dcolucci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern int	g_status;

int	ft_remove_unset(char *arg, int x, t_sh *sh)
{
	char	*trun_env;
	char	**tmp_envp;
	char	**s1;
	char	**s2;
	int		found;

	found = 0;
	trun_env = ft_truncate_eq(sh->envp[x]);
	if (!ft_strncmp(arg, trun_env, \
	ft_max(ft_strlen(arg), ft_strlen(trun_env))))
	{
		s1 = ft_subsplit(sh->envp, 0, x);
		s2 = ft_subsplit(sh->envp, x + 1, ft_splitlen(sh->envp));
		tmp_envp = sh->envp;
		sh->envp = ft_join_split(s1, s2);
		free_arrarr(tmp_envp);
		free_arrarr(s1);
		free_arrarr(s2);
		found = 1;
	}
	ft_safe_free(trun_env);
	return (found);
}

int	ft_unset(t_node *node, t_sh *sh)
{
	int		x;

	x = 0;
	if (!node->full_cmd[1])
	{
		g_status = 0;
		return (1);
	}
	else if (ft_strchr(node->full_cmd[1], '='))
	{
		printf("\033[31munset : %s : invalid parameter name\033\n", \
		node->full_cmd[1]);
		g_status = 1;
		return (1);
	}
	else
	{
		while (sh->envp[x])
		{
			if (ft_remove_unset(node->full_cmd[1], x++, sh))
				break ;
		}	
	}
	g_status = 0;
	return (1);
}

int	ft_pick_builtin(t_sh *sh, t_node *node, int x)
{
	if (!(ft_strncmp(node->full_cmd[x], "echo", \
		ft_max(ft_strlen(node->full_cmd[x]), ft_strlen("echo")))))
		return (ft_echo(node));
	else if (!(ft_strncmp(node->full_cmd[x], "pwd", \
		ft_max(ft_strlen(node->full_cmd[x]), ft_strlen("pwd")))))
		return (ft_pwd());
	else if (!(ft_strncmp(node->full_cmd[x], "exit", \
		ft_max(ft_strlen(node->full_cmd[x]), ft_strlen("exit")))))
	{
		ft_free_shell(sh);
		exit(0);
	}
	else if (!(ft_strncmp(node->full_cmd[x], "env", \
	ft_max(ft_strlen(node->full_cmd[x]), ft_strlen("env")))))
		return (ft_env(sh, node));
	else if (!(ft_strncmp(node->full_cmd[x], "cd", \
	ft_max(ft_strlen(node->full_cmd[x]), ft_strlen("cd")))))
		return (ft_cd(node, sh));
	else if (!(ft_strncmp(node->full_cmd[x], "export", \
	ft_max(ft_strlen(node->full_cmd[x]), ft_strlen("export")))))
		return (ft_export(node, sh));
	else if (!(ft_strncmp(node->full_cmd[x], "unset", \
	ft_max(ft_strlen(node->full_cmd[x]), ft_strlen("unset")))))
		return (ft_unset(node, sh));
	return (0);
}

int	ft_builtins(t_list *cmd, t_sh *sh, int **fd, int i)
{
	int		x;
	int		builtin;
	t_node	*node;

	node = (t_node *)(cmd->content);
	x = 0;
	builtin = 0;
	if (!node->cmds || !node->full_cmd)
		return (0);
	if (node->full_cmd[x])
		builtin = ft_pick_builtin(sh, node, x);
	if (cmd->next)
		close(fd[i][1]);
	return (builtin);
}
