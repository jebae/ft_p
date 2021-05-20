#include <gtest/gtest.h>
#include "server.h"

char		*pwd;
const char	*testdir = "testdir";
const char	*filenames[] = { "woody.py", "buzz.rb", "slinky.js" };
const char	*dirnames[] = { "jessy", "ham" };

class LsTest: public ::testing::Test
{
protected:
	static void	SetUpTestSuite(void)
	{
		int			fd;
		char		buf = 0xFF;

		pwd = getcwd(NULL, 0);
		srand(time(NULL));

		// make test directory
		mkdir(testdir, 0755);
		chdir(testdir);

		// make test files
		for (unsigned long i=0; i < sizeof(filenames) / sizeof(char *); i++)
		{
			int		filesize = rand() % 1024;

			fd = open(filenames[i], O_RDWR | O_CREAT, 0755);
			for (int j=0; j < filesize; j++)
			{
				write(fd, &buf, 1);
			}
			close(fd);
		}

		// make test subdirectories
		for (unsigned long i=0; i < sizeof(dirnames) / sizeof(char *); i++)
		{
			mkdir(dirnames[i], 0755);
		}

		chdir(pwd);
	}

	static void	TearDownTestSuite(void)
	{
		const char	*filenames[] = { "woody.py", "buzz.rb", "slinky.js" };
		const char	*dirnames[] = { "jessy", "ham" };

		chdir(testdir);

		// remove files
		for (unsigned long i=0; i < sizeof(filenames) / sizeof(char *); i++)
		{
			remove(filenames[i]);
		}

		// remove sub directories
		for (unsigned long i=0; i < sizeof(dirnames) / sizeof(char *); i++)
		{
			rmdir(dirnames[i]);
		}

		chdir(pwd);
		free(pwd);
		rmdir(testdir);
	}
};

TEST_F(LsTest, get_ls_payload_simple_info)
{
	char		path[256];
	char		expected_types[] = {'f', 'd', 'd', 'f', 'f'};
	const char	*expected_names[] = {
		"buzz.rb",
		"ham",
		"jessy",
		"slinky.js",
		"woody.py",
	};
	int			file_cnt = sizeof(expected_types);

	// set path
	strcpy(path, pwd);
	strcat(path, "/");
	strcat(path, testdir);

	t_uint64	expected_size = sizeof(t_file_info) * file_cnt;

	for (int i=0; i < file_cnt; i++)
	{
		expected_size += strlen(expected_names[i]) + 1;
	}

	// execute
	t_uint64	size;
	t_uint32	cnt;
	t_uint8		*payload;
	t_file_info	*info;
	int			res;

	res = write_lsack_payload(path, &payload, &size, &cnt);
	info = (t_file_info *)payload;

	// test
	ASSERT_EQ(res, 0);
	ASSERT_EQ(size, expected_size);
	ASSERT_EQ(cnt, (t_uint32)file_cnt);

	for (int i=0; i < file_cnt; i++)
	{
		ASSERT_EQ(info->type, expected_types[i]);
		ASSERT_STREQ((char *)payload + info->name_off, expected_names[i]);
		info++;
	}
	free(payload);
}

TEST_F(LsTest, get_ls_payload_not_exist)
{
	t_uint8		*payload;
	t_uint64	size;
	t_uint32	cnt;

	int res = write_lsack_payload((char *)"/not_exist", &payload, &size, &cnt);

	ASSERT_EQ(res, 0);
	ASSERT_EQ(payload, (t_uint8 *)NULL);
	ASSERT_EQ(size, (t_uint64)0);
	ASSERT_EQ(cnt, (t_uint32)0);
}