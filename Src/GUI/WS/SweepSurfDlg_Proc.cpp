// スイープサーフェス生成ダイアログのプロシージャ

#include <WScom.h>
#include <WSCfunctionList.h>
#include <WSCbase.h>
#include "Kodatuno.h"

// Function: SweepSurfDlg_Proc
// スイープサーフェス生成ダイヤログのプロシージャ
//
// Parameters: 
// *object - このプロシージャを呼び出したオブジェクトへのポインタ
void SweepSurfDlg_Proc(WSCbase* object)
{
 	if(SweepSurf_Dlg == NULL)
		return;

	if(SweepSurf_Dlg->getStatus() == WS_DIALOG_OK){		// OKボタンが押された
		Coord ax;
		ax.x = DOS_X->getProperty(WSNlabelString);		// 各パラメータ取得
		ax.y = DOS_Y->getProperty(WSNlabelString);
		ax.z = DOS_Z->getProperty(WSNlabelString);
		double len = Sweep_Length->getProperty(WSNlabelString);
		Kodatuno.GenSurface(ax,len,SWEEPSURF);				// 回転サーフェス生成
		GuiIF.SetMessage("End");
		SweepSurf_Dlg->setVisible(False);					// ダイアログを閉じる
	}

	Describe_Form->redraw();
}
static WSCfunctionRegister  op("SweepSurfDlg_Proc",(void*)SweepSurfDlg_Proc);
