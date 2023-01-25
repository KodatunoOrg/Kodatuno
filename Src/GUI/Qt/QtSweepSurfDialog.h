#ifndef QTSWEEPSURFDIALOG_H
#define QTSWEEPSURFDIALOG_H

#include <QDialog>

namespace Ui {
    class QtSweepSurfDialog;
}

class QtSweepSurfDialog : public QDialog
{
    Q_OBJECT

public:
    explicit QtSweepSurfDialog(QWidget *parent = 0);
    ~QtSweepSurfDialog();

private slots:
    void on_buttonBox_accepted();   // OKÉ{É^ÉìÇ™âüÇ≥ÇÍÇΩÇ∆Ç´ÅCê›íËÇîΩâf

private:
    Ui::QtSweepSurfDialog *ui;
};

#endif // QTSWEEPSURFDIALOG_H
