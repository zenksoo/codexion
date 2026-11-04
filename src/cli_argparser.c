/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cli_argparser.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amissa <amissa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/25 23:57:35 by amissa            #+#    #+#             */
/*   Updated: 2026/11/04 13:26:10 by amissa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"
#include <stdlib.h>

int	is_valid_number(char *val)
{
	if (*val == '-' || *val == '+')
		val++;
	while (*val >= '0' && *val <= '9')
		val++;
	if (*val)
		return (0);
	return (1);
}

int	valid_passed_argument(char **argv, int argSize)
{
	int	i;

	i = -1;
	while (++i < argSize)
	{
		if (i == argSize - 1)
		{
			if (strcmp(argv[7], "fifo") && strcmp(argv[7], "edf"))
				return (2);
		}
		else
		{
			if (!*argv[i] || !is_valid_number(argv[i]))
				return (1);
			if (atoi(argv[i]) < 0)
				return (1);
		}
	}
	if (atoi(argv[0]) == 0)
		return (1);
	return (0);
}

int	cli_argument_parser(int argc, char **argv)
{
	int	validation_status;

	if (argc == 9)
	{
		validation_status = valid_passed_argument(argv + 1, argc - 1);
		if (validation_status == 1)
			return (_display_error(
					INVALID_ARGUMENT,
					"Invalid Argument!"));
		else if (validation_status == 2)
			return (_display_error(
					INVALID_ARGUMENT,
					"the last argument must be 'fifo' or 'edf'"));
		else if (validation_status == 3)
			return (_display_error(
					INVALID_ARGUMENT,
					"Invalid integer in the first 7 arguments"));
	}
	else if (argc > 9)
		return (_display_error(EXTRA_ARGUMENT, NULL));
	else
		return (_display_error(MISSING_ARGUMENT, NULL));
	return (0);
}
