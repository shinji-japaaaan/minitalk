/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sishizaw <root@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/25 17:50:48 by sishizaw              #+#    #+#             */
/*   Updated: 2024/06/25 17:50:48 by sishizaw             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include "libft/libft.h"

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
        usleep(100);
        i++;
    }
}

void    send_string(int server_pid, const char *str)
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
    if (argc != 3)
    {
        ft_putstr_fd("Usage: client <server_pid> <message>\n", 1);
        return (1);
    }
    int server_pid = ft_atoi(argv[1]);
    const char *message = (const char *)argv[2];

    send_string(server_pid, message);
    return (0);
}