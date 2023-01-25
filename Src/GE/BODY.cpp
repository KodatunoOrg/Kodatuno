#include "BODY.h"


// Function: BODY
// BODYクラスのコンストラクタ．各種初期化
BODY::BODY()
{
	// 初期化
	for(int i=0;i<ALL_ENTITY_TYPE_NUM;i++){
		TypeNum[i] = 0;
	}
	Mesh = NULL;

	MaxCoord = 1;
}


// Function: NewBodyElem
// BODYクラスのメモリー確保
void BODY::NewBodyElem()
{
	int flag=0;

	// エンティティを新たに追加する場合は以下に新たなmallocを記述してください。
	// エンティティタイプの番号が若い順に記述
	if(TypeNum[_CIRCLE_ARC]){
		if(NewCirA(TypeNum[_CIRCLE_ARC]) == NULL)  goto EXIT;
		flag = _CIRCLE_ARC+1;
	}

	if(TypeNum[_COMPOSITE_CURVE]){
		if(NewCompC(TypeNum[_COMPOSITE_CURVE]) == NULL)  goto EXIT;
		flag = _COMPOSITE_CURVE+1;
	}

	if(TypeNum[_CONIC_ARC]){
		if(NewConA(TypeNum[_CONIC_ARC]) == NULL)  goto EXIT;
		flag = _CONIC_ARC+1;
	}

	if(TypeNum[_LINE]){
		if(NewLine(TypeNum[_LINE]) == NULL)  goto EXIT;
		flag = _LINE+1;
	}

	if(TypeNum[_TRANSFORMATION_MATRIX]){
		if(NewTMat(TypeNum[_TRANSFORMATION_MATRIX]) == NULL)  goto EXIT;
		flag = _TRANSFORMATION_MATRIX+1;
	}

	if(TypeNum[_NURBSC]){
		if(NewNurbsC(TypeNum[_NURBSC]) == NULL)  goto EXIT;
		flag = _NURBSC+1;
	}

	if(TypeNum[_NURBSS]){
		if(NewNurbsS(TypeNum[_NURBSS]) == NULL)  goto EXIT;
		flag = _NURBSS+1;
	}

	if(TypeNum[_CURVE_ON_PARAMETRIC_SURFACE]){
		if(NewConpS(TypeNum[_CURVE_ON_PARAMETRIC_SURFACE]) == NULL)  goto EXIT;
		flag = _CURVE_ON_PARAMETRIC_SURFACE+1;
	}

	if(TypeNum[_TRIMMED_SURFACE]){
		if(NewTrmS(TypeNum[_TRIMMED_SURFACE]) == NULL)  goto EXIT;
		flag = _TRIMMED_SURFACE+1;
	}

	Mesh = NULL;		// メッシュはNULLに設定しておく

	return;		// メモリーを正常に確保


EXIT:	// メモリー確保に失敗した場合は、これまで確保した分を解放して終了
	GuiIF.SetMessage("KOD_ERROR: malloc BODY");
	while(flag){
		if(flag == _CURVE_ON_PARAMETRIC_SURFACE+1 && TypeNum[_TRIMMED_SURFACE]){
			free(ConpS);
		}
		else if(flag == _NURBSS+1 && TypeNum[_NURBSS]){
			free(NurbsS);
		}
		else if(flag == _NURBSC+1 && TypeNum[_NURBSC]){
			free(NurbsC);
		}
		else if(flag == _TRANSFORMATION_MATRIX+1 && TypeNum[_TRANSFORMATION_MATRIX]){
			free(TMat);
		}
		else if(flag == _LINE+1 && TypeNum[_LINE]){
			free(Line);
		}
		else if(flag == _CONIC_ARC+1 && TypeNum[_CONIC_ARC]){
			free(ConA);
		}
		else if(flag == _COMPOSITE_CURVE+1 && TypeNum[_COMPOSITE_CURVE]){
			free(CompC);
		}
		else if(flag == _CIRCLE_ARC+1 && TypeNum[_CIRCLE_ARC]){
			free(CirA);
		}
		flag--;
	}
	exit(KOD_ERR);
}


// Function: DleBodyElem
// BODYを構成する全エンティティのメモリ開放
void BODY::DelBodyElem()
{
	NURBS_Func NFunc;

	// エンティティを新たに追加する場合は以下に新たなfreeを追加する
	if(TypeNum[_TRIMMED_SURFACE]){
		NFunc.Free_TrmS_1DArray(TrmS,TypeNum[_TRIMMED_SURFACE]);
		free(TrmS);
	}
	if(TypeNum[_CURVE_ON_PARAMETRIC_SURFACE]){
		free(ConpS);
	}
	if(TypeNum[_NURBSS]){
		NFunc.Free_NurbsS_1DArray(NurbsS,TypeNum[_NURBSS]);
		free(NurbsS);
	}
	if(TypeNum[_NURBSC]){
		NFunc.Free_NurbsC_1DArray(NurbsC,TypeNum[_NURBSC]);
		free(NurbsC);
	}
	if(TypeNum[_TRANSFORMATION_MATRIX]){
		free(TMat);
	}
	if(TypeNum[_LINE]){
		free(Line);
	}
	if(TypeNum[_CONIC_ARC]){
		free(ConA);
	}
	if(TypeNum[_COMPOSITE_CURVE]){
		NFunc.Free_CompC_1DArray(CompC,TypeNum[_COMPOSITE_CURVE]);
		free(CompC);
	}
	if(TypeNum[_CIRCLE_ARC]){
		free(CirA);
	}
	if(Mesh != NULL){
		Mesh->clear();
	}
}


// Function: DelBodyElem
// BODYを構成するエンティティを指定した数だけメモリ開放
//
// Parameters: 
//	TypeNum_[] - 各エンティティ番号をインデックスとした配列に，確保されている各エンティティ数を代入
void BODY::DelBodyElem(int TypeNum_[])
{
	NURBS_Func NFunc;

	// エンティティを新たに追加する場合は以下に新たなfreeを追加する
	if(TypeNum_[_TRIMMED_SURFACE]){
		NFunc.Free_TrmS_1DArray(TrmS,TypeNum_[_TRIMMED_SURFACE]);
		free(TrmS);
	}
	if(TypeNum_[_CURVE_ON_PARAMETRIC_SURFACE]){
		free(ConpS);
	}
	if(TypeNum_[_NURBSS]){
		NFunc.Free_NurbsS_1DArray(NurbsS,TypeNum_[_NURBSS]);
		free(NurbsS);
	}
	if(TypeNum_[_NURBSC]){
		NFunc.Free_NurbsC_1DArray(NurbsC,TypeNum_[_NURBSC]);
		free(NurbsC);
	}
	if(TypeNum_[_TRANSFORMATION_MATRIX]){
		free(TMat);
	}
	if(TypeNum_[_LINE]){
		free(Line);
	}
	if(TypeNum_[_CONIC_ARC]){
		free(ConA);
	}
	if(TypeNum_[_COMPOSITE_CURVE]){
		NFunc.Free_CompC_1DArray(CompC,TypeNum_[_COMPOSITE_CURVE]);
		free(CompC);
	}
	if(TypeNum_[_CIRCLE_ARC]){
		free(CirA);
	}
	if(Mesh != NULL){
		Mesh->clear();
	}
}


// Function: RotBody
// BODYを回転させる

// Parameters:
//	Axis - 回転軸
//	deg - 回転角度
void BODY::RotBody(Coord Axis,double deg)
{
	NURBS_Func NFunc;

	for(int i=0;i<TypeNum[_NURBSS];i++)			// NURBS曲面の回転
		NFunc.RotNurbsS(&NurbsS[i],Axis,deg);
	for(int i=0;i<TypeNum[_NURBSC];i++){		// NURBS曲線の回転
		if(NurbsC[i].EntUseFlag == GEOMTRYELEM)	// NURBS曲面のパラメトリック要素としてのNURBS曲線に関しては何もしない
			NFunc.RotNurbsC(&NurbsC[i],Axis,deg);
	}
}


// Function: ShiftBody
// BODYをシフトさせる
//
// Parameters:
//	d - 移動量
void BODY::ShiftBody(Coord d)
{
	NURBS_Func NFunc;

	for(int i=0;i<TypeNum[_NURBSS];i++)			// NURBS曲面のシフト
		NFunc.ShiftNurbsS(&NurbsS[i],d);
	for(int i=0;i<TypeNum[_NURBSC];i++){		// NURBS曲線のシフト
		if(NurbsC[i].EntUseFlag == GEOMTRYELEM)	// NURBS曲面のパラメトリック要素としてのNURBS曲線に関しては何もしない
			NFunc.ShiftNurbsC(&NurbsC[i],d);
	}
}

// Function: ExpandBody
//			  BODYの拡大縮小
//
// Parameters:
//		  r - X, Y, Z各方向それぞれの拡大(縮小)率(1を基準)
void BODY::ExpandBody(Coord r)
{
	NURBS_Func NFunc;

	for(int i=0;i<TypeNum[_NURBSS];i++)			// NURBS曲面のシフト
		NFunc.ChRatioNurbsS(&NurbsS[i],r);
	for(int i=0;i<TypeNum[_NURBSC];i++){		// NURBS曲線のシフト
		if(NurbsC[i].EntUseFlag == GEOMTRYELEM)	// NURBS曲面のパラメトリック要素としてのNURBS曲線に関しては何もしない
		NFunc.ChRatioNurbsC(&NurbsC[i],r);		// NURBS曲線の拡大
	}
}
// Function: RegistBody
//	自分を新たなBODYとして登録する
// 
// Parameters:
//	*BodyList - 登録先リスト
//	BodyName[] - 登録するBODY名
void BODY::RegistBody(BODYList *BodyList,const char BodyName[])
{
	Mom = BodyList->add(this);				// 読み込んだIGESデータをBODYListに登録する
	GuiIF.AddBodyNameToWin(BodyName);		// BodyリストウィンドウにBODY名を登録
	strcpy(Name,BodyName);					// ファイル名をbody名として登録
}

// Function: RegistNurbsCtoBody
//	1つのNURBS曲線を新たなBODYとして登録する
// 
// Parameters:
//	*BodyList - 登録先リスト
//	Nurb - 登録するNURBS曲線の実体
//  BodyName[] - 登録するBODY名
void BODY::RegistNurbsCtoBody(BODYList *BodyList,NURBSC Nurb,const char BodyName[])
{
	NurbsC = (NURBSC *)malloc(sizeof(NURBSC));
	NurbsC[0] = Nurb;												// NURBS曲面の実体を代入
	TypeNum[_NURBSC] = 1;											// NURBS曲面の数1にする
	ChangeStatColor(this->NurbsC[0].Dstat.Color,0.2,0.2,1.0,0.5);	// 青色
	BodyList->add(this);											// リストに新しいBODYを登録
	GuiIF.AddBodyNameToWin(BodyName);								// BodyリストウィンドウにBODY名を登録
	strcpy(Name,BodyName);											// 新しいBODY名を登録
}

// Function: RegistNurbsCtoBodyN
// N個のNURBS曲線を新たなBODYとして登録する
// 
// Parameters:
//	*BodyList - 登録先リスト
//	Nurb[] - 登録するNURBS曲線の実体
//  BodyName[] - 登録するBODY名
//	N - 登録するNURBS曲線の数
void BODY::RegistNurbsCtoBodyN(BODYList *BodyList,NURBSC Nurb[],const char BodyName[],int N)
{
	NurbsC = (NURBSC *)malloc(sizeof(NURBSC)*N);
	for(int i=0;i<N;i++){
		NurbsC[i] = Nurb[i];										// NURBS曲面の実体を代入
		TypeNum[_NURBSC] = N;										// NURBS曲面の数1にする
		ChangeStatColor(this->NurbsC[i].Dstat.Color,0.2,0.2,1.0,0.5);	// 青色
	}
	BodyList->add((void *)this);									// リストに新しいBODYを登録
	GuiIF.AddBodyNameToWin(BodyName);									// BodyリストウィンドウにBODY名を登録
	strcpy(Name,BodyName);											// 新しいBODY名を登録
}

// Function: RegistNurbsStoBody
// 1個のNURBS曲面を新たなBODYとして登録する
//
// Parameters:
//	*BodyList - 登録先リスト
//	Nurb - 登録するNURBS曲面の実体
//  BodyName[] - 登録するBODY名
void BODY::RegistNurbsStoBody(BODYList *BodyList,NURBSS Nurb,const char BodyName[])
{
	NurbsS = (NURBSS *)malloc(sizeof(NURBSS));
	NurbsS[0] = Nurb;												// NURBS曲面の実体を代入
	NurbsS[0].TrmdSurfFlag = KOD_FALSE;								// トリムのない単純なNURBS曲面であることを明示
	TypeNum[_NURBSS] = 1;											// NURBS曲面の数1にする
	ChangeStatColor(this->NurbsS[0].Dstat.Color,0.2,0.2,1.0,0.5);	// 青色
	BodyList->add((void *)this);									// リストに新しいBODYを登録
	GuiIF.AddBodyNameToWin(BodyName);									// BodyリストウィンドウにBODY名を登録
	strcpy(Name,BodyName);											// 新しいBODY名を登録
}

// Function: RegistNurbsStoBodyN
// N個のNURBS曲面を新たなBODYとして登録する
// 
// Parameters:
//	*BodyList - 登録先リスト
//	Nurb[] - 登録するNURBS曲面の実体
//  BodyName[] - 登録するBODY名
//	N - 登録するNURBS曲面の数
void BODY::RegistNurbsStoBodyN(BODYList *BodyList,NURBSS Nurb[],const char BodyName[],int N)
{
	NurbsS = (NURBSS *)malloc(sizeof(NURBSS)*N);
	for(int i=0;i<N;i++){
		NurbsS[i] = Nurb[i];										// NURBS曲面の実体を代入
		NurbsS[i].TrmdSurfFlag = KOD_FALSE;							// トリムのない単純なNURBS曲面であることを明示
		TypeNum[_NURBSS] = N;										// NURBS曲面の数1にする
		ChangeStatColor(this->NurbsS[i].Dstat.Color,0.2,0.2,1.0,0.5);	// 青色
	}
	BodyList->add((void *)this);									// リストに新しいBODYを登録
	GuiIF.AddBodyNameToWin(BodyName);									// BodyリストウィンドウにBODY名を登録
	strcpy(Name,BodyName);											// 新しいBODY名を登録
}

// Function: ChangeStatColor
// エンティティのステータスで定義されている色を変更
//
// Parameters:
// *col - 色を変更したいエンティティのメンバ変数Dstatのメンバ変数Color[4]へのポインタ
// r,g,b,t - 色属性(0.0 - 1.0)
void BODY::ChangeStatColor(float *col,float r,float g,float b,float t)
{
	col[0] = r;
	col[1] = g;
	col[2] = b;
	col[3] = t;
}

// Function: InitCurveColor
// 線の色の初期値を与える
//
// Parameters:
// *col - 色を変更したいエンティティのメンバ変数Dstatのメンバ変数Color[4]へのポインタ
void BODY::InitCurveColor(float *col)
{
	col[0] = col[1] = col[2] = 1.0;
	col[3] = 0.5;
}

// Function: InitSurfaceColor
// 面の色の初期値を与える
//
// Parameters:
// *col - 色を変更したいエンティティのメンバ変数Dstatのメンバ変数Color[4]へのポインタ
void BODY::InitSurfaceColor(float *col)
{
	col[0] = col[1] = col[2] = 0.2;
	col[3] = 0.5;
}

// Function: NewCirA
// 円・円弧CIRAを指定した数だけメモリー確保し，初期化する
// 
// Parameters:
// N - メモリー確保するCIRAの数
CIRA *BODY::NewCirA(int N)
{
	if((CirA = (CIRA *)malloc(sizeof(CIRA)*N/*TypeNum[_CIRCLE_ARC]*/)) == NULL)
		return NULL;

	for(int i=0;i<N;i++){
		CirA[i].zt = 0;
		CirA[i].cp[0] = CirA[i].cp[1] = CirA[i].cp[3] = SetCoord(0,0,0);
		CirA[i].R = 0;
		CirA[i].t[0] = CirA[i].t[1] = 0;
		CirA[i].U = CirA[i].V = SetCoord(0,0,0);
		CirA[i].EntUseFlag = 0;
		CirA[i].pD = 0;
		CirA[i].Dstat.Color[0] = CirA[i].Dstat.Color[1] = CirA[i].Dstat.Color[2] = CirA[i].Dstat.Color[3] = 0;
	}
	TypeNum[_CIRCLE_ARC] = N;

	return CirA;
}

// Function: NewCompC
// 複合曲線COMPCを指定した数だけメモリー確保し，初期化する
// 
// Parameters:
// N - メモリー確保するCOMPCの数
COMPC *BODY::NewCompC(int N)
{
	if((CompC = (COMPC *)malloc(sizeof(COMPC)*N/*TypeNum[_COMPOSITE_CURVE]*/)) == NULL)
		return NULL;

	for(int i=0;i<N;i++){
		CompC[i].DegeFlag = 0;
		CompC[i].DEType = NULL;
		CompC[i].N = 0;
		CompC[i].pD = 0;
		CompC[i].pDE = NULL;
	}
	TypeNum[_COMPOSITE_CURVE] = N;

	return CompC;
}

// Function: NewConA
// 円錐曲線CONAを指定した数だけメモリー確保し，初期化する
// 
// Parameters:
// N - メモリー確保するCONAの数
CONA *BODY::NewConA(int N)
{
	if((ConA = (CONA *)malloc(sizeof(CONA)*N/*TypeNum[_CONIC_ARC]*/)) == NULL)
		return NULL;

	for(int i=0;i<N;i++){
		ConA[i].cp[0] = ConA[i].cp[1] = SetCoord(0,0,0);
		ConA[i].Dstat.Color[0] = ConA[i].Dstat.Color[1] = ConA[i].Dstat.Color[2] = ConA[i].Dstat.Color[3] = 0;
		ConA[i].pD = 0;
		InitVector(ConA[i].prop,6);
		ConA[i].zt = 0;
	}
	TypeNum[_CONIC_ARC] = N;

	return ConA;
}

// Function: NewLine
// 線分LINE_を指定した数だけメモリー確保し，初期化する
//
// Parameters:
// N - メモリー確保するLINE_の数
LINE_ *BODY::NewLine(int N)
{
	if((Line = (LINE_ *)malloc(sizeof(LINE_)*N/*TypeNum[_LINE]*/)) == NULL)
		return NULL;

	for(int i=0;i<N;i++){
		Line[i].cp[0] = Line[i].cp[1] = SetCoord(0,0,0);
		Line[i].Dstat.Color[0] = Line[i].Dstat.Color[1] = Line[i].Dstat.Color[2] = Line[i].Dstat.Color[3] = 0;
		Line[i].EntUseFlag = 0;
		Line[i].pD = 0;
	}
	TypeNum[_LINE] = N;

	return Line;
}

// Function: NewTMat
// 変換マトリックスTMATを指定した数だけメモリー確保し，初期化する
//
// Parameters:
// N - メモリー確保するTMATの数
TMAT *BODY::NewTMat(int N)
{
	if((TMat = (TMAT *)malloc(sizeof(TMAT)*N/*TypeNum[_TRANSFORMATION_MATRIX]*/)) == NULL)
		return NULL;

	for(int i=0;i<N;i++){
		for(int j=0;j<3;j++)
			InitVector(TMat[i].R[j],3);
		InitVector(TMat[i].T,3);
		TMat[i].pD = 0;
	}
	TypeNum[_TRANSFORMATION_MATRIX] = N;

	return TMat;
}

// Function: NewNurbsC
// NURBS曲線NURBSCを指定した数だけメモリー確保し，初期化する
//
// Parameters:
// N - メモリー確保するNURBSCの数
NURBSC *BODY::NewNurbsC(int N)
{
	if((NurbsC = (NURBSC *)malloc(sizeof(NURBSC)*N/*TypeNum[_NURBSC]*/)) == NULL)
		return NULL;

	for(int i=0;i<N;i++){
		NurbsC[i].cp = NULL;
		NurbsC[i].Dstat.Color[0] = NurbsC[i].Dstat.Color[1] = NurbsC[i].Dstat.Color[2] = NurbsC[i].Dstat.Color[3] = 0;
		NurbsC[i].EntUseFlag = 0;
		NurbsC[i].K = 0;
		NurbsC[i].M = 0;
		NurbsC[i].N = 0;
		NurbsC[i].norm = SetCoord(0,0,0);
		NurbsC[i].OriginEnt = 0;
		NurbsC[i].pD = 0;
		NurbsC[i].pOriginEnt = NULL;
		NurbsC[i].prop[0] = NurbsC[i].prop[1] = NurbsC[i].prop[2] = NurbsC[i].prop[3] = 0;
		NurbsC[i].T = NULL;
		NurbsC[i].V[0] = NurbsC[i].V[1] = 0;
		NurbsC[i].W = NULL;
	}
	TypeNum[_NURBSC] = N;

	return NurbsC;
}

// Function: NewNurbsS
// NURBS曲線NURBSSを指定した数だけメモリー確保し，初期化する
//
// Parameters:
// N - メモリー確保するNURBSSの数
NURBSS *BODY::NewNurbsS(int N)
{
	if((NurbsS = (NURBSS *)malloc(sizeof(NURBSS)*N/*TypeNum[_NURBSS]*/)) == NULL)
		return NULL;

	for(int i=0;i<N;i++){
		NurbsS[i].cp = NULL;
		NurbsS[i].Dstat.Color[0] = NurbsS[i].Dstat.Color[1] = NurbsS[i].Dstat.Color[2] = NurbsS[i].Dstat.Color[3] = 0;
		NurbsS[i].K[0] = NurbsS[i].K[1] = 0;
		NurbsS[i].M[0] = NurbsS[i].M[1] = 0;
		NurbsS[i].N[0] = NurbsS[i].N[0] = 0;
		NurbsS[i].pD = 0;
		NurbsS[i].prop[0] = NurbsS[i].prop[1] = NurbsS[i].prop[2] = NurbsS[i].prop[3] = NurbsS[i].prop[4] = 0;
		NurbsS[i].S = NULL;
		NurbsS[i].T = NULL;
		NurbsS[i].TrmdSurfFlag = 0;
		NurbsS[i].U[0] = NurbsS[i].U[1] = 0;
		NurbsS[i].V[0] = NurbsS[i].V[1] = 0;
		NurbsS[i].W = NULL;
	}
	TypeNum[_NURBSS] = N;

	return NurbsS;
}

// Function: NewConpS
// 面上線CONPSを指定した数だけメモリー確保し，初期化する
//
// Parameters:
// N - メモリー確保するCONPSの数
CONPS *BODY::NewConpS(int N)
{
	if((ConpS = (CONPS *)malloc(sizeof(CONPS)*N/*TypeNum[_CURVE_ON_PARAMETRIC_SURFACE]*/)) == NULL)
		return NULL;

	for(int i=0;i<N;i++){
		ConpS[i].BType = 0;
		ConpS[i].crtn = 0;
		ConpS[i].CType = 0;
		ConpS[i].pB = NULL;
		ConpS[i].pC = NULL;
		ConpS[i].pD = 0;
		ConpS[i].pref = 0;
		ConpS[i].pS = NULL;
		ConpS[i].SType = 0;
	}
	TypeNum[_CURVE_ON_PARAMETRIC_SURFACE] = N;
	return ConpS;
}

// Function: NewTrmS
// トリム面TRMSを指定した数だけメモリー確保し，初期化する
//
// Parameters:
// N - メモリー確保するTRMSの数
TRMS *BODY::NewTrmS(int N)
{
	if((TrmS = (TRMS *)malloc(sizeof(TRMS)*N/*TypeNum[_TRIMMED_SURFACE]*/)) == NULL)
		return NULL;

	for(int i=0;i<N;i++){
		TrmS[i].n1 = 0;
		TrmS[i].n2 = 0;
		TrmS[i].pD = 0;
		TrmS[i].pTI = NULL;
		TrmS[i].pTO = NULL;
		TrmS[i].pts = NULL;
	}
	TypeNum[_TRIMMED_SURFACE] = N;

	return TrmS;
}

// Function: GetNurbsCFromLine
// 直線エンティティをNURBS曲線エンティティへと変換する
//
// Parameters:
// NurbsCount - NURBS曲線への変換後のNURBSCのインデックス番号
// LineCount - 変換したいLINEのインデックス番号
int BODY::GetNurbsCFromLine(int NurbsCount,int LineCount)	
{
	int i=0;
	int KOD_ERRflag=0;

	NurbsC[NurbsCount].K = 2;		// 総和記号の上側添字（コントロールポイント-1）の値
	NurbsC[NurbsCount].M = 2;		// 基底関数の階数
	NurbsC[NurbsCount].N = NurbsC[NurbsCount].K + NurbsC[NurbsCount].M;	// ノットベクトルの数

	// ブーリアン型プロパティ4つ
	NurbsC[NurbsCount].prop[0] = 0;
	NurbsC[NurbsCount].prop[1] = 0;
	NurbsC[NurbsCount].prop[2] = 1;
	NurbsC[NurbsCount].prop[3] = 0;

	// メモリー確保
	KOD_ERRflag++;	// 1
	if((NurbsC[NurbsCount].T = (double *)malloc(sizeof(double)*NurbsC[NurbsCount].N)) == NULL){
		goto EXIT;
	}
	KOD_ERRflag++;	// 2
	if((NurbsC[NurbsCount].W = (double *)malloc(sizeof(double)*NurbsC[NurbsCount].K)) == NULL){
		goto EXIT;
	}
	KOD_ERRflag++;	// 3
	if((NurbsC[NurbsCount].cp = (Coord *)malloc(sizeof(Coord)*NurbsC[NurbsCount].K)) == NULL){
		goto EXIT;
	}

	// ノットベクトルの値	
	NurbsC[NurbsCount].T[0] = 0.;
	NurbsC[NurbsCount].T[1] = 0.;
	NurbsC[NurbsCount].T[2] = 1.;
	NurbsC[NurbsCount].T[3] = 1.;
	
	for(i=0;i<NurbsC[NurbsCount].K;i++){				// Weightの値
		NurbsC[NurbsCount].W[i] = 1.;
	}
	for(i=0;i<NurbsC[NurbsCount].K;i++){				// コントロールポイントの座標値
		NurbsC[NurbsCount].cp[i].x = Line[LineCount].cp[i].x;
		NurbsC[NurbsCount].cp[i].y = Line[LineCount].cp[i].y;
		NurbsC[NurbsCount].cp[i].z = Line[LineCount].cp[i].z;
	}
	
	// パラメータの値
	NurbsC[NurbsCount].V[0] = 0.;
	NurbsC[NurbsCount].V[1] = 1.;

	NurbsC[NurbsCount].EntUseFlag = Line[LineCount].EntUseFlag;	// ディレクトリ部の情報"Entity Use Flag"を得る(NURBSC)
	NurbsC[NurbsCount].OriginEnt = LINE;						// 元は線分要素であったことを記憶
	NurbsC[NurbsCount].pOriginEnt = &Line[LineCount];			// 元は線分要素であったことを記憶
	for(int i=0;i<4;i++)
		NurbsC[NurbsCount].Dstat.Color[i] = Line[LineCount].Dstat.Color[i];

	return KOD_TRUE;

	// メモリー確保に失敗した場合は今まで確保した分を開放してKOD_ERRを返す
EXIT:
		GuiIF.SetMessage("PARAMETER SECTION KOD_ERROR:fail to allocate memory");
	if(KOD_ERRflag == 3){
		free(NurbsC[NurbsCount].cp);
		KOD_ERRflag--;
	}
	if(KOD_ERRflag == 2){
		free(NurbsC[NurbsCount].W);
		KOD_ERRflag--;
	}
	if(KOD_ERRflag == 1){
		free(NurbsC[NurbsCount].T);
	}
	return KOD_ERR;
}

// Function: GetNurbsCFromCirA
// 円・円弧エンティティをNURBS曲線エンティティへと変換する
//
// Parameters:
// NurbsCount - NURBS曲線への変換後のNURBSCのインデックス番号
// CirCount - 変換したいCIRAのインデックス番号
int BODY::GetNurbsCFromCirA(int NurbsCount,int CirCount)	
{
	int	 flag=KOD_TRUE;
	double	angle_deg = 0.0,
			angle_rad = 0.0,
			radius = 0.0;
	Coord	vec[2];
	
	// 円/円弧の中心点O-始点Psベクトル成分、中心点-終点Peベクトル成分をそれぞれ求める
	vec[0] = SubCoord(CirA[CirCount].cp[1],CirA[CirCount].cp[0]);
	vec[1] = SubCoord(CirA[CirCount].cp[2],CirA[CirCount].cp[0]);	

	radius = CirA[CirCount].R;	// 円/円弧の中心点と始点の距離(半径)
	angle_rad = CalcVecAngle2D(vec[0],vec[1]);			// 円/円弧を成す中心角の大きさ(degree)を求める
	angle_deg = RadToDeg(angle_rad);				// 円/円弧を成す中心角の大きさ(radian)を求める

	// 中心角(degree)の大きさごとにセグメント数を変更する
	if( angle_deg > 0 && angle_deg <= 90 ){								// 0°<θ<=90°
		flag = CirAToNurbsC_seg1(NurbsCount ,CirCount ,vec, angle_rad);		// 1セグメント
	}
	else if( angle_deg > 90 && angle_deg <= 270 ){						// 90°<θ<=270°
		flag = CirAToNurbsC_seg2(NurbsCount ,CirCount ,vec, angle_rad);		// 2セグメント
	}
	else if( angle_deg > 270 && angle_deg < 360 ){						// 270°<θ<360°
		flag = CirAToNurbsC_seg3(NurbsCount ,CirCount ,vec, angle_rad);		// 3セグメント
	}
	else if( angle_deg == 0 ){											// θ=0°(360°)
		flag = CirAToNurbsC_seg4(NurbsCount ,CirCount ,vec, radius);			//　4セグメント
	}
	else{
		GuiIF.SetMessage("Center angle of a circle or circular arc is not calculated normally");
		return KOD_ERR;
	}

	NurbsC[NurbsCount].EntUseFlag = CirA[CirCount].EntUseFlag;	// ディレクトリ部の情報"Entity Use Flag"を得る(NURBSC)
	NurbsC[NurbsCount].OriginEnt = CIRCLE_ARC;					// 元は円・円弧要素であったことを記憶
	NurbsC[NurbsCount].pOriginEnt = &CirA[CirCount];		// その円・円弧要素へのポインタ

	return KOD_TRUE;
}

// 1セグメントの円弧(中心角が0°<θ<=90°の時)
int BODY::CirAToNurbsC_seg1(int NurbsCount,int CirCount,Coord vec[], double angle_rad)
{
	int i=0;
	int KOD_ERRflag=0;
	
	Coord	vec_cp;
	
	NurbsC[NurbsCount].K = 3;		// 総和記号の上側添字（コントロールポイント-1）の値
	NurbsC[NurbsCount].M = 3;		// 基底関数の階数
	NurbsC[NurbsCount].N = NurbsC[NurbsCount].K + NurbsC[NurbsCount].M;	// ノットベクトルの数

	// ブーリアン型プロパティ4つ
	NurbsC[NurbsCount].prop[0] = 0;
	NurbsC[NurbsCount].prop[1] = 0;
	NurbsC[NurbsCount].prop[2] = 1;
	NurbsC[NurbsCount].prop[3] = 0;
	
	// メモリー確保
	KOD_ERRflag++;	// 1
	if((NurbsC[NurbsCount].T = (double *)malloc(sizeof(double)*NurbsC[NurbsCount].N)) == NULL){
		goto EXIT;
	}
	KOD_ERRflag++;	// 2
	if((NurbsC[NurbsCount].W = (double *)malloc(sizeof(double)*NurbsC[NurbsCount].K)) == NULL){
		goto EXIT;
	}
	KOD_ERRflag++;	// 3
	if((NurbsC[NurbsCount].cp = (Coord *)malloc(sizeof(Coord)*NurbsC[NurbsCount].K)) == NULL){
		goto EXIT;
	}
	
	// ノットベクトルの値	
	NurbsC[NurbsCount].T[0] = 0.;
	NurbsC[NurbsCount].T[1] = 0.;
	NurbsC[NurbsCount].T[2] = 0.;
	NurbsC[NurbsCount].T[3] = 1.;
	NurbsC[NurbsCount].T[4] = 1.;
	NurbsC[NurbsCount].T[5] = 1.;
		
	// Weightの値
	for(i=0; i<3; i++){
		if(i % 2 == 0){
			NurbsC[NurbsCount].W[i] = 1.;
		}	
		else if(i % 2 == 1){	
			NurbsC[NurbsCount].W[i] = cos(angle_rad/2);
		}
	}
		
	vec_cp = Arc_CP(vec[0], vec[1], cos(angle_rad));	//　円の中心点からコントロールポイントP1へのベクトルを求める
	
	// コントロールポイントの座標値
	NurbsC[NurbsCount].cp[0].x = CirA[CirCount].cp[1].x;
	NurbsC[NurbsCount].cp[0].y = CirA[CirCount].cp[1].y;		
	NurbsC[NurbsCount].cp[1].x = vec_cp.x + CirA[CirCount].cp[0].x;
	NurbsC[NurbsCount].cp[1].y = vec_cp.y + CirA[CirCount].cp[0].y;
	NurbsC[NurbsCount].cp[2].x = CirA[CirCount].cp[2].x;
	NurbsC[NurbsCount].cp[2].y = CirA[CirCount].cp[2].y;

	for(i=0; i<3; i++){
		NurbsC[NurbsCount].cp[i].z = CirA[CirCount].zt;	// Z方向の大きさは一定
	}
		
	NurbsC[NurbsCount].V[0] = 0.;		// パラメータの値
	NurbsC[NurbsCount].V[1] = 1.;
		  
	return KOD_TRUE;

	// メモリー確保に失敗した場合は今まで確保した分を開放してKOD_ERRを返す
EXIT:
		GuiIF.SetMessage("PARAMETER SECTION KOD_ERROR:fail to allocate memory");
	if(KOD_ERRflag == 3){
		free(NurbsC[NurbsCount].cp);
		KOD_ERRflag--;
	}
	if(KOD_ERRflag == 2){
		free(NurbsC[NurbsCount].W);
		KOD_ERRflag--;
	}
	if(KOD_ERRflag == 1){
		free(NurbsC[NurbsCount].T);
	}
	return KOD_ERR;
}

// private
// 2セグメントの円弧(中心角が90°<θ<=270°の時)
int BODY::CirAToNurbsC_seg2(int NurbsCount,int CirCount,Coord vec[], double angle_rad)
{
	int	i = 0,
		KOD_ERRflag = 0;
	double	angle_rad2 = 0.0;
	
	Coord vec_cp[3];
	
	NurbsC[NurbsCount].K = 5;		// 総和記号の上側添字（コントロールポイント-1）の値
	NurbsC[NurbsCount].M = 3;		// 基底関数の階数
	NurbsC[NurbsCount].N = NurbsC[NurbsCount].K + NurbsC[NurbsCount].M;	// ノットベクトルの数
	// ブーリアン型プロパティ4つ
	NurbsC[NurbsCount].prop[0] = 0;
	NurbsC[NurbsCount].prop[1] = 0;
	NurbsC[NurbsCount].prop[2] = 1;
	NurbsC[NurbsCount].prop[3] = 0;
	
	// メモリー確保
	KOD_ERRflag++;	// 1
	if((NurbsC[NurbsCount].T = (double *)malloc(sizeof(double)*NurbsC[NurbsCount].N)) == NULL){
		goto EXIT;
	}
	KOD_ERRflag++;	// 2
	if((NurbsC[NurbsCount].W = (double *)malloc(sizeof(double)*NurbsC[NurbsCount].K)) == NULL){
		goto EXIT;
	}
	KOD_ERRflag++;	// 3
	if((NurbsC[NurbsCount].cp = (Coord *)malloc(sizeof(Coord)*NurbsC[NurbsCount].K)) == NULL){
		goto EXIT;
	}
	
	// ノットベクトルの値	
	NurbsC[NurbsCount].T[0] = 0.;
	NurbsC[NurbsCount].T[1] = 0.;
	NurbsC[NurbsCount].T[2] = 0.;
	NurbsC[NurbsCount].T[3] = 2./4.;
	NurbsC[NurbsCount].T[4] = 2./4.;
	NurbsC[NurbsCount].T[5] = 1.;
	NurbsC[NurbsCount].T[6] = 1.;
	NurbsC[NurbsCount].T[7] = 1.;
		
	// Weightの値
	for(i=0; i<5; i++){
		if(i % 2 == 0){
			NurbsC[NurbsCount].W[i] = 1.;
		}	
		else if(i % 2 == 1){	
			NurbsC[NurbsCount].W[i] = cos(angle_rad/4);
		}
	}
		
	angle_rad2 = angle_rad/2;	// (中心角)÷2
	
	vec_cp[1] = CalcRotVec2D(vec[0], angle_rad2);		// 円の中心点から中心角の半分の位置(コントロールポイントP2)へのベクトルを求める
	vec_cp[0] = Arc_CP(vec[0], vec_cp[1], cos(angle_rad2));	// 円の中心点からコントロールポイントP1へのベクトルを求める
	vec_cp[2] = Arc_CP(vec_cp[1], vec[1], cos(angle_rad2));	// 円の中心点からコントロールポイントP3へのベクトルを求める
	
	// コントロールポイントの座標値
	NurbsC[NurbsCount].cp[0].x = CirA[CirCount].cp[1].x;
	NurbsC[NurbsCount].cp[0].y = CirA[CirCount].cp[1].y;		
 	NurbsC[NurbsCount].cp[1].x = vec_cp[0].x + CirA[CirCount].cp[0].x;
 	NurbsC[NurbsCount].cp[1].y = vec_cp[0].y + CirA[CirCount].cp[0].y;
 	NurbsC[NurbsCount].cp[2].x = vec_cp[1].x + CirA[CirCount].cp[0].x;
 	NurbsC[NurbsCount].cp[2].y = vec_cp[1].y + CirA[CirCount].cp[0].y;
 	NurbsC[NurbsCount].cp[3].x = vec_cp[2].x + CirA[CirCount].cp[0].x;
 	NurbsC[NurbsCount].cp[3].y = vec_cp[2].y + CirA[CirCount].cp[0].y;
 	NurbsC[NurbsCount].cp[4].x = CirA[CirCount].cp[2].x;
 	NurbsC[NurbsCount].cp[4].y = CirA[CirCount].cp[2].y;
	for(i=0; i<5; i++){
		NurbsC[NurbsCount].cp[i].z = CirA[CirCount].zt;	// Z方向の大きさは一定
	}
	
	NurbsC[NurbsCount].V[0] = 0.;		// パラメータの値
	NurbsC[NurbsCount].V[1] = 1.;
		  
	return KOD_TRUE;

	// メモリー確保に失敗した場合は今まで確保した分を開放してKOD_ERRを返す
EXIT:
		GuiIF.SetMessage("PARAMETER SECTION KOD_ERROR:fail to allocate memory");
	if(KOD_ERRflag == 3){
		free(NurbsC[NurbsCount].cp);
		KOD_ERRflag--;
	}
	if(KOD_ERRflag == 2){
		free(NurbsC[NurbsCount].W);
		KOD_ERRflag--;
	}
	if(KOD_ERRflag == 1){
		free(NurbsC[NurbsCount].T);
	}
	return KOD_ERR;
}

// private
// 3セグメントの円弧(中心角が270°<θ<360°の時)
int BODY::CirAToNurbsC_seg3(int NurbsCount,int CirCount,Coord vec[], double angle_rad)
{
	int	i=0,
		KOD_ERRflag=0;
	double	angle_rad3 = 0.0;
	
	Coord	vec_cp[5];
	
	NurbsC[NurbsCount].K = 7;		// 総和記号の上側添字（コントロールポイント-1）の値
	NurbsC[NurbsCount].M = 3;		// 基底関数の階数
	NurbsC[NurbsCount].N = NurbsC[NurbsCount].K + NurbsC[NurbsCount].M;	// ノットベクトルの数
	
	// ブーリアン型プロパティ4つ
	NurbsC[NurbsCount].prop[0] = 0;
	NurbsC[NurbsCount].prop[1] = 0;
	NurbsC[NurbsCount].prop[2] = 1;
	NurbsC[NurbsCount].prop[3] = 0;
	
	// メモリー確保
	KOD_ERRflag++;	// 1
	if((NurbsC[NurbsCount].T = (double *)malloc(sizeof(double)*NurbsC[NurbsCount].N)) == NULL){
		goto EXIT;
	}
	KOD_ERRflag++;	// 2
	if((NurbsC[NurbsCount].W = (double *)malloc(sizeof(double)*NurbsC[NurbsCount].K)) == NULL){
		goto EXIT;
	}
	KOD_ERRflag++;	// 3
	if((NurbsC[NurbsCount].cp = (Coord *)malloc(sizeof(Coord)*NurbsC[NurbsCount].K)) == NULL){
		goto EXIT;
	}
	
	// ノットベクトルの値	
	NurbsC[NurbsCount].T[0] = 0.;
	NurbsC[NurbsCount].T[1] = 0.;
	NurbsC[NurbsCount].T[2] = 0.;
	NurbsC[NurbsCount].T[3] = 1./3.;
	NurbsC[NurbsCount].T[4] = 1./3.;
	NurbsC[NurbsCount].T[5] = 2./3.;
	NurbsC[NurbsCount].T[6] = 2./3.;
	NurbsC[NurbsCount].T[7] = 1.;
	NurbsC[NurbsCount].T[8] = 1.;
	NurbsC[NurbsCount].T[9] = 1.;
	
	// Weightの値
	for(i=0; i<7; i++){
		if(i % 2 == 0){
			NurbsC[NurbsCount].W[i] = 1.;
		}	
		else if(i % 2 == 1){	
			NurbsC[NurbsCount].W[i] = cos(angle_rad/6);
		}
	}

	angle_rad3 = angle_rad/3;	// (中心角)÷3
	
	vec_cp[1] = CalcRotVec2D(vec[0], angle_rad3);		// 円の中心点から中心角の1/3の位置(コントロールポイントP2)へのベクトルを求める
	vec_cp[0] = Arc_CP(vec[0], vec_cp[1], cos(angle_rad3));	// 円の中心点からコントロールポイントP1へのベクトルを求める
	vec_cp[3] = CalcRotVec2D(vec_cp[1], angle_rad3);		// 円の中心点から中心角の2/3の位置(コントロールポイントP4)へのベクトルを求める
	vec_cp[2] = Arc_CP(vec_cp[1], vec_cp[3], cos(angle_rad3));	// 円の中心点からコントロールポイントP3へのベクトルを求める
	vec_cp[4] = Arc_CP(vec_cp[3], vec[1], cos(angle_rad3));	// 円の中心点からコントロールポイントP4へのベクトルを求める
		
	// コントロールポイントの座標値
	NurbsC[NurbsCount].cp[0].x = CirA[CirCount].cp[1].x;
	NurbsC[NurbsCount].cp[0].y = CirA[CirCount].cp[1].y;		
	NurbsC[NurbsCount].cp[1].x = vec_cp[0].x + CirA[CirCount].cp[0].x;
	NurbsC[NurbsCount].cp[1].y = vec_cp[0].y + CirA[CirCount].cp[0].y;
	NurbsC[NurbsCount].cp[2].x = vec_cp[1].x + CirA[CirCount].cp[0].x;
	NurbsC[NurbsCount].cp[2].y = vec_cp[1].y + CirA[CirCount].cp[0].y;
	NurbsC[NurbsCount].cp[3].x = vec_cp[2].x + CirA[CirCount].cp[0].x;
	NurbsC[NurbsCount].cp[3].y = vec_cp[2].y + CirA[CirCount].cp[0].y;
	NurbsC[NurbsCount].cp[4].x = vec_cp[3].x + CirA[CirCount].cp[0].x;
	NurbsC[NurbsCount].cp[4].y = vec_cp[3].y + CirA[CirCount].cp[0].y;
	NurbsC[NurbsCount].cp[5].x = vec_cp[4].x + CirA[CirCount].cp[0].x;
	NurbsC[NurbsCount].cp[5].y = vec_cp[4].y + CirA[CirCount].cp[0].y;
	NurbsC[NurbsCount].cp[6].x = CirA[CirCount].cp[2].x;
	NurbsC[NurbsCount].cp[6].y = CirA[CirCount].cp[2].y;

	for(i=0; i<7; i++){
		NurbsC[NurbsCount].cp[i].z = CirA[CirCount].zt;	// Z方向の大きさは一定
	}
		
	NurbsC[NurbsCount].V[0] = 0.;		// パラメータの値
	NurbsC[NurbsCount].V[1] = 1.;
		  
	return KOD_TRUE;

	// メモリー確保に失敗した場合は今まで確保した分を開放してKOD_ERRを返す
EXIT:
		GuiIF.SetMessage("PARAMETER SECTION KOD_ERROR:fail to allocate memory");
	if(KOD_ERRflag == 3){
		free(NurbsC[NurbsCount].cp);
		KOD_ERRflag--;
	}
	if(KOD_ERRflag == 2){
		free(NurbsC[NurbsCount].W);
		KOD_ERRflag--;
	}
	if(KOD_ERRflag == 1){
		free(NurbsC[NurbsCount].T);
	}
	return KOD_ERR;
}

// private
// 4セグメントの円弧(円)
int BODY::CirAToNurbsC_seg4(int NurbsCount,int CirCount,Coord vec[], double radius)
{
	int i=0;
	int KOD_ERRflag=0;

	NurbsC[NurbsCount].K = 9;		// 総和記号の上側添字（コントロールポイント-1）の値
	NurbsC[NurbsCount].M = 3;		// 基底関数の階数
	NurbsC[NurbsCount].N = NurbsC[NurbsCount].K + NurbsC[NurbsCount].M;	// ノットベクトルの数
	
	// ブーリアン型プロパティ4つ
	NurbsC[NurbsCount].prop[0] = 0;
	NurbsC[NurbsCount].prop[1] = 0;
	NurbsC[NurbsCount].prop[2] = 1;
	NurbsC[NurbsCount].prop[3] = 0;
	
	// メモリー確保
	KOD_ERRflag++;	// 1
	if((NurbsC[NurbsCount].T = (double *)malloc(sizeof(double)*NurbsC[NurbsCount].N)) == NULL){
		goto EXIT;
	}
	KOD_ERRflag++;	// 2
	if((NurbsC[NurbsCount].W = (double *)malloc(sizeof(double)*NurbsC[NurbsCount].K)) == NULL){
		goto EXIT;
	}
	KOD_ERRflag++;	// 3
	if((NurbsC[NurbsCount].cp = (Coord *)malloc(sizeof(Coord)*NurbsC[NurbsCount].K)) == NULL){
		goto EXIT;
	}
	
	// ノットベクトルの値	
	NurbsC[NurbsCount].T[0] = 0.;
	NurbsC[NurbsCount].T[1] = 0.;
	NurbsC[NurbsCount].T[2] = 0.;
	NurbsC[NurbsCount].T[3] = 1./4.;
	NurbsC[NurbsCount].T[4] = 1./4.;
	NurbsC[NurbsCount].T[5] = 2./4.;
	NurbsC[NurbsCount].T[6] = 2./4.;
	NurbsC[NurbsCount].T[7] = 3./4.;
	NurbsC[NurbsCount].T[8] = 3./4.;
	NurbsC[NurbsCount].T[9] = 1.;
	NurbsC[NurbsCount].T[10] = 1.;
	NurbsC[NurbsCount].T[11] = 1.;
		
	// Weightの値
	for(i=0; i<9; i++){
		if(i % 2 == 0){
			NurbsC[NurbsCount].W[i] = 1.;
		}	
		else if(i % 2 == 1){	
			NurbsC[NurbsCount].W[i] = sqrt(2.0)/2;
		}
	}

	// コントロールポイントの座標値
	NurbsC[NurbsCount].cp[0].x = CirA[CirCount].cp[0].x + radius;
	NurbsC[NurbsCount].cp[0].y = CirA[CirCount].cp[0].y;		
	NurbsC[NurbsCount].cp[1].x = CirA[CirCount].cp[0].x + radius;
	NurbsC[NurbsCount].cp[1].y = CirA[CirCount].cp[0].y + radius;
	NurbsC[NurbsCount].cp[2].x = CirA[CirCount].cp[0].x;
	NurbsC[NurbsCount].cp[2].y = CirA[CirCount].cp[0].y + radius;
	NurbsC[NurbsCount].cp[3].x = CirA[CirCount].cp[0].x - radius;
	NurbsC[NurbsCount].cp[3].y = CirA[CirCount].cp[0].y + radius;
	NurbsC[NurbsCount].cp[4].x = CirA[CirCount].cp[0].x - radius;
	NurbsC[NurbsCount].cp[4].y = CirA[CirCount].cp[0].y;
	NurbsC[NurbsCount].cp[5].x = CirA[CirCount].cp[0].x - radius;
	NurbsC[NurbsCount].cp[5].y = CirA[CirCount].cp[0].y - radius;
	NurbsC[NurbsCount].cp[6].x = CirA[CirCount].cp[0].x;
	NurbsC[NurbsCount].cp[6].y = CirA[CirCount].cp[0].y - radius;
	NurbsC[NurbsCount].cp[7].x = CirA[CirCount].cp[0].x + radius;
	NurbsC[NurbsCount].cp[7].y = CirA[CirCount].cp[0].y - radius;
	NurbsC[NurbsCount].cp[8].x = CirA[CirCount].cp[0].x + radius;
	NurbsC[NurbsCount].cp[8].y = CirA[CirCount].cp[0].y;

	for(i=0; i<9; i++){
		NurbsC[NurbsCount].cp[i].z = CirA[CirCount].zt;	// Z方向の大きさは一定
	}
		
	NurbsC[NurbsCount].V[0] = 0.;		// パラメータの値
	NurbsC[NurbsCount].V[1] = 1.;
	
	return KOD_TRUE;

	// メモリー確保に失敗した場合は今まで確保した分を開放してKOD_ERRを返す
EXIT:
		GuiIF.SetMessage("PARAMETER SECTION KOD_ERROR:fail to allocate memory");
	if(KOD_ERRflag == 3){
		free(NurbsC[NurbsCount].cp);
		KOD_ERRflag--;
	}
	if(KOD_ERRflag == 2){
		free(NurbsC[NurbsCount].W);
		KOD_ERRflag--;
	}
	if(KOD_ERRflag == 1){
		free(NurbsC[NurbsCount].T);
	}
	return KOD_ERR;
}

// Function: GetOuterEdgeNum
// トリム面を構成する外側エッジの数を取得する
//
// Return:
// トリム面を構成する外側エッジの数
int TRMS::GetOuterEdgeNum()
{
    COMPC *CompC = (COMPC *)pTO->pB;
    return CompC->N;
}

// Function: GetInnerTrmNum
// トリム面を構成する内側トリムの数を取得する
//
// Return:
// トリム面を構成する内側トリムの数
int TRMS::GetInnerTrmNum()
{
    return n2;
}

// Function: GetInnerEdgeNum
// トリム面を構成する内側エッジの数を取得する
//
// Parameters:
// N - 内側トリムのインデックス番号
//
// Return:
// トリム面を構成する内側エッジの数
int TRMS::GetInnerEdgeNum(int N)
{
    COMPC *CompC = (COMPC *)pTI[N]->pB;
    return CompC->N;
}

// Function: GetOuterCompC
// トリム面を構成する外側トリム曲線(複合曲線)へのポインタを取得する
//
// Return:
// トリム面を構成する外側トリム曲線(複合曲線)へのポインタ
COMPC *TRMS::GetOuterCompC()
{
    return (COMPC *)pTO->pB;
}

// Function: GetInnerCompC
// トリム面を構成する外側トリム曲線(複合曲線)へのポインタを取得する
//
// Parameters:
// N - 内側トリムのインデックス番号
//
// Return:
// トリム面を構成する外側トリム曲線(複合曲線)へのポインタ
COMPC *TRMS::GetInnerCompC(int N)
{
    return (COMPC *)pTI[N]->pB;
}

// Funciton: GetNurbsS
// トリム面を構成するNURBS曲面へのポインタを得る
//
// Return:
// トリム面を構成するNURBS曲面へのポインタ
NURBSS *TRMS::GetNurbsS()
{
    return pts;
}
