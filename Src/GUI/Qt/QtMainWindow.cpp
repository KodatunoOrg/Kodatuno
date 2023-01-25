#include "QtMainWindow.h"
#include "ui_QtMainWindow.h"

// Variable: Kodatuno
// KODatUNOクラスのインスタンスをグローバル宣言
KODatUNO Kodatuno;	

// Function: QtMainWindow
// メインウィンドウを生成する
//
// Parameters: 
// *parent - 呼び出し元の親ウィジェットを指定
QtMainWindow::QtMainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::QtMainWindow)
{
    ui->setupUi(this);
    
    // INIファイルインターフェースのインスタンス
    setting = new QSettings("kodatuno.ini", QSettings::IniFormat);

    // GUIの初期化
    initGUI();

    // Kodatunoの初期化
    Kodatuno.InitializeWin(ui->describeWidget->width(), ui->describeWidget->height());

    // INIファイルからコマンド履歴を取得
    QStringList cmdList = setting->value("cmd_history").toStringList();
    if(cmdList.length() != 0){
        for(int i = 0; i < cmdList.length(); i++){
            Kodatuno.SetConsoleHistory(cmdList[i].toStdString());
        }
    }

    // INIファイルからUserStatus情報を取得
    int mode = setting->value("user_stat_mode").toInt();
    double prop[USERPROPNUM];
    QStringList propList = setting->value("user_stat_prop").toStringList();
    if(propList.length() != 0){
        for(int i=0;i<USERPROPNUM;i++){
            prop[i] = propList[i].toDouble();
        }
    }
    Kodatuno.AcceptUserStat(mode,prop);
}

// Function: initGUI
// GUIの初期化
void QtMainWindow::initGUI()
{
    // INIファイルを読み込んで，ウィジェットに反映
    restoreGeometry(setting->value("geometry").toByteArray());
    restoreState(setting->value("dock").toByteArray());
    
    // GUIのイスタンスをインターフェースにセット
    GuiIF.SetConsoleObject(ui->consoleText);            // コンソール用GUIのオブジェクトのインスタンスをセット
    GuiIF.SetBodyListObject(ui->bodyList);              // BODYリスト用GUIのオブジェクトのインスタンスをセット
    GuiIF.SetOpenFileActionObject(ui->actionOpenBody);	// ファイルオープン用GUIのオブジェクトのインスタンスをセット
    GuiIF.SetDelBodyActionObject(ui->actionDeleteBody);	// BODY削除用GUIのオブジェクトのインスタンスをセット
    GuiIF.SetDescribeWidget(ui->describeWidget);        // OpenGL描画用GUIのオブジェクトのインスタンスをセット
    GuiIF.SetStatusBarObject(ui->statusBar);            // ステータスバー用GUIのオブジェクトのインスタンスをセット
    GuiIF.SetUserFunc1Action(ui->actionUserFunc1);      // UserFunc1のラベルをセット
    GuiIF.SetUserFunc2Action(ui->actionUserFunc2);      // UserFunc2のラベルをセット
    GuiIF.SetUserFunc3Action(ui->actionUserFunc3);      // UserFunc3のラベルをセット
    GuiIF.SetUserFunc4Action(ui->actionUserFunc4);      // UserFunc4のラベルをセット
    GuiIF.SetUserFunc5Action(ui->actionUserFunc5);      // UserFunc5のラベルをセット
    GuiIF.SetUserFunc6Action(ui->actionUserFunc6);      // UserFunc6のラベルをセット
    GuiIF.SetUserFunc7Action(ui->actionUserFunc7);      // UserFunc7のラベルをセット
    GuiIF.SetUserFunc8Action(ui->actionUserFunc8);      // UserFunc8のラベルをセット
}

// Function: ~QtMainWindow
// GUIの終了
QtMainWindow::~QtMainWindow()
{
    Kodatuno.DeleteWin();
    delete ui;
}

// Function: closeEvent
// KodatunoGUIクローズ時のイベント
//
// Parameters:
// *event - 実行orキャンセル
void QtMainWindow::closeEvent(QCloseEvent *event)
{
    /* コマンド履歴を取得 */
    int cmdNum = Kodatuno.GetConsoleHistoryNum();
    std::string cmdTemp;
    QStringList cmdList;
    if(cmdNum != 0){
        for(int i = 0; i < cmdNum; i++){
            Kodatuno.GetUpperConsoleHistory(&cmdTemp);
            cmdList.append(QString(cmdTemp.c_str()));
        }
    }

    // UserStatusを取得
    int mode;
    double prop[USERPROPNUM];
    Kodatuno.GetUserStat(&mode,prop);
    QStringList uslist;
    for(int i=0;i<USERPROPNUM;i++)
        uslist.append(QString::number(prop[i]));


    // INIファイルへコマンド履歴とUserStatus情報を書き込み
    setting->setValue("geometry",saveGeometry());
    setting->setValue("dock",saveState());
    setting->setValue("cmd_history",cmdList);
    setting->setValue("user_stat_mode",mode);
    setting->setValue("user_stat_prop",uslist);
    setting->sync();

    // クローズイベント
    QMainWindow::closeEvent(event);
}

// Function: on_actionRotationSurface_triggered
// メニュー：Create->NURBS Rotation
void QtMainWindow::on_actionRotationSurface_triggered()
{
    QtRotSurfDialog* dlg = new QtRotSurfDialog(this);
    dlg->exec();
    ui->describeWidget->repaint();
}

// Function: on_actionSweepSurface_triggered
// メニュー：Create->Sweep
void QtMainWindow::on_actionSweepSurface_triggered()
{
    QtSweepSurfDialog* dlg = new QtSweepSurfDialog (this);
    dlg->exec();
    ui->describeWidget->repaint();
}

// Function: on_actionNURBSCurve_triggered
// メニュー：Create->NURBS Curve
void QtMainWindow::on_actionNURBSCurve_triggered()
{
    QtNurbsCurveDialog* dlg = new QtNurbsCurveDialog(this);
    dlg->exec();
    ui->describeWidget->repaint();
}

// Function: on_actionNURBSSurface_triggered
// メニュー：Create->NURBS Surface
void QtMainWindow::on_actionNURBSSurface_triggered()
{
    QtNurbsSurfDialog* dlg = new QtNurbsSurfDialog (this);
    dlg->exec();
    ui->describeWidget->repaint();
}

// Function: on_actionOpenBody_triggered
// メニュー：File->Open File
void QtMainWindow::on_actionOpenBody_triggered()
{
    Kodatuno.OpenFile();		
    Kodatuno.OpenDelBtn();      
    ui->describeWidget->repaint();
}

// Function: on_actionDeleteBody_triggered
// メニュー：Edit->Delete BODY
void QtMainWindow::on_actionDeleteBody_triggered()
{
    Kodatuno.DeleteBody();  	
    Kodatuno.OpenDelBtn();      
    ui->describeWidget->repaint();
}

// Function: on_actionViewOnX_triggered
// ツールバー：X-View
void QtMainWindow::on_actionViewOnX_triggered()
{
    Kodatuno.ChangeViewX();
    ui->describeWidget->repaint();
}

// Function: on_actionViewOnY_triggered
// ツールバー：Y-View
void QtMainWindow::on_actionViewOnY_triggered()
{
    Kodatuno.ChangeViewY();
    ui->describeWidget->repaint();
}

// Function: on_actionViewOnZ_triggered
// ツールバー：Z-View
void QtMainWindow::on_actionViewOnZ_triggered()
{
    Kodatuno.ChangeViewZ();
    ui->describeWidget->repaint();
}

// Function: on_actionViewOnXYZ_triggered
// ツールバー：アイソメView
void QtMainWindow::on_actionViewOnXYZ_triggered()
{
    Kodatuno.ChangeViewXYZ();
    ui->describeWidget->repaint();
}

// Function: on_actionViewFit_triggered
// ツールバー：画面にフィット
void QtMainWindow::on_actionViewFit_triggered()
{
    Kodatuno.FitView();
    ui->describeWidget->repaint();
}

// Function: on_actionViewSolid_triggered
// ツールバー：ソリッド表示
void QtMainWindow::on_actionViewSolid_triggered()
{
    Kodatuno.SolidView();
    ui->describeWidget->repaint();
}

// Function: on_actionViewNoEdge_triggered
// ツールバー：ソリッド（外形線なし）表示
void QtMainWindow::on_actionViewNoEdge_triggered()
{
    Kodatuno.NoEdgeView();
    ui->describeWidget->repaint();
}

// Function: on_actionViewWireFlame_triggered
// ツールバー：ワイヤーフレーム表示
void QtMainWindow::on_actionViewWireFlame_triggered()
{
    Kodatuno.WireFlameView();
    ui->describeWidget->repaint();
}

// Function: on_actionViewvoid_triggered
// ツールバー：非表示
void QtMainWindow::on_actionViewVoid_triggered()
{
    Kodatuno.VoidView();
    ui->describeWidget->repaint();
}

// Function: on_actionSelectAll_triggered
// ツールバー：全て選択
void QtMainWindow::on_actionSelectAll_triggered()
{
    Kodatuno.SelectAll();
    ui->describeWidget->repaint();
}

// Function: on_actionSelect_Cancel_triggered
// ツールバー：全ての選択をキャンセル
void QtMainWindow::on_actionSelect_Cancel_triggered()
{
    Kodatuno.SelectionCancel();
    ui->describeWidget->repaint();
}

// Function: on_actionViewErase_triggered
// ツールバー：ユーザーによる描画指令をキャンセル
void QtMainWindow::on_actionViewErase_triggered()
{
    Kodatuno.UserViewCanncel();
    ui->describeWidget->repaint();
}

// Function: on_actionViewBlend_triggered
// BODYを半透明
void QtMainWindow::on_actionViewBlend_triggered()
{
    Kodatuno.ViewBlending();
    ui->describeWidget->repaint();
}

// Function: on_actionShowConsole_triggered
// ConsoleのDOCK表示
void QtMainWindow::on_actionShowConsole_triggered()
{
    ui->consoleDock->setVisible(true);
}

// Function: on_actionShowBodyList_triggered
// BODY ListのDOCK表示
void QtMainWindow::on_actionShowBodyList_triggered()
{
    ui->bodyListDock->setVisible(true);
}

// Function: on_bodyList_itemClicked
// BODY List中のアイテムを選択
//
// Paramerters:
// *item - アイテムの情報
void QtMainWindow::on_bodyList_itemClicked(QListWidgetItem *item)
{
    Kodatuno.BodyListSelect();
}

// Function: on_actionUserStatus_triggered
// ユーザーステータスダイアログの表示
void QtMainWindow::on_actionUserStatus_triggered()
{
    // UserStatusダイアログを表示
    QtUserStatDialog* dlg = new QtUserStatDialog(this);
    dlg->show();
}

// Function: on_actionUserFunc1_triggered
// ユーザー関数1の実行
void QtMainWindow::on_actionUserFunc1_triggered()
{
    Kodatuno.ExecuteUserFuncMain(0);
    Kodatuno.ReDrawUserFunc();			// UserFuncによる描画指令を実行
}

// Function: on_actionUserFunc2_triggered
// ユーザー関数2の実行
void QtMainWindow::on_actionUserFunc2_triggered()
{
    Kodatuno.ExecuteUserFuncMain(1);
    Kodatuno.ReDrawUserFunc();			// UserFuncによる描画指令を実行
}

// Function: on_actionUserFunc3_triggered
// ユーザー関数3の実行
void QtMainWindow::on_actionUserFunc3_triggered()
{
    Kodatuno.ExecuteUserFuncMain(2);
    Kodatuno.ReDrawUserFunc();			// UserFuncによる描画指令を実行
}

// Function: on_actionUserFunc4_triggered
// ユーザー関数4の実行
void QtMainWindow::on_actionUserFunc4_triggered()
{
    Kodatuno.ExecuteUserFuncMain(3);
    Kodatuno.ReDrawUserFunc();			// UserFuncによる描画指令を実行
}

// Function: on_actionUserFunc5_triggered
// ユーザー関数5の実行
void QtMainWindow::on_actionUserFunc5_triggered()
{
    Kodatuno.ExecuteUserFuncMain(4);
    Kodatuno.ReDrawUserFunc();			// UserFuncによる描画指令を実行
}

// Function: on_actionUserFunc6_triggered
// ユーザー関数6の実行
void QtMainWindow::on_actionUserFunc6_triggered()
{
    Kodatuno.ExecuteUserFuncMain(5);
    Kodatuno.ReDrawUserFunc();			// UserFuncによる描画指令を実行
}

// Function: on_actionUserFunc7_triggered
// ユーザー関数7の実行
void QtMainWindow::on_actionUserFunc7_triggered()
{
    Kodatuno.ExecuteUserFuncMain(6);
    Kodatuno.ReDrawUserFunc();			// UserFuncによる描画指令を実行
}

// Function: on_actionUserFunc8_triggered
// ユーザー関数8の実行
void QtMainWindow::on_actionUserFunc8_triggered()
{
    Kodatuno.ExecuteUserFuncMain(7);
    Kodatuno.ReDrawUserFunc();			// UserFuncによる描画指令を実行
}

// Function: on_actionAboutKodatuno_triggered
// AboutBOXの表示
void QtMainWindow::on_actionAboutKodatuno_triggered()
{
    QMessageBox::information(   this,
                                tr("About Kodatuno"),
                                tr("Kodatuno R%1.%2 alpha").arg(KODATUNO_MAJOR_VER).arg(KODATUNO_MINOR_VER)
                                );
}
