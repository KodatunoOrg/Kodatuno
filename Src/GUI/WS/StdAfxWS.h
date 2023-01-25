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

// GUIインターフェースに依存したラップ関数集
// Wide Studioバージョン
class GUI_Interface
{
public:
	int SetMessage(char *);					// コンソールにメッセージを出力
	int SetCommand(char *);					// コンソールにコマンドを出力
	void AddBodyNameToWin(const char *);	// 新たに登録したBODYの名前をウィンドウリストに表示する
	void DelBodyFromWin(int pos);			// 指定された番号のBODYをリストから削除する
    void SetEnableOpenFile(bool );			// ファイルOPEN用オブジェクトの状態設定
    void SetEnableDelBody(bool );			// BODY削除用オブジェクトの状態設定
    void UpdateDescribeWidget();			// 描画用GUIの再描画指令
	void RedrawDescribeForm();				// 描画領域の再描画指令

    void SetUserFunc1Label(const char *);	// UserFunc1のラベルをセット
    void SetUserFunc2Label(const char *);	// UserFunc2のラベルをセット
    void SetUserFunc3Label(const char *);	// UserFunc3のラベルをセット
    void SetUserFunc4Label(const char *);	// UserFunc4のラベルをセット
    void SetUserFunc5Label(const char *);	// UserFunc5のラベルをセット
    void SetUserFunc6Label(const char *);	// UserFunc6のラベルをセット
    void SetUserFunc7Label(const char *);	// UserFunc7のラベルをセット
    void SetUserFunc8Label(const char *);	// UserFunc8のラベルをセット
  
    std::string OpenBodyFileDlg();			// ファイルオープンダイアログの表示（BODY）
    std::string OpenInpFileDlg();			// ファイルオープンダイアログの表示（INP）
    std::string GetFileName(const std::string fullPath);	// フルパスからファイル名抽出
    std::string GetFileExt(const std::string fullPath);		// フルパスからファイルの拡張子抽出

    std::string GetConsoleText();			// コンソールから文字列を取得
    int GetBodyListSelectedPos();			// BODYリストの選択されている番号を取得する

    void ShowNurbsCurveDlg();				// Nurbs曲線ダイアログ表示
    void ShowNurbsSurfDlg();				// Nurbs曲面ダイアログ表示
    void ShowRotSurfDlg();					// 回転サーフェスダイアログ表示
    void ShowSweepSurfDlg();				// スイープサーフェスダイアログ表示

};

extern GUI_Interface GuiIF;		// クラスGUI_Interfaceのインスタンスをグローバルで宣言

#endif