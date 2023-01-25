// BODYリストのプロシージャ
#include <WScom.h>
#include <WSCfunctionList.h>
#include <WSCbase.h>
#include "Kodatuno.h"

// Function: BodyListSel_Proc
// BODYリストのプロシージャ
//
// Parameters: 
// object - このプロシージャを呼び出したオブジェクトへのポインタ
void BodyListSel_Proc(WSCbase* object)
{
	Kodatuno.BodyListSelect();		// セレクトされたアイテムの番号を得る
}
static WSCfunctionRegister  op("BodyListSel_Proc",(void*)BodyListSel_Proc);
