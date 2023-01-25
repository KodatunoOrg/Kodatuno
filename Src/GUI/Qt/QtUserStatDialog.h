#ifndef QTUSERSTATDIALOG_H
#define QTUSERSTATDIALOG_H

#include <QDialog>

// Constants: General Defines
// Non

// Namespace: Ui
// QtUserStatDialogクラスを名前空間Uiで定義
namespace Ui {
    class QtUserStatDialog;
}

// Class: QtUserStatDialog
// QtUserStatDialogクラス
class QtUserStatDialog : public QDialog
{
// Macro: Q_OBJECT
// シグナルとスロットを定義するために必要
    Q_OBJECT

public:
// Constructor: QtUserStatDialog
// QtUserStatDialogクラスのコンストラクタ．暗黙呼び出しを禁止
    explicit QtUserStatDialog(QWidget *parent = 0);

// Destructor: ~QtUserStatDialog
// UserStatダイアログの終了
    ~QtUserStatDialog();

private slots:
// Function: on_buttonBox_accepted
// (private)UserStatダイアログのOKボタンが押されたときに呼び出される
    void on_buttonBox_accepted();   // OKボタンが押されたとき，設定を反映

private:
// Variable: *ui
// UserStatダイアログ
    Ui::QtUserStatDialog *ui;
};

#endif // QTUSERSTATDIALOG_H
