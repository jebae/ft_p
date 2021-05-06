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

void	print_log(const char *format, ...)
{
	va_list	ap;

	va_start(ap, format);
	printf("[File transfer] ");
	vprintf(format, ap);
	printf("\n");
	va_end(ap);
}

void	log_transfer_percent(
	t_uint32 seq, t_uint64 transferred, t_uint64 filesize)
{
	print_log("seq %u %llu%%", seq, transferred * 100 / filesize);
}

int		handle_err(const char *msg)
{
	print_log(msg);
	return (-1);
}