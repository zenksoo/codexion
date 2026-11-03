/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amissa <amissa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/30 22:31:05 by amissa            #+#    #+#             */
/*   Updated: 2026/11/03 00:43:34 by amissa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"
#include <pthread.h>
#include <stdlib.h>

int	is_stoped(t_table *table)
{
	int stopped;

	pthread_mutex_lock(&table->state_lock);
	stopped = table->stop;
	pthread_mutex_unlock(&table->state_lock);
	return stopped;
}

int check_coder(t_table *table, int idx, int *done)
{
	long long	last;
	int			count;

	pthread_mutex_lock(&table->coders[idx]->lock);
	last = table->coders[idx]->last_compile_start;
	count = table->coders[idx]->compile_count;
	pthread_mutex_unlock(&table->coders[idx]->lock);

	if (now_ms() - last >= table->cli_args->t_burnout)
		return 1;

	if (count < table->cli_args->compiles_required)
		*done = 0;
	return 0;
}

void	*monitor_routine(void *arg)
{
	t_table *table;
	int	coder_idx;
	int done;

	table = (t_table *)arg;
	wait_for_start(table);
	while (!is_stoped(table)) {
		coder_idx = -1;
		done = 1;
		while (++coder_idx < table->n)
		{
			if (check_coder(table, coder_idx, &done))
			{
				stop_simulation(table);
				pthread_mutex_lock(&table->log_lock);
				printf("%lld %d burnburned out\n", now_ms() - table->start_time, coder_idx + 1);
				pthread_mutex_unlock(&table->log_lock);
				return (NULL);
			}
		}
		if (done)
			return (stop_simulation(table), NULL);
		usleep(500);
	}
	return (NULL);
}
