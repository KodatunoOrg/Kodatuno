// 回転サーフェス生成ダイアログのプロシージャ

#include <WScom.h>
#include <WSCfunctionList.h>
#include <WSCbase.h>
#include "Kodatuno.h"

// Function: RotSurfDlg_Proc
// 回転サーフェス生成ダイヤログのプロシージャ
//
// Parameters: 
// *object - このプロシージャを呼び出したオブジェクトへのポインタ
void RotSurfDlg_Proc(WSCbase* object)
{
	if(RotSurf_Dlg == NULL)
		return;

	if(RotSurf_Dlg->getStatus() == WS_DIALOG_OK){		// OKボタンが押された
		Coord ax;
		ax.x = AOR_X->getProperty(WSNlabelString);		// 各パラメータ取得
		ax.y = AOR_Y->getProperty(WSNlabelString);
		ax.z = AOR_Z->getProperty(WSNlabelString);
		double ang = AOR_Ang->getProperty(WSNlabelString);
		Kodatuno.GenSurface(ax,ang,ROTSURF);			// 回転サーフェス生成
		GuiIF.SetMessage("End");
		RotSurf_Dlg->setVisible(False);					// ダイアログを閉じる
	}
		
	Describe_Form->redraw();
}

static WSCfunctionRegister  op("RotSurfDlg_Proc",(void*)RotSurfDlg_Proc);
