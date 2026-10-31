/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amissa <amissa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/30 22:31:05 by amissa            #+#    #+#             */
/*   Updated: 2026/10/31 01:16:42 by amissa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"
#include <pthread.h>


int	is_stoped(t_table *table)
{
	int stopped;

	pthread_mutex_lock(&table->state_lock);
	stopped = table->stop;
	pthread_mutex_unlock(&table->state_lock);
	return stopped;
}

void	*monitor_routine(void *arg)
{
	t_table *table;
	int	coder_idx;
	int	all_reach_required_compilation;

	table = (void *)arg;
	wait_for_start(table);
	while (!is_stoped(table)) {
		coder_idx = -1;
		all_reach_required_compilation = 1;
		while (++coder_idx < table->n)
		{
			pthread_mutex_lock(&table->coders[coder_idx].lock);
			if (now_ms() - table->coders[coder_idx].last_compile_start >= table->t_burnout)
			{
				stop_simulation(table); 
				ms_sleep(10, table);
				printf("%lld %d burnburned out\n", now_ms() - table->start_time, coder_idx);
				pthread_mutex_unlock(&table->coders[coder_idx].lock);
				break;
			}
			if (table->coders[coder_idx].compile_count < table->compiles_required)
				all_reach_required_compilation = 0;
			pthread_mutex_unlock(&table->coders[coder_idx].lock);
		}
		if (all_reach_required_compilation)
			return (stop_simulation(table), NULL);
	}
	return (NULL);
}
