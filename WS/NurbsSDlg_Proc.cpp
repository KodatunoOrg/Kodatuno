// Nurbs曲面生成ダイアログのプロシージャ
#include <WScom.h>
#include <WSCfunctionList.h>
#include <WSCbase.h>
#include "Kodatuno.h"

void NurbsSDlg_Proc(WSCbase* object)
{
	if(NurbsSurface_Dlg == NULL)	return;

	if(NurbsSurface_Dlg->getStatus() == WS_DIALOG_OK){		// OKボタンが押された

		WSCstring fname = InpFileName->getProperty(WSNlabelString);		// インプットファイル名取得
		if(!fname.getChars()){
			SetMessage("You do not choose a file.");
			return;
		}

		int val = NurbsSOpt->getProperty(WSNvalue);			// ラジオボタンの選択番号を得る
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
		
		int rank = NurbsS_Rank->getProperty(WSNlabelString);	// 生成するNURBS曲面の点の数と階数を取得
		if(rank > RANKMAX || rank <= 0){
			char mes[256];
			sprintf(mes,"KODATUNO ERROR: Inappropriate Rank ( 0 <= Rank <= %d)");
			SetMessage(mes);
			return;
		}

		Kodatuno.GenNurbsSurface(val,fname,rank);	// NURBS曲面生成

		NurbsSurface_Dlg->setVisible(False);		// ダイアログを閉じる

		Describe_Form->redraw();		// 描画領域に再描画を指示
	}
}
static WSCfunctionRegister  op("NurbsSDlg_Proc",(void*)NurbsSDlg_Proc);
