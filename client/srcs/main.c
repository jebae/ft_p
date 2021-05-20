#include "client.h"

int		main(int argc, char **argv)
{
	(void)argc;
	const char			*ip = "127.0.0.1";
	short				port = ft_atoi(argv[1]);
	int					sockfd, e;
	struct sockaddr_in	server_addr;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);

	server_addr.sin_family = AF_INET;
	server_addr.sin_port = port;
	server_addr.sin_addr.s_addr = inet_addr(ip);

	e = connect(sockfd, (struct sockaddr *)&server_addr, sizeof(struct sockaddr_in));

	if (e == -1)
	{
		printf("connect failed\n");
		return 1;
	}
	printf("connect success\n");

	run_console(sockfd);
	close(sockfd);
}