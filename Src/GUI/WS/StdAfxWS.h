#ifndef _STD_AFX_WS_H_
#define _STD_AFX_WS_H_

#include <WSCfileSelect.h>
#include <WSCmessageDialog.h>
#include <WSCimageSet.h>
#include <WSDkeyboard.h>
#include <WSDmwindowDev.h>
#include <WSDmouse.h>
#include <WSCcolorSet.h>
#include "Kodatuno_Win.h"

#include <string.h>
#include <iostream>

// GUI�C���^�[�t�F�[�X�Ɉˑ��������b�v�֐��W
// Wide Studio�o�[�W����
class GUI_Interface
{
public:
	int SetMessage(char *);					// �R���\�[���Ƀ��b�Z�[�W���o��
	int SetCommand(char *);					// �R���\�[���ɃR�}���h���o��
	void AddBodyNameToWin(const char *);	// �V���ɓo�^����BODY�̖��O���E�B���h�E���X�g�ɕ\������
	void DelBodyFromWin(int pos);			// �w�肳�ꂽ�ԍ���BODY�����X�g����폜����
    void SetEnableOpenFile(bool );			// �t�@�C��OPEN�p�I�u�W�F�N�g�̏�Ԑݒ�
    void SetEnableDelBody(bool );			// BODY�폜�p�I�u�W�F�N�g�̏�Ԑݒ�
    void UpdateDescribeWidget();			// �`��pGUI�̍ĕ`��w��
	void RedrawDescribeForm();				// �`��̈�̍ĕ`��w��

    void SetUserFunc1Label(const char *);	// UserFunc1�̃��x�����Z�b�g
    void SetUserFunc2Label(const char *);	// UserFunc2�̃��x�����Z�b�g
    void SetUserFunc3Label(const char *);	// UserFunc3�̃��x�����Z�b�g
    void SetUserFunc4Label(const char *);	// UserFunc4�̃��x�����Z�b�g
    void SetUserFunc5Label(const char *);	// UserFunc5�̃��x�����Z�b�g
    void SetUserFunc6Label(const char *);	// UserFunc6�̃��x�����Z�b�g
    void SetUserFunc7Label(const char *);	// UserFunc7�̃��x�����Z�b�g
    void SetUserFunc8Label(const char *);	// UserFunc8�̃��x�����Z�b�g
  
    std::string OpenBodyFileDlg();			// �t�@�C���I�[�v���_�C�A���O�̕\���iBODY�j
    std::string OpenInpFileDlg();			// �t�@�C���I�[�v���_�C�A���O�̕\���iINP�j
    std::string GetFileName(const std::string fullPath);	// �t���p�X����t�@�C�������o
    std::string GetFileExt(const std::string fullPath);		// �t���p�X����t�@�C���̊g���q���o

    std::string GetConsoleText();			// �R���\�[�����當������擾
    int GetBodyListSelectedPos();			// BODY���X�g�̑I������Ă���ԍ����擾����

    void ShowNurbsCurveDlg();				// Nurbs�Ȑ��_�C�A���O�\��
    void ShowNurbsSurfDlg();				// Nurbs�Ȗʃ_�C�A���O�\��
    void ShowRotSurfDlg();					// ��]�T�[�t�F�X�_�C�A���O�\��
    void ShowSweepSurfDlg();				// �X�C�[�v�T�[�t�F�X�_�C�A���O�\��

};

extern GUI_Interface GuiIF;		// �N���XGUI_Interface�̃C���X�^���X���O���[�o���Ő錾

#endif