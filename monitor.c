/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcili <buket.cili@student.42.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/01 12:10:53 by bcili             #+#    #+#             */
/*   Updated: 2025/10/20 23:38:09 by bcili            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	all_ate_enough(t_philo *philos, t_data *data)
{
	int	i;

	if (data->must_eat_count == -1)
		return (0);
	i = 0;
	while (i < data->num_philos)
	{
		if ((int)get_value_with_mutex(&philos[i], 1) < data->must_eat_count)
			return (0);
		i++;
	}
	return (1);
}

static void	print_sim_end(t_data *data)
{
	pthread_mutex_lock(&data->print_mutex);
	printf("All philosophers ate at least");
	printf(" %d ", data->must_eat_count);
	printf("times. Simulation ended successfully.\n");
	pthread_mutex_unlock(&data->print_mutex);
	pthread_mutex_lock(&data->dead_mutex);
	data->dead = 1;
	pthread_mutex_unlock(&data->dead_mutex);
}

static void	check_philo_death(t_philo *p, t_data *data)
{
	long	now;
	long	last_meal_time;

	now = get_timestamp_ms();
	last_meal_time = get_value_with_mutex(p, 2);
	if ((now - last_meal_time) > data->time_to_die)
	{
		pthread_mutex_lock(&data->print_mutex);
		print_death(p, data, now);
		pthread_mutex_unlock(&data->print_mutex);
		set_value_with_mutex(p, 0, 1); // dead = 1
		pthread_exit(NULL);
	}
}

// static void	check_philo_death(t_philo *p, t_data *data)
// {
// 	long	now;

// 	pthread_mutex_lock(&data->print_mutex);
// 	now = get_timestamp_ms();
// 	if ((now - get_value_with_mutex(p, 2)) > data->time_to_die)
// 	{
// 		pthread_mutex_unlock(&data->print_mutex);
// 		print_death(p, data, now);
// 		pthread_exit(NULL);
// 	}
// 	pthread_mutex_unlock(&data->print_mutex);
// }

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
		if (check_all_ate(philos, data))
			return (NULL);
		usleep(1000);
	}
	return (NULL);
}

// void	*monitor_routine(void *arg)
// {
// 	t_philo	*philos;
// 	t_data	*data;
// 	int		i;

// 	philos = (t_philo *)arg;
// 	data = philos[0].data;
// 	while (!data->dead)
// 	{
// 		i = 0;
// 		while (i < data->num_philos && !data->dead)
// 			check_philo_death(&philos[i++], data);
// 		if (check_all_ate(philos, data))
// 			return (NULL);
// 		usleep(1000);
// 	}
// 	return (NULL);
// }

/*

void	*monitor_routine(void *arg)
{
	t_philo	*philos;
	t_data	*data;
	int		i;
	long	now;

	philos = (t_philo *)arg;
	data = philos[0].data;
	while (!data->dead)
	{
		i = 0;
		while (i < data->num_philos && !data->dead)
		{
			pthread_mutex_lock(&data->print_mutex);
			now = get_timestamp_ms();
			if ((now - philos[i].last_meal) > data->time_to_die)
			{
				pthread_mutex_unlock(&data->print_mutex);
				print_death(&philos[i], data, now);
				return (NULL);
			}
			pthread_mutex_unlock(&data->print_mutex);
			i++;
		}
		if (all_ate_enough(philos, data))
		{
			print_sim_end(data);
			return (NULL);
		}
		usleep(1000);
	}
	return (NULL);
}
*/