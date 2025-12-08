/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yotakagi <yotakagi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 11:50:58 by yotakagi          #+#    #+#             */
/*   Updated: 2025/12/02 16:29:13 by yotakagi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <unistd.h>

// pwdコマンドを実行する
// カレントワーキングディレクトリを取得し、標準出力に表示する
// @param shell: シェルの状態を保持する構造体
// @param cmd: コマンドの情報を保持する構造体（この関数では未使用）
// @return: 成功した場合は0、失敗した場合は1
int	minishell_pwd(t_shell *shell, t_cmd *cmd)
{
	char	buf[4096];

	(void)cmd;
	if (getcwd(buf, sizeof(buf)))
	{
		ft_putstr_fd(buf, 1);
		write(1, "\n", 1);
		shell->error_num = 0;
	}
	else
	{
		perror("minishell: pwd");
		shell->error_num = 1;
	}
	return (shell->error_num);
}
