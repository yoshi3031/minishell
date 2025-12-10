# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: yotakagi <yotakagi@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/05/18 19:40:36 by nagisa            #+#    #+#              #
#    Updated: 2025/12/09 17:47:25 by yotakagi         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC = cc
CFLAGS = -Wall -Wextra -Werror -Iincludes
LDFLAGS = -lreadline

SRC_DIR = src
OBJ_DIR = obj
INCLUDE_DIR = includes
LIBFT_DIR = $(INCLUDE_DIR)/libft
LIBFT_A = $(LIBFT_DIR)/libft.a

SRCS = \
	src/main/main.c \
	src/parser/parser.c \
	src/parser/parser_utils.c \
	src/parser/parser_error.c \
	src/parser/cmd.c \
	src/parser/cmd_utils.c \
	src/parser/node.c \
	src/parser/shell.c \
	src/parser/redirections.c \
	src/parser/count_quotes.c \
	src/lexer/lexer.c \
	src/lexer/read_word.c \
	src/lexer/read_quoted.c \
	src/lexer/read_operator.c \
	src/lexer/read_dollar.c \
	src/lexer/utils.c \
	src/expander/expand.c \
	src/expander/expand_token.c \
	src/expander/expander_utils.c \
	src/expander/expander_len.c \
	src/executor/executor.c \
	src/executor/exec_cmd.c \
	src/executor/single_cmd.c \
	src/executor/multiple_cmds.c \
	src/executor/path_utils.c \
	src/executor/redirect.c \
	src/executor/wait_until.c \
	src/heredoc/heredoc.c \
	src/signals/signals.c \
	src/builtin/builtin.c \
	src/builtin/echo.c \
	src/builtin/export.c \
	src/builtin/export1.c \
	src/builtin/export2.c \
	src/builtin/exit.c \
	src/builtin/exit_utils.c \
	src/builtin/cd.c \
	src/builtin/pwd.c \
	src/builtin/env.c \
	src/builtin/unset.c
OBJS = $(SRCS:%.c=$(OBJ_DIR)/%.o)

NAME = minishell

all: $(NAME)

$(NAME): $(OBJS) $(LIBFT_A)
	$(CC) $(CFLAGS) $(OBJS) $(LIBFT_A) $(LDFLAGS) -o $(NAME)

$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(LIBFT_A):
	$(MAKE) -C $(LIBFT_DIR)

clean:
	$(MAKE) -C $(LIBFT_DIR) clean
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(NAME)
	$(MAKE) -C $(LIBFT_DIR) fclean

re: fclean all

.PHONY: all clean fclean re
