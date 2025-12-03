/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayamamot <ayamamot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/25 14:20:47 by yotakagi          #+#    #+#             */
/*   Updated: 2025/12/03 02:52:34 by ayamamot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	heredoc_sigint_handler(int sig)
{
	(void)sig;
	g_signal = 1;
	write(1, "\n", 1);
}

static int get_var_len(char *str)
{
	int i;
	
	i = 0;
	if (!str)
		return (0);
	while (str[i] && (ft_isalnum(str[i]) || str[i] == '_'))
		i++;
	return (i);
}

static size_t calc_heredoc_len(char *str, char **env, int status)
{
	size_t	len;
	int		i;
	char	*key;
	char	*val;

	len = 0;
	i = 0;
	while(str[i])
	{
		if (str[i] == '$' && str[i + 1] == '?')
		{
			char	*num = ft_itoa(status);
			if (num)
				len += ft_strlen(num);
			free(num);
			i += 2;
		}
		else if (str[i] == '$' && (ft_isalpha(str[i + 1]) || str[i + 1] == '_'))
		{
			i++;
			int	v_len = get_var_len(&str[i]);
			key	= ft_substr(str, i, v_len);
			val = get_env_value(env, key);
			if (val)
				len += ft_strlen(val);
			free(key);
			free(val);
			i += v_len;
		}
		else
		{
			len++;
			i++;
		}
	}
	return (len);
}

// ヒアドキュメント用の変数展開関数
// (クォートの処理はせず、$VAR だけを展開する簡易版)
static char	*expand_heredoc_line(char *line, char **env, int status)
{
	char	*new_line;
	size_t	len;
	size_t	i;
	size_t	j;

	len = calc_heredoc_len(line, env, status);
	new_line = (char *)malloc(sizeof(char) * (len + 1));
	if (!new_line)
		return (NULL);
	i = 0;
	j = 0;
	while (line[i])
	{
		if(line[i] == '$' && line[i + 1] == '?')
		{
			char *num = ft_itoa(status);
			if (num)
			{
				ft_strlcpy(&new_line[j], num, ft_strlen(num) + 1);
				j += ft_strlen(num);
				free(num);
			}
			i += 2;
		}
		else if (line[i] == '$' && (ft_isalpha(line[i + 1]) || line[i + 1] == '_'))
		{
			i++;
			int v_len = get_var_len(&line[i]);
			char *key = ft_substr(line, i, v_len);
			char *val = get_env_value(env, key);
			if(val)
			{
				ft_strlcpy(&new_line[j], val, ft_strlen(val) + 1);
				j += ft_strlen(val);
				free(val);
			}
			free(key);
			i += v_len;
		}
		else
		{
			new_line[j++] = line[i++];
		}
	}
	new_line[j] = '\0';
	return (new_line);
}
// イベントフック用（Ctrl-C検知）
int	heredoc_signal_check(void)
{
	if (g_signal)
		rl_done = 1; // readlineを強制終了させる
	return (0);
}

int	read_heredoc(const char *delimiter, bool expand, char **env, int status)
{
	char	*line;
	int		pipe_fd[2];

	if (pipe(pipe_fd) == -1)
	{
		perror("minishell: pipe");
		return (-1);
	}

	signal(SIGINT, heredoc_sigint_handler);
	rl_event_hook = heredoc_signal_check;

	while (1)
	{
		line = readline("> ");
		if (g_signal)
		{
			free(line);
			close(pipe_fd[1]);
			close(pipe_fd[0]);
			// パイプなどを閉じて、エラーコードを返す特別な処理が必要
			// 呼び出し元で「実行をキャンセル」する判断ができるようにする
			//Parser側で read_heredoc がシグナル中断（-1など）を返した場合
			//その後のコマンド実行やParser処理をすべて中止して、メインループに戻る実装
			// g_signal = 0;
			return(-1);
		}
		if (!line)
		{
			//ToDo メッセージの修正
			ft_putstr_fd("minishell: warning: here-document delimited by end-of-file\n", 2);
			break ;
		}
		if (ft_strcmp(line, delimiter) == 0)
		{
			free(line);
			break ;
		}
		if (expand)
		{
			char *expanded = expand_heredoc_line(line, env, status);
			free(line);
			line = expanded;
		}
		write(pipe_fd[1], line, ft_strlen(line));
		write(pipe_fd[1], "\n", 1);
		free(line);
	}
	rl_event_hook = NULL;
	init_signals();
	close(pipe_fd[1]);
	return (pipe_fd[0]);
}
