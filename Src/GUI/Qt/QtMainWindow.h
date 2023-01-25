#ifndef QTMAINWINDOW_H
#define QTMAINWINDOW_H

// Kodatunoメインウィンドウ

#include <QMainWindow>
#include <QFileDialog>
#include <QSettings>
#include <QMessageBox>

#include "Kodatuno.h"
#include "QtUserStatDialog.h"

namespace Ui {
    class QtMainWindow;
}

// Kodatunoメインウィンドウクラス
class QtMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit QtMainWindow(QWidget *parent = 0);
    ~QtMainWindow();

private:
    Ui::QtMainWindow *ui;
    QSettings *setting;

    void initGUI();			// Initializing GUI

// スロット関数
private slots:
    // ボタン操作イベント
    // BODYの表示関連
    void on_actionViewOnX_triggered();          // X方向ビュー
    void on_actionViewOnY_triggered();          // Y方向ビュー
    void on_actionViewOnZ_triggered();          // Z方向ビュー
    void on_actionViewOnXYZ_triggered();        // 鳥瞰ビュー
    void on_actionViewFit_triggered();          // 倍率初期化
    void on_actionViewSolid_triggered();        // ソリッド表示
    void on_actionViewNoEdge_triggered();       // エッジ非表示
    void on_actionViewWireFlame_triggered();    // ワイヤーフレーム
    void on_actionViewVoid_triggered();         // 表示しない
    void on_actionSelectAll_triggered();        // すべて選択
    void on_actionSelect_Cancel_triggered();    // 選択解除
    void on_actionViewErase_triggered();        // Userfunc()によって描画されたものを消す
    void on_actionViewBlend_triggered();        // BODYを半透明

    // BODYの操作
    void on_bodyList_itemClicked(QListWidgetItem *item);    // BODYリストでBODYを選択
    void on_actionDeleteBody_triggered();       // BODYの消去
    void on_actionOpenBody_triggered();         // BODYファイルのオープン

    // モデル作成関連
    void on_actionNURBSSarface_triggered();     // NURBS曲面生成
    void on_actionNURBSCurve_triggered();       // NURBS曲線生成
    void on_actionSweepSurface_triggered();     // スイープサーフェス生成
    void on_actionRotationSurface_triggered();  // 回転サーフェス生成

    // DOCKの表示ON
    void on_actionShowBodyList_triggered();     // BODY ListのDOCK表示
    void on_actionShowConsole_triggered();      // ConsoleのDOCK表示

    // UserFunc関連
    void on_actionUserStatus_triggered();       // ユーザーステータスダイアログの表示
    void on_actionUserFunc1_triggered();        // ユーザー関数1の実行
    void on_actionUserFunc2_triggered();        // ユーザー関数2の実行
    void on_actionUserFunc3_triggered();        // ユーザー関数3の実行
    void on_actionUserFunc4_triggered();        // ユーザー関数4の実行
    void on_actionUserFunc5_triggered();        // ユーザー関数5の実行
    void on_actionUserFunc6_triggered();        // ユーザー関数6の実行
    void on_actionUserFunc7_triggered();        // ユーザー関数7の実行
    void on_actionUserFunc8_triggered();        // ユーザー関数8の実行

    void on_actionAboutKodatuno_triggered();    // AboutBOXの表示

protected:
    virtual void closeEvent(QCloseEvent *event);
};

#endif // QTMAINWINDOW_H
