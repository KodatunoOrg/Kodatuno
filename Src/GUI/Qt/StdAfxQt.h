#ifndef _STD_AFX_QT_H_
#define _STD_AFX_QT_H_

//GE�Ŏg�p����Qt�֘A�̃w�b�_
#include <QTextEdit>
#include <QListWidget>
#include <QListWidgetItem>
#include <QFileDialog>
#include <QLabel>
#include <QAction>
#include <QGLWidget>
#include <QStatusBar>
#include <QScrollBar>
#include <QTextCursor>
#include <QObject>
#include "QtNurbsCurveDialog.h"
#include "QtNurbsSurfDialog.h"
#include "QtRotSurfDialog.h"
#include "QtSweepSurfDialog.h"


// GUI�C���^�[�t�F�[�X�Ɉˑ��������b�v�֐��W
// Qt�o�[�W����
class GUI_Interface
{
public:
    int SetMessage(const char *);				// �R���\�[���Ƀ��b�Z�[�W���o��
    int SetCommand( const char *);              // �R���\�[���ɃR�}���h���o��
    void AddBodyNameToWin(const char *);        // �V���ɓo�^����BODY�̖��O���E�B���h�E���X�g�ɕ\������
    void DelBodyFromWin(int pos);				// �w�肳�ꂽ�ԍ���BODY�����X�g����폜����
    void SetEnableOpenFile(bool );              // �t�@�C��OPEN�p�I�u�W�F�N�g�̏�Ԑݒ�
    void SetEnableDelBody(bool );               // BODY�폜�p�I�u�W�F�N�g�̏�Ԑݒ�
    void UpdateDescribeWidget();				// �`��pGUI�̍ĕ`��w��
    void RedrawDescribeForm();                  // �`��̈�̍ĕ`��w��

    void SetInpFileName(const char *);          // INP�t�@�C������\������I�u�W�F�N�g�Ƀt�@�C������ݒ�

    void SetUserFunc1Label(const char *);       // UserFunc1�̃��x�����Z�b�g
    void SetUserFunc2Label(const char *);       // UserFunc2�̃��x�����Z�b�g
    void SetUserFunc3Label(const char *);       // UserFunc3�̃��x�����Z�b�g
    void SetUserFunc4Label(const char *);       // UserFunc4�̃��x�����Z�b�g
    void SetUserFunc5Label(const char *);       // UserFunc5�̃��x�����Z�b�g
    void SetUserFunc6Label(const char *);       // UserFunc6�̃��x�����Z�b�g
    void SetUserFunc7Label(const char *);       // UserFunc7�̃��x�����Z�b�g
    void SetUserFunc8Label(const char *);       // UserFunc8�̃��x�����Z�b�g

    std::string OpenBodyFileDlg();                          // �t�@�C���I�[�v���_�C�A���O�̕\���iBODY�j
    std::string OpenInpFileDlg();                           // �t�@�C���I�[�v���_�C�A���O�̕\���iINP�j
    std::string GetFileName(const std::string fullPath);    // �t���p�X����t�@�C�������o
    std::string GetFileExt(const std::string fullPath);		// �t���p�X����t�@�C���̊g���q���o

    std::string GetConsoleText();				// �R���\�[�����當������擾
    int GetBodyListSelectedPos();				// BODY���X�g�̑I������Ă���ԍ����擾����

    void SetConsoleObject(QTextEdit* textEdit);     // �R���\�[���pGUI�̃I�u�W�F�N�g�̃C���X�^���X���Z�b�g
    void SetBodyListObject(QListWidget* list);      // BODY���X�g�pGUI�̃I�u�W�F�N�g�̃C���X�^���X���Z�b�g
    void SetOpenFileActionObject(QAction* action);  // �t�@�C���I�[�v���pGUI�̃I�u�W�F�N�g�̃C���X�^���X���Z�b�g
    void SetDelBodyActionObject(QAction* action);	// BODY�폜�pGUI�̃I�u�W�F�N�g�̃C���X�^���X���Z�b�g
    void SetDescribeWidget(QGLWidget* widget);		// OpenGL�`��pGUI�̃I�u�W�F�N�g�̃C���X�^���X���Z�b�g
    void SetStatusBarObject(QStatusBar* bar);       // �X�e�[�^�X�o�[�pGUI�̃I�u�W�F�N�g�̃C���X�^���X���Z�b�g

    void SetUserFunc1Action(QAction* action);       // UserFunc1�pGUI�̃I�u�W�F�N�g�̃C���X�^���X���Z�b�g
    void SetUserFunc2Action(QAction* action);       // UserFunc2�pGUI�̃I�u�W�F�N�g�̃C���X�^���X���Z�b�g
    void SetUserFunc3Action(QAction* action);       // UserFunc3�pGUI�̃I�u�W�F�N�g�̃C���X�^���X���Z�b�g
    void SetUserFunc4Action(QAction* action);       // UserFunc4�pGUI�̃I�u�W�F�N�g�̃C���X�^���X���Z�b�g
    void SetUserFunc5Action(QAction* action);       // UserFunc5�pGUI�̃I�u�W�F�N�g�̃C���X�^���X���Z�b�g
    void SetUserFunc6Action(QAction* action);       // UserFunc6�pGUI�̃I�u�W�F�N�g�̃C���X�^���X���Z�b�g
    void SetUserFunc7Action(QAction* action);       // UserFunc7�pGUI�̃I�u�W�F�N�g�̃C���X�^���X���Z�b�g
    void SetUserFunc8Action(QAction* action);       // UserFunc8�pGUI�̃I�u�W�F�N�g�̃C���X�^���X���Z�b�g

    void ShowNurbsCurveDlg();     // Nurbs�Ȑ��_�C�A���O�\��
    void ShowNurbsSurfDlg();      // Nurbs�Ȗʃ_�C�A���O�\��
    void ShowRotSurfDlg();        // ��]�T�[�t�F�X�_�C�A���O�\��
    void ShowSweepSurfDlg();      // �X�C�[�v�T�[�t�F�X�_�C�A���O�\��

private:
    //GUI�I�u�W�F�N�g�̃C���X�^���X�ێ��p�i�g�p����c�[���L�b�g�ɉ����Đ錾����N���X��ύX�j
    QTextEdit* consoleText;         // �R���\�[���pGUI�I�u�W�F�N�g
    QListWidget* bodyList;      	// BODY���X�g�pGUI�I�u�W�F�N�g
    QAction* openFileAction;     	// �t�@�C���I�[�v���pGUI�I�u�W�F�N�g
    QAction* delBodyAction;   		// BODY�폜�pGUI�I�u�W�F�N�g
    QGLWidget* glWidget;            // OpenGL�`��pGUI�I�u�W�F�N�g
    QStatusBar* statusBar;      	// �X�e�[�^�X�o�[�pGUI�̃I�u�W�F�N�g

    QAction* userFunc1;   	// UserFunc1
    QAction* userFunc2;   	// UserFunc2
    QAction* userFunc3;   	// UserFunc3
    QAction* userFunc4;   	// UserFunc4
    QAction* userFunc5;   	// UserFunc5
    QAction* userFunc6;   	// UserFunc6
    QAction* userFunc7;   	// UserFunc7
    QAction* userFunc8;   	// UserFunc8

};

// �N���XGUI_Interface�̃C���X�^���X���O���[�o���Ő錾
extern GUI_Interface GuiIF;

#endif
