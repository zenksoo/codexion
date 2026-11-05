/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amissa <amissa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/30 18:24:05 by amissa            #+#    #+#             */
/*   Updated: 2026/11/05 22:15:29 by amissa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	radio_log(t_coder *coder, char *msg)
{
	t_table	*table;

	table = coder->table;
	if (pthread_mutex_lock(&table->log_lock))
		return (1);
	if (is_stoped(table))
		return (pthread_mutex_unlock(&table->log_lock), 1);
	printf("%lld %d %s\n", now_ms() - table->start_time, coder->id, msg);
	fflush(stdout);
	if (pthread_mutex_unlock(&table->log_lock))
		return (1);
	return (0);
}

int	compile(t_coder *coder)
{
	t_table	*table;

	table = coder->table;
	if (take_dongles(coder))
		return (1);
	pthread_mutex_lock(&coder->lock);
	coder->last_compile_start = now_ms();
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
	if (is_stoped(table))
		return (NULL);
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
