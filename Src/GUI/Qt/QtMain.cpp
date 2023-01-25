#include <QApplication>
#include <QTextCodec>
#include "QtMainWindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    glutInit(&argc,argv);

    // utf-8‚È•¶Žš—ñ‚ðQStringAtr‚ªŽó‚¯•t‚¯‚ç‚ê‚é‚æ‚¤‚É
//    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("utf-8"));
//    QTextCodec::setCodecForTr(QTextCodec::codecForName("utf-8"));
    
    QtMainWindow *mainWindow = new QtMainWindow();
    mainWindow->show();

//    RotSurfDialog dlg;
//    dlg.show();
    
    return a.exec();
}
