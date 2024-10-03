#include "philo.h"


/*
	@what the function does?
	Frees table memory, prints the passed error message.

	@return
	return a NULL pointer.

	@usage
	Use for error management during initialization.
*/
void	*error_null(char *str, t_table *table)
{
	if (table)
		free_table(table);
	putstr_fd(str, 2);
	return (NULL);
}

/*
	@what the function does?
	Frees alloacted table memory.

	@return
	return a NULL pointer.
*/
void	*free_table(t_table *table)
{
	unsigned int	i;

	if (!table)
		return (NULL);
	if (table->fork_lock)
		free(table->fork_lock);
	if (table->philos)
	{
		i = 0;
		while (i < table->philo_amount)
		{
			if (table->philos[i])
				free(table->philos[i]);
			i++;
		}
		free(table->philos);
	}
	free(table);
	return (NULL);
}