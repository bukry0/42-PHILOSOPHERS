/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcili <buket.cili@student.42.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/01 12:11:07 by bcili             #+#    #+#             */
/*   Updated: 2025/10/23 00:57:03 by bcili            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void take_forks(t_philo *p)
{
	int left = p->id - 1;
	int right = p->id % p->data->num_philos;

	if (p->id % 2 == 0)
	{
		pthread_mutex_lock(&p->data->forks[right]);
		print_status(p, "has taken a fork");
		pthread_mutex_lock(&p->data->forks[left]);
		print_status(p, "has taken a fork");
	}
	else
	{
		pthread_mutex_lock(&p->data->forks[left]);
		print_status(p, "has taken a fork");
		pthread_mutex_lock(&p->data->forks[right]);
		print_status(p, "has taken a fork");
	}
}

void	put_forks(t_philo *p)
{
	pthread_mutex_unlock(&p->data->forks[p->id - 1]);
	pthread_mutex_unlock(&p->data->forks[p->id % p->data->num_philos]);
}

static void	*one_philo_routine(void *arg)
{
	t_philo	*p;

	p = (t_philo *)arg;
	print_status(p, "is thinking");
	usleep((p->data->time_to_die) * 1000);
	print_status(p, "died");
	set_value_with_mutex(p, 0, 1);
	return (NULL);
}

// static void	*one_philo_routine(void *arg)
// {
// 	t_philo	*p;

// 	p = (t_philo *)arg;
// 	print_status(p, "is thinking");
// 	usleep((p->data->time_to_die) * 1000);
// 	print_status(p, "died");
// 	set_value_with_mutex(p, 0, 1);
// 	return (NULL);
// }

void	*philo_routine(void *arg)
{
	t_philo	*p;
	long	start_time_eat;
	long	start_time_sleep;

	p = (t_philo *)arg;
	if (p->data->num_philos == 1)
		return (one_philo_routine(arg));
	if (p->id % 2 != 0)
		usleep(1000);
	while (!(int)get_value_with_mutex(p, 0) && (p->data->must_eat_count == -1
			|| (int)get_value_with_mutex(p, 1) < p->data->must_eat_count))
	{
		print_status(p, "is thinking");
		take_forks(p);
		set_value_with_mutex(p, 2, get_timestamp_ms());
		print_status(p, "is eating");
		start_time_eat = get_timestamp_ms();
		while ((get_timestamp_ms() - start_time_eat) < p->data->time_to_eat)
			usleep(500);
		set_value_with_mutex(p, 1, (int)get_value_with_mutex(p, 1) + 1);
		if (p->data->must_eat_count != -1
			&& (int)get_value_with_mutex(p, 1) >= p->data->must_eat_count)
			return (put_forks(p), NULL);
		put_forks(p);
		print_status(p, "is sleeping");
		start_time_sleep = get_timestamp_ms();
		while ((get_timestamp_ms() - start_time_sleep) < p->data->time_to_sleep)
			usleep(500);
	}
	return (NULL);
}
