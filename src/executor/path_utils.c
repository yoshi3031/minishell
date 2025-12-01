/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayamamot <ayamamot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 16:15:54 by nagisa            #+#    #+#             */
/*   Updated: 2025/11/18 07:35:33 by ayamamot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//pathが見つからなかったり、substr失敗時全てNULLを返す
char *find_path(char **env)
{
    int i;
    i = 0;
    while (env[i])
    {
        if (!ft_strncmp(env[i], "PATH=", 5))
            // PATH=以降を取り出して返す
            return (ft_substr(env[i], 5, ft_strlen(env[i]) - 5));
        i++;
    }
    // NULLを返すとft_splitでセグフォの可能性 TODO ft_splitを確認する
    return (NULL);
}

static int	append_slash(char **path_str)
{
	char *tmp;
	size_t len;

	len = ft_strlen(*path_str);
	if (len > 0 && ft_strncmp(&(*path_str)[len - 1], "/", 1) != 0)
	{
		tmp = ft_strjoin(*path_str, "/");
		free(*path_str);
		if(!tmp)
			return (EXIT_FAILURE);
		*path_str = tmp;
	}
	return (EXIT_SUCCESS);
}

// コマンド検索用のディレクトリ一覧を準備
int init_paths_from_env(t_shell *shell)
{
    char *path_from_env;
    int i;

    // 環境変数一覧（env）から PATH= の行を探す
    path_from_env = find_path(shell->env);
	if(path_from_env == NULL)
	{
		shell->paths = NULL;
		return (EXIT_SUCCESS);
	}
    shell->paths = ft_split(path_from_env, ':');
    free(path_from_env);
	if(shell->paths == NULL)
		return(EXIT_FAILURE);//split失敗時のエラー
    i = 0;
    while (shell->paths[i])
    {
        // 末尾が / じゃないものだけに / を付けて、後でコマンド名をそのまま結合できるようにする
		if(append_slash(&shell->paths[i]) == EXIT_FAILURE)
			return (EXIT_FAILURE);
        i++;
    }
    return (EXIT_SUCCESS);
}
