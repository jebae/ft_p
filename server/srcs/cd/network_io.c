#include "server.h"

static int	send_cdack(int sockfd)
{
	t_cdack_hdr	hdr;

	hdr.cmd = CMD_CDACK;
	hdr.size = 0;
	if (send(sockfd, &hdr, sizeof(t_cdack_hdr), 0) == -1)
		return (-1);
	return (0);
}

static int	change_cwd(char **cwd, char *path)
{
	char	*tmp;

	tmp = *cwd;
	if ((*cwd = ft_strdup(path)) == NULL)
		return (-1);
	ft_memdel((void **)&tmp);
	return (0);
}

int			handle_cd(int sockfd, t_cd_hdr *hdr, char **cwd)
{
	char	*path;
	int		res;

	path = (hdr->size == 0)
		? ft_strjoin(ROOT_DIR, "/")
		: resolve_payload_path(sockfd, hdr, *cwd);
	if (path == NULL)
		return (-1);
	if (is_dir_exist(path))
	{
		res = 0;
		if (change_cwd(cwd, path + ft_strlen(ROOT_DIR)) == -1
			|| send_cdack(sockfd) == -1)
			res = -1;
	}
	else
		res = send_error(sockfd, "directory not exist");
	ft_memdel((void **)&path);
	return (0);
}