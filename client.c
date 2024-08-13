/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sishizaw <sishizaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/12 15:41:29 by sishizaw          #+#    #+#             */
/*   Updated: 2024/08/13 15:09:00 by sishizaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include "libft/libft.h"

void display_error(void)
{
    ft_putstr_fd("ERROR\n", 1);
    exit(1);
}

void    argv1_check(char **argv, long pid)
{
	int i;

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

void    send_message(int pid, char *str)
{
    int i;

    i = 0;
    while (*str)
    {
        while (i < 8)
        {
            if (*str & (1 << i))
                kill(pid, SIGUSR1);
            else
                kill(pid, SIGUSR2);
            usleep(100);
            i++;
        }
        str++;
        i = 0;
    }
    while (i < 8)
    {
        kill(pid, SIGUSR2);
        usleep(100);
        i++;
    }
}

int main(int argc, char **argv)
{
    long pid;

    if (argc != 3 || argv[2][0] == '\0')
    {
        display_error();
    }
	pid = (long)ft_atoi(argv[1]);
	argv1_check(argv, pid);
    send_message(pid, argv[2]);
    return (0);
}
