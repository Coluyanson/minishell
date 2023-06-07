/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exe.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcolucci <dcolucci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/18 19:57:26 by dcolucci          #+#    #+#             */
/*   Updated: 2023/06/07 18:10:08 by dcolucci         ###   ########.fr       */
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
		if (!access(node->cmds, F_OK | X_OK))
		{
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

void	ft_exe(t_sh *shell, t_list *cmd)
{
	pid_t	pid;
	t_node	*node;
	int		**fd;
	int		i;
	char	*full_cmd;
	int		status;
	int 	size;
	
	size = ft_lstsize(cmd);
	i = 0;
	fd = 0;
	if (!cmd)
		return ;
	if (size > 1)
	{
		fd = (int **) malloc (sizeof(int *) * (size - 1));
		while (i < size - 1)
		{
			fd[i] = (int *) malloc (sizeof(int) * 2);
			if (pipe(fd[i]) == -1)
			{
				ft_putstr_fd("\033[33mERROR CREATING PIPE\n\033", STDERR_FILENO);
				return;
			}
			i++;
		}
	}
	i = 0;
	while (cmd)
	{
		node = (t_node *)cmd->content;
		ft_prepare_redirection(shell, cmd, fd, i);
		if (node->infile == -1 || node->outfile == -1)
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
		else if (!node->cmds && (ft_in(node) || ft_out(node)))
		{
		}
		else
		{
			if (!ft_builtins(cmd, shell, fd, i))
			{
				full_cmd = ft_cmd_finder(node, shell);
				/* if (!full_cmd && (!ft_in(node) && !ft_out(node)))
					{
						g_status = 2;
						ft_putstr_fd("minishell: syntax error near unexpected token `|'\n", shell->stdout_fd);
						break ;
					} */
				if (!full_cmd)
					break ;
				pid = fork();
				if (pid == 0)
				{
					execve(full_cmd, node->full_cmd, shell->envp);
					//for (int i = 0; i < 10; i++)
						//printf("OIIIIIIIIIIIII\n");
					exit(0);
				}
				else
				{
					signal(SIGINT, ft_set_gstatus);
					signal(SIGQUIT, SIG_IGN);
					if (cmd->next)
						close(fd[i][1]);
					waitpid(pid, &status, 0);
					if (g_status == 130)
					{
						ft_gest_sig_bash();
						return ;
					}
					if (WIFEXITED(status))
						g_status = WEXITSTATUS(status);
					ft_gest_sig_bash();
					free(full_cmd);
				}
			}
		}
		ft_close_redirection((t_node *)cmd->content);
		cmd = cmd->next;
		i++;
	}
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
	ft_reset_redirection(shell);
}