#include "server.h"

static int	err_closing_socket(const char *msg, int sockfd)
{
	close(sockfd);
	printf("%s\n", msg);
	return (-1);
}

static int	create_socket(t_uint16 port)
{
	static const char	*ip = "127.0.0.1";
	int					sockfd;
	struct sockaddr_in	server_addr;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1)
	{
		printf("socket failed\n");
		return (-1);
	}
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = port;
	server_addr.sin_addr.s_addr = inet_addr(ip);
	if (bind(sockfd,
		(struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
		return (err_closing_socket("bind failed", sockfd));
	if (listen(sockfd, 1) == -1)
		return (err_closing_socket("listen failed", sockfd));
	printf("listening %hd\n", port);
	return (sockfd);
}

static void	listen_client(int sockfd, struct sockaddr_in *cli)
{
	int		pid;
	int		res;
	t_hdr	hdr;
	char	*cwd;

	pid = fork();
	if (pid != 0)
		return ;
	if ((cwd = ft_strdup("/")) == NULL)
	{
		err_closing_socket("cwd allocation failed", sockfd);
		exit(EXIT_SUCCESS);
	}
	res = set_recvtimeout(sockfd, RCV_TIMEOUT);
	while (res == 0)
	{
		res = recv(sockfd, &hdr, sizeof(t_hdr), 0) == -1
			? -1 : cmd_route(sockfd, cli, &hdr, &cwd);
	}
	if (res == -1)
		printf("[ERROR] %s:%d\n", inet_ntoa(cli->sin_addr), cli->sin_port);
	close(sockfd);
	ft_memdel((void **)&cwd);
	exit(EXIT_SUCCESS);
}

int			run_server(t_uint16 port)
{
	int					sockfd;
	struct sockaddr_in	cli_addr;
	socklen_t			addr_size;
	int					accfd;

	sockfd = create_socket(port);
	if (sockfd == -1)
		return (-1);
	addr_size = sizeof(cli_addr);
	while (1)
	{
		accfd = accept(sockfd, (struct sockaddr *)&cli_addr, &addr_size);
		if (accfd == -1)
			printf("accept failed\n");
		else
		{
			printf("client accept: %s:%d\n",
				inet_ntoa(cli_addr.sin_addr), cli_addr.sin_port);
			listen_client(accfd, &cli_addr);
		}
	}
	return (0);
}