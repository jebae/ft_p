#include "file_transfer.h"

static int		send_synack(int sockfd)
{
	t_transfer_hdr	hdr;

	hdr.filesize = CHUNK_SIZE;
	hdr.rcwd = CHUNK_COUNT;
	hdr.ack = 0;
	if (send(sockfd, &hdr, sizeof(t_transfer_hdr), 0) == -1)
		return (-1);
	return (0);
}

static int		send_ack(int sockfd, t_uint32 ack, t_uint32 rcwd)
{
	t_transfer_hdr	ack_hdr;

	ack_hdr.ack = ack;
	ack_hdr.rcwd = rcwd;
	if (send(sockfd, &ack_hdr, sizeof(t_transfer_hdr), 0) == -1)
		return (-1);
	return (0);
}

static int		write_chunks(t_uint8 *buf, t_chunk_buf *chunk_buf,
	t_uint32 *expected, int fd)
{
	t_transfer_hdr	*hdr;
	t_chunk_heap	*heap;
	t_uint8			chunk[CHUNK_SIZE];
	t_uint64		size;

	hdr = (t_transfer_hdr *)buf;
	if (write(fd, buf + sizeof(t_transfer_hdr), hdr->filesize) == -1)
		return (-1);
	*expected += 1;
	heap = &chunk_buf->heap;
	while (heap->len > 0 && heap->arr[0].seq == *expected)
	{
		pop_chunk(chunk, &size, chunk_buf);
		if (write(fd, chunk, size) == -1)
			return (-1);
		*expected += 1;
	}
	return (0);
}

static int		receive_chunks(int sockfd, t_uint64 filesize, int fd)
{
	t_uint32		expected;
	t_chunk_buf		chunk_buf;
	t_uint8			buf[sizeof(t_transfer_hdr) + CHUNK_SIZE];
	t_uint32		buf_size;
	t_transfer_hdr	*hdr;

	hdr = (t_transfer_hdr *)buf;
	init_chunk_buf(&chunk_buf);
	buf_size = sizeof(t_transfer_hdr) + CHUNK_SIZE;
	expected = 0;
	while (expected * CHUNK_SIZE < filesize)
	{
		if (recv(sockfd, buf, buf_size, 0) == -1)
			return (handle_err("failed to send SYNACK"));
		if (hdr->seq > expected)
			push_chunk(hdr->seq, hdr->filesize,
				buf + sizeof(t_transfer_hdr), &chunk_buf);
		else if (hdr->seq == expected)
			if (write_chunks(buf, &chunk_buf, &expected, fd) == -1)
				return (handle_err("failed to write chunks"));
		if (send_ack(sockfd, expected, chunk_buf.stack.top + 1) == -1)
			return (handle_err("failed to send ACK"));
		log_transfer_percent(hdr->seq, expected * CHUNK_SIZE, filesize);
	}
	return (0);
}

int				receive_file(int sockfd, t_uint64 filesize, char *filepath)
{
	int		fd;

	if ((fd = open(filepath, O_WRONLY | O_CREAT, 0644)) == -1)
		return (handle_err("failed to open file"));
	if (send_synack(sockfd) == -1)
	{
		close(fd);
		remove(filepath);
		return (handle_err("failed to send SYNACK"));
	}
	if (receive_chunks(sockfd, filesize, fd) == -1)
	{
		close(fd);
		remove(filepath);
		return (handle_err("failed to receive chunks"));
	}
	close(fd);
	return (0);
}