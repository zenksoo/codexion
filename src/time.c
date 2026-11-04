/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commons.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amissa <amissa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/18 12:52:14 by amissa            #+#    #+#             */
/*   Updated: 2026/11/04 14:41:21 by amissa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

long long	now_ms(void)
{
	struct timespec	tp;

	clock_gettime(CLOCK_MONOTONIC, &tp);
	return (tp.tv_sec * 1000 + tp.tv_nsec / 1000000);
}
