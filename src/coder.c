/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amissa <amissa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/30 18:24:05 by amissa            #+#    #+#             */
/*   Updated: 2026/11/01 22:28:56 by amissa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"
#include <pthread.h>


int	release_dongle(t_table *table, t_dongle *dongle)
{
	if (!dongle)
		return 1;
	pthread_mutex_lock(&dongle->lock);
	dongle->busy = 0;
	dongle->available_at = now_ms() + table->cooldown;
	pthread_cond_broadcast(&dongle->cond);
	pthread_mutex_unlock(&dongle->lock);
	return 0;
}

void wait_dongle(t_dongle *d)
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

int	take_dongle(t_coder *coder, t_table *table, t_dongle *dongle)
{
	pthread_mutex_lock(&dongle->lock);
	while (!is_stoped(table) && (dongle->busy || now_ms() < dongle->available_at))
		wait_dongle(dongle);
	if (is_stoped(table))
		return (pthread_mutex_unlock(&dongle->lock), 1);
	dongle->busy = 1;
	pthread_mutex_unlock(&dongle->lock);

	protected_log(table,
		coder->id, "has taken a dongle");
	return (0);

}

int take_dongels_and_compile(t_coder *coder)
{
	t_table *table;

	table = coder->table;
	if (coder->id % 2 == 0)
	{
		if (coder->left && take_dongle(coder, table, coder->left))
			return 1;
		if (coder->right && take_dongle(coder, table, coder->right))
			return (release_dongle(table, coder->left), 1);
	}
	else
	{
		if (coder->right && take_dongle(coder, table, coder->right))
			return 1;
		if (coder->left && take_dongle(coder, table, coder->left))
			return (release_dongle(table, coder->right), 1);
	}
	pthread_mutex_lock(&coder->lock);
	coder->last_compile_start = now_ms();
	pthread_mutex_unlock(&coder->lock);

	pthread_mutex_lock(&coder->lock);
	coder->compile_count++;
	pthread_mutex_unlock(&coder->lock);
	protected_log(table, coder->id, "is compiling");
	if (ms_sleep(coder ->table->t_compile, table))
		return (1);
	release_dongle(table, coder->right);
	release_dongle(table, coder->left);
	return 0;
}


void	*coder_routine(void *arg)
{
	t_coder *coder;

	coder = (t_coder *)arg;
	wait_for_start((void *)coder->table);
	while (coder->id != coder->table->ready_coder_id)
		;
	pthread_mutex_lock(&coder->table->state_lock);
	coder->table->ready_coder_id++;
	pthread_mutex_unlock(&coder->table->state_lock);
	while (!is_stoped(coder->table))
	{
		if (take_dongels_and_compile(coder))
			return NULL;

		protected_log(coder->table,
			coder->id, "is debugging");

		if (ms_sleep(coder->table->t_debug, coder->table))
			return NULL;

		protected_log(coder->table,
			coder->id, "is refactoring");
		if (ms_sleep(coder->table->t_refactor, coder->table))
			return NULL;
	}
	return (NULL);
}
