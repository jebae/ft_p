#include "server.h"

static int	is_dot_path(char *path)
{
	return (ft_strcmp(path, ".") == 0
		|| ft_strcmp(path, "..") == 0);
}

int			remove_dir(char *path)
{
	DIR			*dir;
	t_dirent	*entry;
	int			has_err;
	char		*filepath;

	if ((dir = opendir(path)) == NULL)
		return (-1);
	has_err = 0;
	while (!has_err && (entry = readdir(dir)) != NULL)
	{
		if (is_dot_path(entry->d_name))
			continue ;
		has_err = 1;
		if ((filepath = strcat_all(3, path, "/", entry->d_name)) == NULL)
			break ;
		if (is_file_exist(filepath))
			has_err = remove(filepath) == -1;
		else if (is_dir_exist(filepath))
			has_err = remove_dir(filepath) == -1;
		ft_memdel((void **)&filepath);
	}
	closedir(dir);
	rmdir(path);
	return (has_err ? -1 : 0);
}