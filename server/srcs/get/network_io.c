#include "server.h"

int				handle_get(int sockfd, t_get_hdr *hdr, char *cwd)
{
	char			*path;
	t_transfer_hdr	transfer_hdr;

	path = resolve_payload_path(sockfd, hdr, cwd);
	if (path == NULL)
		return (-1);
	if (!is_file_exist(path))
	{
		ft_memdel((void **)&path);
		return (send_error(sockfd, "file not exist"));
	}
	transfer_hdr.cmd = CMD_GETACK;
	return (send_file(sockfd, &transfer_hdr, path));
}