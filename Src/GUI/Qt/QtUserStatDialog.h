#ifndef QTUSERSTATDIALOG_H
#define QTUSERSTATDIALOG_H

#include <QDialog>

namespace Ui {
    class QtUserStatDialog;
}

class QtUserStatDialog : public QDialog
{
    Q_OBJECT

public:
    explicit QtUserStatDialog(QWidget *parent = 0);
    ~QtUserStatDialog();

private slots:
    void on_buttonBox_accepted();   // OKボタンが押されたとき，設定を反映

private:
    Ui::QtUserStatDialog *ui;
};

#endif // QTUSERSTATDIALOG_H
