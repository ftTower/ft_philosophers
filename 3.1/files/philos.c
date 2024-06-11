/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philos.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tauer <tauer@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/27 23:35:43 by tauer             #+#    #+#             */
/*   Updated: 2024/06/11 02:56:53 by tauer            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <all.h>

void	lone_philo(t_param *template)
{
	printf("%-6ld %-6ld ", (long)0, (long)1);
	t_putstr(YELLOW, "is thinking", true);
	usleep(template->t_die);
	printf("%-6ld %-6ld ", template->t_die, (long)1);
	t_putstr(BG_RED, "is dead", true);
	exit(EXIT_SUCCESS);
}

void	life(t_philo *philo, bool print)
{
	if (get_time(MILLISECOND) - get_long(&philo->info.mutex,
			&philo->info.t_lastmeal) >= philo->param.t_die)
	{
		set_bool(&philo->info.mutex, &philo->info.dead, true);
		set_bool(&philo->sync->mutex, &philo->sync->end, true);
	}
	else if (get_bool(&philo->info.mutex, &philo->info.rdy_to_eat))
	{
		// print_statut_lock(EAT, philo, print, true);
		// print_statut_lock(EAT, philo, print, true);
		print_statut_lock(EAT, philo, print, false);
		// if (get_statut(philo) != EAT)
			set_long(&philo->info.mutex, &philo->info.t_lastmeal, get_time(MILLISECOND));
		set_statut(philo, EAT);
		usleep(philo->param.t_eat);
		increase_long(&philo->info.mutex, &philo->info.n_meal);
		set_bool(&philo->info.mutex, &philo->info.rdy_to_eat, false);
		set_statut(philo, SLEEP);
		print_statut_lock(SLEEP, philo, print, false);
		usleep(philo->param.t_sleep);
	}
	else
	{
		if (get_statut(philo) != THINK)
			print_statut_lock(THINK, philo, print, false);
		set_statut(philo, THINK);
		usleep(philo->param.t_eat);
	}
}

void	*philo_life(void *in_philo)
{
	t_philo	*philo;

	philo = (t_philo *)in_philo;
	while (!get_bool(&philo->sync->mutex, &philo->sync->all_ready))
		write(1, "", 0);
	while (!get_bool(&philo->sync->mutex, &philo->sync->monitor_ready))
		write(1, "", 0);
	usleep(1);
	philo->info.t_spawn = get_time(MILLISECOND);
	set_long(&philo->info.mutex, &philo->info.t_lastmeal, 0);
	while (!get_bool(&philo->sync->mutex, &philo->sync->end))
		life(philo, true);
	return (NULL);
}
