#ifndef QTSWEEPSURFDIALOG_H
#define QTSWEEPSURFDIALOG_H

#include <QDialog>

// Constants: General Defines
// Non

// Namespace: Ui
// QtSweepSurfDialogクラスを名前空間Uiで定義
namespace Ui {
    class QtSweepSurfDialog;
}

// Class: QtSweepSurfDialog
// QtSweepSurfDialogクラス
class QtSweepSurfDialog : public QDialog
{
// Macro: Q_OBJECT
// シグナルとスロットを定義するために必要
    Q_OBJECT

public:
// Constructor: QtSweepSurfDialog
// QtSweepSurfDialogクラスのコンストラクタ．暗黙呼び出しを禁止
    explicit QtSweepSurfDialog(QWidget *parent = 0);
	
// Destructor: ~QtSweepSurfDialog
// スイープサーフェス生成ダイアログの終了
    ~QtSweepSurfDialog();

private slots:
// Function: on_buttonBox_accepted
// (private)SweepSurfダイアログのOKボタンが押されたときに呼び出される
    void on_buttonBox_accepted();   // OKボタンが押されたとき，設定を反映

private:
// Variable: *ui
// SweepSurfダイアログ
    Ui::QtSweepSurfDialog *ui;
};

#endif // QTSWEEPSURFDIALOG_H
