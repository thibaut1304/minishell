/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   new_double_cmd.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thhusser <thhusser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/13 22:21:29 by thhusser          #+#    #+#             */
/*   Updated: 2022/01/19 02:11:49 by thhusser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*get_cmd_quote(char *cmd, char *line, int **i, int **k)
{
	int	c;

	c = line[**i];
	**i += 1;
	while (line[**i] != c && line[**i])
	{
		if (line[**i] == '$' && ft_isdigit(line[**i + 1]))
			**i += 2;
		if ((line[**i] == '\\' && c == '"') && (line[**i + 1] == '\\'
				|| line[**i + 1] == '"' || line[**i + 1] == '$'))
			**i += 1;
		cmd[**k] = line[**i];
		**i += 1;
		**k += 1;
	}
	return (cmd);
}

char	*get_cmd_creat(char *cmd, char *line, int *i, int *k)
{
	int		c;

	c = '\0';
	if (line[*i] == '$' && ft_isdigit(line[*i + 1]))
		*i += 2;
	if (line[*i] == '\'' || line[*i] == '"')
	{
		c = line[*i];
		cmd = get_cmd_quote(cmd, line, &i, &k);
	}
	if (line[*i] == '\\' && line[*i + 1] != '<' && line[*i + 1] != '>')
		*i += 1;
	if (line[*i] != c)
	{
		cmd[*k] = line[*i];
		*k += 1;
	}
	return (cmd);
}

char	**creat_list_arg2(char *line, char **cmd, int i, int j)
{
	int		k;

	k = 0;
	while (line[i])
	{
		if (line[i] == ' ')
		{
			cmd[j][k] = '\0';
			k = 0;
			j += 1;
			cmd[j] = NULL;
			while (line[i] == ' ')
				i++;
			if (line[i] == '\0')
				break ;
			cmd[j] = (char *)ft_calloc(sizeof(char), (ft_strlen(&line[i]) + 1));
			if (!cmd[j])
				return (NULL);
		}
		cmd[j] = get_cmd_creat(cmd[j], line, &i, &k);
		i++;
	}
	cmd = cmd_creatnull(cmd, j, k);
	return (cmd);
}

char	**creat_list_arg(char *line)
{
	char	**command;

	command = (char **)malloc(sizeof(char *) * (count_word(line) + 1));
	if (!command)
		return (NULL);
	command[0] = (char *)ft_calloc(sizeof(char), (ft_strlen(line) + 1));
	if (!command)
		return (NULL);
	command = creat_list_arg2(line, command, 0, 0);
	return (command);
}
