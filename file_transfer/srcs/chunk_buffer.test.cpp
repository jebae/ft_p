#include <gtest/gtest.h>
#include "file_transfer.h"

class ChunkBufferTest: public ::testing::Test
{
protected:
	t_chunk_buf	buf;

	virtual	void SetUp(void)
	{
		init_chunk_buf(&buf);
	}
};

TEST_F(ChunkBufferTest, init_chunk_buf)
{
	// test stack
	ASSERT_EQ(buf.stack.top, CHUNK_COUNT - 1);
	for (int i=0; i <= buf.stack.top; i++)
	{
		ASSERT_EQ(buf.stack.arr[i], buf.stack.top - i);
	}

	// test heap
	ASSERT_EQ(buf.heap.len, 0);
}

TEST_F(ChunkBufferTest, push_chunk_to_empty_buffer)
{
	t_uint8			chunk[CHUNK_SIZE];
	t_uint32		seq = 56;
	t_uint64		size = 42;
	t_chunk_heap	*heap;
	t_heap_entry	*entry;

	// write data to chunk
	for (int i=0; i < CHUNK_SIZE; i++)
	{
		chunk[i] = i;
	}

	// execute
	ASSERT_NE(push_chunk(seq, size, chunk, &buf), -1);

	// test heap
	heap = &buf.heap;
	ASSERT_EQ(heap->len, 1);

	entry = &heap->arr[0];
	ASSERT_EQ(entry->data_idx, 0);
	ASSERT_EQ(entry->seq, seq);
	ASSERT_EQ(entry->size, size);

	// test stack
	ASSERT_EQ(buf.stack.top, CHUNK_COUNT - 2);

	// test data
	for (int i=0; i < CHUNK_SIZE; i++)
	{
		ASSERT_EQ(chunk[i], buf.data[entry->data_idx][i]);
	}
}

TEST_F(ChunkBufferTest, push_increasing_seq_chunk_to_buffer)
{
	t_uint8			chunk[10][CHUNK_SIZE];
	t_chunk_heap	*heap;
	t_heap_entry	*entry;

	srand(time(NULL));

	// write data to chunk
	for (int i=0; i < 10; i++)
	{
		for (int j=0; j < CHUNK_SIZE; j++)
		{
			chunk[i][j] = rand() % 0x100;
		}
	}

	// execute
	for (int i=0; i < 10; i++)
	{
		ASSERT_NE(push_chunk(i, CHUNK_SIZE - i, chunk[i], &buf), -1);
	}

	// test heap
	heap = &buf.heap;
	ASSERT_EQ(heap->len, 10);

	for (int i=0; i < 10; i++)
	{
		entry = &heap->arr[i];
		ASSERT_EQ(entry->data_idx, i);
		ASSERT_EQ(entry->seq, (t_uint32)i);
		ASSERT_EQ(entry->size, (t_uint64)CHUNK_SIZE - i);
	}

	// test stack
	ASSERT_EQ(buf.stack.top, CHUNK_COUNT - 11);

	// test data
	for (int i=0; i < 10; i++)
	{
		entry = &heap->arr[i];
		for (int j=0; j < CHUNK_SIZE; j++)
		{
			ASSERT_EQ(chunk[i][j], buf.data[entry->data_idx][j]);
		}
	}
}

TEST_F(ChunkBufferTest, push_mixed_seq_chunk_to_buffer)
{
	t_uint8			chunk[10][CHUNK_SIZE];
	t_uint32		seqs[10] = {8, 3, 1, 4, 6, 0, 7, 9, 5, 2};
	t_chunk_heap	*heap;
	t_heap_entry	*entry;

	srand(time(NULL));

	// write data to chunk
	for (int i=0; i < 10; i++)
	{
		for (int j=0; j < CHUNK_SIZE; j++)
		{
			chunk[i][j] = rand() % 0x100;
		}
	}

	// execute
	for (int i=0; i < 10; i++)
	{
		ASSERT_NE(push_chunk(seqs[i], CHUNK_SIZE - i, chunk[i], &buf), -1);
	}

	// test heap
	heap = &buf.heap;
	ASSERT_EQ(heap->len, 10);

	// test if parent seq less than child's
	for (int i=0; i < 5; i++)
	{
		t_heap_entry	*left;
		t_heap_entry	*right;

		entry = &heap->arr[i];
		left = &heap->arr[i * 2 + 1];
		ASSERT_LT(entry->seq, left->seq);

		if (i * 2 + 2 < heap->len)
		{
			right = &heap->arr[i * 2 + 2];
			ASSERT_LT(entry->seq, right->seq);
		}
	}

	// test stack
	ASSERT_EQ(buf.stack.top, CHUNK_COUNT - 11);

	// test data
	for (int i=0; i < 10; i++)
	{
		for (int j=0; j < 10; j++)
		{
			if (heap->arr[j].seq == seqs[i])
			{
				entry = &heap->arr[j];
			}
		}
		for (int j=0; j < CHUNK_SIZE; j++)
		{
			ASSERT_EQ(chunk[i][j], buf.data[entry->data_idx][j]);
		}
	}
}

TEST_F(ChunkBufferTest, push_chunk_to_full_buffer)
{
	t_uint8			chunk[CHUNK_SIZE];
	t_uint32		seq = 56;
	t_uint64		size = 42;

	// set buf.stack full
	buf.stack.top = -1;

	// execute
	ASSERT_EQ(push_chunk(seq, size, chunk, &buf), -1);
}

TEST_F(ChunkBufferTest, push_chunk_with_over_CHUNK_SIZE)
{
	t_uint8			chunk[CHUNK_SIZE];
	t_uint32		seq = 56;
	t_uint64		size = CHUNK_SIZE + 1;

	// execute
	ASSERT_EQ(push_chunk(seq, size, chunk, &buf), -1);
}

TEST_F(ChunkBufferTest, pop_chunk_from_continuous_sequence_buffer)
{
	t_uint8			chunk[10][CHUNK_SIZE];
	t_uint8			data[CHUNK_SIZE];
	t_uint32			prev_seq = 0;
	t_chunk_heap	*heap;
	t_uint64		size;

	srand(time(NULL));

	// write data to chunk
	for (int i=0; i < 10; i++)
	{
		for (int j=0; j < CHUNK_SIZE; j++)
		{
			chunk[i][j] = rand() % 0x100;
		}
	}

	// push chunk
	for (int i=0; i < 10; i++)
	{
		ASSERT_NE(push_chunk(i + 1, CHUNK_SIZE - i, chunk[i], &buf), -1);
	}

	heap = &buf.heap;

	for (int i=0; i < 10; i++)
	{
		// test min heap
		ASSERT_LT(prev_seq, heap->arr[0].seq);
		prev_seq = heap->arr[0].seq;

		// execute
		ASSERT_NE(pop_chunk(data, &size, &buf), -1);

		// test heap
		ASSERT_EQ(heap->len, 10 - i - 1);

		// test stack
		ASSERT_EQ(buf.stack.top, CHUNK_COUNT - 10 + i);

		// test data
		for (t_uint64 j=0; j < size; j++)
		{
			ASSERT_EQ(chunk[i][j], data[j]);
		}
	}
}

TEST_F(ChunkBufferTest, pop_chunk_from_mixed_sequence_buffer)
{
	t_uint8			chunk[10][CHUNK_SIZE];
	t_uint8			data[CHUNK_SIZE];
	t_uint32		seqs[10] = {9, 4, 2, 5, 7, 1, 8, 10, 6, 3};
	t_uint32		prev_seq = 0;
	t_chunk_heap	*heap;
	t_uint64		size;

	srand(time(NULL));

	// write data to chunk
	for (int i=0; i < 10; i++)
	{
		for (int j=0; j < CHUNK_SIZE; j++)
		{
			chunk[i][j] = rand() % 0x100;
		}
	}

	// push chunk
	for (int i=0; i < 10; i++)
	{
		ASSERT_NE(push_chunk(seqs[i], CHUNK_SIZE - i, chunk[i], &buf), -1);
	}

	heap = &buf.heap;

	for (int i=0; i < 10; i++)
	{
		// test min heap
		ASSERT_LT(prev_seq, heap->arr[0].seq);
		prev_seq = heap->arr[0].seq;

		// execute
		ASSERT_NE(pop_chunk(data, &size, &buf), -1);

		// test heap
		ASSERT_EQ(heap->len, 10 - i - 1);

		// test stack
		ASSERT_EQ(buf.stack.top, CHUNK_COUNT - 10 + i);

		// test data
		int	chunk_idx;

		for (chunk_idx=0; chunk_idx < 10; chunk_idx++)
		{
			if (seqs[chunk_idx] == prev_seq)
				break ;
		}

		for (t_uint64 j=0; j < size; j++)
		{
			ASSERT_EQ(chunk[chunk_idx][j], data[j]);
		}
	}
}

TEST_F(ChunkBufferTest, pop_chunk_from_empty_buffer)
{
	buf.stack.top = CHUNK_COUNT - 1;
	ASSERT_EQ(pop_chunk(NULL, NULL, &buf), -1);
}