// Kodatunoメインフォームのプロシージャ

#include <WScom.h>
#include <WSCfunctionList.h>
#include <WSCbase.h>
#include <WSDkeyboard.h>
#include "Kodatuno.h"

KODatUNO Kodatuno;		// Kodatunoクラスのインスタンスをグローバルで宣言

// Function: keyhook
// グローバルキーフック
//
// Parameters: 
// key - キーコードを格納
// onoff - キーのオン・オフを格納（True:オン，False:オフ）
//
// Return: 
// コントロールキーが押されていない場合はTrueをリターンし、キーイベントを配る
// コントロールキーが押されている場合はFalseをリターンし、矢印キーイベントは配らない
WSCbool keyhook(long key,WSCbool onoff)
{
	if(onoff == False)	return True;	// ボタンリリース時は何もしない

	WSCbool flag=True;

	if(WSGIappKeyboard()->withCntl()){
		switch(key){
			case WSK_Up:
				flag = Kodatuno.ChangeViewUp();
				break;
			case WSK_Down:
				flag = Kodatuno.ChangeViewDown();
				break;
			case WSK_Left:
				flag = Kodatuno.ChangeViewLeft();
				break;
			case WSK_Right:
				flag = Kodatuno.ChangeViewRight();
				break;
			default:
				return True;
		}
	}

	// コントロールキーが押されていない場合はTrueをリターンし、キーイベントを配る
	// コントロールキーが押されている場合はFalseをリターンし、矢印キーイベントは配らない
	// これでConsoleの履歴表示とViewの回転とを使い分ける
	return flag;
}

// Function: Kodatuno_Win_Proc
// Kodatunoメインフォームのプロシージャ
//
// Parameters: 
// *object - このプロシージャを呼び出したオブジェクトへのポインタ
void Kodatuno_Win_Proc(WSCbase* object)
{
  	WSCprocedure *pwproc = object->getExecutedProcedure();	// プロシージャを取得

	switch(pwproc->getTrigger()){		// 取得したプロシージャが
		case WSEV_INITIALIZE:			// INITIALIZEだったら
			Kodatuno.InitializeWin(Describe_Form->getProperty(WSNwidth),Describe_Form->getProperty(WSNheight));	// ウィンドウ初期化関数を実行
			WSGIappKeyboard()->setGlobalKeyHook(keyhook);	// グローバルキーフックの設定
			break;
		case WSEV_EXIT:					// EXITっだたら
			Kodatuno.DeleteWin();		// ウィンドウ終了処理関数を実行
			break;
		case WSEV_RESIZE:
			Kodatuno.GetResizeWindow(Describe_Form->getProperty(WSNwidth),Describe_Form->getProperty(WSNheight));
			break;
		default:						// それ以外は何もしない
			break;
	}
}
static WSCfunctionRegister  op("Kodatuno_Win_Proc",(void*)Kodatuno_Win_Proc);
