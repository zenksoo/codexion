/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amissa <amissa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/18 12:58:33 by amissa            #+#    #+#             */
/*   Updated: 2026/10/28 08:35:00 by amissa           ###   ########.fr       */
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
# include <sys/time.h>

enum e_ERROR
{
	MISSING_ARGUMENT,
	INVALID_ARGUMENT,
	EXTRA_ARGUMENT,
};

typedef struct s_heap_node
{
	long long	key;
	int			coder_id;
}	t_heap_node;

typedef struct s_heap
{
	t_heap_node	*nodes;
	int			size;
	int			cap;
}	t_heap;

typedef struct s_dongle
{
	// pthread_mutex_t	lock;
	// pthread_cond_t	cond;
	int				busy;
	long long		available_at;
	t_heap			queue;
}	t_dongle;

typedef struct s_coder
{
	int				id;
	pthread_t		thread;
	t_dongle		*left;
	t_dongle		*right;
	long long		last_compile_start;
	int				compile_count;
	// pthread_mutex_t	lock;
	struct s_table	*table;
}	t_coder;

typedef struct s_table
{
	int				n;
	long long		t_burnout;
	long long		t_compile;
	long long		t_debug;
	long long		t_refactor;
	long long		cooldown;
	int				compiles_required;
	int				use_edf;
	long long		start_time;
	int				stop;
	// pthread_mutex_t	state_lock;
	// pthread_mutex_t	log_lock;
	t_dongle		*dongles;
	t_coder			*coders;
	pthread_t		monitor;
}	t_table;

int			cli_argument_parser(int argc, char **argv);
int			valid_passed_argument(char **argv, int argSize);
int			_display_error(enum e_ERROR err_status, void *extra_msg);

int			init_coders(t_table *table);
int			init_dongles(t_table *table);
int			init_table(t_table *table, char **cli_args);

void		run_simulation(t_table *table);
void		*coder_routine(t_coder*coder);
void		*monitor_routine(t_table *table);

double		get_power(int base, int exp);
double		us_to_ms(double us);
long long	now_ms(void);

#endif
