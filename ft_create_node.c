/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_create_node.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcolucci <dcolucci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/18 13:00:19 by dcolucci          #+#    #+#             */
/*   Updated: 2023/06/06 13:08:56 by dcolucci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
extern int g_status;

int	ft_infile(char **sub_cmd, t_node *node)
{
	int fd_k;
	int x;
	int fd;

	x = 0;
	fd = 0;
	node->str_infile = 0;
	while (sub_cmd[x])
	{
		if (in_set(sub_cmd[x][0], "<") && ft_strlen(sub_cmd[x]) == 1)
		{
			if(sub_cmd[x + 1][0] == '<')
			{
				if(fd != 0)
					close(fd);
				fd = ft_heredoc(sub_cmd[x + 2]);
				x = x + 2;
			}
			else
			{
				fd_k = open(sub_cmd[x + 1], O_RDONLY);
				if (fd_k == -1)
				{
					if (node->str_infile)
						free(node->str_infile);
					node->str_infile = ft_strdup(sub_cmd[x + 1]);
					return (-1);
				}
				else
				{
					if(fd != 0)
						close(fd);
					fd = fd_k;
					if (node->str_infile)
						free(node->str_infile);
					node->str_infile = ft_strdup(sub_cmd[x + 1]);
				}
				x = x + 1;
			}
		}
		else
			x++;
	}
	return (fd);
}

int	ft_outfile(char **sub_cmd, t_node *node)
{
	//sadsad
	int fd;
	int x;
	int fd_k;

	x = 0;
	fd = 1;
	node->str_outfile = 0;
	while (sub_cmd[x])
	{
		if (sub_cmd[x][0] == '>' && ft_strlen(sub_cmd[x]) == 1)
		{
			if (sub_cmd[x + 1][0] == '>')
			{
				fd_k = open(sub_cmd[x + 2], O_WRONLY | O_APPEND | O_CREAT, S_IRWXU);
				if (fd_k == -1)
				{
					if (node->str_outfile)
						free(node->str_outfile);
					node->str_outfile = ft_strdup(sub_cmd[x + 2]);
					return (-1);
				}
				else
				{
					if(fd != 1)
						close(fd);
					fd = fd_k;
					if (node->str_outfile)
						free(node->str_outfile);
					node->str_outfile = ft_strdup(sub_cmd[x + 2]);
				}
				x = x + 2;
			}
			else
			{
				fd_k = open(sub_cmd[x + 1], O_WRONLY | O_CREAT | O_TRUNC, S_IRWXU);
				if (fd_k == -1)
				{
					if (node->str_outfile)
						free(node->str_outfile);
					node->str_outfile = ft_strdup(sub_cmd[x + 1]);
					return (-1);
				}
				else
				{
					if(fd != 1)
						close(fd);
					fd = fd_k;
					if (node->str_outfile)
						free(node->str_outfile);
					node->str_outfile = ft_strdup(sub_cmd[x + 1]);
				}
				x = x + 1;
			}
		}
		else
			x++;
	}
	return (fd);
}

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

char	*ft_cmd(char **sub_cmd)
{
	int		i;
	char	*cmd;

	i = 0;
	cmd = 0;
	while (sub_cmd[i])
	{
		if (!in_set(sub_cmd[i][0], "<>|"))
		{
			cmd = (ft_strdup(sub_cmd[i]));
			return (cmd);
		}
		else if (in_set(sub_cmd[i][0], "<>|") && ft_strlen(sub_cmd[i]) != 1)
		{
			cmd = (ft_strdup(sub_cmd[i]));
			return (cmd);
		}
		else
		{
			while (sub_cmd[i] && in_set(sub_cmd[i][0], "<>"))
				i++;
			if (!sub_cmd[i])
				break ;
			else
				i++;
		}
	}
	cmd = ft_restore_neg(cmd);
	return (cmd);
}

char	**ft_full_cmd(char **sub_cmd)
{
	int		i;
	char	**full_cmd;
	char	**tmp;

	i = 0;
	full_cmd = 0;
	while (sub_cmd[i])
	{
		if (!in_set(sub_cmd[i][0], "<>|"))
		{
			tmp = full_cmd;
			full_cmd = ft_add_to_split(full_cmd, sub_cmd[i]);
			free_arrarr(tmp);
			i++;
		}
		else if (in_set(sub_cmd[i][0], "<>|") && ft_strlen(sub_cmd[i]) != 1)
		{
			tmp = full_cmd;
			full_cmd = ft_add_to_split(full_cmd, sub_cmd[i]);
			free_arrarr(tmp);
			i++;
		}
		else
		{
			while (sub_cmd[i] && in_set(sub_cmd[i][0], "<>"))
				i++;
			if (!sub_cmd[i])
				break ;
			else
				i++;
		}
	}
	i = 0;
	if (full_cmd)
	{
		while(full_cmd[i])
		{
			full_cmd[i] = ft_restore_neg(full_cmd[i]);
			i++;
		}
	}
	return (full_cmd);
}