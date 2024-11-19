/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine_actions.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jingwu <jingwu@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 08:54:24 by jingwu            #+#    #+#             */
/*   Updated: 2024/11/19 16:42:30 by jingwu           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/*
	@function
	The function is called in philo_eating(). The philosopher will lock the
	left and right forks before eating. After picking up each fork, it needs
	to check if the simulation stopped or not. If stopped, then needs to put
	the picked up fork(s) down.

	@return
	True: when the philospher picks up two forks;
	False: the simulation stops.
*/
static bool	pick_up_forks(t_philo *philo)
{
	pthread_mutex_lock(philo->l_fork);
	if (has_simulation_stopped(philo->table) == true)
	{
		pthread_mutex_unlock(philo->l_fork);
		return (false);
	}
	print_philo_status_msg(philo, GOT_LEFT_FORK);
	pthread_mutex_lock(philo->r_fork);
	if (has_simulation_stopped(philo->table) == true)
	{
		put_down_forks(philo);
		return (false);
	}
	print_philo_status_msg(philo, GOT_LEFT_FORK);
	return (true);
}

/*
	@function
	During the simulation, update philosopher's status. And print the philo
	status message out.
	If update the status to EATING, then need to update last_eat time to
	the current time.
*/
static bool	update_philo_status(t_philo *philo, t_status status)
{
	pthread_mutex_lock(&(philo->philo_lock));
	philo->status = status;
	if (status == EATING)
	{
		philo->last_eat = get_time_in_ms();
		if (philo->last_eat == -1)
			return (error_msg(GET_TIME_ERR));
	}
	pthread_mutex_unlock(&(philo->philo_lock));
	print_philo_status_msg(philo, status);
	return (true);
}

bool	philo_eating(t_philo *philo)
{
	if (pick_up_forks(philo) == false)
		return (false);
	if (update_philo_status(philo, EATING) == false)
		return (false);
	if (thread_sleep(philo, philo->table->time_to_eat) == false)
		return (false);
	put_down_forks(philo);
	pthread_mutex_lock(&philo->philo_lock);
	philo->meals_eaten++;
	pthread_mutex_unlock(&philo->philo_lock);
	return (true);
}

bool	philo_sleeping(t_philo *philo)
{
	if (update_philo_status(philo, SLEEPING) == false)
		return (false);
	if (thread_sleep(philo, philo->table->time_to_sleep) == false)
		return (false);
	return (true);
}

bool	philo_thinking(t_philo *philo)
{
	if (update_philo_status(philo, THINKING) == false)
		return (false);
	return (true);
}
