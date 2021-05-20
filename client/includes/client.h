#ifndef CLIENT_H
# define CLIENT_H

# ifdef __cplusplus
extern "C" {
# endif
# include "file_transfer.h"
# ifdef __cplusplus
}
# endif

int		run_console(int sockfd);
void	print_file_size(t_int64 size);
void	print_file_created_at(time_t created_at);
int		handle_err_ack(int sockfd, t_hdr *hdr);
int		handle_ls(int sockfd, char **args);

#endif