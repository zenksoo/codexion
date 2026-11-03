/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amissa <amissa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/30 18:24:05 by amissa            #+#    #+#             */
/*   Updated: 2026/11/03 20:29:00 by amissa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"
#include <bits/time.h>
#include <pthread.h>
#include <stdlib.h>
#include <time.h>

void	wait_dongle(t_dongle *d)
{
	struct timespec	ts;
	long long		wake;

	wake = now_ms() + 5;
	if (!d->busy)
		wake = d->available_at;
	ts.tv_sec = wake / 1000;
	ts.tv_nsec = (wake % 1000) * 1000000;
	pthread_cond_timedwait(&d->cond, &d->lock, &ts);
}

int	compile(t_coder *coder)
{
	t_table	*table;

	table = coder->table;
	if (take_dongles(coder))
		return (1);
	pthread_mutex_lock(&coder->lock);
	if (clock_gettime(CLOCK_MONOTONIC, coder->last_compile_start))
		return (1);
	pthread_mutex_unlock(&coder->lock);
	if (radio_log(coder, "is compiling")
		|| simulation_sleep(table, coder->table->args[T_COMPILE]))
		return (release_dongles(coder), 1);
	pthread_mutex_lock(&coder->lock);
	coder->compile_count++;
	pthread_mutex_unlock(&coder->lock);
	release_dongles(coder);
	return (0);
}

void	*coder_routine(void *arg)
{
	t_coder	*coder;
	t_table	*table;

	coder = arg;
	table = coder->table;
	wait_for_start((void *)coder->table);
	while (coder->id != coder->table->ready_coder_id)
		;
	pthread_mutex_lock(&coder->table->state_lock);
	coder->table->ready_coder_id++;
	pthread_mutex_unlock(&coder->table->state_lock);
	while (!is_stoped(coder->table))
	{
		if (compile(coder))
			return (NULL);
		if (radio_log(coder, "is debugging")
			|| simulation_sleep(table, table->args[T_DEBUG]))
			return (NULL);
		if (radio_log(coder, "is refactoring")
			|| simulation_sleep(table, table->args[T_REFACTOR]))
			return (NULL);
	}
	return (NULL);
}
