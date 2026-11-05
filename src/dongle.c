/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amissa <amissa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/03 19:56:16 by amissa            #+#    #+#             */
/*   Updated: 2026/11/05 17:22:05 by amissa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"
#include <pthread.h>
#include <unistd.h>

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

int	release_dongle(t_table *table, t_dongle *dongle)
{
	if (!dongle)
		return (1);
	pthread_mutex_lock(&dongle->lock);
	dongle->busy = 0;
	dongle->available_at = now_ms() + table->args[COOLDOWN];
	pthread_cond_broadcast(&dongle->cond);
	pthread_mutex_unlock(&dongle->lock);
	return (0);
}

int	release_dongles(t_coder *coder)
{
	if (release_dongle(coder->table, coder->left)
		|| release_dongle(coder->table, coder->right))
		return (1);
	return (0);
}

int	take_dongle(t_coder *coder, t_table *table, t_dongle *dongle)
{
	pthread_mutex_lock(&dongle->lock);
	if (heap_push(dongle->queue, coder, table->args[POLICY]))
		return (pthread_mutex_unlock(&dongle->lock), 1);
	while (!is_stoped(table) && (dongle->busy
			|| now_ms() < dongle->available_at
			|| !heap_peek(dongle->queue, coder->id)))
		wait_dongle(dongle);
	if (is_stoped(table))
		return (pthread_mutex_unlock(&dongle->lock), 1);
	heap_pop(dongle->queue);
	dongle->busy = 1;
	pthread_mutex_unlock(&dongle->lock);
	if (radio_log(coder, "has taken a dongle"))
		return (1);
	return (0);
}

int	take_dongles(t_coder *coder)
{
	if (coder->id % 2 == 0)
	{
		if (coder->left && take_dongle(coder, coder->table, coder->left))
			return (1);
		if (coder->right && take_dongle(coder, coder->table, coder->right))
			return (release_dongle(coder->table, coder->left), 1);
	}
	else
	{
		if (coder->right && take_dongle(coder, coder->table, coder->right))
			return (1);
		if (coder->left && take_dongle(coder, coder->table, coder->left))
			return (release_dongle(coder->table, coder->right), 1);
	}
	return (0);
}
