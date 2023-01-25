// BODYリストのプロシージャー
#include <WScom.h>
#include <WSCfunctionList.h>
#include <WSCbase.h>
#include "Kodatuno.h"

// BODYリストのアイテムがセレクトされた
void BodyListSel_Proc(WSCbase* object)
{
	Kodatuno.BodyListSelect();		// セレクトされたアイテムの番号を得る
}
static WSCfunctionRegister  op("BodyListSel_Proc",(void*)BodyListSel_Proc);
