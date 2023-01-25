#ifndef _STD_AFX__H_
#define _STD_AFX__H_

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <iostream>
#include <ctype.h>

// GUIインターフェースに依存したラップ関数集
// Wide Studioバージョン
class GUI_Interface
{
public:
	int SetMessage(char *);					// コンソールにメッセージを出力
	int SetCommand(char *);					// コンソールにコマンドを出力
	void AddBodyNameToWin(const char *);		// 新たに登録したBODYの名前をウィンドウリストに表示する
};

extern GUI_Interface GuiIF;		// クラスGUI_Interfaceのインスタンスをグローバルで宣言

#endif