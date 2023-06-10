/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exe.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcolucci <dcolucci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/18 19:57:26 by dcolucci          #+#    #+#             */
/*   Updated: 2023/06/10 17:59:21 by dcolucci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern int g_status;

char	*ft_cmd_finder(t_node *node, t_sh *shell)
{
	char	*cmd;
	char	**env_PATH;
	char	*full_cmd;
	int		x;
	DIR		*dir;

	(void)shell;
	x = 0;
	if (!(node->cmds))
		return (0);
	if (ft_strchr(node->cmds, '/'))
	{
		if (!access(node->cmds, F_OK))
		{
			if (access(node->cmds, X_OK))
			{
				ft_putstr_fd("\033[31mminishell: ", STDERR_FILENO);
				ft_putstr_fd(node->cmds, STDERR_FILENO);
				ft_putstr_fd(": Permission denied\n\033", STDERR_FILENO);
				g_status = 126;
				return (0);
			}
			dir = opendir(node->cmds);
			if (dir)
			{
				ft_putstr_fd("\033[31mminishell: ", STDERR_FILENO);
				ft_putstr_fd(node->cmds, STDERR_FILENO);
				ft_putstr_fd(": Is a directory\n\033", STDERR_FILENO);
				g_status = 126;
				free(dir);
				return (0);
			}
			else
			{
				return (ft_strdup(node->cmds));
			}
		}
		else
		{
			ft_putstr_fd("\033[31mminishell: ", STDERR_FILENO);
			ft_putstr_fd(node->cmds, STDERR_FILENO);
			ft_putstr_fd(": No such file o directory\n\033", STDERR_FILENO);
			g_status = 127;
			return (0);
		}
	}
	cmd = ft_strjoin("/", node->cmds);
	env_PATH = ft_split(getenv("PATH"), ':');
	while (env_PATH[x])
	{
		full_cmd = ft_strjoin(env_PATH[x], cmd);
		if (!access(full_cmd, F_OK | X_OK))
		{
			free_arrarr(env_PATH);
			free(cmd);
			return (full_cmd);
		}
		free(full_cmd);
		x++;
	}
	if (!env_PATH[x])
	{
		printf("\033[3;31m%s : command not found🐓🐔\n\033[0m", node->cmds);
		g_status = 127;
	}
	free_arrarr(env_PATH);
	free(cmd);
	return (0);
}

void	ft_prepare_redirection(t_sh *shell, t_list *cmd, int **fd, int i)
{
	t_node	*node;

	node = (t_node *)cmd->content;
	if (cmd == *(shell->cmds) && !cmd->next)	//one command
	{
		dup2(node->infile, STDIN_FILENO);
		dup2(node->outfile, STDOUT_FILENO);
	}
	else if (cmd == *(shell->cmds))			//first command
	{
		if (ft_out(node))
			dup2(node->outfile, STDOUT_FILENO);
		else
			dup2(fd[i][1], STDOUT_FILENO);
		dup2(node->infile, STDIN_FILENO);
	}
	else if (!cmd->next)					//last command
	{
		if (ft_in(node))
			dup2(node->infile, STDIN_FILENO);
		else
			dup2(fd[i - 1][0], node->infile);
		dup2(shell->stdout_fd, STDOUT_FILENO);
		dup2(node->outfile, STDOUT_FILENO);
	}
	else									// middle command
	{
		if (ft_out(node))
			dup2(node->outfile, STDOUT_FILENO);
		else
			dup2(fd[i][1], node->outfile);
		if (ft_in(node))
			dup2(node->infile, STDIN_FILENO);
		else
			dup2(fd[i - 1][0], node->infile);
	}
}

void	ft_reset_redirection(t_sh *shell)
{
	dup2(shell->stdin_fd, STDIN_FILENO);
	dup2(shell->stdout_fd, STDOUT_FILENO);
}

void	ft_set_gstatus(int sig)
{
	if (sig == SIGINT)
		g_status = 130;
}

void	ft_close_redirection(t_node *node)
{
	if (node->infile != STDIN_FILENO)
		close(node->infile);
	if (node->outfile != STDOUT_FILENO)
		close(node->outfile);
}

int	**ft_pipe_array(int n_pipes)
{
	int		**fd;
	int		i;

	i = 0;
	fd = 0;
	if (n_pipes <= 0)
		return (0);
	fd = (int **) malloc (sizeof(int *) * (n_pipes));
	while (i < n_pipes)
	{
		fd[i] = (int *) malloc (sizeof(int) * 2);
		if (pipe(fd[i]) == -1)
		{
			ft_putstr_fd("\033[33mERROR CREATING PIPE\n\033", STDERR_FILENO);
			return (0);
		}
		i++;
	}
	return (fd);
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
		exit(0);
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

void	ft_close_array_pipes(int size, int **fd)
{
	int		i;

	i = 0;
	if (size > 1)
	{
		while (i < size - 1)
		{
			close(fd[i][1]);
			close(fd[i][0]);
			free(fd[i++]);
		}
		free(fd);
	}
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
	ft_reset_redirection(shell);
}
