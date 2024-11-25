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
		return (-1);
	}
}

void	ft_free(void *str)
{
	if (str)
		free (str);
	str = NULL;
}

/*
	@what the function does?
	Prints the passed error message to stand error mode.

	@return
	return false (0).

	@usage
	Use for error management for the whole program.
*/
bool	error_msg(char	*message)
{
	if (message)
		putstr_fd(message, 2);
	return (false);
}

/*
	@function
	Print out error message and return NULL.
	Used when error generated during initialization part.

	@return
	return NULL.
*/
void	*error_msg_null(char *message)
{
	if (message)
		error_msg(message);
	return (NULL);
}

/*
	@function
	To output the each status message of a philosopher.
*/
void	print_philo_status_msg(t_philo *philo, t_status status)
{
	time_t	time;

	if (has_simulation_stopped(philo->table) == true)
		return ;
	pthread_mutex_lock(&philo->table->write_lock);
	time = get_time_in_ms() - philo->table->start_time;
	if (status == DIED)
		printf("%ld %zu %s\n", time, philo->id, "died");
	else if (status == EATING)
		printf("%ld %zu %s\n", time, philo->id, "is eating");
	else if (status == THINKING)
		printf("%ld %zu %s\n", time, philo->id, "is thinking");
	else if (status == SLEEPING)
		printf("%ld %zu %s\n", time, philo->id, "is sleeping");
	else if (status == GOT_LEFT_FORK || status == GOT_RIGHT_FORK)
		printf("%ld %zu %s\n", time, philo->id, "has taken a fork");
	pthread_mutex_unlock(&philo->table->write_lock);
}
