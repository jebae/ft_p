#include "client.h"

static int	receive_ack(int sockfd)
{
	t_hdr		hdr;

	if (recv(sockfd, &hdr, sizeof(t_hdr), 0) == -1)
		return (-1);
	if (hdr.cmd == CMD_ERR)
		return (handle_err_ack(sockfd, &hdr));
	return (0);
}

int			handle_rmdir(int sockfd, char **args)
{
	int		send_res;

	if (args[1] == NULL)
		return (arg_required());
	send_res = send_payload_only_cmd(
		sockfd, CMD_RMDIR, args[1], ft_strlen(args[1]) + 1);
	if (send_res == -1)
		return (-1);
	if (receive_ack(sockfd) == -1)
		return (-1);
	return (0);
}