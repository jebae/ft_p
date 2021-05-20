#include "server.h"

static t_uint64	get_name_table_size(t_list *list)
{
	t_file_node	*file;
	t_list_node	*node;
	t_uint64	size;

	size = 0;
	node = list->head;
	while (node)
	{
		file = (t_file_node *)node->data;
		size += ft_strlen(file->name) + 1;
		node = node->next;
	}
	return (size);
}

static void		write_payload(t_list *list, t_uint8 *payload)
{
	char		*name_table;
	t_file_info	*info;
	t_file_node	*file;
	t_list_node	*node;

	info = (t_file_info *)payload;
	name_table = (char *)payload + list->length * sizeof(t_file_info);
	node = list->head;
	while (node)
	{
		file = (t_file_node *)node->data;
		info->type = file->type;
		info->size = file->size;
		info->created_at = file->created_at;
		info->name_off = name_table - (char *)payload;
		ft_strcpy(name_table, file->name);
		name_table += ft_strlen(file->name) + 1;
		info++;
		node = node->next;
	}
}

int				write_lsack_payload(
	char *path, t_uint8 **payload, t_uint64 *size, t_uint32 *cnt)
{
	DIR			*dir;
	t_list		list;

	*size = 0;
	*cnt = 0;
	*payload = NULL;
	if ((dir = opendir(path)) == NULL)
		return (0);
	init_list(&list);
	if (get_file_list(dir, path, &list) == -1)
	{
		closedir(dir);
		return (-1);
	}
	*size = sizeof(t_file_info) * list.length + get_name_table_size(&list);
	*cnt = list.length;
	if ((*payload = (t_uint8 *)ft_memalloc(*size)) != NULL)
		write_payload(&list, *payload);
	clear_list(&list);
	closedir(dir);
	return ((*payload == NULL) ? -1 : 0);
}