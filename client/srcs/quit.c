#include "client.h"

static int	send_quit(int sockfd)
{
	t_quit_hdr	hdr;

	hdr.cmd = CMD_QUIT;
	hdr.size = 0;
	if (send(sockfd, &hdr, sizeof(t_quit_hdr), 0) == -1)
		return (-1);
	return (0);
}

int			handle_quit(int sockfd)
{
	if (send_quit(sockfd) == -1)
		return (-1);
	return (1);
}