#ifndef QTCONSOLE_H
#define QTCONSOLE_H

#include <QTextEdit>
#include <QKeyEvent>
#include "Kodatuno.h"

class QtConsole : public QTextEdit
{
    Q_OBJECT
public:
    explicit QtConsole(QWidget *parent = 0);
    void keyPressEvent(QKeyEvent *e);       // キーイベント
    bool CheckCurPos();                     // カーソル位置判別
};

#endif // QTCONSOLE_H
