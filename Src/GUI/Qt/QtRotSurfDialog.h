#ifndef QTROTSURFDIALOG_H
#define QTROTSURFDIALOG_H

#include <QDialog>

namespace Ui {
    class QtRotSurfDialog;
}

class QtRotSurfDialog : public QDialog
{
    Q_OBJECT

public:
    explicit QtRotSurfDialog(QWidget *parent = 0);
    ~QtRotSurfDialog();

private slots:
    void on_buttonBox_accepted();

private:
    Ui::QtRotSurfDialog *ui;
};

#endif // QTROTSURFDIALOG_H
