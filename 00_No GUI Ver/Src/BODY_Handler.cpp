// BODYのステータス操作、メモリー関連の定義

#include <stdlib.h>
#include "BODY_Handler.h"

// BODYクラスのメモリー確保
void BODY_Handler::New_body(BODY *Body,int TypeNum[])
{
	int flag=0;

	// エンティティを新たに追加する場合は以下に新たなmallocを記述してください。
	// エンティティタイプの番号が若い順に記述
	if(TypeNum[_CIRCLE_ARC]){
		if((Body->CirA = (CIRA *)malloc(sizeof(CIRA)*TypeNum[_CIRCLE_ARC])) == NULL)  goto EXIT;
		flag = _CIRCLE_ARC+1;
	}

	if(TypeNum[_COMPOSITE_CURVE]){
		if((Body->CompC = (COMPC *)malloc(sizeof(COMPC)*TypeNum[_COMPOSITE_CURVE])) == NULL)  goto EXIT;
		flag = _COMPOSITE_CURVE+1;
	}

	if(TypeNum[_CONIC_ARC]){
		if((Body->ConA = (CONA *)malloc(sizeof(CONA)*TypeNum[_CONIC_ARC])) == NULL)  goto EXIT;
		flag = _CONIC_ARC+1;
	}

	if(TypeNum[_LINE]){
		if((Body->Line = (LINE_ *)malloc(sizeof(LINE_)*TypeNum[_LINE])) == NULL)  goto EXIT;
		flag = _LINE+1;
	}

	if(TypeNum[_TRANSFORMATION_MATRIX]){
		if((Body->TMat = (TMAT *)malloc(sizeof(TMAT)*TypeNum[_TRANSFORMATION_MATRIX])) == NULL)  goto EXIT;
		flag = _TRANSFORMATION_MATRIX+1;
	}

	if(TypeNum[_NURBSC]){
		if((Body->NurbsC = (NURBSC *)malloc(sizeof(NURBSC)*TypeNum[_NURBSC])) == NULL)  goto EXIT;
		flag = _NURBSC+1;
	}

	if(TypeNum[_NURBSS]){
		if((Body->NurbsS = (NURBSS *)malloc(sizeof(NURBSS)*TypeNum[_NURBSS])) == NULL)  goto EXIT;
		flag = _NURBSS+1;
	}

	if(TypeNum[_CURVE_ON_PARAMETRIC_SURFACE]){
		if((Body->ConpS = (CONPS *)malloc(sizeof(CONPS)*TypeNum[_CURVE_ON_PARAMETRIC_SURFACE])) == NULL)  goto EXIT;
		flag = _CURVE_ON_PARAMETRIC_SURFACE+1;
	}

	if(TypeNum[_TRIMMED_SURFACE]){
		if((Body->TrmS = (TRMS *)malloc(sizeof(TRMS)*TypeNum[_TRIMMED_SURFACE])) == NULL)  goto EXIT;
		flag = _TRIMMED_SURFACE+1;
	}

	return;		// メモリーを正常に確保


EXIT:	// メモリー確保に失敗した場合は、これまで確保した分を解放して終了
	SetMessage("KOD_ERROR: malloc BODY");
	while(flag){
		if(flag == _CURVE_ON_PARAMETRIC_SURFACE+1 && TypeNum[_TRIMMED_SURFACE]){
			free(Body->ConpS);
		}
		else if(flag == _NURBSS+1 && TypeNum[_NURBSS]){
			free(Body->NurbsS);
		}
		else if(flag == _NURBSC+1 && TypeNum[_NURBSC]){
			free(Body->NurbsC);
		}
		else if(flag == _TRANSFORMATION_MATRIX+1 && TypeNum[_TRANSFORMATION_MATRIX]){
			free(Body->TMat);
		}
		else if(flag == _LINE+1 && TypeNum[_LINE]){
			free(Body->Line);
		}
		else if(flag == _CONIC_ARC+1 && TypeNum[_CONIC_ARC]){
			free(Body->ConA);
		}
		else if(flag == _COMPOSITE_CURVE+1 && TypeNum[_COMPOSITE_CURVE]){
			free(Body->CompC);
		}
		else if(flag == _CIRCLE_ARC+1 && TypeNum[_CIRCLE_ARC]){
			free(Body->CirA);
		}
		flag--;
	}
	exit(KOD_ERR);
}

// BODYを構成する全エンティティのメモリ開放
// TypeCount[ALL_ENTITY_TYPE_NUM]:各エンティティの数が格納されたもの
void BODY_Handler::Free_body(BODY *Body,int TypeCount[])
{
	// エンティティを新たに追加する場合は以下に新たなfreeを追加する
	if(TypeCount[_TRIMMED_SURFACE]){
		NFunc.Free_TrmS_1DArray(Body->TrmS,TypeCount[_TRIMMED_SURFACE]);
		free(Body->TrmS);
	}
	if(TypeCount[_CURVE_ON_PARAMETRIC_SURFACE]){
		free(Body->ConpS);
	}
	if(TypeCount[_NURBSS]){
		NFunc.Free_NurbsS_1DArray(Body->NurbsS,TypeCount[_NURBSS]);
		free(Body->NurbsS);
	}
	if(TypeCount[_NURBSC]){
		NFunc.Free_NurbsC_1DArray(Body->NurbsC,TypeCount[_NURBSC]);
		free(Body->NurbsC);
	}
	if(TypeCount[_TRANSFORMATION_MATRIX]){
		free(Body->TMat);
	}
	if(TypeCount[_LINE]){
		free(Body->Line);
	}
	if(TypeCount[_CONIC_ARC]){
		free(Body->ConA);
	}
	if(TypeCount[_COMPOSITE_CURVE]){
		NFunc.Free_CompC_1DArray(Body->CompC,TypeCount[_COMPOSITE_CURVE]);
		free(Body->CompC);
	}
	if(TypeCount[_CIRCLE_ARC]){
		free(Body->CirA);
	}
}



// 線の色の初期値を与える
void BODY_Handler::InitCurveColor(float *col)
{
	col[0] = col[1] = col[2] = 1.0;
	col[3] = 0.5;
}

// 面の色の初期値を与える
void BODY_Handler::InitSurfaceColor(float *col)
{
	col[0] = col[1] = col[2] = 0.2;
	col[3] = 0.5;
}


// エンティティのステータスで定義されている色を変更
void BODY_Handler::ChangeStatColor(float *col,float r,float g,float b,float t)
{
	col[0] = r;
	col[1] = g;
	col[2] = b;
	col[3] = t;
}

// セレクションされているBODY番号を得る
BODY *BODY_Handler::SearchBodyList(BODYList *BodyList,int key)
{
	return (BODY *)BodyList->getData(key);
}

// BODY初期化
void BODY_Handler::Init_body(BODY *body)
{
	body->MaxCoord = 1.0;

	for(int i=0;i<ALL_ENTITY_TYPE_NUM;i++){
		body->TypeNum[i] = 0;
	}
}

// 1つのNURBS曲線を新たなBODYとして登録する
void BODY_Handler::RegistNurbsCtoBody(BODYList *BodyList,BODY *Body,NURBSC Nurb,char BodyName[])
{
	Init_body(Body);	
	Body->NurbsC = (NURBSC *)malloc(sizeof(NURBSC));
	Body->NurbsC[0] = Nurb;												// NURBS曲面の実体を代入
	Body->TypeNum[_NURBSC] = 1;											// NURBS曲面の数1にする
	ChangeStatColor(Body->NurbsC[0].Dstat.Color,0.2,0.2,1.0,0.5);		// 青色
	BodyList->add((void *)Body);										// リストに新しいBODYを登録
	SetBodyNameToWin(BodyName);											// BodyリストウィンドウにBODY名を登録
	strcpy(Body->Name,BodyName);										// 新しいBODY名を登録
}

// N個のNURBS曲線を新たなBODYとして登録する
void BODY_Handler::RegistNurbsCtoBodyN(BODYList *BodyList,BODY *Body,NURBSC Nurb[],char BodyName[],int N)
{
	Init_body(Body);	
	Body->NurbsC = (NURBSC *)malloc(sizeof(NURBSC)*N);
	for(int i=0;i<N;i++){
		Body->NurbsC[i] = Nurb[i];										// NURBS曲面の実体を代入
		Body->TypeNum[_NURBSC] = N;										// NURBS曲面の数1にする
		ChangeStatColor(Body->NurbsC[i].Dstat.Color,0.2,0.2,1.0,0.5);	// 青色
	}
	BodyList->add((void *)Body);										// リストに新しいBODYを登録
	SetBodyNameToWin(BodyName);											// BodyリストウィンドウにBODY名を登録
	strcpy(Body->Name,BodyName);										// 新しいBODY名を登録
}


// 1個のNURBS曲面を新たなBODYとして登録する
void BODY_Handler::RegistNurbsStoBody(BODYList *BodyList,BODY *Body,NURBSS Nurb,char BodyName[])
{
	Init_body(Body);	
	Body->NurbsS = (NURBSS *)malloc(sizeof(NURBSS));
	Body->NurbsS[0] = Nurb;												// NURBS曲面の実体を代入
	Body->NurbsS[0].TrmdSurfFlag = KOD_FALSE;							// トリムのない単純なNURBS曲面であることを明示
	Body->TypeNum[_NURBSS] = 1;											// NURBS曲面の数1にする
	ChangeStatColor(Body->NurbsS[0].Dstat.Color,0.2,0.2,1.0,0.5);		// 青色
	BodyList->add((void *)Body);										// リストに新しいBODYを登録
	SetBodyNameToWin(BodyName);											// BodyリストウィンドウにBODY名を登録
	strcpy(Body->Name,BodyName);										// 新しいBODY名を登録
}

// N個のNURBS曲面を新たなBODYとして登録する
void BODY_Handler::RegistNurbsStoBodyN(BODYList *BodyList,BODY *Body,NURBSS Nurb[],char BodyName[],int N)
{
	Init_body(Body);	
	Body->NurbsS = (NURBSS *)malloc(sizeof(NURBSS)*N);
	for(int i=0;i<N;i++){
		Body->NurbsS[i] = Nurb[i];										// NURBS曲面の実体を代入
		Body->NurbsS[i].TrmdSurfFlag = KOD_FALSE;						// トリムのない単純なNURBS曲面であることを明示
		Body->TypeNum[_NURBSS] = N;										// NURBS曲面の数1にする
		ChangeStatColor(Body->NurbsS[i].Dstat.Color,0.2,0.2,1.0,0.5);	// 青色
	}
	BodyList->add((void *)Body);										// リストに新しいBODYを登録
	SetBodyNameToWin(BodyName);											// BodyリストウィンドウにBODY名を登録
	strcpy(Body->Name,BodyName);										// 新しいBODY名を登録
}

// BODYの回転
void BODY_Handler::RotBody(BODY *Body,Coord Axis,double deg)
{
	for(int i=0;i<Body->TypeNum[_NURBSS];i++)			// NURBS曲面のシフト
		NFunc.RotNurbsS(&Body->NurbsS[i],Axis,deg);
	for(int i=0;i<Body->TypeNum[_NURBSC];i++){			// NURBS曲線のシフト
		if(Body->NurbsC[i].EntUseFlag == GEOMTRYELEM)	// NURBS曲面のパラメトリック要素としてのNURBS曲線に関しては何もしない
			NFunc.RotNurbsC(&Body->NurbsC[i],Axis,deg);
	}
}

// BODYのシフト
void BODY_Handler::ShiftBody(BODY *Body,Coord d)
{
	for(int i=0;i<Body->TypeNum[_NURBSS];i++)			// NURBS曲面のシフト
		NFunc.ShiftNurbsS(&Body->NurbsS[i],d);
	for(int i=0;i<Body->TypeNum[_NURBSC];i++){			// NURBS曲線のシフト
		if(Body->NurbsC[i].EntUseFlag == GEOMTRYELEM)	// NURBS曲面のパラメトリック要素としてのNURBS曲線に関しては何もしない
			NFunc.ShiftNurbsC(&Body->NurbsC[i],d);
	}
}

