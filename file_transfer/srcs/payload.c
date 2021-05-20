#include "file_transfer.h"

int		read_payload(int sockfd, t_hdr *hdr, t_uint8 **payload)
{
	if ((*payload = (t_uint8 *)ft_memalloc(hdr->size)) == NULL)
		return (-1);
	if (recv(sockfd, *payload, hdr->size, 0) == -1)
	{
		ft_memdel((void **)&payload);
		return (-1);
	}
	return (0);
}