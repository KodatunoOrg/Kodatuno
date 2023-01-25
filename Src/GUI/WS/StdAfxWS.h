#ifndef _STD_AFX_WS_H_
#define _STD_AFX_WS_H_

#include <WSCfileSelect.h>
#include <WSCmessageDialog.h>
#include <WSCimageSet.h>
#include <WSDkeyboard.h>
#include <WSDmwindowDev.h>
#include <WSDmouse.h>
#include <WSCcolorSet.h>
#include "Kodatuno_Win.h"

#include <string.h>
#include <iostream>

// Class: GUI_Interface
// GUIインターフェースに依存したラップ関数集
// Wide Studioバージョン
class GUI_Interface
{
public:
	// Function: SetMessage
	// コンソールにメッセージを出力
	int SetMessage(char *);

	// Function: SetCommand
	// コンソールにコマンド出力
	int SetCommand(char *);

	// Function: ExecCommand
	// コマンド実行
	void ExecCommand(char *);

	// Function: AddBodyNameToWin
	// 新たに登録したBODYの名前をウィンドウリストに表示する
	void AddBodyNameToWin(const char *);

	// Function: DelBodyFromWin
	// 指定された番号のBODYをリストから削除する
	void DelBodyFromWin(int pos);

	// Function: SetEnableOpenFile
	// ファイルOPEN用オブジェクトの状態設定
    void SetEnableOpenFile(bool );
	
	// Function: SetEnableDelBody
	// BODY削除用オブジェクトの状態設定
	void SetEnableDelBody(bool );
	
	// Function: UpdateDescribeWidget
	// 描画用GUIの再描画指令
	void UpdateDescribeWidget();

	// Function: RedrawDescribeForm
	// 描画領域の再描画指令
	void RedrawDescribeForm();

	// Function: SetUserFunc1Label
	// UserFunc1のラベルをセット
    void SetUserFunc1Label(const char *);

	// Function: SetUserFunc2Label
	// UserFunc2のラベルをセット
    void SetUserFunc2Label(const char *);

	// Function: SetUserFunc3Label
	// UserFunc3のラベルをセット
    void SetUserFunc3Label(const char *);

	// Function: SetUserFunc4Label
	// UserFunc4のラベルをセット
    void SetUserFunc4Label(const char *);

	// Function: SetUserFunc5Label
	// UserFunc5のラベルをセット
    void SetUserFunc5Label(const char *);

	// Function: SetUserFunc6Label
	// UserFunc6のラベルをセット
    void SetUserFunc6Label(const char *);

	// Function: SetUserFunc7Label
	// UserFunc7のラベルをセット
    void SetUserFunc7Label(const char *);

	// Function: SetUserFunc8Label
	// UserFunc8のラベルをセット
    void SetUserFunc8Label(const char *);
  
	// Function: OpenBodyFileDlg
	// ファイルオープンダイヤログの表示（BODY）
    std::string OpenBodyFileDlg();

	// Function: OpenInpFileDlg
	// ファイルオープンダイヤログの表示（INP）
    std::string OpenInpFileDlg();

	// Function: GetFileName
	// フルパスからファイル名抽出
    std::string GetFileName(const std::string fullPath);

	// Function: GetFileExt
	// フルパスからファイルの拡張子抽出
    std::string GetFileExt(const std::string fullPath);

	// Function: GetConsoleText
	// コンソールから文字列を取得
    std::string GetConsoleText();

	// Function: GetBodyListSelectedPos
	// BODYリストの選択されている番号を取得する
    int GetBodyListSelectedPos();			// BODYリストの選択されている番号を取得する

	// Function: ShowNurbsCurveDlg
	// NURBS曲線ダイヤログ表示
    void ShowNurbsCurveDlg();

	// Function: ShowNurbsSurfDlg
	// NURBS曲面ダイヤログ表示
    void ShowNurbsSurfDlg();

	// Function: ShowRotSurfDlg
	// 回転サーフェスダイヤログ表示
    void ShowRotSurfDlg();

	// Function: ShowSweepSurfDlg
	// スイープサーフェスダイヤログ表示
    void ShowSweepSurfDlg();

};

// Variable: GuiIF
// クラスGUI_Interfaceのインスタンスをグローバルで宣言
extern GUI_Interface GuiIF;

#endif