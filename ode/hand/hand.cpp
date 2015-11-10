#include <ode/ode.h>
#include <drawstuff/drawstuff.h>
#include "texturepath.h"

#ifdef _MSC_VER
#pragma warning(disable:4244 4305)  // for VC++, no precision loss complaints
#endif

#ifdef dDOUBLE
#define dsDrawCapsule dsDrawCapsuleD
#define dsDrawCylinder dsDrawCylinderD
#define dsDrawBox     dsDrawBoxD
#define dsDrawLine    dsDrawLineD
#define dsDrawSphere  dsDrawSphereD
#endif

#define WHEEL_NUM 4
#define GOAL_PARTS_NUM 7

static dWorldID    world;
static dSpaceID    space;
static dGeomID     ground;
static dGeomID     goal_parts[GOAL_PARTS_NUM];
static dGeomID     goal_parts2[GOAL_PARTS_NUM];

static dJointGroupID contactgroup;
static dsFunctions fn;

typedef struct
{
    dBodyID  body;
    dGeomID  geom;
    dJointID joint;
    dReal    v;         // 角速度
    dReal    m,r,x,y,z; // 質量，半径，位置(x,y,z)
    dReal    lx,ly,lz;  // sizes[3]
} MyObject;

MyObject wheel[WHEEL_NUM], base, ball;


static dReal  M = 9.4, Z = 0.1;  // 質量，初期位置の高さ
static dReal  SIDE[3] = {0.45,0.45,0.1};
static dReal  offset_z = 0.0;
static int  POWER = 100;

// 0: right panel, 1:left panel, 2:back panel, 3;bar,
// 4: right wall paper, 5: left wall paper, 6: back wall paper
static const dReal GOAL_SIDES[GOAL_PARTS_NUM][3] = {{0.635, 0.125, 1.0},
    {0.635, 0.125, 1.0}, {0.125, 2.01,   1.0}, {0.125,  2.27,  0.125},
    {0.5, 0.01, 1.0}  , {0.5, 0.01, 1.0}, {0.01, 2.01, 1.0}
};
static const dReal FIELD_SIDES[3] = {14,10,offset_z};

/*** 台車の生成 ***/
static void makeBase()
{
    dMass mass;
    base.x = base.y = 0.0;
    base.z = Z;
    base.lx = SIDE[0]; base.ly = SIDE[1]; base.lz = SIDE[2];

    base.body  = dBodyCreate(world);
    base.geom = dCreateBox(space,base.lx, base.ly, base.lz);
    dGeomSetBody(base.geom, base.body);

    dMassSetZero(&mass);
    dMassSetBoxTotal(&mass,M,base.lx, base.ly, base.lz);
    dBodySetMass(base.body,&mass);

    dBodySetPosition(base.body, base.x, base.y, base.z);
}

/*** 車輪の生成 ***/
void makeWheel()
{
    dMass mass;
    dReal r = 0.1;
    dReal w = 0.024;
    dReal m = 0.15;
    dReal d = 0.01;
    dReal tmp_z = Z;

    dReal wx[WHEEL_NUM] = {SIDE[0]/2+w/2+d, - (SIDE[0]/2+w/2+d), 0, 0};
    dReal wy[WHEEL_NUM] = {0, 0, SIDE[1]/2+w/2+d, - (SIDE[1]/2+w/2+d)};
    dReal wz[WHEEL_NUM] = {tmp_z, tmp_z, tmp_z, tmp_z};
    dReal jx[WHEEL_NUM] = {SIDE[0]/2, - SIDE[0]/2, 0, 0};
    dReal jy[WHEEL_NUM] = {0, 0, SIDE[1]/2, - SIDE[1]/2};
    dReal jz[WHEEL_NUM] = {tmp_z, tmp_z, tmp_z, tmp_z};

    for (int i = 0; i < WHEEL_NUM; i++)
    {
        wheel[i].v    = 0.0;

// make body, geom and set geom to body.
// The position and orientation of geom is abondoned.
        wheel[i].body = dBodyCreate(world);
        wheel[i].geom = dCreateCylinder(space,r, w);
        dGeomSetBody(wheel[i].geom,wheel[i].body);

// set configure of body
        dMassSetZero(&mass);
        if (i < 2) {
            dMassSetCylinderTotal(&mass,m, 1, r, w);
        } else {
            dMassSetCylinderTotal(&mass,m, 2, r, w);
        }
        dBodySetMass(wheel[i].body,&mass);

// set position and orientation of body.
        dBodySetPosition(wheel[i].body, wx[i], wy[i], wz[i]);
        dMatrix3 R;
        if (i >= 2) {
            dRFromAxisAndAngle(R,1,0,0,M_PI/2.0);
            dBodySetRotation(wheel[i].body,R);
        } else {
            dRFromAxisAndAngle(R,0,1,0,M_PI/2.0);
            dBodySetRotation(wheel[i].body,R);
        }

// make joint.
        wheel[i].joint = dJointCreateHinge(world,0);
        dJointAttach(wheel[i].joint,base.body,wheel[i].body);
        if (i < 2) {
            dJointSetHingeAxis(wheel[i].joint, 1, 0, 0);
        } else {
            dJointSetHingeAxis(wheel[i].joint, 0, -1, 0);
        }
        dJointSetHingeAnchor(wheel[i].joint, jx[i], jy[i], jz[i]);
    }
}

/*** ボールの生成 ***/
static void makeBall()
{
    dMass mass;
    ball.m = 0.45;
    ball.r = 0.11;
    ball.x = 1.0;
    ball.y = 0.0;
    ball.z = 0.14 + offset_z;

    ball.body = dBodyCreate(world);
    dMassSetZero(&mass);
    dMassSetSphereTotal(&mass,ball.m,ball.r);
    dBodySetMass(ball.body,&mass);

    ball.geom = dCreateSphere(space,ball.r);
    dGeomSetBody(ball.geom,ball.body);
    dBodySetPosition(ball.body,ball.x, ball.y, ball.z);
}

/*** ゴールの生成 ***/
static void makeGoal()
{
    // 0:right panel, 1:left panel, 2:back panel, 3;bar,
    // 4: right wall paper, 5: left wall paper, 6: back wall paper

    dReal pos[GOAL_PARTS_NUM][3] = {{-6.318, 1.08, 0.505},{-6.318, -1.08, 0.505},
        {-6.59, 0, 0.505},{-6.0625, 0, 1.07},
        {-6.25, 1.005, 0.505},{-6.25, -1.005, 0.505}, {-6.5055, 0, 0.505}
    };

    for (int i = 0; i < GOAL_PARTS_NUM; i++)
    {
        goal_parts[i] = dCreateBox(space, GOAL_SIDES[i][0],
                                   GOAL_SIDES[i][1], GOAL_SIDES[i][2]);
        dGeomSetPosition(goal_parts[i],pos[i][0], pos[i][1], pos[i][2]);
    }

    dReal pos2[GOAL_PARTS_NUM][3] = {{6.318, 1.08, 0.505},{6.318, -1.08, 0.505},
        {6.59, 0, 0.505},{6.0625, 0, 1.07},
        {6.25, 1.005, 0.505},{6.25, -1.005, 0.505}, {6.5055, 0, 0.505}
    };

    for (int i = 0; i < GOAL_PARTS_NUM; i++)
    {
        goal_parts2[i] = dCreateBox(space, GOAL_SIDES[i][0],
                                    GOAL_SIDES[i][1], GOAL_SIDES[i][2]);
        dGeomSetPosition(goal_parts2[i],pos2[i][0], pos2[i][1], pos2[i][2]+offset_z);
    }
}

/*** 台車の描画 ***/
static void drawBase()
{
    // dsSetColor(1.3, 1.3, 0.0);   // 黄色
    // dsSetColor(0.0, 0.0, 1.3);   // 青
    dsSetColor(0.1, 0.1, 0.1); // 黒
    dsDrawBox(dBodyGetPosition(base.body),dBodyGetRotation(base.body),SIDE);
}

/*** 車輪の描画 ***/
void drawWheel()
{
    dReal radius, length;
    dsSetColor(1.1,1.1,1.1);

    for (int i=0; i< WHEEL_NUM; i++)
    {
        dGeomCylinderGetParams(wheel[i].geom, &radius, &length);
        dsDrawCylinder(dGeomGetPosition(wheel[i].geom),
                       dGeomGetRotation(wheel[i].geom),length, radius);
    }
}

/*** ボールの描画 ***/
void drawBall()
{
    dsSetColor(1.0,0.0,0.0);
    dsDrawSphere(dGeomGetPosition(ball.geom),
                 dGeomGetRotation(ball.geom),dGeomSphereGetRadius(ball.geom));
}

/*** ゴールの描画 ***/
static void drawGoal()
{
    dsSetTexture(DS_NONE);
    for (int i = 0; i < GOAL_PARTS_NUM; i++)
    {
        if (i < 4)   dsSetColor(1.3, 1.3, 1.3);
        else         dsSetColor(1.3, 1.3, 0.0);
        dsDrawBox(dGeomGetPosition(goal_parts[i]),
                  dGeomGetRotation(goal_parts[i]),GOAL_SIDES[i]);

        if (i < 4)   dsSetColor(1.3, 1.3, 1.3);
        else         dsSetColor(0.0, 0.0, 1.3);
        dsDrawBox(dGeomGetPosition(goal_parts2[i]),
                  dGeomGetRotation(goal_parts2[i]),GOAL_SIDES[i]);
    }
}


/*** スタート関数 ***/
static void start()
{
    float xyz[3] = {   1.7f, -1.4f, 0.4f};
    float hpr[3] = { 144.0f, -4.0f, 0.0f};
    dsSetViewpoint(xyz,hpr);
    dsSetSphereQuality(3);
}

/*** コールバック関数 ***/
static void nearCallback(void *data, dGeomID o1, dGeomID o2)
{

    dVector3 tmp_fdir = {0, 0, 0, 0};
    dBodyID b1 = dGeomGetBody(o1);
    dBodyID b2 = dGeomGetBody(o2);
    if (b1 && b2 && dAreConnectedExcluding(b1,b2,dJointTypeContact)) return;

    int wheel_flag = 0;
    for (int j = 0; j < WHEEL_NUM; j++)
    {
        if ((o1 == wheel[j].geom)||(o2 == wheel[j].geom))
        {
            wheel_flag = 1;
            dJointGetHingeAxis(wheel[j].joint,tmp_fdir);
            break;
        }
    }

    static const int N = 10;
    dContact contact[N];
    int n = dCollide(o1,o2,N,&contact[0].geom,sizeof(dContact));
    if (n > 0)
    {
        if (wheel_flag == 1)
        {
            for (int i=0; i<n; i++)
            {
                contact[i].surface.mode =  dContactFDir1| dContactMu2 | dContactSoftERP | dContactSoftCFM;
                contact[i].fdir1[0] = tmp_fdir[0];   // 第１摩擦方向の設定(x軸成分)
                contact[i].fdir1[1] = tmp_fdir[1]; 	 // 第１摩擦方向の設定(y軸成分)
                contact[i].fdir1[2] = tmp_fdir[2];   // 第１摩擦方向の設定(z軸成分)
                contact[i].surface.mu =  0.1;        // 車軸方向の摩擦係数
                contact[i].surface.mu2 = dInfinity;  // 車輪方向の摩擦係数
                contact[i].surface.soft_erp = 0.9;
                contact[i].surface.soft_cfm = 0.001;

                dJointID c = dJointCreateContact(world,contactgroup,&contact[i]);
                dJointAttach(c,b1,b2);

            }
        }
        else
        {
            for (int i=0; i<n; i++)
            {
                contact[i].surface.mode = dContactSoftERP | dContactSoftCFM;
                contact[i].surface.mu  	= dInfinity;
                contact[i].surface.soft_erp = 0.8;
                contact[i].surface.soft_cfm = 1e-5;
                dJointID c = dJointCreateContact(world,contactgroup,&contact[i]);
                dJointAttach(c,b1,b2);
            }
        }
    }
}

/*** キーボードコマンドの処理関数 ***/
static void command(int cmd)
{
    switch (cmd)
    {
    case '1':
        float xyz1[3],hpr1[3];
        dsGetViewpoint(xyz1,hpr1);
        printf("xyz=%4.2f %4.2f %4.2f ",xyz1[0],xyz1[1],xyz1[2]);
        printf("hpr=%6.2f %6.2f %5.2f \n",hpr1[0],hpr1[1],hpr1[2]);
        break;
   case 'k':
        wheel[3].v -= 0.8;
        break; // 後進
    case 's':
        wheel[0].v = wheel[1].v = wheel[2].v = wheel[3].v = 0.0; // 停止
        break;
    case 'x':       // ゴロシュート
    {
        const dReal *bp  = dBodyGetPosition(ball.body);
        const dReal *p   = dBodyGetPosition(wheel[0].body);
        const dReal *R   = dBodyGetRotation(base.body);
        dReal dist  = sqrt(pow(bp[0]-p[0],2)+pow(bp[1]-p[1], 2));
        if (dist < 0.3)
        {
            dReal vmax = POWER * 0.01 * 8.0;
            dBodySetLinearVel(ball.body,vmax * R[0],vmax * R[4], 0.0);
            // printf("z:vx=%f vy=%f \n",vmax*R[0],vmax*R[4]);
        }
    }
    break;
    case 'l':       // ループシュート
    {
        const dReal *bp  = dBodyGetPosition(ball.body);
        const dReal *p   = dBodyGetPosition(wheel[0].body);
        const dReal *R   = dBodyGetRotation(base.body);
        dReal dist  = sqrt(pow(bp[0]-p[0],2)+pow(bp[1]-p[1],2));
        if (dist < 1.0)
        {
            dReal vmax45 = POWER * 0.01 * 8.0 / sqrt(2.0);
            dBodySetLinearVel(ball.body,vmax45 * R[0],vmax45 * R[4], vmax45);
            // printf("z:vx=%f vy=%f \n",vmax*R[0],vmax*R[4]);
        }
    }
    break;
    case 'b':
        dBodySetPosition(ball.body,0,0,0.14+offset_z);
        dBodySetLinearVel(ball.body,0,0,0);
        dBodySetAngularVel(ball.body,0,0,0);
        break;
    }
}

/*** 制  御 ***/
static void control()
{
    dReal fMax  = 10.0;

    for (int i=0; i<WHEEL_NUM;i++)
    {
        dJointSetHingeParam(wheel[i].joint, dParamVel , wheel[i].v);
        dJointSetHingeParam(wheel[i].joint, dParamFMax, fMax);
    }
    printf("POWER=%d \r",POWER);
}

/*** シミュレーションループ ***/
static void simLoop(int pause)
{
    if (!pause)
    {
        dSpaceCollide(space,0,&nearCallback); // add
        control();
        dWorldStep(world, 0.01);
        dJointGroupEmpty(contactgroup); // add
    }

    const dReal *linear_vel = dBodyGetLinearVel(base.body);
    const dReal *angular_vel = dBodyGetAngularVel(base.body);
    printf("linear : %.3f %.3f %.3f\n", linear_vel[0], linear_vel[1], linear_vel[2]);
    printf("angular: %.3f %.3f %.3f\n", angular_vel[0], angular_vel[1], angular_vel[2]);

    drawBase();
    drawWheel();  // add
    drawBall();   //add
    drawGoal();
}

/*** 描画関数の設定 ***/
static void setDrawStuff()
{
    fn.version = DS_VERSION;
    fn.start   = &start;
    fn.step    = &simLoop;
    fn.command = &command;
    fn.path_to_textures = "textures";
}

/*** 全方向移動型ロボットの生成 ***/
void makeOmni()
{
    makeBase();
    makeWheel();
}

/*** メイン関数 ***/
int main(int argc, char *argv[])
{
    dInitODE(); // ODEの初期化
    setDrawStuff(); // 描画関数の設定
    world        = dWorldCreate();              // ワールドの生成
    space        = dHashSpaceCreate(0);         // スペースの生成
    contactgroup = dJointGroupCreate(0);        // 接触点グループの生成
    ground       = dCreatePlane(space,0,0,1,0); // 地面の生成
    dWorldSetGravity(world, 0.0, 0.0, - 9.8); // 重力加速度の設定
    dWorldSetCFM(world,1e-3); // CFMの設定
    dWorldSetERP(world,0.8);  // ERPの設定

    makeOmni(); // 全方向移動ロボットの生成
    makeBall(); // ボールの生成
    makeGoal(); // ゴールの生成
    dsSimulationLoop(argc,argv,640,480,&fn); // シミュレーションループ

    dJointGroupDestroy(contactgroup);  // 接触点グループの破壊
    dSpaceDestroy(space);              // スペースの破壊
    dWorldDestroy(world);              // ワールドの破壊
    dCloseODE(); // ODEの終了
    return 0;
}
