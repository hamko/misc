#include <GL/glut.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <math.h>
#include <hamkoutil.hpp>

//-----------------------------------------------------------------------------------
// �����Х��ѿ�
//-----------------------------------------------------------------------------------
// ��������ߥե饰
static int flg_play = 1;

// ��������
static double g_distance = 5.0, pitch = 0.0, yaw = 0.0;

// �ޥ������Ͼ���
GLint mouse_button = -1;
GLint mouse_x = 0, mouse_y = 0;

// ����å�
static double clock_now, clock_pre;

// ��
static double g_counter = 0;

static const GLfloat light_position[] = { 0.0, 0.0, 0.0, 1.0 };
static const GLfloat light_ambient[] = {1.0, 1.0, 1.0, 1.0};
static const GLfloat light_diffuse[] = {1.0, 1.0, 1.0, 1.0};

static const GLfloat mat_default_color[] = { 1.0, 1.0, 1.0, 1.0 };
static const GLfloat mat_default_specular[] = { 0.0, 0.0, 0.0, 0.0 };
static const GLfloat mat_default_shininess[] = { 100.0 };
static const GLfloat mat_default_emission[] = {0.0, 0.0, 0.0, 0.0};

//-------------------------------------------------------------
// ����������ÿ����֤�(�ޥ������äޤ�)
//-------------------------------------------------------------
const float DEG2RAD = 3.14159/180;

void DrawCircle(double radius)
{
    glBegin(GL_LINE_LOOP);
    {
        for (int i = 0; i < 360; i++) {
            float degInRad = i*DEG2RAD;
            glVertex3f( cos(degInRad)*radius, sin(degInRad)*radius, 0.0f );
        }
    }
    glEnd();
}
void DrawPlanet(double radius )
{
    glPushMatrix();
    {
        glRotatef ( 90.0, 1.0, 0.0, 0.0);
        glutWireSphere(radius, 20, 20);
    }
    glPopMatrix();
}

//-----------------------------------------------------------------------------------
// �����
//-----------------------------------------------------------------------------------
void init(void)
{
    // ���ꥢ���ͤ�����
    glClearColor (0.0, 0.0, 0.0, 0.0);
    glClearDepth( 1.0 );

    // �ǥץ��ƥ��Ȥ�Ԥ�
    glEnable( GL_DEPTH_TEST );
    glDepthFunc( GL_LESS );

    glShadeModel(GL_SMOOTH);

    // �ǥե���ȥ饤��
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_ambient);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    // �ǥե���ȥޥƥꥢ��
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_default_color);
    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_default_color);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_default_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_default_shininess);

    // ����å��ν����
    clock_now = clock_pre = get_clock_now();
}

//-----------------------------------------------------------------------------------
// �������
//-----------------------------------------------------------------------------------
void display(void)
{
    // ����å��ι���
    clock_pre = clock_now;
    clock_now = get_clock_now();

    // 1�ä�1��ʬư���Ȥ���
    if( flg_play )
        g_counter += (clock_now - clock_pre);

    // �ե졼��Хåե��Υ��ꥢ
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // ����������
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0.0, 0.0, g_distance, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
    glRotatef( -pitch, 1.0, 0.0, 0.0 );
    glRotatef( -yaw, 0.0, 1.0, 0.0 );

    //����
//    glPushMatrix();
//    {
//        glRotatef(g_counter * rev_vel_moon, 0.0, 1.0, 0.0); 
//        glTranslatef(orbit_radius_moon, 0.0, 0.0); 
//    }
//    glPopMatrix();
    DrawCircle(0.4);

    glutSwapBuffers();
}


//-----------------------------------------------------------------------------------
// ������ɥ��ꥵ�����Υ�����Хå��ؿ�
//-----------------------------------------------------------------------------------
void reshape (int w, int h)
{
    glViewport (0, 0, (GLsizei) w, (GLsizei) h);

    glMatrixMode (GL_PROJECTION);
    glLoadIdentity ();
    gluPerspective(45.0, (GLfloat) w/(GLfloat) h, 1.0, 20.0);
}

//-----------------------------------------------------------------------------------
// �����ܡ������ϤΥ�����Хå��ؿ�
//-----------------------------------------------------------------------------------

void keyboard (unsigned char key, int x, int y)
{
    cout << key << " " << x << " " << y << endl;
    switch (key) {
    case 'z':
        flg_play = !flg_play;
        usleep(1000);
        break;
    case 27:
        exit(0);
        break;
    }
}

// key: GLUT_KEY_LEFT, GLUT_KEY_UP, GLUT_KEY_RIGHT, GLUT_KEY_DOWN
void special_keyboard (int key, int x, int y)
{
    cout << key << " " << x << " " << y << endl;
}

//-----------------------------------------------------------------------------------
// �ޥ�������å��Υ�����Хå��ؿ�
//-----------------------------------------------------------------------------------
void mouse(int button, int state, int x, int y)
{
    cout << button << " " << state << " " << x << " " << y << endl;
    mouse_button = button;
    mouse_x = x; mouse_y = y;

    if(state == GLUT_UP)
        mouse_button = -1;

    glutPostRedisplay();
}

//-----------------------------------------------------------------------------------
// �ޥ�����ư�Υ�����Хå��ؿ�
//-----------------------------------------------------------------------------------
void motion(int x, int y)
{
    cout << " " << x << " " << y << endl;
    /*
    switch(mouse_button){
    case GLUT_LEFT_BUTTON:
        if( x == mouse_x && y == mouse_y )
            return;
        yaw -= (GLfloat) (x - mouse_x) / 10.0;
        pitch -= (GLfloat) (y - mouse_y) / 10.0;
        break;

    case GLUT_RIGHT_BUTTON: 
        if( y == mouse_y )
            return;
        if( y < mouse_y )
        g_distance += (GLfloat) (mouse_y - y)/50.0;
        else
        g_distance -= (GLfloat) (y-mouse_y)/50.0;

        if( g_distance < 1.0 ) g_distance = 1.0;
        if( g_distance > 10.0 ) g_distance = 10.0;

        break;
    }
    */

    mouse_x = x; mouse_y = y;

    glutPostRedisplay();
}

//-----------------------------------------------------------------------------------
// �����ɥ���Υ�����Хå��ؿ�
//-----------------------------------------------------------------------------------
void idle()
{
    glutPostRedisplay();
}

//-----------------------------------------------------------------------------------
// �ᥤ��ؿ�
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
    glutSpecialFunc(special_keyboard);
    glutIdleFunc(idle);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);

    glutMainLoop();

    return 0;
}
