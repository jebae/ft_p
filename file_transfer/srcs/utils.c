#include "file_transfer.h"

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
	t_uint64	percent;

	percent = transferred * 100 / filesize;
	if (percent > 100)
		percent = 100;
	print_log("seq %u %llu%%", seq, percent);
}

int		handle_err(const char *msg)
{
	print_log(msg);
	return (-1);
}