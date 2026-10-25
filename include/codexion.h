/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amissa <amissa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/18 12:58:33 by amissa            #+#    #+#             */
/*   Updated: 2026/10/26 00:21:36 by amissa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H

# include <getopt.h>
# include <stdlib.h>
# include <unistd.h>
# include <stdbool.h>
# include <pthread.h>
# include <stdio.h>
# include <string.h>

enum e_ERROR
{
	MISSING_ARGUMENT,
	INVALID_ARGUMENT,
	EXTRA_ARGUMENT,
};

typedef struct t_arguments
{
	int		number_of_coders;
	int		time_to_burnout;
	int		time_to_compile;
	int		time_to_debug;
	int		time_to_refactor;
	int		number_of_compiles_required;
	int		dongle_cooldown;
	char	*scheduler;
}	t_args;

int		_display_error(enum e_ERROR err_status, void *extra_msg);
void	pick_arguments(char **args, t_args *data);
int		valid_passed_argument(char **argv, int argSize);
int		cli_argument_parser(int argc, char **argv, t_args *data);

#endif
