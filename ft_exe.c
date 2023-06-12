/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exe.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcolucci <dcolucci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/18 19:57:26 by dcolucci          #+#    #+#             */
/*   Updated: 2023/06/11 17:00:46 by dcolucci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern int	g_status;

void	ft_close_redirection(t_node *node)
{
	if (node->infile != STDIN_FILENO)
		close(node->infile);
	if (node->outfile != STDOUT_FILENO)
		close(node->outfile);
}

void	ft_bad_fd(t_node *node)
{
	if (node->infile == -1)
	{
		ft_putstr_fd("minishell : cannot open file ", STDERR_FILENO);
		ft_putstr_fd(node->str_infile, STDERR_FILENO);
		ft_putstr_fd("\n", STDERR_FILENO);
	}
	else if (node->outfile == -1)
	{
		ft_putstr_fd("minishell : cannot open file ", STDERR_FILENO);
		ft_putstr_fd(node->str_outfile, STDERR_FILENO);
		ft_putstr_fd("\n", STDERR_FILENO);
	}
	g_status = 1;
}

int	ft_executor(t_list *cmd, t_node *node, t_sh *shell, int *fd)
{
	char	*full_cmd;
	int		status;
	pid_t	pid;

	full_cmd = ft_cmd_finder(node, shell);
	if (!full_cmd)
		return (1);
	pid = fork();
	if (pid == 0)
	{
		execve(full_cmd, node->full_cmd, shell->envp);
		free(full_cmd);
		ft_quit_shell(shell);
	}
	free(full_cmd);
	signal(SIGINT, ft_set_gstatus);
	signal(SIGQUIT, SIG_IGN);
	if (cmd->next)
		close(fd[1]);
	waitpid(pid, &status, 0);
	if (g_status == 130)
		return (130);
	if (WIFEXITED(status))
		g_status = WEXITSTATUS(status);
	return (0);
}

int	ft_action_on_cmd(t_list *cmd, t_sh *shell, int **fd, int i)
{
	int		exe;
	t_node	*node;

	exe = 0;
	node = (t_node *)(cmd->content);
	ft_prepare_redirection(shell, cmd, fd, i);
	if (node->infile == -1 || node->outfile == -1)
		ft_bad_fd(node);
	else if (!node->cmds && (ft_in(node) || ft_out(node)))
	{
	}
	else
	{
		if (!ft_builtins(cmd, shell, fd, i))
		{
			if (fd && cmd->next)
				exe = ft_executor(cmd, node, shell, fd[i]);
			else
				exe = ft_executor(cmd, node, shell, 0);
			ft_gest_sig_bash();
		}
	}
	ft_close_redirection(node);
	return (exe);
}

void	ft_exe(t_sh *shell, t_list *cmd)
{
	int		i;
	int		**fd;
	int		exe;

	i = 0;
	if (!cmd)
		return ;
	fd = ft_pipe_array(ft_lstsize(*(shell->cmds)) - 1);
	while (cmd)
	{
		exe = ft_action_on_cmd(cmd, shell, fd, i);
		if (exe == 130)
			return ;
		else if (exe == 1)
			break ;
		cmd = cmd->next;
		i++;
	}
	ft_close_array_pipes(ft_lstsize(*(shell->cmds)), fd);
	dup2(shell->stdin_fd, STDIN_FILENO);
	dup2(shell->stdout_fd, STDOUT_FILENO);
}
