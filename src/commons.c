/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commons.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amissa <amissa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/18 12:52:14 by amissa            #+#    #+#             */
/*   Updated: 2026/11/03 01:06:23 by amissa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"
#include <bits/pthreadtypes.h>
#include <bits/time.h>
#include <bits/types/struct_timeval.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>

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


double	get_power(int base, int exp)
{
	double	result;
	int		sign;
	int		i;

	if (exp == 0)
		return (1.0);
	sign = 1;
	if (exp < 0)
	{
		sign = -1;
		exp = -exp;
	}
	result = 1.0;
	i = -1;
	while (++i < exp)
		result *= base;
	if (sign == 1)
		return (result);
	return (1.0 / result);
}

double	us_to_ms(double us)
{
	return (us * get_power(10, -3));
}

long long	now_ms(void)
{
	struct timeval tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}
int	ms_sleep(long long ms, t_table *table)
{
	long long target;

	target = now_ms() + ms;

	while (now_ms() < target && !is_stoped(table))
		usleep(500);

	if (is_stoped(table))
		return 1;
	return 0;
}
