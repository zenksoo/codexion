/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cli_argparser.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amissa <amissa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/25 23:57:35 by amissa            #+#    #+#             */
/*   Updated: 2026/11/05 22:41:55 by amissa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	display_error(void)
{
	fprintf(stderr, "ERROR!\n");
	fflush(stderr);
	return (1);
}

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
			if (strcmp(argv[i], "fifo") && strcmp(argv[i], "edf"))
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
	if (argc == 9)
	{
		if (valid_passed_argument(argv + 1, argc - 1))
			return (display_error());
	}
	else if (argc > 9)
		return (display_error());
	else
		return (display_error());
	return (0);
}
