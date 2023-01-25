﻿// 3Dデータファイルの読み込みボタンのプロシージャ

#include <WScom.h>
#include <WSCfunctionList.h>
#include <WSCbase.h>
#include "Kodatuno.h"

// Function: OpenFile_Proc
// 3Dデータファイルの読み込みボタンのプロシージャ
// 
// Parameters:
// *object - このプロシージャを呼び出したオブジェクトへのポインタ
void OpenFile_Proc(WSCbase* object)
{
	Kodatuno.OpenFile();			// ファイルオープン
	Kodatuno.SetModelScale();		// モデルスケールを設定
	Describe_Form->redraw();		// 描画領域に再描画を指示
}
static WSCfunctionRegister  op("OpenFile_Proc",(void*)OpenFile_Proc);
