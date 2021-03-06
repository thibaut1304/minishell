/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   variables_1.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thhusser <thhusser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/14 20:29:50 by thhusser          #+#    #+#             */
/*   Updated: 2022/01/19 16:25:05 by thhusser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*my_getenv(t_ms *g, char *path)
{
	int		i;
	char	*usable;
	t_list	*list;

	list = g->env;
	usable = NULL;
	i = 0;
	while (list)
	{
		if (!ft_strncmp(list->content, path, ft_strlen(path)))
		{
			usable = &list->content[ft_strlen(path)];
			break ;
		}
		list = list->next;
	}
	return (usable);
}

int	takepath(char *str, int idx, char *tmp, t_ms *g)
{
	int		i;
	char	*str_tmp;
	char	*tenv;

	i = 0;
	str_tmp = (char *)ft_calloc(sizeof(char), (ft_strlen(str) + 1));
	if (!str_tmp)
		return (-1);
	while (str[idx] && str[idx] != '$' && str[idx] != ' '
		&& (ft_isalnum(str[idx]) || str[idx] == '_'))
	{
		str_tmp[i] = str[idx];
		idx++;
		i++;
	}
	str_tmp[i] = '=';
	str_tmp[i + 1] = '\0';
	tenv = my_getenv(g, str_tmp);
	if (tenv != NULL)
		ft_strcat(tmp, tenv);
	tenv = NULL;
	free(str_tmp);
	str_tmp = NULL;
	return (idx);
}

char	*get_str_var(char *str, char *tmp, int idx, int sp)
{
	while (str[sp] == ' ' && str[sp])
		sp++;
	if (str[sp] != '\0')
		ft_strcat(tmp, &str[idx]);
	str = (char *)ft_calloc(sizeof(char), (ft_strlen(tmp) + 1));
	if (!str)
		return (NULL);
	ft_strcpy(str, tmp);
	free(tmp);
	return (str);
}

char	*variables_in_cmd(t_ms *g, char *cmd, int idx)
{
	char	*cmd_tmp;
	char	*tenv;

	cmd_tmp = (char *)ft_calloc(sizeof(char),
			(ft_strlen(cmd) + PATH_MAX_ENV + 1));
	if (!cmd_tmp)
		return (NULL);
	if (idx != 0)
		ft_strlcpy(cmd_tmp, cmd, idx + 1);
	idx++;
	if (cmd[idx] != '?')
		idx = takepath(cmd, idx, cmd_tmp, g);
	else
	{
		tenv = ft_itoa(g->ret_errno);
		ft_strcat(cmd_tmp, tenv);
		free(tenv);
		idx++;
	}
	cmd = get_str_var(cmd, cmd_tmp, idx, idx);
	if (cmd == NULL)
		return (NULL);
	return (cmd);
}

char	*check_var_cmd(t_ms *g, char *cmd)
{
	int		i;
	int		x;

	x = 0;
	i = 0;
	while (cmd[i])
	{
		if (cmd[i] == '"' && x)
			x = 0;
		else if (cmd[i] == '"')
			x = 1;
		if (cmd[i] == '\\' && cmd[i + 1] != '\0')
			i += 2;
		if (cmd[i] == '\'' && !x)
			i = parseur_quotes_str(cmd, i + 1, '\'');
		if (checkvar(cmd, i))
		{
			cmd = variables_in_cmd(g, cmd, i);
			if (!cmd)
				return (NULL);
		}
		i++;
	}
	return (cmd);
}
