// 描画領域のプロシージャー

#include <WScom.h>
#include <WSCfunctionList.h>
#include <WSCbase.h>
#include "Kodatuno.h"

void Describe_Proc(WSCbase* object)
{		
	WSCprocedure *pwproc = object->getExecutedProcedure();	// プロシージャのインスタンスを生成
	WSDmouse *mouse = WSGIappMouse();						// マウスのインスタンスを生成

	short x,y;
	mouse->getMousePosition(&x,&y,Describe_Form);			// マウス位置を取得
	long btn=mouse->getTargetBtn();							// どのボタンを押したかを取得
	long trig=pwproc->getTrigger();							// トリガー取得

	switch(trig){

		// 再描画指令の場合
		case WSEV_EXPOSE:
			Kodatuno.ReDraw(x,y);				// BODY描画
			Kodatuno.ReDrawUserFunc();			// UserFuncによる描画指令を実行
			Kodatuno.ReDrawUserCommand();		// UserCommandによる描画指令を実行
			break;

		// マウスの処理
		case WSEV_MOUSE_PRESS:					// マウス押下	
		case WSEV_MOUSE_RELEASE:				// マウスのボタンを離した
			Kodatuno.MouseEvent(trig,btn,x,y);	// マウスイベントを処理
			break;	
		case WSEV_MOUSE_MOVE:					// マウス移動中
			Kodatuno.MouseMotionEvent(btn,x,y);	// 押されているボタンに応じてイベントを実行
			break;
		default:
			break;
	}

}

static WSCfunctionRegister  op("Describe_Proc",(void*)Describe_Proc);
