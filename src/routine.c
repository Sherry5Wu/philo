/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jingwu <jingwu@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 10:27:04 by jingwu            #+#    #+#             */
/*   Updated: 2024/11/25 09:53:29 by jingwu           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/*
	@function
	Set philosopher's status to certain value. Don't need print message out.
*/
void	set_philo_status(t_philo *philo, int status)
{
	pthread_mutex_lock(&philo->philo_lock);
	philo->status = status;
	pthread_mutex_unlock(&philo->philo_lock);
}

void	put_down_forks(t_philo *philo)
{
	// printf("Inside put_down_forks\nID=%zu\ntime=%zu\n", philo->id, get_time_in_ms() - philo->table->start_time);
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
bool	thread_sleep(t_philo *philo, size_t duration)
{
	time_t	continue_time;
	time_t	current_time;

	current_time = get_time_in_ms();
	if (current_time == -1)
		return (error_msg(GET_TIME_ERR));
	continue_time = current_time + duration;
	while ( current_time < continue_time)
	{
		usleep (1000);
		if (has_simulation_stopped(philo->table) == true)
		{
			if (philo->status == EATING)
				put_down_forks(philo);
			return (false);
		}
		current_time = get_time_in_ms();
	}
	return (true);
}

static void	*one_philo_routine(t_philo *philo)
{
	pthread_mutex_lock(philo->l_fork);
	print_philo_status_msg(philo, GOT_LEFT_FORK);
	printf("----------------------------4--->\n");// for testing!!!!!!!!!!!!!
	if (thread_sleep(philo, philo->table->time_to_die) == false)
	{
		pthread_mutex_unlock(philo->l_fork);
		return (NULL);
	}
	print_philo_status_msg(philo, DIED);
	usleep(philo->table->time_to_die * 1000);
	pthread_mutex_unlock(philo->l_fork);
	printf("leaving one_philo_routine\n");// for testing!!!!!!!!!!!!!
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
	if (philo->table->philo_nb == 1)
		return (one_philo_routine(philo));
	if (philo->id % 2 != 0)
	{
		if (philo_thinking(philo) == false)
			return (NULL);
		if (thread_sleep(philo, philo->table->time_to_eat / 2) == false)
			return (NULL);
	}
	while (has_simulation_stopped(philo->table) == false)
	{
		if (philo_eating_sleeping(philo) == false)
			break ;
		if (philo_thinking(philo) == false)
			break ;
	}
	return (NULL);
}
