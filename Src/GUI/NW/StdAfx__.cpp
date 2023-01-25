#include "StdAfx__.h"

GUI_Interface GuiIF;		// クラスGUI_Interfaceのインスタンスをグローバルで宣言


// コマンドプロンプトにメッセージを出力
int GUI_Interface::SetMessage(char *mes)
{
	fprintf(stderr,"%s\n",mes);

	return 0;
}


// 新たに登録したBODYの名前をウィンドウリストに表示する
void GUI_Interface::AddBodyNameToWin(const char *name)
{
	// void
}