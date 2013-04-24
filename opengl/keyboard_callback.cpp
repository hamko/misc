#include <iostream>
#include <GL/glut.h>
using namespace std;

void init(void)
{
    glShadeModel(GL_FLAT); // 連続して作成した面を自動的に補完する機能を切る。軽くなる。

    glClearColor(0.0, 0.0, 0.0, 0.0); // バッファを初期化するときに使うRGBA, 真っ黒
    glClear(GL_COLOR_BUFFER_BIT); // カラーバッファを真っ黒に初期化
}

// キーボードのCallback関数
// キーボードが押下された時、そのキーIDと、マウスの位置x, yを返す
void keyboard(unsigned char key, int x, int y)
{
    static int i = 0;
    cout << (i++) << " " << key << " " << x << " " << y << endl;

    glFlush(); // カラーバッファを画面に表示
}
    
int main(int argc, char** argv)
{
    glutInit(&argc, argv);  // glutの初期化
    glutInitDisplayMode(GLUT_RGB); // RGBモード(Alpha値は使わない)
    glutInitWindowSize(500, 500); // 500 * 500のサイズのウィンドウを作る
    glutInitWindowPosition(100, 100);   // (x, y) = (100, 100)の位置にウィンドウを作る
    glutCreateWindow(argv[0]); // Windowを作成する

    init();

    glutKeyboardFunc(keyboard);   // keyboard call backの呼出

    cout << "This program never flushed until any key is pushed." << endl;
    glutMainLoop(); // Callbackを呼ぶ準備を整えて、while(1);と同じ

    return 0;
}
