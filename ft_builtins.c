/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_builtins.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcolucci <dcolucci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/27 17:18:16 by dcolucci          #+#    #+#             */
/*   Updated: 2023/06/08 17:45:12 by dcolucci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern int	g_status;

int	ft_echo(t_node *node)
{
	int	x;
	int	flag;

	flag = 0;
	x = 1;

	while (node->full_cmd[x])
	{
		if (x == 1 && (!(ft_strncmp(node->full_cmd[x], "-n", 3))))
		{
			flag = 1;
			x++;
		}
		else if (node->full_cmd[x + 1])
		{
			ft_putstr_fd(node->full_cmd[x++], STDOUT_FILENO);
			ft_putstr_fd(" ", STDOUT_FILENO);
		}
		else
			ft_putstr_fd(node->full_cmd[x++], STDOUT_FILENO);
	}
	if (flag == 0)
		ft_putstr_fd("\n", STDOUT_FILENO);
	g_status = 0;
	return (1);
}

int	ft_pwd(void)
{
	char	*tmp;

	tmp = getcwd(0, 0);
	printf("%s\n", tmp);
	free(tmp);
	g_status = 0;
	return (1);
}

int	ft_env(t_sh *sh, t_node *node)
{
	int	x;

	x = 0;
	if (node->full_cmd)
	{
		if (node->full_cmd[1])
		{
			if (node->full_cmd[2])
			{
				ft_putstr_fd("minishell: env: too many arguments\n", \
				STDERR_FILENO);
				g_status = 1;
				return (1);
			}
		}
	}
	while (sh->envp[x])
	{
		ft_putstr_fd(sh->envp[x++], STDOUT_FILENO);
		ft_putstr_fd("\n", STDOUT_FILENO);
	}
	g_status = 0;
	return (1);
}

void	ft_change_path(char *path, t_sh *shell)
{
	char	*pwd;
	char	*old_pwd;

	pwd = 0;
	old_pwd = getcwd(0, 0);
	if (chdir(path) == -1)
	{	
		printf("minishell: cd: %s :No such file or directory\n", \
			path);
		ft_safe_free(pwd);
		ft_safe_free(old_pwd);
		g_status = 1;
		return ;
	}
	else
	{
		pwd = getcwd(0, 0);
		ft_setenv(shell, "OLDPWD", old_pwd);
		ft_setenv(shell, "PWD", pwd);
	}
	ft_safe_free(pwd);
	ft_safe_free(old_pwd);
	g_status = 0;
	return ;
}

int	ft_cd(t_node *node, t_sh *shell)
{
	char	*env;

	if (node->full_cmd[1])
	{
		if (node->full_cmd[2])
		{
			printf("minishell: cd: too many arguments\n");
			g_status = 1;
			return (1);
		}
	}
	if (node->full_cmd[1] == NULL)
	{
		env = ft_getenv("HOME", shell->envp);
		ft_change_path(env, shell);
		free(env);
	}
	else
		ft_change_path(node->full_cmd[1], shell);
	return (1);
}

void	ft_set_export(t_sh *sh, char *arg)
{
	char	*var;
	char	*value;

	if (ft_strchr(arg, '='))
	{
		var = ft_truncate_eq(arg);
		value = ft_strdup(ft_strchr(arg, '=') + 1);
		ft_setenv(sh, var, value);
		ft_safe_free(var);
		ft_safe_free(value);
	}
	else
		ft_setenv(sh, arg, 0);
}

int	ft_export(t_node *node, t_sh *sh)
{
	int		x;

	x = 0;
	g_status = 0;
	if (!node->full_cmd[1])
	{
		while (sh->envp[x])
		{
			ft_putstr_fd(sh->envp[x++], STDOUT_FILENO);
			ft_putchar_fd('\n', STDOUT_FILENO);
		}
		return (1);
	}
	x = 1;
	while (node->full_cmd[x])
	{
		ft_set_export(sh, node->full_cmd[x++]);
	}
	return (1);
}

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
	if(node->full_cmd[x])
	{
		if (!(ft_strncmp(node->full_cmd[x], "echo", ft_max(ft_strlen(node->full_cmd[x]), ft_strlen("echo")))))
			builtin = (ft_echo(node));
		else if (!(ft_strncmp(node->full_cmd[x], "pwd", ft_max(ft_strlen(node->full_cmd[x]), ft_strlen("pwd")))))
			builtin = (ft_pwd());
		else if (!(ft_strncmp(node->full_cmd[x], "exit", ft_max(ft_strlen(node->full_cmd[x]), ft_strlen("exit")))))
		{
			ft_free_shell(sh);
			exit(0);
		}
		else if (!(ft_strncmp(node->full_cmd[x], "env", ft_max(ft_strlen(node->full_cmd[x]), ft_strlen("env")))))
			builtin = (ft_env(sh, node));
		else if (!(ft_strncmp(node->full_cmd[x], "cd", ft_max(ft_strlen(node->full_cmd[x]), ft_strlen("cd")))))
			builtin = (ft_cd(node, sh));
		else if (!(ft_strncmp(node->full_cmd[x], "export", ft_max(ft_strlen(node->full_cmd[x]), ft_strlen("export")))))
			builtin = (ft_export(node, sh));
		else if (!(ft_strncmp(node->full_cmd[x], "unset", ft_max(ft_strlen(node->full_cmd[x]), ft_strlen("unset")))))
			builtin = (ft_unset(node, sh));
	}
	if (cmd->next)
		close(fd[i][1]);
	return(builtin);
}