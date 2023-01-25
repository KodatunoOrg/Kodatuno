#ifndef QTROTSURFDIALOG_H
#define QTROTSURFDIALOG_H

#include <QDialog>

// Constants: General Defines
// Non

// Namespace: Ui
// QtRotSurfDialogクラスを名前空間Uiで定義
namespace Ui {
    class QtRotSurfDialog;
}

// Class: QtRotSurfDialog
// QtRotSurfDialogクラス
class QtRotSurfDialog : public QDialog
{
// Macro: Q_OBJECT
// シグナルとスロットを定義するために必要
    Q_OBJECT

public:
// Constructor: QtRotSurfDialog
// QtRotSurfDialogクラスのコンストラクタ．暗黙呼び出しを禁止
    explicit QtRotSurfDialog(QWidget *parent = 0);

// Destructor: ~QtRotSurfDialog
// 回転サーフェス生成ダイアログの終了
    ~QtRotSurfDialog();

private slots:
// Function: on_buttonBox_accepted
// (private)RotSurfダイアログのOKボタンが押されたときに呼び出される
    void on_buttonBox_accepted();

private:
// Variable: *ui
// RotSurfダイアログ
    Ui::QtRotSurfDialog *ui;
};

#endif // QTROTSURFDIALOG_H
