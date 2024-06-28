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

volatile sig_atomic_t g_ack_received = 0;

void    handle_ack(int sig)
{
    (void)sig;
    g_ack_received = 1;
    ft_putstr_fd("received\n", 1);
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
    server_pid = ft_atoi(argv[1]);
    const char  *message = argv[2];
    struct sigaction sa;
    sa.sa_handler = handle_ack;
    sa.sa_flags = SA_RESTART;
    sigemptyset(&sa.sa_mask);
    if (sigaction(SIGUSR1, &sa, NULL) == -1)
    {
        ft_putstr_fd("error", 1);
        exit(EXIT_FAILURE);
    }
    send_str(server_pid, message);
    while (!g_ack_received)
        pause();
    return (0);
}
