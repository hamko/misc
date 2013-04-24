#ifndef __SUPERPARAMS_H_INCLUDED__
#define __SUPERPARAMS_H_INCLUDED__

#define MAX_OPERATOR_NUM 16
 
/* マクロ */
#define abs(a) ((a) > 0 ? (a) : -(a))

/* スーパーパラメータ */
//遺伝計算する世代数
#define GENERATION_NUM 64
// 遺伝計算の個体数
#define PARENT_NUM 8
// エリートの数
#define ELITE_NUM 2

// 各オペレータの重み。
// 突然変異の確率は約2%くらい？
// スーパーパラメータ同定をするなら自分で考える必要はないけど
#define WEIGHT_OP_MUTATE 2
#define MUTATE_CNUM 1
#define MUTATE_PNUM 1

#endif
