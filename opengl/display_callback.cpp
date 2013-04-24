#include <iostream>
#include <GL/glut.h>
using namespace std;

void init(void)
{
    glClearColor(0.0, 0.0, 0.0, 0.0); // バッファを初期化するときに使うRGBA
    glShadeModel(GL_FLAT); // 連続して作成した面を自動的に補完する機能を切る。軽くなる。
}
void display(void)
{
    static int i = 0;
    cout << (i++) << endl;

    glClear(GL_COLOR_BUFFER_BIT); // カラーバッファを初期化
    glutWireCube(1.0); // 張りぼての立方体を描画
    glutSwapBuffers(); // オフスクリーンバッファを表に出す
}
    
int main(int argc, char** argv)
{
    glutInit(&argc, argv);  // glutの初期化
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB); // ダブルバッファリング, RGBモード(Alpha値は使わない)
    glutInitWindowSize(500, 500); // 500 * 500のサイズのウィンドウを作る
    glutInitWindowPosition(100, 100);   // (x, y) = (100, 100)の位置にウィンドウを作る
    glutCreateWindow(argv[0]); // Windowを作成する

    init();

    glutDisplayFunc(display);   // display call backの呼出

    glutMainLoop(); // Callbackを呼ぶ準備を整えて、while(1);と同じ

    return 0;
}
