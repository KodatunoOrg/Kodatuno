//GE����GUI�փf�[�^���Z�b�g���邽�߂̃N���X

#include "StdAfxQt.h"

GUI_Interface GuiIF;		// �N���XGUI_Interface�̃C���X�^���X���O���[�o���Ő錾

//�R���\�[���pGUI�̃I�u�W�F�N�g�̃C���X�^���X���Z�b�g
void GUI_Interface::SetConsoleObject(QTextEdit* textEdit)
{
    consoleText = textEdit;
}

//BODY���X�g�pGUI�̃I�u�W�F�N�g�̃C���X�^���X���Z�b�g
void GUI_Interface::SetBodyListObject(QListWidget* list)
{
    bodyList = list;
}

// �t�@�C���I�[�v���pGUI�̃I�u�W�F�N�g�̃C���X�^���X���Z�b�g
void GUI_Interface::SetOpenFileActionObject(QAction *action)
{
    openFileAction = action;
}

// BODY�폜�pGUI�̃I�u�W�F�N�g�̃C���X�^���X���Z�b�g
void GUI_Interface::SetDelBodyActionObject(QAction *action)
{
    delBodyAction = action;
}

//OpenGL�`��pGUI�̃I�u�W�F�N�g�̃C���X�^���X���Z�b�g
void GUI_Interface::SetDescribeWidget(QGLWidget *widget)
{
    glWidget = widget;
}

//�X�e�[�^�X�o�[���x���pGUI�̃I�u�W�F�N�g�̃C���X�^���X���Z�b�g
void GUI_Interface::SetStatusBarObject(QStatusBar *bar)
{
    statusBar = bar;
}
// UserFunc1�pGUI�̃I�u�W�F�N�g�̃C���X�^���X���Z�b�g
void GUI_Interface::SetUserFunc1Action(QAction* action)
{
    userFunc1 = action;
}

// UserFunc2�pGUI�̃I�u�W�F�N�g�̃C���X�^���X���Z�b�g
void GUI_Interface::SetUserFunc2Action(QAction* action)
{
    userFunc2 = action;    
}

// UserFunc3�pGUI�̃I�u�W�F�N�g�̃C���X�^���X���Z�b�g
void GUI_Interface::SetUserFunc3Action(QAction* action)
{
    userFunc3 = action;    
}

// UserFunc4�pGUI�̃I�u�W�F�N�g�̃C���X�^���X���Z�b�g
void GUI_Interface::SetUserFunc4Action(QAction* action)
{
    userFunc4 = action;    
}

// UserFunc5�pGUI�̃I�u�W�F�N�g�̃C���X�^���X���Z�b�g
void GUI_Interface::SetUserFunc5Action(QAction* action)
{
    userFunc5 = action;    
}

// UserFunc6�pGUI�̃I�u�W�F�N�g�̃C���X�^���X���Z�b�g
void GUI_Interface::SetUserFunc6Action(QAction* action)
{
    userFunc6 = action;
}

// UserFunc7�pGUI�̃I�u�W�F�N�g�̃C���X�^���X���Z�b�g
void GUI_Interface::SetUserFunc7Action(QAction* action)
{
    userFunc7 = action;
}

// UserFunc8�pGUI�̃I�u�W�F�N�g�̃C���X�^���X���Z�b�g
void GUI_Interface::SetUserFunc8Action(QAction* action)
{
    userFunc8 = action;
}

// �R���\�[���Ƀ��b�Z�[�W���o��
int GUI_Interface::SetMessage(const char *mes)
{
    QString text = consoleText->toPlainText() + QString(mes);

    consoleText->setText(text);
    consoleText->append(">");
    
    /* �J�[�\���𖖔��Ɉړ� */
    QTextCursor cursor = consoleText->textCursor();
    cursor.movePosition(QTextCursor::End);
    consoleText->setTextCursor(cursor);

    statusBar->showMessage(QString(mes),0);

    int len = consoleText->toPlainText().length();
    return len;
}

// �R���\�[���ɃR�}���h���o��
int GUI_Interface::SetCommand(const char *cmd)
{
    QString text = consoleText->toPlainText() + QString(cmd);
    consoleText->setText(text);
    consoleText->append(">");

    /* �J�[�\���𖖔��Ɉړ� */
    QTextCursor cursor = consoleText->textCursor();
    cursor.movePosition(QTextCursor::End);
    consoleText->setTextCursor(cursor);

    int len = consoleText->toPlainText().length();
    return len;
}

// �V���ɓo�^����BODY�̖��O���E�B���h�E���X�g�ɕ\������
void GUI_Interface::AddBodyNameToWin(const char *name)
{
    bodyList->addItem(name);
}

// �w�肳�ꂽ�ԍ���BODY�����X�g����폜����
void GUI_Interface::DelBodyFromWin(int pos)
{
    QListWidgetItem* item = bodyList->takeItem(pos);
    delete item;
}

// �t�@�C��OPEN�p�I�u�W�F�N�g�̏�Ԑݒ�
void GUI_Interface::SetEnableOpenFile(bool enable)
{
    openFileAction->setEnabled(enable);
}

// BODY�폜�p�I�u�W�F�N�g�̏�Ԑݒ�
void GUI_Interface::SetEnableDelBody(bool enable)
{
    delBodyAction->setEnabled(enable);
}

//�`��pGUI�̍ĕ`��w��
void GUI_Interface::UpdateDescribeWidget()
{
    glWidget->update();
}

// �R���\�[�����當������擾
std::string GUI_Interface::GetConsoleText()
{
    std::string text(consoleText->toPlainText().toUtf8());
    return text;
}

// BODY���X�g�̑I������Ă���ԍ����擾����
int GUI_Interface::GetBodyListSelectedPos()
{
    return bodyList->currentRow();
}

// �t�@�C���I�[�v���_�C�A���O�̕\���iBODY�j
//return �t�@�C���̃t���p�X
std::string GUI_Interface::OpenBodyFileDlg()
{
    // �t�@�C���̃t�B���^���X�g
    QString fileFilter = QObject::tr("IGS (*.igs);; STL (*.stl);; VRML (*.wrl);; DXF (*.dxf)");

    QString fileName = QFileDialog::getOpenFileName(
        0,                          //�@�ewidget
        QObject::tr("Open File"),   // �_�C�A���O�^�C�g��
        QDir::currentPath(),        // �N�����̃f�B���N�g��
        fileFilter                  // �t�@�C���t�B���^�[
    );

    std::string stdStr(fileName.toUtf8());
    return stdStr;
}

// �t�@�C���I�[�v���_�C�A���O�̕\���iINP�j
//return �t�@�C���̃t���p�X
std::string GUI_Interface::OpenInpFileDlg()
{
    // �t�@�C���̃t�B���^���X�g
    QString fileFilter = QObject::tr("INP (*.inp)");

    QString fileName = QFileDialog::getOpenFileName(
        0,                          //�@�ewidget
        QObject::tr("Open File"),   // �_�C�A���O�^�C�g��
        QDir::currentPath(),        // �N�����̃f�B���N�g��
        fileFilter                  // �t�@�C���t�B���^�[
    );

    std::string stdStr(fileName.toUtf8());
    return stdStr;
}

// �t���p�X����t�@�C�������o
std::string GUI_Interface::GetFileName(const std::string fullPath)
{
    QString fileName = QFileInfo(QString::fromStdString(fullPath)).baseName();
    
    std::string stdStr(fileName.toUtf8());
    return stdStr;
}

// �t���p�X����t�@�C���̊g���q���o
std::string GUI_Interface::GetFileExt(const std::string fullPath)
{
    QString ext  = QFileInfo(QString::fromStdString(fullPath)).suffix();
    
    std::string stdStr(ext.toUtf8());
    return stdStr;
}

// Nurbs�Ȑ��_�C�A���O�\��
void GUI_Interface::ShowNurbsCurveDlg()
{
    QtNurbsCurveDialog *dlg = new QtNurbsCurveDialog();
    dlg->exec();
    glWidget->update();
}

// Nurbs�Ȗʃ_�C�A���O�\��
void GUI_Interface::ShowNurbsSurfDlg()
{
    QtNurbsSurfDialog *dlg = new QtNurbsSurfDialog();
    dlg->exec();
    glWidget->update();    
}

// ��]�T�[�t�F�X�_�C�A���O�\��
void GUI_Interface::ShowRotSurfDlg()
{
    QtRotSurfDialog *dlg = new QtRotSurfDialog();
    dlg->exec();
    glWidget->update();
}

// �X�C�[�v�T�[�t�F�X�_�C�A���O�\��
void GUI_Interface::ShowSweepSurfDlg()
{
    QtSweepSurfDialog *dlg = new QtSweepSurfDialog();
    dlg->exec();
    glWidget->update();    
}

// UserFunc1�̃��x�����Z�b�g
void GUI_Interface::SetUserFunc1Label(const char *label)
{
    userFunc1->setText(QString(label));
}

// UserFunc2�̃��x�����Z�b�g
void GUI_Interface::SetUserFunc2Label(const char *label)
{
    userFunc2->setText(QString(label));    
}

// UserFunc3�̃��x�����Z�b�g
void GUI_Interface::SetUserFunc3Label(const char *label)
{
    userFunc3->setText(QString(label));
}

// UserFunc4�̃��x�����Z�b�g
void GUI_Interface::SetUserFunc4Label(const char *label)
{    
    userFunc4->setText(QString(label));
}

// UserFunc5�̃��x�����Z�b�g
void GUI_Interface::SetUserFunc5Label(const char *label)
{    
    userFunc5->setText(QString(label));
}

// UserFunc6�̃��x�����Z�b�g
void GUI_Interface::SetUserFunc6Label(const char *label)
{
    userFunc6->setText(QString(label));
}
// UserFunc7�̃��x�����Z�b�g
void GUI_Interface::SetUserFunc7Label(const char *label)
{
    userFunc7->setText(QString(label));
}
// UserFunc8�̃��x�����Z�b�g
void GUI_Interface::SetUserFunc8Label(const char *label)
{
    userFunc8->setText(QString(label));
}
