// Kodatuno���C���t�H�[���̃v���V�[�W��

#include <WScom.h>
#include <WSCfunctionList.h>
#include <WSCbase.h>
#include <WSDkeyboard.h>
#include "Kodatuno.h"

KODatUNO Kodatuno;		// Kodatuno�N���X�̃C���X�^���X���O���[�o���Ő錾

// �O���[�o���L�[�t�b�N
WSCbool keyhook(long key,WSCbool onoff)
{
	if(onoff == False)	return True;	// �{�^�������[�X���͉������Ȃ�

	WSCbool flag=True;

	switch(key){
		case WSK_Up:
			flag = Kodatuno.ChangeViewUp();
			break;
		case WSK_Down:
			flag = Kodatuno.ChangeViewDown();
			break;
		case WSK_Left:
			flag = Kodatuno.ChangeViewLeft();
			break;
		case WSK_Right:
			flag = Kodatuno.ChangeViewRight();
			break;
		default:
			return True;
	}

	// �R���g���[���L�[��������Ă��Ȃ��ꍇ��True�����^�[�����A�L�[�C�x���g��z��
	// �R���g���[���L�[��������Ă���ꍇ��False�����^�[�����A���L�[�C�x���g�͔z��Ȃ�
	// �����Console�̗���\����View�̉�]�Ƃ��g��������
	return flag;
}

void Kodatuno_Win_Proc(WSCbase* object)
{
  	WSCprocedure *pwproc = object->getExecutedProcedure();	// �v���V�[�W�����擾

	switch(pwproc->getTrigger()){		// �擾�����v���V�[�W����
		case WSEV_INITIALIZE:			// INITIALIZE��������
			Kodatuno.InitializeWin();	// �E�B���h�E�������֐������s
			WSGIappKeyboard()->setGlobalKeyHook(keyhook);	// �O���[�o���L�[�t�b�N�̐ݒ�
			break;
		case WSEV_EXIT:					// EXIT��������
			Kodatuno.DeleteWin();		// �E�B���h�E�I�������֐������s
			break;
		case WSEV_RESIZE:
			Kodatuno.GetResizeWindow();
			break;
		default:						// ����ȊO�͉������Ȃ�
			break;
	}
}
static WSCfunctionRegister  op("Kodatuno_Win_Proc",(void*)Kodatuno_Win_Proc);

