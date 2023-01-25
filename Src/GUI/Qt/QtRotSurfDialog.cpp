#include "QtRotSurfDialog.h"
#include "ui_QtRotSurfDialog.h"
#include "Kodatuno.h"

QtRotSurfDialog::QtRotSurfDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QtRotSurfDialog)
{
    ui->setupUi(this);
}

QtRotSurfDialog::~QtRotSurfDialog()
{
    delete ui;
}

void QtRotSurfDialog::on_buttonBox_accepted()
{
    Coord ax;
    ax.x = ui->xSpinBox->value();
    ax.y = ui->ySpinBox->value();
    ax.z = ui->zSpinBox->value();

    double ang = ui->angleSpinBox->value();

    Kodatuno.GenSurface(ax,ang,ROTSURF);
}
