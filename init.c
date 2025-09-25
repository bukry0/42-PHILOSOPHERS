/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcili <bcili@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/01 12:11:29 by bcili             #+#    #+#             */
/*   Updated: 2025/09/01 20:38:05 by bcili            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	print_death(t_philo *p, t_data *data, long now)
{
	pthread_mutex_lock(&data->print_mutex);
	if (!data->dead)
	{
		data->dead = 1;
		printf("%ld %d died\n", now - p->last_meal, p->id);
	}
	pthread_mutex_unlock(&data->print_mutex);
}

t_philo	*init_philos(t_data *data)
{
	t_philo	*philos;
	int		i;

	philos = malloc(sizeof(t_philo) * data->num_philos);
	if (!philos)
	{
		error_exit("Malloc failed");
		return (NULL);
	}
	i = 0;
	while (i < data->num_philos)
	{
		philos[i].id = i + 1;
		philos[i].eat_count = 0;
		philos[i].last_meal = get_timestamp_ms();
		philos[i].data = data;
		i++;
	}
	return (philos);
}

int	init_mutexes(t_data *data)
{
	int	i;

	data->forks = malloc(sizeof(pthread_mutex_t) * data->num_philos);
	if (!data->forks)
		return (error_exit("Malloc failed"));
	i = 0;
	while (i < data->num_philos)
		pthread_mutex_init(&data->forks[i++], NULL);
	pthread_mutex_init(&data->print_mutex, NULL);
	return (0);
}
