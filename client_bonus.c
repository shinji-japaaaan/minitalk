/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client_bonus.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sishizaw <sishizaw@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/27 10:43:44 by sishizaw              #+#    #+#             */
/*   Updated: 2024/06/27 10:43:44 by sishizaw             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include "libft/libft.h"

void    handle_ack(int sig)
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

void    send_char(int server_pid, char c)
{
    int i;

    i = 0;
    while (i < 8)
    {
        if (c & (1 << i))
            kill(server_pid, SIGUSR1);
        else
            kill(server_pid, SIGUSR2);
        i++;
        usleep(100);
    }
}

void    send_str(int server_pid, const char *str)
{
    while (*str)
    {
        send_char(server_pid, *str);
        str++;
    }
    send_char(server_pid, '\0');
}

int main(int argc, char **argv)
{
    int server_pid;

    if (argc != 3)
    {
        ft_putstr_fd("Usage: client [server_pid] [message]\n", 1);
        return (1);
    }
    ft_putstr_fd("sent message:", 1);
    ft_putnbr_fd(ft_strlen(argv[2]), 1);
    ft_putstr_fd("文字\n", 1);
    server_pid = ft_atoi(argv[1]);
    struct sigaction sa;
    sa.sa_handler = handle_ack;
    sa.sa_flags = SA_RESTART;
    sigemptyset(&sa.sa_mask);
    sigaddset(&sa.sa_mask, SIGUSR1),sigaddset(&sa.sa_mask, SIGUSR2);
    if (sigaction(SIGUSR1, &sa, NULL) == -1 || sigaction(SIGUSR2, &sa, NULL) == -1)
    {
        ft_putstr_fd("error", 1);
        exit(EXIT_FAILURE);
    }
    send_str(server_pid, argv[2]);
    while (1)
        pause();
    return (0);
}
