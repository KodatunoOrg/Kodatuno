// テキストフォームのプロシージャ

#include <WScom.h>
#include <WSCfunctionList.h>
#include <WSCbase.h>
#include "Kodatuno.h"

void Console_Expose(WSCbase *);
void Console_KeyHook(WSCbase *);

void Console_Proc(WSCbase* object)
{
	WSCprocedure *pwproc = object->getExecutedProcedure();	// 実行されたプロシージャを取得

	switch(pwproc->getTrigger()){		// 取得したプロシージャが
		case WSEV_KEY_HOOK:				// キーフックの場合
			Console_KeyHook(object);
			break;
		case WSEV_EXPOSE:				// 再描画指令の場合
			Console_Expose(object);
			break;
		default:
			break;
	}
}

// 再描画時の処理
void Console_Expose(WSCbase *object)
{
	WSCstring mes = Console_Form->getString();
	int len = mes.getChars();					// コンソール中の文字数を取得
	Console_Form->setProperty(WSNcursorPos,len);// カーソルを最後にもってくる
	Kodatuno.SetConsoleCharNum(Console_Form->getProperty(WSNuserValue));			// 文字数を更新
}

// 入力されたキーをフックし、キー入力を制限する(コンソール風なテキストフォームにする)
void Console_KeyHook(WSCbase *object)
{
	int oldlen,newlen;		// 文字列長を格納
	int pos;
	WSCstring mes,com;

	Console_Form->setProperty(WSNdisplayOnly,0);// 入力可能状態に移行
	WSDkeyboard *key=WSGIappKeyboard();			// 
	long keycode = key->getKey();				// 入力されたキーを取得

	std::string com_string;
	std::string s; 
	switch(keycode){
		// リターンキーが押されたら、入力されたコマンドを抽出し、コマンド解析する
		case WSK_Return:
			Console_Form->setProperty(WSNdisplayOnly,1);	// 入力不可能状態に移行(こうしないとなぜかカーソルを最後に持っていけない)
			oldlen = Kodatuno.GetConsoleCharNum();			// 最後に登録したコンソール文字列長を取得
			Console_Form->addString("\n>");					// コンソール風に味付け
			mes = Console_Form->getString();				// これまでの文字列を全て取得
			newlen = mes.getChars();						// 文字数をカウント
			if(newlen == oldlen)	break;					// 何も入力されていなかったら、何もしない
			else{											// コマンドが入力されていたら
				Console_Form->setProperty(WSNuserValue,newlen);	// 現在までの文字数を保持
				mes.deleteChars(0,oldlen);					// コマンド部のみ抽出
				Kodatuno.ExecCommand((char *)mes);			// コマンド実行
				mes.deleteChars(mes.getChars()-2,2);
				s = std::string(mes);
				Kodatuno.SetConsoleHistory(s);				// コマンドを履歴に登録
				//Kodatuno.SetConsoleCharNum(newlen);		// コンソール文字列長を更新
			}
			break;
		// 上下キーに対してはコマンド履歴を表示する
		case WSK_KP_Up:
			if(Kodatuno.GetUpperConsoleHistory(&com_string) == KOD_TRUE){	// 新たな履歴コマンドを取得
				com = com_string.c_str();
				newlen = Console_Form->getString().getChars();		// コンソールの文字数を取得
				oldlen = Kodatuno.GetConsoleCharNum();				// 最後に登録したコンソール文字列長を取得
				Console_Form->setSelect(oldlen,newlen-oldlen);		// 履歴コマンドが表示済みの場合、
				Console_Form->deleteSelectedString();				// 消す
				GuiIF.GUI_Interface::SetCommand((char *)com);							// 新しい履歴コマンドをコンソールに出力
			}
			key->setKey(0);
			break;
		case WSK_KP_Down:
			if(Kodatuno.GetLowerConsoleHistory(&com_string) == KOD_TRUE){
				com = com_string.c_str();
				newlen = Console_Form->getString().getChars();
				oldlen = Kodatuno.GetConsoleCharNum();				// 最後に登録したコンソール文字列長を取得
				Console_Form->setSelect(oldlen,newlen-oldlen);		// 履歴コマンドが表示済みの場合、
				Console_Form->deleteSelectedString();				// 消す
				GuiIF.SetCommand((char *)com);							// 新しい履歴コマンドをコンソールに出力
			}
			key->setKey(0);
			break;

		//　左右キーに対しては何もしない
		case WSK_KP_Left:
		case WSK_KP_Right:
			break;
		// カーソル位置が現在の文字数より小さい位置にある場合は
		// BackSpaceとDeleteキーは無効にして、文字を消せないようにする
		case WSK_BackSpace:
			pos = Console_Form->getSelectedPos();		// カーソル位置を取得
			oldlen = Kodatuno.GetConsoleCharNum();
			if(pos <= oldlen)
				key->setKey(0);
			break;
		case WSK_Delete:
			pos = Console_Form->getSelectedPos();		// カーソル位置を取得
			oldlen = Kodatuno.GetConsoleCharNum();
			if(pos < oldlen)
				key->setKey(0);
			break;
		default:
			pos = Console_Form->getSelectedPos();		// カーソル位置を取得
			oldlen = Kodatuno.GetConsoleCharNum();
			if(pos < oldlen)									// カーソル位置が現在の文字数より小さい位置にある場合は
				Console_Form->setProperty(WSNcursorPos,Kodatuno.GetConsoleCharNum());	// カーソルを最後にもってくる
			break;
	}

}

static WSCfunctionRegister  op("Console_Proc",(void*)Console_Proc);


