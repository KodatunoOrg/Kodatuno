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
			GuiIF.SetMessage("You do not choose a file.");
			return;
		}

		int val = NurbsSOpt->getProperty(WSNvalue);			// ���W�I�{�^���̑I��ԍ��𓾂�
		switch(val){
			case 1:
				GuiIF.SetMessage("You chose polygonal NURBS surface generation method.");
				break;
			case 2:
				GuiIF.SetMessage("You chose interpolated NURBS surface generation method.");
				break;
			case 3:
				GuiIF.SetMessage("You chose approximated NURBS surface generation method.");
				break;
			case 4:
				GuiIF.SetMessage("No supported");
				break;
			case 5:
				GuiIF.SetMessage("You chose NURBS surface generation method from control points.");
				break;
		}

		WSCstring mes("Input file name is ");
		mes.addString(fname);
		GuiIF.SetMessage(mes);
		
		int rank = NurbsS_Rank->getProperty(WSNlabelString);	// ��������NURBS�Ȗʂ̓_�̐��ƊK�����擾
		if(rank > RANKMAX || rank <= 0){
			char mes[256];
			sprintf(mes,"KODATUNO ERROR: Inappropriate Rank ( 0 <= Rank <= %d)",RANKMAX);
			GuiIF.SetMessage(mes);
			return;
		}

		Kodatuno.GenNurbsSurface(val,fname,rank);	// NURBS�Ȗʐ���

		NurbsSurface_Dlg->setVisible(False);		// �_�C�A���O�����

		Describe_Form->redraw();		// �`��̈�ɍĕ`����w��
	}
}
static WSCfunctionRegister  op("NurbsSDlg_Proc",(void*)NurbsSDlg_Proc);
