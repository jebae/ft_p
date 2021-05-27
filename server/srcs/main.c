#include "server.h"

static int	usage(void)
{
	printf("usage: ./server [port]\n");
	return (1);
}

int			main(int argc, char *argv[])
{
	t_uint16	port;

	if (argc < 2)
		return (usage());
	port = ft_atoi(argv[1]);
	run_server(port);
	return (0);
}