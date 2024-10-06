
#include "philo.h"

/*
	Using (id+1)%(nb_philo) will nicely make a cycle.
*/
static void	assign_forks(t_philo *philo)
{
	if (philo->id % 2)
	{
		philo->fork[0] = (philo->id + 1) % (philo->table->nb_philo);
		philo->fork[1] = philo->id;
	}
	else
	{
		philo->fork[0] = philo->id;
		philo->fork[1] = (philo->id + 1) % (philo->table->nb_philo);
	}
}

/*
	pthread_mutex_init() return:
	0: successfully;
	others: failed;
*/
static t_philo	**init_philo(t_table *table)
{
	t_philo			**philos;
	unsigned int	i;

	philos = (t_philo **)malloc(sizeof(t_philo *) * (table->nb_philo + 1));// should plus 1 here???
	if (!philos)
		return (error_null(MALLOC_ERR, 0));
	i = 0;
	while (i < table->nb_philo)
	{
		philos[i] = malloc(sizeoft(t_philo));
		if (philos[i])
			return (error_null(MALLOC_ERR, 0));
		if (pthread_mutex_init(&philos[i]->meal_time_lock, 0) != 0)
			return (error_null(CREATE_MUTEX_ERR, 0));
		philos[i]->table = table;
		philos[i]->id = i;
		philos[i]->meals_eaten = 0;
		assign_forks(philos[i]);
		i++;
	}
	return (philos);
}
/*
	Initializes mutex locks for forks, writing and the stop simulation flag.

	Returns true if the initalizations were successful, false if initilization failed.
*/
static bool	init_global_mutexes(t_table *table)
{
	table->fork_locks = init_forks(table);
	if (!table->fork_locks)
		return (false);
	if (pthread_mutex_init(&(table->sim_stop_lock), 0) != 0)
	{
		error_null(CREATE_MUTEX_ERR, 0);
		return (false);
	}
	if (pthread_mutex_init(&(table->write_lock), 0) != 0)
	{
		error_null(CREATE_MUTEX_ERR, 0);
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
	table->philos = init_philos(table);
	if (!table->philos)
		return (error_null(INIT_PHILO_ERR, 0));
	if (!init_global_mutexes(table))
		return (error_null(INIT_MUTEX_ERR, 0));
	table->sim_stop = false;
	return (table);
}