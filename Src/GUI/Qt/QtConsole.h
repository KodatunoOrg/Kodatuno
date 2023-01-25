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
    void keyPressEvent(QKeyEvent *e);       // �L�[�C�x���g
    bool CheckCurPos();                     // �J�[�\���ʒu����
};

#endif // QTCONSOLE_H
