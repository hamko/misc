/*!
  \file    labels.h
  \ingroup core
  \author  Sota <jine0125@gmail.com>
  \date    2010/07/11

  \brief   関数リスト用のラベル宣言
 */

#ifndef __INCLUDED_LABELS_H__
#define __INCLUDED_LABELS_H__


//! リストに登録する関数のラベル
/*!
  関数リストはラベルによってソートされる。
  ライブラリに機能を追加するにあたり新たに必要となったラベルは随時追加してよい。

  リストで先頭に来る関数のラベルは数値上は1に対応させる。
  これはsearch_functionやset_functionの返値として0を別の用途で使うためである。
 */
typedef enum {
    LABEL_TIMER = 1, //!< タイマー
    LABEL_MAP,       //!< マップ更新
    LABEL_USER_PRE,  //!< ユーザー用汎用ラベル
    LABEL_DRIVE,     //!< 足回りの制御
    LABEL_BLOCK,     //!< 足回り制御の終了判定
    LABEL_USER_POST  //!< ユーザー用汎用ラベル
} label_t;


#endif
