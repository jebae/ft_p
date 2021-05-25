#include "client.h"

static char	*get_absolute_path(char *filepath)
{
	char	*path;
	char	*tmp;

	path = getcwd(NULL, 0);
	if (path == NULL)
		return (NULL);
	tmp = path;
	path = resolve_path(path, filepath);
	ft_memdel((void **)&tmp);
	if (path == NULL)
		return (NULL);
	return (path);
}

/**
 * 1. get absolute path
 * 2. check file is sendable
 * 3. send
 */
int			handle_put(int sockfd, char **args)
{
	t_transfer_hdr	hdr;
	char			*filepath;
	int				res;

	if (args[1] == NULL)
		return (arg_required());
	filepath = get_absolute_path(args[1]);
	if (filepath == NULL)
		return (-1);
	if (!is_file_exist(filepath))
	{
		ft_memdel((void **)&filepath);
		printf("file not exist\n");
		return (0);
	}
	hdr.cmd = CMD_PUT;
	res = send_file(sockfd, &hdr, filepath);
	ft_memdel((void **)&filepath);
	return (res);
}