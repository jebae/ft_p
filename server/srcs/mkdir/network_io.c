#include "server.h"

static int	make_directory(char *path)
{
	if (mkdir(path, 0755) == -1)
		return (-1);
	return (0);
}

static int	send_mkdirack(int sockfd)
{
	t_mkdirack_hdr	hdr;

	hdr.cmd = CMD_MKDIRACK;
	hdr.size = 0;
	if (send(sockfd, &hdr, sizeof(t_mkdirack_hdr), 0) == -1)
		return (-1);
	return (0);
}

int			handle_mkdir(int sockfd, t_mkdir_hdr *hdr, char *cwd)
{
	char	*path;
	int		res;

	path = resolve_payload_path(sockfd, hdr, cwd);
	if (is_file_exist(path) || is_dir_exist(path))
		return (send_error(sockfd, "file or directory already exist"));
	if (make_directory(path) == -1)
	{
		ft_memdel((void **)&path);
		return (-1);
	}
	res = send_mkdirack(sockfd);
	ft_memdel((void **)&path);
	return (res);
}