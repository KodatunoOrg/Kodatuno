// ���j���[�o�[�̃v���V�[�W��
#include <WScom.h>
#include <WSCfunctionList.h>
#include <WSCbase.h>
#include "Kodatuno.h"

void Menu_Proc(WSCbase* object)
{
	int val = object->getProperty(WSNuserValue);	// ���[�U�[�ݒ�l���擾

	switch(val){
		case 1:
  			Kodatuno.OpenFile();			// �t�@�C���I�[�v��
			Kodatuno.OpenDelBtn();			// Open�CDelete�{�^���̕\�������̕ύX
			break;
		case 2:
			Kodatuno.DeleteBody();			// �I������Ă���BODY������
			Kodatuno.OpenDelBtn();			// Open�CDelete�{�^���̕\�������̕ύX
			break;
		case 100:
			User_Stat_Dlg->setVisible(True);	// User�X�e�[�^�X�_�C�A���O��\��
			break;
		case 101:
		case 102:
		case 103:
		case 104:
		case 105:
			Kodatuno.ExecuteUserFuncMain(val%100-1);	// User�֐��̎��s
			break;
		default:
			break;
	}

	Describe_Form->redraw();		// �`��̈�ɍĕ`����w��
}
static WSCfunctionRegister  op("Menu_Proc",(void*)Menu_Proc);
