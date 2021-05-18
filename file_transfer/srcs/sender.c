#include "file_transfer.h"

static int	safe_return(t_uint8 *buf, int fd, int res, const char *msg)
{
	if (buf != NULL)
		ft_memdel((void **)&buf);
	if (fd != -1)
		close(fd);
	if (msg != NULL)
		print_log(msg);
	return (res);
}

static int	send_syn(
	int sockfd, t_transfer_hdr *hdr, char *filename, t_uint64 filesize)
{
	t_uint8		*buf;
	size_t		size;
	int			res;

	size = sizeof(t_transfer_hdr) + ft_strlen(filename) + 1;
	buf = (t_uint8 *)ft_memalloc(size);
	if (buf == NULL)
		return (-1);
	ft_memcpy(buf, hdr, sizeof(t_transfer_hdr));
	hdr = (t_transfer_hdr *)buf;
	hdr->size = filesize;
	ft_strcpy((char *)buf + sizeof(t_transfer_hdr), filename);
	res = 0;
	if (send(sockfd, buf, size, 0) == -1)
		res = -1;
	ft_memdel((void **)&buf);
	return (res);
}

static int	send_chunk(
	t_uint8 *buf, t_uint32 chunk_size, t_uint32 seq, int fd[2])
{
	t_transfer_hdr	*hdr;
	int				r;

	hdr = (t_transfer_hdr *)buf;
	r = read(fd[1], buf + sizeof(t_transfer_hdr), chunk_size);
	if (r == -1)
		return (-1);
	hdr->size = r;
	hdr->seq = seq;
	if (send(fd[0], buf, sizeof(t_transfer_hdr) + chunk_size, 0) == -1)
		return (-1);
	return (0);
}

static int	send_pipelined(
	int sockfd, t_transfer_hdr *ack, t_uint64 filesize, int fd)
{
	t_uint32		base;
	t_uint32		next;
	t_uint64		chunk_size;
	t_uint8			*buf;

	base = ack->ack;
	next = base;
	chunk_size = ack->size;
	buf = (t_uint8 *)ft_memalloc(sizeof(t_transfer_hdr) + chunk_size);
	if (buf == NULL)
		return (-1);
	while (base * chunk_size < filesize)
	{
		while (next - base < ack->rcwd && next * chunk_size < filesize)
		{
			if (send_chunk(buf, chunk_size, next, (int[2]){sockfd, fd}) == -1)
				return (safe_return(buf, -1, -1, "failed to send chunk"));
			log_transfer_percent(next, next * chunk_size, filesize);
			next++;
		}
		if (recv(sockfd, ack, sizeof(t_transfer_hdr), 0) == -1)
			return (safe_return(buf, -1, -1, "failed to receive ACK"));
		base = (base < ack->ack) ? ack->ack : base;
	}
	return (safe_return(buf, -1, 0, NULL));
}

int			send_file(int sockfd, t_transfer_hdr *hdr, char *filepath)
{
	int				fd;
	char			*filename;
	t_transfer_hdr	ack;
	struct stat		st;

	if ((fd = open(filepath, O_RDONLY)) == -1)
		return (safe_return(NULL, -1, -1, "failed to open file"));
	if (fstat(fd, &st) == -1)
	{
		close(fd);
		return (-1);
	}
	filename = extract_filename(filepath);
	if (send_syn(sockfd, hdr, filename, st.st_size) == -1)
		return (safe_return(NULL, fd, -1, "failed to send SYN"));
	if (recv(sockfd, &ack, sizeof(t_transfer_hdr), 0) == -1)
		return (safe_return(NULL, fd, -1, "failed to receive SYNACK"));
	if (send_pipelined(sockfd, &ack, st.st_size, fd) == -1)
		return (safe_return(NULL, fd, -1, NULL));
	return (safe_return(NULL, fd, 0, "success to send file"));
}