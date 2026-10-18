/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amissa <amissa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/18 12:58:33 by amissa            #+#    #+#             */
/*   Updated: 2026/10/18 13:37:22 by amissa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H

# include <getopt.h>
# include <unistd.h>
# include <stdlib.h>
# include <stdbool.h>
# include <pthread.h>
#include <string.h>

// remote at the end
# include <stdio.h>

typedef enum
{
	MISSING_ARGUMENT,
	INVALID_ARGUMENT,
	EXTRA_ARGUMENT,
}	Errors;

typedef struct t_arguments {
	int		number_of_coders;
	int		time_to_burnout;
	int		time_to_compile;
	int		time_to_debug;
	int		time_to_refactor;
	int		number_of_compiles_required;
	int		dongle_cooldown;
	char	*scheduler;
}	t_args;


void	_display_error(Errors err_status, void *extra_msg);

#endif
