#include "QtMainWindow.h"
#include "ui_QtMainWindow.h"

KODatUNO Kodatuno;	// KODatUNO�N���X�̃C���X�^���X���O���[�o���錾

// ���C���E�B���h�E����
QtMainWindow::QtMainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::QtMainWindow)
{
    ui->setupUi(this);
    
    // INI�t�@�C���C���^�[�t�F�[�X�̃C���X�^���X
    setting = new QSettings("kodatuno.ini", QSettings::IniFormat);

    // GUI�̏�����
    initGUI();

    // Kodatuno�̏�����
    Kodatuno.InitializeWin(ui->describeWidget->width(), ui->describeWidget->height());

    // INI�t�@�C������R�}���h�������擾
    QStringList cmdList = setting->value("cmd_history").toStringList();
    if(cmdList.length() != 0){
        for(int i = 0; i < cmdList.length(); i++){
            Kodatuno.SetConsoleHistory(cmdList[i].toStdString());
        }
    }

    // INI�t�@�C������UserStatus�����擾
    int mode = setting->value("user_stat_mode").toInt();
    double prop[USERPROPNUM];
    QStringList propList = setting->value("user_stat_prop").toStringList();
    if(propList.length() != 0){
        for(int i=0;i<USERPROPNUM;i++){
            prop[i] = propList[i].toDouble();
        }
    }
    Kodatuno.AcceptUserStat(mode,prop);
}

// Initializing GUI
void QtMainWindow::initGUI()
{
    // INI�t�@�C����ǂݍ���ŁC�E�B�W�F�b�g�ɔ��f
    restoreGeometry(setting->value("geometry").toByteArray());
    restoreState(setting->value("dock").toByteArray());
    
    // GUI�̃C�X�^���X���C���^�[�t�F�[�X�ɃZ�b�g
    GuiIF.SetConsoleObject(ui->consoleText);            // �R���\�[���pGUI�̃I�u�W�F�N�g�̃C���X�^���X���Z�b�g
    GuiIF.SetBodyListObject(ui->bodyList);              // BODY���X�g�pGUI�̃I�u�W�F�N�g�̃C���X�^���X���Z�b�g
    GuiIF.SetOpenFileActionObject(ui->actionOpenBody);	// �t�@�C���I�[�v���pGUI�̃I�u�W�F�N�g�̃C���X�^���X���Z�b�g
    GuiIF.SetDelBodyActionObject(ui->actionDeleteBody);	// BODY�폜�pGUI�̃I�u�W�F�N�g�̃C���X�^���X���Z�b�g
    GuiIF.SetDescribeWidget(ui->describeWidget);        // OpenGL�`��pGUI�̃I�u�W�F�N�g�̃C���X�^���X���Z�b�g
    GuiIF.SetStatusBarObject(ui->statusBar);            // �X�e�[�^�X�o�[�pGUI�̃I�u�W�F�N�g�̃C���X�^���X���Z�b�g
    GuiIF.SetUserFunc1Action(ui->actionUserFunc1);      // UserFunc1�̃��x�����Z�b�g
    GuiIF.SetUserFunc2Action(ui->actionUserFunc2);      // UserFunc2�̃��x�����Z�b�g
    GuiIF.SetUserFunc3Action(ui->actionUserFunc3);      // UserFunc3�̃��x�����Z�b�g
    GuiIF.SetUserFunc4Action(ui->actionUserFunc4);      // UserFunc4�̃��x�����Z�b�g
    GuiIF.SetUserFunc5Action(ui->actionUserFunc5);      // UserFunc5�̃��x�����Z�b�g
    GuiIF.SetUserFunc6Action(ui->actionUserFunc6);      // UserFunc6�̃��x�����Z�b�g
    GuiIF.SetUserFunc7Action(ui->actionUserFunc7);      // UserFunc7�̃��x�����Z�b�g
    GuiIF.SetUserFunc8Action(ui->actionUserFunc8);      // UserFunc8�̃��x�����Z�b�g
}

QtMainWindow::~QtMainWindow()
{
    Kodatuno.DeleteWin();
    delete ui;
}

// KodatunoGUI�N���[�Y���̃C�x���g
void QtMainWindow::closeEvent(QCloseEvent *event)
{
    /* �R�}���h�������擾 */
    int cmdNum = Kodatuno.GetConsoleHistoryNum();
    std::string cmdTemp;
    QStringList cmdList;
    if(cmdNum != 0){
        for(int i = 0; i < cmdNum; i++){
            Kodatuno.GetUpperConsoleHistory(&cmdTemp);
            cmdList.append(QString(cmdTemp.c_str()));
        }
    }

    // UserStatus���擾
    int mode;
    double prop[USERPROPNUM];
    Kodatuno.GetUserStat(&mode,prop);
    QStringList uslist;
    for(int i=0;i<USERPROPNUM;i++)
        uslist.append(QString::number(prop[i]));


    // INI�t�@�C���փR�}���h������UserStatus������������
    setting->setValue("geometry",saveGeometry());
    setting->setValue("dock",saveState());
    setting->setValue("cmd_history",cmdList);
    setting->setValue("user_stat_mode",mode);
    setting->setValue("user_stat_prop",uslist);
    setting->sync();

    // �N���[�Y�C�x���g
    QMainWindow::closeEvent(event);
}

// ���j���[�FCreate->NURBS Rotation
void QtMainWindow::on_actionRotationSurface_triggered()
{
    QtRotSurfDialog* dlg = new QtRotSurfDialog(this);
    dlg->exec();
    ui->describeWidget->repaint();
}

// ���j���[�FCreate->Sweep
void QtMainWindow::on_actionSweepSurface_triggered()
{
    QtSweepSurfDialog* dlg = new QtSweepSurfDialog (this);
    dlg->exec();
    ui->describeWidget->repaint();
}

// ���j���[�FCreate->NURBS Curve
void QtMainWindow::on_actionNURBSCurve_triggered()
{
    QtNurbsCurveDialog* dlg = new QtNurbsCurveDialog(this);
    dlg->exec();
    ui->describeWidget->repaint();
}

// ���j���[�FCreate->NURBS Surface
void QtMainWindow::on_actionNURBSSarface_triggered()
{
    QtNurbsSurfDialog* dlg = new QtNurbsSurfDialog (this);
    dlg->exec();
    ui->describeWidget->repaint();
}

// ���j���[�FFile->Open File
void QtMainWindow::on_actionOpenBody_triggered()
{
    Kodatuno.OpenFile();		// �t�@�C���I�[�v��
    Kodatuno.OpenDelBtn();      // Open�CDelete�{�^���̕\�������̕ύX
    ui->describeWidget->repaint();
}

// ���j���[�FEdit->Delete BODY
void QtMainWindow::on_actionDeleteBody_triggered()
{
    Kodatuno.DeleteBody();  	// �I������Ă���BODY������
    Kodatuno.OpenDelBtn();      // Open�CDelete�{�^���̕\�������̕ύX
    ui->describeWidget->repaint();
}

// �c�[���o�[�FX-View
void QtMainWindow::on_actionViewOnX_triggered()
{
    Kodatuno.ChangeViewX();
    ui->describeWidget->repaint();
}

// �c�[���o�[�FY-View
void QtMainWindow::on_actionViewOnY_triggered()
{
    Kodatuno.ChangeViewY();
    ui->describeWidget->repaint();
}

// �c�[���o�[�FZ-View
void QtMainWindow::on_actionViewOnZ_triggered()
{
    Kodatuno.ChangeViewZ();
    ui->describeWidget->repaint();
}

// �c�[���o�[�F�A�C�\��View
void QtMainWindow::on_actionViewOnXYZ_triggered()
{
    Kodatuno.ChangeViewXYZ();
    ui->describeWidget->repaint();
}

// �c�[���o�[�F��ʂɃt�B�b�g
void QtMainWindow::on_actionViewFit_triggered()
{
    Kodatuno.FitView();
    ui->describeWidget->repaint();
}

// �c�[���o�[�F�\���b�h�\��
void QtMainWindow::on_actionViewSolid_triggered()
{
    Kodatuno.SolidView();
    ui->describeWidget->repaint();
}

// �c�[���o�[�F�\���b�h�i�O�`���Ȃ��j�\��
void QtMainWindow::on_actionViewNoEdge_triggered()
{
    Kodatuno.NoEdgeView();
    ui->describeWidget->repaint();
}

// �c�[���o�[�F���C���[�t���[���\��
void QtMainWindow::on_actionViewWireFlame_triggered()
{
    Kodatuno.WireFlameView();
    ui->describeWidget->repaint();
}

// �c�[���o�[�F��\��
void QtMainWindow::on_actionViewVoid_triggered()
{
    Kodatuno.VoidView();
    ui->describeWidget->repaint();
}

// �c�[���o�[�F�S�đI��
void QtMainWindow::on_actionSelectAll_triggered()
{
    Kodatuno.SelectAll();
    ui->describeWidget->repaint();
}

// �c�[���o�[�F�S�Ă̑I�����L�����Z��
void QtMainWindow::on_actionSelect_Cancel_triggered()
{
    Kodatuno.SelectionCancel();
    ui->describeWidget->repaint();
}

// �c�[���o�[�F���[�U�[�ɂ��`��w�߂��L�����Z��
void QtMainWindow::on_actionViewErase_triggered()
{
    Kodatuno.UserViewCanncel();
    ui->describeWidget->repaint();
}

// �c�[���o�[�F�������\��
void QtMainWindow::on_actionViewBlend_triggered()
{
    Kodatuno.ViewBlending();
    ui->describeWidget->repaint();
}

// ���j���[�FView->Show Console
void QtMainWindow::on_actionShowConsole_triggered()
{
    ui->consoleDock->setVisible(true);
}

// ���j���[�FView->Show Body List
void QtMainWindow::on_actionShowBodyList_triggered()
{
    ui->bodyListDock->setVisible(true);
}

// BODY List���̃A�C�e����I��
void QtMainWindow::on_bodyList_itemClicked(QListWidgetItem *item)
{
    Kodatuno.BodyListSelect();
}

// ���j���[�FUser->User Status
void QtMainWindow::on_actionUserStatus_triggered()
{
    // UserStatus�_�C�A���O��\��
    QtUserStatDialog* dlg = new QtUserStatDialog(this);
    dlg->show();
}

// ���j���[�FUser->User Status->User Func1
void QtMainWindow::on_actionUserFunc1_triggered()
{
    Kodatuno.ExecuteUserFuncMain(0);
    Kodatuno.ReDrawUserFunc();			// UserFunc�ɂ��`��w�߂����s
}

// ���j���[�FUser->User Status->User Func2
void QtMainWindow::on_actionUserFunc2_triggered()
{
    Kodatuno.ExecuteUserFuncMain(1);
    Kodatuno.ReDrawUserFunc();			// UserFunc�ɂ��`��w�߂����s
}

// ���j���[�FUser->User Status->User Func3
void QtMainWindow::on_actionUserFunc3_triggered()
{
    Kodatuno.ExecuteUserFuncMain(2);
    Kodatuno.ReDrawUserFunc();			// UserFunc�ɂ��`��w�߂����s
}

// ���j���[�FUser->User Status->User Func4
void QtMainWindow::on_actionUserFunc4_triggered()
{
    Kodatuno.ExecuteUserFuncMain(3);
    Kodatuno.ReDrawUserFunc();			// UserFunc�ɂ��`��w�߂����s
}

// ���j���[�FUser->User Status->User Func5
void QtMainWindow::on_actionUserFunc5_triggered()
{
    Kodatuno.ExecuteUserFuncMain(4);
    Kodatuno.ReDrawUserFunc();			// UserFunc�ɂ��`��w�߂����s
}

// ���j���[�FUser->User Status->User Func6
void QtMainWindow::on_actionUserFunc6_triggered()
{
    Kodatuno.ExecuteUserFuncMain(5);
    Kodatuno.ReDrawUserFunc();			// UserFunc�ɂ��`��w�߂����s
}

// ���j���[�FUser->User Status->User Func7
void QtMainWindow::on_actionUserFunc7_triggered()
{
    Kodatuno.ExecuteUserFuncMain(6);
    Kodatuno.ReDrawUserFunc();			// UserFunc�ɂ��`��w�߂����s
}

// ���j���[�FUser->User Status->User Func8
void QtMainWindow::on_actionUserFunc8_triggered()
{
    Kodatuno.ExecuteUserFuncMain(7);
    Kodatuno.ReDrawUserFunc();			// UserFunc�ɂ��`��w�߂����s
}

// ���j���[�FHelp->About Kodatuno
void QtMainWindow::on_actionAboutKodatuno_triggered()
{
    QMessageBox::information(   this,
                                tr("About Kodatuno"),
                                tr("Kodatuno R%1.%2").arg(KODATUNO_MAJOR_VER).arg(KODATUNO_MINOR_VER)
                                );
}
