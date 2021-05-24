#include "client.h"

static int	arg_required(void)
{
	printf("argument required\n");
	return (0);
}

/**
 * return value
 *  0: GETACK received
 *  1: ERRACK received
 * -1: error
 */
static int	receive_ack(int sockfd, t_transfer_hdr *tf_hdr, char **filepath)
{
	t_hdr	hdr;
	char	*tmp;

	if (recv(sockfd, &hdr, sizeof(t_hdr), 0) == -1)
		return (-1);
	ft_memcpy(tf_hdr, &hdr, sizeof(t_hdr));
	if (hdr.cmd == CMD_ERR)
		return (handle_err_ack(sockfd, &hdr) == -1 ? -1 : 1);
	if (hdr.cmd != CMD_GETACK)
		return (-1);
	if (recv(sockfd, (void *)tf_hdr + sizeof(t_hdr),
		sizeof(t_transfer_hdr) - sizeof(t_hdr), 0) == -1)
		return (-1);
	if (read_payload(sockfd, (t_hdr *)&hdr, (t_uint8 **)filepath) == -1)
		return (-1);
	tmp = *filepath;
	*filepath = strcat_all(3, ROOT_DIR, "/", *filepath);
	if (filepath == NULL)
	{
		ft_memdel((void **)&tmp);
		return (-1);
	}
	ft_memdel((void **)&tmp);
	return (0);
}

int			handle_get(int sockfd, char **args)
{
	t_transfer_hdr	hdr;
	char			*filepath;
	int				ack_res;

	if (args[1] == NULL)
		return (arg_required());
	if (send_payload_only_cmd(
		sockfd, CMD_GET, args[1], ft_strlen(args[1]) + 1) == -1)
		return (-1);
	ack_res = receive_ack(sockfd, &hdr, &filepath);
	if (ack_res == -1)
		return (-1);
	else if (ack_res == 1)
		return (0);
	if (receive_file(sockfd, hdr.filesize, filepath) == -1)
	{
		ft_memdel((void **)&filepath);
		return (-1);
	}
	ft_memdel((void **)&filepath);
	return (0);
}