#ifndef QTCONSOLE_H
#define QTCONSOLE_H

#include <QTextEdit>
#include <QKeyEvent>
#include "Kodatuno.h"

// Constants: General Defines
// Non

// Class: QtConsole
// コンソール内でのイベントを統括するクラス
class QtConsole : public QTextEdit
{
    Q_OBJECT
public:
// Constructor: QtConsole
// QtConsoleクラスのコンストラクタ．
// 暗黙呼び出しを禁止
    explicit QtConsole(QWidget *parent = 0);
	
// Function: keyPressEvent
// コンソール上でキーによる入力が行われた場合，キーに応じて処理を行う
    void keyPressEvent(QKeyEvent *e);       
	
// Function: CheckCurPos
// カーソルの現在位置が最後の">"以降にあるのかを調べる
    bool CheckCurPos();                     
};

#endif // QTCONSOLE_H
