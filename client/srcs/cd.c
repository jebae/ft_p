#include "client.h"

static int	send_cd_root(int sockfd)
{
	t_cd_hdr	hdr;

	hdr.cmd = CMD_CD;
	hdr.size = 0;
	if (send(sockfd, &hdr, sizeof(t_cd_hdr), 0) == -1)
		return (-1);
	return (0);
}

static int	receive_ack(int sockfd)
{
	t_hdr		hdr;

	if (recv(sockfd, &hdr, sizeof(t_hdr), 0) == -1)
		return (-1);
	if (hdr.cmd == CMD_ERR)
		return (handle_err_ack(sockfd, &hdr));
	return (0);
}

int			handle_cd(int sockfd, char **args)
{
	int		send_res;

	send_res = (args[1] == NULL)
		? send_cd_root(sockfd)
		: send_payload_only_cmd(
			sockfd, CMD_CD, args[1], ft_strlen(args[1]) + 1);
	if (send_res == -1)
		return (-1);
	if (receive_ack(sockfd) == -1)
		return (-1);
	return (0);
}