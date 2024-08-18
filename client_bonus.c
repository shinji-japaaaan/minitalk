/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client_bonus.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sishizaw <sishizaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/12 18:50:56 by sishizaw          #+#    #+#             */
/*   Updated: 2024/08/18 09:31:51 by sishizaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include "libft/libft.h"

void	display_error(void)
{
	ft_putstr_fd("ERROR\n", 1);
	exit(1);
}

void	argv1_check(char **argv, long pid)
{
	int	i;

	i = 0;
	while (argv[1][i] != '\0')
	{
		if (argv[1][i] < '0' || '9' < argv[1][i])
			display_error();
		i++;
	}
	if (pid > 2147483647)
		display_error();
	if (pid <= 1)
		display_error();
}

void	handle_ack(int sig)
{
	static int	count = 0;

	if (sig == SIGUSR1)
		++count;
	else
	{
		ft_putstr_fd("received:", 1);
		ft_putnbr_fd(count, 1);
		ft_putstr_fd("文字\n", 1);
		exit(0);
	}
}

void	send_message(int pid, char *str)
{
	int	i;

	i = 0;
	while (*str)
	{
		while (i < 8)
		{
			if (*str & (1 << i))
				kill(pid, SIGUSR1);
			else
				kill(pid, SIGUSR2);
			i++;
			usleep(250);
		}
		str++;
		i = 0;
	}
	while (i < 8)
	{
		kill(pid, SIGUSR2);
		usleep(250);
		i++;
	}
}

int	main(int argc, char **argv)
{
	long				pid;
	struct sigaction	sa;

	if (argc != 3 || argv[2][0] == '\0')
	{
		display_error();
	}
	pid = (long)ft_atoi(argv[1]);
	argv1_check(argv, pid);
	sa.sa_handler = handle_ack;
	sa.sa_flags = SA_RESTART;
	sigemptyset(&sa.sa_mask);
	sigaddset(&sa.sa_mask, SIGUSR1);
	sigaddset(&sa.sa_mask, SIGUSR2);
	if (sigaction(SIGUSR1, &sa, NULL) == -1 || \
	sigaction(SIGUSR2, &sa, NULL) == -1)
	{
		ft_putstr_fd("error", 1);
		exit(EXIT_FAILURE);
	}
	send_message(pid, argv[2]);
	while (1)
		pause();
	return (0);
}
