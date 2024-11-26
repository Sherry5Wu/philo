/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jingwu <jingwu@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 10:27:26 by jingwu            #+#    #+#             */
/*   Updated: 2024/11/04 10:27:26 by jingwu           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	ft_strlen(char *str)
{
	int	i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

/*
	It will always return false.
*/
void	putstr_fd(char	*message, int fd)
{
	if (message)
	{
		write(fd, message, ft_strlen(message));
		write(fd, "\n", 1);
	}
}

int	ft_atoi(const char *str)
{
	int				sign;
	long long int	integer;
	long long int	check;

	sign = 1;
	integer = 0;
	while ((*str >= 9 && *str <= 13) || *str == 32)
		str++;
	if (*str == '+' || *str == '-')
		if (*str++ == '-')
			sign = -1;
	while (*str >= '0' && *str <= '9')
	{
		check = integer * 10 + (*str - '0');
		if ((check / 10) != integer && sign == -1)
			return (0);
		if ((check / 10) != integer && sign == 1)
			return (-1);
		integer = check;
		str++;
	}
	return (((int)integer) * sign);
}
