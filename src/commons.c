/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commons.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zenkso <zenkso@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/18 12:52:14 by amissa            #+#    #+#             */
/*   Updated: 2026/10/18 13:54:59 by zenkso           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"
#include <stdio.h>

int _display_error(Errors err_status, void *extra_msg)
{
	if (err_status == MISSING_ARGUMENT)
	{
		fprintf(stderr, "\033[41m  MISSING ARGUMENT  \033[49m\n");
		fprintf(stderr, "    missing requirements arguments\n");
		fprintf(stderr, "    usage:\n\t ./codexion ");
		fprintf(stderr, "\n\t\t(int)<number_of_coders>");
		fprintf(stderr, "\n\t\t(int)<time_to_burnout>");
		fprintf(stderr, "\n\t\t(int)<time_to_compile>");
		fprintf(stderr, "\n\t\t(int)<time_to_debug>");
		fprintf(stderr, "\n\t\t(int)<time_to_refactor>");
		fprintf(stderr, "\n\t\t(int)<number_of_compiles_required>");
		fprintf(stderr, "\n\t\t(int)<dongle_cooldown>\n\t\t<scheduler>(str)\n");
	}
	else if (err_status == EXTRA_ARGUMENT)
	{
		fprintf(stderr, "\033[41m  EXTRA ARGUMENT  \033[49m\n");
		fprintf(stderr, "	extra argument is given\n");
	}
	else if (err_status == INVALID_ARGUMENT) {
		fprintf(stderr, "\033[41m  INVALID ARGUMENT  \033[49m\n\n");
		if (extra_msg)
		{	fprintf(stderr, "%s\n", (char *)extra_msg);
		}
	}
    return 1;
}
