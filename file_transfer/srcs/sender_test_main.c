#include "file_transfer.h"

int		main(int argc, char **argv)
{
	const char			*ip = "127.0.0.1";
	short				port = 4321;
	int					sockfd, e;
	struct sockaddr_in	server_addr;
	t_transfer_hdr		hdr;

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

	send_file(sockfd, &hdr, argv[1]);
	close(sockfd);
}