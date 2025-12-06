/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yotakagi <yotakagi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/27 09:35:41 by ayamamot          #+#    #+#             */
/*   Updated: 2025/12/06 15:20:53 by yotakagi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SIGNALS_H
# define SIGNALS_H

# include "minishell.h"
# include <signal.h>

extern volatile sig_atomic_t	g_signal;

void							init_signals(void);
void							handle_sigint(int sig);
void							heredoc_sigint_handler(int sig);

#endif
