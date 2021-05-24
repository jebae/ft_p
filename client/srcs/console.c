#include "client.h"

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
	return (0);
}

int			run_console(int sockfd)
{
	char	*input;
	char	**args;
	int		res;

	res = 0;
	while (res != -1)
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
	return (1);
}