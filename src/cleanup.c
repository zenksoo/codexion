/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amissa <amissa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/04 12:00:21 by amissa            #+#    #+#             */
/*   Updated: 2026/11/04 22:13:33 by amissa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"
#include <pthread.h>

int	destroy_coders(t_table *table)
{
	int	i;

	i = -1;
	while (++i < table->n && table->coders && table->coders[i])
	{
		if (table->coders[i]->initialized_mutex)
			pthread_mutex_destroy(&table->coders[i]->lock);
		free(table->coders[i]);
	}
	if (table->coders)
		free(table->coders);
	return (0);
}

int	destroy_dongles(t_table *table)
{
	int	i;

	i = -1;
	while (++i < table->n)
	{
		if (table->dongles[i]->destroy_mutex)
			pthread_mutex_destroy(&table->dongles[i]->lock);
		if (table->dongles[i]->destroy_conds[0])
			pthread_cond_destroy(&table->dongles[i]->cond);
		if (table->dongles[i]->destroy_conds[1])
			pthread_condattr_destroy(&table->dongles[i]->attr);
		if (table->dongles[i]->queue)
		{
			if (table->dongles[i]->queue->nodes)
				free(table->dongles[i]->queue->nodes);
			free(table->dongles[i]->queue);
		}
		if (table->dongles[i])
			free(table->dongles[i]);
	}
	if (table->dongles)
		free(table->dongles);
	return (0);
}

void	general_cleanup(t_table *table)
{
	if (table->destroy_mutex[0])
		pthread_mutex_destroy(&table->log_lock);
	if (table->destroy_mutex[1])
		pthread_mutex_destroy(&table->state_lock);
	if (table->args)
		free(table->args);
	destroy_coders(table);
	destroy_dongles(table);
}
