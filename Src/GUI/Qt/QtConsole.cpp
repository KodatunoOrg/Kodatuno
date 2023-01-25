#include "QtConsole.h"
#include <QByteArray>

QtConsole::QtConsole(QWidget *parent):
    QTextEdit(parent)
{
    this->setReadOnly(false);
}

void QtConsole::keyPressEvent(QKeyEvent *e)
{
    int curPos;         // カーソルの現在位置
    int lastLen;
    QString text, textTemp, cmd;
    std::string cmdTemp;
    QTextCursor cursor;

    if(!e) return;

    switch (e->key()){
    // リターンキー入力されたら，コマンド抽出・解析・実行
    case Qt::Key_Return:
    case Qt::Key_Enter:
        text = this->toPlainText();    // consoleからテキストを取得
        cmd = text.split(">").last();   // ">"で分割した最後のテキストを取得 = コマンドの抽出
        this->append(">");               // console風に味付け

        if(cmd == "") break;            // 空白なら
        else{
            // コマンドを実行し，履歴に登録
            QString temp = cmd + "\n>";
            Kodatuno.ExecCommand(temp.toUtf8().data());
            Kodatuno.SetConsoleHistory(cmd.toStdString());
        }

        break;

    // 上キー入力に対して，コマンド履歴表示
    case Qt::Key_Up:
        if(Kodatuno.GetUpperConsoleHistory(&cmdTemp) == KOD_TRUE){
            text = this->toPlainText();                 // consoleからテキストを取得
            lastLen = text.split(">").last().count();   // ">"で分割した最後のテキストの文字数を取得
            text.resize(text.count() - lastLen);        // 入力されていた文字を削除
            text += QString(cmdTemp.c_str());
            this->setText(text);                        // consoleに表示

            // カーソルを末尾に移動
            cursor = this->textCursor();
            cursor.movePosition(QTextCursor::End);
            this->setTextCursor(cursor);
        }
        break;

    // 下キー入力に対して，コマンド履歴表示
    case Qt::Key_Down:
        if(Kodatuno.GetLowerConsoleHistory(&cmdTemp) == KOD_TRUE){
            text = this->toPlainText();                 // consoleからテキストを取得
            lastLen = text.split(">").last().count();   // ">"で分割した最後のテキストの文字数を取得
            text.resize(text.count() - lastLen);        // 入力されていた文字を削除
            text += QString(cmdTemp.c_str());
            this->setText(text);                        // consoleに表示

            // カーソルを末尾に移動
            cursor = this->textCursor();
            cursor.movePosition(QTextCursor::End);
            this->setTextCursor(cursor);
        }
        break;

    // ">"で止める
    case Qt::Key_Left:
        text = this->toPlainText();                 // consoleからテキストを取得
        lastLen = text.split(">").last().count();   // ">"で分割した最後のテキストの文字数を取得

        // カーソルの位置取得
        cursor = this->textCursor();
        curPos = cursor.position();

        // ">"で止める
        if( curPos > (text.count() - lastLen) ){
            QTextEdit::keyPressEvent(e);    // 通常の処理
        }
        break;

    // ">"で止める
    case Qt::Key_Backspace:
    case Qt::Key_Delete:
        if(CheckCurPos() == false || this->textCursor().hasSelection()==true)
            break;
        text = this->toPlainText();         // consoleからテキストを取得
        textTemp = text.split(">").last();  // ">"で分割した最後のテキストの文字を取得
        if(textTemp != ""){
            QTextEdit::keyPressEvent(e);    // 通常の処理
        }
        break;

    // それ以外のキーは通常の処理
    default:
        // 現在のカーソル位置が最後の">"より前にある場合は，コンソールの最後にカーソルを持ってくる
        if(CheckCurPos() == false || this->textCursor().hasSelection()==true){
            cursor = this->textCursor();
            cursor.movePosition(QTextCursor::End);
            this->setTextCursor(cursor);
        }
        QTextEdit::keyPressEvent(e);
        break;
    }
}

// カーソルの現在位置が最後の">"以降にあるのかを調べる
// 返値　">"以降：ture　　　">"以前：false
bool QtConsole::CheckCurPos()
{
    int curpos = this->textCursor().position();
    QString text = this->toPlainText();
    if(curpos - (text.count()-text.split(">").last().count()) < 0)
        return false;
    else
        return true;
}
