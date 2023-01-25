#ifndef _STD_AFX_QT_H_
#define _STD_AFX_QT_H_

//GEで使用するQt関連のヘッダ
#include <QTextEdit>
#include <QListWidget>
#include <QListWidgetItem>
#include <QFileDialog>
#include <QLabel>
#include <QAction>
#include <QGLWidget>
#include <QStatusBar>
#include <QScrollBar>
#include <QTextCursor>
#include <QObject>
#include "QtNurbsCurveDialog.h"
#include "QtNurbsSurfDialog.h"
#include "QtRotSurfDialog.h"
#include "QtSweepSurfDialog.h"
#include "QtGui"

// Constants: GeneralDefines
// Non

// Class: GUI_Interface
class GUI_Interface
{
public:
// Function: SetMesage
// コンソールにメッセージを出力
    int SetMessage(const char *);				
	
// Function: SetCommand
// コンソールにコマンドを出力
    int SetCommand( const char *);              
	
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
	
// Function: UserDescribeWidget
// 描画用GUIの再描画指令
    void UpdateDescribeWidget();				
	
// Function: RedrawDescribeForm
// 描画領域の再描画指令
    void RedrawDescribeForm();                  

// Function: SetInpFileName
// INPファイル名を表示するオブジェクトにファイル名を設定
    void SetInpFileName(const char *);          

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
	// ファイルオープンダイアログの表示(BODY)
    std::string OpenBodyFileDlg();                          
	
// Function: OpenInpFileDlg
	// ファイルオープンダイアログの表示(INP)
	std::string OpenInpFileDlg();                           
	
// Function: GetFileName
// フルパスからファイル名抽出
    std::string GetFileName(const std::string fullPath);    
	
// Function: GetFileName
// フルパスからファイルの拡張子抽出
    std::string GetFileExt(const std::string fullPath);		
	
// Function: GetConsoleText
// コンソールから文字列を取得
    std::string GetConsoleText();				
	
// Function: GetBodyListSelectedPos
// BODYリストの選択されている番号を取得する
    int GetBodyListSelectedPos();				
	
// Function: SetConsoleObject
// コンソール用GUIのオブジェクトのインスタンスをセット
    void SetConsoleObject(QTextEdit* textEdit);     
	
// Function: SetBodyListObject
// BODYリスト用GUIのオブジェクトのインスタンスをセット
    void SetBodyListObject(QListWidget* list);      
	
// Function: SetOpenFileActionObject
// ファイルオープン用GUIのオブジェクトのインスタンスをセット
    void SetOpenFileActionObject(QAction* action);  
	
// Function: SetDelBodyActionObject
// BODY削除用GUIのオブジェクトのインスタンスをセット
    void SetDelBodyActionObject(QAction* action);	
	
// Function: SetDescribeWidget
// OpenGl描画用GUIのオブジェクトのインスタンスをセット
    void SetDescribeWidget(QGLWidget* widget);		
	
// Function: SetStatusBarObject
// ステータスバー用GUIのオブジェクトのインスタンスをセット
    void SetStatusBarObject(QStatusBar* bar);       
	
// Function: SetUserFunc1Action
// UserFunc1用GUIのオブジェクトのインスタンスをセット
    void SetUserFunc1Action(QAction* action);       
	
// Function: SetUserFunc2Action
// UserFunc2用GUIのオブジェクトのインスタンスをセット
    void SetUserFunc2Action(QAction* action);       
	
// Function: SetUserFunc3Action
// UserFunc3用GUIのオブジェクトのインスタンスをセット
    void SetUserFunc3Action(QAction* action);       
	
// Function: SetUserFunc4Action
// UserFunc4用GUIのオブジェクトのインスタンスをセット
    void SetUserFunc4Action(QAction* action);       
	
// Function: SetUserFunc5Action
// UserFunc5用GUIのオブジェクトのインスタンスをセット
    void SetUserFunc5Action(QAction* action);       
	
// Function: SetUserFunc6Action
// UserFunc6用GUIのオブジェクトのインスタンスをセット
    void SetUserFunc6Action(QAction* action);       
	
// Function: SetUserFunc7Action
// UserFunc7用GUIのオブジェクトのインスタンスをセット
    void SetUserFunc7Action(QAction* action);       
	
// Function: SetUserFunc8Action
// UserFunc8用GUIのオブジェクトのインスタンスをセット
    void SetUserFunc8Action(QAction* action);       
	
// Function: ShowNurbsCurveDlg
// Nurbs曲線ダイアログ表示
    void ShowNurbsCurveDlg();     
	
// Function: ShowNurbsSurfDlg
// Nurbs曲面ダイアログ表示
    void ShowNurbsSurfDlg();      
	
// Function: ShowRotSurfDlg
// 回転サーフェスダイアログ表示
    void ShowRotSurfDlg();        
	
// Function: ShowSweepSurfDlg
// スイープサーフェスダイアログ表示
    void ShowSweepSurfDlg();      

private:
// Variable: *consoleText
// コンソール用GUIオブジェクト
    QTextEdit* consoleText;         
	
// Variable: *bodyList
// BODYリスト用GUIオブジェクト
    QListWidget* bodyList;      	
	
// Variable: *openFileAction
// ファイルオープン用GUIオブジェクト
    QAction* openFileAction;     	
	
// Variable: *delBodyAction
// BODY削除用GUIオブジェクト
    QAction* delBodyAction;   		

// Variable: *glWidget
// OpenGL描画用GUIオブジェクト
    QGLWidget* glWidget;            
	
// Variable: *statusBar
// ステータスバー用GUIのオブジェクト
    QStatusBar* statusBar;      	

// Variable: *userFunc1
// UserFunc1
    QAction* userFunc1;   	
	
// Variable: *userFunc2
// UserFunc2
    QAction* userFunc2;   	
	
// Variable: *userFunc3
// UserFunc3
    QAction* userFunc3;   	
	
// Variable: *userFunc4
// UserFunc4
    QAction* userFunc4;   	
	
// Variable: *userFunc5
// UserFunc5
    QAction* userFunc5;   	
	
// Variable: *userFunc6
// UserFunc6
    QAction* userFunc6;   	
	
// Variable: *userFunc7
// UserFunc7
    QAction* userFunc7;   	
	
// Variable: *userFunc8
// UserFunc8
    QAction* userFunc8;   	

};

// Variable: GuiIF
// クラスGUI_Interfaceのインスタンスをグローバルで宣言
extern GUI_Interface GuiIF;

#endif
