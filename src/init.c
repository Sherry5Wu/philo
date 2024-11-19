/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jingwu <jingwu@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 10:27:21 by jingwu            #+#    #+#             */
/*   Updated: 2024/11/04 10:27:21 by jingwu           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/*
	Using (id+1)%(nb_philo) will nicely make a cycle.
	The logic to assign forks: (consider the left fork is their own fork)
	-	the odd id philo will first pick the fork on their right, then pick up
		the fork on the left (their own fork);
	-	the even id philo will first pick the for on their left(their own fork)
		then pick the fork on the right.
*/
static void	assign_forks(pthread_mutex_t *forks, t_philo *philo)
{
	if (philo->id % 2)
	{
		philo->l_fork = &forks[(philo->id + 1) % (philo->table->philo_nb)];
		philo->r_fork = &forks[philo->id];
	}
	else
	{
		philo->l_fork = &forks[philo->id];
		philo->r_fork = &forks[(philo->id + 1) % (philo->table->philo_nb)];
	}
}

/*
	pthread_mutex_init() return:
	0: successfully;
	others: failed;
*/
static t_philo	**init_philos(t_table *table)
{
	t_philo			**philos;
	size_t			i;

	philos = (t_philo **)malloc(sizeof(t_philo *) * (table->philo_nb));
	if (!philos)
		return (error_msg_null(MALLOC_ERR));
	i = 0;
	while (i < table->philo_nb)
	{
		philos[i] = malloc(sizeof(t_philo));
		if (philos[i])
			return (error_msg_null(MALLOC_ERR));
		if (pthread_mutex_init(&philos[i]->philo_lock, 0) != 0)
			return (error_msg_null(CREATE_MUTEX_ERR));
		philos[i]->table = table;
		philos[i]->id = i + 1;
		philos[i]->meals_eaten = 0;
		philos[i]->status = THINKING;
		assign_forks(table->fork_locks, philos[i]);
		i++;
	}
	return (philos);
}

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
	size_t			i;

	i = 0;
	forks = malloc(sizeof(pthread_mutex_t) * table->philo_nb);
	if (!forks)
		return (NULL);
	while (i < table->philo_nb)
	{
		if (pthread_mutex_init(&forks[i], 0) != 0)
		{
			ft_free(forks);
			forks = NULL;
			return (NULL);
		}
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
static bool	init_global_mutexes(t_table *table)
{
	table->fork_locks = init_forks(table);
	if (!table->fork_locks)
		return (false);
	if (pthread_mutex_init(&(table->sim_stop_lock), 0) != 0)
		return (false);
	if (pthread_mutex_init(&(table->write_lock), 0) != 0)
		return (false);
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
	if (!init_global_mutexes(table))
		return (error_msg(INIT_MUTEX_ERR));
	table->philos = init_philos(table);
	if (!table->philos)
		return (error_msg(INIT_PHILO_ERR));
	return (true);
}
