// メニューバーのプロシージャ
#include <WScom.h>
#include <WSCfunctionList.h>
#include <WSCbase.h>
#include "Kodatuno.h"

void Menu_Proc(WSCbase* object)
{
	int val = object->getProperty(WSNuserValue);	// ユーザー設定値を取得

	switch(val){
		case 1:
  			Kodatuno.OpenFile();			// ファイルオープン
			Kodatuno.OpenDelBtn();			// Open，Deleteボタンの表示属性の変更
			break;
		case 2:
			Kodatuno.DeleteBody();			// 選択されているBODYを消去
			Kodatuno.OpenDelBtn();			// Open，Deleteボタンの表示属性の変更
			break;
		case 100:
			User_Stat_Dlg->setVisible(True);	// Userステータスダイアログを表示
			break;
		case 101:
		case 102:
		case 103:
		case 104:
		case 105:
			Kodatuno.ExecuteUserFuncMain(val%100-1);	// User関数の実行
			break;
		default:
			break;
	}

	Describe_Form->redraw();		// 描画領域に再描画を指示
}
static WSCfunctionRegister  op("Menu_Proc",(void*)Menu_Proc);
