/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yotakagi <yotakagi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/04 15:27:01 by yotakagi          #+#    #+#             */
/*   Updated: 2025/12/09 16:09:33 by yotakagi         ###   ########.fr       */
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

	// 引数がない場合 (`exit`)
	if (!cmd->str[1])
		// 直前のコマンドの終了ステータスで終了
		clean_exit(shell, shell->error_num);
	// 第1引数が数値でない、またはlong longの範囲を超える場合
	if (!is_numeric(cmd->str[1]) || is_overflow(cmd->str[1]))
	{
		// "numeric argument required" エラーを表示
		exit_numeric_error(cmd->str[1]);
		// 終了コード2で終了
		clean_exit(shell, 2);
	}
	// 引数が2つ以上ある場合 (`exit 1 2`)
	if (cmd->str[2])
	{
		// "too many arguments" エラーを表示し、終了はしない
		exit_too_many_args(shell);
		return (1);
	}
	// 正常な数値引数が1つだけの場合
	exit_code = ft_atoi(cmd->str[1]);
	// 終了コードをunsigned charの範囲に収めて終了
	clean_exit(shell, (unsigned char)exit_code);
	return (0);
}
