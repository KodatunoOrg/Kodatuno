// �`��̈�̃v���V�[�W���[

#include <WScom.h>
#include <WSCfunctionList.h>
#include <WSCbase.h>
#include "Kodatuno.h"

void Describe_Proc(WSCbase* object)
{		
	WSCprocedure *pwproc = object->getExecutedProcedure();	// �v���V�[�W���̃C���X�^���X�𐶐�
	WSDmouse *mouse = WSGIappMouse();						// �}�E�X�̃C���X�^���X�𐶐�

	short x,y;
	mouse->getMousePosition(&x,&y,Describe_Form);			// �}�E�X�ʒu���擾
	long btn=mouse->getTargetBtn();							// �ǂ̃{�^���������������擾
	long trig=pwproc->getTrigger();							// �g���K�[�擾

	switch(trig){

		// �ĕ`��w�߂̏ꍇ
		case WSEV_EXPOSE:
			Kodatuno.ReDraw(x,y);				// BODY�`��
			Kodatuno.ReDrawUserFunc();			// UserFunc�ɂ��`��w�߂����s
			Kodatuno.ReDrawUserCommand();		// UserCommand�ɂ��`��w�߂����s
			break;

		// �}�E�X�̏���
		case WSEV_MOUSE_PRESS:					// �}�E�X����	
		case WSEV_MOUSE_RELEASE:				// �}�E�X�̃{�^���𗣂���
			Kodatuno.MouseEvent(trig,btn,x,y);	// �}�E�X�C�x���g������
			break;	
		case WSEV_MOUSE_MOVE:					// �}�E�X�ړ���
			Kodatuno.MouseMotionEvent(btn,x,y);	// ������Ă���{�^���ɉ����ăC�x���g�����s
			break;
		default:
			break;
	}

}

static WSCfunctionRegister  op("Describe_Proc",(void*)Describe_Proc);
