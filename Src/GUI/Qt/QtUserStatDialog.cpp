#include "QtUserStatDialog.h"
#include "ui_QtUserStatDialog.h"
#include "Kodatuno.h"

// Function: QtUserStatDialog
// UserStatダイアログがオープンしたときに呼び出される
// 
// Parameters:
// *parent - 呼び出し元の親ウィジェットを指定
QtUserStatDialog::QtUserStatDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QtUserStatDialog)
{
    ui->setupUi(this);

    ui->buttonGroup->setId(ui->mode1RadBtn,0);
    ui->buttonGroup->setId(ui->mode2RadBtn,1);
    ui->buttonGroup->setId(ui->mode3RadBtn,2);
    ui->buttonGroup->setId(ui->mode4RadBtn,3);
    ui->buttonGroup->setId(ui->mode5RadBtn,4);
    ui->buttonGroup->setId(ui->mode6RadBtn,5);
    ui->buttonGroup->setId(ui->mode7RadBtn,6);
    ui->buttonGroup->setId(ui->mode8RadBtn,7);
    ui->buttonGroup->setId(ui->mode9RadBtn,8);
    ui->buttonGroup->setId(ui->mode10RadBtn,9);
    ui->buttonGroup->setId(ui->mode11RadBtn,10);
    ui->buttonGroup->setId(ui->mode12RadBtn,11);
    ui->buttonGroup->setId(ui->mode13RadBtn,12);
    ui->buttonGroup->setId(ui->mode14RadBtn,13);
    ui->buttonGroup->setId(ui->mode15RadBtn,14);

    // UserStatusの現在値を読み込み，UserStatダイアログに反映させる
    int mode;
    double prop[USERPROPNUM];
    Kodatuno.GetUserStat(&mode,prop);

    switch(mode){
        case 0: ui->mode1RadBtn->setChecked(true); break;
        case 1: ui->mode2RadBtn->setChecked(true); break;
        case 2: ui->mode3RadBtn->setChecked(true); break;
        case 3: ui->mode4RadBtn->setChecked(true); break;
        case 4: ui->mode5RadBtn->setChecked(true); break;
        case 5: ui->mode6RadBtn->setChecked(true); break;
        case 6: ui->mode7RadBtn->setChecked(true); break;
        case 7: ui->mode8RadBtn->setChecked(true); break;
        case 8: ui->mode9RadBtn->setChecked(true); break;
        case 9: ui->mode10RadBtn->setChecked(true); break;
        case 10: ui->mode11RadBtn->setChecked(true); break;
        case 11: ui->mode12RadBtn->setChecked(true); break;
        case 12: ui->mode13RadBtn->setChecked(true); break;
        case 13: ui->mode14RadBtn->setChecked(true); break;
        case 14: ui->mode15RadBtn->setChecked(true); break;
    }

    ui->doubleSpinBox_1->setValue(prop[0]);
    ui->doubleSpinBox_2->setValue(prop[1]);
    ui->doubleSpinBox_3->setValue(prop[2]);
    ui->doubleSpinBox_4->setValue(prop[3]);
    ui->doubleSpinBox_5->setValue(prop[4]);
    ui->doubleSpinBox_6->setValue(prop[5]);
    ui->doubleSpinBox_7->setValue(prop[6]);
    ui->doubleSpinBox_8->setValue(prop[7]);
    ui->doubleSpinBox_9->setValue(prop[8]);
    ui->doubleSpinBox_10->setValue(prop[9]);
    ui->doubleSpinBox_11->setValue(prop[10]);
    ui->doubleSpinBox_12->setValue(prop[11]);
    ui->doubleSpinBox_13->setValue(prop[12]);
    ui->doubleSpinBox_14->setValue(prop[13]);
    ui->doubleSpinBox_15->setValue(prop[14]);

}

// Function: ~QtUserStatDialog
// UserStatダイアログの終了
QtUserStatDialog::~QtUserStatDialog()
{
    delete ui;
}

// Function: on_buttonBox_accepted
// UserStatダイアログのOKボタンが押されたときに呼び出される
void QtUserStatDialog::on_buttonBox_accepted()
{
    int mode = ui->buttonGroup->checkedId();

    double prop[USERPROPNUM];
    prop[0] = ui->doubleSpinBox_1->value();
    prop[1] = ui->doubleSpinBox_2->value();
    prop[2] = ui->doubleSpinBox_3->value();
    prop[3] = ui->doubleSpinBox_4->value();
    prop[4] = ui->doubleSpinBox_5->value();
    prop[5] = ui->doubleSpinBox_6->value();
    prop[6] = ui->doubleSpinBox_7->value();
    prop[7] = ui->doubleSpinBox_8->value();
    prop[8] = ui->doubleSpinBox_9->value();
    prop[9] = ui->doubleSpinBox_10->value();
    prop[10] = ui->doubleSpinBox_11->value();
    prop[11] = ui->doubleSpinBox_12->value();
    prop[12] = ui->doubleSpinBox_13->value();
    prop[13] = ui->doubleSpinBox_14->value();
    prop[14] = ui->doubleSpinBox_15->value();

    Kodatuno.AcceptUserStat(mode,prop);

    GuiIF.SetMessage("User status were Accepted.");
}
