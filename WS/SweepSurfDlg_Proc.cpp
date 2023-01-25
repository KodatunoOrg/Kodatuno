// �X�C�[�v�T�[�t�F�X�����_�C�A���O�̃v���V�[�W��

#include <WScom.h>
#include <WSCfunctionList.h>
#include <WSCbase.h>
#include "Kodatuno.h"

void SweepSurfDlg_Proc(WSCbase* object)
{
 	if(SweepSurf_Dlg == NULL)
		return;

	if(SweepSurf_Dlg->getStatus() == WS_DIALOG_OK){		// OK�{�^���������ꂽ
		Coord ax;
		ax.x = DOS_X->getProperty(WSNlabelString);		// �e�p�����[�^�擾
		ax.y = DOS_Y->getProperty(WSNlabelString);
		ax.z = DOS_Z->getProperty(WSNlabelString);
		double len = Sweep_Length->getProperty(WSNlabelString);
		Kodatuno.GenSurface(ax,len,SWEEPSURF);				// ��]�T�[�t�F�X����
		SetMessage("End");
		SweepSurf_Dlg->setVisible(False);					// �_�C�A���O�����
	}

	Describe_Form->redraw();
}
static WSCfunctionRegister  op("SweepSurfDlg_Proc",(void*)SweepSurfDlg_Proc);
