#include "client.h"

static int	err_closing_socket(const char *msg, int sockfd)
{
	close(sockfd);
	printf("%s\n", msg);
	return (-1);
}

int			connect_server(t_uint16 port)
{
	static const char	*ip = "127.0.0.1";
	struct sockaddr_in	server_addr;
	int					sockfd;
	int					res;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1)
	{
		printf("socket failed\n");
		return (-1);
	}
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(port);
	server_addr.sin_addr.s_addr = inet_addr(ip);
	if (set_recvtimeout(sockfd, RCV_TIMEOUT) == -1)
		return (err_closing_socket("set receive timtout failed", sockfd));
	if (connect(sockfd,
		(struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
		return (err_closing_socket("connect failed", sockfd));
	printf("connect success\n");
	res = run_console(sockfd);
	if (res == -1)
		printf("[ERROR]\n");
	close(sockfd);
	return (res);
}