#include "server.h"

char	*get_absolute_path(char *path)
{
	return (ft_strjoin(ROOT_DIR, path));
}

char	*resolve_payload_path(int sockfd, t_ls_hdr *hdr, char *cwd)
{
	char	*path;
	char	*tmp;

	if (read_payload(sockfd, (t_hdr *)hdr, (t_uint8 **)&path) == -1)
		return (NULL);
	tmp = path;
	if ((path = resolve_path(cwd, path)) == NULL)
	{
		ft_memdel((void **)&tmp);
		return (NULL);
	}
	ft_memdel((void **)&tmp);
	tmp = path;
	path = get_absolute_path(path);
	ft_memdel((void **)&tmp);
	return (path);
}

int		send_error(int sockfd, const char *payload)
{
	t_uint8				msg[128];
	t_hdr				*hdr;

	hdr = (t_hdr *)msg;
	hdr->cmd = CMD_ERR;
	hdr->size = ft_strlen(payload) + 1;
	ft_strcpy((char *)msg + sizeof(t_hdr), payload);
	if (send(sockfd, msg, hdr->size + sizeof(t_hdr), 0) == -1)
		return (-1);
	return (0);
}