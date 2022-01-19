#include "../includes/minishell.h"

void print_2Dtab(char** tab, char *str)
{
	int i = 0;
	while(tab[i])
    	{
		printf("%s[%d]: %s\n", str, i, tab[i]);
		i++;
    	}
}

char *get_pwd()
{
	char	buf[1024];
	char	*cwd;

	cwd = getcwd(buf, sizeof(buf));
	return (cwd);
}

int	get_argc_redir(char **tab)
{
	int	i;
	int	argc;

	i = 0;
	argc = 1;
	while (tab[i] && tab[i+1])
	{
		if (ft_strequ(tab[i+1], ">") || ft_strequ(tab[i+1], ">>"))
		{
			//TODO: other descriptor on the left
			if (ft_strequ(tab[i], "1") || ft_strequ(tab[i], "2") || ft_strequ(tab[i], "&"))
				argc = i;
			else
				argc = i + 1;
			break;
		}
		else if (ft_strequ(tab[i+1], "<") || ft_strequ(tab[i+1], "<<"))
		{
			argc = i + 1;
			break;
		}
		i++;
		argc++;
	}
	return (argc);
}

char **get_argv_redir(char *cmd)
{
	char			**tab;
	char			**argv;
	int	argc;
	int	i;

	tab = creat_list_arg(cmd);
	argc = get_argc_redir(tab);
	argv = (char **)malloc(sizeof(char *) * (argc + 1));
	argv[argc] = NULL;
	i = 0;
	while (i < argc)
	{
		argv[i] = ft_strdup(tab[i]);
		i++;
	}
	free_split(tab);
	return (argv);
}

char **get_env_tab(t_list *env)
{
	t_list *l;
	char **ret;
	int i;

	ret = (char **)malloc(sizeof(char *) * (ft_lstsize(env) + 1));
	i = 0;
	l = env;
	while (l)
	{
		ret[i++] = ft_strdup((char*)(l->content));
		l = l->next;
	}
	ret[i] = NULL;
	return (ret);
}

int handle_redir_in(int *fd, char **tab, int i)
{
	if (*fd > 0)
		close(*fd);
	*fd = open(tab[i + 1], O_RDONLY);
	if (*fd < 0)
	{
		error_out2(NULL, tab[i + 1], "No such file or directory");
		free_split(tab);
		g_ms->ret_errno = 1;
		return (-1);
	}
	//printf("redir < :fd: %d\n", fd);
	return (0);
}

void handle_redir_in_in(int *fd, char **tab, int i)
{
	char *delimitor;
	char *s;
	
	if (*fd > 0)
		close(*fd);
	delimitor = tab[i + 2];
	*fd = open("redir_lessless", O_CREAT | O_WRONLY | O_APPEND | O_TRUNC, 0644);
	while (1)
	{
		s = readline("> ");
		if (ft_strequ(s, delimitor))
			break;
		ft_putstr_fd(s, *fd);
		ft_putstr_fd("\n",*fd);
	}
	close(*fd);
	*fd = open("redir_lessless", O_RDONLY);//TODO:unlink file
	//printf("redir << :fd: %d\n", fd);
}

void handle_redir_out(int *fd, char **tab, int i, int *is_double, char **redir_file)
{
	if (ft_strequ(tab[i], ">"))
	{
		*fd = open(tab[i + 1], O_WRONLY | O_CREAT | O_TRUNC, 0664);
		close(*fd);
		if (*redir_file)
			free(*redir_file);
		*redir_file = ft_strdup(tab[i + 1]);
	}
	else if (ft_strequ(tab[i], ">>"))
	{
		*fd = open(tab[i + 1], O_WRONLY | O_CREAT | O_APPEND, 0664);
		close(*fd);
		if (*redir_file)
			free(*redir_file);
		*redir_file = ft_strdup(tab[i + 1]);
		*is_double = 1;
	}
}

int get_redir_in_file(char *cmd)
{
	char  **tab;
	int i;
	int fd;

	fd = 0;
	tab = creat_list_arg(cmd);
	i = 0;
	while (tab[i] && tab[i + 1])
	{
        	if (ft_strequ(tab[i], "<") && !ft_strequ(tab[i+1], "<"))
		{
			if (handle_redir_in(&fd, tab, i) == -1)
				return (-1);
		}
        	else if (ft_strequ(tab[i], "<") && ft_strequ(tab[i+1], "<") && tab[i + 2])
		{
			handle_redir_in_in(&fd, tab, i);
			i++;
		}
		i++;
	}
	free_split(tab);
	return (fd);
}
int get_redir_out_file(char *cmd)
{
	char  **tab;
	int i;
	int fd;
	int is_double;
	char *redir_file;

	redir_file = NULL;
	is_double = 0;
	tab = creat_list_arg(cmd);
	i = 0;
	while (tab[i] && tab[i + 1])
	{
		handle_redir_out(&fd, tab, i, &is_double, &redir_file);
		i++;
	}
	free_split(tab);
	if (!redir_file)
		return (0);
	if (is_double == 0)
		fd = open(redir_file, O_WRONLY | O_CREAT | O_TRUNC, 0664);
	else
		fd = open(redir_file, O_WRONLY | O_CREAT | O_APPEND, 0664);
	free(redir_file);
	return (fd);
}

