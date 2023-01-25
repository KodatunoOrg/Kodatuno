#include "QtSweepSurfDialog.h"
#include "ui_QtSweepSurfDialog.h"
#include "Kodatuno.h"

QtSweepSurfDialog::QtSweepSurfDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QtSweepSurfDialog)
{
    ui->setupUi(this);
}

QtSweepSurfDialog::~QtSweepSurfDialog()
{
    delete ui;
}

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
