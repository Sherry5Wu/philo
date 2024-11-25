/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jingwu <jingwu@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 13:57:23 by jingwu            #+#    #+#             */
/*   Updated: 2024/11/25 08:22:46 by jingwu           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/*
	@function
	To check if the passed philosopher is died or not.

	@what condition will a philosopher die?
	If a philosopher's hungry time is greater or equal than time_to_die,
	then this philospher is dead. hungry time = current_time - (last_meal_time
	+ time_to_eat).

	@return
	0: philosoper is dead;
	1: philosoper is NOT dead;
	-1: there is an error;
*/
static int	is_dead(t_philo *philo)
{
	time_t	last_meal;
	time_t	current_time;

	pthread_mutex_lock(&philo->philo_lock);
	last_meal = philo->last_eat;
	pthread_mutex_unlock(&philo->philo_lock);
	current_time = get_time_in_ms();
	if (current_time == -1)
	{
		error_msg(GET_TIME_ERR);
		return (-1);
	}
	if ((size_t)(current_time - last_meal) >= philo->table->time_to_die)
		return (0);
	else
		return (1);
}

/*
	@function
	Looping the philosophers to check if someone is dead. If found someone
	dead, then print out the message.

	@return
	True: someone is dead;
	False: Everyone is alive;
*/
static bool	is_someone_dead(t_table *table)
{
	size_t	i;
	int		value;

	i = 0;
	while (i < table->philo_nb)
	{
		value = is_dead(table->philos[i]);
		if ( value != 1)
		{
			if (value == 0)
			{
				// printf("found one dead\n");// for testing!!!!!!!!!!!!1
				print_philo_status_msg(table->philos[i], DIED);
			}
			// printf("in is_someone_dead, going to clean threads\n");// for testing!!!!!!!!!!!!1
			clean_threads(table, table->philo_nb);
			return (true);
		}
		i++;
	}
	return (false);
}

/*
	@function
	To check if every philosopher eats enough times (each or greater than
	must_eat_times)

	@logic
	First, checking if the program needs to monitor how many time the
	philosoper is eaten. If the value == -1, it means it doesn't need to
	be momitored. Otherwise, to check if each philosopher's meals_eaten is
	smaller then must_eat_times, it means that philosopher isn't full.

	@return
	true: every philosopher is full;
	false: not every philosoper is full;
*/
static bool	is_everyone_full(t_table *table)
{
	size_t	i;
	size_t	full_nb;

	i = 0;
	full_nb = 0;
	if (table->must_eat_times != -1)
	{
		while (i < table->philo_nb)
		{
			if (table->philos[i]->meals_eaten >= (size_t)table->must_eat_times)
				full_nb++;
			i++;
		}
		if (full_nb == table->philo_nb)
		{
			clean_threads(table, table->philo_nb);
			return (true);
		}
	}
	return (false);
}

/*
	@function
	It monitors the whole progrom running time, to check if the system should
	stop the program or not. If the program needs to be stopped, then set
	simulation_stop = true.
*/
void	monitor(t_table *table)
{
	while (true)
	{
		usleep(200);
		if (is_someone_dead(table) == true)
		{
			// pthread_mutex_lock(&table->sim_stop_lock);
			// // printf("moniter someone is dead\n");//////for testing
			// table->simulation_stop = true;
			// pthread_mutex_unlock(&table->sim_stop_lock);
			break ;
		}
		if (is_everyone_full(table) == true)
		{
			pthread_mutex_lock(&table->sim_stop_lock);
			// printf("moniter every is full\n");//////for testing
			table->simulation_stop = true;
			pthread_mutex_unlock(&table->sim_stop_lock);
			break ;
		}
	}
}

/*
	@function
	To check if the simulation is ened.
	The stop flag is protected by a mutex lock to allow any thread to check
	the simulation status without conflict.

	@return
	true: simulation is stopped;
	false: simulation continues;
*/
bool	has_simulation_stopped(t_table *table)
{
	bool	flag;

	flag = false;
	pthread_mutex_lock(&table->sim_stop_lock);
	if (table->simulation_stop == true)
		flag = true;
	pthread_mutex_unlock(&table->sim_stop_lock);
	// printf("has_simulation_stopped flag=%d\n", flag); //////for testing
	return (flag);
}
