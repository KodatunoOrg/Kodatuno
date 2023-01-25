// ���_�؊��{�^���̃v���V�[�W��
#include <WScom.h>
#include <WSCfunctionList.h>
#include <WSCbase.h>
#include "Kodatuno.h"

void ChangeView_Proc(WSCbase* object)
{

	int val = object->getProperty(WSNuserValue);	// ���[�U�[�ݒ�l���擾

	switch(val){
		case 1:
			Kodatuno.ChangeViewX();		// X�����r���[
			break;
		case 2:
			Kodatuno.ChangeViewY();		// Y�����r���[
			break;
		case 3:
			Kodatuno.ChangeViewZ();		// Z�����r���[
			break;
		case 4:
			Kodatuno.ChangeViewXYZ();	// ���Ճr���[
			break;
		case 5:
			Kodatuno.FitView();			// �{��������
			break;
		case 10:
			Kodatuno.SolidView();		// �\���b�h�\��
			break;
		case 11:
			Kodatuno.NoEdgeView();		// �G�b�W���\���ɂ���
			break;
		case 12:
			Kodatuno.WireFlameView();	// ���C���[�t���[���\��
			break;		
		case 13:
			Kodatuno.VoidView();		// �\�����Ȃ�
			break;	
		case 100:
			Kodatuno.SelectAll();		// �S�ăZ���N�g
			break;
		case 101:
			Kodatuno.SelectionCancel();	// �Z���N�V�����L�����Z��
			break;
		case 102:
			Kodatuno.UserViewCanncel();	// Userfunc()�ɂ���ĕ`�悳�ꂽ���̂�����
			break;
		case 1000:
			Kodatuno.ViewBlending();	// BODY�𔼓����ɂ���
			break;
		default:
			break;
	}

	Describe_Form->redraw();

}
static WSCfunctionRegister  op("ChangeView_Proc",(void*)ChangeView_Proc);
