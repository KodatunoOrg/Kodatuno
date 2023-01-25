// Nurbs曲面生成ダイアログのプロシージャ
#include <WScom.h>
#include <WSCfunctionList.h>
#include <WSCbase.h>
#include "Kodatuno.h"

// Function: NurbsSDlg_Proc
// Nurbs曲面生成ダイヤログのプロシージャ
//
// Parameters: 
// *object - このプロシージャを呼び出したオブジェクトへのポインタ
void NurbsSDlg_Proc(WSCbase* object)
{
	if(NurbsSurface_Dlg == NULL)	return;

	if(NurbsSurface_Dlg->getStatus() == WS_DIALOG_OK){		// OKボタンが押された

		WSCstring fname = InpFileName->getProperty(WSNlabelString);		// インプットファイル名取得
		if(!fname.getChars()){
			GuiIF.SetMessage("You do not choose a file.");
			return;
		}

		int val = NurbsSOpt->getProperty(WSNvalue);			// ラジオボタンの選択番号を得る
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
		
		int rank = NurbsS_Rank->getProperty(WSNlabelString);	// 生成するNURBS曲面の点の数と階数を取得
		if(rank > RANKMAX || rank <= 0){
			char mes[256];
			sprintf(mes,"KODATUNO ERROR: Inappropriate Rank ( 0 <= Rank <= %d)",RANKMAX);
			GuiIF.SetMessage(mes);
			return;
		}

		Kodatuno.GenNurbsSurface(val,fname,rank);	// NURBS曲面生成

		NurbsSurface_Dlg->setVisible(False);		// ダイアログを閉じる

		Describe_Form->redraw();		// 描画領域に再描画を指示
	}
}
static WSCfunctionRegister  op("NurbsSDlg_Proc",(void*)NurbsSDlg_Proc);
