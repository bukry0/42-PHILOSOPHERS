/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcili <buket.cili@student.42.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/01 12:10:53 by bcili             #+#    #+#             */
/*   Updated: 2025/10/24 00:11:01 by bcili            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	all_ate_enough(t_philo *philos, t_data *data)
{
	int	i;

	if (data->must_eat_count == -1)
		return (0);
	i = 0;
	pthread_mutex_lock(&data->eat_mutex);
	while (i < data->num_philos)
	{
		if (philos[i].eat_count < data->must_eat_count)
			return (pthread_mutex_unlock(&data->eat_mutex), 0);
		i++;
	}
	pthread_mutex_unlock(&data->eat_mutex);
	return (1);
}

static void	print_sim_end(t_data *data)
{
	pthread_mutex_lock(&data->print_mutex);
	printf("All philosophers ate at least");
	printf(" %d ", data->must_eat_count);
	printf("times. Simulation ended successfully.\n");
	pthread_mutex_unlock(&data->print_mutex);
	// pthread_mutex_lock(&data->dead_mutex);
	// data->dead = 1;
	// pthread_mutex_unlock(&data->dead_mutex);
}

static void	check_philo_death(t_philo *p, t_data *data)
{
	long	now;

	pthread_mutex_lock(&data->meal_mutex);
	now = get_timestamp_ms();
	if ((now - p->last_meal) >= data->time_to_die)
	{
		pthread_mutex_lock(&data->dead_mutex);
		if (!data->dead)
		{
			data->dead = 1;
			pthread_mutex_lock(&data->print_mutex);
			printf("%ld %d died\n", now - data->start_time, p->id);
			pthread_mutex_unlock(&data->print_mutex);
		}
		pthread_mutex_unlock(&data->dead_mutex);
	}
	pthread_mutex_unlock(&data->meal_mutex);
}

static int	check_all_ate(t_philo *philos, t_data *data)
{
	if (all_ate_enough(philos, data))
	{
		print_sim_end(data);
		return (1);
	}
	return (0);
}

void	*monitor_routine(void *arg)
{
	t_philo	*philos;
	t_data	*data;
	int		i;

	philos = (t_philo *)arg;
	data = philos[0].data;
	while (!(int)get_value_with_mutex(&philos[0], 0))
	{
		i = 0;
		while (i < data->num_philos && !(int)get_value_with_mutex(&philos[0], 0))
		{
			check_philo_death(&philos[i], data);
			i++;
		}
		if ((int)get_value_with_mutex(&philos[0], 0)
			|| check_all_ate(philos, data))
			return (NULL);
		usleep(data->time_to_die / 10 * 1000);
	}
	return (NULL);
}
