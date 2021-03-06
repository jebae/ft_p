#ifndef CLIENT_H
# define CLIENT_H

# ifdef __cplusplus
extern "C" {
# endif
# include "file_transfer.h"
# ifdef __cplusplus
}
# endif

# define ROOT_DIR		".storage"
# define RCV_TIMEOUT	60

int		run_console(int sockfd);
void	print_file_size(t_int64 size);
void	print_file_created_at(time_t created_at);
int		handle_err_ack(int sockfd, t_hdr *hdr);
int		handle_ls(int sockfd, char **args);
int		handle_cd(int sockfd, char **args);
int		handle_get(int sockfd, char **args);
int		handle_put(int sockfd, char **args);
int		handle_del(int sockfd, char **args);
int		handle_mkdir(int sockfd, char **args);
int		handle_rmdir(int sockfd, char **args);
int		handle_pwd(int sockfd);
int		handle_quit(int sockfd);
int		handle_lls(char **args);
int		send_payload_only_cmd(
	int sockfd, t_uint8 type, char *payload, t_uint64 size);
int		arg_required(void);
int		connect_server(t_uint16 port);

#endif