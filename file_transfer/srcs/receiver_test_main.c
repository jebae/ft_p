#include "file_transfer.h"

int	main()
{
	const char			*ip = "127.0.0.1";
	short				port = 4321;
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


	t_uint8			buf[256];
	t_transfer_hdr	*hdr;

	hdr = (t_transfer_hdr *)buf;
	while (1)
	{
		addr_size = sizeof(cli_addr);
		accfd = accept(sockfd, (struct sockaddr *)&cli_addr, &addr_size);
		if (accfd == -1)
		{
			printf("accept failed\n");
			return 1;
		}

		printf("client accpet: %s:%d\n", inet_ntoa(cli_addr.sin_addr), cli_addr.sin_port);
		if (recv(accfd, buf, 256, 0) == -1)
		{
			printf("failed to receive SYN\n");
			return (-1);
		}
		receive_file(accfd, hdr->filesize, "./received.txt");
		printf("receive file complete\n");
	}
}