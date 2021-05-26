#include <gtest/gtest.h>
#include "server.h"

const char	*dirs[] = {
	"a",
	"a/b", "a/c",
	"a/c/d",
};

const char	*files[] = {
	"a/woody.txt", "a/buzz.txt",
	"a/b/slinky.py", "a/b/potato.rb",
	"a/c/ham.js", "a/c/alex.js",
	"a/c/d/zugg.py",
};

class RemoveDirTest: public ::testing::Test
{
protected:
	void	SetUp(void)
	{
		for (int i=0; i < (int)(sizeof(dirs) / sizeof(char *)); i++)
		{
			if (mkdir(dirs[i], 0755) == -1)
				throw "failed to mkdir";
		}

		for (int i=0; i < (int)(sizeof(files) / sizeof(char *)); i++)
		{
			int fd = open(files[i], O_CREAT, 0644);

			if (fd == -1)
				throw "failed to create file";
			close(fd);
		}
	}

	void	TearDown(void)
	{
		struct stat	st;

		for (int i=0; i < (int)(sizeof(files) / sizeof(char *)); i++)
		{
			if (lstat(files[i], &st) == -1)
				continue ;

			if (remove(files[i]) == -1)
				throw "failed to remove file";
		}

		for (int i=(int)(sizeof(dirs) / sizeof(char *)) - 1; i >= 0; i--)
		{
			if (lstat(dirs[i], &st) == -1)
				continue ;

			rmdir(dirs[i]);
		}

	}
};

TEST_F(RemoveDirTest, single_level)
{
	struct stat		st;

	// execute
	ASSERT_EQ(remove_dir((char *)"a/b"), 0);

	// test
	ASSERT_EQ(lstat((char *)"a/b", &st), -1);
}

TEST_F(RemoveDirTest, has_subdir)
{
	struct stat		st;

	// execute
	ASSERT_EQ(remove_dir((char *)"a/c"), 0);

	// test
	ASSERT_EQ(lstat((char *)"a/c", &st), -1);
}

TEST_F(RemoveDirTest, has_multi_level_subdir)
{
	struct stat		st;

	// execute
	ASSERT_EQ(remove_dir((char *)"a"), 0);

	// test
	ASSERT_EQ(lstat((char *)"a", &st), -1);
}