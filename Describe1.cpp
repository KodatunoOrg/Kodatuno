// OpenGL UserInterface

#include "Describe.h"

// DESCRIBEクラスで用いるスタティック変数
BODY *DESCRIBE::glbody;					// IGESデータリストの先頭アドレス
BODY *DESCRIBE::Curr_body;				// IGESデータリストの注目中のアドレスを格納
IGES_PARSER DESCRIBE::Parser;			// IGESパーサー用オブジェクトを生成
QUATERNION DESCRIBE::QFunc;				// クォータニオン演算ライブラリクラス
NURBS_Func DESCRIBE::NFunc;				// NURBS_Funcクラスのオブジェクトを生成
USER DESCRIBE::User;					// Userクラスのオブジェクトを生成
int  DESCRIBE::ScreenHeight;			// 描画ウィンドウの高さ
int  DESCRIBE::ScreenWidth;				// 描画ウィンドウの幅
int  DESCRIBE::StartX;					// ドラッグ開始位置X
int  DESCRIBE::StartY;					// ドラッグ開始位置Y
int  DESCRIBE::OldPosX;					// ウィンドウ再描画直前のX座標
int  DESCRIBE::OldPosY;					// ウィンドウ再描画直前のY座標
int  DESCRIBE::CurrentX;				// ドラッグ中の位置X
int  DESCRIBE::CurrentY;				// ドラッグ中の位置Y
int  DESCRIBE::SweepSelectFlag;			// スイープセレクションを開始したことを示すフラグ
int  DESCRIBE::Vp[4];					// ビューポイント
GLuint  DESCRIBE::SelectBuf[MAXSELECT]; // セレクトバッファ
int  DESCRIBE::LBtnFlag;				// 左クリックしたことを示すフラグ
int  DESCRIBE::RBtnFlag;				// 右クリックしたことを示すフラグ
int  DESCRIBE::MBtnFlag;				// 中クリックしたことを示すフラグ
int  DESCRIBE::ShiftKeyFlag;			// Shiftキーを押したことを示すフラグ
int  DESCRIBE::CtrlKeyFlag;				// Ctrlキーを押したことを示すフラグ
int  DESCRIBE::AltKeyFlag;				// Altキーを押したことを示すフラグ
Quat DESCRIBE::StartQ;					// 右クリックした瞬間の回転角を格納するクォータニオン
Quat DESCRIBE::TargetQ;					// ぐりぐりやっている途中のクォータニオン
double DESCRIBE::RotMx[TMATELEMNUM];	// クォータニオン→同時変換マトリックス
double DESCRIBE::Trl[3];				// 平行移動量
double DESCRIBE::AxisScale;				// 軸のスケール
double DESCRIBE::ModelScale;			// モデルのスケール
double DESCRIBE::ModelScale1st;			// モデルスケールの初期値
double DESCRIBE::IdentX;				// マウスの絶対位置X→ウィンドウ内での相対位置の換算係数X
double DESCRIBE::IdentY;				// マウスの絶対位置Y→ウィンドウ内での相対位置の換算係数Y
int DESCRIBE::DrawSurfFlagCount;		// Surface描画フラグメニューが何回選択されたかを数えるカウンタ
int DESCRIBE::DrawBODYFlagCount;		// BODY描画フラグメニューが何回選択されたかを数えるカウンタ
int DESCRIBE::DrawUserFuncFlag;			// PDT描画フラグ
OBJECT DESCRIBE::SelectedEntList[MAXSELECT];// セレクションされたエンティティを逐次格納していく
int DESCRIBE::PickCount;				// ピック回数をカウント
int DESCRIBE::SPickCount;				// NURBS曲面がピックされた数をカウント
int DESCRIBE::CPickCount;				// NURBS曲線がピックされた数をカウント
int DESCRIBE::BodyCount;				// bodyの数をカウント


// DESCRIBEクラスのコンストラクタ
DESCRIBE::DESCRIBE()
{
	// 初期化
	LBtnFlag = GL_FALSE;
	RBtnFlag = GL_FALSE;
	MBtnFlag = GL_FALSE;
	ShiftKeyFlag = GL_FALSE;
	CtrlKeyFlag = GL_FALSE;
	AltKeyFlag = GL_FALSE;
	SweepSelectFlag = GL_FALSE;
	StartX = 0;
	StartY = 0;
	OldPosX = 0;
	OldPosY = 0;
	CurrentX = 0;
	CurrentY = 0;
	ScreenHeight = 240;
	ScreenWidth = 320;
	AxisScale = 0.2;
	ModelScale = 1;
	StartQ = QFunc.QInit(1.0,0.0,0.0,0.0);
	TargetQ = QFunc.QInit(1.0,0.0,0.0,0.0);
	Vp[0] = Vp[1] = Vp[2] = Vp[3] = 0;
	Trl[0] = Trl[1] = Trl[2] = 0.0;
	DrawBODYFlag = TRUE;
	DrawSurfFlag = TRUE;
	DrawBODY1st = FALSE;
	DrawSurfFlagCount = 0;
	DrawBODYFlagCount = 0;
	DrawUserFuncFlag = FALSE;
	PickCount = 0;
	CPickCount = 0;
	SPickCount = 0;
	BodyCount = 0;
}

// DESCRIBEクラスのデストラクタ（glutイベントループを抜けると必ず呼ばれる）
DESCRIBE::~DESCRIBE()
{
	Curr_body = glbody;
	for(int i=0;i<BodyCount;i++){
		DelBodyList(&glbody,i);
	}
}

// OpenGL描画
int DESCRIBE::BODY_Describe_Main(int argc,char *argv[])
{
	int sub_menu_ID1,sub_menu_ID2;
	char YorN = 'y';
	char buf[256];

	// IGESファイルを読み込む
	glbody = NULL;
	while(YorN=='y' || YorN=='Y'){
		if(GenBodyList() == ERR)		// IGESを読み込み、BODYクラスのリストを形成
			return ERR;
		fprintf(stderr,"Inport new IGES file? ('y' or 'n'):");
		fgets(buf,sizeof(buf),stdin);
		sscanf(buf,"%c",&YorN);
	}
	fprintf(stderr,"IGESファイル読み込み終了\n");

	//fprintf(stderr,"MaxCoord = %lf\n",glbody->MaxCoord); 
	ModelScale = ModelScale/glbody->MaxCoord;			// 初期画面にオブジェクト全体が写るように調節
	ModelScale1st = ModelScale;

	glutInit(&argc, argv);								// OpenGL初期化
	Init("KODATUNO");									// グラフィック初期設定(ウィンドウ名を渡す)
	glutKeyboardFunc(Keyboard);							// キーイベントの設定
	glutSpecialFunc(Special);							// 特殊キーイベントの設定
	glutMouseFunc(Mouse);								// マウスイベントの設定
	glutMotionFunc(Motion);								// マウスモーションイベントの設定
	glutReshapeFunc(Reshape);							// 再描画イベントの設定
	glutDisplayFunc(Display);							// 描画コア(ここでBODYが描画される)

	sub_menu_ID1 = glutCreateMenu(Sub_Menu_View);		// 視点切換用メニューを登録
	glutAddMenuEntry("View on x-axis",S_M_XView);		// 視点切換用メニューサブメニュー
	glutAddMenuEntry("View on y-axis",S_M_YView);		// 視点切換用メニューサブメニュー
	glutAddMenuEntry("View on z-axis",S_M_ZView);		// 視点切換用メニューサブメニュー
	sub_menu_ID2 = glutCreateMenu(Sub_Menu_User);		// User自作関数を登録(メイン1個+サブ5個)
	glutAddMenuEntry(User.MenuLabelMain,M_Label_Main);	// Userメイン関数
	glutAddMenuEntry(User.MenuLabel1,M_Label1);			// Userサブ関数1
	glutAddMenuEntry(User.MenuLabel2,M_Label2);			// Userサブ関数2
	glutAddMenuEntry(User.MenuLabel3,M_Label3);			// Userサブ関数3
	glutAddMenuEntry(User.MenuLabel4,M_Label4);			// Userサブ関数4
	glutAddMenuEntry(User.MenuLabel5,M_Label5);			// Userサブ関数5
	glutCreateMenu(Menu);								// メインメニューイベントの設定
	glutAddSubMenu("Execute User Function",sub_menu_ID2);	// メニュー登録(User関数)
	glutAddMenuEntry("Selection Cancel",M_UnSelect);		// メニュー登録(PDT描画を初期化)
	glutAddMenuEntry("Draw BODY ON/OFF",M_Draw_Body);		// メニュー登録(BODY描画 ON/OFF切換)
	glutAddMenuEntry("Draw Surface ON/OFF",M_Draw_Surf);	// メニュー登録(Surface描画 ON/OFF切換)
	glutAddSubMenu("View Angle",sub_menu_ID1);				// メニュー登録(視点切換)
	glutAddMenuEntry("Version info",M_Version);				// メニュー登録(バージョン情報)
	glutAttachMenu(GLUT_RIGHT_BUTTON);						// 右ボタンにメニューを配置

	glutMainLoop();											// イベント待ちループ

	// イベントループを抜けると、KODatuno::~KODatUNOが呼ばれる

	return TRUE;
}

// メインメニューのイベントを登録
void DESCRIBE::Menu(int val)
{
	switch(val){
		// バージョン情報
		case M_Version:
			VersionInfo();
			break;

		// セレクション解除
		case M_UnSelect:
			for(int i=0;i<PickCount;i++){		// 色を元に戻す
				Curr_body = glbody;							
				Curr_body = SearchBodyList(Curr_body,SelectedEntList[i].Body);						// どのBODYが選択されているか調べる
				if(SelectedEntList[i].Type == _NURBSC){
					ChangeStatColor(Curr_body->NurbsC[SelectedEntList[i].Num].Dstat.Color,1,1,1,1);	// 選択済みを示すため色を変更
				}
				else if(SelectedEntList[i].Type == _TRIMMED_SURFACE){
					ChangeStatColor(Curr_body->NurbsS[SelectedEntList[i].Num].Dstat.Color,0.2,0.2,0.2,1);	// 選択済みを示すため色を変更
				}
			}
			PickCount = 0;				// セレクション数を0に初期化
			DrawUserFuncFlag = FALSE;	// User関数実行済みフラグOFF
			UpDateDisplayList(TRUE);	// 描画オブジェクトの変更を更新
			break;

		// Surface描画のON/OFF
		case M_Draw_Surf:
			DrawSurfFlagCount = 1 - DrawSurfFlagCount;
			if(DrawSurfFlagCount)
				gluNurbsProperty(NurbsSurf, GLU_DISPLAY_MODE, GLU_OUTLINE_PATCH);
			else
				gluNurbsProperty(NurbsSurf, GLU_DISPLAY_MODE, GLU_FILL);
			UpDateDisplayList(FALSE);
			break;

		// BODY描画のON/OFF	
		case M_Draw_Body:
			DrawBODYFlagCount = 1 - DrawBODYFlagCount;
			if(DrawBODYFlagCount)
				DrawBODYFlag = FALSE;
			else
				DrawBODYFlag = TRUE;
			break;
	}

	glutPostRedisplay();
}


// User関数をmenuに登録
void DESCRIBE::Sub_Menu_User(int val)
{
	switch(val){
		// Userメイン関数
		case M_Label_Main:
			if(PickCount){
				AddEntSymbolToObj();								// OBJECT構造体に何番目の曲線or曲面かの情報を付加する
				glNewList(COMMAND_DRAW_USER,GL_COMPILE);			// User関数ではUserメイン関数での処理のみ描画高速化処理を施す
				User.UserFuncMain(glbody,SelectedEntList,PickCount);// Userメイン関数を実行
				glEndList();
				DrawUserFuncFlag = TRUE;							// Userメイン関数実行済みフラグON
			}
			break;

		// Userサブ関数1
		case M_Label1:
			AddEntSymbolToObj();								// OBJECT構造体に何番目の曲線or曲面かの情報を付加する
			User.UserFunc1(glbody,SelectedEntList,PickCount);	// Userサブ関数1を実行
			break;

		// Userサブ関数2
		case M_Label2:
			AddEntSymbolToObj();								// OBJECT構造体に何番目の曲線or曲面かの情報を付加する
			User.UserFunc2(glbody,SelectedEntList,PickCount);	// Userサブ関数1を実行
			break;

		// Userサブ関数3
		case M_Label3:
			AddEntSymbolToObj();								// OBJECT構造体に何番目の曲線or曲面かの情報を付加する
			User.UserFunc3(glbody,SelectedEntList,PickCount);	// Userサブ関数1を実行
			break;

		// Userサブ関数4
		case M_Label4:
			AddEntSymbolToObj();								// OBJECT構造体に何番目の曲線or曲面かの情報を付加する
			User.UserFunc4(glbody,SelectedEntList,PickCount);	// Userサブ関数1を実行
			break;

		// Userサブ関数5
		case M_Label5:
			AddEntSymbolToObj();								// OBJECT構造体に何番目の曲線or曲面かの情報を付加する
			User.UserFunc5(glbody,SelectedEntList,PickCount);	// Userサブ関数1を実行
			break;
	}
	glutPostRedisplay();
}

// 視点切換
void DESCRIBE::Sub_Menu_View(int val)
{
	switch(val){
		// X-View
		case S_M_XView:
			StartQ = QFunc.QInit(1,0,0,0);		// 今の回転角情報を初期化
			QFunc.QtoR(RotMx,StartQ);			// 今の回転角情報を初期化
			TargetQ = QFunc.QMult(QFunc.QGenRot(PI/2,0,0,1), StartQ);	// クォータニオン上で初期位置に対してZ軸回りに90°回転
			QFunc.QtoR(RotMx,TargetQ);			// 回転行列に代入
			StartQ = QFunc.QCopy(TargetQ);
			ModelScale = ModelScale1st;			// スケール初期化
			Trl[0] = Trl[1] = Trl[2] = 0;		// 平行移動量初期化
			break;
		// Y-View
		case S_M_YView:
			StartQ = QFunc.QInit(1,0,0,0);
			QFunc.QtoR(RotMx,StartQ);
			ModelScale = ModelScale1st;
			Trl[0] = Trl[1] = Trl[2] = 0;
			break;
		// Z-View
		case S_M_ZView:
			StartQ = QFunc.QInit(1,0,0,0);
			QFunc.QtoR(RotMx,StartQ);
			TargetQ = QFunc.QMult(QFunc.QGenRot(PI/2,1,0,0), StartQ);	// クォータニオン上で初期位置に対してX軸回りに90°回転
			QFunc.QtoR(RotMx,TargetQ);	
			StartQ = QFunc.QCopy(TargetQ);
			ModelScale = ModelScale1st;
			Trl[0] = Trl[1] = Trl[2] = 0;
			break;
	}
	glutPostRedisplay();
}

// IGES描画コア
void DESCRIBE::Display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);						// モデルビュー行列

	glLoadIdentity();								// モデルビュー行列の先頭に単位行列を挿入

	gluLookAt(0,1,0.0,0.0,0.0,0.0,0.0,0.0,1.0);		// 視点を設定

	glScaled(ModelScale,ModelScale,ModelScale);		// モデルスケールを設定

	glTranslatef(Trl[0],Trl[1],Trl[2]);				// 座標系を平行移動する
	
	glMultMatrixd(RotMx);							// モデルビュー行列に変更された回転行列をかけていく

	glDrawAxis(AxisScale,ModelScale);				// 座標軸を描画

	// BODY描画フラグがONなら描画
	if(DrawBODYFlag == TRUE){
		if(DrawBODY1st == FALSE){									// 一番初回のみディスプレイリストにBODYを登録
			glNewList(COMMAND_DRAW_BODY,GL_COMPILE_AND_EXECUTE);	// glDrawBODY()をコマンドとしてメモリーに保存(セレクション処理の高速化)
			glDrawBODY();											// IGESデータを描画
			glEndList();
			DrawBODY1st = TRUE;
		}
		else{
			glCallList(COMMAND_DRAW_BODY);			// 2回目以降の描画呼び出しはディスプレイリストを呼び出す
		}
	}

	// UserFuncによって生成された描画指令を実行
	if(DrawUserFuncFlag == TRUE)
		glCallList(COMMAND_DRAW_USER);

	// スイープセレクションフラグがONなら描画
	if(SweepSelectFlag == GL_TRUE){
		glMatrixMode(GL_PROJECTION);				// 投影変換行列スタックを選択
		glPushMatrix();								// 現在の座標系を保存
		glLoadIdentity();							// 投影変換行列スタックの先頭に単位行列を挿入
		gluOrtho2D(0,ScreenWidth,ScreenHeight,0); 	// 視点を設定(ラバーバンドを正しい位置に表示させるため)

		glMatrixMode(GL_MODELVIEW);					// モデルビュー行列
		glPushMatrix();								// 現在の座標系を保存
		glLoadIdentity();							// モデルビュー行列の先頭に単位行列を挿入
		
		glDrawRubberband(StartX,StartY,CurrentX,CurrentY);		// ラバーバンドを描画

		// 座標系を元に戻す
		glMatrixMode(GL_MODELVIEW);
		glPopMatrix();
		glMatrixMode(GL_PROJECTION);
		glPopMatrix();
	}

	glutSwapBuffers();
}

// ウインドウの再作成
void DESCRIBE::Reshape(int width, int height)
{
	double ratio;
	double a=2;

	ScreenWidth = width;			// 変更されたスクリーンの幅を得る
	ScreenHeight = height;			// 変更されたスクリーンの高さを得る

	glViewport(0,0,width,height);

	glMatrixMode(GL_PROJECTION);	// 投影変換行列スタックを選択
	glLoadIdentity();				// 投影変換行列スタックの先頭に単位行列を挿入

	// 平行投影を設定(アスペクト比によってモデルの縦横比が変わらないように処理を分ける)
	if(width <= height){
		ratio = (double)height/(double)width;
		glOrtho(-a*ratio,a*ratio,-a,a,-100000,100000);
	}
	else{
		ratio = (double)width/(double)height;
		glOrtho(-a*ratio,a*ratio,-a,a,-100000,100000);
	}

	glMatrixMode(GL_MODELVIEW);		// モデルビュー行列スタックを選択
}

// グラフィックの初期設定
void DESCRIBE::Init(char *progname)
{
	GLfloat diffuse[] = { 0.9, 0.9, 0.9, 1.0 };
	GLfloat specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat ambient[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat mat_diffuse[] = { 0.7, 0.7, 0.7, 1.0 };
	GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat mat_shininess[] = { 100.0 };
	GLfloat mat_ambient[] = { 0.2, 0.2, 0.2, 1.0 };

	glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH );
	glutInitWindowSize(ScreenWidth, ScreenHeight);		// ウィンドウサイズ
	glutInitWindowPosition(100, 100);					// ウィンドウ位置
	glutCreateWindow(progname);							// ウィンドウ生成
	glClearColor( 0.0, 0.0, 0.0, 1.0 );					// 背景色

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

	SetNurbsStat();

	QFunc.QtoR(RotMx,StartQ);		// 回転行列の初期化
}

// キーイベントの設定
void DESCRIBE::Keyboard(unsigned char key, int x, int y)
{
	if(key == 27){		// ESCキーで終了
		exit(0);
	}
}

// セレクション(マウスピッキング)の設定
void DESCRIBE::DoSelect(int x,int y)
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
	if(DrawBODYFlag == TRUE)					// BODY描画フラグがONなら
		glCallList(COMMAND_DRAW_BODY);			// 各エンティティで階層化されたセレクション用番号を得る(あらかじめ登録しておいたコマンドを実行)
	glMatrixMode(GL_PROJECTION);				// 射影変換モードに戻る
	glPopMatrix();								// 射影変換マトリクスを元に戻す
	hits = glRenderMode(GL_RENDER);				// セレクションモード終了、レンダリングモードに戻る

	if(StartX == x && StartY == y)				// クリックで選択した場合
		ClickPicking(SelectBuf,hits);
	else
		DragPicking(SelectBuf,hits);			// ドラッグで選択した場合

	glutPostRedisplay();
}

// クリックによるマウスピッキング
void DESCRIBE::ClickPicking(GLuint SelectBuf[],int hits)
{
	int i;
	int objnum=0;

	objnum = ObjSelect(SelectBuf,hits);			// ヒットしたオブジェクトの中でデプス値の一番小さいオブジェクトを選択する

	// 以前にセレクトされたものを再セレクトした場合はカウントしない
	for(i=0;i<PickCount;i++){
		if(SelectedEntList[i].Body == SelectBuf[objnum*(3+SelectBuf[0])+3] &&
			SelectedEntList[i].Type == SelectBuf[objnum*(3+SelectBuf[0])+4] &&
			SelectedEntList[i].Num == SelectBuf[objnum*(3+SelectBuf[0])+5]){
				hits = 0;
		}
	}
	
	// 新しいオブジェクトにヒットした場合
	if(hits >= 1){
		SelectedEntList[PickCount].Body = SelectBuf[objnum*(3+SelectBuf[0])+3];	// セレクトされたBODY
		SelectedEntList[PickCount].Type = SelectBuf[objnum*(3+SelectBuf[0])+4];	// セレクトされたエンティティタイプ
		SelectedEntList[PickCount].Num = SelectBuf[objnum*(3+SelectBuf[0])+5];	// セレクトされたエンティティタイプにおける識別番号
		BODY *Curr = glbody;
		while(Curr != NULL){
			if(SelectedEntList[PickCount].Body == Curr->key && SelectedEntList[PickCount].Type == _NURBSC){
				ChangeStatColor(Curr->NurbsC[SelectedEntList[PickCount].Num].Dstat.Color,1,0.2,1,1);	// 選択済みを示すため色を変更
			}
			else if(SelectedEntList[PickCount].Body == Curr->key && SelectedEntList[PickCount].Type == _TRIMMED_SURFACE){
				ChangeStatColor(Curr->NurbsS[SelectedEntList[PickCount].Num].Dstat.Color,1,1,0.2,1);	// 選択済みを示すため色を変更
			}
			Curr = ShiftBodyList(Curr);
		}
		PickCount++;
		UpDateDisplayList(FALSE);
	}
}

// ドラッグによるマウスピッキング
void DESCRIBE::DragPicking(GLuint SelectBuf[],int hits)
{
	int i,j;
	int temp=0;	// 再セレクト判別用カウンタ

	// 範囲選択によってオブジェクトにヒットした場合
	for(i=0;i<hits;i++){
		for(j=0;j<PickCount;j++){
			if(SelectedEntList[i].Body == SelectBuf[j*(3+SelectBuf[0])+3] &&
				SelectedEntList[i].Type == SelectBuf[j*(3+SelectBuf[0])+4] &&
				SelectedEntList[i].Num == SelectBuf[j*(3+SelectBuf[0])+5]){
					temp++;
			}
		}
		// オブジェクトが新しくヒットした場合
		if(temp == 0){
			SelectedEntList[PickCount].Body = SelectBuf[i*(3+SelectBuf[0])+3];	// セレクトされたBODY
			SelectedEntList[PickCount].Type = SelectBuf[i*(3+SelectBuf[0])+4];	// セレクトされたエンティティタイプ
			SelectedEntList[PickCount].Num = SelectBuf[i*(3+SelectBuf[0])+5];	// セレクトされたエンティティタイプにおける識別番号
			BODY *Curr = glbody;
			while(Curr != NULL){
				if(SelectedEntList[PickCount].Body == Curr->key && SelectedEntList[PickCount].Type == _NURBSC){
					ChangeStatColor(Curr->NurbsC[SelectedEntList[PickCount].Num].Dstat.Color,1,0.2,1,1);	// 選択済みを示すため色を変更
				}
				else if(SelectedEntList[PickCount].Body == Curr->key && SelectedEntList[PickCount].Type == _TRIMMED_SURFACE){
					ChangeStatColor(Curr->NurbsS[SelectedEntList[PickCount].Num].Dstat.Color,1,1,0.2,1);	// 選択済みを示すため色を変更
				}
				Curr = ShiftBodyList(Curr);
			}
			PickCount++;
		}
		temp = 0;
	}
	UpDateDisplayList(FALSE);
}

// オブジェクトの選択判別
int DESCRIBE::ObjSelect(GLuint SelectBuf[],int hits)
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

// 特殊キーイベントの設定
void DESCRIBE::Special(int key,int x,int y)
{
	switch(key){
		case GLUT_KEY_UP:
			ModelScale += 0.2*ModelScale;	// 拡大
			break;
		case GLUT_KEY_DOWN:
			ModelScale -= 0.2*ModelScale;	// 縮小
			break;
		default:
			break;
	}
	StartX = x;
	StartY = y;
	glutPostRedisplay();
}

// マウスクリックイベントの設定
void DESCRIBE::Mouse(int button,int state,int x,int y)
{
	int GetModif = glutGetModifiers();		// Shift,Ctrl,Altキーの取得

	// 中クリック
	if(button == GLUT_MIDDLE_BUTTON && state == GLUT_DOWN){
		StartX = x;
		StartY = y;
		MBtnFlag = GL_TRUE;
		if(GetModif == GLUT_ACTIVE_SHIFT){		// Sift
			OldPosX = x;
			OldPosY = y;
			ShiftKeyFlag = GL_TRUE;
		}
		if(GetModif == GLUT_ACTIVE_CTRL){		// Ctrl
			OldPosX = x;
			OldPosY = y;
			CtrlKeyFlag = GL_TRUE;
		}
	}
	// 左クリック
	else if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN){
		StartX = x;
		StartY = y;
		LBtnFlag = GL_TRUE;
	}
	// ボタンを離した
	else{
		if(MBtnFlag == GL_TRUE){
			StartQ = QFunc.QCopy(TargetQ);
		}
		if(LBtnFlag == GL_TRUE){
			DoSelect(x,y);		// オブジェクト選択判別
		}
		SweepSelectFlag = GL_FALSE;
		LBtnFlag = GL_FALSE;
		MBtnFlag = GL_FALSE;
		ShiftKeyFlag = GL_FALSE;
		CtrlKeyFlag = GL_FALSE;
	}

	glutPostRedisplay();
}

// マウスが移動したときに呼び出されるコールバックの設定
void DESCRIBE::Motion(int x,int y)
{
	double dx,dy;				// 単位化されたマウス移動量
	double dox,doy;				// 単位化されたウィンドウ再描画直前からのマウス移動量
	double a;					// マウス移動距離
	double ar;					// 回転角
	Quat dq;					// 回転クォータニオン
	Coord axis;					// 回転軸

	// 左ドラッグ中
	if(LBtnFlag == GL_TRUE){
		SweepSelectFlag = GL_TRUE;		// スィープセレクションON
		CurrentX = x;
		CurrentY = y;
	}

	// 真ん中ドラッグ中
	if(MBtnFlag == GL_TRUE){
		// マウスポインタの位置のドラッグ開始位置からの変位
		dx = (double)(x-StartX)/(double)ScreenWidth;
		dy = (double)(y-StartY)/(double)ScreenHeight;
		// マウスポインタの位置のウィンドウ再描画直前からの変位
		dox = (double)(x-OldPosX)/(double)ScreenWidth;
		doy = (double)(y-OldPosY)/(double)ScreenHeight;

		// Shift,Ctrlキーを押していない-->モデル回転
		if(ShiftKeyFlag == GL_FALSE && CtrlKeyFlag == GL_FALSE){
			a = sqrt(dx*dx + dy*dy);	// マウスポインタの位置のドラッグ開始位置からの距離
			if(a != 0.0){
				ar = 2*PI*a;			// 回転角を算出
				// 回転軸axisを求める
				axis.x = dx/a;
				axis.y = 0.0;
				axis.z = -dy/a;
				dq = QFunc.QGenRot(ar,axis.z,axis.y,axis.x);	// 回転クォータニオン生成
				TargetQ = QFunc.QMult(dq, StartQ);				// 回転の初期値StartQにdqを掛けて回転を合成
				QFunc.QtoR(RotMx, TargetQ);						// クォータニオンから回転の変換行列を求める
			}
		}

		// Shiftキーを押している-->平行移動
		else if(ShiftKeyFlag == GL_TRUE){
			Trl[0] -= 4*dox/ModelScale;							// マウスを左右へ動かしたら左右へ平行移動
			Trl[2] -= 4*doy/ModelScale;							// マウスを上下へ動かしたら上下へ平行移動
		}

		// Ctrlキーを押している-->拡大縮小
		else if(CtrlKeyFlag == GL_TRUE){
			a = sqrt(dox*dox + doy*doy);
			if(x-OldPosX > 0)	ModelScale += a*ModelScale;		// マウスを右へ動かしたら拡大
			else				ModelScale -= a*ModelScale;		// マウスを左へ動かしたら縮小
			if(ModelScale < 0.0001) ModelScale = 0.0001;		// 縮小スケールの最小値
		}			

		OldPosX = x;
		OldPosY = y;
	}

	glutPostRedisplay();
}

// bodyを読み込みリストに登録
int DESCRIBE::GenBodyList()
{
	BODY *new_body;

	new_body = AllocBodyList();					// bodyを新たに1つメモリー確保

	if(Parser.IGES_Parser_Main(new_body) == ERR){		// IGESデータの読み込み
		return ERR;
	}

	CheckDegenracy(new_body);					// 縮退(2Dパラメトリック曲線の始点と終点が一致しているか)のチェック

	new_body->key = BodyCount;					// 現在のbody番号を得る
	
	AddBodyList(&glbody,BodyCount-1,new_body);	// 読み込んだbodyをリストの最後尾に繋ぐ

	BodyCount++;

	return TRUE;
}



// OBJECT構造体に何番目の曲線or曲面かの情報を付加する
void DESCRIBE::AddEntSymbolToObj()
{
	int Cnum = 0;
	int Snum = 0;

	for(int i=0;i<PickCount;i++){
		if(SelectedEntList[i].Type == _NURBSC){
			SelectedEntList[i].CCount = Cnum;
			Cnum++;
		}
		else if(SelectedEntList[i].Type == _TRIMMED_SURFACE){
			SelectedEntList[i].SCount = Snum;
			Snum++;
		}
		else{
			fprintf(stderr,"DESCRIBE ERROR:サポート外のエンティティを検出しました\n");
		}
	}
}

// アイドリング中の処理
void DESCRIBE::Idle()
{
	glutPostRedisplay();
}

// ディスプレイリストの更新
void DESCRIBE::UpDateDisplayList(int Val)
{
	glNewList(COMMAND_DRAW_BODY,GL_COMPILE);	// glDrawBODY()をコマンドとしてメモリーに保存(セレクション処理の高速化)
	glDrawBODY();								// IGESデータを描画
	glEndList();

	if(DrawUserFuncFlag == TRUE && Val == TRUE){	// UserFuncが実行済みならUserFuncをリスト化する(UserFucnが実行済みでも、リスト化したくない場合はValにFALSEを与える)
		glNewList(COMMAND_DRAW_USER,GL_COMPILE);
		User.UserFuncMain(glbody,SelectedEntList,PickCount);
		glEndList();
	}
}

// 軸描画
void DESCRIBE::glDrawAxis(double Ascale,double Mscale)
{
	double AxisPos = Ascale/Mscale;
	double CharPos = (Ascale+0.1)/Mscale;

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
	glutSolidCone(AxisPos*0.1,AxisPos*0.5,4.0,1.0);
	glPopMatrix();

	/* Draw Ycone */
	glColor3f(0.0, 1.0, 0.0);
	glPushMatrix();
	glTranslatef(0.0, AxisPos, 0.0);
	glRotatef(-90.0, 1.0, 0.0, 0.0);
	glutSolidCone(AxisPos*0.1,AxisPos*0.5,4.0,1.0);
	glPopMatrix();

	/* Draw Zcone */
	glColor3f(0.0, 0.0, 1.0);
	glPushMatrix();
	glTranslatef(0.0, 0.0, AxisPos);
	glutSolidCone(AxisPos*0.1,AxisPos*0.5,4.0,1.0);
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
void DESCRIBE::glDrawRubberband(double sx,double sy,double ex,double ey)
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

// バージョン情報を出力
void DESCRIBE::VersionInfo()
{
	fprintf(stderr,"KODATUNO R1.0\n");
}
