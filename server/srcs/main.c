#include "server.h"

int		main(int argc, char *argv[])
{
	(void)argc;
	const char			*ip = "127.0.0.1";
	short				port = ft_atoi(argv[1]);
	int					sockfd, accfd, e;
	struct sockaddr_in	server_addr;
	struct sockaddr_in	cli_addr;
	socklen_t			addr_size;

	// create socket
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1)
	{
		printf("socket failed\n");
		return 1;
	}
	printf("listening socket created\n");

	// set server_addr
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = port;
	server_addr.sin_addr.s_addr = inet_addr(ip);

	// bind
	e = bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr));
	if (e == -1)
	{
		printf("bind failed\n");
		return 1;
	}
	printf("bind success\n");

	// listen
	if (listen(sockfd, 10) == -1)
	{
		printf("listen failed\n");
		close(sockfd);
		return 1;
	}
	printf("listening...\n");


	t_hdr	hdr;
	int		pid;
	int		res;
	char	*cwd = malloc(1024);

	ft_strcpy(cwd, "/");

	while (1)
	{
		addr_size = sizeof(cli_addr);
		accfd = accept(sockfd, (struct sockaddr *)&cli_addr, &addr_size);
		if (accfd == -1)
		{
			printf("accept failed\n");
			return 1;
		}

		pid = fork();

		if (pid == 0)
		{
			printf("client accpet: %s:%d\n", inet_ntoa(cli_addr.sin_addr), cli_addr.sin_port);

			// receive message
			while (1)
			{
				if (recv(accfd, &hdr, sizeof(t_hdr), 0) == -1)
				{
					printf("failed to receive message\n");
					return (-1);
				}
				res = cmd_route(accfd, &cli_addr, &hdr, (char **)&cwd);
			}
		}
		else
			waitpid(pid, NULL, 0);
	}
}