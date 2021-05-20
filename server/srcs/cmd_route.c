#include "server.h"

static	void	log_cmd(
	const char *type, struct sockaddr_in *cli_addr)
{
	printf("[%s] %s:%d\n",
		type, inet_ntoa(cli_addr->sin_addr), cli_addr->sin_port);
}

int		cmd_route(
	int sockfd, struct sockaddr_in *cli_addr, t_hdr *hdr, char **cwd)
{
	int		res;

	res = -1;
	if (hdr->cmd == CMD_LS)
	{
		log_cmd("LS", cli_addr);
		res = handle_ls(sockfd, (t_ls_hdr *)hdr, *cwd);
	}
	if (res == -1)
		send_error(sockfd);
	return (res);
}