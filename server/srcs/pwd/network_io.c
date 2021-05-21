#include "server.h"

static t_uint8	*write_pwdack_msg(char *cwd)
{
	t_pwdack_hdr	*hdr;
	t_uint64		size;
	t_uint8			*msg;
	t_uint8			*payload;

	if (write_pwdack_payload(cwd, &payload, &size) == -1)
		return (NULL);
	if ((msg = ft_memalloc(sizeof(t_pwdack_hdr) + size)) == NULL)
	{
		ft_memdel((void **)&payload);
		return (NULL);
	}
	hdr = (t_pwdack_hdr *)msg;
	hdr->cmd = CMD_PWDACK;
	hdr->size = size;
	ft_memcpy(msg + sizeof(t_pwdack_hdr), payload, size);
	ft_memdel((void **)&payload);
	return (msg);
}

static int		send_pwdack(int sockfd, char *cwd)
{
	t_uint8			*msg;
	t_pwdack_hdr	*hdr;

	if ((msg = write_pwdack_msg(cwd)) == NULL)
		return (-1);
	hdr = (t_pwdack_hdr *)msg;
	if (send(sockfd, msg, hdr->size + sizeof(t_pwdack_hdr), 0) == -1)
	{
		ft_memdel((void **)&msg);
		return (-1);
	}
	ft_memdel((void **)&msg);
	return (0);
}

int				handle_pwd(int sockfd, char *cwd)
{
	return (send_pwdack(sockfd, cwd));
}