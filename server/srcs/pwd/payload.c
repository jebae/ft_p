#include "server.h"

int		write_pwdack_payload(char *cwd, t_uint8 **payload, t_uint64 *size)
{
	*size = ft_strlen(cwd) + 1;
	*payload = ft_memalloc(*size);
	if (*payload == NULL)
		return (-1);
	ft_strcpy((char *)*payload, cwd);
	return (0);
}