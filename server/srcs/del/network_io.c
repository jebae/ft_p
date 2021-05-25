#include "server.h"

static int	send_delack(int sockfd)
{
	t_cd_hdr	hdr;

	hdr.cmd = CMD_DELACK;
	hdr.size = 0;
	if (send(sockfd, &hdr, sizeof(t_delack_hdr), 0) == -1)
		return (-1);
	return (0);
}

int			handle_del(int sockfd, t_del_hdr *hdr, char *cwd)
{
	char	*path;
	int		res;

	path = resolve_payload_path(sockfd, hdr, cwd);
	if (path == NULL)
		return (-1);
	if (is_file_exist(path))
		res = (remove(path) == -1)
			? -1
			: send_delack(sockfd);
	else
		res = send_error(sockfd, "file not exist");
	ft_memdel((void **)&path);
	return (res);
}