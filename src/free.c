/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jingwu <jingwu@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 10:27:10 by jingwu            #+#    #+#             */
/*   Updated: 2024/11/04 10:27:10 by jingwu           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/*
	@what the function does?
	Frees table memory, prints the passed error message.

	@return
	return a NULL pointer.

	@usage
	Use for error management during initialization.
*/
void	*error(char *str, t_table *table)
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
	if (table->fork_locks)
		free(table->fork_locks);
	if (table->philos)
	{
		i = 0;
		while (i < table->philo_nb)
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
