/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amissa <amissa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/30 22:31:05 by amissa            #+#    #+#             */
/*   Updated: 2026/11/05 22:15:12 by amissa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	radio_monitor_log(t_coder *coder, char *msg)
{
	t_table	*table;

	table = coder->table;
	if (pthread_mutex_lock(&table->log_lock))
		return (1);
	printf("%lld %d %s\n", now_ms() - table->start_time, coder->id, msg);
	fflush(stdout);
	if (pthread_mutex_unlock(&table->log_lock))
		return (1);
	return (0);
}

int	is_stoped(t_table *table)
{
	int	stopped;

	if (pthread_mutex_lock(&table->state_lock))
		return (1);
	stopped = table->stop;
	if (pthread_mutex_unlock(&table->state_lock))
		return (1);
	return (stopped);
}

int	is_coder_burnout(t_table *table, int idx, int *done)
{
	long long	last;
	int			count;

	pthread_mutex_lock(&table->coders[idx]->lock);
	last = table->coders[idx]->last_compile_start;
	count = table->coders[idx]->compile_count;
	pthread_mutex_unlock(&table->coders[idx]->lock);
	if (now_ms() - last >= table->args[T_BURNOUT])
		return (1);
	if (count < table->args[C_REQUIRED])
		*done = 0;
	return (0);
}

int	check_coders_burnout(t_table *table, int *done)
{
	int	idx;

	idx = -1;
	while (++idx < table->n)
	{
		if (is_coder_burnout(table, idx, done))
		{
			table->stop = 1;
			pthread_mutex_unlock(&table->state_lock);
			radio_monitor_log(table->coders[idx], "burned out");
			return (1);
		}
	}
	return (0);
}

void	*monitor_routine(void *arg)
{
	t_table	*table;
	int		done;

	table = (t_table *)arg;
	wait_for_start(table);
	while (!is_stoped(table))
	{
		done = 1;
		pthread_mutex_lock(&table->state_lock);
		if (check_coders_burnout(table, &done))
			return (wake_coders(table), NULL);
		if (done)
		{
			table->stop = 1;
			pthread_mutex_unlock(&table->state_lock);
			return (wake_coders(table), NULL);
		}
		pthread_mutex_unlock(&table->state_lock);
		usleep(500);
	}
	return (NULL);
}
