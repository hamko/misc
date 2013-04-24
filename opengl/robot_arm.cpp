#include <GL/glut.h>
#include <stdlib.h>
#include <iostream>

using namespace std;

// 視点情報
static double dist = 5.0, pitch = 0.0, yaw = 0.0;
static double camerax, cameray, cameraz;
// 関節情報
static int shoulder = 0, elbow = 0, hand = 0;
// マウス入力情報
GLint mouse_button = -1;
GLint mouse_x = 0, mouse_y = 0;

// Objectの色を設定する
void set_color_red() {const GLfloat mat_default_color[] = { 1.0, 0.0, 0.0, 0.0 }; glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_default_color); glMaterialfv(GL_FRONT, GL_AMBIENT, mat_default_color);}
void set_color_green() {const GLfloat mat_default_color[] = { 0.0, 1.0, 0.0, 0.0 }; glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_default_color); glMaterialfv(GL_FRONT, GL_AMBIENT, mat_default_color);}
void set_color_blue() {const GLfloat mat_default_color[] = { 0.0, 0.0, 1.0, 0.0 }; glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_default_color); glMaterialfv(GL_FRONT, GL_AMBIENT, mat_default_color);}

void init(void)
{
    glClearColor (0.0, 0.0, 0.0, 0.0);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    // 0番ライトの設定。GL_LIGHT0からGL_LIGHT7までは絶対にある
    // ライトの基本的なパラメータは以下の4つ
    // 位置(POSITION):      光源の位置
    // 環境光(AMBIENT):     全方向から照らされる光の設定
    // 拡散光(DIFFUSE):     ある一点から照らされる光の設定 
    // 鏡面光(SPECULAR):    オブジェクトの面で反射される光の設定
    const GLfloat light_position[] = { 1.0, 1.0, 1.0, 0.0 };
    const GLfloat light_ambient[] = {0.2, 0.2, 0.2, 1.0};
    const GLfloat light_diffuse[] = {0.5, 0.5, 0.5, 0.5};
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    // デフォルトマテリアル, 質感の設定
    // 質感の基本的なパラメータは以下の5つ
    // 環境光反応(AMBIENT):     環境光をどれだけ反射するか。
    // 拡散光反応(DIFFUSE):     拡散光をどれだけ反射するか。
    // 鏡面光反応(SPECULAR):    鏡面光をどれだけ反射するか。
    // 輝度(SHINIESS):          拡散光の角度反応。60くらいでかなり光沢感が出る。
    // 発光(EMISSION):          物体そのものが光る。周りのものは照らされない。
    const GLfloat mat_default_color[] = { 0.5, 0.5, 0.5, 1.0 };
    const GLfloat mat_default_specular[] = { 0.5, 0.5, 0.5, 1.0 };
    const GLfloat mat_default_shininess[] = { 20.0 };
    const GLfloat mat_default_emission[] = {0.0, 0.0, 0.0, 0.0};
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_default_color);
    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_default_color);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_default_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_default_shininess);
    glMaterialfv(GL_FRONT, GL_EMISSION, mat_default_emission);

}

void display(void)
{
   glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   // 視点の設定
   glMatrixMode(GL_MODELVIEW); // 行列演算のターゲットとなる行列のスタックを指定する。今後の行列の演算対象はこれに限られる　

   /* パン */
   glLoadIdentity(); // 現在の行列を単位行列に置き換える. gluLookAtで視点を変えたい時は、必ずglLoadIdentityする。
   // キャンパスの平行移動
   glTranslatef(camerax, cameray, cameraz); // キャンパスを(x, y, z)だけ動かす (パンに相当)

   /* 視点 */
   // 視点
   gluLookAt(0, 0, dist, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0); // カメラ位置x, y, z : カメラ注視点x, y, z : カメラの上方向x, y, z
   // 回転 (リンク0中心を中心に回転)
   glRotatef( -pitch, 1.0, 0.0, 0.0 );
   glRotatef( -yaw, 0.0, 1.0, 0.0 );

   /* オブジェクト */
   glPushMatrix(); // もとの見え方
   {
       glTranslatef (-1.0, 0.0, 0.0);
       glRotatef ((GLfloat) shoulder, 0.0, 0.0, 1.0);
       glTranslatef (1.0, 0.0, 0.0);
       glPushMatrix(); // 一番目のリンクの中心座標
       {
           glScalef (2.0, 0.4, 1.0);
           set_color_red();
           glutSolidCube (1.0);
       }
       glPopMatrix();

       glTranslatef (1.0, 0.0, 0.0);
       glRotatef ((GLfloat) elbow, 0.0, 0.0, 1.0);
       glTranslatef (1.0, 0.0, 0.0);
       glPushMatrix(); // 二番目のリンクの中心座標 
       {
           glScalef (2.0, 0.4, 1.0);
           set_color_blue();
           glutSolidCube (1.0);
       }
       glPopMatrix();

       glTranslatef (1.0, 0.0, 0.0);
       glPushMatrix(); // 二番目のリンクの先端
       {
           glTranslatef(0.0, 0.15, 0.0);
           glRotatef((GLfloat)-hand, 0.0, 0.0, 1.0);
           glTranslatef(0.25, 0.0, 0.0);
           glPushMatrix(); // 三番目上側リンクの中心
           {
               glScalef (0.5, 0.1, 1.0);
               set_color_green();
               glutSolidCube (1.0);
           }
           glPopMatrix();
       }
       glPopMatrix();

       glPushMatrix();// 二番目のリンクの先端
       {
           glTranslatef(0.0, -0.15, 0.0);
           glRotatef((GLfloat) hand, 0.0, 0.0, 1.0);
           glTranslatef(0.25, 0.0, 0.0);
           glPushMatrix(); // 三番目上側リンクの中心
           {
               glScalef (0.5, 0.1, 1.0);
               set_color_green();
               glutSolidCube (1.0);
           }
           glPopMatrix();
       }
       glPopMatrix();
   }
   glPopMatrix();
   glutSwapBuffers();
}

void reshape (int w, int h)
{
   glViewport (0, 0, (GLsizei) w, (GLsizei) h);
   glMatrixMode (GL_PROJECTION);
   glLoadIdentity ();
   gluPerspective(65.0, (GLfloat) w/(GLfloat) h, 1.0, 20.0);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
   glTranslatef (0.0, 0.0, -5.0);
}

void mouse(int button, int state, int x, int y)
{
    mouse_button = button;
    mouse_x = x; mouse_y = y;

    if(state == GLUT_UP)
        mouse_button = -1; 

    glutPostRedisplay();
}

void motion(int x, int y)
{
    switch(mouse_button){
    case GLUT_LEFT_BUTTON://マウス左ボタン
        if( x == mouse_x && y == mouse_y )
            return;
        yaw -= (GLfloat) (x - mouse_x) / 10.0;
        pitch -= (GLfloat) (y - mouse_y) / 10.0;
        cout << yaw << " " << pitch << endl;
        break;
    case GLUT_RIGHT_BUTTON://マウス右ボタン
        if( y == mouse_y )
            return;
        if( y < mouse_y )
            cameraz += (GLfloat) (mouse_y - y)/50.0;
        else
            cameraz -= (GLfloat) (y - mouse_y)/50.0;
        if( cameraz < 1.0 ) cameraz = 1.0;
        if( cameraz > 10.0 ) cameraz = 10.0;
        break;
    }
    mouse_x = x;
    mouse_y = y;

    glutPostRedisplay();
}



// keyboard call back
void keyboard (unsigned char key, int x, int y)
{
   switch (key) {
      case 'x':
         camerax += 0.05;
         glutPostRedisplay();
         break;
      case 'X':
         camerax -= 0.05;
         glutPostRedisplay();
         break;
      case 'y':
         cameray += 0.05;
         glutPostRedisplay();
         break;
      case 'Y':
         cameray -= 0.05;
         glutPostRedisplay();
         break;
      case 'z':
         cameraz += 0.05;
         glutPostRedisplay();
         break;
      case 'Z':
         cameraz -= 0.05;
         glutPostRedisplay();
         break;
 
      case 's':
         shoulder = (shoulder + 1) % 360;
         glutPostRedisplay();
         break;
      case 'S':
         shoulder = (shoulder - 1) % 360;
         glutPostRedisplay();
         break;
      case 'e':
         elbow = (elbow + 1) % 360;
         glutPostRedisplay();
         break;
      case 'h':
         hand = (hand + 1) % 360;
         if (hand > 10) 
             hand = 10;
         if (hand < -10)
             hand = -10;
         glutPostRedisplay();
         break;
      case 'H':
         hand = (hand - 1) % 360;
         if (hand > 10) 
             hand = 10;
         if (hand < -10)
             hand = -10;
         glutPostRedisplay();
         break;
      case 'E':
         elbow = (elbow - 1) % 360;
         glutPostRedisplay();
         break;
      case 27:
         exit(0);
         break;
      default:
         break;
   }
}

int main(int argc, char** argv)
{
   cout << "type [e, E, h, H, s, S]" << endl;
   glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
   glutInitWindowSize (640, 480);
   glutInitWindowPosition (100, 100);
   glutCreateWindow (argv[0]);

   init();

   glutDisplayFunc(display);
   glutReshapeFunc(reshape);
   glutMotionFunc(motion);
   glutMouseFunc(mouse);
   glutKeyboardFunc(keyboard);
   glutMainLoop();
   return 0;
}
