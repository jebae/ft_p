#include "server.h"

int		send_error(int sockfd, char *payload)
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