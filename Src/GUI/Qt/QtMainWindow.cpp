#include "QtMainWindow.h"
#include "ui_QtMainWindow.h"

KODatUNO Kodatuno;	// KODatUNOクラスのインスタンスをグローバル宣言

// メインウィンドウ生成
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

// Initializing GUI
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

QtMainWindow::~QtMainWindow()
{
    Kodatuno.DeleteWin();
    delete ui;
}

// KodatunoGUIクローズ時のイベント
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

// メニュー：Create->NURBS Rotation
void QtMainWindow::on_actionRotationSurface_triggered()
{
    QtRotSurfDialog* dlg = new QtRotSurfDialog(this);
    dlg->exec();
    ui->describeWidget->repaint();
}

// メニュー：Create->Sweep
void QtMainWindow::on_actionSweepSurface_triggered()
{
    QtSweepSurfDialog* dlg = new QtSweepSurfDialog (this);
    dlg->exec();
    ui->describeWidget->repaint();
}

// メニュー：Create->NURBS Curve
void QtMainWindow::on_actionNURBSCurve_triggered()
{
    QtNurbsCurveDialog* dlg = new QtNurbsCurveDialog(this);
    dlg->exec();
    ui->describeWidget->repaint();
}

// メニュー：Create->NURBS Surface
void QtMainWindow::on_actionNURBSSarface_triggered()
{
    QtNurbsSurfDialog* dlg = new QtNurbsSurfDialog (this);
    dlg->exec();
    ui->describeWidget->repaint();
}

// メニュー：File->Open File
void QtMainWindow::on_actionOpenBody_triggered()
{
    Kodatuno.OpenFile();		// ファイルオープン
    Kodatuno.OpenDelBtn();      // Open，Deleteボタンの表示属性の変更
    ui->describeWidget->repaint();
}

// メニュー：Edit->Delete BODY
void QtMainWindow::on_actionDeleteBody_triggered()
{
    Kodatuno.DeleteBody();  	// 選択されているBODYを消去
    Kodatuno.OpenDelBtn();      // Open，Deleteボタンの表示属性の変更
    ui->describeWidget->repaint();
}

// ツールバー：X-View
void QtMainWindow::on_actionViewOnX_triggered()
{
    Kodatuno.ChangeViewX();
    ui->describeWidget->repaint();
}

// ツールバー：Y-View
void QtMainWindow::on_actionViewOnY_triggered()
{
    Kodatuno.ChangeViewY();
    ui->describeWidget->repaint();
}

// ツールバー：Z-View
void QtMainWindow::on_actionViewOnZ_triggered()
{
    Kodatuno.ChangeViewZ();
    ui->describeWidget->repaint();
}

// ツールバー：アイソメView
void QtMainWindow::on_actionViewOnXYZ_triggered()
{
    Kodatuno.ChangeViewXYZ();
    ui->describeWidget->repaint();
}

// ツールバー：画面にフィット
void QtMainWindow::on_actionViewFit_triggered()
{
    Kodatuno.FitView();
    ui->describeWidget->repaint();
}

// ツールバー：ソリッド表示
void QtMainWindow::on_actionViewSolid_triggered()
{
    Kodatuno.SolidView();
    ui->describeWidget->repaint();
}

// ツールバー：ソリッド（外形線なし）表示
void QtMainWindow::on_actionViewNoEdge_triggered()
{
    Kodatuno.NoEdgeView();
    ui->describeWidget->repaint();
}

// ツールバー：ワイヤーフレーム表示
void QtMainWindow::on_actionViewWireFlame_triggered()
{
    Kodatuno.WireFlameView();
    ui->describeWidget->repaint();
}

// ツールバー：非表示
void QtMainWindow::on_actionViewVoid_triggered()
{
    Kodatuno.VoidView();
    ui->describeWidget->repaint();
}

// ツールバー：全て選択
void QtMainWindow::on_actionSelectAll_triggered()
{
    Kodatuno.SelectAll();
    ui->describeWidget->repaint();
}

// ツールバー：全ての選択をキャンセル
void QtMainWindow::on_actionSelect_Cancel_triggered()
{
    Kodatuno.SelectionCancel();
    ui->describeWidget->repaint();
}

// ツールバー：ユーザーによる描画指令をキャンセル
void QtMainWindow::on_actionViewErase_triggered()
{
    Kodatuno.UserViewCanncel();
    ui->describeWidget->repaint();
}

// ツールバー：半透明表示
void QtMainWindow::on_actionViewBlend_triggered()
{
    Kodatuno.ViewBlending();
    ui->describeWidget->repaint();
}

// メニュー：View->Show Console
void QtMainWindow::on_actionShowConsole_triggered()
{
    ui->consoleDock->setVisible(true);
}

// メニュー：View->Show Body List
void QtMainWindow::on_actionShowBodyList_triggered()
{
    ui->bodyListDock->setVisible(true);
}

// BODY List中のアイテムを選択
void QtMainWindow::on_bodyList_itemClicked(QListWidgetItem *item)
{
    Kodatuno.BodyListSelect();
}

// メニュー：User->User Status
void QtMainWindow::on_actionUserStatus_triggered()
{
    // UserStatusダイアログを表示
    QtUserStatDialog* dlg = new QtUserStatDialog(this);
    dlg->show();
}

// メニュー：User->User Status->User Func1
void QtMainWindow::on_actionUserFunc1_triggered()
{
    Kodatuno.ExecuteUserFuncMain(0);
    Kodatuno.ReDrawUserFunc();			// UserFuncによる描画指令を実行
}

// メニュー：User->User Status->User Func2
void QtMainWindow::on_actionUserFunc2_triggered()
{
    Kodatuno.ExecuteUserFuncMain(1);
    Kodatuno.ReDrawUserFunc();			// UserFuncによる描画指令を実行
}

// メニュー：User->User Status->User Func3
void QtMainWindow::on_actionUserFunc3_triggered()
{
    Kodatuno.ExecuteUserFuncMain(2);
    Kodatuno.ReDrawUserFunc();			// UserFuncによる描画指令を実行
}

// メニュー：User->User Status->User Func4
void QtMainWindow::on_actionUserFunc4_triggered()
{
    Kodatuno.ExecuteUserFuncMain(3);
    Kodatuno.ReDrawUserFunc();			// UserFuncによる描画指令を実行
}

// メニュー：User->User Status->User Func5
void QtMainWindow::on_actionUserFunc5_triggered()
{
    Kodatuno.ExecuteUserFuncMain(4);
    Kodatuno.ReDrawUserFunc();			// UserFuncによる描画指令を実行
}

// メニュー：User->User Status->User Func6
void QtMainWindow::on_actionUserFunc6_triggered()
{
    Kodatuno.ExecuteUserFuncMain(5);
    Kodatuno.ReDrawUserFunc();			// UserFuncによる描画指令を実行
}

// メニュー：User->User Status->User Func7
void QtMainWindow::on_actionUserFunc7_triggered()
{
    Kodatuno.ExecuteUserFuncMain(6);
    Kodatuno.ReDrawUserFunc();			// UserFuncによる描画指令を実行
}

// メニュー：User->User Status->User Func8
void QtMainWindow::on_actionUserFunc8_triggered()
{
    Kodatuno.ExecuteUserFuncMain(7);
    Kodatuno.ReDrawUserFunc();			// UserFuncによる描画指令を実行
}

// メニュー：Help->About Kodatuno
void QtMainWindow::on_actionAboutKodatuno_triggered()
{
    QMessageBox::information(   this,
                                tr("About Kodatuno"),
                                tr("Kodatuno R%1.%2").arg(KODATUNO_MAJOR_VER).arg(KODATUNO_MINOR_VER)
                                );
}
