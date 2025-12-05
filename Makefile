# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ayamamot <ayamamot@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/05/18 19:40:36 by nagisa            #+#    #+#              #
#    Updated: 2025/12/05 12:37:48 by ayamamot         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC = cc
CFLAGS = -Wall -Wextra -Werror -Iincludes
LDFLAGS = -lreadline

# ディレクトリ定義
SRC_DIR = src
OBJ_DIR = obj
INCLUDE_DIR = includes
LIBFT_DIR = $(INCLUDE_DIR)/libft

# 再帰的に .c ファイルを全て収集
SRCS = $(shell find $(SRC_DIR) $(LIBFT_DIR) -name "*.c")
OBJS = $(SRCS:%.c=$(OBJ_DIR)/%.o)

# 出力ファイル
NAME = minishell

# デフォルトターゲット
all: $(NAME)

# ルール：minishell を作る
$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LDFLAGS) -o $(NAME)

# .c -> .o 変換ルール（.o を obj/ 以下に配置）
$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# クリーン系
clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re

# CC = cc
# CFLAGS = -Wall -Wextra -Werror -Iincludes
# LDFLAGS = -lreadline

# SRCS =	src/main/builtin/builtin.c \
# 		src/main/builtin/builtin_libft.c \
# 		src/main/parser/cmd.c \
# 		src/main/parser/libft.c \
# 		src/main/parser/node.c \
# 		src/main/parser/ft_error.c \
# 		src/main/parser/parser_utils.c \
# 		src/main/parser/parser.c \
# 		src/main/parser/redirections.c \
# 		src/main/parser/shell.c \
# 		src/main/lexer/lexer.c \
# 		src/main/lexer/read_dollar.c \
# 		src/main/lexer/read_operator.c \
# 		src/main/lexer/read_quoted.c \
# 		src/main/lexer/read_word.c \
# 		src/main/lexer/utils.c \
# 		src/main/main.c

# OBJS = $(SRCS:.c=.o)

# NAME = minishell

# all: $(NAME)

# $(NAME): $(OBJS)
# 	$(CC) $(CFLAGS) $(OBJS) $(LDFLAGS) -o $(NAME)

# clean:
# 	rm -f $(OBJS)

# fclean: clean
# 	rm -f $(NAME)

# re: fclean all

# .PHONY: all clean fclean re
