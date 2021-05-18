#include "file_transfer.h"

#define MIN(a, b) ((a) < (b)) ? (a) : (b)

void	interactive_send_chunk(int sockfd)
{
	t_transfer_hdr	syn;
	t_transfer_hdr	ack;
	t_transfer_hdr	*hdr;
	t_uint32		base;
	t_uint32		next;
	t_uint32		chunk_size;
	t_uint32		max_rcwd;
	t_uint8			*buf;
	t_uint8			*chunk;
	t_uint64		filesize = 3200;

	// send SYN
	syn.size = filesize;
	if (send(sockfd, &syn, sizeof(t_transfer_hdr), 0) == -1)
	{
		printf("failed to send SYN\n");
		return ;
	}

	// receive SYNACK
	if (recv(sockfd, &ack, sizeof(t_transfer_hdr), 0) == -1)
	{
		printf("failed to receive SYNACK");
		return ;
	}
	chunk_size = ack.size;
	max_rcwd = ack.rcwd;
	buf = malloc(chunk_size + sizeof(t_transfer_hdr));
	hdr = (t_transfer_hdr *)buf;
	chunk = buf + sizeof(t_transfer_hdr);

	// send chunks
	char		*line;
	t_uint32	endseq = filesize / chunk_size;

	if (filesize % chunk_size > 0)
		endseq++;

	int		*memo = (int *)ft_memalloc(sizeof(int) * (endseq + 1));

	base = 0;
	while (base * chunk_size < filesize)
	{
		printf("seq to send [");
		t_uint32 until = MIN(base + max_rcwd, endseq);

		for (t_uint32 i=base; i < until; i++)
		{
			if (!memo[i])
				printf("%u ", i);
		}
		printf("]: ");
		fflush(stdout);

		get_next_line(0, &line);

		hdr->seq = (t_uint32)atoi(line);
		hdr->size = chunk_size;
		for (t_uint32 i=0; i < chunk_size / 4; i++)
		{
			*((t_uint32 *)chunk + i) = hdr->seq;
		}
		ft_memdel((void **)&line);

		// send chunk
		if (send(sockfd, buf, sizeof(t_transfer_hdr) + chunk_size, 0) == -1)
		{
			printf("failed to send chunk\n");
			break ;
		}
		memo[hdr->seq] = 1;

		// receive ACK
		if (recv(sockfd, &ack, sizeof(t_transfer_hdr), 0) == -1)
		{
			printf("failed to receive ACK\n");
			break ;
		}

		base = (base < ack.ack) ? ack.ack : base;
	}
	free(buf);
	free(memo);
}

int		main(void)
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

	interactive_send_chunk(sockfd);
	close(sockfd);
}