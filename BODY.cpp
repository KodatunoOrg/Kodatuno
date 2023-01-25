#include <stdlib.h>
#include "BODY.h"


// BODYクラスのメモリー確保
void New_body(BODY *Body,int TypeNum[])
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
	fprintf(stderr,"ERROR: malloc BODY\n");
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
	exit(ERR);
}

// BODYを構成する全エンティティのメモリ開放
// TypeCount[ALL_ENTITY_TYPE_NUM]:各エンティティの数が格納されたもの
void Free_body(BODY *Body,int TypeCount[])
{
	// エンティティを新たに追加する場合は以下に新たなfreeを追加する
	if(TypeCount[_TRIMMED_SURFACE]){
		Free_TrmS_1DArray(Body->TrmS,TypeCount[_TRIMMED_SURFACE]);
		free(Body->TrmS);
	}
	if(TypeCount[_CURVE_ON_PARAMETRIC_SURFACE]){
		free(Body->ConpS);
	}
	if(TypeCount[_NURBSS]){
		Free_NurbsS_1DArray(Body->NurbsS,TypeCount[_NURBSS]);
		free(Body->NurbsS);
	}
	if(TypeCount[_NURBSC]){
		Free_NurbsC_1DArray(Body->NurbsC,TypeCount[_NURBSC]);
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
		Free_CompC_1DArray(Body->CompC,TypeCount[_COMPOSITE_CURVE]);
		free(Body->CompC);
	}
	if(TypeCount[_CIRCLE_ARC]){
		free(Body->CirA);
	}
}

// 複合曲線のメモリー確保
int New_CompC(COMPC *compc,int num)
{
	if((compc->DEType = (int *)malloc(sizeof(int)*num)) == NULL){
		return ERR;
	}
	
	if((compc->pDE = (COMPELEM **)malloc(sizeof(COMPELEM *)*num)) == NULL){
		free(compc->DEType);
		return ERR;
	}

	compc->N = num;

	return TRUE;
}

// 複合曲線配列のメモリー解放
void Free_CompC_1DArray(COMPC *a,int num)
{
	for(int i=0;i<num;i++)
		Free_CompC(&a[i]);
}

// 複合曲線のメモリー解放
void Free_CompC(COMPC *a)
{
	free(a->DEType);
	free(a->pDE);
}

// Nurbs曲線のメモリー確保
// 引数  *nurb:メモリー確保するNurbs曲線へのポインタ、K:コントロールポイントの数、N:ノットベクトルの数
int New_NurbsC(NURBSC *nurb,int K, int N)
{
	if((nurb->T = (double *)malloc(sizeof(double)*N)) == NULL)
		return ERR;
	if((nurb->W = (double *)malloc(sizeof(double)*K)) == NULL){
		free(nurb->T);
		return ERR;
	}
	if((nurb->cp = (Coord *)malloc(sizeof(Coord)*K)) == NULL){
		free(nurb->T);
		free(nurb->W);
		return ERR;
	}

	return TRUE;
}

// NURBS曲線配列のメモリー解放
void Free_NurbsC_1DArray(NURBSC *a,int num)
{
	for(int i=0;i<num;i++)
		Free_NurbsC(&a[i]);
}

// 1本のNURBS曲線のメモリー解放
void Free_NurbsC(NURBSC *a)
{
	free(a->T);
	free(a->W);
	free(a->cp);
}

// NURBS曲面のメモリー確保
// 引数  *nurb:メモリー確保するNurbs曲面へのポインタ、K[2]:u,vコントロールポイントの数、N[2]:u,vノットベクトルの数
int New_NurbsS(NURBSS *nurb,int K[2],int N[2])
{
	int errflag = 0;

	if((nurb->S = (double *)malloc(sizeof(double)*N[0])) == NULL)
		goto EXIT;
	errflag++;	// 1
	if((nurb->T = (double *)malloc(sizeof(double)*N[1])) == NULL)
		goto EXIT;
	errflag++;	// 2
	if((nurb->W = NewMatrix(K[0],K[1])) == NULL)
		goto EXIT;
	errflag++; // 3
	if((nurb->cp = NewCoord(K[0],K[1])) == NULL)
		goto EXIT;

	return TRUE;

EXIT:
	if(errflag == 3){
		FreeMatrix(nurb->W);
		errflag--;
	}
	if(errflag == 2){
		free(nurb->T);
		errflag--;
	}
	if(errflag == 1){
		free(nurb->S);
	}

	return ERR;
}

// NURBS曲面配列のメモリー解放
void Free_NurbsS_1DArray(NURBSS *a,int num)
{
	for(int i=0;i<num;i++)
		Free_NurbsS(&a[i]);
}

// NURBS曲面のメモリー解放
void Free_NurbsS(NURBSS *a)
{
	free(a->S);
	free(a->T);
	FreeMatrix(a->W);
	FreeCoord(a->cp);
}

// トリム面のメモリー確保
int New_TrmS(TRMS *trms,int num)
{
	if((trms->pTI = (CONPS **)malloc(sizeof(CONPS *)*num)) == NULL){
		return ERR;
	}

	return TRUE;
}

// トリム面配列のメモリー解放
void Free_TrmS_1DArray(TRMS *a,int num)
{
	for(int i=0;i<num;i++)
		Free_TrmS(&a[i]);
}

// トリム面のメモリー解放
void Free_TrmS(TRMS *a)
{
	free(a->pTI);
}

// エンティティのステータスで定義されている色を変更
void ChangeStatColor(float *col,float r,float g,float b,float t)
{
	col[0] = r;
	col[1] = g;
	col[2] = b;
	col[3] = t;
}

// bodyリストの次のデータへ移動する
BODY *ShiftBodyList(BODY *body)
{
	return(body->next);
}

// bodyリストを1つメモリー確保する
BODY *AllocBodyList()
{
	return((BODY *)malloc(sizeof(BODY)));

}

// bodyリストの指定した位置に新たなbodyを挿入する
// 引数   **body:BODYリストの先頭アドレス  key:新たに挿入したい場所の1つ手前のBODY番号  new_body:新たに挿入するBODY
void AddBodyList(BODY **body,int key,BODY *new_body)
{
	BODY *Curr;		// 注目中のBODYを指す

	// 1個目のBODYの場合
	if(key < 0){
		*body = new_body;		// BODYリストの先頭に新たなBODYを追加
		(*body)->next = NULL;	// 次の要素はNULLにする
		return;
	}
	// 2個目以降の場合
	else{
		Curr = *body;							// BODYリストの先頭に移動
		while(Curr != NULL){					// リストのエンドまでループ
			if(Curr->key == key){				// 新たなBODYを挿入したい場所の直前のBODYを発見したら
				new_body->next = Curr->next;	// リストの鎖を繋ぎかえる
				Curr->next = new_body;			// リストの鎖を繋ぎかえる
				return;
			}
			Curr = ShiftBodyList(Curr);			// 次のBODYへ移動
		}
	}
}

// 指定したbody番号のbodyをリストから削除する
// 引数  **body:BODYリストの先頭アドレス  key:削除したいBODY要素番号
void DelBodyList(BODY **body,int key)
{
	BODY *pre;
	BODY *Curr;
	
	pre = Curr = *body;

	while(Curr != NULL){
		if(Curr->key == key){
			Free_body(Curr,Curr->TypeNum);	// bodyのメモリー解放
			if(Curr == *body)
				*body = Curr->next;
			else
				pre->next = Curr->next;
			free(Curr);						// 最後に自分も消滅
			return;
		}
		pre = Curr;
		Curr = ShiftBodyList(Curr);
	}
}

// 指定したBody番号(key)の位置を見つける. 見つからない場合はNULLを返す
BODY *SearchBodyList(BODY *body,int key)
{
	BODY *Curr;

	Curr = body;
	while(Curr != NULL){
		if(Curr->key == key){
			return Curr;
		}
		Curr = ShiftBodyList(Curr);
	}

	return NULL;
}
