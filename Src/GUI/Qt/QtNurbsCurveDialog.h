#ifndef QTNURBSCURVEDIALOG_H
#define QTNURBSCURVEDIALOG_H

#include <QDialog>

namespace Ui {
    class QtNurbsCurveDialog;
}

class QtNurbsCurveDialog : public QDialog
{
    Q_OBJECT

public:
    explicit QtNurbsCurveDialog(QWidget *parent = 0);
    ~QtNurbsCurveDialog();
    
private slots:
    void on_fileBtn_clicked();
    void on_buttonBox_accepted();

private:
    Ui::QtNurbsCurveDialog *ui;
    static const QString title ;
};

#endif // QTNURBSCURVEDIALOG_H
