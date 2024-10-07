
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

	table->start_time = get_time_in_ms() + (table->nb_philo * 2 * 10);//underway
	i = 0;
	while (i < table->nb_philo)
	{
		if (pthread_create(&table->philos[i]->thread, NULL,
			&philosophers, table->philos[i]) != 0)//underway philosopher
		{
			error_null(CREATE_THREAD_ERR, table);
			return (false);
		}
		i++;
	}
	if (table->nb_philo > 1)
	{
		if (pthread_create(&table->grim_reaper, NULL,
			&grim_reaper, table) != 0)//underway grim_reaper
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
	while (i < table->nb_philo)
	{
		pthread_join((table->philos[i])->thread, NULL);
		i++;
	}
	if (table->nb_philo > 1)
		pthread_join(table->grim_reaper, NULL);
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
		table = init_table(ac, av, 1)// Done
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
