/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayamamot <ayamamot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/18 19:00:40 by nagisa            #+#    #+#             */
/*   Updated: 2025/12/01 06:18:59 by ayamamot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
#define MINISHELL_H

#define _POSIX_C_SOURCE 200809L

#include "builtin.h"
#include "lexer.h"
#include "libft/libft.h"
#include "parser.h"
#include "executor.h"
#include "expand.h"
#include "heredoc.h"
#include "signals.h"
#include <readline/history.h>
#include <readline/readline.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/stat.h>


#endif
