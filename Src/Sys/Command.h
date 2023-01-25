#ifndef _COMMAND_H_
#define _COMMAND_H_

#include"StdAfx.h"

// Constants: General Defines
// MAXCOMMANDLEN -					コマンド文字数の最大値(16)
// MAXCOMMANDOPNUM -				登録できるコマンドオプション数(5)
// COMMANDNUMMAX -					登録できるコマンド数(200)
// SYSCOMMANDNUM -					Kodatunoがあらかじめ用意するコマンドの最大値(100)
// USERCOMMANDNUM - 				ユーザーが独自に登録できるコマンドの最大値(100)
#define MAXCOMMANDLEN	16			
#define MAXCOMMANDOPNUM 5			
#define COMMANDNUMMAX   200			
#define SYSCOMMANDNUM	100			
#define USERCOMMANDNUM	100			

// Structures: CommandMap
// コマンド用構造体
// 
// Variables:
// char Name[MAXCOMMANDLEN] - 		コマンド名
// char Op[MAXCOMMANDOPNUM] - 		オプション名
// int  CmdNo -						キー
// void (*Command)(int,char *[]) - 	コマンド実行関数へのポインタ
typedef struct{
	char Name[MAXCOMMANDLEN];	
	char Op[MAXCOMMANDOPNUM];	
	int  CmdNo;					
	void (*Command)(int,char *[]);			
}CommandMap;

// Function: SetCmdList
// コマンドを追加する
void SetCmdList(CommandMap *,const char *,const char *,int,void (*Command)(int,char *[]));	

// Function: AnalCommandLine
// コマンドを解析する
int AnalCommandLine(char *,char *[]);				

// Function: SearchRegdCmdNum
// 引数で指定したコマンドの登録Noを調べる
int SearchRegdCmdNum(CommandMap *,char *);			




#endif
