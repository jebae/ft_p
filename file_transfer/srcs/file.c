#include "file_transfer.h"

char	*extract_filename(char *filepath)
{
	int		i;

	i = ft_strlen(filepath);
	while (i >= 0)
	{
		if (filepath[i] == '/')
			return (filepath + i + 1);
		i--;
	}
	return (filepath);
}

int		is_dir_exist(char *path)
{
	DIR	*dir;

	dir = opendir(path);
	if (dir == NULL)
		return (0);
	closedir(dir);
	return (1);
}

int		is_file_exist(char *path)
{
	struct stat	st;
	int			fd;
	int			res;

	fd = open(path, O_RDONLY, 0);
	if (fd == -1)
		return (0);
	if (fstat(fd, &st) == -1)
	{
		close(fd);
		return (0);
	}
	res = get_file_type(st.st_mode) == 'f';
	close(fd);
	return (res);
}

char	get_file_type(mode_t mode)
{
	mode = mode & S_IFMT;
	if (mode == S_IFDIR)
		return ('d');
	else if (mode == S_IFREG)
		return ('f');
	return ('x');
}