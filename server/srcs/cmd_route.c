#include "server.h"

static	void	log_cmd(
	t_uint8 type, struct sockaddr_in *cli_addr)
{
	char	cmd[8];

	if (type == CMD_LS)
		ft_strcpy(cmd, "LS");
	else if (type == CMD_CD)
		ft_strcpy(cmd, "CD");
	else if (type == CMD_PWD)
		ft_strcpy(cmd, "PWD");
	else if (type == CMD_GET)
		ft_strcpy(cmd, "GET");
	else if (type == CMD_PUT)
		ft_strcpy(cmd, "PUT");
	else if (type == CMD_DEL)
		ft_strcpy(cmd, "DEL");
	else if (type == CMD_MKDIR)
		ft_strcpy(cmd, "MKDIR");
	else if (type == CMD_RMDIR)
		ft_strcpy(cmd, "RMDIR");

	printf("[%s] %s:%d\n",
		cmd, inet_ntoa(cli_addr->sin_addr), cli_addr->sin_port);
}

/**
 * return values
 *  0: no error
 *  1: quit connection
 * -1: error
 */
int		cmd_route(
	int sockfd, struct sockaddr_in *cli_addr, t_hdr *hdr, char **cwd)
{
	int		res;

	res = -1;
	log_cmd(hdr->cmd, cli_addr);
	if (hdr->cmd == CMD_LS)
		res = handle_ls(sockfd, (t_ls_hdr *)hdr, *cwd);
	else if (hdr->cmd == CMD_CD)
		res = handle_cd(sockfd, (t_cd_hdr *)hdr, cwd);
	else if (hdr->cmd == CMD_PWD)
		res = handle_pwd(sockfd, *cwd);
	else if (hdr->cmd == CMD_GET)
		res = handle_get(sockfd, (t_get_hdr *)hdr, *cwd);
	else if (hdr->cmd == CMD_PUT)
		res = handle_put(sockfd, (t_hdr *)hdr, *cwd);
	else if (hdr->cmd == CMD_DEL)
		res = handle_del(sockfd, (t_hdr *)hdr, *cwd);
	else if (hdr->cmd == CMD_MKDIR)
		res = handle_mkdir(sockfd, (t_hdr *)hdr, *cwd);
	else if (hdr->cmd == CMD_RMDIR)
		res = handle_rmdir(sockfd, (t_hdr *)hdr, *cwd);
	if (res == -1)
		send_error(sockfd, "server error");
	return (res);
}