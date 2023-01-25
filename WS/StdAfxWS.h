#ifndef _STD_AFX_WS_H_
#define _STD_AFX_WS_H_

#include <WSCfileSelect.h>
#include <WSCmessageDialog.h>
#include <WSCimageSet.h>
#include <WSDkeyboard.h>
#include <WSDmwindowDev.h>
#include <WSDmouse.h>
#include <WSCcolorSet.h>
#include "Kodatuno_Win.h"


// コンソール操作
int SetMessage(char *);			// コンソールにメッセージを出力
int SetCommand(char *);			// コンソールにコマンドを出力
void SetBodyNameToWin(char *);	// 新たに登録したBODYの名前をウィンドウリストに表示する

#endif