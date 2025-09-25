/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcili <bcili@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/01 12:11:07 by bcili             #+#    #+#             */
/*   Updated: 2025/09/01 18:53:19 by bcili            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	print_status(t_philo *p, char *status)
{
	pthread_mutex_lock(&p->data->print_mutex);
	if (!p->data->dead)
		printf("%ld %d %s\n",
			get_timestamp_ms() - p->data->start_time,
			p->id, status);
	pthread_mutex_unlock(&p->data->print_mutex);
}

void	take_forks(t_philo *p)
{
	pthread_mutex_lock(&p->data->forks[p->id - 1]);
	print_status(p, "has taken a fork");
	pthread_mutex_lock(&p->data->forks[p->id % p->data->num_philos]);
	print_status(p, "has taken a fork");
}

void	put_forks(t_philo *p)
{
	pthread_mutex_unlock(&p->data->forks[p->id - 1]);
	pthread_mutex_unlock(&p->data->forks[p->id % p->data->num_philos]);
}

void	*philo_routine(void *arg)
{
	t_philo	*p;

	p = (t_philo *)arg;
	if (p->data->num_philos == 1)
	{
		print_status(p, "is thinking");
		usleep((p->data->time_to_die) * 1000);
		print_status(p, "died");
		p->data->dead = 1;
		return (NULL);
	}
	while (!p->data->dead && (p->data->must_eat_count == -1
			|| p->eat_count <= p->data->must_eat_count))
	{
		print_status(p, "is thinking");
		take_forks(p);
		p->last_meal = get_timestamp_ms();
		print_status(p, "is eating");
		usleep((p->data->time_to_eat) * 1000);
		p->eat_count++;
		put_forks(p);
		print_status(p, "is sleeping");
		usleep((p->data->time_to_sleep) * 1000);
	}
	return (NULL);
}
