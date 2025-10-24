/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mutex_functions.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcili <bcili@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/20 20:53:41 by bcili             #+#    #+#             */
/*   Updated: 2025/10/24 15:50:13 by bcili            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	print_status(t_philo *p, char *status)
{
	pthread_mutex_lock(&p->data->print_mutex);
	if (!(int)get_value_with_mutex(p, 0))
		printf("%ld %d %s\n", get_timestamp_ms() - p->data->start_time,
			p->id, status);
	pthread_mutex_unlock(&p->data->print_mutex);
}

long	get_value_with_mutex(t_philo *p, int index)
{
	long	value;

	value = 0;
	if (index == 0)
	{
		pthread_mutex_lock(&p->data->dead_mutex);
		value = (long)p->data->dead;
		pthread_mutex_unlock(&p->data->dead_mutex);
	}
	else if (index == 1)
	{
		pthread_mutex_lock(&p->data->eat_mutex);
		value = (long)p->eat_count;
		pthread_mutex_unlock(&p->data->eat_mutex);
	}
	else if (index == 2)
	{
		pthread_mutex_lock(&p->data->meal_mutex);
		value = p->last_meal;
		pthread_mutex_unlock(&p->data->meal_mutex);
	}
	return (value);
}

void	set_value_with_mutex(t_philo *p, int index, long new_value)
{
	if (index == 0)
	{
		pthread_mutex_lock(&p->data->dead_mutex);
		p->data->dead = (int)new_value;
		pthread_mutex_unlock(&p->data->dead_mutex);
	}
	else if (index == 1)
	{
		pthread_mutex_lock(&p->data->eat_mutex);
		p->eat_count = (int)new_value;
		pthread_mutex_unlock(&p->data->eat_mutex);
	}
	else if (index == 2)
	{
		pthread_mutex_lock(&p->data->meal_mutex);
		p->last_meal = new_value;
		pthread_mutex_unlock(&p->data->meal_mutex);
	}
}
