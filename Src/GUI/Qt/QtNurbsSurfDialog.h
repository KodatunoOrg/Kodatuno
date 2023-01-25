#ifndef QTNURBSSURFDIALOG_H
#define QTNURBSSURFDIALOG_H

#include <QDialog>

// Constants: General Defines
// Non

// Namespace: Ui
// QtNurbeSurfDialogクラスを名前空間Uiで定義
namespace Ui {
    class QtNurbsSurfDialog;
}

// Class: QtNurbsSurfDialog
// QtNurbsSurfDialogクラス
class QtNurbsSurfDialog : public QDialog
{
// Macro: Q_OBJECT
// シグナルとスロットを定義するために必要
    Q_OBJECT

public:
// Constructor: QtNurbsSurfDialog
// NURBS曲面生成ダイアログの設定
    explicit QtNurbsSurfDialog(QWidget *parent = 0);
	
// Destructor: ~QtNurbsSurfDialog
// NURBS曲面生成ダイアログの終了
    ~QtNurbsSurfDialog();

private slots:
// Function: on_fileBtn_clicked
// (private)ファイルオープン
    void on_fileBtn_clicked();
	
// Function: on_buttonBox_accepted
// (private)選択した方法に合わせて処理
    void on_buttonBox_accepted();

private:
// Variable: *ui
// NurbsSurfダイアログ
    Ui::QtNurbsSurfDialog *ui;
	
// Variable: title
// タイトルの設定
    static const QString title ;
};

#endif // QTNURBSSURFDIALOG_H
