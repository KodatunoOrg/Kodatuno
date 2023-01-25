// インプットファイル読込ボタンのプロシージャ
#include <WScom.h>
#include <WSCfunctionList.h>
#include <WSCbase.h>
#include "Kodatuno.h"

// Function: OpenInpFile_Proc
// インプットファイル読み込みボタンのプロシージャ
//
// Parameters: 
// *object - このプロシージャを呼び出したオブジェクトへのポインタ
void OpenInpFile_Proc(WSCbase* object)
{
	std::string full_name = Kodatuno.OpenInpFile();
	InpFileName->setProperty(WSNlabelString,full_name.c_str());	// ファイルのフルパスをユーザー設定文字列にセット

}
static WSCfunctionRegister  op("OpenInpFile_Proc",(void*)OpenInpFile_Proc);
