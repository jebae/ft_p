#include "server.h"

void	send_error(int sockfd)
{
	static const char	*payload = "server error\n";
	t_uint8				msg[32];
	t_hdr				*hdr;

	hdr = (t_hdr *)msg;
	hdr->cmd = CMD_ERR;
	hdr->size = ft_strlen(payload) + 1;
	ft_strcpy((char *)msg + sizeof(t_hdr), payload);
	send(sockfd, msg, hdr->size + sizeof(t_hdr), 0);
}