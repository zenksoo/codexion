/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amissa <amissa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/30 18:24:05 by amissa            #+#    #+#             */
/*   Updated: 2026/10/31 01:02:07 by amissa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int take_right_dongle(t_coder *coder, t_table *table)
{
	pthread_mutex_lock(&coder->right->lock);
	while (!is_stoped(table) && (coder->right->busy || now_ms() < coder->right->available_at))
		pthread_cond_wait(&coder->right->cond, &coder->right->lock);
	if (is_stoped(table))
	{
		coder->right->busy = 0;
		return (pthread_mutex_unlock(&coder->right->lock), 1);
	}
	coder->right->busy = 1;
	pthread_mutex_unlock(&coder->right->lock);

	protected_log(&table->log_lock,
		now_ms() - table->start_time,
		coder->id, "has taken a dongle");
	return (0);
}

int take_left_dongle(t_coder *coder, t_table *table)
{
	pthread_mutex_lock(&coder->left->lock);
	while (!is_stoped(table) && (coder->left->busy || now_ms() < coder->left->available_at))
		pthread_cond_wait(&coder->left->cond, &coder->left->lock);
	if (is_stoped(table))
	{
		coder->left->busy = 0;
		return (pthread_mutex_unlock(&coder->left->lock), 1);
	}
	coder->left->busy = 1;
	pthread_mutex_unlock(&coder->left->lock);

	protected_log(&table->log_lock,
		now_ms() - table->start_time,
		coder->id, "has taken a dongle");
	return (0);
}


int take_dongels_and_compile(t_coder *coder)
{
	t_table *table;

	table = coder->table;
	if (coder->id % 2 == 0)
	{
		if (take_left_dongle(coder, table) || take_right_dongle(coder, table))
			return (1);
	}
	else
	{
		if (take_right_dongle(coder, table) || take_left_dongle(coder, table))
			return (1);
	}
	pthread_mutex_lock(&coder->lock);
	coder->last_compile_start = now_ms();
	pthread_mutex_unlock(&coder->lock);

	protected_log(&coder->table->log_lock,
			now_ms() - coder->table->start_time,
			coder->id, "is compiling");

	ms_sleep(coder ->table->t_compile, table);

	pthread_mutex_lock(&coder->lock);
	coder->compile_count++;
	pthread_mutex_unlock(&coder->lock);

	pthread_mutex_lock(&coder->right->lock);
	coder->right->busy = 0;
	coder->right->available_at = now_ms() + coder->table->cooldown;
	pthread_cond_broadcast(&coder->right->cond);
	pthread_mutex_unlock(&coder->right->lock);

	pthread_mutex_lock(&coder->left->lock);
	coder->left->busy = 0;
	coder->left->available_at = now_ms() + coder->table->cooldown;
	pthread_cond_broadcast(&coder->left->cond);
	pthread_mutex_unlock(&coder->left->lock);

	return 0;
}


void	*coder_routine(void *arg)
{
	t_coder *coder;

	coder = (t_coder *)arg;
	wait_for_start((void *)coder->table);
	while (!is_stoped(coder->table))
	{
		if (take_dongels_and_compile(coder))
			break;


		protected_log(&coder->table->log_lock,
			now_ms() - coder->table->start_time,
			coder->id, "is debugging");

		ms_sleep(coder->table->t_debug, coder->table);

		protected_log(&coder->table->log_lock,
			now_ms() - coder->table->start_time,
			coder->id, "is refactoring");

		ms_sleep(coder->table->t_refactor, coder->table);
	}
	return (NULL);
}
