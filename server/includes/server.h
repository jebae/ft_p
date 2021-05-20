#ifndef SERVER_H
# define SERVER_H

# include "file_transfer.h"

# include <dirent.h>

# define ROOT_DIR		".storage"

typedef struct dirent	t_dirent;

typedef struct			s_file_node
{
	char		type;
	t_int64		size;
	time_t		created_at;
	char		*name;
}						t_file_node;

int				get_file_list(DIR *dir, char *path, t_list *list);

int				write_lsack_payload(
	char *path, t_uint8 **payload, t_uint64 *size, t_uint32 *cnt);

int				handle_ls(int sockfd, t_ls_hdr *hdr, char *cwd);

int				cmd_route(
	int sockfd, struct sockaddr_in *client_addr, t_hdr *hdr, char **cwd);

void			send_error(int sockfd);

#endif