#include "QtNurbsCurveDialog.h"
#include "ui_QtNurbsCurveDialog.h"
#include "Kodatuno.h"

const QString QtNurbsCurveDialog::title = QString("NURBS Curve Generator");

QtNurbsCurveDialog::QtNurbsCurveDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QtNurbsCurveDialog)
{
    ui->setupUi(this);
    this->setWindowTitle(title);   

    ui->buttonGroup->setId(ui->plygRadBtn,1);
    ui->buttonGroup->setId(ui->intRadBtn,2);
    ui->buttonGroup->setId(ui->appRadBtn,3);
    ui->buttonGroup->setId(ui->closedIntRadBtn,4);
    ui->buttonGroup->setId(ui->cpRadBtn,5);
}

QtNurbsCurveDialog::~QtNurbsCurveDialog()
{
    delete ui;
}

void QtNurbsCurveDialog::on_fileBtn_clicked()
{
    QString fileName(Kodatuno.OpenInpFile().c_str());
    ui->inpFileLabel->setText(fileName);
}

void QtNurbsCurveDialog::on_buttonBox_accepted()
{
    QString fileName = ui->inpFileLabel->text();
    if(fileName.isEmpty()){
        GuiIF.SetMessage("You do not choice input file!");
        return;
    }
    int opt = ui->buttonGroup->checkedId();
    switch(opt){
        case 0:
            GuiIF.SetMessage("You choice Polygonal");
            break;
        case 1:
            GuiIF.SetMessage("You choice Interpolated");
            break;
        case 2:
            GuiIF.SetMessage("You choice Approximated");
            break;
        case 3:
            GuiIF.SetMessage("You choice Interpolated2");
            break;
        case 4:
            GuiIF.SetMessage("You choice Control point");
            break;
    }
    GuiIF.SetMessage(tr("Input file is %1").arg(fileName).toUtf8().data());

    int rank = ui->rankSpin->value();
    if(rank > RANKMAX || rank <= 0){
        GuiIF.SetMessage(tr("Error : Inappropriate Rank ( 0 < Rank < %1 )").arg(RANKMAX).toUtf8().data());
        return;
    }
    Kodatuno.GenNurbsCurve(opt,fileName.toUtf8().data(),rank);
}
