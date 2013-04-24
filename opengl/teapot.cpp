#include <GL/glut.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

//-----------------------------------------------------------------------------------
// グローバル変数
//-----------------------------------------------------------------------------------
// 視点情報
static double distance = 5.0, pitch = 0.0, yaw = 0.0;

// マウス入力情報
GLint mouse_button = -1;
GLint mouse_x = 0, mouse_y = 0;

//-----------------------------------------------------------------------------------
// 初期化
//-----------------------------------------------------------------------------------
void init(void)
{
    // クリアの値の設定
    glClearColor (0.0, 0.0, 0.0, 0.0);
    glClearDepth( 1.0 );

    // デプステストを行う
    glEnable( GL_DEPTH_TEST );
    glDepthFunc( GL_LESS );

    glShadeModel (GL_SMOOTH); // 連続して作成した面を自動的に補完する。重い。
//    glShadeModel (GL_FLAT); // 連続して作成した面を自動的に補完しない。

    // 0番ライトの設定。GL_LIGHT0からGL_LIGHT7までは絶対にある
    // ライトの基本的なパラメータは以下の4つ
    // 位置(POSITION):      光源の位置
    // 環境光(AMBIENT):     全方向から照らされる光の設定
    // 拡散光(DIFFUSE):     ある一点から照らされる光の設定 
    // 鏡面光(SPECULAR):    オブジェクトの面で反射される光の設定
    const GLfloat light_position[] = { 1.0, 1.0, 1.0, 0.0 };
    const GLfloat light_ambient[] = {0.2, 0.2, 0.2, 1.0};
    const GLfloat light_diffuse[] = {1.0, 1.0, 1.0, 1.0};
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_ambient);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    // デフォルトマテリアル, 質感の設定
    // 質感の基本的なパラメータは以下の5つ
    // 環境光反応(AMBIENT):     環境光をどれだけ反射するか。
    // 拡散光反応(DIFFUSE):     拡散光をどれだけ反射するか。
    // 鏡面光反応(SPECULAR):    鏡面光をどれだけ反射するか。
    // 輝度(SHINIESS):          拡散光の角度反応。60くらいでかなり光沢感が出る。
    // 発光(EMISSION):          物体そのものが光る。周りのものは照らされない。
    const GLfloat mat_default_color[] = { 1.0, 1.0, 1.0, 1.0 };
    const GLfloat mat_default_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    const GLfloat mat_default_shininess[] = { 30.0 };
    const GLfloat mat_default_emission[] = {0.0, 0.0, 0.0, 0.0};
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_default_color);
    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_default_color);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_default_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_default_shininess);
    glMaterialfv(GL_FRONT, GL_EMISSION, mat_default_emission);
}

//-----------------------------------------------------------------------------------
// レンダリング
//-----------------------------------------------------------------------------------
void display(void)
{
    // フレームバッファのクリア
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // 視点の設定
    glMatrixMode(GL_MODELVIEW); // 行列演算のターゲットとなる行列のスタックを指定する。今後の行列の演算対象はこれに限られる　

    glLoadIdentity(); // 現在の行列を単位行列に置き換える
    // カメラ位置x, y, z : カメラ注視点x, y, z : カメラの上方向x, y, z
    gluLookAt(0.0, 0.0, distance, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
    glRotatef( -pitch, 1.0, 0.0, 0.0 );
    glRotatef( -yaw, 0.0, 1.0, 0.0 );

    // ティーポットを描く
    // 現在の行列をスタックに積む。このあとPopされとPushする直前の行列に戻る。
    // 中での操作は外側に影響しない。
    glPushMatrix();
    {
        glutSolidTeapot( 1.5 );
    }
    glPopMatrix();

    glutSwapBuffers();
}


//-----------------------------------------------------------------------------------
// ウィンドウリサイズのコールバック関数
//-----------------------------------------------------------------------------------
// resize後の座標が得られる
void reshape (int w, int h)
{
    glViewport (0, 0, (GLsizei) w, (GLsizei) h); // ビューポート方形 (=普通見えてる画面) の原点x, y長さx, y

    glMatrixMode (GL_PROJECTION);
    glLoadIdentity ();
    gluPerspective(60.0, (GLfloat) w/(GLfloat) h, 1.0, 20.0);
}

//-----------------------------------------------------------------------------------
// キーボード入力のコールバック関数
//-----------------------------------------------------------------------------------
void keyboard (unsigned char key, int x, int y)
{
    switch (key) {
    case 27://ESCで終了
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
    case GLUT_LEFT_BUTTON://マウス左ボタン
        if( x == mouse_x && y == mouse_y )
            return;
        yaw -= (GLfloat) (x - mouse_x) / 10.0;
        pitch -= (GLfloat) (y - mouse_y) / 10.0;
        break;
    case GLUT_RIGHT_BUTTON://マウス右ボタン
        if( y == mouse_y )
            return;
        if( y < mouse_y )
            distance += (GLfloat) (mouse_y - y)/50.0;
        else
            distance -= (GLfloat) (y - mouse_y)/50.0;
        if( distance < 1.0 ) distance = 1.0;
        if( distance > 10.0 ) distance = 10.0;
        break;
    }
    mouse_x = x;
    mouse_y = y;

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
int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode ( GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH );
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
