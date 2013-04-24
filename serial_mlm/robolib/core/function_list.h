/*!
  \file    function_list.h
  \ingroup core
  \author  Sota <jine0125@gmail.com>
  \date    2010/07/11

  \brief   割り込みで呼ぶべき関数をリストにして管理

  robolibの核。
  割り込みで呼ぶべき関数をリストにして管理。

  例)\n
   init_map()内でset_function(renew_map, LABEL_MAP);\n
   go_curve()内でset_function(control_go_curve, LABEL_DRIVE);\n
                 set_function(check_go_curve, LABEL_DRIVE_END);
 */

#ifndef __INCLUDED_FUNCTION_LIST_H__
#define __INCLUDED_FUNCTION_LIST_H__


#include "labels.h"

//! 関数リストを初期化
/*!
  リストの内容をクリアする
 */
void init_function_list(void);

//! リストに登録された関数を呼び出す
/*!
  robolibを使用する場合これをcontrol()の頭で呼ぶ。
 */
void call_function_list(void);

//! 関数リストに関数を追加
/*!
  \param fp リストに登録する関数のポインタ
  \param label 関数に付与するラベル
  \retval 0 登録成功
  \retval -1 リストに空きがない
  \retval ラベル 既に関数が登録されている

  関数ポインタとしてNULLを渡した場合は登録は行わない。
 */
int set_function(void (*fp)(void), label_t label);

//! 指定した関数をリストから削除
void remove_function(void (*fp)(void));

//! 指定したラベルの関数へのポインタを取得
/*!
  候補が複数ある場合はリストの先頭にあるものを取得する。
  \retval NULL 関数が存在しない
 */
void (*get_function(label_t label))(void);

//! 関数リストに関数が登録されているかを調べる
int search_function(void (*fp)(void));
/*!
  \param fp 検索対象の関数
  \retval 0 登録されていない
  \retval ラベル 返値のラベルをつけて登録されている
 */


//! 指定したラベルの関数をすべてリストから削除
void remove_labeled_functions(label_t label);

//! (おまけ) call_function_listを実行中かどうかを判定
/*!
  \retval 1 実行中
  \retval 0 実行中でない
  \retval 2以上 多重割り込みが発生している(数値は多重度)
 */
int is_interrupting(void);


#endif
