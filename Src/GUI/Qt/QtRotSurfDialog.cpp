#include "QtRotSurfDialog.h"
#include "ui_QtRotSurfDialog.h"
#include "Kodatuno.h"

// Function: QtRotSurfDialog
// RotSurfダイアログが開いたときに呼び出される
//
// Parameters:
// *parent - 呼び出し元の親ウィジェットを指定
QtRotSurfDialog::QtRotSurfDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QtRotSurfDialog)
{
    ui->setupUi(this);
}

// Function: ~QtRotSurfDialog
// 回転サーフェス生成ダイアログの終了
QtRotSurfDialog::~QtRotSurfDialog()
{
    delete ui;
}

// Function: on_buttonBox_accepted
// RotSurfダイアログのOKボタンが押されたときに呼び出される
void QtRotSurfDialog::on_buttonBox_accepted()
{
    Coord ax;
    ax.x = ui->xSpinBox->value();
    ax.y = ui->ySpinBox->value();
    ax.z = ui->zSpinBox->value();

    double ang = ui->angleSpinBox->value();

    Kodatuno.GenSurface(ax,ang,ROTSURF);
}
