#include <GL/glut.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <math.h>

//-----------------------------------------------------------------------------------
// グローバル変数
//-----------------------------------------------------------------------------------
// 再生、停止フラグ
static int flg_play = 1;

// 視点情報
static double distance = 5.0, panx, pany;

// マウス入力情報
GLint mouse_button = -1;
GLint mouse_x = 0, mouse_y = 0;

// クロック
static double clock_now, clock_pre;

// 日
static double day = 0;

static const GLfloat light_position[] = { 0.0, 0.0, 0.0, 1.0 };
static const GLfloat light_ambient[] = {1.0, 1.0, 1.0, 1.0};
static const GLfloat light_diffuse[] = {1.0, 1.0, 1.0, 1.0};

static const GLfloat mat_default_color[] = { 1.0, 1.0, 1.0, 1.0 };
static const GLfloat mat_default_specular[] = { 0.0, 0.0, 0.0, 0.0 };
static const GLfloat mat_default_shininess[] = { 100.0 };
static const GLfloat mat_default_emission[] = {0.0, 0.0, 0.0, 0.0};

//-------------------------------------------------------------
// 紀元からの秒数を返す(マイクロ秒まで)
//-------------------------------------------------------------
double get_clock_now(){
  struct timeval tv;
  gettimeofday(&tv, NULL);
  return tv.tv_sec + (double)tv.tv_usec*1e-6;
}


//const float DEG2RAD = 3.14159/180;
const float DEG2RAD = 3.14159/10;

void DrawCircle(double x, double y, double radius)
{
    glPushMatrix();
    {
        glTranslatef(x, y, 0);
        glBegin(GL_LINE_LOOP);
        {
            for (int i = 0; i < 2 * M_PI / DEG2RAD; i++) {
                float degInRad = i*DEG2RAD;
                glVertex3f( cos(degInRad)*radius, sin(degInRad)*radius, 0.0);
            }
        }
        glEnd();
    }
    glPopMatrix();
}

//-----------------------------------------------------------------------------------
// 初期化
//-----------------------------------------------------------------------------------
void init(void)
{
    // クリアの値の設定
    glClearColor (0.0, 0.0, 0.0, 0.0);
    glClearDepth( 1.0 );

    glDepthFunc( GL_LESS );

    glShadeModel(GL_FLAT);

    // クロックの初期化
    clock_now = clock_pre = get_clock_now();
}

//-----------------------------------------------------------------------------------
// レンダリング
//-----------------------------------------------------------------------------------
int radian;
void display(void)
{
    // フレームバッファのクリア
    glClear (GL_COLOR_BUFFER_BIT);

    // 視点の設定
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(panx, pany, 0);
    gluLookAt(0.0, 0.0, distance, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

    // 惑星の軌道を描く
    DrawCircle(cos(radian), sin(radian), 1.0);

    /*
    int i;
    glPointSize(1.0);
    glBegin(GL_POINTS);
    for (i = 0; i < 100; i++) {
        glVertex2f((rand()%100)*0.01, (rand()%100)*0.01);
    }
    glEnd();
    */

    glutSwapBuffers();
}


//-----------------------------------------------------------------------------------
// ウィンドウリサイズのコールバック関数
//-----------------------------------------------------------------------------------
void reshape (int w, int h)
{
    glViewport (0, 0, (GLsizei) w, (GLsizei) h);

    glMatrixMode (GL_PROJECTION);
    glLoadIdentity ();
    gluPerspective(45.0, (GLfloat) w/(GLfloat) h, 1.0, 20.0);
}

//-----------------------------------------------------------------------------------
// キーボード入力のコールバック関数
//-----------------------------------------------------------------------------------
void keyboard (unsigned char key, int x, int y)
{
    switch (key) {
    case 27:
        exit(0);
        break;
    }
}

//-----------------------------------------------------------------------------------
// マウスクリックのコールバック関数
//-----------------------------------------------------------------------------------
void mouse(int button, int state, int x, int y)
{
    mouse_button = button;
    mouse_x = x; mouse_y = y;

    if(state == GLUT_UP)
        mouse_button = -1;

    glutPostRedisplay();
}

//-----------------------------------------------------------------------------------
// マウス移動のコールバック関数
//-----------------------------------------------------------------------------------
void motion(int x, int y)
{
    switch(mouse_button){
    case GLUT_LEFT_BUTTON:
        if( x == mouse_x && y == mouse_y )
            return;
        panx += (GLfloat) (x - mouse_x) / 100.0;
        pany -= (GLfloat) (y - mouse_y) / 100.0;
        break;

    case GLUT_RIGHT_BUTTON: 
        if( y == mouse_y )
            return;
        if( y < mouse_y )
        distance += (GLfloat) (mouse_y - y)/50.0;
        else
        distance -= (GLfloat) (y-mouse_y)/50.0;

        if( distance < 1.0 ) distance = 1.0;
        if( distance > 10.0 ) distance = 10.0;

        break;
    }

    mouse_x = x; mouse_y = y;

    glutPostRedisplay();
}

//-----------------------------------------------------------------------------------
// アイドル時のコールバック関数
//-----------------------------------------------------------------------------------
void idle()
{
    glutPostRedisplay();
}

//-----------------------------------------------------------------------------------
// メイン関数
//-----------------------------------------------------------------------------------
int gl_main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode ( GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize (640, 480);
    glutInitWindowPosition (50, 50);
    glutCreateWindow ("planet");

    init();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutIdleFunc(idle);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);

    glutMainLoop();

    return 0;
}
