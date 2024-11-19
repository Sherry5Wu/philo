#include <stdio.h>
#include <sys/time.h>
#include <unistd.h>

time_t	get_time_in_ms(void)
{
	struct timeval	tv;
	time_t			value;

	if (gettimeofday(&tv, NULL) == 0)
	{
		value = tv.tv_sec * 1000 + tv.tv_usec / 1000;
		return (value);
	}
}

int	main(void)
{
	time_t	now1;
	time_t	now2;
	size_t	num;
	time_t	value;

	num = 200;
	now1 = get_time_in_ms();
	printf("now1=%ld\n", now1);
	sleep(10);
	now2 = get_time_in_ms();
	printf("now2=%ld\n", now2);
	value = now2 - now1;
	printf("value=%ld\n",value);
	printf("value=%lu\n value-num=%lu\n", (size_t)value, (size_t)value - num);
}
