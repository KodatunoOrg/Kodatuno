#include <QApplication>
//#include <QTextCodec>
#include "QtMainWindow.h"


// Function: main
// QApplicationオブジェクトを生成，アプリケーションはイベントループ開始
//
// Prameters:
// argc - コマンド引数の数
// *argv[] - コマンド引数文字列
//
// Retern:
// 制御をQtに渡す
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // glutInit(&argc,argv);

    // utf-8な文字列をQString、trが受け付けられるように
    //  QTextCodec::setCodecForCStrings(QTextCodec::codecForName("utf-8"));
    // QTextCodec::setCodecForTr(QTextCodec::codecForName("utf-8"));
    
    QtMainWindow *mainWindow = new QtMainWindow();
    mainWindow->show();

//    RotSurfDialog dlg;
//    dlg.show();
    
    return a.exec();
}

