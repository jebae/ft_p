#ifndef FILE_TRANSFER_H
# define FILE_TRANSFER_H

# include <fcntl.h>
# include <sys/stat.h>
# include <sys/socket.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <stdarg.h>
# include <dirent.h>
# include <stdio.h>

# ifdef __cplusplus
extern "C" {
# endif
# include "libft.h"
# include "get_next_line.h"
# ifdef __cplusplus
}
# endif

# ifndef CHUNK_SIZE
# define CHUNK_SIZE		1024
# endif

# ifndef CHUNK_COUNT
# define CHUNK_COUNT	32
# endif

# define CMD_ERR		0
# define CMD_LS			1
# define CMD_LSACK		2

# define KB				(1 << 10)
# define MB				(1 << 20)
# define GB				(1 << 30)

typedef char				t_int8;
typedef unsigned char		t_uint8;
typedef short				t_int16;
typedef unsigned short		t_uint16;
typedef int					t_int32;
typedef unsigned int		t_uint32;
typedef long long			t_int64;
typedef unsigned long long	t_uint64;

# pragma pack(push, 1)

typedef struct			s_chunk_stack
{
	t_int32		arr[CHUNK_COUNT];
	t_int32		top;
}						t_chunk_stack;

typedef struct			s_heap_entry
{
	t_int32		data_idx;
	t_uint32	seq;
	t_uint64	size;
}						t_heap_entry;

typedef struct			s_chunk_heap
{
	t_heap_entry	arr[CHUNK_COUNT];
	t_int32			len;
}						t_chunk_heap;

typedef struct			s_chunk_buf
{
	t_chunk_stack	stack;
	t_chunk_heap	heap;
	t_uint8			data[CHUNK_COUNT][CHUNK_SIZE];
}						t_chunk_buf;

typedef struct			s_hdr
{
	t_uint8		cmd;
	t_uint64	size;
}						t_hdr;

typedef struct			s_transfer_hdr
{
	t_uint8		cmd;
	t_uint64	size;
	t_uint32	seq;
	t_uint32	ack;
	t_uint32	rcwd;
}						t_transfer_hdr;

typedef struct			s_ls_hdr
{
	t_uint8		cmd;
	t_uint64	size;
}						t_ls_hdr;

typedef struct			s_lsack_hdr
{
	t_uint8		cmd;
	t_uint64	size;
	t_uint32	cnt;
}						t_lsack_hdr;

typedef struct			s_file_info
{
	char		type;
	t_int64		size;
	time_t		created_at;
	t_uint32	name_off;
}						t_file_info;

# pragma pack(pop)

void					init_chunk_buf(t_chunk_buf *buf);
int						push_chunk(
	t_uint32 seq, t_uint64 size, t_uint8 *chunk, t_chunk_buf *buf);

int						pop_chunk(
	t_uint8 *data, t_uint64 *size, t_chunk_buf *buf);

char					*extract_filename(char *filepath);
void					print_log(const char *format, ...);

void					log_transfer_percent(
	t_uint32 seq, t_uint64 transferred, t_uint64 filesize);

int						handle_err(const char *msg);

int						is_dir_exist(char *path);

int						send_file(
	int sockfd, t_transfer_hdr *hdr, char *filepath);

int						receive_file(
	int sockfd, t_uint64 filesize, char *filepath);

int						read_payload(
	int sockfd, t_hdr *hdr, t_uint8 **payload);

#endif