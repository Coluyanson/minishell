/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exe.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcolucci <dcolucci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/18 19:57:26 by dcolucci          #+#    #+#             */
/*   Updated: 2023/05/22 17:32:43 by dcolucci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_executor(char *full_cmd, t_sh *shell, t_node *node)
{
	int	pid;

	pid = fork();
	if (pid == 0)
		execve(full_cmd, node->full_cmd, shell->envp);
	else if (pid > 0)
		waitpid(pid, 0, 0);
	else
		printf("Error executing command\n");
}

void	ft_path_finder(t_node *node, t_sh *shell)
{
	char	*cmd;
	char	**env_PATH;
	char	*full_cmd;
	int		x;

	x = 0;
	if(!(node->cmds))
		return;
	if (ft_strchr(node->cmds, '/'))
	{
		if (!access(node->cmds, F_OK | X_OK))
			ft_executor(node->cmds, shell, node);
		else
			printf("%s : no such file or directory\n", node->cmds);
		return ;
	}
	if(ft_builtins(node, shell))
		return ;
	cmd = ft_strjoin("/", node->cmds);
	env_PATH = ft_split(getenv("PATH"), ':');
	while (env_PATH[x])
	{
		full_cmd = ft_strjoin(env_PATH[x], cmd);
		if (!access(full_cmd, F_OK | X_OK))
		{
			ft_executor(full_cmd, shell, node);
			break ;
		}
		free(full_cmd);
		x++;
	}
	if (!env_PATH[x])
		printf("\033[3;31m%s : command not found🐓🐔\n\033[0m", node->cmds);
}

void	ft_exe(t_list *cmds, t_sh *shell)
{
	t_list	*tmp;

	tmp = cmds;
	if (!cmds)
		return ;
	while (tmp)
	{
		ft_path_finder((t_node *)tmp->content, shell);
		tmp = tmp->next;
	}
}
