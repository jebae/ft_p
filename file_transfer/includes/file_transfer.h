#ifndef FILE_TRANSFER_H
# define FILE_TRANSFER_H

# ifdef __cplusplus
extern "C" {
# endif
# include "libft.h"
# include "ft_printf.h"
# ifdef __cplusplus
}
# endif

# ifndef CHUNK_SIZE
# define CHUNK_SIZE		1024
# endif

# ifndef CHUNK_COUNT
# define CHUNK_COUNT	32
# endif

typedef char				t_int8;
typedef unsigned char		t_uint8;
typedef short				t_int16;
typedef unsigned short		t_uint16;
typedef int					t_int32;
typedef unsigned int		t_uint32;
typedef long long			t_int64;
typedef unsigned long long	t_uint64;

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

void					init_chunk_buf(t_chunk_buf *buf);
int						push_chunk(
	t_uint32 seq, t_uint64 size, t_uint8 *chunk, t_chunk_buf *buf);

int						pop_chunk(
	t_uint8 *data, t_uint64 *size, t_chunk_buf *buf);

#endif