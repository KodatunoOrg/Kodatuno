#include "StdAfxWS.h"


// コンソールにメッセージを出力
int SetMessage(char *mes)
{
	fprintf(stderr,"%s\n",mes);						// メッセージを標準出力
	Console_Form->addString(mes);					// メッセージをコンソール出力
	Console_Form->addString("\n>");					// 次のコマンド位置へ移動
	int len = Console_Form->getString().getChars();	// 文字数カウント
	Console_Form->setProperty(WSNuserValue,len);	// 現在までの文字数を保持
	return len;
}

// コンソールにコマンドを出力
int SetCommand(char *cmd)
{
	Console_Form->addString(cmd);					// メッセージを出力
	int len = Console_Form->getString().getChars();	// 文字数カウント
	Console_Form->setProperty(WSNcursorPos,len);	// カーソルを最後にもってくる
	return len;
}

// 新たに登録したBODYの名前をウィンドウリストに表示する
void SetBodyNameToWin(char *name)
{
	Body_List_Win->addItem(name);									// BodyリストウィンドウにBODY名を登録
}
