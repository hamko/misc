#include <GL/glut.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <math.h>

//-----------------------------------------------------------------------------------
// �����Х��ѿ�
//-----------------------------------------------------------------------------------
// ��������ߥե饰
static int flg_play = 1;

// ��������
static double distance = 5.0, panx, pany;

// �ޥ������Ͼ���
GLint mouse_button = -1;
GLint mouse_x = 0, mouse_y = 0;

// ����å�
static double clock_now, clock_pre;

// ��
static double day = 0;

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
// �����
//-----------------------------------------------------------------------------------
void init(void)
{
    // ���ꥢ���ͤ�����
    glClearColor (0.0, 0.0, 0.0, 0.0);
    glClearDepth( 1.0 );

    glDepthFunc( GL_LESS );

    glShadeModel(GL_FLAT);

    // ����å��ν����
    clock_now = clock_pre = get_clock_now();
}

//-----------------------------------------------------------------------------------
// �������
//-----------------------------------------------------------------------------------
int radian;
void display(void)
{
    // �ե졼��Хåե��Υ��ꥢ
    glClear (GL_COLOR_BUFFER_BIT);

    // ����������
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(panx, pany, 0);
    gluLookAt(0.0, 0.0, distance, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

    // �����ε�ƻ������
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
    switch (key) {
    case 27:
        exit(0);
        break;
    }
}

//-----------------------------------------------------------------------------------
// �ޥ�������å��Υ�����Хå��ؿ�
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
// �ޥ�����ư�Υ�����Хå��ؿ�
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
// �����ɥ���Υ�����Хå��ؿ�
//-----------------------------------------------------------------------------------
void idle()
{
    glutPostRedisplay();
}

//-----------------------------------------------------------------------------------
// �ᥤ��ؿ�
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
