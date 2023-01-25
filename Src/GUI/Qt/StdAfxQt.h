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


// GUIインターフェースに依存したラップ関数集
// Qtバージョン
class GUI_Interface
{
public:
    int SetMessage(const char *);				// コンソールにメッセージを出力
    int SetCommand( const char *);              // コンソールにコマンドを出力
    void AddBodyNameToWin(const char *);        // 新たに登録したBODYの名前をウィンドウリストに表示する
    void DelBodyFromWin(int pos);				// 指定された番号のBODYをリストから削除する
    void SetEnableOpenFile(bool );              // ファイルOPEN用オブジェクトの状態設定
    void SetEnableDelBody(bool );               // BODY削除用オブジェクトの状態設定
    void UpdateDescribeWidget();				// 描画用GUIの再描画指令
    void RedrawDescribeForm();                  // 描画領域の再描画指令

    void SetInpFileName(const char *);          // INPファイル名を表示するオブジェクトにファイル名を設定

    void SetUserFunc1Label(const char *);       // UserFunc1のラベルをセット
    void SetUserFunc2Label(const char *);       // UserFunc2のラベルをセット
    void SetUserFunc3Label(const char *);       // UserFunc3のラベルをセット
    void SetUserFunc4Label(const char *);       // UserFunc4のラベルをセット
    void SetUserFunc5Label(const char *);       // UserFunc5のラベルをセット
    void SetUserFunc6Label(const char *);       // UserFunc6のラベルをセット
    void SetUserFunc7Label(const char *);       // UserFunc7のラベルをセット
    void SetUserFunc8Label(const char *);       // UserFunc8のラベルをセット

    std::string OpenBodyFileDlg();                          // ファイルオープンダイアログの表示（BODY）
    std::string OpenInpFileDlg();                           // ファイルオープンダイアログの表示（INP）
    std::string GetFileName(const std::string fullPath);    // フルパスからファイル名抽出
    std::string GetFileExt(const std::string fullPath);		// フルパスからファイルの拡張子抽出

    std::string GetConsoleText();				// コンソールから文字列を取得
    int GetBodyListSelectedPos();				// BODYリストの選択されている番号を取得する

    void SetConsoleObject(QTextEdit* textEdit);     // コンソール用GUIのオブジェクトのインスタンスをセット
    void SetBodyListObject(QListWidget* list);      // BODYリスト用GUIのオブジェクトのインスタンスをセット
    void SetOpenFileActionObject(QAction* action);  // ファイルオープン用GUIのオブジェクトのインスタンスをセット
    void SetDelBodyActionObject(QAction* action);	// BODY削除用GUIのオブジェクトのインスタンスをセット
    void SetDescribeWidget(QGLWidget* widget);		// OpenGL描画用GUIのオブジェクトのインスタンスをセット
    void SetStatusBarObject(QStatusBar* bar);       // ステータスバー用GUIのオブジェクトのインスタンスをセット

    void SetUserFunc1Action(QAction* action);       // UserFunc1用GUIのオブジェクトのインスタンスをセット
    void SetUserFunc2Action(QAction* action);       // UserFunc2用GUIのオブジェクトのインスタンスをセット
    void SetUserFunc3Action(QAction* action);       // UserFunc3用GUIのオブジェクトのインスタンスをセット
    void SetUserFunc4Action(QAction* action);       // UserFunc4用GUIのオブジェクトのインスタンスをセット
    void SetUserFunc5Action(QAction* action);       // UserFunc5用GUIのオブジェクトのインスタンスをセット
    void SetUserFunc6Action(QAction* action);       // UserFunc6用GUIのオブジェクトのインスタンスをセット
    void SetUserFunc7Action(QAction* action);       // UserFunc7用GUIのオブジェクトのインスタンスをセット
    void SetUserFunc8Action(QAction* action);       // UserFunc8用GUIのオブジェクトのインスタンスをセット

    void ShowNurbsCurveDlg();     // Nurbs曲線ダイアログ表示
    void ShowNurbsSurfDlg();      // Nurbs曲面ダイアログ表示
    void ShowRotSurfDlg();        // 回転サーフェスダイアログ表示
    void ShowSweepSurfDlg();      // スイープサーフェスダイアログ表示

private:
    //GUIオブジェクトのインスタンス保持用（使用するツールキットに応じて宣言するクラスを変更）
    QTextEdit* consoleText;         // コンソール用GUIオブジェクト
    QListWidget* bodyList;      	// BODYリスト用GUIオブジェクト
    QAction* openFileAction;     	// ファイルオープン用GUIオブジェクト
    QAction* delBodyAction;   		// BODY削除用GUIオブジェクト
    QGLWidget* glWidget;            // OpenGL描画用GUIオブジェクト
    QStatusBar* statusBar;      	// ステータスバー用GUIのオブジェクト

    QAction* userFunc1;   	// UserFunc1
    QAction* userFunc2;   	// UserFunc2
    QAction* userFunc3;   	// UserFunc3
    QAction* userFunc4;   	// UserFunc4
    QAction* userFunc5;   	// UserFunc5
    QAction* userFunc6;   	// UserFunc6
    QAction* userFunc7;   	// UserFunc7
    QAction* userFunc8;   	// UserFunc8

};

// クラスGUI_Interfaceのインスタンスをグローバルで宣言
extern GUI_Interface GuiIF;

#endif
