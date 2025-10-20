/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcili <buket.cili@student.42.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/01 12:10:33 by bcili             #+#    #+#             */
/*   Updated: 2025/10/20 18:40:21 by bcili            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	cleanup(t_data *data, t_philo *philos)
{
	int	i;

	i = 0;
	while (i < data->num_philos)
	{
		pthread_mutex_destroy(&data->forks[i]);
		i++;
	}
	pthread_mutex_destroy(&data->print_mutex);
	free(data->forks);
	free(philos);
}

static int	start_threads(t_philo *philos, pthread_t *monitor)
{
	int	i;

	i = 0;
	while (i < philos[0].data->num_philos)
	{
		if (pthread_create(&philos[i].thread, NULL,
				philo_routine, &philos[i]))
			return (1);
		i++;
	}
	if (pthread_create(monitor, NULL, monitor_routine, philos))
		return (1);
	return (0);
}

static void	wait_threads(t_philo *philos, pthread_t *monitor, t_data *data)
{
	int	i;

	i = 0;
	while (i < data->num_philos)
	{
		pthread_join(philos[i].thread, NULL);
		i++;
	}
	pthread_join(*monitor, NULL);
}

int	main(int argc, char **argv)
{
	t_data		data;
	t_philo		*philos;
	pthread_t	monitor;

	if (argc != 5 && argc != 6)
		return (error_exit("Wrong number of arguments"));
	if (init_data(&data, argc, argv))
		return (1);
	philos = init_philos(&data);
	if (!philos)
		return (error_exit("Malloc failed"));
	data.start_time = get_timestamp_ms();
	if (start_threads(philos, &monitor))
		return (1);
	wait_threads(philos, &monitor, &data);
	cleanup(&data, philos);
	return (0);
}
