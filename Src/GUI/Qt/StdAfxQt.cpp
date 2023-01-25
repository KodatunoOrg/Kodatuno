//GEからGUIへデータをセットするためのクラス

#include "StdAfxQt.h"

// Variable: GuiIF
// クラスGUI_Interfaceのインスタンスをグローバルで宣言
GUI_Interface GuiIF;		

// Fnction: SetConsoleObject
// コンソール用GUIのオブジェクトのインスタンスをセット
//
// Parameters:
// *textEdit - Consoleウィジェットのオブジェクトを取得する
void GUI_Interface::SetConsoleObject(QTextEdit* textEdit)
{
    consoleText = textEdit;
}

// Function: setBodyListObject
// BODYリスト用GUIのオブジェクトのインスタンスをセット
// 
// Parameters: 
// *list - bodyListウィジェットのオブジェクトを取得する
void GUI_Interface::SetBodyListObject(QListWidget* list)
{
    bodyList = list;
}

// Function: SetOpenFileActionObject
// ファイルオープン用GUIのオブジェクトのインスタンスをセット
// 
// Parameters: 
// *action - actionOpenBodyウィジェットのオブジェクトを取得する
void GUI_Interface::SetOpenFileActionObject(QAction *action)
{
    openFileAction = action;
}

// Function: SetDelBodyActionObject
// BODY削除用GUIのオブジェクトのインスタンスをセット
//
// Parameters:
// *action - actionDeleteBodyウィジェットのオブジェクトを取得する
void GUI_Interface::SetDelBodyActionObject(QAction *action)
{
    delBodyAction = action;
}

// Function: SetDescribeWidget
// OpenGL描画用GUIのオブジェクトのインスタンスをセット
//
// Parameters:
// *widget - descriveウィジェットのオブジェクトを取得する
void GUI_Interface::SetDescribeWidget(QGLWidget *widget)
{
    glWidget = widget;
}

// Function: SetStatusBarObject
// ステータスバーラベル用GUIのオブジェクトのインスタンスをセット
// 
// Parameters:
// *bar - statusBarウィジェットのオブジェクトを取得する
void GUI_Interface::SetStatusBarObject(QStatusBar *bar)
{
    statusBar = bar;
}

// Function: SetUserFunc1Action
// UserFunc1用GUIのオブジェクトのインスタンスをセット
//
// Parameters:
// *action - actionUserFunc1ウィジェットのオブジェクトを取得する
void GUI_Interface::SetUserFunc1Action(QAction* action)
{
    userFunc1 = action;
}

// Function: SetUserFunc2Action
// UserFunc2用GUIのオブジェクトのインスタンスをセット
//
// Parameters: 
// *action - actionUserFunc2ウィジェットのオブジェクトを取得する
void GUI_Interface::SetUserFunc2Action(QAction* action)
{
    userFunc2 = action;    
}

// Function: SetUserFunc3Action
// UserFunc3用GUIのオブジェクトのインスタンスをセット
//
// Parameters:
// *action - actionUserFunc3ウィジェットのオブジェクトを取得する
void GUI_Interface::SetUserFunc3Action(QAction* action)
{
    userFunc3 = action;    
}

// Function: SetUserFunc4Action
// UserFunc4用GUIのオブジェクトのインスタンスをセット
//
// Parameters: 
// *action - actionUserFunc4ウィジェットのオブジェクトを取得する
void GUI_Interface::SetUserFunc4Action(QAction* action)
{
    userFunc4 = action;    
}

// Function: SetUserFunc5Action
// UserFunc5用GUIのオブジェクトのインスタンスをセット
//
// Parameters:
// *action - actionUserFunc5ウィジェットのオブジェクトを取得する
void GUI_Interface::SetUserFunc5Action(QAction* action)
{
    userFunc5 = action;    
}

// Function: SetUserFunc6Action
// UserFunc6用GUIのオブジェクトのインスタンスをセット
//
// Parameters:
// *action - actionUserFunc6ウィジェットのオブジェクトを取得する
void GUI_Interface::SetUserFunc6Action(QAction* action)
{
    userFunc6 = action;
}

// Function: SetUserFunc7Action
// UserFunc7用GUIのオブジェクトのインスタンスをセット
//
// Parameteers:
// *action - actionUserFunc7ウィジェットのオブジェクトを取得する
void GUI_Interface::SetUserFunc7Action(QAction* action)
{
    userFunc7 = action;
}

// Function: SetUserFunc8Action
// UserFunc8用GUIのオブジェクトのインスタンスをセット
// 
// Parameters:
// *action - actionUserFunc8ウィジェットのオブジェクトを取得する
void GUI_Interface::SetUserFunc8Action(QAction* action)
{
    userFunc8 = action;
}

// Function: SetMessage
// コンソールにメッセージを出力
//
// Parameters:
// *mes - コンソールに出力したいメッセージ
int GUI_Interface::SetMessage(const char *mes)
{
    QString text = consoleText->toPlainText() + QString(mes);

    consoleText->setText(text);
    consoleText->append(">");
    
    /* カーソルを末尾に移動 */
    QTextCursor cursor = consoleText->textCursor();
    cursor.movePosition(QTextCursor::End);
    consoleText->setTextCursor(cursor);

    statusBar->showMessage(QString(mes),0);

    int len = consoleText->toPlainText().length();
    return len;
}

// Function: SetCommand
// コンソールにコマンドを出力
//
// Parameters: 
// *cmd - コンソールに出力したいコマンド
int GUI_Interface::SetCommand(const char *cmd)
{
    QString text = consoleText->toPlainText() + QString(cmd);
    consoleText->setText(text);
    consoleText->append(">");

    /* カーソルを末尾に移動 */
    QTextCursor cursor = consoleText->textCursor();
    cursor.movePosition(QTextCursor::End);
    consoleText->setTextCursor(cursor);

    int len = consoleText->toPlainText().length();
    return len;
}

// Function: ExecCommand
// コマンドを実行する
// Parameters:
// cmd - 実行するコマンド文字列
void GUI_Interface::ExecCommand(const char *cmd)
{
    QString text = consoleText->toPlainText() + QString(cmd);
    consoleText->setText(text);

    QKeyEvent keyevent(QEvent::KeyPress,Qt::Key_Enter,Qt::NoModifier);
    QApplication::sendEvent(consoleText,&keyevent);
}

// Function: AddBodyNameToWin
// 新たに登録したBODYの名前をウィンドウリストに表示する
//
// Paranmeters:
// *name - ウィンドウリストに表示するBODYの名前
void GUI_Interface::AddBodyNameToWin(const char *name)
{
    bodyList->addItem(name);
}

// Function: DelBodyFromWin
// 指定された番号のBODYをリストから削除する
//
// Parameters:
// pos - bodyの番号
void GUI_Interface::DelBodyFromWin(int pos)
{
    QListWidgetItem* item = bodyList->takeItem(pos);
    delete item;
}

// Function: SetEnableOpenFile
// ファイルOPEN用オブジェクトの状態設定
//
// Parameters:
// enable - 正負の判定
void GUI_Interface::SetEnableOpenFile(bool enable)
{
    openFileAction->setEnabled(enable);
}

// Function: SetEnableDelBody
// BODY削除用オブジェクトの状態設定
//
// Parameters:
// enable - 正負の判定
void GUI_Interface::SetEnableDelBody(bool enable)
{
    delBodyAction->setEnabled(enable);
}

// Function: UpdateDescribeWidget
//描画用GUIの再描画指令
void GUI_Interface::UpdateDescribeWidget()
{
    glWidget->update();
}

// Function: GetConsoleText
// コンソールから文字列を取得
//
// Return:
// 取得した文字列
std::string GUI_Interface::GetConsoleText()
{
    std::string text(consoleText->toPlainText().toUtf8().constData());

    return text;
}

// Function: GetBodyListSelectedPos
// BODYリストの選択されている番号を取得する
//
// Return: 
// bodyList中の選択されている行番号
int GUI_Interface::GetBodyListSelectedPos()
{
    return bodyList->currentRow();
}

// Function: OpenBodyFileDlg
// ファイルオープンダイアログの表示（BODY）
//
// Return:  
// ファイルのフルパス
std::string GUI_Interface::OpenBodyFileDlg()
{
    // ファイルのフィルタリスト
    QString fileFilter = QObject::tr("IGS (*.igs);; STL (*.stl);; VRML (*.wrl);; DXF (*.dxf)");

    QString fileName = QFileDialog::getOpenFileName(
        0,                          //　親widget
        QObject::tr("Open File"),   // ダイアログタイトル
        QDir::currentPath(),        // 起動時のディレクトリ
        fileFilter                  // ファイルフィルター
    );

    std::string stdStr(fileName.toLocal8Bit());
    return stdStr;
}

// Function: OpenInpFileDlg
// ファイルオープンダイアログの表示（INP）
//
// Return: 
// ファイルのフルパス
std::string GUI_Interface::OpenInpFileDlg()
{
    // ファイルのフィルタリスト
    QString fileFilter = QObject::tr("INP (*.inp)");

    QString fileName = QFileDialog::getOpenFileName(
        0,                          //　親widget
        QObject::tr("Open File"),   // ダイアログタイトル
        QDir::currentPath(),        // 起動時のディレクトリ
        fileFilter                  // ファイルフィルター
    );

    std::string stdStr(fileName.toUtf8());
    return stdStr;
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
    QString fileName = QFileInfo(QString::fromStdString(fullPath)).baseName();
    
    std::string stdStr(fileName.toUtf8());
    return stdStr;
}

// Function: GetFileExt
// フルパスからファイルの拡張子抽出
//
// Parameters:
// fullPath - フルパス
// 
// Return: 
// 拡張子
std::string GUI_Interface::GetFileExt(const std::string fullPath)
{
    QString ext  = QFileInfo(QString::fromStdString(fullPath)).suffix();
    
    std::string stdStr(ext.toUtf8());
    return stdStr;
}

// Function: ShowNurbsCurveDlg
// Nurbs曲線ダイアログ表示
void GUI_Interface::ShowNurbsCurveDlg()
{
    QtNurbsCurveDialog *dlg = new QtNurbsCurveDialog();
    dlg->exec();
    glWidget->update();
}

// Function: ShowNurbsSurfDlg
// Nurbs曲面ダイアログ表示
void GUI_Interface::ShowNurbsSurfDlg()
{
    QtNurbsSurfDialog *dlg = new QtNurbsSurfDialog();
    dlg->exec();
    glWidget->update();    
}

// Function: ShowRotSurfDlg
// 回転サーフェスダイアログ表示
void GUI_Interface::ShowRotSurfDlg()
{
    QtRotSurfDialog *dlg = new QtRotSurfDialog();
    dlg->exec();
    glWidget->update();
}

// Function: ShowSweepSurfDlg
// スイープサーフェスダイアログ表示
void GUI_Interface::ShowSweepSurfDlg()
{
    QtSweepSurfDialog *dlg = new QtSweepSurfDialog();
    dlg->exec();
    glWidget->update();    
}

// Function: SetUserFunc1Label
// UserFunc1のラベルをセット
//
// Parameters:
// *label - Userメニュー名(0)
void GUI_Interface::SetUserFunc1Label(const char *label)
{
    userFunc1->setText(QString(label));
}

// Function: SetUserFunc2Label
// UserFunc2のラベルをセット
//
// Parameters:
// *label - Userメニュー名(1)
void GUI_Interface::SetUserFunc2Label(const char *label)
{
    userFunc2->setText(QString(label));    
}

// Function: SetUserFunc3Label
// UserFunc3のラベルをセット
//
// Parameters:
// *label - Userメニュー名(2)
void GUI_Interface::SetUserFunc3Label(const char *label)
{
    userFunc3->setText(QString(label));
}

// Function: SetUserFunc4Label
// UserFunc4のラベルをセット
//
// Parameters:
// *label - Userメニュー名(3)
void GUI_Interface::SetUserFunc4Label(const char *label)
{    
    userFunc4->setText(QString(label));
}

// Function: SetUserFunc5Label
// UserFunc5のラベルをセット
//
// Parameters:
// *label - Userメニュー名(4)
void GUI_Interface::SetUserFunc5Label(const char *label)
{    
    userFunc5->setText(QString(label));
}

// Function: SetUserFunc6Label
// UserFunc6のラベルをセット
//
// Parameters:
// *label - Userメニュー名(5)
void GUI_Interface::SetUserFunc6Label(const char *label)
{
    userFunc6->setText(QString(label));
}

// Function: SetUserFunc7Label
// UserFunc7のラベルをセット
//
// Parameters:
// *label - Userメニュー名(6)
void GUI_Interface::SetUserFunc7Label(const char *label)
{
    userFunc7->setText(QString(label));
}

// Function: SetUserFunc8Label
// UserFunc8のラベルをセット
//
// Parameters:
// *label - Userメニュー名(7)
void GUI_Interface::SetUserFunc8Label(const char *label)
{
    userFunc8->setText(QString(label));
}
