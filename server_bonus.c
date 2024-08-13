/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_bonus.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sishizaw <sishizaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/13 15:28:10 by sishizaw          #+#    #+#             */
/*   Updated: 2024/08/13 15:28:12 by sishizaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include "libft/libft.h"

pid_t g_client_pid = 0;

void    handle_signal(int sig, siginfo_t *info, void *context)
{
    static char c = 0;
    static int  bits = 0;

    (void)context;	
    if (sig == SIGUSR1)
        c |= (1 << bits);
    bits++;
    if (bits == 8)
    {
       if (c == '\0')
       {
            ft_putchar_fd('\n', 1);
            kill(g_client_pid, SIGUSR2);
       }    
        else
        {
            ft_putchar_fd(c, 1);
            kill(g_client_pid, SIGUSR1);
        }
        c = 0;
        bits = 0;
    }
    g_client_pid = info->si_pid;
}

int main(void)
{
    struct sigaction sa;

    ft_putstr_fd("Server PID:", 1);
    ft_putnbr_fd(getpid(), 1);
    ft_putstr_fd("\n", 1);
    sa.sa_sigaction = handle_signal;
    sa.sa_flags = SA_SIGINFO | SA_RESTART;
    sigemptyset(&sa.sa_mask);
    sigaddset(&sa.sa_mask, SIGUSR1);
	sigaddset(&sa.sa_mask, SIGUSR2);
    if (sigaction(SIGUSR1, &sa, NULL) == -1 || sigaction(SIGUSR2, &sa, NULL) == -1)
    {
        ft_putstr_fd("error", 1);
        exit(EXIT_FAILURE);
    }
    while (1)
        pause();
    return (0);
}
