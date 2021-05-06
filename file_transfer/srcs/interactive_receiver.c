#include "file_transfer.h"

void	interactive_receive_chunk(int sockfd,
	t_uint32 block_size, t_uint32 block_count)
{
	t_uint8			buf[256];
	t_transfer_hdr	*hdr;
	t_transfer_hdr	ack;
	t_uint64		filesize;

	hdr = (t_transfer_hdr *)buf;

	// receive SYN
	if (recv(sockfd, buf, 256, 0) == -1)
	{
		printf("failed to receive SYN\n");
		return ;
	}
	filesize = hdr->filesize;
	printf("SYN received: %s [%lluB]\n",
		buf + sizeof(t_transfer_hdr), filesize);


	// send SYNACK
	ack.ack = 0;
	ack.filesize = block_size;
	ack.rcwd = block_count;
	if (send(sockfd, &ack, sizeof(ack), 0) == -1)
	{
		printf("failed to send SYNACK\n");
		return ;
	}
	printf("SYNACK sent\n");

	// receive chunks
	t_uint32	expected = 0;

	while (expected * block_size < filesize)
	{
		if (recv(sockfd, buf, sizeof(t_transfer_hdr) + block_size, 0) == -1)
		{
			printf("failed to receive chunk\n");
			return ;
		}

		// send ACK
		char	*line;
		int		i = 0;

		printf("[seq %u] insert ack and rcwd: ", hdr->seq);
		fflush(stdout);
		get_next_line(0, &line);
		ack.ack = (t_uint32)atoi(line);
		while (line[i] != ' ')
			i++;
		ack.rcwd = (t_uint32)atoi(&line[i + 1]);
		ft_memdel((void **)&line);

		if (send(sockfd, &ack, sizeof(ack), 0) == -1)
		{
			printf("failed to send ACK\n");
			return ;
		}
	}
}

int		main(void)
{
	const char			*ip = "127.0.0.1";
	short				port = 4321;
	int					sockfd, accfd, e;
	struct sockaddr_in	server_addr;
	struct sockaddr_in	cli_addr;
	socklen_t			addr_size;
	t_uint32			block_size;
	t_uint32			block_count;
	char				*line;

	// set block size and count
	printf("block size: ");
	fflush(stdout);
	get_next_line(0, &line);
	block_size = (t_uint32)atoi(line);
	ft_memdel((void **)&line);

	printf("block count: ");
	fflush(stdout);
	get_next_line(0, &line);
	block_count = (t_uint32)atoi(line);
	ft_memdel((void **)&line);

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
		interactive_receive_chunk(accfd, block_size, block_count);
		printf("receive file complete\n");
	}
}