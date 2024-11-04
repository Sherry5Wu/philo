/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jingwu <jingwu@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 13:57:23 by jingwu            #+#    #+#             */
/*   Updated: 2024/11/04 14:01:38 by jingwu           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

bool	is_simulation_stopped(t_table *table)
{
	bool	sign;

	sign = false;
	pthread_mutex_lock(&table->sim_stop_lock);
	if (table->simulation_stop == true)
		sign = true;
	pthread_mutex_unlock(&table->sim_stop_lock);
	return (sign);
}
