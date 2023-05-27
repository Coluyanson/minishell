#include "minishell.h"

int	ft_check_export(char *str, char *env)
{
	int	x;

	x = 0;
	while (env[x] && env[x] != '=')
		x++;
	return (ft_strncmp(str, env, x));
}

char *ft_truncate_eq(char *str)
{
	int		x;
	char	*trun;

	if (!str)
		return (0);
	if (!ft_strchr(str, '='))
		return (0);
	x = 0;
	while (str[x] && str[x] != '=')
		x++;
	trun = (char *) malloc (sizeof(char) * (x + 1));
	ft_strlcpy(trun, str, x + 1);
	return (trun);
}