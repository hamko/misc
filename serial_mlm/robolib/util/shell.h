/*!
  \file    shell.h
  \ingroup shell
  \author  Sota <jine0125@gmail.com>
  \date    2010/08/07

  \brief   デバッグ用シェル

  このファイルに用意されているのは最小限の機能のみ。
  必要なコマンドは各自で定義してadd_command()で追加する。
 */

#include "install_option.h"
#ifndef __INSTALL_SHELL__
#error shell is not installed
#endif

#ifndef __INCLUDED_SHELL_H__
#define __INCLUDED_SHELL_H__

//! シェルの初期化
/*!
  初期設定コマンドを追加する
 */
void init_shell(void);

//! シェルにコマンドを追加する
/*!
  \return 0 成功
  \return 1 失敗
 */
int add_command(const char *name, void (*fp)(void));

//! コマンド取得
/*!
  入力文字列を引数に分解する
  (例: "echo abc  123" -> "echo","abc","123")
  \param buffer 入力文字列
  \param argc   出力用ポインタ(実行後引数の個数)
  \param argv   出力用ポインタ(実行後各引数)
  \param buffer_size バッファの要素数
  \param max_argc   argvの要素数
 */
void get_command(char buffer[], char *argc, char *argv[], int buffer_size, int argv_size);

//! 引数の個数を取得
char get_argc(void);

//! 引数へのポインタ配列を取得
char **get_argv(void);

//! シェルを実行する
void execute_shell(void);

#endif
