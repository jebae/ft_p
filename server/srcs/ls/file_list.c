#include "server.h"

static int			cmp_name(void *a, void *b)
{
	return (ft_strcmp(((t_file_node *)a)->name,
		((t_file_node *)b)->name));
}

static char			get_file_type(mode_t mode)
{
	mode = mode & S_IFMT;
	if (mode == S_IFDIR)
		return ('d');
	return ('f');
}

static t_file_node	*null_with_closing_file(int fd)
{
	close(fd);
	return (NULL);
}

static t_file_node	*get_file_node(char *parent_dir, t_dirent *entry)
{
	t_file_node		*info;
	char			path[1024];
	struct stat		st;
	int				fd;

	ft_strcpy(path, parent_dir);
	ft_strcat(path, "/");
	ft_strcat(path, entry->d_name);
	if ((fd = open(path, O_RDONLY)) == -1)
		return (NULL);
	if (fstat(fd, &st) == -1)
		return (null_with_closing_file(fd));
	info = (t_file_node *)ft_memalloc(sizeof(t_file_node));
	if (info == NULL)
		return (null_with_closing_file(fd));
	info->type = get_file_type(st.st_mode);
	info->name = entry->d_name;
	info->size = st.st_size;
	info->created_at = st.st_ctime;
	close(fd);
	return (info);
}

int					get_file_list(DIR *dir, char *path, t_list *list)
{
	t_dirent	*entry;
	t_file_node	*node;

	while ((entry = readdir(dir)) != NULL)
	{
		if (ft_strcmp(entry->d_name, ".") == 0
			|| ft_strcmp(entry->d_name, "..") == 0)
			continue ;
		node = get_file_node(path, entry);
		if (node == NULL)
			return (-1);
		if (push_list_node(node, list) == -1)
			return (-1);
	}
	sort_list(list, cmp_name);
	return (0);
}