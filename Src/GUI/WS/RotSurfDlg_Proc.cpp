// ��]�T�[�t�F�X�����_�C�A���O�̃v���V�[�W��

#include <WScom.h>
#include <WSCfunctionList.h>
#include <WSCbase.h>
#include "Kodatuno.h"

void RotSurfDlg_Proc(WSCbase* object)
{
	if(RotSurf_Dlg == NULL)
		return;

	if(RotSurf_Dlg->getStatus() == WS_DIALOG_OK){		// OK�{�^���������ꂽ
		Coord ax;
		ax.x = AOR_X->getProperty(WSNlabelString);		// �e�p�����[�^�擾
		ax.y = AOR_Y->getProperty(WSNlabelString);
		ax.z = AOR_Z->getProperty(WSNlabelString);
		double ang = AOR_Ang->getProperty(WSNlabelString);
		Kodatuno.GenSurface(ax,ang,ROTSURF);			// ��]�T�[�t�F�X����
		GuiIF.SetMessage("End");
		RotSurf_Dlg->setVisible(False);					// �_�C�A���O�����
	}
		
	Describe_Form->redraw();
}

static WSCfunctionRegister  op("RotSurfDlg_Proc",(void*)RotSurfDlg_Proc);
