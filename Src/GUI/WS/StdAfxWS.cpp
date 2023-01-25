#include "StdAfxWS.h"

GUI_Interface GuiIF;		// クラスGUI_Interfaceのインスタンスをグローバルで宣言

// コンソールにメッセージを出力
int GUI_Interface::SetMessage(char *mes)
{
	fprintf(stderr,"%s\n",mes);						// メッセージを標準出力
	Console_Form->addString(mes);					// メッセージを出力
	Console_Form->addString("\n>");					// 次のコマンド位置へ移動
	int len = Console_Form->getString().getChars();	// 文字数カウント
	Console_Form->setProperty(WSNuserValue,len);	// 現在までの文字数を保持
	return len;
}

// コンソールにコマンドを出力
int GUI_Interface::SetCommand(char *cmd)
{
	Console_Form->addString(cmd);					// メッセージを出力
	int len = Console_Form->getString().getChars();	// 文字数カウント
	Console_Form->setProperty(WSNcursorPos,len);	// カーソルを最後にもってくる
	return len;
}

// 新たに登録したBODYの名前をウィンドウリストに表示する
void GUI_Interface::AddBodyNameToWin(const char *name)
{
	char* mes = const_cast<char*>(name);
	Body_List_Win->addItem(mes);					// BodyリストウィンドウにBODY名を登録
}

// 指定された番号のBODYをリストから削除する
void GUI_Interface::DelBodyFromWin(int pos)
{
	Body_List_Win->delPos(pos);
}

// ファイルOPEN用オブジェクトの状態設定
void GUI_Interface::SetEnableOpenFile(bool flag)
{
	Open_Body_Btn->setSensitive(flag);
}

// BODY削除用オブジェクトの状態設定
void GUI_Interface::SetEnableDelBody(bool flag)
{
	Del_Body_Btn->setSensitive(flag);
}

// 描画用GUIの再描画指令
void GUI_Interface::UpdateDescribeWidget()
{
	Describe_Form->execEventProc(WSEV_EXPOSE,NULL);
}

// 描画領域の再描画指令
void GUI_Interface::RedrawDescribeForm()
{
	Describe_Form->redraw();
}

// UserFunc1のラベルをセット
void GUI_Interface::SetUserFunc1Label(const char *label)
{
	User_Func_Btn1->setProperty(WSNlabelString,label);
}

// UserFunc2のラベルをセット
void GUI_Interface::SetUserFunc2Label(const char *label)
{
	User_Func_Btn2->setProperty(WSNlabelString,label);
}

// UserFunc3のラベルをセット
void GUI_Interface::SetUserFunc3Label(const char *label)
{
	User_Func_Btn3->setProperty(WSNlabelString,label);
}

// UserFunc4のラベルをセット
void GUI_Interface::SetUserFunc4Label(const char *label)
{    
	User_Func_Btn4->setProperty(WSNlabelString,label);
}

// UserFunc5のラベルをセット
void GUI_Interface::SetUserFunc5Label(const char *label)
{    
	User_Func_Btn5->setProperty(WSNlabelString,label);
}

// UserFunc6のラベルをセット
void GUI_Interface::SetUserFunc6Label(const char *label)
{    
	User_Func_Btn6->setProperty(WSNlabelString,label);
}

// UserFunc7のラベルをセット
void GUI_Interface::SetUserFunc7Label(const char *label)
{    
	User_Func_Btn7->setProperty(WSNlabelString,label);
}

// UserFunc8のラベルをセット
void GUI_Interface::SetUserFunc8Label(const char *label)
{    
	User_Func_Btn8->setProperty(WSNlabelString,label);
}

// ファイルオープンダイアログの表示（BODY）
// return ファイルのフルパス
std::string GUI_Interface::OpenBodyFileDlg()
{
	WSCfileSelect *fs=WSGIappFileSelect();			// ファイル選択ダイアログのアドレスを取得

	fs->setProperty(WSNmaskFileName,"IGS,igs,STL,stl,WRL,wrl,dxf,DXF");		// IGS,STL,VRML,DXFファイルのみ選択可

	fs->setProperty(WSNx,300);						// ダイアログ表示位置
	fs->setProperty(WSNy,320);

	long fchoice_dlg = fs->popup();					// ファイル選択ダイアログを開く
	if(fchoice_dlg == WS_DIALOG_OK){
		WSCstring full_name = fs->getFileName();	// ファイルのフルパス取得
		std::string s= std::string(full_name);
		return s;
	}
	
	return "";
}

// ファイルオープンダイアログの表示（INP）
//return ファイルのフルパス
std::string GUI_Interface::OpenInpFileDlg()
{
	WSCfileSelect *fs=WSGIappFileSelect();			// ファイル選択ダイアログのアドレスを取得

	fs->setProperty(WSNmaskFileName,"INP,inp");		// inpファイルのみ選択可

	long fchoice_dlg = fs->popup();					// ファイル選択ダイアログを開く

	if(fchoice_dlg == WS_DIALOG_OK){				// ダイアログのOKボタンが押されたら
		WSCstring full_name = fs->getFileName();	// ファイルのフルパス取得
		InpFileName->setProperty(WSNlabelString,full_name);	// ファイルのフルパスをユーザー設定文字列にセット
		std::string s= std::string(full_name);
		return s;
	}
	return "";
}

// フルパスからファイル名抽出
std::string GUI_Interface::GetFileName(const std::string fullPath)
{
	WSCstring full_name = fullPath.c_str();
	int priodnum = full_name.getWords("/");				// ファイル名に含まれているスラッシュの数を調べる
	WSCstring fname = full_name.getWord(priodnum-1,"/");	// ファイル名を取得
	std::string s= std::string(fname);
	return s;
}

// フルパスからファイルの拡張子抽出
std::string GUI_Interface::GetFileExt(const std::string fullPath)
{
	WSCstring full_name = fullPath.c_str();
	int priodnum = full_name.getWords(".");				// ファイル名に含まれているスラッシュの数を調べる
	WSCstring ext = full_name.getWord(priodnum-1,".");	// ファイル名を取得
	std::string s= std::string(ext);
	return s;
}

// コンソールから文字列を取得
std::string GUI_Interface::GetConsoleText()
{
	std::string mes = std::string(Console_Form->getString());
    return mes;
}

// BODYリストの選択されている番号を取得する
int GUI_Interface::GetBodyListSelectedPos()
{
    return Body_List_Win->getSelectedPos();
}

// Nurbs曲線ダイアログ表示
void GUI_Interface::ShowNurbsCurveDlg()
{
	NurbsCurve_Dlg->setVisible(True);
}

// Nurbs曲面ダイアログ表示
void GUI_Interface::ShowNurbsSurfDlg()
{ 
	NurbsSurface_Dlg->setVisible(True);
}

// 回転サーフェスダイアログ表示
void GUI_Interface::ShowRotSurfDlg()
{
	RotSurf_Dlg->setVisible(True);		// 回転サーフェス生成ダイアログを表示
}

// スイープサーフェスダイアログ表示
void GUI_Interface::ShowSweepSurfDlg()
{
	SweepSurf_Dlg->setVisible(True);	// スイープサーフェス生成ダイアログを表示
}

