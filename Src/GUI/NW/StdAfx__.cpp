#include "StdAfx__.h"

GUI_Interface GuiIF;		// �N���XGUI_Interface�̃C���X�^���X���O���[�o���Ő錾


// �R�}���h�v�����v�g�Ƀ��b�Z�[�W���o��
int GUI_Interface::SetMessage(char *mes)
{
	fprintf(stderr,"%s\n",mes);

	return 0;
}


// �V���ɓo�^����BODY�̖��O���E�B���h�E���X�g�ɕ\������
void GUI_Interface::AddBodyNameToWin(const char *name)
{
	// void
}