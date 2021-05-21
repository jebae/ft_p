#ifndef SERVER_H
# define SERVER_H

# include "file_transfer.h"

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

int				write_pwdack_payload(
	char *cwd, t_uint8 **payload, t_uint64 *size);

int				handle_ls(int sockfd, t_ls_hdr *hdr, char *cwd);
int				handle_cd(int sockfd, t_cd_hdr *hdr, char **cwd);
int				handle_pwd(int sockfd, char *cwd);

int				cmd_route(
	int sockfd, struct sockaddr_in *client_addr, t_hdr *hdr, char **cwd);

char			*get_absolute_path(char *path);
char			*resolve_payload_path(int sockfd, t_ls_hdr *hdr, char *cwd);
int				send_error(int sockfd, char *payload);

#endif