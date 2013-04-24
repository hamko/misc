/*!
  \file    function_list.c
  \ingroup core
  \author  Sota <jine0125@gmail.com>
  \date    2010/07/11

  \brief   割り込みで呼ぶべき関数をリストにして管理
  \todo    動作確認

  robolibの核。
  割り込みで呼ぶべき関数をリストにして管理。
 */

#include "install_option.h"
#include "function_list.h"
#include "labels.h"

#undef NULL
#define NULL ((void *)0)    //!< ヌルポインタ

//! リストの要素
typedef struct _flist{
    label_t label;          //!< 関数のラベル
    void (*fp)(void);       //!< 関数ポインタ
    struct _flist *next;    //!< 次の要素へのリンク
} flist;

static flist *alloc(void);            //!< 未使用領域から領域を確保
static void free(flist *tar);         //!< 確保した領域を解放


static flist fl_area[LISTSIZE]={};    //!< リスト用領域
static flist *fl_head = NULL;         //!< リストの先頭
static flist *fl_free = &fl_area[0];  //!< 未使用領域の先頭
static flist *fl_exe = NULL;          //!< 実行中の要素

static int _init = 0;                 //!< 初期化判別フラグ
static int _dup = 0;                  //!< 割り込みの多重度カウンタ

void call_function_list(void)
{
    _dup++;

    // 初期化
    if (!_init){
        init_function_list();
    }

    // 実行
    for(fl_exe=fl_head;fl_exe!=NULL;fl_exe=(fl_exe == NULL ? fl_head : fl_exe->next)){
        (*(fl_exe->fp))();
    }

    _dup--;
}


void init_function_list(void)
{
    int i;

    // 未使用領域のリンクを作成
    for (i=1; i<LISTSIZE; i++){
        fl_area[i-1].next = &fl_area[i];
    }
    fl_area[LISTSIZE-1].next = NULL;
    fl_free = &fl_area[0];

    // 登録リストをクリア
    fl_head = NULL;

    _init = 1;
}

// 未使用領域から領域を確保
static flist *alloc(void)
{
    flist *fl;
    if ((fl=fl_free)!=NULL){
        fl_free = fl_free->next;
        fl->next = NULL;
    }
    return fl;
}

// 確保した領域を解放
static void free(flist *tar)
{
    tar->next = fl_free;
    fl_free = tar; // tar->fpは初期化しない
}

// 関数をリストに登録する
int set_function(void (*fp)(void), label_t label)
{
    flist *fl, *fl_new;

    if (fp == NULL){
        return 0;
    }

    // 手前まで辿る
    for (fl=fl_head; fl != NULL; fl=fl->next){
        if (fl->next == NULL){
            break;
        }
        if (fl->label == label && fl->fp == fp){
            // 同一関数の同一ラベルでの登録は許さない
            return label;
        }
        if (fl->next->label > label){
            // 同一のlabelを持つものは登録順に並ぶ
            break;
        }
    }

    // 領域取得
    if ((fl_new = alloc())==NULL){
        return -1;
    }

    // リンク作成
    if (fl != NULL){
        fl_new->next = fl->next;
        fl->next = fl_new;
    }
    else {
        // fl == NULLになるのは初期化時(fl_head = NULL)のみ。
        // それ以外はnextがNULLになった時点で止まるはず
        fl_head = fl_new;
    }

    fl_new->label = label;
    fl_new->fp    = fp;

    return 0;
}

// 登録した関数を削除する
void remove_function(void (*fp)(void))
{
    flist *fl, *fln;
    fl = fl_head;

    // そもそもリストに関数がない
    if (fl==NULL){
        return;
    }

    // 先頭の関数の場合(例外処理)
    if (fl->fp == fp){
        if (fl_exe == fl){
            fl_exe = NULL;
        }
        fl_head = fl->next;
        free(fl); // fl->fpはまだ上書きしない
        return;
    }

    // それ以外: 手前まで辿る
    while(fl->next!=NULL){
        if (fl->next->fp == fp){
            break;
        }
        fl=fl->next;
    }

    // リンクの貼りなおし、領域解放
    if (fl->next != NULL){
        fln = fl->next;
        if (fl_exe == fln){
            // 自己削除時のぬるぽ防止
            // (これをしないとこの関数が終わった後call_function_listの関数呼出で
            // 開放したはずのリスト要素の次を叩きに行ってしまう)
            fl_exe = fl;
        }
        fl->next = fln->next;
        free(fln);
    }
}


// 指定したラベルの関数へのポインタを取得
void (*get_function(label_t label))(void)
{
    flist *fl;

    // 手前まで辿る
    for (fl=fl_head; fl != NULL; fl=fl->next){
        if (fl->label > label){
            break;
        }
        if (fl->label == label){
            return fl->fp;
        }
    }

    return NULL;
}



// 関数リストに関数が登録されているかを調べる
int search_function(void (*fp)(void))
{
    flist *fl;

    for (fl=fl_head; fl != NULL; fl=fl->next){
        if (fl->fp == fp){
            return fl->label;
        }
    }

    return 0;
}

// 指定したラベルの関数をすべてリストから削除
void remove_labeled_functions(label_t label)
{
    flist *fl, *fln;

    while(fl_head!=NULL){
        fl = fl_head;

        // 先頭の関数の場合(例外処理)
        if (fl->label == label){
            if (fl_exe == fl){
                fl_exe = NULL;
            }
            fl_head = fl->next;
            free(fl);
            continue;
        }

        // それ以外: 手前まで辿る
        while(fl->next!=NULL){
            if (fl->next->label == label){
                break;
            }
            fl=fl->next;
        }

        // もうlabelを持つ関数はない
        if (fl->next == NULL){
            return;
        }

        // リンクの貼りなおし、領域解放
        fln = fl->next;
        if (fl_exe == fln){
            fl_exe = fl;        // 自己削除時のぬるぽ防止
        }
        fl->next = fln->next;
        free(fln);
    }
}


// 割り込み多重度判定
int is_interrupting(void)
{
    return _dup;
}
