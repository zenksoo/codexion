/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commons.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amissa <amissa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/18 12:52:14 by amissa            #+#    #+#             */
/*   Updated: 2026/11/03 20:29:37 by amissa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	_display_error(enum e_ERROR err_status, void *extra_msg)
{
	if (err_status == MISSING_ARGUMENT)
	{
		fprintf(stderr, "\033[41m  MISSING ARGUMENT  \033[49m\n");
		fprintf(stderr, "    missing requirements arguments\n");
		fprintf(stderr, "    usage:\n\t ./codexion ");
		fprintf(stderr, " (int)<number_of_coders>");
		fprintf(stderr, " (int)<time_to_burnout>");
		fprintf(stderr, " (int)<time_to_compile>");
		fprintf(stderr, " (int)<time_to_debug>");
		fprintf(stderr, " (int)<time_to_refactor>");
		fprintf(stderr, " (int)<number_of_compiles_required>");
		fprintf(stderr, " (int)<dongle_cooldown> (str)<scheduler>\n");
	}
	else if (err_status == EXTRA_ARGUMENT)
	{
		fprintf(stderr, "\033[41m  EXTRA ARGUMENT  \033[49m\n");
		fprintf(stderr, "	extra argument are given\n");
	}
	else if (err_status == INVALID_ARGUMENT)
	{
		fprintf(stderr, "\033[41m  INVALID ARGUMENT  \033[49m\n");
		if (extra_msg)
			fprintf(stderr, "\t%s\n", (char *)extra_msg);
	}
	return (1);
}

long long	now_ms(void)
{
	struct timespec	tp;

	clock_gettime(CLOCK_MONOTONIC, &tp);
	return (tp.tv_sec * 1000 + tp.tv_nsec / 1000000);
}

long long	timespec_to_ms(struct timespec ts)
{
	return (ts.tv_sec * 1000 + ts.tv_nsec / 1000000);
}

int	ms_sleep(long long ms, t_table *table)
{
	long long	target;

	target = now_ms() + ms;
	while (now_ms() < target && !is_stoped(table))
		usleep(500);
	if (is_stoped(table))
		return (1);
	return (0);
}
