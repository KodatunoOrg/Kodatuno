#include "StdAfx__.h"

// コマンドプロンプトにメッセージを出力
int SetMessage(char *mes)
{
	fprintf(stderr,"%s\n",mes);

	return 0;
}


// 新たに登録したBODYの名前をコマンドプロンプトに表示する
void SetBodyNameToWin(char *name)
{
	fprintf(stderr,"%s\n",name);
}
