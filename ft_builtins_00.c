/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_builtins_00.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcolucci <dcolucci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/10 20:37:35 by dcolucci          #+#    #+#             */
/*   Updated: 2023/06/10 20:41:46 by dcolucci         ###   ########.fr       */
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

static void	ft_change_path(char *path, t_sh *shell)
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
