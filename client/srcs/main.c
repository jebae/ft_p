#include "client.h"

static int	usage(void)
{
	printf("usage: ./client [port]\n");
	return (1);
}

int			main(int argc, char **argv)
{
	t_uint16	port;

	if (argc < 2)
		return (usage());
	port = ft_atoi(argv[1]);
	if (connect_server(port) == -1)
		return (1);
	return (0);
}