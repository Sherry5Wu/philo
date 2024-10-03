
#include "philo.h"

/*
	@The function does:
	Initializes the "dining table", the data structure containing
	all of the program's parameters.

	@return
	Returns a pointer to the allocated table structure, or NULL if
	an error occured during initialization.

	@why pass i here?
	Because of the norm rules, one funcion can't be more than 25 lines,
	so passed int i here to reduce the lines.
*/
t_table	*init_table(int ac, char **av, int i)
{
	t_table	*table;

	table = malloc(sizeof(t_table));
	if (!table)
		return (error_null(MALLOC_ERR, 0));
	table->philo_amount = (size_t)ft_atoi(args[1]);
	table->time_to_die = (size_t)ft_atoi(args[2]);
	table->time_to_eat = (size_t)ft_atoi(args[3]);
	table->time_to_sleep = (size_t)ft_atoi(args[4]);
	table->each_eat_times = -1;
	if (ac - 1 == 5)
		table->each_eat_times = (size_t)ft_atoi(args[5]);
	table->philos = init_philos(table); // underway
	if (!table->philos)
		return (error_null(INIT_PHILO_ERR, 0));
	if (!init_global_mutexes(table)) // underway
		return (error_null(INIT_MUTEX_ERR, 0));
	table->sim_stop = false;
	return (table);
}