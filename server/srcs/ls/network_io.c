#include "server.h"

static t_uint8	*write_lsack_msg(char *path)
{
	t_uint64	size;
	t_uint32	cnt;
	t_uint8		*msg;
	t_uint8		*payload;
	t_lsack_hdr	*hdr;

	if (write_lsack_payload(path, &payload, &size, &cnt) == -1)
		return (NULL);
	if ((msg = (t_uint8 *)ft_memalloc(size + sizeof(t_lsack_hdr))) == NULL)
	{
		ft_memdel((void **)&payload);
		return (NULL);
	}
	hdr = (t_lsack_hdr *)msg;
	hdr->cmd = CMD_LSACK;
	hdr->size = size;
	hdr->cnt = cnt;
	ft_memcpy(msg + sizeof(t_lsack_hdr), payload, size);
	ft_memdel((void **)&payload);
	return (msg);
}

static int		send_lsack_msg(int sockfd, char *path)
{
	t_uint8		*msg;
	t_lsack_hdr	*hdr;

	if ((msg = write_lsack_msg(path)) == NULL)
		return (-1);
	hdr = (t_lsack_hdr *)msg;
	if (send(sockfd, msg, hdr->size + sizeof(t_lsack_hdr), 0) == -1)
	{
		ft_memdel((void **)&msg);
		return (-1);
	}
	ft_memdel((void **)&msg);
	return (0);
}

static char		*get_path(int sockfd, t_ls_hdr *hdr, char *cwd)
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
	path = ft_strjoin(ROOT_DIR, path);
	ft_memdel((void **)&tmp);
	return (path);
}

int				handle_ls(int sockfd, t_ls_hdr *hdr, char *cwd)
{
	char	*path;
	int		res;

	if ((path = get_path(sockfd, hdr, cwd)) == NULL)
		return (-1);
	res = (is_dir_exist(path))
		? send_lsack_msg(sockfd, path)
		: send_error(sockfd, "directory not exist");
	ft_memdel((void **)&path);
	return (res);
}