#ifndef _STD_AFX__H_
#define _STD_AFX__H_

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <iostream>
#include <ctype.h>

// GUI�C���^�[�t�F�[�X�Ɉˑ��������b�v�֐��W
// Wide Studio�o�[�W����
class GUI_Interface
{
public:
	int SetMessage(char *);					// �R���\�[���Ƀ��b�Z�[�W���o��
	int SetCommand(char *);					// �R���\�[���ɃR�}���h���o��
	void AddBodyNameToWin(const char *);		// �V���ɓo�^����BODY�̖��O���E�B���h�E���X�g�ɕ\������
};

extern GUI_Interface GuiIF;		// �N���XGUI_Interface�̃C���X�^���X���O���[�o���Ő錾

#endif