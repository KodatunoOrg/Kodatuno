// Kodatunoメインクラス
// BODY描画、インターフェースを定義

#include "Kodatuno.h"

// Kodatunoフォームの初期化処理
void KODatUNO::InitializeWin()
{

	SetMessage("Welcome to Kodatuno!");			// コンソール初期化
	WSCstring mes = Console_Form->getString();
	SetConsoleCharNum(mes.getChars());			// 現在のコンソール文字数を保存

	ScreenWidth = Describe_Form->getProperty(WSNwidth);		// 描画ウィンドウの幅を取得
	ScreenHeight = Describe_Form->getProperty(WSNheight);	// 描画ウィンドウの高さを取得

	// コンソールコマンドを登録
	SetCmdList(CmdMap,"verinf","fF",0,CmdVerInf);			// Command バージョン情報を出力
	SetCmdList(CmdMap,"open","N",1,CmdFileOpen);			// Command ファイルオープンダイアログをオープン
	SetCmdList(CmdMap,"wire","",2,CmdUVWire);				// Command Bodyをワイヤーフレーム表示にする
	SetCmdList(CmdMap,"scale","r",3,CmdChScale);			// Command スケールの確認及び変更
	SetCmdList(CmdMap,"rots","",4,CmdGenRotSurf);			// Command 回転サーフェスを生成する
	SetCmdList(CmdMap,"sweeps","",5,CmdGenSweepSurf);		// Command スイープサーフェスを生成する
	SetCmdList(CmdMap,"nurbsc","",6,CmdGenNurbsCurve);		// Command 与えられた点群からNURBS曲線を生成する
	SetCmdList(CmdMap,"nurbss","",7,CmdGenNurbsSurface);	// Command 与えられた点群からNURBS曲面を生成する
	SetCmdList(CmdMap,"tol","r",8,CmdChTolerance);			// Commnad 面描画のトレランス値の確認及び変更
	SetCmdList(CmdMap,"move","r",9,CmdMoveBody);			// Commnad 選択されているBodyを指定した移動量だけ平行移動する
	SetCmdList(CmdMap,"rot","r",10,CmdRotateBody);			// Command 選択されているBodyを指定した角度だけ平行移動する
	SetCmdList(CmdMap,"cpview","",11,CmdCPView);			// Command コントロールポイントを描画
	SetCmdList(CmdMap,"surfinfo","",12,CmdSurfInfo);		// Command 曲面情報を出力
	SetCmdList(CmdMap,"exp","r",13,CmdExpand);				// Command Bodyを拡大する

	// Userコマンドの登録
	User.RegistUserCommand(CmdMap);

	User.SetMenuLabelName();	// Userメニュー名を登録
	User_Func_Btn1->setProperty(WSNlabelString,User.GetMenuLabelName(0));	// ボタンラベルに反映
	User_Func_Btn2->setProperty(WSNlabelString,User.GetMenuLabelName(1));
	User_Func_Btn3->setProperty(WSNlabelString,User.GetMenuLabelName(2));
	User_Func_Btn4->setProperty(WSNlabelString,User.GetMenuLabelName(3));
	User_Func_Btn5->setProperty(WSNlabelString,User.GetMenuLabelName(4));

	// その他初期化
	Focus_Body = KOD_ERR;
	OpenGLInitFlag = KOD_FALSE;
	LBtnFlag = KOD_FALSE;
	RBtnFlag = KOD_FALSE;
	MBtnFlag = KOD_FALSE;
	ShiftKeyFlag = KOD_FALSE;
	CtrlKeyFlag = KOD_FALSE;
	AltKeyFlag = KOD_FALSE;
	SweepSelectFlag = KOD_FALSE;
	SolidViewFlag = KOD_TRUE;
	EdgeViewFlag = KOD_TRUE;
	WireFlameViewFlag = KOD_FALSE;
	BlendViewFlag = KOD_FALSE;
	CPViewFlag = KOD_FALSE;
	InitCmdFlags();
	CmdStackNum = 0;
	CmdStackNow = 0;
	StartX = 0;
	StartY = 0;
	OldPosX = 0;
	OldPosY = 0;
	CurrentX = 0;
	CurrentY = 0;
	AxisScale = 0.2;
	ModelScale = 1;
	ModelScale1st = 1;
	StartQ = QFunc.QInit(1.0,0,0,0);
	TargetQ = QFunc.QInit(1.0,0,0,0);
	QFunc.QtoR(RotMx,StartQ);
	Vp[0] = Vp[1] = Vp[2] = Vp[3] = 0;
	Trl[0] = Trl[1] = Trl[2] = 0.0;
	DrawBODYFlag = KOD_FALSE;
	DrawSurfFlag = KOD_TRUE;
	ReDrawBODYFlag = KOD_FALSE;
	for(int i=0;i<USERFUNCNUMMAX;i++)
		ExecUserFuncFlag[i] = KOD_FALSE;
	for(int i=0;i<USERCOMMANDNUM;i++)
		ExecUserCommandFlag[i] = KOD_FALSE;
	DoSelectFlag = KOD_FALSE;
	Tolerance = TOLERANCE;

	glGenLists(USERFUNCNUMMAX+1);

}

// Kodatunoフォームの終了処理
void KODatUNO::DeleteWin()
{
	int bodynum = BodyList.getNum();				// 読み込んだBODYの数を得る

	for(int i=0;i<bodynum;i++){
		BODY *body = (BODY *)BodyList.getData(i);	// i番目のBODYを選択
		Free_body(body,body->TypeNum);				// BODY内で確保しているメモリーの解放
		delete (BODY *)body;						// BODY自身のメモリー解放
	}
	BodyList.clear();		// BODYリスト自身も消去

	exit(0);
}

// OpenGL描画領域の再描画時に実行される(OpenGLが用意するAPIはこの関数にぶら下がっていなければならない)
void KODatUNO::ReDraw(int x,int y)
{
	SetOpenGLStat();				// 描画ステータスの設定

	// セレクショントリガ
	if(DoSelectFlag == KOD_TRUE){
		DoSelect(x,y);				// セレクション処理実行
		DoSelectFlag = KOD_FALSE;	// セレクショントリガOFF
		ReDrawBODYFlag = KOD_FALSE;	// 再描画
	}

	// 半透明表示切替
	if(BlendViewFlag == KOD_TRUE){
		glDepthMask(GL_FALSE);
		glDisable(GL_BLEND); 
		Describe_Main();				// BODY描画
		glDepthMask(GL_TRUE);
		glEnable(GL_BLEND);  
		Describe_Main();				// BODY描画
	}
	else{
		glDepthMask(GL_TRUE);
		glDisable(GL_BLEND); 
		Describe_Main();				// BODY描画
	}

	// スイープセレクションフラグがONなら描画
	if(SweepSelectFlag == KOD_TRUE){
		glMatrixMode(GL_PROJECTION);				// 投影変換行列スタックを選択
		glPushMatrix();								// 現在の座標系を保存
		glLoadIdentity();							// 投影変換行列スタックの先頭に単位行列を挿入
		gluOrtho2D(0,ScreenWidth,ScreenHeight,0); 	// 視点を設定(ラバーバンドを正しい位置に表示させるため)

		glMatrixMode(GL_MODELVIEW);					// モデルビュー行列
		glPushMatrix();								// 現在の座標系を保存
		glLoadIdentity();							// モデルビュー行列の先頭に単位行列を挿入
		
		DrawRubberband(StartX,StartY,CurrentX,CurrentY);		// ラバーバンドを描画

		// 座標系を元に戻す
		glMatrixMode(GL_MODELVIEW);
		glPopMatrix();
	}

	// コマンド実行による描画
	if(UVWireFlameViewFlag == KOD_TRUE)				// ワイヤーフレームの詳細描画
		UVWireView();
	if(CPViewFlag == KOD_TRUE)						// コントロールポイントの描画
		DescribeCP();



}

// BODY描画メイン
void KODatUNO::Describe_Main()
{
	if(DrawBODYFlag != KOD_TRUE) return;

	// BODY描画フラグがONなら描画
	if(ReDrawBODYFlag == KOD_FALSE){							// 一番初回のみディスプレイリストにBODYを登録
		SetNurbsSTolerance(Tolerance);							// 再描画演算指令を受けた場合、現在のモデルスケールに合ったNurbsトレランスを指定
		glNewList(COMMAND_DRAW_BODY,GL_COMPILE_AND_EXECUTE);	// glDrawBODY()をコマンドとしてメモリーに保存(セレクション処理の高速化)
		DrawBODY();												// IGESデータを描画
		glEndList();
		ReDrawBODYFlag = KOD_TRUE;
	}
	else{
		glCallList(COMMAND_DRAW_BODY);			// 2回目以降の描画呼び出しはディスプレイリストを呼び出す
	}
}

// IGESデータの描画
void KODatUNO::DrawBODY()
{
	int i;
	BODY *body;

	// body描画
	for(int k=0;k<BodyList.getNum();k++){					// 現在リストに登録されているBODYの数だけループ
		if((body = (BODY *)BodyList.getData(k)) != NULL){	// k番目のリストに登録されているBODYのデータを取得
			glPushName(k);									// セレクションバッファを階層化
			// 各タイプごとに描画を実行していく(内部表現をNURBSに限定したので、線分や円などは描画しない)
			for(i=0;i<ALL_ENTITY_TYPE_NUM;i++){
				if(i == _CIRCLE_ARC){						// 円・円弧
					// 円・円弧はNRBS曲線に変換される
					//glPushName(_CIRCLE_ARC);				// セレクションバッファを階層化
					//Draw_CircleArc();
					//glPopName();
				}
				else if(i == _CONIC_ARC){					// 円錐曲線
					//glPushName(_CONIC_ARC);				// セレクションバッファを階層化
					//Draw_ConicArc();
					//glPopName();
				}
				else if(i == _LINE){						// 線分
					// 線分はNURBS曲線に変換される
					//glPushName(_LINE);					// セレクションバッファを階層化
					//Draw_Line();
					//glPopName();
				}
				else if(i == _NURBSC){						// NURBS曲線
					if(EdgeViewFlag == KOD_TRUE){			// Edge表示/非表示切り替え
					glPushName(_NURBSC);					// セレクションバッファを階層化
					Draw_NurbsCurve(body);
					glPopName();
					}
				}
				else if(i == _NURBSS){
					glPushName(_NURBSS);
					Draw_NurbsSurface(body);
					glPopName();
				}
				else if(i == _TRIMMED_SURFACE){				// トリム面(NURBS曲面)
					glPushName(_TRIMMED_SURFACE);
					Draw_TrimSurfe(body);
					glPopName();
				}
			}
			glPopName();
		}
	}
}


// NURBS曲線の描画
void KODatUNO::Draw_NurbsCurve(BODY *Curr_body)
{
	for(int i=0;i<Curr_body->TypeNum[_NURBSC];i++){
		glPushName(i);		// ネームスタックの先頭にiを挿入
		glColor3f(Curr_body->NurbsC[i].Dstat.Color[0],Curr_body->NurbsC[i].Dstat.Color[1],Curr_body->NurbsC[i].Dstat.Color[2]);
		if(Curr_body->NurbsC[i].EntUseFlag == GEOMTRYELEM){				// IGESディレクトリ部の"Entity Use Flag"が0の場合は実際のモデル要素として描画する
			DrawNurbsCurve(Curr_body->NurbsC[i]);						// 描画
		}
		glPopName();		// ネームスタックの先頭を削除
	}
}

// NURBS曲面の描画
void KODatUNO::Draw_NurbsSurface(BODY *Curr_body)
{
	for(int i=0;i<Curr_body->TypeNum[_NURBSS];i++){
		if(Curr_body->NurbsS[i].TrmdSurfFlag == KOD_TRUE)	// トリム面としてNURBS曲面が登録されているなら
			continue;		// 描画しない
		else{
			glPushName(i);
			DrawNurbsSurfe(Curr_body->NurbsS[i]);	// NURBS曲面描画
			glPopName();
		}
	}
}


// トリム面の描画
void KODatUNO::Draw_TrimSurfe(BODY *Curr_body)
{
	for(int i=0;i<Curr_body->TypeNum[_TRIMMED_SURFACE];i++){
		glPushName(i);			// ネームスタックの先頭にiを挿入
		DrawTrimdSurf(Curr_body->TrmS[i]);
		glPopName();			// ネームスタックの先頭を削除
	}
}

// 軸描画
void KODatUNO::DrawAxis(double Ascale,double Mscale)
{
	GLdouble AxisPos = Ascale/Mscale;
	GLdouble CharPos = (Ascale+0.01)/Mscale;

	glDisable(GL_LIGHTING);

	glLineWidth(1);
	glBegin(GL_LINES);
	//draw Xaxis　赤
	glColor3f(1.0, 0.0, 0.0);
	glVertex3d(0.0, 0.0, 0.0);
	glVertex3d(AxisPos, 0.0, 0.0);

	//draw Yaxis　緑
	glColor3f(0.0, 1.0, 0.0 );
	glVertex3d(0.0, 0.0, 0.0);
	glVertex3d(0.0, AxisPos, 0.0);

	//draw Zaxis　青
	glColor3f(0.0, 0.0, 1.0);
	glVertex3d(0.0, 0.0, 0.0);
	glVertex3d(0.0, 0.0, AxisPos);
	glEnd();

	/* Draw Xcone */
	glColor3f(1.0, 0.0, 0.0);
	glPushMatrix();
	glTranslatef(AxisPos, 0.0, 0.0);
	glRotatef(90.0, 0.0, 1.0, 0.0);
	glutSolidCone(AxisPos*0.1,AxisPos*0.5,4,1);
	glPopMatrix();

	/* Draw Ycone */
	glColor3f(0.0, 1.0, 0.0);
	glPushMatrix();
	glTranslatef(0.0, AxisPos, 0.0);
	glRotatef(-90.0, 1.0, 0.0, 0.0);
	glutSolidCone(AxisPos*0.1,AxisPos*0.5,4,1);
	glPopMatrix();

	/* Draw Zcone */
	glColor3f(0.0, 0.0, 1.0);
	glPushMatrix();
	glTranslatef(0.0, 0.0, AxisPos);
	glutSolidCone(AxisPos*0.1,AxisPos*0.5,4,1);
	glPopMatrix();

	// 軸名 X
	glColor3d(1.0,0.0,0.0);
	glRasterPos3d(CharPos,0.0,0.0);
	glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10,'X');

	// 軸名 Y
	glColor3d(0.0,1.0,0.0);
	glRasterPos3d(0.0,CharPos,0.0);
	glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10,'Y');

	// 軸名 Z
	glColor3d(0.0,0.0,1.0);
	glRasterPos3d(0.0,0.0,CharPos);
	glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10,'Z');

	glEnable(GL_LIGHTING);
}

// ラバーバンド描画
void KODatUNO::DrawRubberband(double sx,double sy,double ex,double ey)
{
	glDisable(GL_LIGHTING);
	glLineWidth(1);
	glLineStipple(1, 0xE0E0);
	glColor3f(0.2, 0.9, 0.9);
	glEnable(GL_LINE_STIPPLE);
	glBegin(GL_LINE_STRIP);
	glVertex2d(sx,sy);
	glVertex2d(ex,sy);
	glVertex2d(ex,ey);
	glVertex2d(sx,ey);
	glVertex2d(sx,sy);
	glEnd();
	glDisable(GL_LINE_STIPPLE);
	glEnable(GL_LIGHTING);
}

// コンソール文字数を保存
void KODatUNO::SetConsoleCharNum(int len)
{
	ConsoleCharNum = len;
}

// コンソール文字数を出力
int KODatUNO::GetConsoleCharNum()
{
	return ConsoleCharNum;
}

// 3Dデータファイルの読み込み
int KODatUNO::OpenFile()
{
	WSCfileSelect *fs=WSGIappFileSelect();			// ファイル選択ダイアログのアドレスを取得

	fs->setProperty(WSNmaskFileName,"IGS,igs,STL,stl");		// IGS,STLファイルのみ選択可

	fs->setProperty(WSNx,300);						// ダイアログ表示位置
	fs->setProperty(WSNy,320);

	long fchoice_dlg = fs->popup();					// ファイル選択ダイアログを開く

	if(fchoice_dlg == WS_DIALOG_OK){				// ダイアログのOKボタンが押されたら
		SetMessage("Reading...");
		WSCstring full_name = fs->getFileName();		// ファイルのフルパス取得
		WSCstring fname = fs->getProperty(WSNfileName);	// ファイル名取得
		fname.to_lower();								// ファイル名を全て小文字に変換
		Body_List_Win->addItem(fname);					// Bodyリストウィンドウに新たに読み込んだBODY名を付加
		int priodnum = fname.getWords(".");				// ファイル名に含まれているピリオドの数を調べる
		WSCstring ext = fname.getWord(priodnum-1,".");	// 拡張子を取得

		BODY *body = new BODY;							// BODYを1つメモリー確保

		WSCstring ext_igs("igs");
		WSCstring ext_stl("stl");

		int flag;
		if(ext == ext_igs){				// 拡張子が"igs"
			IGES_PARSER Iges;
			flag = Iges.IGES_Parser_Main(body,full_name);			// IGESデータを読み込んで、bodyに格納
			if(flag == KOD_TRUE)	Iges.NormalizeKnotRange(body);	// ノットベクトルの正規化
			if(flag == KOD_TRUE)	Iges.CheckDegenracy(body);		// 縮退(2Dパラメトリック曲線の始点と終点が一致しているか)のチェック
			if(flag == KOD_TRUE)	Iges.ModifyParamConect(body);	// パラメトリック平面内のトリム曲線同士のつながりをチェック、修正する
		}
		else if(ext == ext_stl){		// 拡張子が"stl"
			STL_PARSER Stl;
			flag = Stl.STL_Parser_Main(body,full_name);
		}
		if(flag == KOD_ERR){
			delete body;					// エラーの場合はBODYのメモリー解放
			SetMessage("This file was not able to be read normaly.");
			return KOD_ERR;
		}
		strcpy(body->Name,fname);				// ファイル名をbody名として登録
		BodyList.add((void *)body);				// リストに読み込んだbodyを登録
		SetMessage("Finished");
	}
	
	DrawBODYFlag = KOD_TRUE;				// BODY描画してもOKフラグON
	ReDrawBODYFlag = KOD_FALSE;				// BODYのメモリリストを再取得

	SetModelScale();						// モデルスケールを最適化

	return KOD_TRUE;
}

// 3Dデータファイルの読み込み(オーバーロード)
int KODatUNO::OpenFile(char *Fname)
{
	WSCstring fname(Fname);
	fname.to_lower();							// ファイル名を全て小文字に変換
	Body_List_Win->addItem(fname);				// Bodyリストウィンドウに新たに読み込んだBODY名を付加
	int priodnum = fname.getWords(".");				// ファイル名に含まれているピリオドの数を調べる
	WSCstring ext = fname.getWord(priodnum-1,".");	// 拡張子を取得

	BODY *body = new BODY;						// BODYを1つメモリー確保

	WSCstring ext_igs("igs");
	WSCstring ext_stl("stl");

	int flag;
	if(ext == ext_igs){				// 拡張子が"igs"
		IGES_PARSER Iges;
		flag = Iges.IGES_Parser_Main(body,fname);		// IGESデータを読み込んで、bodyに格納
		Iges.CheckDegenracy(body);					// 縮退(2Dパラメトリック曲線の始点と終点が一致しているか)のチェック
	}
	else if(ext == ext_stl){		// 拡張子が"stl"
		STL_PARSER Stl;
		flag = Stl.STL_Parser_Main(body,fname);
	}
	if(flag == KOD_ERR){
		delete body;					// エラーの場合はBODYのメモリー解放
		SetMessage("This file was not able to be read normaly.");
		return KOD_ERR;
	}
	strcpy(body->Name,fname);				// ファイル名をbody名として登録
	BodyList.add((void *)body);				// リストに読み込んだbodyを登録
	SetMessage("Finished");

	DrawBODYFlag = KOD_TRUE;				// BODY描画してもOKフラグON
	ReDrawBODYFlag = KOD_FALSE;				// BODYのメモリリストを再取得

	SetModelScale();						// モデルスケールを最適化

	OpenDelBtn();

	return KOD_TRUE;
}

// インプットファイルの読み込み
int KODatUNO::OpenInpFile()
{
	WSCfileSelect *fs=WSGIappFileSelect();			// ファイル選択ダイアログのアドレスを取得

	fs->setProperty(WSNmaskFileName,"INP,inp");		// inpファイルのみ選択可

	long fchoice_dlg = fs->popup();					// ファイル選択ダイアログを開く

	if(fchoice_dlg == WS_DIALOG_OK){				// ダイアログのOKボタンが押されたら
		WSCstring full_name = fs->getFileName();	// ファイルのフルパス取得
		InpFileName->setProperty(WSNlabelString,full_name);	// ファイルのフルパスをユーザー設定文字列にセット
	}

	return KOD_TRUE;
}


// 選択されているBODYを消去
void KODatUNO::DeleteBody()
{
	if(!SeldEntList.getNum() && Focus_Body == KOD_ERR)	return;			// セレクション,リストウィンドウ選択されていない場合は何もしない

	int buf=KOD_ERR;
	int delnum[BODYLISTMAX];
	int delcount=0;
	BODY *body;
	OBJECT *obj;


	// セレクションされていないが、リストウィンドウ上では選択されている場合
	if(!SeldEntList.getNum()){
		body = (BODY *)BodyList.getData(Focus_Body);		// リストウィンドウ上で選択されているBODY番号を引っ張ってくる
		DeleteBodySub(body,Focus_Body);						// BODY削除
		Focus_Body = KOD_ERR;								// リストウィンドウでなにもフォーカスされていない状態に戻す
	}

	// セレクションされている場合,
	// セレクションされているエンティティから、どのBODYが選択されているか抽出
	for(int i=0;i<SeldEntList.getNum();i++){
		obj = (OBJECT *)SeldEntList.getData(i);
		if(buf != obj->Body){
			buf = obj->Body;
			delnum[delcount] = obj->Body;
			delcount++;
		}
	}
	BubbleSort(delnum,delcount);

	for(int i=delcount-1;i>=0;i--){
		body = (BODY *)BodyList.getData(delnum[i]);
		DeleteBodySub(body,delnum[i]);
	}

	ClearSeldEntList();

	ReDrawBODYFlag = KOD_FALSE;		// 再描画指令
}

// 選択されているBODYを消去Sub
void KODatUNO::DeleteBodySub(BODY *body,int n)
{
	if (body == NULL) return;

	char mes[256];
	sprintf(mes,"%s was deleted.",body->Name);
	SetMessage(mes);

	BodyList.delPos(n);							// リストからはずす
	Body_List_Win->delPos(n);					// リストウィンドウからはずす
	Free_body(body,body->TypeNum);				// BODY内で確保しているメモリーの解放
	delete (BODY *)body;						// BODY自身のメモリー解放
}

// Open，Deleteボタンの表示属性の変更
void KODatUNO::OpenDelBtn()
{
    //開くボタンの設定
    if(BodyList.getNum() == BODYLISTMAX)//ファイルが8つ開いていたら
        Open_Body_Btn->setSensitive(False); //淡色表示（押せない）
	else
        Open_Body_Btn->setSensitive(True);  //通常表示（押せる）
    
    //削除ボタンの設定
    if(BodyList.getNum() == 0)//ファイルがなかったら
        Del_Body_Btn->setSensitive(False);  //淡色表示（押せない）
    else
        Del_Body_Btn->setSensitive(True);   //通常表示（押せる）
}

// ボディリストでの選択
int KODatUNO::BodyListSelect()
{
	Focus_Body = Body_List_Win->getSelectedPos();	//選択項目の出力

	return KOD_TRUE;
}

// 最適モデルスケールを取得
void KODatUNO::SetModelScale()
{
	BODY *body;
	double max=0;

	if(!BodyList.getNum()) return;

	for(int i=0;i<BodyList.getNum();i++){
		body = (BODY *)BodyList.getData(i);
		if(max < body->MaxCoord)
			max = body->MaxCoord;
	}
	ModelScale = 1/max;						// 初期画面にオブジェクト全体が写るように描画スケールを調節
	ModelScale1st = ModelScale;				// モデルスケールの初期値を覚えておく
}

// BODYの最大長を設定する
void KODatUNO::SetMaxCoord()
{
	BODY *body;

	if(!BodyList.getNum()) return;

	for(int i=0;i<BodyList.getNum();i++){
		body = (BODY *)BodyList.getData(i);
		double max = 0;
		for(int j=0;j<body->TypeNum[_NURBSS];j++){
			for(int u=0;u<body->NurbsS[j].K[0];u++){
				for(int v=0;v<body->NurbsS[j].K[1];v++){
					double d = CalcEuclid(body->NurbsS[j].cp[u][v]);
					if(d > max)	max = d;
				}
			}
		}
		body->MaxCoord = max;
	}
}


// ウィンドウのサイズを取得
void KODatUNO::GetResizeWindow()
{
	ScreenWidth = Describe_Form->getProperty(WSNwidth);		// 幅を取得
	ScreenHeight = Describe_Form->getProperty(WSNheight);	// 高さを取得

	glViewport(0,0,ScreenWidth,ScreenHeight);
}

// OpenGLの初期化
void KODatUNO::SetOpenGLStat()
{
	if(OpenGLInitFlag == KOD_FALSE){
		
		GLfloat diffuse[] = { 0.9, 0.9, 0.9, 1.0 };
		GLfloat specular[] = { 1.0, 1.0, 1.0, 1.0 };
		GLfloat ambient[] = { 1.0, 1.0, 1.0, 1.0 };
		GLfloat mat_diffuse[] = { 0.7, 0.7, 0.7, 1.0 };
		GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
		GLfloat mat_shininess[] = { 100.0 };
		GLfloat mat_ambient[] = { 0.2, 0.2, 0.2, 1.0 };

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// 光の設定
		glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
		glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
		glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);

		// マテリアルの設定
		glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
		glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
		glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_ambient);

		glEnable(GL_LIGHTING);
		glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
		glEnable(GL_LIGHT0);
		glDepthFunc(GL_LESS);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_AUTO_NORMAL);
		glEnable(GL_NORMALIZE);
		glEnable(GL_POINT_SMOOTH);
		glEnable(GL_LINE_SMOOTH);
		glEnable(GL_POLYGON_SMOOTH);

		if(SolidViewFlag == KOD_TRUE)
			SetNurbsSProperty(GLU_DISPLAY_MODE, GLU_FILL);
		else if(WireFlameViewFlag == KOD_TRUE)
			SetNurbsSProperty(GLU_DISPLAY_MODE, GLU_OUTLINE_PATCH);

		glBlendFunc(GL_ONE_MINUS_DST_COLOR, GL_ONE); 

		// glClearColor(1,1,1,0);	// 背景白

		OpenGLInitFlag = KOD_TRUE;
	}

	double a=2;
	double ratio;

	glViewport(0,0,ScreenWidth,ScreenHeight);
	glMatrixMode(GL_PROJECTION);	// 投影変換行列スタックを選択
	glLoadIdentity();				// 投影変換行列スタックの先頭に単位行列を挿入

	// 平行投影を設定(アスペクト比によってモデルの縦横比が変わらないように処理を分ける)
	if(ScreenWidth <= ScreenHeight){
		ratio = (double)ScreenHeight/(double)ScreenWidth;
		glOrtho(-a*ratio,a*ratio,-a,a,-100000,100000);
	}
	else{
		ratio = (double)ScreenWidth/(double)ScreenHeight;
		glOrtho(-a*ratio,a*ratio,-a,a,-100000,100000);
	}

	glMatrixMode(GL_MODELVIEW);						// モデルビュー行列
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();								// モデルビュー行列の先頭に単位行列を挿入
	gluLookAt(0,1,0,0.0,0.0,0.0,0.0,0.0,1.0);		// 視点を設定
	glScaled(ModelScale,ModelScale,ModelScale);		// モデルスケールを設定
	glTranslatef(Trl[0],Trl[1],Trl[2]);				// 座標系を平行移動する
	glMultMatrixd(RotMx);							// モデルビュー行列に変更された回転行列をかけていく
	DrawAxis(AxisScale,ModelScale);					// 座標軸を描画

}

// マウスイベントの処理
// 引数  Trig:トリガーの種類	Btn:どのボタンが押されたか		x,y:マウスの位置
void KODatUNO::MouseEvent(long Trig,long Btn,short x,short y)
{
	// 右クリック
	if((Trig == WSEV_MOUSE_PRESS) && (Btn & WS_MOUSE_BTN3)){
		StartX = x;
		StartY = y;
		RBtnFlag = KOD_TRUE;				// 右ボタンドラッグ中フラグON
	}

	// 真ん中クリック
	if((Trig == WSEV_MOUSE_PRESS) && (Btn & WS_MOUSE_BTN2)){
		MBtnFlag = KOD_TRUE;
	}

	// 左クリック
	if((Trig == WSEV_MOUSE_PRESS) && (Btn & WS_MOUSE_BTN1)){
		StartX = x;
		StartY = y;
		LBtnFlag = KOD_TRUE;
	}

	// ボタンを離した
	else if(Trig == WSEV_MOUSE_RELEASE){
		if(LBtnFlag == KOD_TRUE){			// 左クリックだった場合
			DoSelectFlag = KOD_TRUE;		// オブジェクト選択判別
		}
		StartQ = QFunc.QCopy(TargetQ);		// 現在のクォータニオン値を次の初期値とする
		MBtnFlag = KOD_FALSE;				// 真ん中ボタンドラッグ中フラグOFF
		RBtnFlag = KOD_FALSE;				// 右ボタンドラッグ中フラグOFF
		LBtnFlag = KOD_FALSE;				// 左ボタンドラッグ中フラグOFF
		SweepSelectFlag = KOD_FALSE;		// スイープセレクションフラグOFF
		Describe_Form->execEventProc(WSEV_EXPOSE,NULL);		// 再描画指令
	}	
}

// マウス移動中のイベント処理
void KODatUNO::MouseMotionEvent(long Btn,short x,short y)
{
	// 右ボタンドラッグ中-->回転
	if(Btn & WS_MOUSE_BTN3 && RBtnFlag == KOD_TRUE){
		double dx = (double)(x-StartX)/(double)ScreenWidth;
		double dy = (double)(y-StartY)/(double)ScreenHeight;
		double a = sqrt(dx*dx + dy*dy);						// マウスポインタの位置のドラッグ開始位置からの距離
		Quat dq = QFunc.QGenRot(2*PI*a,-dy/a,0,dx/a);		// 回転クォータニオン生成
		TargetQ = QFunc.QMult(dq, StartQ);					// 回転の初期値StartQにdqを掛けて回転を合成
		QFunc.QtoR(RotMx, TargetQ);							// クォータニオンから回転の変換行列を求める
		Describe_Form->execEventProc(WSEV_EXPOSE,NULL);		// 再描画指令
	}

	// 左ボタンドラッグ中-->ラバーバンド
	if(Btn & WS_MOUSE_BTN1 && LBtnFlag == KOD_TRUE){
		SweepSelectFlag = KOD_TRUE;
		CurrentX = x;
		CurrentY = y;
		Describe_Form->execEventProc(WSEV_EXPOSE,NULL);		// 再描画指令
	}

	// 真ん中ボタンドラッグ中-->平行移動 or Ctrl+真ん中ボタンドラッグ中-->拡大縮小
	else if(Btn & WS_MOUSE_BTN2 && MBtnFlag == KOD_TRUE){
		WSDkeyboard *key = WSGIappKeyboard();					// キーボードのインスタンスを生成
		double dx = (double)(x-OldPosX)/(double)ScreenWidth;	// 移動距離取得
		double dy = (double)(y-OldPosY)/(double)ScreenHeight;
		// Ctrlキーを押していない-->平行移動
		if(!key->withCntl()){
			Trl[0] -= 5*dx/ModelScale;
			Trl[2] -= 5*dy/ModelScale;
		}
		// Ctrlキーを押してる-->拡大縮小
		else{
			double a = 2*sqrt(dx*dx + dy*dy);
			if(x-OldPosX > 0)	ModelScale += a*ModelScale;		// マウスを右へ動かしたら拡大
			else				ModelScale -= a*ModelScale;		// マウスを左へ動かしたら縮小
			if(ModelScale < 0.0001) ModelScale = 0.0001;		// 縮小スケールの最小値
		}	
		Describe_Form->execEventProc(WSEV_EXPOSE,NULL);		// 再描画指令
		ReDrawBODYFlag = KOD_FALSE;							// 再描画指令
	}

	OldPosX = x;
	OldPosY = y;
}



// セレクション(マウスピッキング)の設定
void KODatUNO::DoSelect(int x,int y)
{
	int hits;
	double ratio;
	double a=2;

    glGetIntegerv(GL_VIEWPORT,Vp);
	glSelectBuffer(MAXSELECT, SelectBuf);	// セレクションされたオブジェクトの情報を入れておくバッファを用意
	glRenderMode(GL_SELECT);				// セレクションモードに移行
	glInitNames();							// オブジェクトにつける名前(整数値)を登録しておくネームスタックを初期化

	glMatrixMode(GL_PROJECTION);			// 射影変換モード
	glPushMatrix();							// これまでの変換マトリックスを保存
	glLoadIdentity();						// 射影変換マトリックスを初期化

	// 通常のセレクションの場合
	if(StartX == x && StartY == y){
		gluPickMatrix((GLdouble)x,(GLdouble)(Vp[3] - y),5.0,5.0,Vp);	// 表示領域がマウスポインタの周囲だけになるように射影変換行列を設定
	}
	// スイープセレクションの場合
	else{
		gluPickMatrix((GLdouble)((StartX+x)/2),(GLdouble)((Vp[3]*2-y-StartY)/2),fabs((double)(x-StartX)),fabs((double)(StartY-y)),Vp);	// 表示領域がラバーバンド内になるように射影変換行列を設定
	}

	if(Vp[2] <= Vp[3]){
		ratio = (double)Vp[3]/(double)Vp[2];
		glOrtho(-a*ratio,a*ratio,-a,a,-100000,100000);
	}
	else{
		ratio = (double)Vp[2]/(double)Vp[3];
		glOrtho(-a*ratio,a*ratio,-a,a,-100000,100000);
	}

	glMatrixMode(GL_MODELVIEW);					// モデル変換モードに移行
	glLoadIdentity();							// モデルビュー行列の先頭に単位行列を挿入
	gluLookAt(0,1,0.0,0.0,0.0,0.0,0.0,0.0,1.0);	// 視点を設定
	glScaled(ModelScale,ModelScale,ModelScale);	// モデルスケールを設定
	glTranslatef(Trl[0],Trl[1],Trl[2]);			// 座標系を平行移動する
	glMultMatrixd(RotMx);						// モデルビュー行列に変更された回転行列をかけていく
	if(DrawBODYFlag == KOD_TRUE)				// BODY描画フラグがONなら
		glCallList(COMMAND_DRAW_BODY);			// 各エンティティで階層化されたセレクション用番号を得る(あらかじめ登録しておいたコマンドを実行)
	glMatrixMode(GL_PROJECTION);				// 射影変換モードに戻る
	glPopMatrix();								// 射影変換マトリクスを元に戻す
	hits = glRenderMode(GL_RENDER);				// セレクションモード終了、レンダリングモードに戻る

	if(StartX == x && StartY == y)				// クリックで選択した場合
		ClickPicking(SelectBuf,hits);
	else
		DragPicking(SelectBuf,hits);			// ドラッグで選択した場合
}

// クリックによるマウスピッキング
void KODatUNO::ClickPicking(GLuint SelectBuf[],int hits)
{
	int objnum=0;

	objnum = ObjSelect(SelectBuf,hits);			// ヒットしたオブジェクトの中でデプス値の一番小さいオブジェクトを選択する


	// 以前にセレクトされたものを再セレクトした場合はカウントしない
	for(int i=0;i<SeldEntList.getNum();i++){
		OBJECT *obj;
		obj = (OBJECT *)SeldEntList.getData(i);
		if(obj->Body == SelectBuf[objnum*(3+SelectBuf[0])+3] &&
			obj->Type == SelectBuf[objnum*(3+SelectBuf[0])+4] &&
			obj->Num == SelectBuf[objnum*(3+SelectBuf[0])+5]){
			hits = 0;
		}
	}
	
	// 新しいオブジェクトにヒットした場合
	if(hits >= 1)
		SetNewObject(SelectBuf[objnum*(3+SelectBuf[0])+3],SelectBuf[objnum*(3+SelectBuf[0])+4],SelectBuf[objnum*(3+SelectBuf[0])+5]);
}

// ドラッグによるマウスピッキング
void KODatUNO::DragPicking(GLuint SelectBuf[],int hits)
{
	int temp=0;	// 再セレクト判別用カウンタ

	// 範囲選択によってオブジェクトにヒットした場合
	for(int i=0;i<hits;i++){
		for(int j=0;j<SeldEntList.getNum();j++){
			OBJECT *obj = (OBJECT *)SeldEntList.getData(j);
			if(obj != NULL && obj->Body == SelectBuf[i*(3+SelectBuf[0])+3] &&
				obj->Type == SelectBuf[i*(3+SelectBuf[0])+4] &&
				obj->Num == SelectBuf[i*(3+SelectBuf[0])+5]){
					temp++;
			}
		}
		// オブジェクトが新しくヒットした場合
		if(temp == 0)
			SetNewObject(SelectBuf[i*(3+SelectBuf[0])+3],SelectBuf[i*(3+SelectBuf[0])+4],SelectBuf[i*(3+SelectBuf[0])+5]);

		temp = 0;
	}
}

// ピックされたオブジェクトをOBJECTリストに登録
void KODatUNO::SetNewObject(int BodyNum,int TypeNum,int NumNum)
{
	OBJECT *obj = new OBJECT;
	obj->Body = BodyNum;
	obj->Type = TypeNum;
	obj->Num = NumNum;
	SeldEntList.add((void *)obj);							// セレクトされたエンティティ情報をリストに登録
	BODY *body;
	body = (BODY *)BodyList.getData(obj->Body);
	if(obj->Type == _NURBSC){
		ChangeStatColor(body->NurbsC[obj->Num].Dstat.Color,1,0.2,1,0.5);	// 選択済みを示すため色を変更
	}
	else if(obj->Type == _TRIMMED_SURFACE || obj->Type == _NURBSS){
		ChangeStatColor(body->NurbsS[obj->Num].Dstat.Color,0.3,0.1,0.2,0.5);	// 選択済みを示すため色を変更
	}

}

// 全てセレクト状態にする
void KODatUNO::SelectAll()
{
	BODY *body;

	for(int i=0;i<BodyList.getNum();i++){					// 現在リストに登録されているBODYの数だけループ
		if((body = (BODY *)BodyList.getData(i)) != NULL){	// i番目のリストに登録されているBODYのデータを取得
			// NURBSCを選択済みにする操作
			for(int j=0;j<body->TypeNum[_NURBSC];j++){
				// 現在注目中の要素が既に選択済みかを判別
				bool flag = false;
				for(int k=0;k<SeldEntList.getNum();k++){
					OBJECT *obj_ = (OBJECT *)SeldEntList.getData(k);
					if(obj_ != NULL && obj_->Body == i && obj_->Type == _NURBSC && obj_->Num == j){
						flag = true;
						break;
					}
				}
				// まだ選択されていなかったら
				if(flag == false){
					SetNewObject(i,_NURBSC,j);
				}
			}

			// _TRIMMED_SURFACEを選択済みにする操作
			for(int j=0;j<body->TypeNum[_TRIMMED_SURFACE];j++){
				// 現在注目中の要素が既に選択済みかを判別
				bool flag = false;
				for(int k=0;k<SeldEntList.getNum();k++){
					OBJECT *obj_ = (OBJECT *)SeldEntList.getData(k);
					if(obj_ != NULL && obj_->Body == i && obj_->Type == _TRIMMED_SURFACE && obj_->Num == j){
						flag = true;
						break;
					}
				}
				// まだ選択されていなかったら
				if(flag == false)
					SetNewObject(i,_TRIMMED_SURFACE,j);
			}
			// _NURBSSを選択済みにする操作
			for(int j=0;j<body->TypeNum[_NURBSS];j++){
				bool flag = false;
				// トリムド曲面として選択済みの場合は何もしない
				for(int k=0;k<body->TypeNum[_TRIMMED_SURFACE];k++){
					if(body->TrmS[j].pts == &body->NurbsS[k]){
						flag = true;
						break;
					}
				}
				// トリムド曲面でない場合は、既に選択済みかを調べる
				if(flag == false){
					for(int k=0;k<SeldEntList.getNum();k++){
						OBJECT *obj_ = (OBJECT *)SeldEntList.getData(k);
						if(obj_ != NULL && obj_->Body == i && obj_->Type == _NURBSS && obj_->Num == j){
							flag = true;
							break;
						}
					}
				}
				// まだ選択されていなかったら
				if(flag == false)
					SetNewObject(i,_NURBSS,j);
			}
		}
	}

	ReDrawBODYFlag = KOD_FALSE;		// 描画メモリリストを再設定
}

// セレクションキャンセル
void KODatUNO::SelectionCancel()
{
	BODY *body;
	OBJECT *obj;

	for(int i=0;i<SeldEntList.getNum();i++){
		obj = (OBJECT *)SeldEntList.getData(i);
		body = SearchBodyList(&BodyList,obj->Body);			// セレクションされているエンティティが属するBODY番号を調べる
		if (body == NULL) continue;							// DeleteBodyで消去されていた場合
		if(obj->Type == _NURBSC){
			InitCurveColor(body->NurbsC[obj->Num].Dstat.Color);		// 選択解除を示すため色を元に戻す
		}
		else if(obj->Type == _TRIMMED_SURFACE || obj->Type == _NURBSS){
			InitSurfaceColor(body->NurbsS[obj->Num].Dstat.Color);	// 選択解除を示すため色を元に戻す
		}
	}
	ClearSeldEntList();				// セレクションリストをクリア
	InitCmdFlags();					// Cmd用フラグを全てKOD_FALSEに初期化
	ReDrawBODYFlag = KOD_FALSE;		// 描画メモリリストを再設定
}

// オブジェクトの選択判別
int KODatUNO::ObjSelect(GLuint SelectBuf[],int hits)
{
	int i;
	int ZdepthMin=0;
	double Zdepth[MAXSELECT];
	double depthbuf[MAXSELECT];
	double temp=0;

	// ヒットした全てのオブジェクトのデプス値を得る
	for(i=0;i<hits;i++){
		Zdepth[i] = ((((double)SelectBuf[i*(3+SelectBuf[0])+1]/0xFFFFFFFF)+((double)SelectBuf[i*(3+SelectBuf[0])+2]/0xFFFFFFFF))/2.0);	// デプスバッファの最大値と最小値の中間の値
		depthbuf[i] = Zdepth[i];				// デプス値のソート処理用のバッファを用意
	}
	
	// ヒットしたオブジェクトの中でデプス値が最小のものを求める
	if(hits == 1)
		ZdepthMin = 0;
	else if(hits > 1)
		BubbleSort(depthbuf,hits);		// バブルソートによりデプス値が最小のものを得る
	
	// デプス値が最小のものが複数あった場合、面より線を優先的に選択する
	for(i=0;i<hits;i++){
		if(Zdepth[i] == depthbuf[0] && SelectBuf[i*(3+SelectBuf[0])+4] == _NURBSC){
			ZdepthMin = i;
			temp++;
			break;
		}
	}
	
	// ヒットした中に線がない場合
	if(temp == 0){
		for(i=0;i<hits;i++){
			if(Zdepth[i] == depthbuf[0])
				ZdepthMin = i;
		}
	}

	return ZdepthMin;
}

// X軸上からの視点に切り替える
void KODatUNO::ChangeViewX()
{
	StartQ = QFunc.QInit(1,0,0,0);		// 今の回転角情報を初期化
	QFunc.QtoR(RotMx,StartQ);			// 今の回転角情報を初期化
	TargetQ = QFunc.QMult(QFunc.QGenRot(PI/2,0,0,1), StartQ);	// クォータニオン上で初期位置に対してZ軸回りに90°回転
	QFunc.QtoR(RotMx,TargetQ);			// 回転行列に代入
	StartQ = QFunc.QCopy(TargetQ);
	Trl[0] = Trl[1] = Trl[2] = 0;		// 平行移動はリセット
}

// Y軸上からの視点に切り替える
void KODatUNO::ChangeViewY()
{
	StartQ = QFunc.QInit(1,0,0,0);
	QFunc.QtoR(RotMx,StartQ);
	Trl[0] = Trl[1] = Trl[2] = 0;
}

// Z軸上からの視点に切り替える
void KODatUNO::ChangeViewZ()
{
	StartQ = QFunc.QInit(1,0,0,0);
	QFunc.QtoR(RotMx,StartQ);
	TargetQ = QFunc.QMult(QFunc.QGenRot(PI/2,1,0,0), StartQ);	// クォータニオン上で初期位置に対してX軸回りに90°回転
	QFunc.QtoR(RotMx,TargetQ);	
	StartQ = QFunc.QCopy(TargetQ);
	Trl[0] = Trl[1] = Trl[2] = 0;		// 平行移動はリセット
}

// 鳥瞰視点に切り替える
void KODatUNO::ChangeViewXYZ()
{
	StartQ = QFunc.QInit(1,0,0,0);
	QFunc.QtoR(RotMx,StartQ);
	TargetQ = QFunc.QMult(QFunc.QGenRot(PI/6,0,0,1), StartQ);		// Z軸回りに30°
	TargetQ = QFunc.QMult(QFunc.QGenRot(-PI/10,1,0,0), TargetQ);	// X軸回りに18°
	QFunc.QtoR(RotMx,TargetQ);	
	StartQ = QFunc.QCopy(TargetQ);
	Trl[0] = Trl[1] = Trl[2] = 0;		// 平行移動はリセット
}

// Ctrlキー＋上矢印キーが押された場合のBODY回転
WSCbool KODatUNO::ChangeViewUp()
{
	if(!WSGIappKeyboard()->withCntl())	return True;	// Ctrlキーが押されていない場合は何もしない

	Quat dq = QFunc.QGenRot(PI/12,1,0,0);				// 回転クォータニオン生成
	TargetQ = QFunc.QMult(dq, StartQ);					// 回転の初期値StartQにdqを掛けて回転を合成
	QFunc.QtoR(RotMx, TargetQ);							// クォータニオンから回転の変換行列を求める
	StartQ = QFunc.QCopy(TargetQ);						// 更新
	Describe_Form->execEventProc(WSEV_EXPOSE,NULL);		// 再描画指令

	return False;
}

// Ctrlキー＋下矢印キーが押された場合のBODY回転
WSCbool KODatUNO::ChangeViewDown()
{
	if(!WSGIappKeyboard()->withCntl())	return True;	// Ctrlキーが押されていない場合は何もしない

	Quat dq = QFunc.QGenRot(PI/12,-1,0,0);				// 回転クォータニオン生成
	TargetQ = QFunc.QMult(dq, StartQ);					// 回転の初期値StartQにdqを掛けて回転を合成
	QFunc.QtoR(RotMx, TargetQ);							// クォータニオンから回転の変換行列を求める
	StartQ = QFunc.QCopy(TargetQ);						// 更新
	Describe_Form->execEventProc(WSEV_EXPOSE,NULL);		// 再描画指令

	return False;
}


// Ctrlキー＋左矢印キーが押された場合のBODY回転
WSCbool KODatUNO::ChangeViewLeft()
{
	if(!WSGIappKeyboard()->withCntl())	return True;	// Ctrlキーが押されていない場合は何もしない

	Quat dq = QFunc.QGenRot(PI/12,0,0,-1);				// 回転クォータニオン生成
	TargetQ = QFunc.QMult(dq, StartQ);					// 回転の初期値StartQにdqを掛けて回転を合成
	QFunc.QtoR(RotMx, TargetQ);							// クォータニオンから回転の変換行列を求める
	StartQ = QFunc.QCopy(TargetQ);						// 更新
	Describe_Form->execEventProc(WSEV_EXPOSE,NULL);		// 再描画指令

	return False;
}


// Ctrlキー＋右矢印キーが押された場合のBODY回転
WSCbool KODatUNO::ChangeViewRight()
{
	if(!WSGIappKeyboard()->withCntl())	return True;	// Ctrlキーが押されていない場合は何もしない

	Quat dq = QFunc.QGenRot(PI/12,0,0,1);				// 回転クォータニオン生成
	TargetQ = QFunc.QMult(dq, StartQ);					// 回転の初期値StartQにdqを掛けて回転を合成
	QFunc.QtoR(RotMx, TargetQ);							// クォータニオンから回転の変換行列を求める
	StartQ = QFunc.QCopy(TargetQ);						// 更新
	Describe_Form->execEventProc(WSEV_EXPOSE,NULL);		// 再描画指令

	return False;
}


// 倍率をデフォルトに戻す
void KODatUNO::FitView()
{
	ModelScale = ModelScale1st;
}

// BODY描画形式をSolid表示に変更
void KODatUNO::SolidView()
{
	DrawBODYFlag = KOD_TRUE;		// BODY描画フラグON
	EdgeViewFlag = KOD_TRUE;		// エッジ描画フラグON
	SolidViewFlag = KOD_TRUE;		// ソリッド描画フラグON
	WireFlameViewFlag = KOD_FALSE;	// ワイヤーフレーム描画フラグOFF

	ReDrawBODYFlag = KOD_FALSE;		// BODY描画メモリリスト再設定フラグON
	OpenGLInitFlag = KOD_FALSE;		// OpenGLステータス再設定フラグON
}

// Edgeを非表示にする
void KODatUNO::NoEdgeView()
{
	DrawBODYFlag = KOD_TRUE;		// BODY描画フラグON
	EdgeViewFlag = KOD_FALSE;		// エッジ描画フラグOFF
	SolidViewFlag = KOD_TRUE;		// ソリッド描画フラグON
	WireFlameViewFlag = KOD_FALSE;	// ワイヤーフレーム描画フラグOFF

	ReDrawBODYFlag = KOD_FALSE;		// BODY描画メモリリスト再設定フラグON
	OpenGLInitFlag = KOD_FALSE;		// OpenGLステータス再設定フラグON
}

// BODY描画形式をWireFlame表示に変更
void KODatUNO::WireFlameView()
{
	DrawBODYFlag = KOD_TRUE;		// BODY描画フラグON
	EdgeViewFlag = KOD_TRUE;		// エッジ描画フラグON
	WireFlameViewFlag = KOD_TRUE;	// ワイヤーフレーム描画フラグON
	SolidViewFlag = KOD_FALSE;		// ソリッド描画フラグOFF

	ReDrawBODYFlag = KOD_FALSE;		// BODY描画メモリリスト再設定フラグON
	OpenGLInitFlag = KOD_FALSE;		// OpenGLステータス再設定
}

// BODY非表示化
void KODatUNO::VoidView()
{
	DrawBODYFlag = KOD_FALSE;		// BODY描画フラグOFF
	EdgeViewFlag = KOD_FALSE;		// エッジ描画フラグON
	WireFlameViewFlag = KOD_FALSE;	// ワイヤーフレーム描画フラグOFF
	SolidViewFlag = KOD_FALSE;		// ソリッド描画フラグOFF
}

// BODYを半透明にする
void KODatUNO::ViewBlending()
{
	BlendViewFlag = KOD_TRUE - BlendViewFlag;
}

// セレクションリスト及びOBJECTの初期化
void KODatUNO::ClearSeldEntList()
{
	OBJECT *obj;
	int objnum = SeldEntList.getNum();

	for(int i=0;i<objnum;i++){
		obj = (OBJECT *)SeldEntList.getData(i);
		delete obj;
	}
	SeldEntList.clear();
}


// Cmd用フラグを全てKOD_FALSEに初期化
void KODatUNO::InitCmdFlags()
{
	UVWireFlameViewFlag = KOD_FALSE;
	CPViewFlag = KOD_FALSE;
}

// UserFuncを実行
void KODatUNO::ExecuteUserFuncMain(int val)
{
	for(int i=0;i<USERFUNCNUMMAX;i++){
		if(i == val){
			AddEntSymbolToObj();				// OBJECT構造体に何番目の曲線or曲面かの情報を付加する
			User.ExecWSFunc(val);				// User関数内でDialog操作を行いたい場合はこの関数を通して行われる
			ExecUserFuncFlag[i] = KOD_TRUE;
			return;
		}
	}

}

// UserCommand実行時の再描画呼び出し(OpenGL描画ループ内)
void KODatUNO::ReDrawUserCommand()
{
	for(int i=0;i<USERCOMMANDNUM;i++){
		if(ExecUserCommandFlag[i] == KOD_TRUE){
			ExecUserCommandFlag[i] = KOD_DONE;
			glNewList(COMMAND_DRAW_USER_COMMAND+i,GL_COMPILE_AND_EXECUTE);		// User関数iをメモリーリストに登録
			User.Command(&BodyList,&SeldEntList,SeldEntList.getNum(),i+SYSCOMMANDNUM,argc,argv);	// Userコマンドを実行
			glEndList();												// メモリーリスト登録終了
			//ReDrawBODYFlag = KOD_FALSE;		// BODY描画メモリリスト再設定フラグON
			//Describe_Form->redraw();		// 描画領域に再描画を指示
			OpenDelBtn();					// Delボタン属性の変更
			return;
		}
	}

	// 作成したUser関数のメモリーリストを呼び出す
	for(int i=0;i<USERCOMMANDNUM;i++){
		if(ExecUserCommandFlag[i] == KOD_DONE){			// User関数i実行済みフラグがONなら
			glCallList(COMMAND_DRAW_USER_COMMAND+i);			// User関数iのメモリーリストを呼び出す(User関数i内で呼び出されたOpenGL描画を実行)
		}
	}
}

// UserFunc実行時の再描画呼び出し(OpenGL描画ループ内)
void KODatUNO::ReDrawUserFunc()
{
	for(int i=0;i<USERFUNCNUMMAX;i++){
		if(ExecUserFuncFlag[i] == KOD_TRUE){
			ExecUserFuncFlag[i]  = KOD_DONE;
			glNewList(COMMAND_DRAW_USER+i,GL_COMPILE_AND_EXECUTE);		// User関数iをメモリーリストに登録
			int (USER::*userfn)(BODYList *,OBJECTList *,int);	
			userfn = User.UserFunc[i];
			ExecUserFuncFlag[i] = (User.*userfn)(&BodyList,&SeldEntList,SeldEntList.getNum());		// User関数iを実行()
			if(ExecUserFuncFlag[i]  != KOD_FALSE && ExecUserFuncFlag[i]  != KOD_ERR){
				ExecUserFuncFlag[i]  = KOD_DONE;
			}
			glEndList();												// メモリーリスト登録終了
			//ReDrawBODYFlag = KOD_FALSE;	// BODY描画メモリリスト再設定フラグON
			//Describe_Form->redraw();		// 描画領域に再描画を指示
			OpenDelBtn();					// Delボタン属性の変更
			return;
		}
	}

	// 作成したUser関数のメモリーリストを呼び出す
	for(int i=0;i<USERFUNCNUMMAX;i++){
		if(ExecUserFuncFlag[i] == KOD_DONE){			// User関数i実行済みフラグがONなら
			glCallList(COMMAND_DRAW_USER+i);			// User関数iのメモリーリストを呼び出す(User関数i内で呼び出されたOpenGL描画を実行)
		}
	}

}

// UserStatusの設定
void KODatUNO::AcceptUserStat(int mode,double prop[])
{
	User.SetUserStatus(mode,prop);	// ダイアログからの値をUserクラスのUserStat構造体に格納
}

// UserFuncによって描画された画像を消す
void KODatUNO::UserViewCanncel()
{
	for(int i=0;i<USERFUNCNUMMAX;i++)
		ExecUserFuncFlag[i] = KOD_FALSE;
	for(int i=0;i<USERCOMMANDNUM;i++)
		ExecUserCommandFlag[i] = KOD_FALSE;

	User.ExecdClear();	// UserクラスのExecdClear()を呼び出す

	InitCmdFlags();
}


// OBJECT構造体に何番目の曲線or曲面かの情報を付加する
int KODatUNO::AddEntSymbolToObj()
{
	OBJECT *obj;
	int Cnum = 0;
	int Snum = 0;

	for(int i=0;i<SeldEntList.getNum();i++){
		obj = (OBJECT *)SeldEntList.getData(i);
		if(obj->Type == _NURBSC){
			obj->CCount = Cnum;
			Cnum++;
		}
		else if(obj->Type == _TRIMMED_SURFACE || obj->Type == _NURBSS){
			obj->SCount = Snum;
			Snum++;
		}
		else{
			SetMessage("ERROR:Unsupported entity was detected.");
			return KOD_ERR;
		}
	}

	return KOD_TRUE;
}


// コンソールからのコマンドを実行
void KODatUNO::ExecCommand(char *allcmdline)
{
	argc = AnalCommandLine(allcmdline,argv);		// コマンドラインを解析し、argc,argvを得る
	if(argc == KOD_ERR)								// 何も入力されていなかったら
		return;

	int cmdno = SearchRegdCmdNum(CmdMap,argv[0]);	// コマンドの登録番号を調べる
	if(cmdno == KOD_ERR){
		SetMessage("No such command name!");
		return;
	}

	if(cmdno<SYSCOMMANDNUM)
		CmdMap[cmdno].Command(argc,argv);			// 登録番号を判別し、システムコマンドを実行

	else if(cmdno >= SYSCOMMANDNUM && cmdno < COMMANDNUMMAX)
		ExecUserCommandFlag[cmdno-SYSCOMMANDNUM] = KOD_TRUE;
		//User.Command(&BodyList,&SeldEntList,SeldEntList.getNum(),cmdno,argc,argv);

	Describe_Form->redraw();		// 描画領域に再描画を指示
}

// UVパラメータで分割されたワイヤーフレームを表示
void KODatUNO::UVWireView()
{
	if(!BodyList.getNum())	return;

	BODY *body;
	OBJECT *obj;
	NURBSS *NurbsS;
	NURBS_Func NFunc;
	Coord p;
	glDisable(GL_LIGHTING);
	glLineWidth(1);
	glColor3f(0.5,0.5,0.5);

	for(int pnum=0;pnum<SeldEntList.getNum();pnum++){
		obj = (OBJECT *)SeldEntList.getData(pnum);
		body = (BODY *)BodyList.getData(obj->Body);
		if(obj->Type != _NURBSS && obj->Type != _TRIMMED_SURFACE)	continue;
		int num;
		if(body->NurbsS[0].TrmdSurfFlag == KOD_TRUE)
			num = body->TypeNum[_TRIMMED_SURFACE];
		else
			num = body->TypeNum[_NURBSS];
		for(int i=0;i<num;i++){
			if(body->NurbsS[0].TrmdSurfFlag == KOD_TRUE)
				NurbsS = body->TrmS[i].pts;
			else
				NurbsS = &body->NurbsS[i];
			for(int j=0;j<11;j++){
				glBegin(GL_LINE_STRIP);
				for(int k=0;k<51;k++){		// v方向パラメータライン描画
					p = NFunc.CalcNurbsSCoord(NurbsS,0.1*(double)j,0.02*(double)k);
					glVertex3d(p.x,p.y,p.z);
				}
				glEnd();
				glBegin(GL_LINE_STRIP);
				for(int k=0;k<51;k++){		// u方向パラメータライン描画
					p = NFunc.CalcNurbsSCoord(NurbsS,0.02*(double)k,0.1*(double)j);
					glVertex3d(p.x,p.y,p.z);
				}
				glEnd();
			}
		}
	}
	glEnable(GL_LIGHTING);
}

// モデルスケールをセット
void KODatUNO::SetModelScale(double scale)
{
	if(scale == 0) return;
	ModelScale = scale;
}

// モデルスケールを取得
void KODatUNO::GetModelScale()
{
	char mes[256];
	sprintf(mes,"%.6lf",ModelScale);
	SetConsoleCharNum(SetMessage(mes));
}

// トレランス値をセット
void KODatUNO::SetTolerance(double val)
{
	if (val == 0) return;

	Tolerance = val;
	ReDrawBODYFlag = KOD_FALSE;		// 再描画指令
	Describe_Form->redraw();
}

// トレランス値を取得
void KODatUNO::GetTolerance()
{
	char mes[256];
	sprintf(mes,"%.6lf",Tolerance);
	SetConsoleCharNum(SetMessage(mes));	// トレランス値を表示、コンソール文字数更新
}

// BODYを平行移動させる
void KODatUNO::GetShiftBody(Coord d)
{
	if(!BodyList.getNum())	return;

	BODY *body;
	OBJECT *obj;	
	NURBS_Func NFunc;

	obj = (OBJECT *)SeldEntList.getData(0);
	body = (BODY *)BodyList.getData(obj->Body);
	ShiftBody(body,d);
	
	DrawBODYFlag = KOD_TRUE;				// BODY描画してもOKフラグON
	ReDrawBODYFlag = KOD_FALSE;				// BODYのメモリリストを再取得
}

// BODYを回転させる
void KODatUNO::GetRotateBody(Coord ax,double d)
{
	if(!BodyList.getNum())	return;

	BODY *body;
	OBJECT *obj;	
	NURBS_Func NFunc;

	obj = (OBJECT *)SeldEntList.getData(0);
	body = (BODY *)BodyList.getData(obj->Body);
	RotBody(body,ax,d);

	DrawBODYFlag = KOD_TRUE;				// BODY描画してもOKフラグON
	ReDrawBODYFlag = KOD_FALSE;				// BODYのメモリリストを再取得
}

// BODYを拡大
void KODatUNO::ExpandBody(Coord r)
{
	NURBS_Func NFunc;
	OBJECT *obj;
	BODY *body;

	for(int i=0;i<SeldEntList.getNum();i++){					// セレクションされている全てのエンティティに対して
		obj = (OBJECT *)SeldEntList.getData(i);					// そのセレクションされているエンティティ情報を得る
		body = SearchBodyList(&BodyList,obj->Body);				// BODYの実体を得る
		if(obj->Type == _NURBSC){								// エンティティがNURBS曲線なら
			NFunc.ChRatioNurbsC(&body->NurbsC[obj->Num],r);		// NURBS曲線の拡大
		}
		else if(obj->Type == _NURBSS){
			NFunc.ChRatioNurbsS(&body->NurbsS[obj->Num],r);
		}
		else if(obj->Type == _TRIMMED_SURFACE){
			NFunc.ChRatioNurbsS(body->TrmS[obj->Num].pts,r);
		}
	}

	DrawBODYFlag = KOD_TRUE;				// BODY描画してもOKフラグON
	ReDrawBODYFlag = KOD_FALSE;				// BODYのメモリリストを再取得
}


// 各種サーフェスを生成
int KODatUNO::GenSurface(Coord Axis,double Prop,int Flag)
{
	NURBS_Func NFunc;
	OBJECT *obj;
	BODY *body,*newbody;

	for(int i=0;i<SeldEntList.getNum();i++){										// セレクションされている全てのエンティティに対して
		obj = (OBJECT *)SeldEntList.getData(i);										// そのセレクションされているエンティティ情報を得る
		if(obj->Type == _NURBSC){													// エンティティがNURBS曲線なら
			body = SearchBodyList(&BodyList,obj->Body);								// そのNURBS曲線が属しているBODYの実体を得る
			newbody = new BODY;														// 新しく生成する回転サーフェス用のBODYをメモリー確保
			Init_body(newbody);														// newbody初期化
			newbody->NurbsS = (NURBSS *)malloc(sizeof(NURBSS));						// NURBS曲面を1つメモリー確保
			if(Flag == ROTSURF){
				if(NFunc.GenRotNurbsS(newbody->NurbsS,body->NurbsC[obj->Num],Axis,Prop) == KOD_ERR)	// 回転サーフェス生成
					goto EXIT;
			}
			else if(Flag == SWEEPSURF){
				if(NFunc.GenSweepNurbsS(newbody->NurbsS,body->NurbsC[obj->Num],Axis,Prop) == KOD_ERR)	// スイープサーフェス生成
					goto EXIT;
			}

			newbody->NurbsS[0].TrmdSurfFlag = KOD_FALSE;							// トリムのない単純なNURBS曲面であることを明示
			newbody->TypeNum[_NURBSS] = 1;											// NURBS曲面の数1にする
			ChangeStatColor(newbody->NurbsS[0].Dstat.Color,0.2,0.2,1.0,0.5);		// 青色
			BodyList.add((void *)newbody);											// リストにnewbodyを登録
			Body_List_Win->addItem("NewBody");										// Bodyリストウィンドウにもnewbodyを登録
			strcpy(newbody->Name,"NewBody");										// とりあえずnewbodyの名前は"NewBody"としておく
		}
	}
	DrawBODYFlag = KOD_TRUE;				// BODY描画してもOKフラグON
	ReDrawBODYFlag = KOD_FALSE;				// BODYのメモリリストを再取得

	return KOD_TRUE;

EXIT:
	free(newbody->NurbsS);
	delete newbody;
	return KOD_ERR;
}

// NURBS曲線を生成
int KODatUNO::GenNurbsCurve(int Val,char *Fname,int M)
{
	if(M < 2){
		SetMessage("NURBS ERROR: Few Rank. You should set rank more than 2.");
		return KOD_ERR;
	}

	NURBS_Func NFunc;
	FILE *fp;
	char buf[256];
	Coord pt[CTLPNUMMAX];
	int ptnum=0;
	BODY *newbody;

	// ファイル読込
	if((fp = fopen(Fname,"r")) == NULL){
		SetMessage("KODATUNO ERROR: fail to open file");
		return KOD_ERR;
	}
	while(fgets(buf,sizeof(buf),fp) != NULL){
		if(buf[0] == '#' || buf[0] == '\n')	continue;
		sscanf(buf,"%lf,%lf,%lf",&pt[ptnum].x,&pt[ptnum].y,&pt[ptnum].z);
		ptnum++;
		if(ptnum == CTLPNUMMAX){
			sprintf(buf,"KODATUNO ERROR: Too many points.  Reduce point number to %d or fewer.",CTLPNUMMAX);
			SetMessage(buf);
			return KOD_ERR;
		}
	}
	fclose(fp);

	// NURBS曲線生成
	NURBSC nurb;
	switch(Val){
		case 1:
			NFunc.GenPolygonalLine(&nurb,pt,ptnum);	// 折れ線
			break;
		case 2:
			if(NFunc.GenInterpolatedNurbsC1(&nurb,pt,ptnum,M) != KOD_TRUE)	// 点列補間NURBS曲線
				goto EXIT;
			break;
		case 3:
			if(NFunc.GenApproximationNurbsC(&nurb,pt,ptnum,M) != KOD_TRUE)	// 点列近似NURBS曲線
				goto EXIT;
			break;
		case 4:
			if(NFunc.GenInterpolatedNurbsC2(&nurb,pt,ptnum,M) != KOD_TRUE)	// 点列補間NURBS曲線
				goto EXIT;
			break;			
		case 5:
			if(NFunc.GenNurbsCfromCP(&nurb,pt,ptnum,M) != KOD_TRUE)				// コントロールポイントからNURBS曲線を生成
				goto EXIT;
			break;
		default:
			goto EXIT;
	}

	// 登録
	newbody = new BODY;
	RegistNurbsCtoBody(&BodyList,newbody,nurb,"NewBody_C");	// 新たなNURBS曲線を新たなBODYとして登録
	DrawBODYFlag = KOD_TRUE;				// BODY描画してもOKフラグON
	ReDrawBODYFlag = KOD_FALSE;				// BODYのメモリリストを再取得
	OpenDelBtn();							// Delボタン属性の変更

	return KOD_TRUE;

EXIT:
	return KOD_ERR;
}

// NURBS曲面を生成
// 引数
// Val:曲面の種類(1:折れ面 2:点列補間NURBS曲面)
// *Fname:ファイル名   M:階数
int KODatUNO::GenNurbsSurface(int Val,char *Fname,int M)
{
	if(M < 2 && Val != 1){
		SetMessage("NURBS ERROR: Few Rank. You should set Rank more than 2.");
		return KOD_ERR;
	}

	NURBS_Func NFunc;
	FILE *fp;
	char buf[256];
	Coord **pt;
	int row,col;
	int flag=KOD_FALSE;
	int ptnum=0;
	int i=0,j=0;
	BODY *newbody;

	// ファイル読込
	if((fp = fopen(Fname,"r")) == NULL){
		SetMessage("KODATUNO ERROR: fail to open file");
		return KOD_ERR;
	}

	// ファイルから点列の座標を得る
	while(fgets(buf,sizeof(buf),fp) != NULL){
		if(buf[0] == '#' || buf[0] == '\n')	continue;
		if(!flag){
			sscanf(buf,"%d,%d",&row,&col);
			if((pt = NewCoord2(row,col)) == NULL){		// 点を格納する領域のメモリー確保
				SetMessage("KODATUNO ERROR: fail to allocate memory");
				return KOD_ERR;
			}
			flag = KOD_TRUE;
		}
		else{
			sscanf(buf,"%lf,%lf,%lf",&pt[i][j].x,&pt[i][j].y,&pt[i][j].z);
			j++;
			if(j == col){
				i++;
				j = 0;
			}
			ptnum++;
			if(ptnum > PTNUMMAX){
				sprintf(buf,"KODATUNO ERROR: Too many points (%d).  Reduce point number to %d or fewer.",ptnum,PTNUMMAX);
				SetMessage(buf);
				FreeCoord2(pt,col);
				return KOD_ERR;
			}
		}
	}
	fclose(fp);

	// NURBS曲面生成
	NURBSS nurb;
	switch(Val){
		case 1:
			NFunc.GenPolygonalSurface(&nurb,pt,row,col);	// 折れ面
			break;
		case 2:
			if(NFunc.GenInterpolatedNurbsS1(&nurb,pt,row,col,M,M) == KOD_ERR)	// 点列補間NURBS曲面
				goto EXIT;
			break;
		case 3:
			if(NFunc.GenApproximationNurbsS(&nurb,pt,row,col,M,M) == KOD_ERR)	// 点列補間NURBS曲面
				goto EXIT;
			break;
		case 5:
			if(NFunc.GenNurbsSfromCP(&nurb,pt,row,col,M,M) == KOD_ERR)	// コントロールポイントからNURBS曲面を生成
				goto EXIT;
			break;
		default:
			goto EXIT;
	}

	// 登録
	newbody = new BODY;
	RegistNurbsStoBody(&BodyList,newbody,nurb,"NewBody_S");	// 新たなNURBS曲面を新たなBODYとして登録
	DrawBODYFlag = KOD_TRUE;				// BODY描画してもOKフラグON
	ReDrawBODYFlag = KOD_FALSE;				// BODYのメモリリストを再取得
	OpenDelBtn();							// Delボタン属性の変更

	SetMaxCoord();			// 作成したBODYの最大Coord値を調べる
	SetModelScale();		// モデルスケールを最適化

	FreeCoord2(pt,row);
	return KOD_TRUE;

EXIT:
	FreeCoord2(pt,row);
	return KOD_ERR;
}

// コマンド登録
void KODatUNO::SetConsoleHistory(WSCstring His)
{
	if(His == (WSCstring)"\n>")	return;		// コマンドなし

	CmdStackNow = 0;		// 現在注目中の履歴コマンド番号を0に初期化

	// CmdStack[0]を空ける
	if(CmdStackNum){
		for(int i=CmdStackNum-1;i>=0;i--){
			if(CmdStackNum == HISTORYNUMMAX && i == CmdStackNum-1)	continue;
			sprintf(CmdStack[i+1],"%s",CmdStack[i]);
		}
	}

	// CmdStack[0]に新たなコマンドを記録
	His.cutString(His.getChars()-2);
	sprintf(CmdStack[0],"%s",(char *)His);

	if(CmdStackNum != HISTORYNUMMAX)
		CmdStackNum++;

}

// コマンド取得(上矢印キー)
int KODatUNO::GetUpperConsoleHistory(WSCstring *cmd)
{
	if(!CmdStackNum)
		return KOD_FALSE;		// 履歴コマンドなし

	if(CmdStackNow == CmdStackNum)		// 上矢印キーを履歴コマンド数以上に押した場合は最初に戻る
		CmdStackNow = 0;

	*cmd = (WSCstring)CmdStack[CmdStackNow];	// コマンド取り出し
	CmdStackNow++;				// 履歴コマンド番号をインクリメント
	return KOD_TRUE;
}

// コマンド取得(下矢印キー)
int KODatUNO::GetLowerConsoleHistory(WSCstring *cmd)
{
	if(!CmdStackNum)
		return KOD_FALSE;

	CmdStackNow--;

	if(CmdStackNow <= 0)
		CmdStackNow = CmdStackNum;

	*cmd = (WSCstring)CmdStack[CmdStackNow-1];
	return KOD_TRUE;
}

// 選択されている曲面のコントロールポイントを描画
void KODatUNO::DescribeCP()
{
	if(!BodyList.getNum())	return;

	BODY *body;
	OBJECT *obj;
	double col[3] = {0,1,0};
	char buf[256];

	glDisable(GL_LIGHTING);

	for(int pnum=0;pnum<SeldEntList.getNum();pnum++){
		obj = (OBJECT *)SeldEntList.getData(pnum);
		body = (BODY *)BodyList.getData(obj->Body);
		//if(obj->Type == _NURBSC){
		//	DrawPoints(body->NurbsC[obj->Num].cp,body->NurbsC[obj->Num].K,1,3,col);
		//}
		if(obj->Type == _NURBSS){
			for(int i=0;i<body->NurbsS[obj->Num].K[0];i++){
				for(int j=0;j<body->NurbsS[obj->Num].K[1];j++){
					DrawPoint(body->NurbsS[obj->Num].cp[i][j],1,3,col);
				}
			}
		}
		else if(obj->Type == _TRIMMED_SURFACE){
			for(int i=0;i<body->TrmS[obj->Num].pts->K[0];i++){
				for(int j=0;j<body->TrmS[obj->Num].pts->K[1];j++){
					DrawPoint(body->TrmS[obj->Num].pts->cp[i][j],1,3,col);
				}
			}
		}
	}

	glEnable(GL_LIGHTING);
}

// 選択されている曲面の情報をコンソール出力する
void KODatUNO::GetSurfInfo()
{
	if(!BodyList.getNum())	return;

	BODY *body;
	OBJECT *obj;
	NURBSS *ns;
	char buf[256];

	for(int pnum=0;pnum<SeldEntList.getNum();pnum++){
		obj = (OBJECT *)SeldEntList.getData(pnum);
		body = (BODY *)BodyList.getData(obj->Body);
		if(obj->Type == _NURBSS){
			ns=&body->NurbsS[obj->Num];
			sprintf(buf,"NURBS Surface");
			SetMessage(buf);
		}
		else if(obj->Type == _TRIMMED_SURFACE){
			ns=body->TrmS[obj->Num].pts;
			sprintf(buf,"TRIMED Surface");
			SetMessage(buf);
		}
		sprintf(buf,"No. of CP: %d x %d = %d",ns->K[0],ns->K[1],ns->K[0]*ns->K[1]);
		SetMessage(buf);
		sprintf(buf,"Rank: %d, %d",ns->M[0],ns->M[1]);
		SetMessage(buf);
		sprintf(buf,"No. of Knot [Range]: %d[%.3lf - %.3lf], %d[%.3lf - %.3lf]",ns->N[0],ns->U[0],ns->U[1],ns->N[1],ns->V[0],ns->V[1]);
		SetMessage(buf);
	}
}