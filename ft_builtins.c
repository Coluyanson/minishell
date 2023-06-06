/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_builtins.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcolucci <dcolucci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/27 17:18:16 by dcolucci          #+#    #+#             */
/*   Updated: 2023/06/06 19:32:42 by dcolucci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern int g_status;

int	ft_echo(t_node *node)
{
	int x;
	int flag;

	flag = 0;
	x = 1;
	if (node->full_cmd[1])
	{
		if (!(ft_strncmp(node->full_cmd[1], "-n", 3)))
		{
			flag = 1;
			x++;
		}
	}
	while(node->full_cmd[x])
	{
		if (node->full_cmd[x + 1])
		{
			ft_putstr_fd(node->full_cmd[x++], STDOUT_FILENO);
			ft_putstr_fd(" ", STDOUT_FILENO);
		}	//printf("%s ", node->full_cmd[x++]);
		else
			ft_putstr_fd(node->full_cmd[x++], STDOUT_FILENO);
			//printf("%s", node->full_cmd[x++]);
		}
	if(flag == 0)
		ft_putstr_fd("\n", STDOUT_FILENO);
	g_status = 0;
	return (1);
}

int	ft_pwd(void)
{
	char *tmp;

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
				printf("minishell: env: too many arguments\n");
				g_status = 1;
				return (1);
			}
		}
	}
	while (sh->envp[x])
		printf("%s\n", sh->envp[x++]);
	g_status = 0;
	return (1);
}


int	ft_cd(t_node *node, t_sh *shell)
{
	char	*old_pwd;
	char	*pwd;
	char	*env;

	old_pwd = getcwd(0, 0);
	if (node->full_cmd[1] == NULL)
	{
		env = ft_getenv("HOME", shell->envp);
		chdir(env);
		pwd = getcwd(0, 0);
		ft_setenv(shell, "OLDPWD", old_pwd);
		ft_setenv(shell, "PWD", pwd);
		free(pwd);
		free(env);
	}
	else if (node->full_cmd[1] != NULL)
	{	
		if (node->full_cmd[2])
		{
			printf("minishell: cd: too many arguments\n");
			g_status = 1;
			return (1);
		}
		if (chdir(node->full_cmd[1]) == -1)
		{	
			printf("minishell: cd: %s :No such file or directory\n", \
				node->full_cmd[1]);
			g_status = 1;
			return (1);
		}
		else
		{
			pwd = getcwd(0, 0);
			ft_setenv(shell, "OLDPWD", old_pwd);
			ft_setenv(shell, "PWD", pwd);
			free(pwd);
		}
	}
	free(old_pwd);
	g_status = 0;
	return (1);
}

int	ft_export(t_node *node, t_sh *sh)
{
	int		x;
	int		y;
	char	*var;
	char	*value;

	x = 0;
	y = 1;
	if (!node->full_cmd[1])
	{
		while (sh->envp[x])
			printf("%s\n", sh->envp[x++]);
		g_status = 0;
		return (1);
	}
	while (node->full_cmd[y])
	{
		if (ft_strchr(node->full_cmd[y], '='))
		{
			var = ft_truncate_eq(node->full_cmd[y]);
			value = ft_strdup(ft_strchr(node->full_cmd[y], '=') + 1);
			ft_setenv(sh, var, value);
			ft_safe_free(var);
			ft_safe_free(value);
		}
		else
			ft_setenv(sh, node->full_cmd[y], 0);
		
		y++;
	}
	return (1);
}

int	ft_unset(t_node *node, t_sh *sh)
{
	char	*trun_env;
	int		x;
	char	**s1;
	char	**s2;
	char	**tmp_sh;

	x = 0;
	if (!node->full_cmd[1])
	{
		g_status = 0;
		return (1);
	}
	else if (ft_strchr(node->full_cmd[1], '='))
	{
		printf("\033[31munset : %s : invalid parameter name\033\n", node->full_cmd[1]);
		g_status = 1;
		return (1);
	}
	else
	{
		while (sh->envp[x])
		{
			trun_env = ft_truncate_eq(sh->envp[x]);
			if (!ft_strncmp(node->full_cmd[1], trun_env, ft_max(ft_strlen(node->full_cmd[1]), ft_strlen(trun_env))))
			{
				s1 = ft_subsplit(sh->envp, 0, x);
				s2 = ft_subsplit(sh->envp, x + 1, ft_splitlen(sh->envp));
				tmp_sh = sh->envp;
				sh->envp = ft_join_split(s1, s2);
				free_arrarr(tmp_sh);
				free_arrarr(s1);
				free_arrarr(s2);
				ft_safe_free(trun_env);
				return (1);
			}
			ft_safe_free(trun_env);
			x++;
		}	
	}
	g_status = 0;
	return (1);
}

int	ft_builtins(t_node *node, t_sh *sh, int *fd, t_list *cmd)
{
	int x;
	int	builtin;

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
		close(fd[1]);
	return(builtin);
}