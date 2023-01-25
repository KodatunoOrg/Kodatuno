#include "QtNurbsSurfDialog.h"
#include "ui_QtNurbsSurfDialog.h"
#include "Kodatuno.h"

const QString QtNurbsSurfDialog::title = QString("NURBS Curve Generator");

QtNurbsSurfDialog::QtNurbsSurfDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QtNurbsSurfDialog)
{
    ui->setupUi(this);
    this->setWindowTitle(title);

    ui->buttonGroup->setId(ui->plyRadBtn,1);
    ui->buttonGroup->setId(ui->intRadBtn,2);
    ui->buttonGroup->setId(ui->appRadBtn,3);
    ui->buttonGroup->setId(ui->nullRadBtn,4);
    ui->buttonGroup->setId(ui->cpRadBtn,5);
}

QtNurbsSurfDialog::~QtNurbsSurfDialog()
{
    delete ui;
}

void QtNurbsSurfDialog::on_fileBtn_clicked()
{
    QString fileName(Kodatuno.OpenInpFile().c_str());
    ui->inpFileLabel->setText(fileName);
}

void QtNurbsSurfDialog::on_buttonBox_accepted()
{
    QString fileName = ui->inpFileLabel->text();
    if(fileName.isEmpty()){
        GuiIF.SetMessage("You do not choice input file!");
        return;
    }
    int opt = ui->buttonGroup->checkedId();
    switch(opt){
        case 1:
            GuiIF.SetMessage("You choice Polygonal");
            break;
        case 2:
            GuiIF.SetMessage("You choice Interpolated");
            break;
        case 3:
            GuiIF.SetMessage("You choice Approximated");
            break;
        case 4:
            GuiIF.SetMessage("You choice null");
            break;
        case 5:
            GuiIF.SetMessage("You choice Control point");
            break;
    }
    GuiIF.SetMessage(tr("Input file is %1").arg(fileName).toUtf8().data());

    int rank = ui->rankSpin->value();
    if(rank > RANKMAX || rank <= 0){
        GuiIF.SetMessage(tr("Error : Inappropriate Rank ( 0 < Rank < %1 )").arg(RANKMAX).toUtf8().data());
        return;
    }
    Kodatuno.GenNurbsSurface(opt,fileName.toUtf8().data(),rank);
}
