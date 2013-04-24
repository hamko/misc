#include <GL/glut.h>

/*	Create checkerboard texture	*/
#define	checkImageWidth 64
#define	checkImageHeight 64
GLubyte checkImage[checkImageWidth][checkImageHeight][3];

void makeCheckImage(void)
{
    int i, j, h;

    for (i = 0; i < checkImageWidth; i++) 
        for (j = 0; j < checkImageHeight; j++) 
            for (h = 0; h < 3; h++) 
                checkImage[i][j][h] = (GLubyte)(!(i&8)^!(j&8))*255;
}

void myinit(void)
{
    glClearColor (0.0, 0.0, 0.0, 0.0);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    // checker board柄を配列に代入
    makeCheckImage();

    // UNPACK_ALIGNMENTでは、第二引数に画像が何ビットごとに保持されているかを渡す(must be 2^n)
    // RGBAなら4bitなので、4を渡す
    // RGBは3bitだが、must be 2^nなので、1を渡す。
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); 

    // Textureの張り方
    glTexImage2D(GL_TEXTURE_2D, 0, 3,           // 2DTextureを使う、MIPMAPを使用しない、Texture色数3
            checkImageWidth, checkImageHeight,  // Textureの横幅、縦幅(must be 2^n)
            0, GL_RGB, GL_UNSIGNED_BYTE,        // 境界線の太さ0, RGBで(RGBAだったらGL_RGBAに), 1pixel unsigned 1byte
            &checkImage[0][0][0]);              // checkImageの先頭ポインタ

    // 2次元Textureの補完の仕方を指定する
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP); // texture平面sの指定が0.0以下なら0.0に、1.0以上なら1.0にする
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP); // texture平面tの指定が0.0以下なら0.0に、1.0以上なら1.0にする
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // 拡大時に線形補完
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // 縮小時に線形補完
    
    // 第一、第二引数は固定。
    // 第三引数は
    // MODULATE : Texture色と物体色をかけあわせる
    // REPLACE  : Texture色で上書きする
    // DECAL    : 透明度に基づいて線形混合される
    // BLEND    : Blend関数を使って混合する
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);

    glEnable(GL_TEXTURE_2D);
}

void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glutSolidTeapot(1.0);
    glutSwapBuffers();
}

void myReshape(int w, int h)
{
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, 1.0*(GLfloat)w/(GLfloat)h, 1.0, 30.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0.0, 0.0, -3.6);
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutCreateWindow("checker texture teapot");
    myinit();
    glutReshapeFunc (myReshape);
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}
