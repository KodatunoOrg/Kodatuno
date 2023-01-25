#ifndef QTMAINWINDOW_H
#define QTMAINWINDOW_H

// Kodatunoメインウィンドウ

#include <QMainWindow>
#include <QFileDialog>
#include <QSettings>
#include <QMessageBox>

#include "Kodatuno.h"
#include "QtUserStatDialog.h"

// Constants: General Defines
// Non

// Namespace: Ui
// QtMainWindowクラスを名前空間Uiで定義
namespace Ui {
    class QtMainWindow;
}

// Class: QtMainWindow
// Kodatunoメインウィンドウクラス
class QtMainWindow : public QMainWindow
{
// Macro: Q_OBJECT
// シグナルとスロットを定義するために必要
    Q_OBJECT

public:
// Constructor: QtMainWindow
// QtMainWindowクラスのコンストラクタ．暗黙呼び出しを禁止
    explicit QtMainWindow(QWidget *parent = 0);
	
// Destructor: ~QtMainWindow
// GUIの終了
    ~QtMainWindow();

private:
// Variable: *ui
// Kodatunoメインウィンドウ
    Ui::QtMainWindow *ui;

// Variable: *setting
// QSettingsオブジェクト
    QSettings *setting;

// Function: initGUI
// GUIの初期化
    void initGUI();			


private slots:
	
// Function: om_actionViewOnX_triggered
// (private)X方向ビュー
    void on_actionViewOnX_triggered();         
	
// on_actionViewOnY_triggered
// (private)Y方向ビュー
    void on_actionViewOnY_triggered();          
	
// Function: on_actionViewOnZ_triggered
// (private)Z方向ビュー
    void on_actionViewOnZ_triggered();          
	
// Function: on_actionViewOnXYZ_triggered
// (private)鳥瞰ビュー
    void on_actionViewOnXYZ_triggered();        
	
// Function: on_actionViewFit_triggered
// (private)倍率初期化
    void on_actionViewFit_triggered();          
	
// Function: on_actionViewSolid_triggered
// (private)ソリッド表示
    void on_actionViewSolid_triggered();        
	
// Fnction: on_actionViewNoEdge_triggered
// (private)エッジ非表示
    void on_actionViewNoEdge_triggered();       
	
// on_actionViewWireFlame_triggered
// (private)ワイヤーフレーム
    void on_actionViewWireFlame_triggered();    
	
// Function: on_actionViewVoid_triggered
// (private)表示しない
    void on_actionViewVoid_triggered();         
	
// Function: on_actionSelectAll_triggered
// (private)すべて選択
    void on_actionSelectAll_triggered();        
	
// Function: on_actionSelect_Cancel_triggered
// (private)選択解除
    void on_actionSelect_Cancel_triggered();    
	
// Function: on_actionViewErase_triggered
// (private)UserFunc()によって描画されたものを消す
    void on_actionViewErase_triggered();        
	
// Function: on_actionViewBlend_triggered
// (private)BODYを半透明
    void on_actionViewBlend_triggered();        

// Function: on_bodyList_itemClicked
// (private)VODYリストでBODYを選択
    void on_bodyList_itemClicked(QListWidgetItem *item);    
	
// Function: on_actionDeleteBody_triggered
// (private)BODYの消去
    void on_actionDeleteBody_triggered();       
	
// Function: on_actionOpenBody_triggered
// BODYファイルのオープン
    void on_actionOpenBody_triggered();         
	
// Function: on_actionNURBSCurve_triggered
// NURBS曲線生成
    void on_actionNURBSCurve_triggered();       
	
// Function: on_actionSweepSurface_triggered
// スイープサーフェス生成
	void on_actionSweepSurface_triggered();     
	
// Function: on_actionRotationSurface_triggered
// 回転サーフェス生成
    void on_actionRotationSurface_triggered();  

// Function: on_actionShowBodyList_triggered
// BODYListのDOCK表示
    void on_actionShowBodyList_triggered();     
	
// Function: on_actionShowConsole_triggered
// ConsoleのDOCK表示
	void on_actionShowConsole_triggered();      

// Function: on_actionUserStatus_triggered
// ユーザーステータスダイアログの表示
    void on_actionUserStatus_triggered();       
	
// Function: on_actionUserFunc1_triggered
// ユーザー関数1の実行
    void on_actionUserFunc1_triggered();        
	
// Function: on_actionUserFunc2_triggered
// ユーザー関数2の実行
    void on_actionUserFunc2_triggered();        
	
// Function: on_actionUserFunc3_triggered
// ユーザー関数3の実行
	void on_actionUserFunc3_triggered();        
	
// Function: on_actionUserFunc4_triggered
// ユーザー関数4の実行
    void on_actionUserFunc4_triggered();        

// Function: on_actionUserFunc5_triggered
// ユーザー関数5の実行
    void on_actionUserFunc5_triggered();        
	
// Function: on_actionUserFunc6_triggered
// ユーザー関数6の実行
    void on_actionUserFunc6_triggered();        
	
// Function: on_actionUserFunc7_triggered
// ユーザー関数7の実行
    void on_actionUserFunc7_triggered();        
	
// Function: on_actionUserFunc8_triggered
// ユーザー関数8の実行
    void on_actionUserFunc8_triggered();        

// FUnction: on_actionAboutKodatuno_triggered
// AboutBOXの表示
    void on_actionAboutKodatuno_triggered();    

// Function: on_actionNURBSSurface_triggered
// NURBS曲面生成
    void on_actionNURBSSarface_triggered();

protected:
// Function: closeEvent
// KodatunoGUIクローズ時のイベント. 仮想関数で宣言
    virtual void closeEvent(QCloseEvent *event);
};

#endif // QTMAINWINDOW_H
