/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commons.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amissa <amissa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/18 12:52:14 by amissa            #+#    #+#             */
/*   Updated: 2026/10/18 13:47:16 by amissa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	_display_error(Errors err_status, void *extra_msg)
{
	if (err_status == MISSING_ARGUMENT)
	{
		printf("\033[41m  MISSING ARGUMENT  \033[49m\n");
		printf("    missing requirements arguments\n");
		printf("    usage:\n\t ./codexion ");
		printf("\n\t\t(int)<number_of_coders>");
		printf("\n\t\t(int)<time_to_burnout>");
		printf("\n\t\t(int)<time_to_compile>");
		printf("\n\t\t(int)<time_to_debug>");
		printf("\n\t\t(int)<time_to_refactor>");
		printf("\n\t\t(int)<number_of_compiles_required>");
		printf("\n\t\t(int)<dongle_cooldown>\n\t\t<scheduler>(str)\n");
		exit(1);
	}
	else if (err_status == EXTRA_ARGUMENT)
	{
		printf("\033[41m  EXTRA ARGUMENT  \033[49m\n");
		printf("	extra argument is given\n");
		exit(1);
	}
	else if (err_status == INVALID_ARGUMENT) {
		printf("\033[41m  INVALID ARGUMENT  \033[49m\n\n");
		if (extra_msg)
		{	printf("%s\n", (char *)extra_msg);
		}
		exit(1);
	}
}