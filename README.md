# Minishell - As beautiful as a shell

![Minishell](https://img.shields.io/badge/42-Minishell-000000?style=for-the-badge&logo=42&logoColor=white)

## 概要 (Overview)
このプロジェクトは、C言語を使用して独自のUnixシェルをゼロから作成する課題です。
Bashをモデルにしており、プロセスの作成、ファイルディスクリプタの操作、シグナル処理、メモリ管理など、Unixシステムプログラミングの核心を深く学びました。

## 特徴 (Features)
私たちのMinishellは以下の機能をサポートしています：

- **コマンド実行**: 絶対パス、相対パス、環境変数 `PATH` に基づくコマンドの検索と実行
- **リダイレクト**:
  - `<` : 入力リダイレクト
  - `>` : 出力リダイレクト（上書き）
  - `>>` : 出力リダイレクト（追記）
  - `<<` : ヒアドキュメント (Here-document)
- **パイプ (`|`)**: 複数のコマンドを接続し、出力を次の入力へ渡す機能
- **環境変数**: `$` を使用した変数の展開（`$?` による終了ステータス取得も対応）
- **クォート処理**:
  - `' '` (シングルクォート): 全てのメタ文字を無効化
  - `" "` (ダブルクォート): `$` 以外のメタ文字を無効化
- **シグナルハンドリング**: `Ctrl-C`, `Ctrl-D`, `Ctrl-\` の適切な挙動
- **ビルトインコマンド**:
  - `echo` (with `-n`)
  - `cd`
  - `pwd`
  - `export`
  - `unset`
  - `env`
  - `exit`

## インストールと実行 (Installation & Usage)

### 要件
- GCC or Clang
- Make
- Readline library

### ビルド方法
リポジトリをクローンし、`make` コマンドを実行してください。

```bash
git clone [リポジトリのURL] minishell
cd minishell
make
```
### 実行方法
起動するとプロンプトが表示されます。通常のシェルと同様にコマンドを入力してください。

```bash
./minishell
```

## 開発者 (Authors)

* **[yotakagi]** - `Lexer`, `Builtin`, `Expander`
  * 入力文字列の字句解析、環境変数の展開、および主要なビルトインコマンドの実装を担当。

* **[o4kira]** - `Parser`, `Executor`, `Signals`, `Heredoc`
  * 構文解析（AST構築）、パイプラインやリダイレクトを含むコマンド実行ロジック、シグナルハンドリングを担当。
