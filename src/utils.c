/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jingwu <jingwu@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 10:27:42 by jingwu            #+#    #+#             */
/*   Updated: 2024/11/04 10:27:42 by jingwu           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	error_msg(char	*message)
{
	if (!message)
		return ;
	printf("%s\n", message);
}

/*
	@function
	Get the current time in miliseconds.

	@return
	return the current time in miliseconds unit.

	@gettimeofday()
	included in <sys/time.h>;

	Syntax:
	int gettimeofday(struct timeval *tv, struct timezone *tz);
	tv: A pointer to a struct timeval, which will store the current time.
	This structure has two fields:
		tv_sec: The number of seconds since the Epoch.
		tv_usec: The number of microseconds (1,000,000 microseconds = 1 second)
		since the last second.
	tz: A pointer to a struct timezone, which was originally meant for timezone
	data but is usually set to NULL as it’s obsolete.

	Return Value
		gettimeofday() returns 0 on success. If an error occurs, it returns -1,
		and errno is set appropriately.

*/
time_t	get_time_in_ms(void)
{
	struct timeval	tv;
	time_t			value;

	if (gettimeofday(&tv, NULL) == 0)
	{
		value = tv.tv_sec * 1000 + tv.tv_usec / 1000;
		return (value);
	}
	else
	{
		error_msg(GET_TIME_ERR);
		return (NULL);
	}
}

/*
	@function
	To output the each status message of a philosopher.
*/
void	print_status_msg(t_philo *philo, bool stop_sign, t_status status)
{
	time_t	time;

	pthread_mutex_lock(&philo->table->write_lock);
	if (is_simulation_stopped(philo->table) == true && stop_sign == false)
	{
		pthread_mutex_unlock(&philo->table->write_lock);
		return ;
	}
	time = get_time_in_ms() - philo->table->start_time;
	if (status == DIED)
		printf("%ld %d %s\n", time, philo->id + 1, "died");
	else if (status == EATING)
		printf("%ld %d %s\n", time, philo->id + 1, "is eating");
	else if (status == THINKING)
		printf("%ld %d %s\n", time, philo->id + 1, "is thinking");
	else if (status == SLEEPING)
		printf("%ld %d %s\n", time, philo->id + 1, "is sleeping");
	else if (status == GOT_LEFT_FORK || status == GOT_RIGHT_FORK)
		printf("%ld %d %s\n", time, philo->id + 1, "has taken a fork");
	pthread_mutex_unlock(&philo->table->write_lock);
}
