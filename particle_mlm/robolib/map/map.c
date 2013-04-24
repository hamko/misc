/*!
  \file    map.c
  \ingroup map
  \author  Sota <jine0125@gmail.com>
  \author  Akazaki <g940331@ecc.mail.u-tokyo.ac.jp>
  \author  Oka <tetramorium.tsushimae@gmail.com>

  \date    2010/07/14

  \brief   マップの基本関数、依存関係の定義

  マップは大別して、独自に座標を計算する能力を持つrootマップと、その座標変換で得られる子マップ群に分かれる。
  マップリストでは親子の依存関係を保持し、子マップの座標を生成する。
 */

#include "install_option.h"
#ifdef __INSTALL_MAP__

#include "map.h"

#undef NULL
#define NULL ((void *)0)    //!< ヌルポインタ

//! リストの要素
typedef struct _maplist{
    p_map_t map;             //!< マップの原点(親からの相対座標)
    struct _maplist *parent; //!< 親マップ
    struct _maplist *prev;   //!< 前の要素(必ずしも親ではない)
    struct _maplist *next;   //!< 次の要素(必ずしも子ではない)
    uint8_t generation;      //!< 世代数
    position_t pos;          //!< このマップの原点から見た現在座標
} mlist_t;


static mlist_t ml_area[LISTSIZE]={};   //!< マップリスト用領域
static mlist_t *ml_free = &ml_area[0]; //!< 未使用領域の先頭
static mlist_t *ml_head = NULL;        //!< リストの先頭


static mlist_t *alloc(void);            //!< 未使用領域から領域を確保
static void free(mlist_t *tar);         //!< 確保した領域を解放

static void remove_map_mlist(mlist_t *map);
static void renew_map_mlist(mlist_t *ml);                    //!< 座標変換用関数
static mlist_t *p_map_to_mlist(p_map_t map);                 //!< p_mapの入力に一致するm_listを返す関数
static mlist_t *get_common_parent(mlist_t *m1, mlist_t *m2); //!< 2つのマップの共通の親マップを検索


//! マップライブラリの初期化
/*!
  \author  Sota <jine0125@gmail.com>
 */
void init_map(void)
{
    int i;
    ml_area[0].prev = NULL;
    // 未使用領域にリンクを生成
    for (i=0;i<LISTSIZE-1;i++){
        ml_area[i].next = &ml_area[i+1];
        ml_area[i+1].prev = &ml_area[i];
    }
    ml_area[LISTSIZE-1].next = NULL;
    ml_free = &ml_area[0];

    // 登録リストをクリア
    ml_head = NULL;
}


//! 未使用領域から領域を確保
/*!
  \author  Sota <jine0125@gmail.com>
 */
static mlist_t *alloc(void)
{
    mlist_t *ml;
    if ((ml=ml_free)!=NULL){
        // mlのリスト構造部分以外はいじらない
        ml_free = ml_free->next;
        ml->next = NULL;
        ml->prev = NULL;
    }
    return ml;
}

//! 確保した領域を解放
/*!
  \author  Sota <jine0125@gmail.com>
 */
static void free(mlist_t *tar)
{
    // tarのリスト構造部分以外はいじらない
    tar->next = ml_free;
    ml_free = tar;
}


//! マップリストにマップを追加
/*!
  \author  Sota <jine0125@gmail.com>
 */
int add_map(p_map_t map, p_map_t parent)
{
    mlist_t *ml, *ml_new; // 探索用、新規登録用

    // 不正な入力を弾く
    if (map == NULL){
        return 0;
    }

    // 多重登録防止
    if (p_map_to_mlist(map) != NULL){
        return 0;
    }

    // rootマップ用の例外処理
    if (parent == NULL){
        // 領域取得
        if ((ml_new = alloc())==NULL){
            return -1;
        }
        ml_new->map = map;
        ml_new->parent = NULL;
        ml_new->generation = 0;
        ml_new->pos = (position_t){0,0,0,0,0};
        ml_new->next = ml_head;
        ml_head = ml_new; // rootマップは追加順に先頭におく
        return 0;
    }

    // 親探し
    if ((ml = p_map_to_mlist(parent)) == NULL){
        return -1;
    }

    // 領域取得
    if ((ml_new = alloc())==NULL){
        return -1;
    }

    // 親子関係を保存
    ml_new->map = map;
    ml_new->parent = ml;
    ml_new->generation = ml->generation + 1;

    // 同世代探し
    // mlは最後の年上のポインタになる
    while (ml->next!=NULL){
        if (ml->next->generation <= ml_new->generation){
            break;
        }
        ml = ml->next;
    }

    // リンク作成
    ml_new->prev = ml;
    ml_new->next = ml->next; // ml_newは同世代の先頭におく
    if (ml->next != NULL){
        ml->next->prev = ml_new;
    }
    ml->next = ml_new;

    // 新しく追加したマップの座標を更新
    renew_map(map);

    return 0;
}


//! マップリストからマップを削除
/*!
  \author  Sota <jine0125@gmail.com>
 */
int remove_map(p_map_t map)
{
    mlist_t *ml;
    if ((ml = p_map_to_mlist(map))!=NULL){
        remove_map_mlist(ml);
        return 0;
    }
    return 1;
}

//! 子マップ削除処理
/*!
  \author  Sota <jine0125@gmail.com>
 */
static void remove_map_mlist(mlist_t *map)
{
    mlist_t *ml;    //検索用

    //入力されたmlの子供に対して変更のお知らせを渡す
    for (ml=map;ml!=NULL;ml=ml->next){
        if (ml->parent == map){
            remove_map_mlist(ml);
        }
        if (ml->generation > map->generation+1){
            break;
        }
    }

    // 最下層の子供から順に領域を開放する
    if (map->prev != NULL){
        map->prev->next = map->next;
    }
    if (map->next != NULL){
        map->next->prev = map->prev;
    }
    free(map);

    return;
}



//! 子マップの座標計算
/*!
  \author  Akazaki <g940331@ecc.mail.u-tokyo.ac.jp>
 */
void renew_map(p_map_t map)
{
    //mlist型に変換して処理
    renew_map_mlist(p_map_to_mlist(map));
    return;
}

//! 子マップの座標計算の内部処理
/*!
  \author  Akazaki <g940331@ecc.mail.u-tokyo.ac.jp>
 */
static void renew_map_mlist(mlist_t *ml_input)
{
    mlist_t *ml;    //検索用
    int32_t tmp_x,tmp_y;
    
    if (ml_input == NULL){
        return;
    }

    //親マップの座標を元に座標を更新
    if (ml_input->parent != NULL){
        tmp_x = ml_input->parent->pos.x - ml_input->map->x;
        tmp_y = ml_input->parent->pos.y - ml_input->map->y;

        ml_input->pos.x = multiply_divide(tmp_x, i_cos(ml_input->map->theta), UNIT_MM) + multiply_divide(tmp_y, i_sin(ml_input->map->theta), UNIT_MM);
        ml_input->pos.y = - multiply_divide(tmp_x, i_sin(ml_input->map->theta), UNIT_MM) + multiply_divide(tmp_y, i_cos(ml_input->map->theta), UNIT_MM);
        ml_input->pos.theta = ml_input->parent->pos.theta - ml_input->map->theta;
        ml_input->pos.v = ml_input->parent->pos.v;
        ml_input->pos.omega = ml_input->parent->pos.omega;
    }

    //入力されたmlの子供に対して変更のお知らせを渡す
    for (ml=ml_input;ml!=NULL;ml=ml->next){
        if (ml->parent == ml_input){
            renew_map_mlist(ml);
        }
        if (ml->generation > ml_input->generation+1){
            break;
        }
    }
    return;
}

//! 指定したマップの座標を取得
/*!
  \author  Sota <jine0125@gmail.com>
 */
position_t get_map_pos(p_map_t map)
{
    mlist_t *ml;

    if ((ml=p_map_to_mlist(map))==NULL){
        return (position_t){0,0,0,0,0};
    }

    return ml->pos;
}


//! 指定したrootマップの座標を変更
/*!
  \author  Sota <jine0125@gmail.com>
 */
int set_root_map_pos(p_map_t root_map, position_t pos)
{
    mlist_t *ml;

    // リストに存在しない
    if ((ml = p_map_to_mlist(root_map))==NULL){
        return -1;
    }

    // rootでない
    if (ml->generation){
        return -1;
    }

    ml->pos = pos;
    renew_map(root_map);
    return 0;
}


//! p_map_t型からmlist_t型への変換
/*!
  \author  Akazaki <g940331@ecc.mail.u-tokyo.ac.jp>
 */
static mlist_t *p_map_to_mlist(p_map_t map)
{
    mlist_t *ml;    // 入力検索用
    for (ml=ml_head;ml!=NULL;ml=ml->next){
        if (ml->map == map){
            break;
        }
    }
    return ml;
}

//! 元の座標系でのベクトルが、他の座標系ではどう表されるか
/*!
  \author  Oka <tetramorium.tsushimae@gmail.com>
 */
map_t get_vector_transform(p_map_t old_map, p_map_t new_map, int32_t x, int32_t y)
{
    mlist_t *ml, *ml_0, *ml_dst, *ml_now, *ml_top;
    ml_0 = p_map_to_mlist(old_map);           // 変換前のマップ
    ml_dst = p_map_to_mlist(new_map);         // 変換後のマップ
    ml_top = get_common_parent(ml_0, ml_dst); // 両マップ共通の親マップ
    map_t tmp_map = (map_t){x,y,0};
    int32_t new_x, new_y;
    int32_t theta;

    // 共通の親マップがないときはエラー
    if (ml_top == NULL){
        return (map_t){0,0,0};
    }

    theta = 0;

    // 初期マップから共通の親まで変換
    for (ml=ml_0; ml != NULL; ml=ml->parent){
        if (ml == ml_top){
            break;
        }
        
        // 子から親への変換
        // 位置ベクトルなので角度だけ見ればいい
        theta += ml->map->theta;
    }

    // 共通の親から目標マップまで変換
    ml_now = ml_top;
    while (ml_now != ml_dst){
        // 子探し
        // ml_dstからml_nowの一つ下の世代の子探し
        for (ml=ml_dst; ml != NULL; ml=ml->parent){
            if (ml->parent == ml_now){
                break;
            }
        }
        
        // 子がいない時はエラー
        if (ml == NULL){
            break; // 最後に例外処理(ここでreturnしたほうが早い?)
        }

        //親から子への変換
        theta -= ml->map->theta;

        ml_now = ml;
    }

    // 今thetaはml_dstから見たml_0の角度
    // -> ml_0を-theta回転させればいい
    // -> ベクトルをtheta回転させればいい
    new_x = multiply_divide(tmp_map.x, i_cos(theta), UNIT_MM)
        - multiply_divide(tmp_map.y, i_sin(theta), UNIT_MM);
    new_y = multiply_divide(tmp_map.x, i_sin(theta), UNIT_MM)
        + multiply_divide(tmp_map.y, i_cos(theta), UNIT_MM);

    tmp_map.x = new_x;
    tmp_map.y = new_y;
    
    if (ml == NULL){
        return (map_t){0,0,0};
    }else{
        return tmp_map;
    }
}

//! 元の座標系での座標が、他の座標系ではどう表されるか
/*!
  \author  Oka <tetramorium.tsushimae@gmail.com>
 */
map_t get_pos_transform(p_map_t old_map, p_map_t new_map, int32_t x, int32_t y, int32_t theta)
{
    mlist_t *ml, *ml_0, *ml_dst, *ml_now, *ml_top;
    ml_0 = p_map_to_mlist(old_map);           // 変換前のマップ
    ml_dst = p_map_to_mlist(new_map);         // 変換後のマップ
    ml_top = get_common_parent(ml_0, ml_dst); // 両マップ共通の親マップ
    map_t tmp_map = (map_t){x,y,theta};
    int32_t new_x, new_y;
    
    // 共通の親マップがないときはエラー
    if (ml_top == NULL){
        return (map_t){0,0,0};
    }

    // 初期マップから共通の親まで変換
    for (ml=ml_0; ml != NULL; ml=ml->parent){
        if (ml == ml_top){
            break;
        }
        
        // 子から親への変換
        // 座標なので各変換ごとに(x, y)を回転させる
        new_x = multiply_divide(tmp_map.x, i_cos(ml->map->theta), UNIT_MM)
            - multiply_divide(tmp_map.y, i_sin(ml->map->theta), UNIT_MM)
            + ml->map->x;
        new_y = multiply_divide(tmp_map.x, i_sin(ml->map->theta), UNIT_MM)
            + multiply_divide(tmp_map.y, i_cos(ml->map->theta), UNIT_MM)
            + ml->map->y;
        tmp_map.theta += ml->map->theta;

        tmp_map.x = new_x;
        tmp_map.y = new_y;
    }
    
    // 共通の親から目標マップまで変換
    ml_now = ml_top;
    while (ml_now != ml_dst){
        // 子探し
        // ml_dstからml_nowの一つ下の世代の子探し
        for (ml=ml_dst; ml != NULL; ml=ml->parent){
            if (ml->parent == ml_now){
                break;
            }
        }

        // 子がいない時はエラー
        if (ml == NULL){
            break;
        }

        //親から子への変換
        new_x = multiply_divide(tmp_map.x - ml->map->x, i_cos(ml->map->theta), UNIT_MM)
            + multiply_divide(tmp_map.y - ml->map->y, i_sin(ml->map->theta), UNIT_MM);
        new_y = - multiply_divide(tmp_map.x - ml->map->x, i_sin(ml->map->theta), UNIT_MM)
            + multiply_divide(tmp_map.y - ml->map->y, i_cos(ml->map->theta), UNIT_MM);
        tmp_map.theta -= ml->map->theta;

        tmp_map.x = new_x;
        tmp_map.y = new_y;

        ml_now = ml;
    }
    
    if (ml == NULL){
        return (map_t){0,0,0};
    }else{
        return tmp_map;
    }
}

//! 親マップを取得する
/*!
  \author  Oka <tetramorium.tsushimae@gmail.com>
 */
p_map_t get_parent(p_map_t map)
{
    mlist_t *ml;
    
    ml = p_map_to_mlist(map);
    if (ml->parent != NULL){
        return ml->parent->map;
    }else{
        return NULL;
    }
}


//! 2つのマップの共通の親マップを検索
/*!
  \author  Sota <jine0125@gmail.com>
 */
static mlist_t *get_common_parent(mlist_t *m1, mlist_t *m2)
{
    if (m1 == NULL || m2 == NULL){
        return NULL;
    }

    // 世代を揃える
    // generationは必ず連番で存在するはず
    while(m1->generation > m2->generation){
        m1 = m1->parent;
    }
    while(m1->generation < m2->generation){
        m2 = m2->parent;
    }

    if (m1 == m2){
        return m1;
    }

    // 親が一致するまで遡る
    while(m1->parent != m2->parent){
        m1 = m1->parent;
        m2 = m2->parent;
    }
    return m1->parent; // 一致する親がなければroot->prevのNULLが返る
}

#endif
