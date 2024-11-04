/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jingwu <jingwu@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 10:27:36 by jingwu            #+#    #+#             */
/*   Updated: 2024/11/04 10:27:36 by jingwu           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/*
	The function:
	Launches the simulation by creating a grim reaper thread as well as
	one thread for each philosopher.

	@return
	returns true if the simulation was successfully started;
	return false if there was an error.
*/
static bool	start_simulation(t_table *table)
{
	unsigned int	i;

	table->start_time = get_time_in_ms() + (table->philo_nb * 10 * 2);
	i = 0;
	while (i < table->philo_nb)
	{
		if (pthread_create(&table->philos[i]->thread, NULL,
			&routine, table->philos[i]) != 0)//underway philosopher
		{
			error_null(CREATE_THREAD_ERR, table);
			return (false);
		}
		i++;
	}
	if (table->philo_nb > 1)
	{
		if (pthread_create(&table->monitor, NULL,
			&mointor, table) != 0)//underway grim_reaper
		{
			error_null(CREATE_THREAD_ERR, table);
			return (false);
		}
	}
	return (true);
}

static void	stop_simulation(t_table *table)
{
	unsigned int	i;

	i = 0;
	while (i < table->philo_nb)
	{
		pthread_join((table->philos[i])->thread, NULL);
		i++;
	}
	if (table->philo_nb > 1)
		pthread_join(table->monitor, NULL);
	if (DEBUG_FORMAT == true && table->each_eat_times != -1)
		write_outcome(table);//underway
	destroy_mutex(table);//underway
	free_table(table);
}

int	main(int ac, char **av)
{
	t_table	*table;

	if (ac == 5 || ac == 6)
	{
		if(!validate_args(ac, av)) // Done
			return (EXIT_FAILURE);
		table = init_table(ac, av, 1);// Done
		if (!table)
			return (EXIT_FAILURE);
		if (!start_simulation(table))//underway
			return (EXIT_FAILURE);
		stop_simulation(table);//underway
		return (EXIT_SUCCESS);
	}
	else if (ac < 5)
		putstr_fd(TOO_LESS, 2);
	else
		putstr_fd(TOO_MANY, 2);
	return (EXIT_FAILURE);
}
