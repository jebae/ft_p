#include "client.h"

static int	g_sockfd;

static void	clear_arr(char **arr)
{
	int		i;

	if (arr == NULL)
		return ;
	i = 0;
	while (arr[i])
	{
		ft_memdel((void **)&arr[i]);
		i++;
	}
	ft_memdel((void **)&arr);
}

static int	handle_cmd(int sockfd, char **args)
{
	if (ft_strcmp(args[0], "ls") == 0)
		return (handle_ls(sockfd, args));
	else if (ft_strcmp(args[0], "cd") == 0)
		return (handle_cd(sockfd, args));
	else if (ft_strcmp(args[0], "pwd") == 0)
		return (handle_pwd(sockfd));
	else if (ft_strcmp(args[0], "get") == 0)
		return (handle_get(sockfd, args));
	else if (ft_strcmp(args[0], "lls") == 0)
		return (handle_lls(args));
	else if (ft_strcmp(args[0], "put") == 0)
		return (handle_put(sockfd, args));
	else if (ft_strcmp(args[0], "del") == 0)
		return (handle_del(sockfd, args));
	else if (ft_strcmp(args[0], "mkdir") == 0)
		return (handle_mkdir(sockfd, args));
	else if (ft_strcmp(args[0], "rmdir") == 0)
		return (handle_rmdir(sockfd, args));
	else if (ft_strcmp(args[0], "quit") == 0)
		return (handle_quit(sockfd));
	return (0);
}

static void	handle_signal_quit(int sig)
{
	(void)sig;
	handle_quit(g_sockfd);
	exit(0);
}

int			run_console(int sockfd)
{
	char	*input;
	char	**args;
	int		res;

	g_sockfd = sockfd;
	signal(SIGINT, handle_signal_quit);
	res = 0;
	while (res == 0)
	{
		printf("$> ");
		fflush(stdout);
		if (get_next_line(0, &input) == -1)
			return (-1);
		args = ft_strsplit(input, ' ');
		if (args == NULL)
			printf("failed to parse args\n");
		else if (args[0] != NULL)
			res = handle_cmd(sockfd, args);
		ft_memdel((void **)&input);
		clear_arr(args);
	}
	return (res);
}