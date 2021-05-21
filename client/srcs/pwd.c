#include "client.h"

static int	send_pwd(int sockfd)
{
	t_pwd_hdr	hdr;

	hdr.cmd = CMD_PWD;
	hdr.size = 0;
	if (send(sockfd, &hdr, sizeof(t_pwd_hdr), 0) == -1)
		return (-1);
	return (0);
}

static int	receive_ack(int sockfd)
{
	t_hdr	hdr;
	char	*payload;

	if (recv(sockfd, &hdr, sizeof(t_hdr), 0) == -1)
		return (-1);
	if (hdr.cmd == CMD_ERR)
		return (handle_err_ack(sockfd, &hdr));
	if (read_payload(sockfd, &hdr, (t_uint8 **)&payload) == -1)
		return (-1);
	printf("%s\n", payload);
	ft_memdel((void **)&payload);
	return (0);
}

int			handle_pwd(int sockfd)
{
	if (send_pwd(sockfd) == -1)
		return (-1);
	if (receive_ack(sockfd) == -1)
		return (-1);
	return (0);
}