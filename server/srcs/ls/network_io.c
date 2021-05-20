#include "server.h"

static t_uint8	*write_lsack_msg(char *path)
{
	t_uint64	size;
	t_uint32	cnt;
	t_uint8		*msg;
	t_uint8		*payload;

	if ((path = ft_strjoin(ROOT_DIR, path)) == NULL)
		return (NULL);
	if (write_lsack_payload(path, &payload, &size, &cnt) == -1)
	{
		ft_memdel((void **)&path);
		return (NULL);
	}
	ft_memdel((void **)&path);
	if ((msg = (t_uint8 *)ft_memalloc(size + sizeof(t_lsack_hdr))) == NULL)
	{
		ft_memdel((void **)&payload);
		return (NULL);
	}
	((t_lsack_hdr *)msg)->cmd = CMD_LSACK;
	((t_lsack_hdr *)msg)->size = size;
	((t_lsack_hdr *)msg)->cnt = cnt;
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

int				handle_ls(int sockfd, t_ls_hdr *hdr, char *cwd)
{
	char	*path;
	char	*tmp;

	if (hdr->size == 0)
		return (send_lsack_msg(sockfd, cwd));
	else
	{
		if (read_payload(sockfd, (t_hdr *)hdr, (t_uint8 **)&path) == -1)
			return (-1);
		tmp = path;
		if ((path = resolve_path(cwd, path)) == NULL)
		{
			ft_memdel((void **)&tmp);
			return (-1);
		}
		ft_memdel((void **)&tmp);
		if (send_lsack_msg(sockfd, path) == -1)
		{
			ft_memdel((void **)&path);
			return (-1);
		}
		ft_memdel((void **)&path);
		return (0);
	}
}