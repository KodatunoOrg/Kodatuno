#include "StdAfxWS.h"

GUI_Interface GuiIF;		// クラスGUI_Interfaceのインスタンスをグローバルで宣言

// Function: SetMessage
// コンソールにメッセージを出力
//
// Parameters: 
// *mes - メッセージ文字列を格納
// Return: 
// メッセージの文字数lenを返す
int GUI_Interface::SetMessage(char *mes)
{
	fprintf(stderr,"%s\n",mes);						// メッセージを標準出力
	Console_Form->addString(mes);					// メッセージを出力
	Console_Form->addString("\n>");					// 次のコマンド位置へ移動
	int len = Console_Form->getString().getChars();	// 文字数カウント
	Console_Form->setProperty(WSNuserValue,len);	// 現在までの文字数を保持
	return len;
}

// Function: SetCommand
// コンソールにコマンドを出力
//
// Parameters: 
// *cmd - コマンド文字列を格納
// Return: 
// コマンドのの文字数lenを返す
int GUI_Interface::SetCommand(char *cmd)
{
	Console_Form->addString(cmd);					// メッセージを出力
	int len = Console_Form->getString().getChars();	// 文字数カウント
	Console_Form->setProperty(WSNcursorPos,len);	// カーソルを最後にもってくる
	return len;
}

// Function: AddBodyNameToWin
// 新たに登録したBODYの名前をウィンドウリストに表示する
//
// Parameters: 
// *name - BODY名の文字列を格納
void GUI_Interface::AddBodyNameToWin(const char *name)
{
	char* mes = const_cast<char*>(name);
	Body_List_Win->addItem(mes);					// BodyリストウィンドウにBODY名を登録
}

// Function: DelBodyFromWin
// 指定された番号のBODYをリストから削除する
//
// Parameters: 
// pos - 
// Return: 
// メッセージの文字数lenを返す
// 指定された番号のBODYをリストから削除する
void GUI_Interface::DelBodyFromWin(int pos)
{
	Body_List_Win->delPos(pos);
}

// Function: SetEnableOpenFile
// ファイルOPEN用オブジェクトの状態設定
//
// Parameters: 
// flag - 
void GUI_Interface::SetEnableOpenFile(bool flag)
{
	Open_Body_Btn->setSensitive(flag);
}

// Function: SetEnableDelBody
// BODY削除用オブジェクトの状態設定
//
// Parameters: 
// flag - 
void GUI_Interface::SetEnableDelBody(bool flag)
{
	Del_Body_Btn->setSensitive(flag);
}

// Function: UpdateDescribeWidget
// 描画用GUIの再描画指令
void GUI_Interface::UpdateDescribeWidget()
{
	Describe_Form->execEventProc(WSEV_EXPOSE,NULL);
}

// Function: RedrawDescribeForm
// 描画領域の再描画指令
void GUI_Interface::RedrawDescribeForm()
{
	Describe_Form->redraw();
}
// Function: SetUserFunc1Label
// UserFunc1のラベルをセット
//
// Parameters: 
// *label - labelの文字列の格納
void GUI_Interface::SetUserFunc1Label(const char *label)
{
	User_Func_Btn1->setProperty(WSNlabelString,label);
}

// Function: SetUserFunc2Label
// UserFunc2のラベルをセット
//
// Parameters: 
// *label - labelの文字列の格納
void GUI_Interface::SetUserFunc2Label(const char *label)
{
	User_Func_Btn2->setProperty(WSNlabelString,label);
}

// Function: SetUserFunc3Label
// UserFunc3のラベルをセット
//
// Parameters: 
// *label - labelの文字列の格納
void GUI_Interface::SetUserFunc3Label(const char *label)
{
	User_Func_Btn3->setProperty(WSNlabelString,label);
}

// Function: SetUserFunc4Label
// UserFunc4のラベルをセット
//
// Parameters: 
// *label - labelの文字列の格納
void GUI_Interface::SetUserFunc4Label(const char *label)
{    
	User_Func_Btn4->setProperty(WSNlabelString,label);
}

// Function: SetUserFunc5Label
// UserFunc5のラベルをセット
//
// Parameters: 
// *label - labelの文字列の格納
void GUI_Interface::SetUserFunc5Label(const char *label)
{    
	User_Func_Btn5->setProperty(WSNlabelString,label);
}

// Function: SetUserFunc6Label
// UserFunc6のラベルをセット
//
// Parameters: 
// *label - labelの文字列の格納
void GUI_Interface::SetUserFunc6Label(const char *label)
{    
	User_Func_Btn6->setProperty(WSNlabelString,label);
}

// Function: SetUserFunc7Label
// UserFunc7のラベルをセット
//
// Parameters: 
// *label - labelの文字列の格納
void GUI_Interface::SetUserFunc7Label(const char *label)
{    
	User_Func_Btn7->setProperty(WSNlabelString,label);
}

// Function: SetUserFunc8Label
// UserFunc8のラベルをセット
//
// Parameters: 
// *label - labelの文字列の格納
void GUI_Interface::SetUserFunc8Label(const char *label)
{    
	User_Func_Btn8->setProperty(WSNlabelString,label);
}
// Function: OpenBodyFileDlg
// ファイルオープンダイアログの表示（BODY）
//
// Return: 
// ファイルのフルパス
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

// Function: OpenInpFileDlg
// ファイルオープンダイアログの表示（INP）
//
// Return: 
// ファイルのフルパス
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

// Function: GetFileName
// フルパスからファイル名抽出
//
// Parameters: 
// fullPath - フルパス
//
// Return: 
// ファイル名
std::string GUI_Interface::GetFileName(const std::string fullPath)
{
	WSCstring full_name = fullPath.c_str();
	int priodnum = full_name.getWords("/");				// ファイル名に含まれているスラッシュの数を調べる
	WSCstring fname = full_name.getWord(priodnum-1,"/");	// ファイル名を取得
	std::string s= std::string(fname);
	return s;
}

// Function: GetFileExt
// フルパスからファイルの拡張子抽出
//
// Parameters: 
// fullPath - フルパス
//
// Return: 
// ファイルの拡張子
std::string GUI_Interface::GetFileExt(const std::string fullPath)
{
	WSCstring full_name = fullPath.c_str();
	int priodnum = full_name.getWords(".");				// ファイル名に含まれているスラッシュの数を調べる
	WSCstring ext = full_name.getWord(priodnum-1,".");	// ファイル名を取得
	std::string s= std::string(ext);
	return s;
}

// Function: GetConsoleText
// コンソールから文字列を取得
//
// Return: 
// 取得した文字列
std::string GUI_Interface::GetConsoleText()
{
	std::string mes = std::string(Console_Form->getString());
    return mes;
}

// Function: GetBodyListSelectedPos
// BODYリストの選択されている番号を取得する
int GUI_Interface::GetBodyListSelectedPos()
{
    return Body_List_Win->getSelectedPos();
}

// Function: ShowNurbsCurveDlg
// Nurbs曲線ダイアログ表示
void GUI_Interface::ShowNurbsCurveDlg()
{
	NurbsCurve_Dlg->setVisible(True);
}

// Function: ShowNurbsSurfDlg
// Nurbs曲面ダイアログ表示
void GUI_Interface::ShowNurbsSurfDlg()
{ 
	NurbsSurface_Dlg->setVisible(True);
}

// Function: ShowRotSurfDlg
// 回転サーフェスダイアログ表示
void GUI_Interface::ShowRotSurfDlg()
{
	RotSurf_Dlg->setVisible(True);		// 回転サーフェス生成ダイアログを表示
}

// Function: ShowSweepSurfDlg
// スイープサーフェスダイアログ表示
void GUI_Interface::ShowSweepSurfDlg()
{
	SweepSurf_Dlg->setVisible(True);	// スイープサーフェス生成ダイアログを表示
}

