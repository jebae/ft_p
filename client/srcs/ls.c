#include "client.h"

static void	print_file_list(t_lsack_hdr *ack_hdr, t_uint8 *payload, int detail)
{
	t_uint32	i;
	t_file_info	*info;

	info = (t_file_info *)payload;
	i = 0;
	while (i < ack_hdr->cnt)
	{
		printf("%c %s ", info->type, &payload[info->name_off]);
		if (detail)
		{
			print_file_size(info->size);
			print_file_created_at(info->created_at);
		}
		printf("\n");
		info++;
		i++;
	}
}

static int	receive_ack(int sockfd, int detail)
{
	t_hdr		hdr;
	t_lsack_hdr	ack_hdr;
	t_uint8		*payload;

	if (recv(sockfd, &hdr, sizeof(t_hdr), 0) == -1)
		return (-1);
	if (hdr.cmd == CMD_LSACK)
	{
		ft_memcpy(&ack_hdr, &hdr, sizeof(t_hdr));
		if (recv(sockfd, (void *)&ack_hdr + sizeof(t_hdr),
			sizeof(t_lsack_hdr) - sizeof(t_hdr), 0) == -1)
			return (-1);
		if (read_payload(sockfd, (t_hdr *)&ack_hdr, &payload) == -1)
			return (-1);
		print_file_list(&ack_hdr, payload, detail);
		ft_memdel((void **)&payload);
		return (0);
	}
	return (handle_err_ack(sockfd, &hdr));
}

static int	send_ls_cwd(int sockfd)
{
	t_ls_hdr	hdr;

	hdr.cmd = CMD_LS;
	hdr.size = 0;
	if (send(sockfd, &hdr, sizeof(t_ls_hdr), 0) == -1)
		return (-1);
	return (0);
}

static int	send_ls(int sockfd, char *path)
{
	t_ls_hdr	*hdr;
	t_uint8		*msg;
	t_uint32	size;

	if (path == NULL)
		return (send_ls_cwd(sockfd));
	size = ft_strlen(path) + 1;
	msg = (t_uint8 *)ft_memalloc(sizeof(t_ls_hdr) + size);
	if (msg == NULL)
		return (-1);
	hdr = (t_ls_hdr *)msg;
	hdr->cmd = CMD_LS;
	hdr->size = size;
	ft_strcpy((char *)msg + sizeof(t_ls_hdr), path);
	if (send(sockfd, msg, sizeof(t_ls_hdr) + size, 0) == -1)
	{
		ft_memdel((void **)&msg);
		return (-1);
	}
	ft_memdel((void **)&msg);
	return (0);
}

int		handle_ls(int sockfd, char **args)
{
	int		send_res;
	int		detail;

	detail = 0;
	if (args[1] == NULL)
		send_res = send_ls_cwd(sockfd);
	else if (ft_strcmp(args[1], "-l") == 0)
	{
		detail = 1;
		send_res = send_ls(sockfd, args[2]);
	}
	else
		send_res = send_ls(sockfd, args[1]);
	if (send_res == -1)
		return (-1);
	if (receive_ack(sockfd, detail) == -1)
		return (-1);
	return (0);
}