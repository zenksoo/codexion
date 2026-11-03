/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   queue_helper.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amissa <amissa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/02 22:53:50 by amissa            #+#    #+#             */
/*   Updated: 2026/11/03 00:27:38 by amissa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"


void	shift_up(t_heap *heap, int i)
{
	int	parent;

	while (i > 0)
	{
		parent = (i - 1) / 2;
		if (!is_better(heap->nodes[i], heap->nodes[parent]))
			break ;
		swap_nodes(&heap->nodes[i], &heap->nodes[parent]);
		i = parent;
	}
}

void	shift_down(t_heap *heap, int i)
{
	int	best;
	int	left;
	int	right;

	while (1)
	{
		best = i;
		left = 2 * i + 1;
		right = 2 * i + 2;
		if (left < heap->size
			&& is_better(heap->nodes[left], heap->nodes[best]))
			best = left;
		if (right < heap->size
			&& is_better(heap->nodes[right], heap->nodes[best]))
			best = right;
		if (best == i)
			break ;
		swap_nodes(&heap->nodes[i], &heap->nodes[best]);
		i = best;
	}
}

void	heap_remove(t_heap *heap, int id)
{
	int	i;

	i = 0;
	while (i < heap->size && heap->nodes[i].coder_id != id)
		i++;
	if (i == heap->size)
		return ;
	heap->size--;
	if (i == heap->size)
		return ;
	heap->nodes[i] = heap->nodes[heap->size];
	shift_down(heap, i);
	shift_up(heap, i);
}
