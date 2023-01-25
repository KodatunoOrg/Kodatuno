#include "Command.h"

// コマンド追加
// 引数 *CmdMap:コマンド情報を格納する構造体へのポインタ,  *name:コマンド名,  *op:コマンドオプション名
//      key:コマンドを代表する値(0～99:Kodatunoが使用、100～199:Userに割り当て),  (*Cmd)(int,char *[]):実行されるコマンドの関数へのポインタ
void SetCmdList(CommandMap *CmdMap,char *name,char *op,int key,void (*Cmd)(int,char *[]))
{
	if(key >= COMMANDNUMMAX){
        GuiIF.SetMessage("ERROR:your selected key number is out of the region!");
        GuiIF.SetMessage(" select range from (int)100 to (int)199.");
		return ;
	}

	strcpy(CmdMap[key].Name,name);	// コマンド名取得
	strcpy(CmdMap[key].Op,op);		// コマンドオプション取得
	CmdMap[key].CmdNo = key;		// コマンド代表値取得
	CmdMap[key].Command = Cmd;		// コマンド実行関数取得
}

// コマンド解析
// Qt版ではコマンド部分を取り出した文字列を渡す
int AnalCommandLine(char *str,char *argv[])
{
	int argc=0;
	int len;
	char *p;

	len = strlen(str);						// コマンドラインの文字数をカウント
	len -= 2;
	str[len] = '\0';						// おしりに"\n>"がついているので、消す

	// 入力文字列なし
	if(!strcmp(str,"")){
		return KOD_ERR;
	}

	// コマンドライン検出
	else{
		// strを分解し、argc,argvを得る
		p = str;
			while(((p = strchr(str,' ')) != NULL) || ((p = strchr(str,'>')) != NULL)){
				str[p-str] = '\0';
				argv[argc] = str;
				str = p;
				str++;
				argc++;
			}
			argv[argc] = str;
	}

	return argc;
}

// 引数で指定したコマンドの登録Noを調べる
int SearchRegdCmdNum(CommandMap *CmdMap,char *cmd)
{
	for(int i=0;i<COMMANDNUMMAX;i++){
		if(!strcmp(cmd,CmdMap[i].Name))
			return CmdMap[i].CmdNo;
	}

	return KOD_ERR;
}


