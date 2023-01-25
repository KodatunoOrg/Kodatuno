// Nurbs�Ȗʐ����_�C�A���O�̃v���V�[�W��
#include <WScom.h>
#include <WSCfunctionList.h>
#include <WSCbase.h>
#include "Kodatuno.h"

void NurbsSDlg_Proc(WSCbase* object)
{
	if(NurbsSurface_Dlg == NULL)	return;

	if(NurbsSurface_Dlg->getStatus() == WS_DIALOG_OK){		// OK�{�^���������ꂽ

		WSCstring fname = InpFileName->getProperty(WSNlabelString);		// �C���v�b�g�t�@�C�����擾
		if(!fname.getChars()){
			SetMessage("You do not choose a file.");
			return;
		}

		int val = NurbsSOpt->getProperty(WSNvalue);			// ���W�I�{�^���̑I��ԍ��𓾂�
		switch(val){
			case 1:
				SetMessage("You chose generating Polygonal Nurbs Surface.");
				break;
			case 2:
				SetMessage("You chose generating Interpolated Nurbs Surface.");
				break;
			case 3:
			case 4:
			case 5:
				SetMessage("No supported");
				break;
		}

		WSCstring mes("Input file name is ");
		mes.addString(fname);
		SetMessage(mes);
		
		int rank = NurbsS_Rank->getProperty(WSNlabelString);	// ��������NURBS�Ȗʂ̓_�̐��ƊK�����擾
		if(rank > RANKMAX || rank <= 0){
			char mes[256];
			sprintf(mes,"KODATUNO ERROR: Inappropriate Rank ( 0 <= Rank <= %d)");
			SetMessage(mes);
			return;
		}

		Kodatuno.GenNurbsSurface(val,fname,rank);	// NURBS�Ȗʐ���

		NurbsSurface_Dlg->setVisible(False);		// �_�C�A���O�����

		Describe_Form->redraw();		// �`��̈�ɍĕ`����w��
	}
}
static WSCfunctionRegister  op("NurbsSDlg_Proc",(void*)NurbsSDlg_Proc);
