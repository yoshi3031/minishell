/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yotakagi <yotakagi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/04 15:27:01 by yotakagi          #+#    #+#             */
/*   Updated: 2025/12/06 14:51:08 by yotakagi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// exitコマンドを実行する
// 引数に応じてシェルの終了コードを決定し、クリーンアップ後に終了する
// - 引数なし: 直前のコマンドの終了コードで終了
// - 数値でない引数 or オーバーフロー: エラーメッセージを表示し、終了コード2で終了
// - 引数が多すぎる: エラーメッセージを表示し、終了せずに1を返す
// - 正常な数値引数: 指定された終了コードで終了
// @param shell: シェルの状態を保持する構造体
// @param cmd: コマンドの情報を保持する構造体
// @return: 引数が多すぎる場合のみ1を返す。それ以外の場合は終了するため返らない。
int	minishell_exit(t_shell *shell, t_cmd *cmd)
{
	int	exit_code;

	if (!cmd->str[1])
		clean_exit(shell, shell->error_num);
	if (!is_numeric(cmd->str[1]) || is_overflow(cmd->str[1]))
	{
		exit_numeric_error(cmd->str[1]);
		clean_exit(shell, 2);
	}
	if (cmd->str[2])
	{
		exit_too_many_args(shell);
		return (1);
	}
	exit_code = ft_atoi(cmd->str[1]);
	clean_exit(shell, (unsigned char)exit_code);
	return (0);
}
