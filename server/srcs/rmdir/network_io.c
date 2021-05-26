#include "server.h"

static int	send_rmdirack(int sockfd)
{
	t_rmdirack_hdr	hdr;

	hdr.cmd = CMD_RMDIRACK;
	hdr.size = 0;
	if (send(sockfd, &hdr, sizeof(t_rmdirack_hdr), 0) == -1)
		return (-1);
	return (0);
}

int			handle_rmdir(int sockfd, t_rmdir_hdr *hdr, char *cwd)
{
	char	*path;
	int		res;

	path = resolve_payload_path(sockfd, hdr, cwd);
	if (path == NULL)
		return (-1);
	if (is_dir_exist(path))
		res = remove_dir(path) == -1
			? -1
			: send_rmdirack(sockfd);
	else
		res = send_error(sockfd, "directory not exist");
	ft_memdel((void **)&path);
	return (res);
}