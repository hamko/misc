/*!
  \file    install_option.h
  \ingroup core
  \date    2010/07/08

  \brief   コンパイル時にインストールするモジュールを選択する
 */

#ifndef __INCLUDED_INSTALL_OPTION_H__
#define __INCLUDED_INSTALL_OPTION_H__

/*!
  \defgroup core
  \brief ライブラリのコア

  ライブラリの基本動作を担うモジュール。
  割り込みで呼び出すべき関数を一括でリスト管理したり、コンパイルするファイルを選択したりする。
 */

#ifdef function_list_c
/*!
  \addtogroup core
  @{
  \name オプション一覧
 */
//@{

//! 関数リストに登録できる関数の最大数
#define LISTSIZE 10

#endif
//@}
// !@}

/****** Stable ******/
/*!
  \defgroup Stable
  \brief 動作確認済みのモジュール @{
 */


/*!
  \defgroup i_math
  \brief マイコン用数学関数

  テーブル参照による三角関数演算、および関連の初等的な演算を提供する。\n

  テーブルの値は線形補間して利用するが、STRICT_MODEマクロを用いることで線形補完の必要がない大きいテーブルを使うこともできる。
  ただし、コードサイズが肥大化するため推奨はされない。どうしても演算に精度が必要な場合のみ定義する。\n

  現在、ライブラリ内の関数は数学演算が必要な場合は大抵このモジュールから関数を参照するようになっている。
 */
#define __INSTALL_I_MATH__
/*!
  \addtogroup i_math
  @{
  \name オプション一覧
 */
//@{
#ifdef i_math_c

//! \def STRICT_MODE
/*!
  \brief 精度向上モード

  コードサイズが増えるので注意
 */
#define STRICT_MODE
#undef STRICT_MODE

#endif
//@}
//! @}

/*!
  \defgroup shell
  \brief デバッグ用シェル

  デバッグ用のシェルと、基本コマンドを提供するモジュール。
  任意にコマンド関数を自作してシェルに追加できるようになっている。\n

  現在ヒストリ機能などは備えられておらず、開発中(デバッグ中)。
 */
#undef __INSTALL_SHELL__

/*!
  \addtogroup shell
  @{
  \name オプション一覧
 */
//@{
#ifdef shell_c

#define MAX_STR_LEN 64 //!< 一行で許す入力文字数
#define MAX_ARG_NUM 12 //!< 引数の最大個数
#define MAX_CMD_NUM 32 //!< コマンドの最大登録数
#define LS_COL      80 //!< lsで一行に表示する最大文字数

#endif
//@}
//! @}

/*!
  \defgroup operate
  \brief 制御指令関数
  \depend \ref timer
  \sa kakitani locus

  以下に定義する制御指令関数を扱うための基本的なツールを提供する。\n

  指令関数をmain関数から呼び出すと、割り込み関数リストに制御関数、終了判定関数が登録される。\n
  付加されるラベルは、DRIVE, BROCKである。\n

  制御関数はタイヤの速度などを計算し、driveにその情報を渡すことでマシンを駆動する。\n

  指令関数は終了判定関数がリストから削除されるまで待機する。\n
  終了判定関数はあくまで指令関数の終了のためのものであり、削除されたからといって必ずしも制御関数は削除されないことに注意。\n

  制御関数の登録は、基本的にset_drive_functionを介して行う。\n
  これは足回り制御用の関数を登録するための関数であり、これを介して登録する限り、複数の足回り制御関数が干渉することはない。\n

  wait_for_controlは、制御終了までの待機を行う関数で、基本的にすべての指令関数から呼び出される。\n
 */
#define __INSTALL_OPERATE__


/*!
  \defgroup timer
  \brief タイマーと関連ツール

  マイコン起動からの時間、割り込み総回数を管理するツール。\n

  一定時間待機する関数なども備えており、多くのモジュールから参照される。
 */


#define __INSTALL_TIMER__


/*!
  \defgroup drive
  \brief 駆動輪の駆動
  \sa drive_tw

  タイヤなどのアクチュエータに対し、目標速度や角速度などの入力を受け取ってそれを実現するようにDUTYをかける。
  与える入力の種類によって異なる型が用意されており、詳しい使い方は各型の関数を参照のこと。
  driveにラベル付けされた関数は、型によらない一般的なツールである。
 */
#define __INSTALL_DRIVE__


/*!
  \defgroup drive_tw
  \brief 二輪マシンのタイヤを駆動する
  \depend \ref device \ref drive \ref map \ref i_math

  車軸中心の目標速度と目標角速度を受け取り、それを実現するようにタイヤを駆動する関数群。
 */
#define __INSTALL_DRIVE_TW__


/*!
  \defgroup run_watcher
  \brief 指定時間後走行を停止する安全装置
  \depend \ref timer \ref drive
 */
#define __INSTALL_RUN_WATCHER__


/*!
  \defgroup device
  \brief HALに物理的な意味づけを加えて扱う

  物理的な意味づけとは、具体的にはモーターやエンコーダに向きを定義したり、ラインセンサの並び順を定義したりすることを指す。
  ライブラリ内の関数はアクチュエータやセンサを呼び出す際、基本的にこのモジュールを介して行う。
 */
#define __INSTALL_DEVICE__


/*!
  \defgroup map
  \brief マシンの現在位置計算
  \depend \ref device
  \sa duty_odometry

  マシンの現在位置計算、およびそれに関連するツール群。\n

  基本的には計算された位置情報を元にした座標変換をサポートするためツールとして提供されるが、
  使用頻度が高いためエンコーダでの位置計算モジュールも同梱されている。本来は分けるべきな気もするが・・\n

  マシンの位置情報は生の計算値だけでは扱いに不便で、実用上は出発点から見て、目標地点から見て、
  といった異なる原点からの座標情報が必要な場合が多い。このモジュールではこのような必要な視点を管理するリストを提供する。\n

  座標と原点の組をマップと呼ぶ。同じ位置を表すマップ間には親子関係が定義できる。
  すなわち、同じ位置を表すのに、新たにマップBの原点を、マップAからみた座標で定義するとき、Aは子マップBの親マップという。\n

  生の計算値から得たマップには親マップが定義できない。これをルートマップと呼ぶ。すべてのマップはルートマップの子マップである。
 */
#define __INSTALL_MAP__
/*!
  \addtogroup map
  @{
  \name オプション一覧
 */
//@{
#ifdef map_c

#define LISTSIZE 10 //!< リストに保持できるマップの数

#endif
#ifdef odometry_c

#define CYCLES_PER_SEC 1000 // !< 1secあたりの割り込み回数
/*!<
 ここは初期化時に引数として設定できるようにするべき?
 */

#endif
//@}
//! @}

//! @}



/****** Unstable ******/
/*!
  \defgroup Unstable
  \brief 動作確認されていないモジュール 
  @{
 */

/*!
  \defgroup util_frac
  \brief 分数演算

  このモジュールは分数型を定義し、それらの間の四則演算を提供する。\n

  ライブラリ内では整数型での精度を確保するため様々な単位系を用いるが、それらの間の変換はユーザーが逐一単位系を調べて行う必要がある。
  これらを解決するには値と単位の組を分数で表現して保持すればよく、単位の変換を意識せずに計算ができて便利である。\n

  ただし、C言語は演算子のオーバーロードをサポートしておらず、四則演算をすべて関数の形で表す必要があり煩雑となるため、
  ライブラリの標準演算としては採用されていない。
 */
#undef __INSTALL_UTIL_FRAC__


/*!
  \defgroup duty_odometry
  \brief Dutyの積算により簡易的なマップを生成
  \depend \ref device \ref map \ref i_math

  センサを用いないマップ生成。\n
  このマップを利用することで、エンコーダ、およびマップの利用を前提とした機能へアクセスが可能になる。\n
  ただし精度はまったく期待できない。
 */
#undef __INSTALL_DUTY_ODOMETRY__


/*!
  \defgroup cordic
  \brief CORDIC法による初等関数計算

  CORDIC法とはCOordinate Rotation DIgital Computer methodの略称で、三角関数、双曲線関数をシフトと和のみで計算するアルゴリズムである。
  テーブルを参照するi_mathに比べると計算にやや時間を要するが、比較的高速であること、同程度の精度が出せること、
  大きなテーブルを用意する必要がないことなどが利点である。
  またビット数が半減するi_mathとは異なりベクトルの長さを精度を保って計算することができる。\n

  アルゴリズムの概要を簡潔に記すと、角度が目標値に収束するよう制御しながらベクトルを回転させて、座標から目的の値を得る、というものである。
  tanが2の負冪になるように角度を選べば、ベクトルの長さは変化してしまうが回転をシフトと和で表すことができる。
  一回毎に回す角度と回数を固定すれば、ベクトルがどれだけ縮むかは予め知ることができるので、予め長さを伸ばしてから回せばベクトルの回転が実現する。\n

  詳しいアルゴリズムについては以下を参照のこと。\n

  参考文献\n
  http://en.wikipedia.org/wiki/CORDIC \n
  http://www.andraka.com/cordic.htm
 */
#define __INSTALL_CORDIC__
/*!
  \addtogroup cordic
  @{
  \name オプション一覧
 */
//@{
#ifdef cordic_c

//! 計算回数(1-15回)
#define LOOP 15
//! 何ビット高い精度で計算するか(0-10 bit)
#define SHIFT 7

#endif
//@}
//! @}

/*!
  \defgroup kakitani
  \brief 柿谷制御
  \depend \ref device \ref map \ref drive \ref drive_tw \ref operate \ref i_math \ref timer

  カーブを描きながら目標の直線に向かって走行する。信地旋回用の関数も含む。\n
  指定したmap,submapから得た座標をもとにv,omegaを計算し、drive_twに渡す。\n
  指令関数はoperateの定義に準拠する。\n

  用意されている終了判定関数は、判定と同時に制御関数を削除し、動輪を停止させるように設定されている。\n

  指令関数には通常のものと、_withなるサフィックスを付加したものを用意してある。\n
  _withでは終了判定関数をユーザーが指定できるようになっており、
  例えば壁への衝突をもって制御を終了とすることや、そもそも終了を判定しない、といったことが可能である。\n

  内部の演算にはi_mathを用いる。\n
  マシンを停止させるときにはdrive_stopが呼び出される。動作終了後この関数がfunction_listに登録されることに注意すること。\n

  基本的に、エンコーダを備えた二輪マシンが想定されているが、
  mapの指定を変更したり、drive_twに別の足回り形態への変換関数を用意することで他の構成のマシンにも流用できる。
*/
#define __INSTALL_KAKITANI__


/*!
  \defgroup locus
  \brief 関数追従
  \depend \ref device \ref map \ref drive \ref drive_tw \ref operate \ref i_math \ref cordic \ref kakitani

  パラメータsの関数で表される経路(X(s),Y(s))に追従して走行する。\n
  走行速度も関数v(s)で与える。\n
  制御の理論は、ドキュメントとして詳細に記してあるのでWikiを参照のこと。\n

  v,omegaの計算にmapを用い、drive_twを通してマシンを駆動する。\n
  submapは常に目標座標を原点とすることを保証しているが、関数追従の動作途中では、角度にpiの整数倍のずれが生じうる。
  このずれは関数追従の終了時に補正される。\n
  計算過程でkakitaniの関数を利用する。\n
  指令関数はoperateの定義に準拠する。\n

  用意されている終了判定関数は、判定と同時に制御関数を削除し、動輪を停止させるように設定されている。\n

  内部の演算にはi_mathとcordicを利用している。\n
  cordicなしでも動作できるようになっているが、利用したほうが合理的な計算ができる。\n

  マシンを停止させるときにはdrive_stopが呼び出される。動作終了後この関数がfunction_listに登録されることに注意すること。\n

  基本的に、エンコーダを備えた二輪マシンが想定されているが、
  mapの指定を変更したり、drive_twに別の足回り形態への変換関数を用意することで他の構成のマシンにも流用できる。
 */
#define __INSTALL_LOCUS__
/*!
  \addtogroup locus
  @{
  \name オプション一覧
 */
//@{
#ifdef locus_c

//! 二次補正を加える
/*!
  計算量がやや増える。精度がやや上がる。なくてもほとんど違いはない。
 */
#define USE_NEWTON_METHOD
//#undef USE_NEWTON_METHOD

#endif
//@}
//! @}

/*!
  \defgroup mlm
  \brief 最尤推定ライン補正
  \depend \ref device \ref map \ref i_math

  最尤推定ライン補正とは、ラインセンサの反応点情報を実現する確率を最大化する座標のずれを計算し、
  その推定に基づいて座標のずれを修正するライン補正の手法である。ライブラリは格子状のラインに対する補正を提供する。
  どのような走行経路に対しても有効であることを特徴とする。\n

  確率および推定値の計算は解析的には困難であるので、座標のずれを微小項として近似的に計算する。
  すなわち、座標のずれが大きい場合には補正の正確さは保証されないことに注意されたい。\n

  計算式の導出や原理などは、Wikiのドキュメントを参照。
  格子状でないラインパターンに対する補正は、原理に基づき別途推定値を計算する必要がある。
*/
#define __INSTALL_MLM__


//! @}



/****** Testing ******/
/*!
  \defgroup Testing
  \brief 開発中のモジュール @{
 */


#endif
