/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amissa <amissa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/30 18:24:05 by amissa            #+#    #+#             */
/*   Updated: 2026/10/31 15:02:37 by amissa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

// int	unlock_dongle(t_table *table, t_dongle *dongle)
// {
// 	pthread_mutex_lock(&dongle->lock);
// 	dongle->busy = 0;
// 	dongle->available_at = now_ms() + table->cooldown;
// 	pthread_cond_broadcast(&dongle->cond);
// 	pthread_mutex_unlock(&dongle->lock);
// 	return 0;
// }

// int	take_dongle(t_coder *coder, t_table *table, t_dongle *dongle)
// {
// 	pthread_mutex_lock(&dongle->lock);
// 	while (!is_stoped(table) && (dongle->busy || now_ms() < dongle->available_at))
// 		pthread_cond_wait(&dongle->cond, &dongle->lock);
// 	if (is_stoped(table))
// 		return (pthread_mutex_unlock(&dongle->lock), 1);
// 	dongle->busy = 1;
// 	pthread_mutex_unlock(&dongle->lock);

// 	protected_log(&table->log_lock,
// 		now_ms() - table->start_time,
// 		coder->id, "has taken a dongle");
// 	return (0);

// }

// int take_dongels_and_compile(t_coder *coder)
// {
// 	t_table *table;

// 	table = coder->table;
// 	if (coder->id % 2 == 0)
// 	{
// 		if (take_dongle(coder, table, coder->left))
// 		{
// 			if (coder->right->busy)
// 				unlock_dongle(table, coder->right);
// 			return 1;
// 		}
// 		if (take_dongle(coder, table, coder->right))
// 		{
// 			if (coder->left->busy)
// 				unlock_dongle(table, coder->left);
// 			return 1;
// 		}
// 	}
// 	else
// 	{
// 		if (take_dongle(coder, table, coder->right))
// 		{
// 			if (coder->left->busy)
// 				unlock_dongle(table, coder->left);
// 			return 1;
// 		}
// 		if (take_dongle(coder, table, coder->left))
// 		{
// 			if (coder->left->busy)
// 				unlock_dongle(table, coder->right);
// 			return 1;
// 		}
// 	}
// 	pthread_mutex_lock(&coder->lock);
// 	coder->last_compile_start = now_ms();
// 	pthread_mutex_unlock(&coder->lock);

// 	protected_log(&coder->table->log_lock,
// 			now_ms() - coder->table->start_time,
// 			coder->id, "is compiling");

// 	ms_sleep(coder ->table->t_compile, table);

// 	pthread_mutex_lock(&coder->lock);
// 	coder->compile_count++;
// 	pthread_mutex_unlock(&coder->lock);

// 	unlock_dongle(table, coder->right);
// 	unlock_dongle(table, coder->left);

// 	return 0;
// }


void	*coder_routine(void *arg)
{
	t_coder *coder;

	coder = (t_coder *)arg;
	wait_for_start((void *)coder->table);
	printf("the coder %d start working\n", coder->id);
	// while (!is_stoped(coder->table))
	// {
	// 	if (take_dongels_and_compile(coder))
	// 		return NULL;

	// 	protected_log(&coder->table->log_lock,
	// 		now_ms() - coder->table->start_time,
	// 		coder->id, "is debugging");

	// 	ms_sleep(coder->table->t_debug, coder->table);

	// 	protected_log(&coder->table->log_lock,
	// 		now_ms() - coder->table->start_time,
	// 		coder->id, "is refactoring");

	// 	ms_sleep(coder->table->t_refactor, coder->table);
	// }
	return (NULL);
}
