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
	
	long KODbtn;
	switch(btn){
		case WS_MOUSE_BTN1:
			KODbtn = KDT_MOUSE_LEFT;
			break;
		case WS_MOUSE_BTN2:
			KODbtn = KDT_MOUSE_MIDDLE;
			break;
		case WS_MOUSE_BTN3:
			KODbtn = KDT_MOUSE_RIGHT;
			break;
		default:
			KODbtn = KDT_MOUSE_NONE;
			break;
	}

	long KODtrig;
	switch(trig){
		case WSEV_MOUSE_PRESS:
			KODtrig = KDT_MOUSE_PRESS;
			break;
		case WSEV_MOUSE_RELEASE:
			KODtrig = KDT_MOUSE_RELEASE;
			break;
	}

	WSDkeyboard *key = WSGIappKeyboard();
	long KODkey;
	if(key->withCntl()){
		KODkey = KDT_MOD_CTRL;
	}
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
			Kodatuno.MouseEvent(KODtrig,KODbtn,x,y);	// �}�E�X�C�x���g������
			break;	
		case WSEV_MOUSE_MOVE:					// �}�E�X�ړ���
			Kodatuno.MouseMotionEvent(KODbtn,KODkey,x,y);	// ������Ă���{�^���ɉ����ăC�x���g�����s
			break;
		default:
			break;
	}

}

static WSCfunctionRegister  op("Describe_Proc",(void*)Describe_Proc);
