/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jingwu <jingwu@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 10:27:04 by jingwu            #+#    #+#             */
/*   Updated: 2024/11/04 15:11:03 by jingwu           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/*
	usleep(100) = 100 microseconds = 0.1 milliseconds.
*/
void	wait_time(t_table *table, time_t duration)//??????
{
	time_t	wake_up_time;

	wake_up_time = get_time_in_ms + table->time_to_die;
	if (!wake_up_time)
		return ;
	while (get_time_in_ms < wake_up_time)
	{
		if (is_simulation_stopped(table))
			break ;
		usleep (100);
	}
}

/*
	@function

*/
static void	eat_sleep_routine(t_philo *philo)
{
	pthread_mutex_lock(&philo->table->fork_locks[philo->fork[0]]);
	print_status_msg(philo, false, GOT_LEFT_FORK);
	pthread_mutex_lock(&philo->table->fork_locks[philo->fork[1]]);
	print_status_msg(philo, false, GOT_RIGHT_FORK);
	print_status_msg(philo, false, EATING);
	pthread_mutex_lock(&philo->meal_time_lock);
	philo->last_eat = get_time_in_ms();
	pthread_mutex_unlock(&philo->meal_time_lock);

}

static void	*one_philo_routine(t_philo *philo)
{
	pthread_mutex_lock(&philo->table->fork_locks[philo->fork[0]]);
	print_status_msg(philo, false, GOT_LEFT_FORK);
	wait_time(philo->table, philo->table->time_to_die);//underway
	print_msg(philo, false, DIED);
	pthread_mutex_unlock(&philo->table->fork_locks[philo->fork[0]]);
	return (NULL);
}

/*
	@function
	It is called after a thread being created in start_simulation(). And it
	must be declared as "void	*<function_name>(void *<parameter_name>)"
*/
void	*routine(void *data)
{
	t_philo	*philo;

	philo = (t_philo *)data;
	pthread_mutex_lock(&philo->meal_time_lock);
	philo->last_eat = philo->table->start_time;
	pthread_mutex_unlock(&philo->meal_time_lock);
	if (philo->table->philo_nb == 1)
		return (one_phlio_routine(philo));
	else if (philo->id % 2)
		think_routine(philo, true);
	while (is_simulation_stopped(philo->table) == false)
	{
		eat_sleep_routine(philo);
		think_routine(philo, false);//underway
	}
	return (NULL);
}
