#include "BODY.h"

// コンストラクタ
BODY::BODY()
{
	// 初期化
	for(int i=0;i<ALL_ENTITY_TYPE_NUM;i++){
		TypeNum[i] = 0;
	}
	Mesh = NULL;
	Mom = NULL;

	MaxCoord = 1;
}

// BODYクラスのメモリー確保
void BODY::NewBodyElem()
{
	int flag=0;

	// エンティティを新たに追加する場合は以下に新たなmallocを記述してください。
	// エンティティタイプの番号が若い順に記述
	if(TypeNum[_CIRCLE_ARC]){
		if((CirA = (CIRA *)malloc(sizeof(CIRA)*TypeNum[_CIRCLE_ARC])) == NULL)  goto EXIT;
		flag = _CIRCLE_ARC+1;
	}

	if(TypeNum[_COMPOSITE_CURVE]){
		if((CompC = (COMPC *)malloc(sizeof(COMPC)*TypeNum[_COMPOSITE_CURVE])) == NULL)  goto EXIT;
		flag = _COMPOSITE_CURVE+1;
	}

	if(TypeNum[_CONIC_ARC]){
		if((ConA = (CONA *)malloc(sizeof(CONA)*TypeNum[_CONIC_ARC])) == NULL)  goto EXIT;
		flag = _CONIC_ARC+1;
	}

	if(TypeNum[_LINE]){
		if((Line = (LINE_ *)malloc(sizeof(LINE_)*TypeNum[_LINE])) == NULL)  goto EXIT;
		flag = _LINE+1;
	}

	if(TypeNum[_TRANSFORMATION_MATRIX]){
		if((TMat = (TMAT *)malloc(sizeof(TMAT)*TypeNum[_TRANSFORMATION_MATRIX])) == NULL)  goto EXIT;
		flag = _TRANSFORMATION_MATRIX+1;
	}

	if(TypeNum[_NURBSC]){
		if((NurbsC = (NURBSC *)malloc(sizeof(NURBSC)*TypeNum[_NURBSC])) == NULL)  goto EXIT;
		flag = _NURBSC+1;
	}

	if(TypeNum[_NURBSS]){
		if((NurbsS = (NURBSS *)malloc(sizeof(NURBSS)*TypeNum[_NURBSS])) == NULL)  goto EXIT;
		flag = _NURBSS+1;
	}

	if(TypeNum[_CURVE_ON_PARAMETRIC_SURFACE]){
		if((ConpS = (CONPS *)malloc(sizeof(CONPS)*TypeNum[_CURVE_ON_PARAMETRIC_SURFACE])) == NULL)  goto EXIT;
		flag = _CURVE_ON_PARAMETRIC_SURFACE+1;
	}

	if(TypeNum[_TRIMMED_SURFACE]){
		if((TrmS = (TRMS *)malloc(sizeof(TRMS)*TypeNum[_TRIMMED_SURFACE])) == NULL)  goto EXIT;
		flag = _TRIMMED_SURFACE+1;
	}

	Mesh = NULL;		// メッシュはNULLに設定しておく

	return;		// メモリーを正常に確保


EXIT:	// メモリー確保に失敗した場合は、これまで確保した分を解放して終了
	SetMessage("KOD_ERROR: malloc BODY");
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
// BODYを構成するエンティティを指定した数だけメモリ開放
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

// BODYの回転
void BODY::RotBody(Coord Axis,double deg)
{
	NURBS_Func NFunc;

	for(int i=0;i<TypeNum[_NURBSS];i++)			// NURBS曲面のシフト
		NFunc.RotNurbsS(&NurbsS[i],Axis,deg);
	for(int i=0;i<TypeNum[_NURBSC];i++){			// NURBS曲線のシフト
		if(NurbsC[i].EntUseFlag == GEOMTRYELEM)	// NURBS曲面のパラメトリック要素としてのNURBS曲線に関しては何もしない
			NFunc.RotNurbsC(&NurbsC[i],Axis,deg);
	}
}

// BODYのシフト
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

// BODYの拡大縮小
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

// 自分を新たなBODYとして登録する
void BODY::RegistBody(BODYList *BodyList, char BodyName[])
{
	Mom = BodyList->add(this);				// 読み込んだIGESデータをBODYListに登録する
	strcpy(Name,BodyName);					// ファイル名をbody名として登録
}

// 自分自身を消去する
void BODY::DeleteBody(BODYList *BodyList)
{
	BodyList->delData(Mom);			// リストから読み込んだBODYをはずす
	DelBodyElem();					// BODY内で確保しているメモリーの解放
}

// 1つのNURBS曲線を新たなBODYとして登録する
void BODY::RegistNurbsCtoBody(BODYList *BodyList,NURBSC Nurb,char BodyName[])
{
	NurbsC = (NURBSC *)malloc(sizeof(NURBSC));
	NurbsC[0] = Nurb;												// NURBS曲面の実体を代入
	TypeNum[_NURBSC] = 1;											// NURBS曲面の数1にする
	ChangeStatColor(this->NurbsC[0].Dstat.Color,0.2,0.2,1.0,0.5);		// 青色
	BodyList->add(this);										// リストに新しいBODYを登録
	//SetBodyNameToWin(BodyName);											// BodyリストウィンドウにBODY名を登録
	strcpy(Name,BodyName);										// 新しいBODY名を登録
}

// N個のNURBS曲線を新たなBODYとして登録する
void BODY::RegistNurbsCtoBodyN(BODYList *BodyList,NURBSC Nurb[],char BodyName[],int N)
{
	NurbsC = (NURBSC *)malloc(sizeof(NURBSC)*N);
	for(int i=0;i<N;i++){
		NurbsC[i] = Nurb[i];										// NURBS曲面の実体を代入
		TypeNum[_NURBSC] = N;										// NURBS曲面の数1にする
		ChangeStatColor(this->NurbsC[i].Dstat.Color,0.2,0.2,1.0,0.5);	// 青色
	}
	BodyList->add((void *)this);										// リストに新しいBODYを登録
	//SetBodyNameToWin(BodyName);											// BodyリストウィンドウにBODY名を登録
	strcpy(Name,BodyName);										// 新しいBODY名を登録
}


// 1個のNURBS曲面を新たなBODYとして登録する
void BODY::RegistNurbsStoBody(BODYList *BodyList,NURBSS Nurb,char BodyName[])
{
	NurbsS = (NURBSS *)malloc(sizeof(NURBSS));
	NurbsS[0] = Nurb;												// NURBS曲面の実体を代入
	NurbsS[0].TrmdSurfFlag = KOD_FALSE;								// トリムのない単純なNURBS曲面であることを明示
	TypeNum[_NURBSS] = 1;											// NURBS曲面の数1にする
	ChangeStatColor(this->NurbsS[0].Dstat.Color,0.2,0.2,1.0,0.5);	// 青色
	this->Mom = BodyList->add((void *)this);						// リストに新しいBODYを登録
	//SetBodyNameToWin(BodyName);										// BodyリストウィンドウにBODY名を登録
	strcpy(Name,BodyName);											// 新しいBODY名を登録
}

// N個のNURBS曲面を新たなBODYとして登録する
void BODY::RegistNurbsStoBodyN(BODYList *BodyList,NURBSS Nurb[],char BodyName[],int N)
{
	NurbsS = (NURBSS *)malloc(sizeof(NURBSS)*N);
	for(int i=0;i<N;i++){
		NurbsS[i] = Nurb[i];										// NURBS曲面の実体を代入
		NurbsS[i].TrmdSurfFlag = KOD_FALSE;							// トリムのない単純なNURBS曲面であることを明示
		TypeNum[_NURBSS] = N;										// NURBS曲面の数1にする
		ChangeStatColor(this->NurbsS[i].Dstat.Color,0.2,0.2,1.0,0.5);	// 青色
	}
	this->Mom = BodyList->add((void *)this);						// リストに新しいBODYを登録
	//SetBodyNameToWin(BodyName);										// BodyリストウィンドウにBODY名を登録
	strcpy(Name,BodyName);											// 新しいBODY名を登録
}

// エンティティのステータスで定義されている色を変更
void BODY::ChangeStatColor(float *col,float r,float g,float b,float t)
{
	col[0] = r;
	col[1] = g;
	col[2] = b;
	col[3] = t;
}

// 線の色の初期値を与える
void BODY::InitCurveColor(float *col)
{
	col[0] = col[1] = col[2] = 1.0;
	col[3] = 0.5;
}

// 面の色の初期値を与える
void BODY::InitSurfaceColor(float *col)
{
	col[0] = col[1] = col[2] = 0.2;
	col[3] = 0.5;
}
