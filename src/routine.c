/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jingwu <jingwu@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 10:27:04 by jingwu            #+#    #+#             */
/*   Updated: 2024/11/19 16:42:42 by jingwu           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/*
	@function
	Set philosopher's status to certain value.
*/
void	set_philo_status(t_philo *philo, int status)
{
	pthread_mutex_lock(&philo->philo_lock);
	philo->status = status;
	pthread_mutex_unlock(&philo->philo_lock);
}

void	put_down_forks(t_philo *philo)
{
	pthread_mutex_unlock(philo->l_fork);
	pthread_mutex_unlock(philo->r_fork);
}

/*
	@function
	The fucntion will let the philosopher thread waits for a certain amount
	of time in miliseconds. In the meanwhile, need to check if the simulation
	has ended during the pause time and cuts the pause short if it has.

	@param
	duration: the time duration that the thread needs paused.

	usleep(100) = 100 microseconds = 0.1 milliseconds.
*/
bool	thread_sleep(t_philo *philo, time_t duration)
{
	time_t	continue_time;

	continue_time = get_time_in_ms() + duration;
	if (continue_time == -1)
		return (error_msg(GET_TIME_ERR));
	while (get_time_in_ms() < continue_time)
	{
		usleep (100);
		if (has_simulation_stopped(philo->table) == true)
		{
			if (philo->status == EATING)
				put_down_forks(philo);
			return (false);
		}
	}
	return (true);
}

static void	*one_philo_routine(t_philo *philo)
{
	pthread_mutex_lock(philo->l_fork);
	print_philo_status_msg(philo, GOT_LEFT_FORK);
	if (thread_sleep(philo, philo->table->time_to_die) == false)
	{
		pthread_mutex_unlock(philo->l_fork);
		return (NULL);
	}
	print_philo_status_msg(philo, DIED);
	pthread_mutex_unlock(philo->l_fork);
	return (NULL);
}

/*
	@function
	It is called after a thread being created in start_simulation(). And it
	must be declared as "void	*<function_name>(void *<parameter_name>)"

	@logic
	Firstly, it will initialize last_eat time of each philosopher to
	simulation's start_time.
	If there is only one philosopher, it will call one_philo_routine();
	For avoiding fork racing, when there is more than one philosopher,
	the philosophers with odd IDs will do think_routine first;
	the philosophers with even IDs will start eat first.
*/
void	*routine(void *data)
{
	t_philo	*philo;

	philo = (t_philo *)data;
	pthread_mutex_lock(&philo->philo_lock);
	philo->last_eat = philo->table->start_time;
	pthread_mutex_unlock(&philo->philo_lock);
	if (philo->table->philo_nb == 1)
		return (one_philo_routine(philo));
	else if (philo->id % 2 != 0)
	{
		if (philo_thinking(philo) == false)
			return (NULL);
		if (thread_sleep(philo, philo->table->time_to_eat / 2) == false)
			return (NULL);
	}
	while (has_simulation_stopped(philo->table) == false)
	{
		if (philo_eating(philo) == false)
			break ;
		if (philo_sleeping(philo) == false)
			break ;
		if (philo_thinking(philo) == false)
			break ;
	}
	return (NULL);
}
