// glDrawPixels(), glCopyPixels(), and glPixelZoom().
#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include "readjpeg.h"

/*	Create checkerboard image	*/
#define	checkImageWidth 64
#define	checkImageHeight 64
GLubyte checkImage[checkImageHeight][checkImageWidth][3];
GLubyte *image = NULL;
GLsizei imageWidth, imageHeight, imageDepth;
GLenum	imageFormat;

static GLdouble zoomFactor = 1.0;
static GLint height;

void makeCheckImage(void)
{
    int i, j, h;

    for (i = 0; i < checkImageWidth; i++) 
        for (j = 0; j < checkImageHeight; j++) 
            for (h = 0; h < 3; h++) 
                checkImage[i][j][h] = (GLubyte)(!(i&8)^!(j&8))*255;
}

void init(char* filename)
{
    if(!(image = readJPEGimage(filename, &imageWidth, &imageHeight, &imageDepth))){
        fprintf(stderr,"JPEG Image file read Error!!\n");
        exit(0);
    }

    if(imageDepth >= 4) 
        imageFormat = GL_RGBA;
    else if(imageDepth == 3) 
        imageFormat = GL_RGB;
    else 
        imageFormat = GL_LUMINANCE;

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    printf("%s: %d, %d, %d\n",filename, imageWidth, imageHeight, imageDepth);
}

void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT);
    glRasterPos2i(0, 0); // 画像の左端をpixelで指定
    glDrawPixels(imageWidth, imageHeight, imageFormat, GL_UNSIGNED_BYTE, image);

    glutSwapBuffers();
}

// この(x, y)のy上からのピクセル
void motion(int x, int y)
{
    static GLint screeny;

    screeny = height - (GLint) y;
    glRasterPos2i (x, screeny);

    glPixelZoom (zoomFactor, zoomFactor);
    // Copyというか、Rasterした領域に設定された領域をDrawしちゃう関数. 
    // 何に使うんだろう。
    glCopyPixels (0, 0, imageWidth, imageHeight, GL_COLOR); // x, y, width, height, buffer
    glPixelZoom (1.0, 1.0);

    glutSwapBuffers();
}

void reshape(int w, int h)
{
    glViewport(0, 0, (GLsizei) w, (GLsizei) h);
    height = (GLint) h;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, (GLdouble) w, 0.0, (GLdouble) h);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

/* ARGSUSED1 */
void keyboard(unsigned char key, int x, int y)
{
   switch (key) {
      case 'r': case 'R':
         zoomFactor = 1.0;
         glutPostRedisplay();
         printf ("zoomFactor reset to 1.0\n");
         break;
      case 'z':
         zoomFactor += 0.5;
         if (zoomFactor >= 3.0)
            zoomFactor = 3.0;
         printf ("zoomFactor is now %4.1f\n", zoomFactor);
         break;
      case 'Z':
         zoomFactor -= 0.5;
         if (zoomFactor <= 0.5)
            zoomFactor = 0.5;
         printf ("zoomFactor is now %4.1f\n", zoomFactor);
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
   glutInit(&argc, argv);
   glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
   glutInitWindowSize(250, 250);
   glutInitWindowPosition(100, 100);
   glutCreateWindow(argv[0]);

   init(argv[1]);

   glutDisplayFunc(display);
   glutReshapeFunc(reshape);
   glutMotionFunc(motion);
   glutKeyboardFunc(keyboard);
   glutMainLoop();
   return 0;
}
