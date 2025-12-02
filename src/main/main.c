/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayamamot <ayamamot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 14:13:27 by yotakagi          #+#    #+#             */
/*   Updated: 2025/12/02 04:25:09 by ayamamot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"
#include <stdio.h>
#include <stdlib.h>

char	**ft_arrdup(char **arr)
{
	char	**result;
	size_t	i;

	// indexはマイナスに成り得ないため、size_tを使う方が標準的で安全
	i = 0;
	while (arr[i] != NULL)
		i++;
	// callocは全てをNULLで埋める。末尾に必ずNULLを入れたい。
	result = ft_calloc(sizeof(char *), i + 1);
	if (!result)
		return (NULL);
	i = 0;
	while (arr[i] != NULL)
	{
		result[i] = ft_strdup(arr[i]);
		if (result[i] == NULL)
		{
			free_arr(result);
			return (NULL);
		}
		i++;
	}
	return (result);
}

int	main(int ac, char **av, char **env)
{
	t_shell	shell;

	(void)ac;
	(void)av;
	// if (argc != 1 || argv[1])
	// {
	// 	printf("This program does not accept arguments\n");
	// 	exit(0);
	// }
	shell.env = ft_arrdup(env);
	// find_pwd(&tools);
	init_shell(&shell);
	// printf("\n%s\n\n", WELCOME_MSG);
	shell.error_num = loop(&shell);
	return (0);
}

// 以前のmain
//  int	main(void)
//  {
//  	t_shell	shell;

// 	init_shell(&shell);
// 	while (1)
// 	{
// 		shell.args = readline("minishell> ");
// 		if (!shell.args) // Ctrl-D
// 		{
// 			printf("exit\n");
// 			break ;
// 		}
// 		if (shell.args[0] == '\0')
// 		{
// 			free(shell.args);
// 			continue ;
// 		}
// 		add_history(shell.args);
// 		if (!validate_quotes(shell.args))
// 		{
// 			ft_error(2, &shell);
// 			free(shell.args);
// 			continue ;
// 		}
// 		shell.lexer_list = lexer(shell.args);
// 		if (!shell.lexer_list)
// 		{
// 			ft_error(1, &shell);
// 			free(shell.args);
// 			continue ;
// 		}
// 		if (parser(&shell) == -1)
// 		{
// 			fprintf(stderr, "Parser error\n");
// 			free_lexer(&shell.lexer_list);
// 			free(shell.args);
// 			continue ;
// 		}
// 		print_parsed_s(&shell);
// 		reset_shell(&shell);
// 	}
// 	return (0);
// }

// yoshiharuが最初に書いたmain
// #include <readline/history.h>
// #include <readline/readline.h>
// #include <stdlib.h>

// int	main(int ac, char **av)
// {
// 	char	*line;

// 	while (1)
// 	{
// 		line = readline("minishell> ");
// 		if (!line)
// 			break ;
// 		if (*line)
// 			add_history(line);
// 		free(line);
// 	}
// 	return (0);
// }
