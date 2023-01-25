// Nurbs�Ȑ������_�C�A���O�̃v���V�[�W��
#include <WScom.h>
#include <WSCfunctionList.h>
#include <WSCbase.h>
#include "Kodatuno.h"


void NurbsCDlg_Proc(WSCbase* object)
{
	if(NurbsCurve_Dlg == NULL)	return;

	if(NurbsCurve_Dlg->getStatus() == WS_DIALOG_OK){		// OK�{�^���������ꂽ

		WSCstring fname = InpFileName->getProperty(WSNlabelString);		// �C���v�b�g�t�@�C�����擾
		if(!fname.getChars()){
			GuiIF.SetMessage("You do not choose a file.");
			return;
		}

		int val = NurbsCOpt->getProperty(WSNvalue);			// ���W�I�{�^���̑I��ԍ��𓾂�
		switch(val){
			case 1:
				GuiIF.SetMessage("You chose generating Polygonal Nurbs Curve.");
				break;
			case 2:
				GuiIF.SetMessage("You chose generating Interpolated Nurbs Curve (1).");
				break;
			case 3:
				GuiIF.SetMessage(" You chose generating Approximated Nurbs Curve.");
				break;
			case 4:
				GuiIF.SetMessage("You chose generating Interpolated Nurbs Curve (2).");
				break;
			case 5:
				GuiIF.SetMessage("You chose NURBS curve generation method from control points.");
				break;
		}

		WSCstring mes("Input file name is ");
		mes.addString(fname);
		GuiIF.SetMessage(mes);

		int rank = NurbsC_Rank->getProperty(WSNlabelString);	// ��������NURBS�Ȑ��̊K�����擾
		if(rank > RANKMAX || rank <= 0){
			char mes[256];
			sprintf(mes,"KODATUNO ERROR: Inappropriate Rank ( 0 <= Rank <= %d)",RANKMAX);
			GuiIF.SetMessage(mes);
			return;
		}

		Kodatuno.GenNurbsCurve(val,fname,rank);		// NURBS�Ȑ�����

		NurbsCurve_Dlg->setVisible(False);			// �_�C�A���O�����

		Describe_Form->redraw();		// �`��̈�ɍĕ`����w��
	}
}
static WSCfunctionRegister  op("NurbsCDlg_Proc",(void*)NurbsCDlg_Proc);
