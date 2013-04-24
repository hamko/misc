#ifndef _WORLD_H_
#define _WORLD_H_

typedef struct _world_pos_t {
    float x;		/* mm */
    float y;		/* mm */
    float theta;		/* rad */
    float v;		/* mm/s */
    float omega;		/* rad/s */
} world_pos_t;

//! ワールド座標系における座標のポインタを取得
extern world_pos_t* get_world_pos(void);
//! ワールド座標系の座標設定、v, omegaは前後で保存される
extern void set_world_pos(float x, float y, float theta);
//! ワールド座標系の座標設定
extern void set_world_pos_all(float x, float y, float theta, float v, float omega);

#endif //_WHEEL_H_
