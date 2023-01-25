//GEからGUIへデータをセットするためのクラス

#include "StdAfxQt.h"

GUI_Interface GuiIF;		// クラスGUI_Interfaceのインスタンスをグローバルで宣言

//コンソール用GUIのオブジェクトのインスタンスをセット
void GUI_Interface::SetConsoleObject(QTextEdit* textEdit)
{
    consoleText = textEdit;
}

//BODYリスト用GUIのオブジェクトのインスタンスをセット
void GUI_Interface::SetBodyListObject(QListWidget* list)
{
    bodyList = list;
}

// ファイルオープン用GUIのオブジェクトのインスタンスをセット
void GUI_Interface::SetOpenFileActionObject(QAction *action)
{
    openFileAction = action;
}

// BODY削除用GUIのオブジェクトのインスタンスをセット
void GUI_Interface::SetDelBodyActionObject(QAction *action)
{
    delBodyAction = action;
}

//OpenGL描画用GUIのオブジェクトのインスタンスをセット
void GUI_Interface::SetDescribeWidget(QGLWidget *widget)
{
    glWidget = widget;
}

//ステータスバーラベル用GUIのオブジェクトのインスタンスをセット
void GUI_Interface::SetStatusBarObject(QStatusBar *bar)
{
    statusBar = bar;
}
// UserFunc1用GUIのオブジェクトのインスタンスをセット
void GUI_Interface::SetUserFunc1Action(QAction* action)
{
    userFunc1 = action;
}

// UserFunc2用GUIのオブジェクトのインスタンスをセット
void GUI_Interface::SetUserFunc2Action(QAction* action)
{
    userFunc2 = action;    
}

// UserFunc3用GUIのオブジェクトのインスタンスをセット
void GUI_Interface::SetUserFunc3Action(QAction* action)
{
    userFunc3 = action;    
}

// UserFunc4用GUIのオブジェクトのインスタンスをセット
void GUI_Interface::SetUserFunc4Action(QAction* action)
{
    userFunc4 = action;    
}

// UserFunc5用GUIのオブジェクトのインスタンスをセット
void GUI_Interface::SetUserFunc5Action(QAction* action)
{
    userFunc5 = action;    
}

// UserFunc6用GUIのオブジェクトのインスタンスをセット
void GUI_Interface::SetUserFunc6Action(QAction* action)
{
    userFunc6 = action;
}

// UserFunc7用GUIのオブジェクトのインスタンスをセット
void GUI_Interface::SetUserFunc7Action(QAction* action)
{
    userFunc7 = action;
}

// UserFunc8用GUIのオブジェクトのインスタンスをセット
void GUI_Interface::SetUserFunc8Action(QAction* action)
{
    userFunc8 = action;
}

// コンソールにメッセージを出力
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

// コンソールにコマンドを出力
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

// 新たに登録したBODYの名前をウィンドウリストに表示する
void GUI_Interface::AddBodyNameToWin(const char *name)
{
    bodyList->addItem(name);
}

// 指定された番号のBODYをリストから削除する
void GUI_Interface::DelBodyFromWin(int pos)
{
    QListWidgetItem* item = bodyList->takeItem(pos);
    delete item;
}

// ファイルOPEN用オブジェクトの状態設定
void GUI_Interface::SetEnableOpenFile(bool enable)
{
    openFileAction->setEnabled(enable);
}

// BODY削除用オブジェクトの状態設定
void GUI_Interface::SetEnableDelBody(bool enable)
{
    delBodyAction->setEnabled(enable);
}

//描画用GUIの再描画指令
void GUI_Interface::UpdateDescribeWidget()
{
    glWidget->update();
}

// コンソールから文字列を取得
std::string GUI_Interface::GetConsoleText()
{
    std::string text(consoleText->toPlainText().toUtf8());
    return text;
}

// BODYリストの選択されている番号を取得する
int GUI_Interface::GetBodyListSelectedPos()
{
    return bodyList->currentRow();
}

// ファイルオープンダイアログの表示（BODY）
//return ファイルのフルパス
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

    std::string stdStr(fileName.toUtf8());
    return stdStr;
}

// ファイルオープンダイアログの表示（INP）
//return ファイルのフルパス
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

// フルパスからファイル名抽出
std::string GUI_Interface::GetFileName(const std::string fullPath)
{
    QString fileName = QFileInfo(QString::fromStdString(fullPath)).baseName();
    
    std::string stdStr(fileName.toUtf8());
    return stdStr;
}

// フルパスからファイルの拡張子抽出
std::string GUI_Interface::GetFileExt(const std::string fullPath)
{
    QString ext  = QFileInfo(QString::fromStdString(fullPath)).suffix();
    
    std::string stdStr(ext.toUtf8());
    return stdStr;
}

// Nurbs曲線ダイアログ表示
void GUI_Interface::ShowNurbsCurveDlg()
{
    QtNurbsCurveDialog *dlg = new QtNurbsCurveDialog();
    dlg->exec();
    glWidget->update();
}

// Nurbs曲面ダイアログ表示
void GUI_Interface::ShowNurbsSurfDlg()
{
    QtNurbsSurfDialog *dlg = new QtNurbsSurfDialog();
    dlg->exec();
    glWidget->update();    
}

// 回転サーフェスダイアログ表示
void GUI_Interface::ShowRotSurfDlg()
{
    QtRotSurfDialog *dlg = new QtRotSurfDialog();
    dlg->exec();
    glWidget->update();
}

// スイープサーフェスダイアログ表示
void GUI_Interface::ShowSweepSurfDlg()
{
    QtSweepSurfDialog *dlg = new QtSweepSurfDialog();
    dlg->exec();
    glWidget->update();    
}

// UserFunc1のラベルをセット
void GUI_Interface::SetUserFunc1Label(const char *label)
{
    userFunc1->setText(QString(label));
}

// UserFunc2のラベルをセット
void GUI_Interface::SetUserFunc2Label(const char *label)
{
    userFunc2->setText(QString(label));    
}

// UserFunc3のラベルをセット
void GUI_Interface::SetUserFunc3Label(const char *label)
{
    userFunc3->setText(QString(label));
}

// UserFunc4のラベルをセット
void GUI_Interface::SetUserFunc4Label(const char *label)
{    
    userFunc4->setText(QString(label));
}

// UserFunc5のラベルをセット
void GUI_Interface::SetUserFunc5Label(const char *label)
{    
    userFunc5->setText(QString(label));
}

// UserFunc6のラベルをセット
void GUI_Interface::SetUserFunc6Label(const char *label)
{
    userFunc6->setText(QString(label));
}
// UserFunc7のラベルをセット
void GUI_Interface::SetUserFunc7Label(const char *label)
{
    userFunc7->setText(QString(label));
}
// UserFunc8のラベルをセット
void GUI_Interface::SetUserFunc8Label(const char *label)
{
    userFunc8->setText(QString(label));
}
