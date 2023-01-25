#include "QtSweepSurfDialog.h"
#include "ui_QtSweepSurfDialog.h"
#include "Kodatuno.h"

// Function: QtSweepSurfDialog
// SweepSurfダイアログが開いたときに呼び出される
// 
// Parameters:
// 呼び出し元の親ウィジェットを指定
QtSweepSurfDialog::QtSweepSurfDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QtSweepSurfDialog)
{
    ui->setupUi(this);
}

// Function: ~QtSweepSurfDialog
// スイープサーフェス生成ダイアログの終了
QtSweepSurfDialog::~QtSweepSurfDialog()
{
    delete ui;
}

// Function: on_buttonBox_accepted
// SweepSurfダイアログのOKボタンが押されたときに呼び出される
void QtSweepSurfDialog::on_buttonBox_accepted()
{
    Coord ax;
    ax.x = ui->SpinBoxX->value();
    ax.y = ui->SpinBoxY->value();
    ax.z = ui->SpinBoxZ->value();

    double len = ui->SpinBoxL->value();

    if(Kodatuno.GenSurface(ax,len,SWEEPSURF) == KOD_TRUE){
        GuiIF.SetMessage("Success to generating Sweep surface!");
        GuiIF.UpdateDescribeWidget();
    }
    else{
        GuiIF.SetMessage("Fail to generating Sweep surface!");
    }
}
