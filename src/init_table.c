/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_table.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jingwu <jingwu@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 13:38:56 by jingwu            #+#    #+#             */
/*   Updated: 2024/11/20 13:38:56 by jingwu           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/*
	@function
	Allocate memory for the fork mutex and initialization them.

	@return
	return a poninter to the fork mutex array,
	or NULL if an error occurd.
*/
static pthread_mutex_t	*init_forks(t_table *table)
{
	pthread_mutex_t	*forks;
	int				i;

	i = 0;
	// printf("philo_nb=%zu\n", table->philo_nb);// for testing!!!!!!!!!!!
	forks = malloc(sizeof(pthread_mutex_t) * table->philo_nb);
	if (!forks)
		return (NULL);
	while (i < (int)table->philo_nb)
	{
		if (pthread_mutex_init(&forks[i], 0) != 0)
		{
			while (--i >= 0)
				pthread_mutex_destroy(&forks[i]);
			ft_free(forks);
			forks = NULL;
			return (NULL);
		}
		// printf("i=%d\n", i);// for testing!!!!!!!!
		i++;
	}
	return (forks);
}
/*
	@function
	Initializes mutex locks for forks, writing and the stop simulation flag.

	@reurn
	true if the initalizations were successful;
	false if initilization failed;
*/
static bool	init_mutexes_for_table(t_table *table)
{
	int	i;

	i = -1;
	table->fork_locks = init_forks(table);
	if (!table->fork_locks)
		return (false);
	if (pthread_mutex_init(&(table->sim_stop_lock), 0) != 0)
	{
		while (++i < (int)table->philo_nb)
			pthread_mutex_destroy(&table->fork_locks[i]);
		return (false);
	}
	if (pthread_mutex_init(&(table->write_lock), 0) != 0)
	{
		while (++i < (int)table->philo_nb)
			pthread_mutex_destroy(&table->fork_locks[i]);
		pthread_mutex_destroy(&table->sim_stop_lock);
		return (false);
	}
	return (true);
}

/*
	@The function does:
	Initializes the "dining table", the data structure containing
	all of the program's parameters.

	@return
	Returns a pointer to the allocated table structure, or NULL if
	an error occured during initialization.
*/
bool	init_table(t_table *table, int ac, char **args)
{
	table->philo_nb = (size_t)ft_atoi(args[1]);
	table->time_to_die = (size_t)ft_atoi(args[2]);
	table->time_to_eat = (size_t)ft_atoi(args[3]);
	table->time_to_sleep = (size_t)ft_atoi(args[4]);
	table->simulation_stop = false;
	table->must_eat_times = -1;
	if (ac - 1 == 5)
		table->must_eat_times = (size_t)ft_atoi(args[5]);
	table->start_time = get_time_in_ms();
	if (!init_mutexes_for_table(table))
		return (error_msg(INIT_MUTEX_ERR));
	table->philos = init_philos(table);
	if (!table->philos)
	{
		destroy_mutexes_of_table(table);
		return (error_msg(INIT_PHILO_ERR));
	}
	return (true);
}
