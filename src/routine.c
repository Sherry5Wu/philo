/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jingwu <jingwu@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 10:27:04 by jingwu            #+#    #+#             */
/*   Updated: 2024/12/02 09:27:52 by jingwu           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/*
	@function
	Set philosopher's status to certain value. Don't need print message out.
*/
void	update_philo_state(t_philo *philo, int state)
{
	pthread_mutex_lock(&philo->philo_lock);
	philo->state = state;
	pthread_mutex_unlock(&philo->philo_lock);
}

int	get_philo_state(t_philo *philo)
{
	int	state;

	pthread_mutex_lock(&philo->philo_lock);
	state = philo->state;
	pthread_mutex_unlock(&philo->philo_lock);
	return (state);
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
bool	thread_sleep(t_philo *philo, size_t duration)
{
	time_t	continue_time;
	time_t	current_time;

	current_time = get_time_in_ms();
	if (current_time == -1)
		return (error_msg(GET_TIME_ERR));
	continue_time = current_time + duration;
	while (current_time < continue_time)
	{
		usleep (100);
		if (get_philo_state(philo) == OVER)
		{
			pthread_mutex_lock(&philo->philo_lock);
			if (philo->action == EATING)
				put_down_forks(philo);
			pthread_mutex_unlock(&philo->philo_lock);
			return (false);
		}
		current_time = get_time_in_ms();
		if (current_time == -1)
			return (error_msg(GET_TIME_ERR));
	}
	return (true);
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
	
	@what is "__thread"?
	Use the __thread or thread_local keyword to ensure that error_code is unique to
	each thread or properly synchronized if shared.
*/
void	*routine(void *data)
{
	t_philo				*philo;
	static __thread int	error_code;

	philo = (t_philo *)data;
	error_code = -1;
	if (philo->table->philo_nb != 1 && philo->id % 2 != 0)
	{
		if (philo_thinking(philo)== false)
			return (&error_code);
		if (thread_sleep(philo, philo->table->time_to_eat / 2) == false)
			return (&error_code);
	}
	while (get_philo_state(philo) != OVER)
	{
		if (philo_eating_sleeping(philo) == false)
			break ;
		if (philo_thinking(philo) == false)
			break ;
	}
	return (NULL);
}
