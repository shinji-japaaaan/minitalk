/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sishizaw <sishizaw@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/25 17:56:15 by sishizaw              #+#    #+#             */
/*   Updated: 2024/06/25 17:56:15 by sishizaw             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include "libft/libft.h"

volatile sig_atomic_t g_received = 0;

void    handle_signal(int sig)
{
    static char c = 0;
    static int  bits = 0;

    if (sig == SIGUSR1)
        c |= (1 << bits);
    bits++;
    if (bits == 8)
    {
        if (c == '\0')
            ft_putstr_fd("\n", 1);
        else
            ft_putstr_fd(&c, 1);
        c = 0;
        bits = 0;
    }
    g_received = 1;
}

int main(void)
{
    struct sigaction sa;

    ft_putstr_fd("Server PID:", 1);
    ft_putnbr_fd(getpid(), 1);
    ft_putstr_fd("\n", 1);
    sa.sa_handler = handle_signal;
    sa.sa_flags = SA_RESTART;
    sigemptyset(&sa.sa_mask);
    if (sigaction(SIGUSR1, &sa, NULL) == -1 || sigaction(SIGUSR2, &sa, NULL) == -1)
    {
        ft_putstr_fd("error", 1);
        exit(EXIT_FAILURE);
    }
    while (1)
    {
        pause();
        if (g_received)
            g_received = 0;
    }
    return (0);
}
