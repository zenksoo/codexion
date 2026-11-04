/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amissa <amissa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/30 22:31:05 by amissa            #+#    #+#             */
/*   Updated: 2026/11/04 10:44:02 by amissa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

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

int	check_coder(t_table *table, int idx, int *done)
{
	long long	last;
	int			count;

	pthread_mutex_lock(&table->coders[idx]->lock);
	// last = timespec_to_ms(*table->coders[idx]->last_compile_start);
	last = table->coders[idx]->last_compile_start;
	count = table->coders[idx]->compile_count;
	pthread_mutex_unlock(&table->coders[idx]->lock);
	if (now_ms() - last >= table->args[T_BURNOUT])
		return (1);
	if (count < table->args[C_REQUIRED])
		*done = 0;
	return (0);
}

void	*monitor_routine(void *arg)
{
	t_table	*table;
	int		coder_idx;
	int		done;

	table = (t_table *)arg;
	if (wait_for_start(table))
		return (NULL);
	while (!is_stoped(table)) {
		coder_idx = -1;
		done = 1;
		while (++coder_idx < table->n)
		{
			if (check_coder(table, coder_idx, &done))
			{
				stop_simulation(table);
				if (radio_monitor_log(table->coders[coder_idx], "burned out"))
					return NULL;
				return (NULL);
			}
		}
		if (done)
			return (stop_simulation(table), NULL);
		usleep(500);
	}
	return (NULL);
}
