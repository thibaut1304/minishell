/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thhusser <thhusser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/19 19:01:28 by thhusser          #+#    #+#             */
/*   Updated: 2022/01/20 17:08:02 by thhusser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	clean_line(t_ms *g)
{
	int		i;
	char	*dest;

	i = 0;
	while (g->line[i] && g->line[i] == ' ')
		i++;
	dest = ft_strdup(g->line + i);
	ft_del_line(g->line);
	g->line = dest;
}

int	check_nb_pipe(const char *str, t_ms *g)
{
	int	i;
	int	nb;

	i = 0;
	nb = 0;
	while (str[i])
	{
		if (str[i] == '\'' || str[i] == '"')
		{
			i = parseur_quotes(g, i + 1, str[i]);
			if (i == -1)
				break ;
		}
		if (str[i] == '|')
			nb++;
		if (str[i] == '\\')
			i++;
		i++;
	}
	return (nb);
}

int	clean_command(t_ms *g)
{
	int		pipe;
	char	*command;

	command = NULL;
	pipe = 0;
	if (parseur(g, -1, 0))
		return (1);
	clean_line(g);
	if (g->line)
	{
		command = check_in_out(g, g->line);
		pipe = check_nb_pipe(command, g);
		if (pipe)
			pipe_command(g, pipe);
		else if (!find_cmd_path(command, g))
		{
			norm_clean_cmd(g, command);
			free_split(g->cmd_tab);
		}
		ft_del_line(command);
		ft_del_line(g->ret_dir);
	}
	return (0);
}
