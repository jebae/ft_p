#include "server.h"

static int	receive_syn(
	int sockfd, t_transfer_hdr *tf_hdr, char *cwd, char **filepath)
{
	if (recv(sockfd, (t_uint8 *)tf_hdr + sizeof(t_hdr),
		sizeof(t_transfer_hdr) - sizeof(t_hdr), 0) == -1)
		return (-1);
	*filepath = resolve_payload_path(sockfd, (t_hdr *)tf_hdr, cwd);
	if (*filepath == NULL)
		return (-1);
	return (0);
}

int			handle_put(int sockfd, t_hdr *hdr, char *cwd)
{
	t_transfer_hdr	tf_hdr;
	char			*filepath;
	int				res;

	ft_memcpy(&tf_hdr, hdr, sizeof(t_hdr));
	if (receive_syn(sockfd, &tf_hdr, cwd, &filepath) == -1)
		return (-1);
	res = receive_file(sockfd, tf_hdr.filesize, filepath);
	ft_memdel((void **)&filepath);
	return (res);
}