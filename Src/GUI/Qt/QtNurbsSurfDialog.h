#ifndef QTNURBSSURFDIALOG_H
#define QTNURBSSURFDIALOG_H

#include <QDialog>

namespace Ui {
    class QtNurbsSurfDialog;
}

class QtNurbsSurfDialog : public QDialog
{
    Q_OBJECT

public:
    explicit QtNurbsSurfDialog(QWidget *parent = 0);
    ~QtNurbsSurfDialog();

private slots:
    void on_fileBtn_clicked();
    void on_buttonBox_accepted();

private:
    Ui::QtNurbsSurfDialog *ui;
    static const QString title ;
};

#endif // QTNURBSSURFDIALOG_H
