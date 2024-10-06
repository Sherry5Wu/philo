
#include "philo.h"

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
