#include "StdAfxWS.h"


// �R���\�[���Ƀ��b�Z�[�W���o��
int SetMessage(char *mes)
{
	fprintf(stderr,"%s\n",mes);						// ���b�Z�[�W��W���o��
	Console_Form->addString(mes);					// ���b�Z�[�W���R���\�[���o��
	Console_Form->addString("\n>");					// ���̃R�}���h�ʒu�ֈړ�
	int len = Console_Form->getString().getChars();	// �������J�E���g
	Console_Form->setProperty(WSNuserValue,len);	// ���݂܂ł̕�������ێ�
	return len;
}

// �R���\�[���ɃR�}���h���o��
int SetCommand(char *cmd)
{
	Console_Form->addString(cmd);					// ���b�Z�[�W���o��
	int len = Console_Form->getString().getChars();	// �������J�E���g
	Console_Form->setProperty(WSNcursorPos,len);	// �J�[�\�����Ō�ɂ����Ă���
	return len;
}

// �V���ɓo�^����BODY�̖��O���E�B���h�E���X�g�ɕ\������
void SetBodyNameToWin(char *name)
{
	Body_List_Win->addItem(name);									// Body���X�g�E�B���h�E��BODY����o�^
}
