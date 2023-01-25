// 視点切換ボタンのプロシージャ
#include <WScom.h>
#include <WSCfunctionList.h>
#include <WSCbase.h>
#include "Kodatuno.h"

void ChangeView_Proc(WSCbase* object)
{

	int val = object->getProperty(WSNuserValue);	// ユーザー設定値を取得

	switch(val){
		case 1:
			Kodatuno.ChangeViewX();		// X方向ビュー
			break;
		case 2:
			Kodatuno.ChangeViewY();		// Y方向ビュー
			break;
		case 3:
			Kodatuno.ChangeViewZ();		// Z方向ビュー
			break;
		case 4:
			Kodatuno.ChangeViewXYZ();	// 鳥瞰ビュー
			break;
		case 5:
			Kodatuno.FitView();			// 倍率初期化
			break;
		case 10:
			Kodatuno.SolidView();		// ソリッド表示
			break;
		case 11:
			Kodatuno.NoEdgeView();		// エッジを非表示にする
			break;
		case 12:
			Kodatuno.WireFlameView();	// ワイヤーフレーム表示
			break;		
		case 13:
			Kodatuno.VoidView();		// 表示しない
			break;	
		case 100:
			Kodatuno.SelectAll();		// 全てセレクト
			break;
		case 101:
			Kodatuno.SelectionCancel();	// セレクションキャンセル
			break;
		case 102:
			Kodatuno.UserViewCanncel();	// Userfunc()によって描画されたものを消す
			break;
		case 1000:
			Kodatuno.ViewBlending();	// BODYを半透明にする
			break;
		default:
			break;
	}

	Describe_Form->redraw();

}
static WSCfunctionRegister  op("ChangeView_Proc",(void*)ChangeView_Proc);
