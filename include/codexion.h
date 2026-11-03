/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amissa <amissa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/18 12:58:33 by amissa            #+#    #+#             */
/*   Updated: 2026/11/03 01:13:10 by amissa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H

# include <unistd.h>
# include <stdbool.h>
# include <stdio.h>
# include <string.h>
# include <pthread.h>
# include <sys/time.h>
# include <stdlib.h>

# define FIFO 1
# define EDF 2

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
	int			priority;
}	t_heap;

typedef struct s_dongle
{
	int				id;
	pthread_mutex_t	lock;
	pthread_cond_t	cond;
	int				busy;
	long long		available_at;
	t_heap			*queue;
}	t_dongle;

typedef struct s_coder
{
	int				id;
	pthread_t		thread;
	t_dongle		*left;
	t_dongle		*right;
	long long		last_compile_start;
	int				compile_count;
	pthread_mutex_t	lock;
	struct s_table	*table;
}	t_coder;

typedef struct s_cliargs
{
	long long		t_burnout;
	long long		t_compile;
	long long		t_debug;
	long long		t_refactor;
	long long		cooldown;
	int				compiles_required;
	int				scheduler;

}	t_cliargs;

typedef struct s_table
{
	int				n;
	t_cliargs		*cli_args;
	long long		start_time;
	int				stop;
	int				ready;
	int				ready_coder_id;
	pthread_mutex_t	state_lock;
	pthread_mutex_t	log_lock;
	t_dongle		**dongles;
	t_coder			**coders;
	pthread_t		monitor;
}	t_table;

int			cli_argument_parser(int argc, char **argv);
int			valid_passed_argument(char **argv, int argSize);
int			_display_error(enum e_ERROR err_status, void *extra_msg);


void	swap_nodes(t_heap_node *a, t_heap_node *b);
int	is_better(t_heap_node a, t_heap_node b);


t_heap		*init_queue(int capacity, int scheduler);
t_heap_node	heap_pop(t_heap *heap);
int			heap_push(t_heap *heap, t_coder *coder, int policy);
int			heap_peek(t_heap *heap, int target);
void		destroy_queue(t_heap *heap);

void		shift_up(t_heap *heap, int i);
void		shift_down(t_heap *heap, int i);
void		heap_remove(t_heap *heap, int id);

int			init_coders(t_table *table);
int			init_dongles(t_table *table);
int			init_table(t_table *table, char **cli_args);

void		wait_for_start(void *arg);

void		run_simulation(t_table *table);
void		stop_simulation(t_table *table);

void		*coder_routine(void *arg);
void		*monitor_routine(void *arg);

int			is_stoped(t_table *table);

double		get_power(int base, int exp);

double		us_to_ms(double us);
long long	now_ms(void);
int			ms_sleep(long long ms, t_table *table);

void		protected_log(t_table *table, int id, char *msg);

#endif
