#ifndef QTNURBSCURVEDIALOG_H
#define QTNURBSCURVEDIALOG_H

#include <QDialog>

// Constants: General Defines
// Non

// Namespace: Ui
// QtNurbsCurveDialogクラスを名前空間Uiで定義
namespace Ui {
    class QtNurbsCurveDialog;
}

// Class: QtNurbsCurveDialog
// QtNurbsCurveDialogクラス
class QtNurbsCurveDialog : public QDialog
{
// Macro: Q_OBJECT
// シグナルとスロットを定義するために必要
    Q_OBJECT

public:
// Constructor: QtNurbsCurveDialog
// QtNurbsCurveDialogクラスのコンストラクタ．暗黙呼び出しを禁止
    explicit QtNurbsCurveDialog(QWidget *parent = 0);
	
// Destructor: ~QtNurbsCurveDialog
// NURBS曲線生成ダイアログの終了
    ~QtNurbsCurveDialog();
    
private slots:
// Function: on_fileBtn_clicked
// (private)オープンファイル
    void on_fileBtn_clicked();
	
// Function: on_buttonBox_accepted
// (private)NurbsCurveダイアログのOKボタンが押されたときに呼び出される
    void on_buttonBox_accepted();

private:
// Variable: *ui
// NURBS曲線生成ダイアログ
    Ui::QtNurbsCurveDialog *ui;

// Variable: title
// タイトルの設定
    static const QString title ;
};

#endif // QTNURBSCURVEDIALOG_H