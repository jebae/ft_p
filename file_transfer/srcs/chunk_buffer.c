#include "file_transfer.h"

void		init_chunk_buf(t_chunk_buf *buf)
{
	t_chunk_stack	*stack;
	int				i;

	stack = &buf->stack;
	stack->top = CHUNK_COUNT - 1;
	i = 0;
	while (i <= stack->top)
	{
		stack->arr[i] = stack->top - i;
		i++;
	}
	buf->heap.len = 0;
}

static void	heappush(
	t_int32 data_idx, t_uint32 seq, t_uint64 size, t_chunk_heap *heap)
{
	t_int32			idx;
	t_int32			parent;
	t_heap_entry	temp;

	heap->arr[heap->len] = (t_heap_entry){data_idx, seq, size};
	idx = heap->len;
	while (idx > 0)
	{
		parent = (idx - 1) >> 1;
		if (heap->arr[parent].seq > heap->arr[idx].seq)
		{
			temp = heap->arr[parent];
			heap->arr[parent] = heap->arr[idx];
			heap->arr[idx] = temp;
			idx = parent;
		}
		else
			break ;
	}
	heap->len++;
}

int			push_chunk(
	t_uint32 seq, t_uint64 size, t_uint8 *chunk, t_chunk_buf *buf)
{
	t_chunk_stack	*stack;
	t_int32			data_idx;

	if (buf->stack.top < 0 || size > CHUNK_SIZE)
		return (-1);
	stack = &buf->stack;
	data_idx = stack->arr[stack->top--];
	heappush(data_idx, seq, size, &buf->heap);
	ft_memcpy(buf->data[data_idx], chunk, CHUNK_SIZE);
	return (0);
}

static t_heap_entry	heappop(t_chunk_heap *heap)
{
	t_heap_entry	entry[2];
	t_int32			idx;
	t_int32			next;
	t_int32			child[2];

	entry[0] = heap->arr[0];
	heap->arr[0] = heap->arr[--heap->len];
	idx = 0;
	while ((idx << 1) + 1 < heap->len)
	{
		next = idx;
		child[0] = (idx << 1) + 1;
		child[1] = (idx << 1) + 2;
		if (heap->arr[child[0]].seq < heap->arr[next].seq)
			next = child[0];
		if (child[1] < heap->len && heap->arr[child[1]].seq < heap->arr[next].seq)
			next = child[1];
		if (idx == next)
			break ;
		entry[1] = heap->arr[idx];
		heap->arr[idx] = heap->arr[next];
		heap->arr[next] = entry[1];
		idx = next;
	}
	return (entry[0]);
}

int			pop_chunk(t_uint8 *data, t_uint64 *size, t_chunk_buf *buf)
{
	t_chunk_stack	*stack;
	t_heap_entry	poped;

	if (buf->stack.top == CHUNK_COUNT - 1)
		return (-1);
	stack = &buf->stack;
	poped = heappop(&buf->heap);
	ft_memcpy(data, buf->data[poped.data_idx], poped.size);
	*size = poped.size;
	stack->arr[++stack->top] = poped.data_idx;
	return (0);
}