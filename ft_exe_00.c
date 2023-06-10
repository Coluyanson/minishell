/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exe_00.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcolucci <dcolucci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/10 20:53:34 by dcolucci          #+#    #+#             */
/*   Updated: 2023/06/10 20:55:57 by dcolucci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern int	g_status;

char	*ft_check_file(char *path)
{
	DIR		*dir;

	if (access(path, X_OK))
	{
		ft_putstr_fd("\033[31mminishell: ", STDERR_FILENO);
		ft_putstr_fd(path, STDERR_FILENO);
		ft_putstr_fd(": Permission denied\n\033", STDERR_FILENO);
		g_status = 126;
		return (0);
	}
	dir = opendir(path);
	if (dir)
	{
		ft_putstr_fd("\033[31mminishell: ", STDERR_FILENO);
		ft_putstr_fd(path, STDERR_FILENO);
		ft_putstr_fd(": Is a directory\n\033", STDERR_FILENO);
		g_status = 126;
		free(dir);
		return (0);
	}
	else
		return (ft_strdup(path));
	return (0);
}

char	*ft_search_env(char **env_path, char *cmd, t_node *node)
{
	int		x;
	char	*full_cmd;

	x = 0;
	while (env_path[x])
	{
		full_cmd = ft_strjoin(env_path[x], cmd);
		if (!access(full_cmd, F_OK | X_OK))
		{
			free_arrarr(env_path);
			free(cmd);
			return (full_cmd);
		}
		free(full_cmd);
		x++;
	}
	if (!env_path[x])
	{
		ft_putstr_fd(node->cmds, STDOUT_FILENO);
		ft_putstr_fd(" : command not found🐓🐔\n", STDOUT_FILENO);
		g_status = 127;
	}
	free_arrarr(env_path);
	free(cmd);
	return (0);
}

char	*ft_find_cmd(t_node *node, t_sh *shell)
{
	char	*cmd;
	char	**env_path;
	char	*tmp_env;

	cmd = ft_strjoin("/", node->cmds);
	tmp_env = ft_getenv("PATH", shell->envp);
	env_path = ft_split(tmp_env, ':');
	free(tmp_env);
	return (ft_search_env(env_path, cmd, node));
}

char	*ft_cmd_finder(t_node *node, t_sh *shell)
{
	if (!(node->cmds))
		return (0);
	if (ft_strchr(node->cmds, '/'))
	{
		if (!access(node->cmds, F_OK))
			return (ft_check_file(node->cmds));
		else
		{
			ft_putstr_fd("\033[31mminishell: ", STDERR_FILENO);
			ft_putstr_fd(node->cmds, STDERR_FILENO);
			ft_putstr_fd(": No such file o directory\n\033", STDERR_FILENO);
			g_status = 127;
			return (0);
		}
	}
	return (ft_find_cmd(node, shell));
}
