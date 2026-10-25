/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commons.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amissa <amissa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/18 12:52:14 by amissa            #+#    #+#             */
/*   Updated: 2026/10/26 00:22:21 by amissa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"
#include <stdio.h>

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
