#include "client.h"

void	print_file_size(t_int64 size)
{
	if (size > GB)
		printf("%.1fG ", (float)size / GB);
	else if (size > MB)
		printf("%.1fM ", (float)size / MB);
	else if (size > KB)
		printf("%.1fK ", (float)size / KB);
	else
		printf("%lldB ", size);
}

void	print_file_created_at(time_t created_at)
{
	t_datetime	dt;

	if (get_datetime(created_at, &dt) == -1)
		return ;
	printf("%d-%02d-%02d %02d:%02d ", dt.year, dt.month, dt.day, dt.hour, dt.minute);
}

int		handle_err_ack(int sockfd, t_hdr *hdr)
{
	char	*payload;

	payload = (char *)ft_memalloc(hdr->size);
	if (recv(sockfd, payload, hdr->size, 0) == -1)
	{
		ft_memdel((void **)&payload);
		return (-1);
	}
	printf("[ERROR] %s\n", payload);
	ft_memdel((void **)&payload);
	return (0);
}