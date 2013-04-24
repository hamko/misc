#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <GL/gl.h>
#include <GL/glut.h>
#include <AR/gsub.h>
#include <AR/video.h>
#include <AR/param.h>
#include <AR/ar.h>

#define MAX(a,b) ((a)>(b)?(a):(b))

/******************/
// Pattern Definition
/******************/
// 認識するパターン一覧
enum _pattern_names {
    PATTERN_HIRO,
    PATTERN_SQUARE,
    PATTERN_SPHERE,
    PATTERN_CONE,
    PATTERN_CHRA,
    PATTERN_CHRB,
    PATTERN_CHRC,
    PATTERN_CHRD,
    PATTERN_CHRE,
    PATTERN_CHRF,
    PATTERN_ONE,
    PATTERN_TWO,
    PATTERN_THREE,
    PATTERN_FOUR,
    PATTERN_FIVE,
    PATTERN_SIX,
    PATTERN_PROFILE,
    PATTERN_LEFT,
    PATTERN_RIGHT,
    PATTERN_TETRIS,
    _MAX_PATTERN_NUM
};

/******************/
// Window Parameters.
/******************/
int xsize, ysize;

/******************/
// Detection Parameters.
/******************/
int thresh = 100;
int count = 0;

/******************/
// Camera configuration.
/******************/
char *vconf = "";
// カメラのキャリブレーションデータ
char *cparam_name    = "Data/camera_para.dat";
ARParam cparam;

/******************/
// Function Declaration.
/******************/
static void init(void);
static void cleanup(void);
static void keyEvent( unsigned char key, int x, int y);
static void mainLoop(void);
static void define_objects(void);

/******************/
// ARObject
/******************/
// パターンの数。実は_MAX_PATTERN_NUMと一致するのでいらない。
#define OBJECT_NUM 20
// 認識できたかどうかを記憶しておくリングバッファの大きさ
#define VRING_SIZE 10
class ARObject {
    public:
        int counter;    // オブジェクトが生成されてからの時間
        int vring[VRING_SIZE]; // 認識できたかどうかを記憶しておくリングバッファの大きさ
        int mark_id;    // マーカの識別番号。
        int visible;    // ある瞬間で、マーカが見えたかどうかを判別するための変数
        double center[2];   // マーカの中心位置（スクリーン座標系）
        double width;       // マーカの大きさ (mm) 
        double trans[3][4]; // カメラ座標系からマーカ座標系への変換行列
        int patt_id;        // ARToolkit内部でのディスクリプタのようなもの
        
        // 一辺widthの_mark_idのマーカを_patt_nameから作り、ARObjectを生成する。
        ARObject(int _mark_id, char* _patt_name, int _width);

        // 毎制御周期呼ばれる関数。仮想関数。
        virtual void control(void) {
            fprintf(stderr, "not overrided!\n");
        }
        // ringbufferの更新
        void revise_visible_ringbuf(void) {
            vring[(counter++)%VRING_SIZE] = visible;
        }
        // ringbufferの表示　
        void print_revise_visible_ringbuf(void) {
            for (int i = 0; i < VRING_SIZE; i++)
                printf("%d", vring[i]);
            printf("\n");
        }
        // 過去VRING_SIZEで何個そのマーカが見えたかを数える
        int countVisibleNum(void) {
            int ret = 0;
            for (int i = 0; i < VRING_SIZE; i++)
                ret += vring[i];
            return ret;
        }

};

ARObject::ARObject(int _mark_id, char* _patt_name, int _width)
{
    mark_id = _mark_id;
    width = _width;
    if((patt_id = arLoadPatt(_patt_name)) < 0 ) {
        fprintf(stderr, "%s, pattern load error !!\n", _patt_name);
        exit(0);
    }
    counter = 0;
    for (int i = 0; i < VRING_SIZE; i++) 
        vring[i] = 0;
}

// ARObject array
ARObject* object[OBJECT_NUM];

/******************/
// HiroObject
/******************/
class HiroObject : public ARObject {
    public:
        HiroObject(int _mark_id, char* _patt_name, int _width) : ARObject(_mark_id, _patt_name, _width) {
            printf("Constuctor: Hiro Object\n");
        }
        virtual void control(void);
};

void HiroObject::control(void)
{
    if (!visible)
        return;

    printf("I want to control Hiro");
    double    gl_para[16];
    // 光の設定
    GLfloat   mat_ambient[]     = {0.0, 0.0, 1.0, 1.0};
    GLfloat   mat_flash[]       = {0.0, 0.0, 1.0, 1.0};
    GLfloat   mat_flash_shiny[] = {50.0};
    GLfloat   light_position[]  = {100.0,-200.0,200.0,0.0};
    GLfloat   ambi[]            = {0.1, 0.1, 0.1, 0.1};
    GLfloat   lightZeroColor[]  = {0.9, 0.9, 0.9, 0.1};
    
    // Depthの設定
    argDrawMode3D();
    argDraw3dCamera( 0, 0 );
    glClearDepth( 1.0 );
    glClear(GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    
    /* load the camera transformation matrix */
    argConvGlpara(trans, gl_para);
    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixd(gl_para);

    // 表示
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambi);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightZeroColor);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_flash);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_flash_shiny);	
    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMatrixMode(GL_MODELVIEW);
    glTranslatef( 0.0, 0.0, 25.0 );
    glutSolidTeapot(40.0);
    glDisable( GL_LIGHTING );

    glDisable( GL_DEPTH_TEST );
}

/******************/
// SquareObject
/******************/
class SquareObject : public ARObject {
    public:
        SquareObject(int _mark_id, char* _patt_name, int _width) : ARObject(_mark_id, _patt_name, _width) {
            printf("Constuctor: Square Object\n");
        }
        virtual void control(void);
};

void SquareObject::control(void)
{
    if (!visible)
        return;

    printf("I want to control Square");
    double    gl_para[16];
    GLfloat   mat_ambient[]     = {0.0, 1.0, 1.0, 1.0};
    GLfloat   mat_flash[]       = {0.0, 1.0, 1.0, 1.0};
    GLfloat   mat_flash_shiny[] = {50.0};
    GLfloat   light_position[]  = {100.0,-200.0,200.0,0.0};
    GLfloat   ambi[]            = {0.1, 0.1, 0.1, 0.1};
    GLfloat   lightZeroColor[]  = {0.9, 0.9, 0.9, 0.1};
    
    argDrawMode3D();
    argDraw3dCamera( 0, 0 );
    glClearDepth( 1.0 );
    glClear(GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    
    /* load the camera transformation matrix */
    argConvGlpara(trans, gl_para);
    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixd( gl_para );

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambi);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightZeroColor);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_flash);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_flash_shiny);	
    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMatrixMode(GL_MODELVIEW);
    glTranslatef( 0.0, 0.0, 25.0 );
    glutSolidCube(40.0);
    glDisable( GL_LIGHTING );

    glDisable( GL_DEPTH_TEST );

}

/******************/
// SphereObject
/******************/
class SphereObject : public ARObject {
    public:
        SphereObject(int _mark_id, char* _patt_name, int _width) : ARObject(_mark_id, _patt_name, _width) {
            printf("Constuctor: Sphere Object\n");
        }
        virtual void control(void);
};

void SphereObject::control(void)
{
    if (!visible)
        return;

    printf("I want to control Sphere");
    double    gl_para[16];
    GLfloat   mat_ambient[]     = {0.0, 0.0, 1.0, 1.0};
    GLfloat   mat_flash[]       = {0.0, 0.0, 1.0, 1.0};
    GLfloat   mat_flash_shiny[] = {50.0};
    GLfloat   light_position[]  = {100.0,-200.0,200.0,0.0};
    GLfloat   ambi[]            = {0.1, 0.1, 0.1, 0.1};
    GLfloat   lightZeroColor[]  = {0.9, 0.9, 0.9, 0.1};
    
    argDrawMode3D();
    argDraw3dCamera( 0, 0 );
    glClearDepth( 1.0 );
    glClear(GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    
    /* load the camera transformation matrix */
    argConvGlpara(trans, gl_para);
    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixd( gl_para );

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambi);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightZeroColor);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_flash);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_flash_shiny);	
    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMatrixMode(GL_MODELVIEW);
    glTranslatef( 0.0, 0.0, 25.0 );
    glutSolidSphere(40.0, 16, 16);
    glDisable( GL_LIGHTING );

    glDisable( GL_DEPTH_TEST );
}

/******************/
// ConeObject
/******************/
class ConeObject : public ARObject {
    public:
        ConeObject(int _mark_id, char* _patt_name, int _width) : ARObject(_mark_id, _patt_name, _width) {
            printf("Constuctor: Cone Object\n");
        }
        virtual void control(void);
};

void ConeObject::control(void)
{
    if (!visible)
        return;

    printf("I want to control Cone");
    double    gl_para[16];
    GLfloat   mat_ambient[]     = {1.0, 0.0, 1.0, 1.0};
    GLfloat   mat_flash[]       = {1.0, 0.0, 1.0, 1.0};
    GLfloat   mat_flash_shiny[] = {50.0};
    GLfloat   light_position[]  = {100.0,-200.0,200.0,0.0};
    GLfloat   ambi[]            = {0.1, 0.1, 0.1, 0.1};
    GLfloat   lightZeroColor[]  = {0.9, 0.9, 0.9, 0.1};
    
    argDrawMode3D();
    argDraw3dCamera( 0, 0 );
    glClearDepth( 1.0 );
    glClear(GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    
    /* load the camera transformation matrix */
    argConvGlpara(trans, gl_para);
    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixd( gl_para );

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambi);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightZeroColor);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_flash);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_flash_shiny);	
    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMatrixMode(GL_MODELVIEW);
    glTranslatef( 0.0, 0.0, 25.0 );
    glutSolidCone(40.0, 100.0, 8, 8);
    glDisable( GL_LIGHTING );

    glDisable( GL_DEPTH_TEST );
}

void control_charactor(ARObject* obj, double r, double g, double b, char chara)
{
    double    gl_para[16];
    
    argDrawMode3D();
    argDraw3dCamera( 0, 0 );
    glClearDepth( 1.0 );
    glClear(GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    
    /* load the camera transformation matrix */
    argConvGlpara(obj->trans, gl_para);
    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixd( gl_para );

    glColor3f(r, g, b);
    glTranslatef( 0.0, 0.0, 25.0 );
    void* font = GLUT_BITMAP_TIMES_ROMAN_24;
    glRasterPos3f(0, 0, 0);

    // 一文字表示
    glutBitmapCharacter(font, chara);
    
    glDisable( GL_LIGHTING );
    glDisable( GL_DEPTH_TEST );
}

/******************/
// ChrAObject
/******************/
class ChrAObject : public ARObject {
    public:
        ChrAObject(int _mark_id, char* _patt_name, int _width) : ARObject(_mark_id, _patt_name, _width) {
            printf("Constuctor: ChrA Object\n");
        }
        virtual void control(void);
};

void ChrAObject::control(void)
{
    if (!visible)
        return;

    printf("I want to control ChrA");
    control_charactor(this, 0.8, 0.0, 0.0, 'A');
}

/******************/
// ChrAObject
/******************/
class ChrBObject : public ARObject {
    public:
        ChrBObject(int _mark_id, char* _patt_name, int _width) : ARObject(_mark_id, _patt_name, _width) {
            printf("Constuctor: ChrB Object\n");
        }
        virtual void control(void);
};

void ChrBObject::control(void)
{
    if (!visible)
        return;

    printf("I want to control ChrB");
    control_charactor(this, 0.8, 0.0, 0.0, 'B');
}


/******************/
// ChrCObject
/******************/
class ChrCObject : public ARObject {
    public:
        ChrCObject(int _mark_id, char* _patt_name, int _width) : ARObject(_mark_id, _patt_name, _width) {
            printf("Constuctor: ChrC Object\n");
        }
        virtual void control(void);
};

void ChrCObject::control(void)
{
    if (!visible)
        return;

    printf("I want to control ChrC");
    control_charactor(this, 0.8, 0.0, 0.0, 'C');
}


/******************/
// ChrDObject
/******************/
class ChrDObject : public ARObject {
    public:
        ChrDObject(int _mark_id, char* _patt_name, int _width) : ARObject(_mark_id, _patt_name, _width) {
            printf("Constuctor: ChrD Object\n");
        }
        virtual void control(void);
};

void ChrDObject::control(void)
{
    if (!visible)
        return;

    printf("I want to control ChrD");
    control_charactor(this, 0.8, 0.0, 0.0, 'D');
}


/******************/
// ChrEObject
/******************/
class ChrEObject : public ARObject {
    public:
        ChrEObject(int _mark_id, char* _patt_name, int _width) : ARObject(_mark_id, _patt_name, _width) {
            printf("Constuctor: ChrE Object\n");
        }
        virtual void control(void);
};

void ChrEObject::control(void)
{
    if (!visible)
        return;

    printf("I want to control ChrE");
    control_charactor(this, 0.8, 0.0, 0.0, 'E');
}


/******************/
// ChrFObject
/******************/
class ChrFObject : public ARObject {
    public:
        ChrFObject(int _mark_id, char* _patt_name, int _width) : ARObject(_mark_id, _patt_name, _width) {
            printf("Constuctor: ChrF Object\n");
        }
        virtual void control(void);
};

void ChrFObject::control(void)
{
    if (!visible)
        return;

    printf("I want to control ChrF");
    control_charactor(this, 0.8, 0.0, 0.0, 'F');
}


/******************/
// ProfileObject
/******************/
class ProfileObject : public ARObject {
    public:
        ProfileObject(int _mark_id, char* _patt_name, int _width) : ARObject(_mark_id, _patt_name, _width) {
            printf("Constuctor: Profile Object\n");
        }
        virtual void control(void);
};

void ProfileObject::control(void)
{
    if (!visible)
        return;

    printf("I want to control Profile");
    double    gl_para[16];
    
    argDrawMode3D();
    argDraw3dCamera( 0, 0 );
    glClearDepth( 1.0 );
    glClear(GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    
    /* load the camera transformation matrix */
    argConvGlpara(trans, gl_para);
    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixd( gl_para );

    glTranslatef(-20.0, -40.0, 60.0);

    // 文字列を表示
    static int timer = 0;
    timer++;
    glColor3f(0.8, 1.0 * !((timer+1) % 2), 1.0 * !((timer+1) % 2));
    char* toprint = "t120292 Ryo Wakatabe";
    void* font = GLUT_BITMAP_TIMES_ROMAN_24;
    glRasterPos3f(0, 0, 0);
    while (*toprint) {
        glutBitmapCharacter(font, *toprint);
        toprint++;
    }
}

/******************/
// FlyingSphereObject
/******************/
double angvel = 2.0;
class FlyingSphereObject : public ARObject {
    double angle; // 一個目の球の角度
    public:
        FlyingSphereObject(int _mark_id, char* _patt_name, int _width) : ARObject(_mark_id, _patt_name, _width) {
            printf("Constuctor: Flying Sphere Object\n");
            angle = 0;
        }
        virtual void control(void);
};

void FlyingSphereObject::control(void)
{
    if (!visible)
        return;
    int spnum = this->mark_id - PATTERN_ONE + 1;
    printf("sphere num: %d\n", spnum);
    printf("I want to draw FlyingSphere");

    angle += angvel;

    double    gl_para[16];
    GLfloat   mat_ambient[]     = {0.0, 1.0, MAX(1.0, angvel / 4), 1.0};
    GLfloat   mat_flash[]       = {0.0, 1.0, MAX(1.0, angvel / 4), 1.0};
    GLfloat   mat_flash_shiny[] = {50.0};
    GLfloat   light_position[]  = {100.0,-200.0,200.0,0.0};
    GLfloat   ambi[]            = {0.1, 0.1, 0.1, 0.1};
    GLfloat   lightZeroColor[]  = {0.9, 0.9, 0.9, 0.1};
    
    argDrawMode3D();
    argDraw3dCamera( 0, 0 );
    glClearDepth( 1.0 );
    glClear(GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    
    /* load the camera transformation matrix */
    argConvGlpara(trans, gl_para);
    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixd( gl_para );

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambi);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightZeroColor);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_flash);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_flash_shiny);	
    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMatrixMode(GL_MODELVIEW);

    // 回転する球を一個ずつ表示していく。
    glTranslatef( 0.0, 0.0, 25.0 );
    glRotatef(angle, 0.0, 0.0, 1.0);
    for (int i = 0; i < spnum; i++) {
        glRotatef(360.0 / spnum, 0.0, 0.0, 1.0);
        glTranslatef(30.0, 0.0, 0.0);
        glutSolidSphere(10.0, 16, 16);
        glTranslatef(-30.0, 0.0, 0.0);
    }
    printf("\n");
    glDisable( GL_LIGHTING );

    glDisable( GL_DEPTH_TEST );
}

/******************/
// SwitchObject
/******************/
class SwitchObject : public ARObject {
    public:
        int push; // ある瞬間で押されたら、その瞬間だけ1になる
        int rel; //  ある瞬間で離されたら、その瞬間だけ1になる
        SwitchObject(int _mark_id, char* _patt_name, int _width) : ARObject(_mark_id, _patt_name, _width) {
            printf("Constuctor: Switch Object\n");
            push = 0;
            rel = 0;
        }
        virtual void control(void);
};

#define rbelem(a) (vring[(counter + VRING_SIZE * 16 - a) % VRING_SIZE])
#define RECOG_CYCNUM 3
void SwitchObject::control(void)
{
    // 押されたか判定
    push = 1;
    for (int i = 1; i <= RECOG_CYCNUM; i++) {
        if (rbelem(i)) {
            push = 0;
            break;
        }
    }
    for (int i = RECOG_CYCNUM + 1; i <= 2 * RECOG_CYCNUM; i++) {
        if (!rbelem(i)) {
            push = 0;
            break;
        }
    }

    // 離されたか判定
    rel = 1;
    for (int i = 1; i <= RECOG_CYCNUM; i++) {
        if (!rbelem(i)) {
            rel = 0;
            break;
        }
    }
    for (int i = RECOG_CYCNUM + 1; i <= 2 * RECOG_CYCNUM; i++) {
        if (rbelem(i)) {
            rel = 0;
            break;
        }
    }

    printf("push: %d, rel: %d\n", push, rel);
}

/******************/
// TetrisObject
/******************/
// スイッチで動かせるオブジェクトの定義
class TetrisObject : public ARObject {
    public:
        double x;
        TetrisObject(int _mark_id, char* _patt_name, int _width) : ARObject(_mark_id, _patt_name, _width) {
            printf("Constuctor: Tetris Object\n");
            x = 0;
        }
        virtual void control(void);
};

void TetrisObject::control(void)
{
    if (!visible)
        return;

    printf("I want to control Tetris");
    double    gl_para[16];
    GLfloat   mat_ambient[]     = {0.0, 1.0, 1.0, 1.0};
    GLfloat   mat_flash[]       = {0.0, 1.0, 1.0, 1.0};
    GLfloat   mat_flash_shiny[] = {50.0};
    GLfloat   light_position[]  = {100.0,-200.0,200.0,0.0};
    GLfloat   ambi[]            = {0.1, 0.1, 0.1, 0.1};
    GLfloat   lightZeroColor[]  = {0.9, 0.9, 0.9, 0.1};
    
    argDrawMode3D();
    argDraw3dCamera( 0, 0 );
    glClearDepth( 1.0 );
    glClear(GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    
    /* load the camera transformation matrix */
    argConvGlpara(trans, gl_para);
    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixd( gl_para );

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambi);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightZeroColor);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_flash);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_flash_shiny);	
    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMatrixMode(GL_MODELVIEW);
    // 17番スイッチが押されたら右に。
    if (((SwitchObject*)object[17])->push)
        x += 40;
    // 17番スイッチが押されたら左に。
    if (((SwitchObject*)object[18])->push)
        x -= 40;
    glTranslatef( x, 0.0, 25.0 );
    glutSolidCube(40.0);
    glDisable( GL_LIGHTING );

    glDisable( GL_DEPTH_TEST );

}


// パターンを定義　
static void define_objects(void)
{
    object[0] = new HiroObject(PATTERN_HIRO, "Data/patt.hiro", 80.0);
    object[1] = new SquareObject(PATTERN_SQUARE, "Data/patt.square", 80.0);
    object[2] = new SphereObject(PATTERN_SPHERE, "Data/patt.sphere", 80.0);
    object[3] = new ConeObject(PATTERN_CONE, "Data/patt.triangle", 80.0);
    object[4] = new ChrAObject(PATTERN_CHRA, "Data/patt.a", 80.0);
    object[5] = new ChrBObject(PATTERN_CHRB, "Data/patt.b", 80.0);
    object[6] = new ChrCObject(PATTERN_CHRC, "Data/patt.c", 80.0);
    object[7] = new ChrDObject(PATTERN_CHRD, "Data/patt.d", 80.0);
    object[8] = new ChrEObject(PATTERN_CHRE, "Data/patt.e", 80.0);
    object[9] = new ChrFObject(PATTERN_CHRF, "Data/patt.f", 80.0);
    object[10] = new ProfileObject(PATTERN_PROFILE, "Data/patt.profile", 80.0);
    object[11] = new FlyingSphereObject(PATTERN_ONE, "Data/patt.one", 80.0);
    object[12] = new FlyingSphereObject(PATTERN_TWO, "Data/patt.two", 80.0);
    object[13] = new FlyingSphereObject(PATTERN_THREE, "Data/patt.three", 80.0);
    object[14] = new FlyingSphereObject(PATTERN_FOUR, "Data/patt.four", 80.0);
    object[15] = new FlyingSphereObject(PATTERN_FIVE, "Data/patt.five", 80.0);
    object[16] = new FlyingSphereObject(PATTERN_SIX, "Data/patt.six", 80.0);
    object[17] = new SwitchObject(PATTERN_LEFT, "Data/patt.left", 80.0);
    object[18] = new SwitchObject(PATTERN_RIGHT, "Data/patt.right", 80.0);
    object[19] = new TetrisObject(PATTERN_TETRIS, "Data/patt.tetris", 80.0);
}

/* main loop */
static void mainLoop(void)
{
    ARUint8         *dataPtr;
    ARMarkerInfo    *marker_info;
    int             marker_num;
    int             j, k;

    // カメラの画像のCapture
    /* grab a vide frame */
    if( (dataPtr = (ARUint8 *)arVideoGetImage()) == NULL ) {
        arUtilSleep(2);
        return;
    }
    if( count == 0 ) arUtilTimerReset();
    count++;

    argDrawMode2D();
    argDispImage( dataPtr, 0,0 );

    /* detect the markers in the video frame */
    if( arDetectMarker(dataPtr, thresh, &marker_info, &marker_num) < 0 ) {
        cleanup();
        exit(0);
    }
    for (int i = 0; i < marker_num; i++) 
        printf("%d ", marker_info[i].id);
    printf("\n");

    arVideoCapNext();

    /* check for object visibility */
    for (int i = 0; i < OBJECT_NUM; i++) {
        k = -1;
        for( j = 0; j < marker_num; j++ ) 
            if (object[i]->patt_id == marker_info[j].id && (k == -1 || marker_info[k].cf < marker_info[j].cf))
                k = j;
        if (k == -1) {
            object[i]->visible = 0;
        } else {
            object[i]->visible = 1;
            /* get the transformation between the marker and the real camera */
            arGetTransMat(&marker_info[k], object[i]->center, object[i]->width, object[i]->trans);
        }
        object[i]->revise_visible_ringbuf();
        object[i]->control();
    }

    argSwapBuffers();
}

/************************************/
// Define Key Event
/************************************/
static void keyEvent( unsigned char key, int x, int y)
{
    /* quit if the ESC key is pressed */
    if( key == 0x1b ) {
        printf("*** %f (frame/sec)\n", (double)count/arUtilTimer());
        cleanup();
        exit(0);
    }
    /* add angvel */
    if( key == 'a' ) {
        angvel += 0.5;
        printf("*** angvel is %f (dig/s)\n", angvel);
    }
    /* sub angvel */
    if( key == 's' ) {
        angvel -= 0.5;
        if (angvel < 0.0)
            angvel = 0.0;
        printf("*** angvel is %f (dig/s)\n", angvel);
    }
    /* reset angvel */
    if( key == 'r' ) {
        angvel = 2.0;
        printf("*** angvel is %f (dig/s)\n", angvel);
    }
}

/************************************/
// You need not touch below.
/************************************/
static void init( void )
{
    ARParam  wparam;
	
    /* open the video path */
    if( arVideoOpen( vconf ) < 0 ) exit(0);
    /* find the size of the window */
    if( arVideoInqSize(&xsize, &ysize) < 0 ) exit(0);
    printf("Image size (x,y) = (%d,%d)\n", xsize, ysize);

    /* set the initial camera parameters */
    if( arParamLoad(cparam_name, 1, &wparam) < 0 ) {
        printf("Camera parameter load error !!\n");
        exit(0);
    }
    arParamChangeSize( &wparam, xsize, ysize, &cparam );
    arInitCparam( &cparam );
    printf("*** Camera Parameter ***\n");
    arParamDisp( &cparam );

    define_objects();

    /* open the graphics window */
    argInit( &cparam, 1.3, 0, 0, 0, 0 );
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	init();

    arVideoCapStart();
    argMainLoop( NULL, keyEvent, mainLoop );
	return (0);
}

/* cleanup function called when program exits */
static void cleanup(void)
{
    arVideoCapStop();
    arVideoClose();
    argCleanup();
}
