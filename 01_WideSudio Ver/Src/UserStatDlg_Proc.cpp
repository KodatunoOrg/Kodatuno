// Userステータス設定のプロシージャ
#include <WScom.h>
#include <WSCfunctionList.h>
#include <WSCbase.h>
#include "Kodatuno.h"

void UserStatDlg_Proc(WSCbase* object)
{
	if(User_Stat_Dlg == NULL)	return;

	if(User_Stat_Dlg->getStatus() == WS_DIALOG_OK){	// OKボタンが押された

		int mode = UserMode1->getGroupValue();	// ラジオボタンの選択番号を得る

		// プロパティ値を得る
		double prop[USERPROPNUM];
		prop[0] = User_Prop1->getProperty(WSNlabelString);
		prop[1] = User_Prop2->getProperty(WSNlabelString);
		prop[2] = User_Prop3->getProperty(WSNlabelString);
		prop[3] = User_Prop4->getProperty(WSNlabelString);
		prop[4] = User_Prop5->getProperty(WSNlabelString);
		prop[5] = User_Prop6->getProperty(WSNlabelString);
		prop[6] = User_Prop7->getProperty(WSNlabelString);
		prop[7] = User_Prop8->getProperty(WSNlabelString);
		prop[8] = User_Prop9->getProperty(WSNlabelString);
		prop[9] = User_Prop10->getProperty(WSNlabelString);
		prop[10] = User_Prop11->getProperty(WSNlabelString);
		prop[11] = User_Prop12->getProperty(WSNlabelString);
		prop[12] = User_Prop13->getProperty(WSNlabelString);
		prop[13] = User_Prop14->getProperty(WSNlabelString);
		prop[14] = User_Prop15->getProperty(WSNlabelString);

		Kodatuno.AcceptUserStat(mode,prop);			// 設定値を取得

		User_Stat_Dlg->setVisible(False);			// ダイアログを閉じる

		SetMessage("User Status were Accepted."); 
	}

}

static WSCfunctionRegister  op("UserStatDlg_Proc",(void*)UserStatDlg_Proc);
