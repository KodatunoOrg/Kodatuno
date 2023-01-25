#ifndef _COMMAND_H_
#define _COMMAND_H_

#include<WSClistData.h>
#include"StdAfx.h"

#define MAXCOMMANDLEN	16			// コマンド文字数の最大値
#define MAXCOMMANDOPNUM 5			// 登録できるコマンドオプション数
#define COMMANDNUMMAX   200			// 登録できるコマンド数
#define SYSCOMMANDNUM	100			// Kodatunoがあらかじめ用意するコマンドの最大値(0-99)
#define USERCOMMANDNUM	100			// ユーザーが独自に登録できるコマンドの最大値(100-199)

// コマンド用構造体
typedef struct{
	char Name[MAXCOMMANDLEN];	// コマンド名
	char Op[MAXCOMMANDOPNUM];	// オプション名
	int  CmdNo;					// キー
	void (*Command)(int,char *[]);			// コマンド実行関数へのポインタ
}CommandMap;

void SetCmdList(CommandMap *,char *,char *,int,void (*Command)(int,char *[]));	// コマンドを登録
int AnalCommandLine(char *,char *[]);				// コマンド解析
int SearchRegdCmdNum(CommandMap *,char *);			// 引数で指定したコマンドの登録Noを調べる




#endif
