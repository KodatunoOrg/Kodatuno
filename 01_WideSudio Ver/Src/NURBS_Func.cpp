#include "NURBS_Func.h"

// Nurbs曲線のメモリー確保
// 引数  *nurb:メモリー確保するNurbs曲線へのポインタ、K:コントロールポイントの数、N:ノットベクトルの数
int NURBS_Func::New_NurbsC(NURBSC *nurb,int K, int N)
{
	if((nurb->T = (double *)malloc(sizeof(double)*N)) == NULL)
		return KOD_ERR;
	if((nurb->W = (double *)malloc(sizeof(double)*K)) == NULL){
		free(nurb->T);
		return KOD_ERR;
	}
	if((nurb->cp = (Coord *)malloc(sizeof(Coord)*K)) == NULL){
		free(nurb->T);
		free(nurb->W);
		return KOD_ERR;
	}

	return KOD_TRUE;
}

// NURBS曲線配列のメモリー解放
void NURBS_Func::Free_NurbsC_1DArray(NURBSC *a,int num)
{
	for(int i=0;i<num;i++)
		Free_NurbsC(&a[i]);
}

// 1本のNURBS曲線のメモリー解放
void NURBS_Func::Free_NurbsC(NURBSC *a)
{
	free(a->T);
	free(a->W);
	free(a->cp);
}

// NURBS曲面のメモリー確保
// 引数  *nurb:メモリー確保するNurbs曲面へのポインタ、K[2]:u,vコントロールポイントの数、N[2]:u,vノットベクトルの数
int NURBS_Func::New_NurbsS(NURBSS *nurb,int K[2],int N[2])
{
	int KOD_ERRflag = 0;

	if((nurb->S = (double *)malloc(sizeof(double)*N[0])) == NULL)
		goto EXIT;
	KOD_ERRflag++;	// 1
	if((nurb->T = (double *)malloc(sizeof(double)*N[1])) == NULL)
		goto EXIT;
	KOD_ERRflag++;	// 2
	if((nurb->W = NewMatrix(K[0],K[1])) == NULL)
		goto EXIT;
	KOD_ERRflag++; // 3
	if((nurb->cp = NewCoord2(K[0],K[1])) == NULL)
		goto EXIT;

	return KOD_TRUE;

EXIT:
	if(KOD_ERRflag == 3){
		FreeMatrix(nurb->W,nurb->K[0]);
		KOD_ERRflag--;
	}
	if(KOD_ERRflag == 2){
		free(nurb->T);
		KOD_ERRflag--;
	}
	if(KOD_ERRflag == 1){
		free(nurb->S);
	}

	return KOD_ERR;
}

// NURBS曲面配列のメモリー解放
void NURBS_Func::Free_NurbsS_1DArray(NURBSS *a,int num)
{
	for(int i=0;i<num;i++)
		Free_NurbsS(&a[i]);
}

// NURBS曲面のメモリー解放
void NURBS_Func::Free_NurbsS(NURBSS *a)
{
	free(a->S);
	free(a->T);
	FreeMatrix(a->W,a->K[0]);
	FreeCoord2(a->cp,a->K[0]);
}

// トリム面のメモリー確保
int NURBS_Func::New_TrmS(TRMS *trms,int num)
{
	if((trms->pTI = (CONPS **)malloc(sizeof(CONPS *)*num)) == NULL){
		return KOD_ERR;
	}

	return KOD_TRUE;
}

// トリム面配列のメモリー解放
void NURBS_Func::Free_TrmS_1DArray(TRMS *a,int num)
{
	for(int i=0;i<num;i++)
		Free_TrmS(&a[i]);
}

// トリム面のメモリー解放
void NURBS_Func::Free_TrmS(TRMS *a)
{
	free(a->pTI);
}

// 複合曲線のメモリー確保
int NURBS_Func::New_CompC(COMPC *compc,int num)
{
	if((compc->DEType = (int *)malloc(sizeof(int)*num)) == NULL){
		return KOD_ERR;
	}
	
	if((compc->pDE = (COMPELEM **)malloc(sizeof(COMPELEM *)*num)) == NULL){
		free(compc->DEType);
		return KOD_ERR;
	}

	compc->N = num;

	return KOD_TRUE;
}

// 複合曲線配列のメモリー解放
void NURBS_Func::Free_CompC_1DArray(COMPC *a,int num)
{
	for(int i=0;i<num;i++)
		Free_CompC(&a[i]);
}

// 複合曲線のメモリー解放
void NURBS_Func::Free_CompC(COMPC *a)
{
	free(a->DEType);
	free(a->pDE);
}


// 1つのNurbs曲線を生成する
int NURBS_Func::GenNurbsC(NURBSC *Nurbs,int K,int M,int N,double T[],double W[],Coord cp[],double V[],int prop[],int euflag)
{
	int i;

	Nurbs->K = K;
	Nurbs->M = M;
	Nurbs->N = N;
	Nurbs->V[0] = V[0];
	Nurbs->V[1] = V[1];
	Nurbs->T = (double *)malloc(sizeof(double)*Nurbs->N);
	Nurbs->W = (double *)malloc(sizeof(double)*Nurbs->K);
	Nurbs->cp = (Coord *)malloc(sizeof(Coord)*Nurbs->K);
	Nurbs->EntUseFlag = euflag;
	
	for(i=0;i<4;i++){
		Nurbs->prop[i] = prop[i];
	}
	for(i=0;i<N;i++){
		Nurbs->T[i] = T[i];
	}
	for(i=0;i<K;i++){
		Nurbs->W[i] = W[i];
		Nurbs->cp[i] = SetCoord(cp[i]);
	}
	Nurbs->Dstat.Color[0] = Nurbs->Dstat.Color[1] = Nurbs->Dstat.Color[2] = 1.0;
	Nurbs->Dstat.Color[3] = 0.5;

	return KOD_TRUE;
}

// 1つのNurbs曲線を生成する(NURBS曲線のコピー)(オーバーロード)
// 引数　*Nurbs:新たに生成するNURBS曲線　nurb:代入元
int NURBS_Func::GenNurbsC(NURBSC *Nurbs,NURBSC nurb)
{
	int i;

	Nurbs->K = nurb.K;
	Nurbs->M = nurb.M;
	Nurbs->N = nurb.N;
	
	Nurbs->T = (double *)malloc(sizeof(double)*Nurbs->N);
	Nurbs->W = (double *)malloc(sizeof(double)*Nurbs->K);
	Nurbs->cp = (Coord *)malloc(sizeof(Coord)*Nurbs->K);
	for(i=0;i<nurb.N;i++){
		Nurbs->T[i] = nurb.T[i];
	}
	for(i=0;i<nurb.K;i++){
		Nurbs->W[i] = nurb.W[i];
		Nurbs->cp[i] = SetCoord(nurb.cp[i]);
	}

	return KOD_TRUE;
}

// GenNurbsC()によって生成されたNURBS曲線を削除する
void NURBS_Func::DelNurbsC(NURBSC *Nurbs)
{
	
	NURBS_Func hbody;
	hbody.Free_NurbsC(Nurbs);
}

// 1つのNurbs曲面を生成する(NURBS曲面のコピー)
// 引数　Mu,Mv:階数  Ku,Kv:コントロールポイントの数  *S,*T:u,v方向ノットベクトル  **W:ウエイト
// **Cp:コントロールポイント  U_s,U_e,V_s,V_e:u方向ノットベクトルの開始値,終了値
int NURBS_Func::GenNurbsS(NURBSS *Nurbs,int Mu,int Mv,int Ku,int Kv,double *S,double *T,double **W,Coord **Cp,double U_s,double U_e,double V_s,double V_e)
{
	Nurbs->K[0] = Ku;
	Nurbs->K[1] = Kv;
	Nurbs->M[0] = Mu;
	Nurbs->M[1] = Mv;
	Nurbs->U[0] = U_s;
	Nurbs->U[1] = U_e;
	Nurbs->V[0] = V_s;
	Nurbs->V[1] = V_e;
	Nurbs->N[0] = Mu+Ku;
	Nurbs->N[1] = Mv+Kv;

	for(int i=0;i<5;i++)
		Nurbs->prop[i] = 0;

	Nurbs->Dstat.Color[0] = Nurbs->Dstat.Color[1] = Nurbs->Dstat.Color[2] = 0.2;
	Nurbs->Dstat.Color[3] = 0.5;

	if(New_NurbsS(Nurbs,Nurbs->K,Nurbs->N) == KOD_ERR){
		SetMessage("NURBS_Func KOD_ERROR:fail to allocate memory");
		return KOD_ERR;
	}

	//Nurbs->S = (double *)malloc(sizeof(double)*Nurbs->N[0]);
	//Nurbs->T = (double *)malloc(sizeof(double)*Nurbs->N[1]);
	//Nurbs->W = NewMatrix(Nurbs->K[0],Nurbs->K[1]);
	//Nurbs->cp = NewCoord2(Nurbs->K[0],Nurbs->K[1]);

	for(int i=0;i<Nurbs->N[0];i++)
		Nurbs->S[i] = S[i];

	for(int i=0;i<Nurbs->N[1];i++)
		Nurbs->T[i] = T[i];

	for(int i=0;i<Nurbs->K[0];i++){
		for(int j=0;j<Nurbs->K[1];j++){
			Nurbs->W[i][j] = W[i][j];
			Nurbs->cp[i][j] = SetCoord(Cp[i][j]);
		}
	}

	return KOD_TRUE;
}


// 1つのNurbs曲面を生成する(NURBS曲面のコピー)
// 引数　*Nurbs:新たに生成するNURBS曲面　nurb:代入元
int NURBS_Func::GenNurbsS(NURBSS *Nurbs,NURBSS nurb)
{
	NURBS_Func hbody;
	int i,j;

	for(i=0;i<2;i++){
		Nurbs->K[i] = nurb.K[i];
		Nurbs->M[i] = nurb.M[i];
		Nurbs->N[i] = nurb.N[i];
		Nurbs->U[i] = nurb.U[i];
		Nurbs->V[i] = nurb.V[i];
	}
	for(i=0;i<5;i++)
		Nurbs->prop[i] = nurb.prop[i];

	Nurbs->Dstat = nurb.Dstat;

	// メモリー確保
	if(hbody.New_NurbsS(Nurbs,Nurbs->K,Nurbs->N) == KOD_ERR){
		SetMessage("NURBS_Func KOD_ERROR:fail to allocate memory");
		return KOD_ERR;
	}

	for(i=0;i<Nurbs->N[0];i++)
		Nurbs->S[i] = nurb.S[i];
	for(i=0;i<Nurbs->N[1];i++)
		Nurbs->T[i] = nurb.T[i];
	for(i=0;i<Nurbs->K[0];i++){
		for(j=0;j<Nurbs->K[1];j++){
			Nurbs->W[i][j] = nurb.W[i][j];
			Nurbs->cp[i][j] = SetCoord(nurb.cp[i][j]);
		}
	}

	return KOD_TRUE;
}

// NurbsCを原点を通るAxis回りにdegだけ回転させた回転サーフェスNurbsSを生成する
// Axisは単位ベクトルとする
int NURBS_Func::GenRotNurbsS(NURBSS *NurbsS,NURBSC NurbsC,Coord Axis,double deg)
{
	// 360度以外は未実装
	if(deg != 360){
		SetMessage("Unsupported");
		return KOD_ERR;
	}

	Axis = NormalizeVec(Axis);		// 正規化

	// NurbsSを生成
	double S[12] = {0,0,0,0.25,0.25,0.5,0.5,0.75,0.75,1,1,1};		// u方向ノットベクトル
	double **W;			// ウエイト
	Coord  **Cp;		// コントロールポイント
	if((W = NewMatrix(9,NurbsC.K)) == NULL){
		SetMessage("NURBS_Func KOD_ERROR:fail to allocate memory");
		return KOD_ERR;
	}
	if((Cp = NewCoord2(9,NurbsC.K)) == NULL){
		SetMessage("NURBS_Func KOD_ERROR:fail to allocate memory");
		FreeMatrix(W,9);
		return KOD_ERR;
	}
	for(int i=0;i<9;i++){		// u方向
		for(int j=0;j<NurbsC.K;j++){		// v方向
			Coord Q_ = CalcRotVec(NurbsC.cp[j],Axis,(double)i*PI/4);		// 元々のNURBS曲線上のコントロールポイントをAxis周りに45度回転
			Coord P = CalcNormalLine(NurbsC.cp[j],SetCoord(0,0,0),Axis);	// Axis上の回転中心の座標
			Coord PQ_ = SubCoord(Q_,P);										// PQ_ベクトルを生成
			if(i%2 == 0){													// i=0,2,4,6のとき
				W[i][j] = 1;												// ウエイトは1
				Cp[i][j] = SetCoord(Q_);									// Q_がそのままコントロールポイントになる
			}
			else{															// i=1,3,5,7のとき
				W[i][j] = cos(PI/4);										// ウエイト計算
				Cp[i][j] = AddCoord(MulCoord(PQ_,1/cos(PI/4)),P);			// コントロールポイント計算
			}
		}
	}

	GenNurbsS(NurbsS,3,NurbsC.M,9,NurbsC.K,S,NurbsC.T,W,Cp,0,1,0,1);		// NURBS曲面生成

	FreeMatrix(W,2);
	FreeCoord2(Cp,2);

	return KOD_TRUE;
}

// 1つのNURBS曲線からある軸方向にある距離だけスイープさせたスイープサーフェスを生成する
int NURBS_Func::GenSweepNurbsS(NURBSS *NurbsS,NURBSC NurbsC,Coord Axis,double Len)
{
	Axis = NormalizeVec(Axis);		// 正規化

	// NurbsSを生成
	double T[4] = {0,0,1,1};		// v方向ノットベクトル
	double **W;			// ウエイト
	Coord  **Cp;		// コントロールポイント
	if((W = NewMatrix(NurbsC.K,2)) == NULL){
		SetMessage("NURBS_Func KOD_ERROR:fail to allocate memory");
		return KOD_ERR;
	}
	if((Cp = NewCoord2(NurbsC.K,2)) == NULL){
		SetMessage("NURBS_Func KOD_ERROR:fail to allocate memory");
		FreeMatrix(W,2);
		return KOD_ERR;
	}

	for(int i=0;i<NurbsC.K;i++){
		for(int j=0;j<2;j++){
			W[i][j] = NurbsC.W[i];	// ウエイト計算
			if(j==0)
				Cp[i][j] = SetCoord(NurbsC.cp[i]);		// コントロールポイント計算
			else
				Cp[i][j] = SetCoord(AddCoord(NurbsC.cp[i],MulCoord(Axis,Len)));		// コントロールポイント計算
		}
	}

	GenNurbsS(NurbsS,NurbsC.M,2,NurbsC.K,2,NurbsC.T,T,W,Cp,0,1,NurbsC.V[0],NurbsC.V[1]);	// NURBS曲面生成

	FreeMatrix(W,2);
	FreeCoord2(Cp,2);

	return KOD_TRUE;
}

// NURBS曲面上のu方向パラメータ値を固定したときのアイソパラメトリックNURBS曲線を生成
// 引数    *P:アイソパラメトリック曲線生成元のNURBS曲面   u:u方向の固定パラメータ   *C:生成されたアイソパラメトリック曲線
// 返値    KOD_ERR:引数uが*Pのuパラメータ範囲外
int NURBS_Func::GenIsoparamCurveU(NURBSS *P,double u,NURBSC *C)
{
	if(u < P->U[0] || u > P->U[1])	return KOD_ERR;

	double V[2] = {P->V[0],P->V[1]};	// ノットベクトルの範囲
	int prop[4] = {0,0,1,0};		// パラメータ

	Coord *Q = NewCoord1(P->K[1]);	// コントロールポイント
	double *W = NewVector(P->K[1]);	// ウェイト

	for(int i=0;i<P->K[1];i++){
		InitCoord(&Q[i]);
		W[i] = 0;
		for(int j=0;j<P->K[0];j++){
			double bs = CalcBSbasis(u,P->S,P->N[0],j,P->M[0]);
			Q[i] = AddCoord(Q[i],MulCoord(P->cp[j][i],bs));
			W[i] += bs*P->W[j][i]; 
		}
	}

	GenNurbsC(C,P->K[1],P->M[1],P->N[1],P->T,W,Q,V,prop,0);

	FreeCoord1(Q);
	FreeVector(W);

	return KOD_TRUE;
}

// NURBS曲面上のv方向パラメータ値を固定したときのアイソパラメトリックNURBS曲線を生成
// 引数    *S:アイソパラメトリック曲線生成元のNURBS曲面   v:v方向の固定パラメータ   *C:生成されたアイソパラメトリック曲線
// 返値    KOD_ERR:引数vが*Pのuパラメータ範囲外
int NURBS_Func::GenIsoparamCurveV(NURBSS *P,double v,NURBSC *C)
{
	if(v < P->V[0] || v > P->V[1])	return KOD_ERR;

	double V[2] = {P->U[0],P->U[1]};	// ノットベクトルの範囲
	int prop[4] = {0,0,1,0};			// パラメータ

	Coord *Q = NewCoord1(P->K[0]);		// コントロールポイント
	double *W = NewVector(P->K[0]);		// ウェイト

	for(int i=0;i<P->K[0];i++){
		InitCoord(&Q[i]);
		W[i] = 0;
		for(int j=0;j<P->K[1];j++){
			double bs = CalcBSbasis(v,P->T,P->N[1],j,P->M[1]);
			Q[i] = AddCoord(Q[i],MulCoord(P->cp[i][j],bs));
			W[i] += bs*P->W[i][j]; 
		}
	}

	GenNurbsC(C,P->K[0],P->M[0],P->N[0],P->S,W,Q,V,prop,0);

	FreeCoord1(Q);
	FreeVector(W);

	return KOD_TRUE;
}

// GenNurbsS()によって生成されたNURBS曲面を削除する
void NURBS_Func::DelNurbsS(NURBSS *Nurbs)
{
	NURBS_Func hbody;
	hbody.Free_NurbsS(Nurbs);
}

// トリム面を有するNURBS曲面を生成する
int NURBS_Func::GenTrimdNurbsS(TRIMD_NURBSS *TNurbs,TRIMD_NURBSS  tnurb)
{
	NURBSS *nurbsS;
	NURBSC *nurbsC;
	CONPS *conps_o,*conps_i;
	COMPC *compc_o,*compc_i;
	int curve_num=0;

	nurbsS = (NURBSS *)malloc(sizeof(NURBSS));		// NURBS曲面のメモリー確保
	conps_o = (CONPS *)malloc(sizeof(CONPS));		// 外側トリムを構成する面上線のメモリー確保
	compc_o = (COMPC *)malloc(sizeof(COMPC));		// 外側トリムを構成する複合曲線のメモリー確保

	// トリム面を構成するNURBS曲線の総数をカウント
	for(int i=0;i<tnurb.n2;i++){
		for(int j=0;j<tnurb.pTI[i]->pB->CompC.N;j++){
			curve_num++;
		}
	}
	curve_num += tnurb.pTO->pB->CompC.N;

	nurbsC = (NURBSC *)malloc(sizeof(NURBSC)*curve_num);	// トリム面を構成するNURBS曲線の数だけNURBS曲線のメモリーを確保

	GenNurbsS(nurbsS,*tnurb.pts);							// 新たなNURBS曲面を1つ得る
	TNurbs->pts = nurbsS;									// NURBS曲面をトリム面に関連付ける

	New_TrmS(TNurbs,tnurb.n2);						// トリム面のメモリー確保

	conps_i = (CONPS *)malloc(sizeof(CONPS)*tnurb.n2);		// 内側を構成する面上線のメモリー確保
	compc_i = (COMPC *)malloc(sizeof(COMPC)*tnurb.n2);		// 内側を構成する複合曲線のメモリー確保

	// NURBS曲線をトリム部分を構成するNURBS曲線に関連付ける
	// 外周トリム
	TNurbs->pTO = conps_o;
	New_CompC(compc_o,tnurb.pTO->pB->CompC.N);
	for(int i=0;i<tnurb.pTO->pB->CompC.N;i++){
		GenNurbsC(&nurbsC[i],tnurb.pTO->pB->CompC.pDE[i]->NurbsC);
		compc_o->pDE[i] = (COMPELEM *)(&nurbsC[i]);
		compc_o->DEType[i] = tnurb.pTO->pB->CompC.DEType[i];
	}
	TNurbs->pTO->pB = (CURVE *)compc_o;
	TNurbs->pTO->BType = tnurb.pTO->BType;
	TNurbs->pTO->pB->CompC.DegeFlag = tnurb.pTO->pB->CompC.DegeFlag;
	TNurbs->pTO->pB->CompC.DegeNurbs = tnurb.pTO->pB->CompC.DegeNurbs;

	// 内周トリム
	curve_num = 0;
	for(int i=0;i<tnurb.n2;i++){
		TNurbs->pTI[i] = &(conps_i[i]);
		New_CompC(&compc_i[i],tnurb.pTI[i]->pB->CompC.N);
		for(int j=0;j<tnurb.pTI[i]->pB->CompC.N;j++){
			GenNurbsC(&nurbsC[tnurb.pTO->pB->CompC.N+curve_num],tnurb.pTI[i]->pB->CompC.pDE[j]->NurbsC);
			compc_i[i].pDE[j] = (COMPELEM *)(&nurbsC[tnurb.pTO->pB->CompC.N+curve_num]);
			compc_i[i].DEType[j] = tnurb.pTI[i]->pB->CompC.DEType[j];
			curve_num++;
		}
		TNurbs->pTI[i]->pB = (CURVE *)(&(compc_i[i]));
		TNurbs->pTI[i]->BType = tnurb.pTI[i]->BType;
		TNurbs->pTI[i]->pB->CompC.DegeFlag = tnurb.pTI[i]->pB->CompC.DegeFlag;
		TNurbs->pTI[i]->pB->CompC.DegeNurbs = tnurb.pTI[i]->pB->CompC.DegeNurbs;
	}

	TNurbs->n1 = tnurb.n1;
	TNurbs->n2 = tnurb.n2;

	return KOD_TRUE;
}

// GenTrimdNurbsS()によって生成されたトリム面を削除する
int NURBS_Func::DelTrimdNurbsS(TRIMD_NURBSS *TNurbs)
{
	NURBS_Func hbody;
	int curve_num = 0;

	// トリム面を構成する全てのNURBS曲線の本数を調べる
	for(int i=0;i<TNurbs->n2;i++){
		for(int j=0;j<TNurbs->pTI[i]->pB->CompC.N;j++){
			curve_num++;
		}
	}
	curve_num += TNurbs->pTO->pB->CompC.N;

	hbody.Free_NurbsC_1DArray((NURBSC *)TNurbs->pTO->pB->CompC.pDE[0],curve_num);		// トリム面を構成する全てのNURBS曲線パラメータのメモリー解放

	hbody.Free_NurbsS(TNurbs->pts);						// トリム面を構成するNURBS曲面パラメータのメモリー解放
	free(TNurbs->pts);								// トリム面を構成するNURBS曲面のメモリー解放

	hbody.Free_NurbsC(&TNurbs->pTO->pB->CompC.DegeNurbs);	// トリム面外周を構成する複合曲線を構成する縮退用NURBS曲線のメモリー解放
	hbody.Free_CompC((COMPC *)TNurbs->pTO->pB);			// トリム面外周を構成する複合曲線を構成するNURBS曲線のメモリー解放
	free(TNurbs->pTO->pB);							// トリム面外周を構成する複合曲線のメモリー解放
	free(TNurbs->pTO);								// トリム面外周を構成する面上線のメモリー解放

	for(int i=0;i<TNurbs->n2;i++){
		hbody.Free_NurbsC(&TNurbs->pTI[i]->pB->CompC.DegeNurbs);	// トリム面内周を構成する複合曲線を構成する縮退用NURBS曲線のメモリー解放
		hbody.Free_CompC((COMPC *)TNurbs->pTI[i]->pB);	// トリム面内周を構成する複合曲線を構成するNURBS曲線のメモリー解放
		free(TNurbs->pTI[i]->pB);					// トリム面内周を構成する複合曲線のメモリー解放
	}
	hbody.Free_TrmS(TNurbs);								// トリム面パラメータのメモリー解放

	return KOD_TRUE;
}

// 指定したノットtでのNURBS曲線の座標値を求める
Coord NURBS_Func::CalcNurbsCCoord(NURBSC *NurbsC,double t)
{
	Coord p;
	Coord bscpw;
	double bsw=0;
	double bs=0;
	int i;

	InitCoord(&bscpw);	// 初期化

	for(i=0;i<NurbsC->K;i++){
		bs = CalcBSbasis(t,NurbsC->T,NurbsC->N,i,NurbsC->M);		// Bスプライン基底関数を求める
		bsw += bs*NurbsC->W[i];							// 分母
		bscpw = AddCoord(bscpw,MulCoord(NurbsC->cp[i],bs*NurbsC->W[i]));	// 分子
	}
	
	p = DivCoord(bscpw,bsw);	// 座標値を求める

	return p;
}

// 指定したノットt群でのNURBS曲線の座標値を求める
// 引数   *NurbsS:NURBS曲面へのポインタ   Ptnum:求める点群の数   *T:tパラメータ群を格納した配列
//        *Pt:実座標値を格納
void NURBS_Func::CalcNurbsCCoords(NURBSC *NurbsC,int Ptnum,double *T,Coord *Pt)
{
	for(int i=0;i<Ptnum;i++){
		Pt[i] = CalcNurbsCCoord(NurbsC,T[i]);
	}
}

// 指定したノットu,vでのNURBS曲面の座標値を求める
Coord NURBS_Func::CalcNurbsSCoord(NURBSS *NurbsS,double div_u,double div_v)
{
	int i,j;
	double bs_u,bs_v;		// u,v方向Bスプライン基底関数
	double bsw=0;			// 分母
	Coord bscpw;			// 分子

	InitCoord(&bscpw);		// 初期化

	for(i=0;i<NurbsS->K[0];i++){
		bs_u = CalcBSbasis(div_u,NurbsS->S,NurbsS->N[0],i,NurbsS->M[0]);			// u方向Bスプライン基底関数を求める
		for(j=0;j<NurbsS->K[1];j++){
			bs_v = CalcBSbasis(div_v,NurbsS->T,NurbsS->N[1],j,NurbsS->M[1]);		// v方向Bスプライン基底関数を求める
			bsw += bs_u*bs_v*NurbsS->W[i][j];
			bscpw = AddCoord(bscpw,MulCoord(NurbsS->cp[i][j],bs_u*bs_v*NurbsS->W[i][j]));
		}
	}
	//if(fabs(bsw) < APPROX_ZERO)
		//return(SetCoord(0,0,0));

	return(DivCoord(bscpw,bsw));
}

// 指定したノットu,v群でのNURBS曲面の座標値群を求める
// 引数   *NurbsS:NURBS曲面へのポインタ   Ptnum:求める点群の数   *UV:u,vパラメータ群を格納したCoord型配列(UV[].xにu方向、UV[].ｙにV方向のパラメータを格納しておくこと)
//        *Pt:実座標値を格納
void NURBS_Func::CalcNurbsSCoords(NURBSS *NurbsS,int Ptnum,Coord *UV,Coord *Pt)
{
	for(int i=0;i<Ptnum;i++){
		Pt[i] = CalcNurbsSCoord(NurbsS,UV[i].x,UV[i].y);
	}
}

// Bスプライン基底関数を計算し、計算結果を返す
// t:ノット　knot[]:ノットベクトル  N:ノットベクトルの数  I:Bspl基底関数下添字の1つ目(0〜)  M:階数(Bspl基底関数下添字の2つ目)  
// N_I,M(t)
double NURBS_Func::CalcBSbasis(double t, double knot[],int N,int I,int M)
{
	// 階数(order)が1の時
	if(M == 1){
		// 注目中のノットの値がノットベクトルの終端値と同じ場合、基底関数が1を取りうる範囲をknot[I+1]も含むようにする
		// こうしないと、このときだけ全ての基底関数値が0になってしまう。
		if(t==knot[N-1]){
			if(knot[I] <= t && t <= knot[I+1])	return 1.0;
			else		return 0.0;
		}
		else{
			if(knot[I] <= t && t < knot[I+1])	return 1.0;
			else	return 0.0;
		}
	}

	// それ以外の時
	else{
		double n1=0.0;
		double n2=0.0;
		double denom;

		denom = knot[I+M-1] - knot[I];	// 分母
		if(denom > 0.0){
			n1 = (t-knot[I])/denom * CalcBSbasis(t,knot,N,I,M-1);		// 1項目
		}

		denom = knot[I+M] - knot[I+1];
		if(denom > 0.0){
			n2 = (knot[I+M]-t)/denom * CalcBSbasis(t,knot,N,I+1,M-1);	// 2項目
		}

		return(n1+n2);
	}
}


// Bスプライン基底関数の1階微分係数を求める
// t:ノット　knot[]:ノットベクトル  N:ノットベクトルの数  I:注目中のコントロールポイント  M:階数
double NURBS_Func::CalcDiffBSbasis(double t,double knot[],int N,int I,int M)
{
	double n1 = knot[I+M-1]-knot[I];
	double n2 = knot[I+M]-knot[I+1];

	if(n1 != 0.0) n1 = (M-1)/n1*CalcBSbasis(t,knot,N,I,M-1);
	
	if(n2 != 0.0) n2 = (M-1)/n2*CalcBSbasis(t,knot,N,I+1,M-1);
	
	return(n1-n2);
}

// Bスプライン基底関数のN階微分係数を求める
// t:ノット　knot[]:ノットベクトル  N:ノットベクトルの数  I:注目中のコントロールポイント  M:階数  Dn:微分階数
double NURBS_Func::CalcDiffBSbasisN(double t,double knot[],int N,int I,int M, int Dn)
{
	double n1 = knot[I+M-1]-knot[I];
	double n2 = knot[I+M]-knot[I+1];

	if(Dn==0){
		return(CalcBSbasis(t,knot,N,I,M));
	}
	if(Dn==1){
		return(CalcDiffBSbasis(t,knot,N,I,M));
	}
	if(n1 != 0) n1 = (M-1)/n1*CalcDiffBSbasisN(t,knot,N,I,M-1,Dn-1);
	if(n2 != 0) n2 = (M-1)/n2*CalcDiffBSbasisN(t,knot,N,I+1,M-1,Dn-1);

	return(n1-n2);
}


// NURBS曲線の1階微分係数を求める
Coord NURBS_Func::CalcDiffNurbsC(NURBSC *NurbsC,double t)
{
	Coord Ft,diff_Ft;		// NURBS曲線の分子
	double Gt,diff_Gt;		// NURBS曲線の分母
	double bs,diff_bs;		// Bスプライン基底関数
	Coord p;
	int i;

	InitCoord(&Ft);
	InitCoord(&diff_Ft);
	Gt = 0;
	diff_Gt = 0;

	// 各係数算出
	for(i=0;i<NurbsC->K;i++){
		bs = CalcBSbasis(t,NurbsC->T,NurbsC->N,i,NurbsC->M);
		diff_bs = CalcDiffBSbasis(t,NurbsC->T,NurbsC->N,i,NurbsC->M);

		Ft = AddCoord(Ft,MulCoord(NurbsC->cp[i],bs*NurbsC->W[i]));
		diff_Ft = AddCoord(diff_Ft,MulCoord(NurbsC->cp[i],diff_bs*NurbsC->W[i]));

		Gt += bs*NurbsC->W[i];
		diff_Gt += diff_bs*NurbsC->W[i];
	}
	if(fabs(Gt) < APPROX_ZERO)	return(SetCoord(0,0,0));

	// 1階微分を求める
	p = SubCoord(DivCoord(diff_Ft,Gt),DivCoord(MulCoord(Ft,diff_Gt),Gt*Gt));

	return p;
}

// NURBS曲線の2階微分係数を求める
Coord NURBS_Func::CalcDiff2NurbsC(NURBSC *NurbsC,double t)
{
	double w0=0;
	double w1=0;
	double w2=0;
	Coord  A2=SetCoord(0,0,0);
	Coord  P0=SetCoord(0,0,0);
	Coord  P1=SetCoord(0,0,0);

	P0 = CalcNurbsCCoord(NurbsC,t);
	P1 = CalcDiffNurbsC(NurbsC,t);

	for(int i=0;i<NurbsC->K;i++){
		w0 += CalcBSbasis(t,NurbsC->T,NurbsC->N,i,NurbsC->M) * NurbsC->W[i];
		w1 += CalcDiffBSbasis(t,NurbsC->T,NurbsC->N,i,NurbsC->M) * NurbsC->W[i];
		w2 += CalcDiffBSbasisN(t,NurbsC->T,NurbsC->N,i,NurbsC->M,2) * NurbsC->W[i];
		A2 = AddCoord(A2,MulCoord(NurbsC->cp[i],CalcDiffBSbasisN(t,NurbsC->T,NurbsC->N,i,NurbsC->M,2) * NurbsC->W[i]));
	}

	return DivCoord(SubCoord(A2,AddCoord(MulCoord(P1,2*w1),MulCoord(P0,2*w2))),w0);
}

// NURBS曲線のr階微分係数を求める
Coord NURBS_Func::CalcDiffNNurbsC(NURBSC *NurbsC,int r,double t)
{
	if(!r)
		return CalcNurbsCCoord(NurbsC,t);

	Coord Ar = InitCoord();
	double W = 0;
	for(int i=0;i<NurbsC->K;i++){
		double bsr = CalcDiffBSbasisN(t,NurbsC->T,NurbsC->N,i,NurbsC->M,r);
		Ar = AddCoord(Ar,MulCoord(NurbsC->cp[i],bsr*NurbsC->W[i]));
		W += NurbsC->W[i]*CalcBSbasis(t,NurbsC->T,NurbsC->N,i,NurbsC->M);
	}

	Coord Br = InitCoord();
	for(int i=1;i<=r;i++){
		double Wi = 0;
		for(int j=0;j<NurbsC->K;j++){
			double bsi = CalcDiffBSbasisN(t,NurbsC->T,NurbsC->N,j,NurbsC->M,i);
			Wi += bsi*NurbsC->W[j];
		}
		if(Wi == 0.0)  return(InitCoord());
		Br = AddCoord(Br,MulCoord(CalcDiffNNurbsC(NurbsC,r-i,t),(double)nCr(r,i)*Wi));	// 回帰
	}

	return (DivCoord(SubCoord(Ar,Br),W));
}

// NURBS曲面のu方向の1階微分係数を得る
Coord NURBS_Func::CalcDiffuNurbsS(NURBSS *NurbsS,double div_u,double div_v)
{
	int i,j;
	Coord Ft,diff_Ft;
	double Gt,diff_Gt;
	double bs_u,bs_v;		// u,v方向Bスプライン基底関数
	double diff_bs_u;
	Coord p;

	InitCoord(&Ft);
	InitCoord(&diff_Ft);
	Gt = 0;
	diff_Gt = 0;

	for(i=0;i<NurbsS->K[0];i++){
		bs_u = CalcBSbasis(div_u,NurbsS->S,NurbsS->N[0],i,NurbsS->M[0]);				// u方向Bスプライン基底関数を求める
		diff_bs_u = CalcDiffBSbasis(div_u,NurbsS->S,NurbsS->N[0],i,NurbsS->M[0]);	// u方向Bスプライン基底関数の1階微分を求める
		for(j=0;j<NurbsS->K[1];j++){
			bs_v = CalcBSbasis(div_v,NurbsS->T,NurbsS->N[1],j,NurbsS->M[1]);			// v方向Bスプライン基底関数を求める
			Ft = AddCoord(Ft,MulCoord(NurbsS->cp[i][j],bs_u*bs_v*NurbsS->W[i][j]));
			diff_Ft = AddCoord(diff_Ft,MulCoord(NurbsS->cp[i][j],diff_bs_u*bs_v*NurbsS->W[i][j]));
			Gt += bs_u*bs_v*NurbsS->W[i][j];
			diff_Gt += diff_bs_u*bs_v*NurbsS->W[i][j];
		}
	}
	if(fabs(Gt) < APPROX_ZERO_H)	return(SetCoord(0,0,0));

	// 1階微分を求める
	p = SubCoord(DivCoord(diff_Ft,Gt),DivCoord(MulCoord(Ft,diff_Gt),Gt*Gt));

	return p;
}

// NURBS曲面のv方向の1階微分係数を得る
Coord NURBS_Func::CalcDiffvNurbsS(NURBSS *NurbsS,double div_u,double div_v)
{
	int i,j;
	Coord Ft,diff_Ft;
	double Gt,diff_Gt;
	double bs_u,bs_v;		// u,v方向Bスプライン基底関数
	double diff_bs_v;
	Coord p;

	InitCoord(&Ft);
	InitCoord(&diff_Ft);
	Gt = 0;
	diff_Gt = 0;

	for(i=0;i<NurbsS->K[0];i++){
		bs_u = CalcBSbasis(div_u,NurbsS->S,NurbsS->N[0],i,NurbsS->M[0]);				// u方向Bスプライン基底関数を求める
		for(j=0;j<NurbsS->K[1];j++){
			bs_v = CalcBSbasis(div_v,NurbsS->T,NurbsS->N[1],j,NurbsS->M[1]);				// v方向Bスプライン基底関数を求める
			diff_bs_v = CalcDiffBSbasis(div_v,NurbsS->T,NurbsS->N[1],j,NurbsS->M[1]);	// v方向Bスプライン基底関数の1階微分を求める
			Ft = AddCoord(Ft,MulCoord(NurbsS->cp[i][j],bs_u*bs_v*NurbsS->W[i][j]));
			diff_Ft = AddCoord(diff_Ft,MulCoord(NurbsS->cp[i][j],bs_u*diff_bs_v*NurbsS->W[i][j]));
			Gt += bs_u*bs_v*NurbsS->W[i][j];
			diff_Gt += bs_u*diff_bs_v*NurbsS->W[i][j];
		}
	}
	if(fabs(Gt) < APPROX_ZERO_H)	return(SetCoord(0,0,0));

	// 1階微分を求める
	p = SubCoord(DivCoord(diff_Ft,Gt),DivCoord(MulCoord(Ft,diff_Gt),Gt*Gt));

	return p;
}

// NURBS曲面の各方向を任意階微分したときの微分係数を求める
// 引数   *S:NURBS曲面へのポインタ   k:u方向の微分階数    l:v方向の微分階数   u,v:u方向v方向それぞれのパラメータ
Coord NURBS_Func::CalcDiffNNurbsS(NURBSS *S,int k,int l,double u,double v)
{
	double w = CalcDiffNurbsSDenom(S,0,0,u,v);
	Coord  A = CalcDiffNurbsSNumer(S,k,l,u,v);
	Coord  B = SetCoord(0,0,0);
	Coord  C = SetCoord(0,0,0);
	Coord  D = SetCoord(0,0,0);

	if(!k && !l)
		return(CalcNurbsSCoord(S,u,v));
		
	for(int i=1;i<=k;i++)
		B = AddCoord(B,MulCoord(CalcDiffNNurbsS(S,k-i,l,u,v),nCr(k,i)*CalcDiffNurbsSDenom(S,i,0,u,v)));
	for(int j=1;j<=l;j++)
		C = AddCoord(C,MulCoord(CalcDiffNNurbsS(S,k,l-j,u,v),nCr(l,j)*CalcDiffNurbsSDenom(S,0,j,u,v)));
	for(int i=1;i<=k;i++){
		for(int j=1;j<=l;j++){
			D = AddCoord(D,MulCoord(CalcDiffNNurbsS(S,k-i,l-j,u,v),nCr(l,j)*CalcDiffNurbsSDenom(S,i,j,u,v)));
		}
		D = MulCoord(D,nCr(k,i));
	}
	return(DivCoord(SubCoord(A,AddCoord(B,AddCoord(C,D))),w));

}

// NURBS曲面分母の各方向を任意階微分したときの微分係数を求める
// 引数   *S:NURBS曲面へのポインタ   k:u方向の微分階数    l:v方向の微分階数   u,v:u方向v方向それぞれのパラメータ
double NURBS_Func::CalcDiffNurbsSDenom(NURBSS *S,int k,int l,double u,double v)
{
	double w=0;
	for(int i=0;i<S->K[0];i++){
		double Nk = CalcDiffBSbasisN(u,S->S,S->N[0],i,S->M[0],k);		// u方向のk階微分
		for(int j=0;j<S->K[1];j++){
			double Nl = CalcDiffBSbasisN(v,S->T,S->N[1],j,S->M[1],l);	// v方向のl階微分
			w += Nk*Nl*S->W[i][j];
		}
	}
	return w;
}

// NURBS曲面分子の各方向を任意階微分したときの微分係数を求める
// 引数   *S:NURBS曲面へのポインタ   k:u方向の微分階数    l:v方向の微分階数   u,v:u方向v方向それぞれのパラメータ
Coord NURBS_Func::CalcDiffNurbsSNumer(NURBSS *S,int k,int l,double u,double v)
{
	Coord A=SetCoord(0,0,0);
	for(int i=0;i<S->K[0];i++){
		double Nk = CalcDiffBSbasisN(u,S->S,S->N[0],i,S->M[0],k);		// u方向のk階微分
		for(int j=0;j<S->K[1];j++){
			double Nl = CalcDiffBSbasisN(v,S->T,S->N[1],j,S->M[1],l);	// v方向のl階微分
			A = AddCoord(A,MulCoord(S->cp[i][j],Nk*Nl*S->W[i][j]));
		}
	}
	return A;
}


// NURBS曲面上の(u,v)における法線ベクトルをもとめる
Coord NURBS_Func::CalcNormVecOnNurbsS(NURBSS *nurb,double u,double v)
{
	Coord a = CalcDiffuNurbsS(nurb,u,v);
	Coord b = CalcDiffvNurbsS(nurb,u,v);

	return(NormalizeVec(CalcOuterProduct(a,b)));
}

// NURBS曲面上の(u,v)における法線ベクトルのu方向1階微分をもとめる
// Nu = Suu×Sv + Su×Suv
Coord NURBS_Func::CalcDiffuNormVecOnNurbsS(NURBSS *nurb,double u,double v)
{
	Coord Suu = CalcDiffNNurbsS(nurb,2,0,u,v);
	Coord Suv = CalcDiffNNurbsS(nurb,1,1,u,v);
	Coord Su = CalcDiffuNurbsS(nurb,u,v);
	Coord Sv = CalcDiffvNurbsS(nurb,u,v);

	return (NormalizeVec(AddCoord(CalcOuterProduct(Suu,Sv),CalcOuterProduct(Su,Suv))));
}

// NURBS曲面上の(u,v)における法線ベクトルのv方向1階微分をもとめる
// Nv = Suv×Sv + Su×Svv
Coord NURBS_Func::CalcDiffvNormVecOnNurbsS(NURBSS *nurb,double u,double v)
{
	Coord Suv = CalcDiffNNurbsS(nurb,1,1,u,v);
	Coord Svv = CalcDiffNNurbsS(nurb,0,2,u,v);
	Coord Su = CalcDiffuNurbsS(nurb,u,v);
	Coord Sv = CalcDiffvNurbsS(nurb,u,v);

	return (NormalizeVec(AddCoord(CalcOuterProduct(Suv,Sv),CalcOuterProduct(Su,Svv))));
}

// NURBS曲面上の(u,v)における平均曲率を求める
double NURBS_Func::CalcMeanCurvature(NURBSS *nurb,double u,double v)
{
	Coord du = CalcDiffuNurbsS(nurb,u,v);			// u方向1階微分
	Coord dv = CalcDiffvNurbsS(nurb,u,v);			// v方向1階微分
	double E = CalcInnerProduct(du,du);				// 第1基本量
	double F = CalcInnerProduct(du,dv);				// 第1基本量
	double G = CalcInnerProduct(dv,dv);				// 第1基本量
	Coord duu = CalcDiffNNurbsS(nurb,2,0,u,v);		// u方向2階微分
	Coord dvv = CalcDiffNNurbsS(nurb,0,2,u,v);		// v方向2階微分
	Coord duv = CalcDiffNNurbsS(nurb,1,1,u,v);		// u,v方向各1階微分
	Coord n = CalcNormVecOnNurbsS(nurb,u,v);		// 法線ベクトル
	double L = CalcInnerProduct(duu,n);				// 第2基本量
	double M = CalcInnerProduct(duv,n);				// 第2基本量
	double N = CalcInnerProduct(dvv,n);				// 第2基本量
	double H = -(G*L+E*N-2*F*M)/(E*G-F*F)/2;		// 平均曲率

	return H;
}

// NURBS曲面上の(u,v)における平均曲率法線ベクトルをもとめる
Coord NURBS_Func::CalcMeanCurvatureNormVec(NURBSS *nurb,double u,double v)
{
	Coord n = CalcNormVecOnNurbsS(nurb,u,v);				// 法線ベクトル
	Coord Hn = MulCoord(n,CalcMeanCurvature(nurb,u,v));		// 平均曲率法線ベクトル

	return Hn;
}

// NURBS曲面上の(u,v)におけるガウス曲率を求める
double NURBS_Func::CalcGaussCurvature(NURBSS *nurb,double u,double v)
{
	Coord du = CalcDiffuNurbsS(nurb,u,v);			// u方向1階微分
	Coord dv = CalcDiffvNurbsS(nurb,u,v);			// v方向1階微分
	double E = CalcInnerProduct(du,du);				// 第1基本量
	double F = CalcInnerProduct(du,dv);				// 第1基本量
	double G = CalcInnerProduct(dv,dv);				// 第1基本量
	Coord duu = CalcDiffNNurbsS(nurb,2,0,u,v);		// u方向2階微分
	Coord dvv = CalcDiffNNurbsS(nurb,0,2,u,v);		// v方向2階微分
	Coord duv = CalcDiffNNurbsS(nurb,1,1,u,v);		// u,v方向各1階微分
	Coord n = CalcNormVecOnNurbsS(nurb,u,v);		// 法線ベクトル
	double L = CalcInnerProduct(duu,n);				// 第2基本量
	double M = CalcInnerProduct(duv,n);				// 第2基本量
	double N = CalcInnerProduct(dvv,n);				// 第2基本量
	double K = (L*N-M*M)/(E*G-F*F);					// ガウス曲率

	return K;
}

// NURBS曲面上の(u,v)におけるガウス曲率法線ベクトルをもとめる
Coord NURBS_Func::CalcGaussCurvatureNormVec(NURBSS *nurb,double u,double v)
{
	Coord n = CalcNormVecOnNurbsS(nurb,u,v);				// 法線ベクトル
	Coord Kn = MulCoord(n,CalcGaussCurvature(nurb,u,v));		// ガウス曲率法線ベクトル

	return Kn;
}

// NURBS曲面S(u,v)とNURBS曲面R(w,t)の干渉を検出する(トリム無)
// 引数  *nurbS:NURBS曲面S(u,v) *nurbR:NURBS曲面R(w,t) divnum:パラメータ分割数
// 返値  干渉の有無(干渉有:KOD_TRUE 干渉無:KOD_FALSE)
int NURBS_Func::DetectInterfereNurbsS(NURBSS *nurbR,NURBSS *nurbS,int divnum)
{
	// 各曲面を指定の分割数でuv分割し、それらの点における補助平面を生成して交線上の任意の1点に収束させる
	for(int w=0;w<divnum;w++){
		for(int t=0;t<divnum;t++){
			for(int u=0;u<divnum;u++){
				for(int v=0;v<divnum;v++){
					// 各曲面に分割点を生成する
					double w0 = nurbR->U[0] + (nurbR->U[1] - nurbR->U[0])*(double)w/(double)divnum;
					double t0 = nurbR->V[0] + (nurbR->V[1] - nurbR->V[0])*(double)t/(double)divnum;
					double u0 = nurbS->U[0] + (nurbS->U[1] - nurbS->U[0])*(double)u/(double)divnum;
					double v0 = nurbS->V[0] + (nurbS->V[1] - nurbS->V[0])*(double)v/(double)divnum;
					for(int i=0;i<10;i++){
						// 各種パラメータを算出する
						Coord p0 = CalcNurbsSCoord(nurbR,w0,t0);					// R(w0,t0)となる点(初期点)の座標
						Coord q0 = CalcNurbsSCoord(nurbS,u0,v0);					// S(u0,v0)となる点(初期点)の座標
						Coord rw = CalcDiffuNurbsS(nurbR,w0,t0);					// 点R(w0,t0)のu偏微分(基本ベクトル)
						Coord rt = CalcDiffvNurbsS(nurbR,w0,t0);					// 点R(w0,t0)のv偏微分(基本ベクトル)
						double rwt = CalcEuclid(CalcOuterProduct(rw,rt));
						if(rwt==0.0) break;
						Coord np = DivCoord(CalcOuterProduct(rw,rt),rwt);			// 点R(w0,t0)の単位法線ベクトル
						Coord su = CalcDiffuNurbsS(nurbS,u0,v0);					// 点S(u0,v0)のu偏微分(基本ベクトル)
						Coord sv = CalcDiffvNurbsS(nurbS,u0,v0);					// 点S(u0,v0)のv偏微分(基本ベクトル)
						double suv = CalcEuclid(CalcOuterProduct(su,sv));
						if(suv==0.0) break;
						Coord nq = DivCoord(CalcOuterProduct(su,sv),CalcEuclid(CalcOuterProduct(su,sv)));	// 点S(u0,v0)の単位法線ベクトル
						double npq = CalcEuclid(CalcOuterProduct(np,nq));
						if(npq==0.0) break;
						Coord nn = DivCoord(CalcOuterProduct(np,nq),CalcEuclid(CalcOuterProduct(np,nq)));	// 平面Fpと平面Fqに直交する平面Fnの単位法線ベクトル
						double dp = CalcInnerProduct(p0,np);						// 原点から平面Fpまでの距離
						double dq = CalcInnerProduct(q0,nq);						// 原点から平面Fqまでの距離
						double dn = CalcInnerProduct(p0,nn);						// 原点から平面Fnまでの距離
						Coord cross_nqn = CalcOuterProduct(nq,nn);					// 単位法線ベクトルnq,nnのベクトル積
						Coord cross_nnp = CalcOuterProduct(nn,np);					// 単位法線ベクトルnn,npのベクトル積
						Coord cross_npq = CalcOuterProduct(np,nq);					// 単位法線ベクトルnp,nqのベクトル積
						Coord nume_p_sub =  AddCoord(MulCoord(cross_nqn,dp),MulCoord(cross_nnp,dq));	// 3平面Fp,Fq,Fnの交点pの分子の最初の2項を計算
						Coord nume_p = AddCoord(nume_p_sub,MulCoord(cross_npq,dn));			// pの分子を算出
						double denom_p = CalcScalarTriProduct(np,nq,nn);				// pの分母を算出
						Coord p = DivCoord(nume_p,denom_p);						// pを算出
						Coord deltap0 = SubCoord(p,p0);							// 点pと点p0の差ベクトルを算出
						Coord deltaq0 = SubCoord(p,q0);							// 点pと点q0の差ベクトルを算出
						Coord rw_sub = CalcOuterProduct(rw,np);						// 基本ベクトルrwと法線ベクトルnpに直交するベクトル
						Coord rt_sub = CalcOuterProduct(rt,np);						// 基本ベクトルrtと法線ベクトルnpに直交するベクトル
						Coord su_sub = CalcOuterProduct(su,nq);						// 基本ベクトルsuと法線ベクトルnqに直交するベクトル
						Coord sv_sub = CalcOuterProduct(sv,nq);						// 基本ベクトルsvと法線ベクトルnqに直交するベクトル
						double dw = CalcInnerProduct(rt_sub,deltap0)/CalcInnerProduct(rt_sub,rw);	// 新しい点r(w0+dw,t0+dt)を与えるためのdwを算出
						double dt = CalcInnerProduct(rw_sub,deltap0)/CalcInnerProduct(rw_sub,rt);	// 新しい点r(w0+dw,t0+dt)を与えるためのdtを算出
						double du = CalcInnerProduct(sv_sub,deltaq0)/CalcInnerProduct(sv_sub,su);	// 新しい点r(w0+dw,t0+dt)を与えるためのdwを算出
						double dv = CalcInnerProduct(su_sub,deltaq0)/CalcInnerProduct(su_sub,sv);	// 新しい点r(w0+dw,t0+dt)を与えるためのdtを算出
						w0 += dw;									// 新しい点のwパラメータを得る
						t0 += dt;									// 新しい点のtパラメータを得る
						u0 += du;									// 新しい点のuパラメータを得る
						v0 += dv;									// 新しい点のvパラメータを得る
						
						// 曲面の範囲外に出てしまったらループを抜ける
						if(!CheckRange(nurbR->U[0],nurbR->U[1],w0,1) || !CheckRange(nurbR->V[0],nurbR->V[1],t0,1)){
							break;
						}
						if(!CheckRange(nurbS->U[0],nurbS->U[1],u0,1) || !CheckRange(nurbS->V[0],nurbS->V[1],v0,1)){
							break;
						}
						
						Coord deltapq = SubCoord(p0,q0);						// 点p0と点q0の差ベクトルを算出
						double deltapq_dis = CalcEuclid(deltapq);					// |p0-q0|の距離を算出
											
						// 十分収束したら交点が存在するため干渉有
						if(deltapq_dis < CONVERG_GAP){
							//SetMessage("Interference with the NURBS surface was detected");
							return KOD_TRUE;
						}
					}
				}
			}
		}
	}
	
	//SetMessage("Interference with the NURBS surface was not detected");
	return KOD_FALSE;
}

// NURBS曲面S(u,v)とNURBS曲面R(w,t)の干渉を検出する(トリム有)
// 引数  *tNurbS:NURBS曲面S(u,v)(トリム有) *tNurbR:NURBS曲面R(w,t)(トリム有) divnum:パラメータ分割数
// 返値  干渉の有無(干渉有:KOD_TRUE 干渉無:KOD_FALSE)
int NURBS_Func::DetectInterfereTrmS(TRIMD_NURBSS *tNurbR,TRIMD_NURBSS *tNurbS,int divnum)
{
	int count=0;

	// 各曲面を指定の分割数でuv分割し、それらの点における補助平面を生成して交線上の任意の1点に収束させる
	for(int w=0;w<divnum;w++){
		for(int t=0;t<divnum;t++){
			for(int u=0;u<divnum;u++){
				for(int v=0;v<divnum;v++){
					// 各曲面に分割点を生成する
					double w0 = tNurbR->pts->U[0] + (tNurbR->pts->U[1] - tNurbR->pts->U[0])*(double)w/(double)divnum;
					double t0 = tNurbR->pts->V[0] + (tNurbR->pts->V[1] - tNurbR->pts->V[0])*(double)t/(double)divnum;
					double u0 = tNurbS->pts->U[0] + (tNurbS->pts->U[1] - tNurbS->pts->U[0])*(double)u/(double)divnum;
					double v0 = tNurbS->pts->V[0] + (tNurbS->pts->V[1] - tNurbS->pts->V[0])*(double)v/(double)divnum;
					for(int i=0;i<10;i++){
						// 各種パラメータを算出する
						Coord p0 = CalcNurbsSCoord(tNurbR->pts,w0,t0);					// R(w0,t0)となる点(初期点)の座標
						Coord q0 = CalcNurbsSCoord(tNurbS->pts,u0,v0);					// S(u0,v0)となる点(初期点)の座標
						Coord rw = CalcDiffuNurbsS(tNurbR->pts,w0,t0);					// 点R(w0,t0)のu偏微分(基本ベクトル)
						Coord rt = CalcDiffvNurbsS(tNurbR->pts,w0,t0);					// 点R(w0,t0)のv偏微分(基本ベクトル)
						double rwt = CalcEuclid(CalcOuterProduct(rw,rt));
						if(rwt==0.0) break;
						Coord np = DivCoord(CalcOuterProduct(rw,rt),rwt);				// 点R(w0,t0)の単位法線ベクトル
						Coord su = CalcDiffuNurbsS(tNurbS->pts,u0,v0);					// 点S(u0,v0)のu偏微分(基本ベクトル)
						Coord sv = CalcDiffvNurbsS(tNurbS->pts,u0,v0);					// 点S(u0,v0)のv偏微分(基本ベクトル)
						double suv = CalcEuclid(CalcOuterProduct(su,sv));
						if(suv==0.0) break;
						Coord nq = DivCoord(CalcOuterProduct(su,sv),suv);	// 点S(u0,v0)の単位法線ベクトル
						double npq = CalcEuclid(CalcOuterProduct(np,nq));
						if(npq==0.0) break;
						Coord nn = DivCoord(CalcOuterProduct(np,nq),CalcEuclid(CalcOuterProduct(np,nq)));	// 平面Fpと平面Fqに直交する平面Fnの単位法線ベクトル
						double dp = CalcInnerProduct(p0,np);						// 原点から平面Fpまでの距離
						double dq = CalcInnerProduct(q0,nq);						// 原点から平面Fqまでの距離
						double dn = CalcInnerProduct(p0,nn);						// 原点から平面Fnまでの距離
						Coord cross_nqn = CalcOuterProduct(nq,nn);					// 単位法線ベクトルnq,nnのベクトル積
						Coord cross_nnp = CalcOuterProduct(nn,np);					// 単位法線ベクトルnn,npのベクトル積
						Coord cross_npq = CalcOuterProduct(np,nq);					// 単位法線ベクトルnp,nqのベクトル積
						Coord nume_p_sub =  AddCoord(MulCoord(cross_nqn,dp),MulCoord(cross_nnp,dq));	// 3平面Fp,Fq,Fnの交点pの分子の最初の2項を計算
						Coord nume_p = AddCoord(nume_p_sub,MulCoord(cross_npq,dn));			// pの分子を算出
						double denom_p = CalcScalarTriProduct(np,nq,nn);				// pの分母を算出
						Coord p = DivCoord(nume_p,denom_p);						// pを算出
						Coord deltap0 = SubCoord(p,p0);							// 点pと点p0の差ベクトルを算出
						Coord deltaq0 = SubCoord(p,q0);							// 点pと点q0の差ベクトルを算出
						Coord rw_sub = CalcOuterProduct(rw,np);						// 基本ベクトルrwと法線ベクトルnpに直交するベクトル
						Coord rt_sub = CalcOuterProduct(rt,np);						// 基本ベクトルrtと法線ベクトルnpに直交するベクトル
						Coord su_sub = CalcOuterProduct(su,nq);						// 基本ベクトルsuと法線ベクトルnqに直交するベクトル
						Coord sv_sub = CalcOuterProduct(sv,nq);						// 基本ベクトルsvと法線ベクトルnqに直交するベクトル
						double dw = CalcInnerProduct(rt_sub,deltap0)/CalcInnerProduct(rt_sub,rw);	// 新しい点r(w0+dw,t0+dt)を与えるためのdwを算出
						double dt = CalcInnerProduct(rw_sub,deltap0)/CalcInnerProduct(rw_sub,rt);	// 新しい点r(w0+dw,t0+dt)を与えるためのdtを算出
						double du = CalcInnerProduct(sv_sub,deltaq0)/CalcInnerProduct(sv_sub,su);	// 新しい点r(w0+dw,t0+dt)を与えるためのdwを算出
						double dv = CalcInnerProduct(su_sub,deltaq0)/CalcInnerProduct(su_sub,sv);	// 新しい点r(w0+dw,t0+dt)を与えるためのdtを算出
						w0 += dw;									// 新しい点のwパラメータを得る
						t0 += dt;									// 新しい点のtパラメータを得る
						u0 += du;									// 新しい点のuパラメータを得る
						v0 += dv;									// 新しい点のvパラメータを得る
						
						// 曲面の範囲外に出てしまったらループを抜ける
						if(!CheckRange(tNurbR->pts->U[0],tNurbR->pts->U[1],w0,1) || !CheckRange(tNurbR->pts->V[0],tNurbR->pts->V[1],t0,1)){
							break;
						}
						if(!CheckRange(tNurbS->pts->U[0],tNurbS->pts->U[1],u0,1) || !CheckRange(tNurbS->pts->V[0],tNurbS->pts->V[1],v0,1)){
							break;
						}
						
						Coord deltapq = SubCoord(p0,q0);						// 点p0と点q0の差ベクトルを算出
						double deltapq_dis = CalcEuclid(deltapq);					// |p0-q0|の距離を算出
										
						// 十分収束したら交点が存在するため干渉有
						if(deltapq_dis < CONVERG_GAP){
							if(DetermPtOnTRMSurf(tNurbR,w0,t0) >= KOD_TRUE && DetermPtOnTRMSurf(tNurbS,u0,v0) >= KOD_TRUE){	// トリムされなければ
								//SetMessage("Interference with the trimmed NURBS surface was detected");
								return KOD_TRUE;
							}
						}
					}
				}
			}
		}
	}
	
	//SetMessage("Interference with the trimmed NURBS surface was not detected");
	return KOD_FALSE;
}

// 3次以下のNURBS曲面と平面との交点群を代数解法にて求める(vパラメータ分割)
// 引数  *nurb:NURBS曲面  pt:平面上の一点  nvec:平面の法線ベクトル  v_divnum:vパラメータ分割数
//       *ans:算出された交点のu,vパラメータ値をそれぞれans.x,ans.yに格納,  ans_size:ansの配列長
// 返値  交点の個数(交点の数がans_sizeを超えた：KOD_ERR)
int NURBS_Func::CalcIntersecPtsPlaneV3(NURBSS *nurb,Coord pt,Coord nvec,int v_divnum,Coord *ans,int ans_size)
{
	double v_const;			// 定数と置いたときのvパラメータ
	double *N;				// Bスプライン基底関数の計算値を格納
	double *A;
	Coord  *B;
	double **coef;
	double Q[4];
	Coord  P[4];
	double a[4];
	double t[3];
	int ansnum;
	int allansnum=0;

	N = (double *)malloc(sizeof(double)*nurb->K[1]);
	A = (double *)malloc(sizeof(double)*nurb->K[0]);
	B = (Coord *)malloc(sizeof(Coord)*nurb->K[0]);
	coef = NewMatrix(nurb->M[0],nurb->M[0]);

	// vパラメータを区間内で分割し、各vパラメータ上のNURBS曲線C(u)と平面(pt,nvec)との交点を求める
	for(int v=0;v<=v_divnum;v++){
		v_const = (nurb->V[1] - nurb->V[0])*(double)v/(double)v_divnum;		// 適当なv方向パラメータを設定
		for(int i=0;i<nurb->K[1];i++){
			N[i] = CalcBSbasis(v_const,nurb->T,nurb->N[0],i,nurb->M[1]);		// v_const時のBスプライン基底関数を求める
		}
		for(int i=0;i<nurb->K[0];i++){
			A[i] = 0;
			InitCoord(&B[i]);
			for(int j=0;j<nurb->K[1];j++){
				A[i] += N[j]*nurb->W[i][j];			// v_const上のNURBS曲線C(u)の分母の係数
				B[i] = AddCoord(B[i],MulCoord(nurb->cp[i][j],N[j]*nurb->W[i][j]));		// v_const上のNURBS曲線C(u)の分子の係数
			}
		}
		for(int i=0;i<nurb->K[0]-nurb->M[0]+1;i++){						// i番目の曲線に対して
			InitMatrix(coef,nurb->M[0],nurb->M[0]);
			InitVector(a,4);
			InitCoord(P,4);
			InitVector(Q,4);
			InitVector(t,3);
			if(nurb->M[0]-1 == 3){										// 3次
				GetBSplCoef3(nurb->M[0],nurb->K[0],i,nurb->S,coef);		// 3次のBスプライン基底関数の係数を求める
			}
			else if(nurb->M[0]-1 == 2){									// 2次
				GetBSplCoef2(nurb->M[0],nurb->K[0],i,nurb->S,coef);		// 2次のBスプライン基底関数の係数を求める
			}
			else if(nurb->M[0]-1 == 1){									// 1次
				GetBSplCoef1(nurb->M[0],nurb->K[0],i,nurb->S,coef);		// 1次のBスプライン基底関数の係数を求める
			}
			GetNurbsSCoef(nurb->M[0],coef,A,B,i,P,Q);					// 固定されたvパラメータ上のNURBS曲線C(u)の係数を求める
			GetIntersecEquation(nurb->M[0],P,Q,pt,nvec,a);				// 方程式を導出
			ansnum = CalcEquation(a,t,nurb->M[0]-1);					// 方程式を解く
			int hitnum = 0;						// 条件に適合する解の数をカウントする
			for(int j=0;j<ansnum;j++){			// 3つの解それぞれに対して
				if(t[j] >= nurb->S[i+nurb->M[0]-1] && t[j] <= nurb->S[i+nurb->M[0]]){	// 注目中のノットベクトルの範囲内なら
					ans[allansnum+hitnum] = SetCoord(t[j],v_const,0);		// 解として登録
					hitnum++;
				}
			}
			allansnum += hitnum;				// 条件適合解の数だけ総解数をカウントアップ
			if(allansnum >= ans_size){
				SetMessage("NURBS KOD_ERR:Intersection points exceeded the allocated array length");
				allansnum = KOD_ERR;
				goto EXIT;
			}
		}
	}

EXIT:
	free(N);
	free(A);
	free(B);
	FreeMatrix(coef,nurb->M[0]);

	return allansnum;
}

// 3次以下のNURBS曲面と平面との交点群を代数解法にて求める(uパラメータ分割)
// 引数  *nurb:NURBS曲面  pt:平面上の一点  nvec:平面の法線ベクトル  u_divnum:uパラメータ分割数
//       *ans:算出された交点のu,vパラメータ値ををそれぞれans.x,ans.yに格納,  ans_size:ansの配列長
// 返値  交点の個数(交点の数がans_sizeを超えた：KOD_ERR)
int NURBS_Func::CalcIntersecPtsPlaneU3(NURBSS *nurb,Coord pt,Coord nvec,int u_divnum,Coord *ans,int ans_size)
{
	double u_const;			// 定数と置いたときのvパラメータ
	double *N;				// Bスプライン基底関数の計算値を格納
	double *A;
	Coord  *B;
	double **coef;
	double Q[4];
	Coord  P[4];
	double a[4];
	double t[3];
	int ansnum;
	int allansnum=0;

	N = (double *)malloc(sizeof(double)*nurb->K[0]);
	A = (double *)malloc(sizeof(double)*nurb->K[1]);
	B = (Coord *)malloc(sizeof(Coord)*nurb->K[1]);
	coef = NewMatrix(nurb->M[1],nurb->M[1]);

	// uパラメータを区間内で分割し、各uパラメータ上のNURBS曲線C(v)と平面(pt,nvec)との交点を求める
	for(int u=0;u<=u_divnum;u++){
		u_const = (nurb->U[1] - nurb->U[0])*(double)u/(double)u_divnum;		// 適当なu方向パラメータを設定
		for(int i=0;i<nurb->K[0];i++){
			N[i] = CalcBSbasis(u_const,nurb->S,nurb->N[0],i,nurb->M[0]);		// u_const時のBスプライン基底関数を求める
		}
		for(int j=0;j<nurb->K[1];j++){
			A[j] = 0;
			InitCoord(&B[j]);
			for(int i=0;i<nurb->K[0];i++){
				A[j] += N[i]*nurb->W[i][j];			// u_const上のNURBS曲線C(v)の分母の係数
				B[j] = AddCoord(B[j],MulCoord(nurb->cp[i][j],N[i]*nurb->W[i][j]));		// u_const上のNURBS曲線C(v)の分子の係数
			}
		}
		for(int i=0;i<nurb->K[1]-nurb->M[1]+1;i++){						// i番目の曲線に対して
			if(nurb->M[1]-1 == 3){										// 3次
				GetBSplCoef3(nurb->M[1],nurb->K[1],i,nurb->T,coef);		// 3次のBスプライン基底関数の係数を求める
			}
			else if(nurb->M[1]-1 == 2){									// 2次
				GetBSplCoef2(nurb->M[1],nurb->K[1],i,nurb->T,coef);		// 2次のBスプライン基底関数の係数を求める
			}
			else if(nurb->M[1]-1 == 1){									// 1次
				GetBSplCoef1(nurb->M[1],nurb->K[1],i,nurb->T,coef);		// 1次のBスプライン基底関数の係数を求める
			}
			GetNurbsSCoef(nurb->M[1],coef,A,B,i,P,Q);					// 固定されたuパラメータ上のNURBS曲線C(v)の係数を求める
			GetIntersecEquation(nurb->M[1],P,Q,pt,nvec,a);				// 方程式を導出
			ansnum = CalcEquation(a,t,nurb->M[1]-1);					// 方程式を解く

			int hitnum = 0;						// 条件に適合する解の数をカウントする
			for(int j=0;j<ansnum;j++){			// 3つの解それぞれに対して
				if(t[j] >= nurb->T[i+nurb->M[1]-1] && t[j] <= nurb->T[i+nurb->M[1]]){	// 注目中のノットベクトルの範囲内なら
					ans[allansnum+hitnum] = SetCoord(u_const,t[j],0);		// 解として登録
					hitnum++;
				}
			}
			allansnum += hitnum;				// 条件適合解の数だけ総解数をカウントアップ
			if(allansnum >= ans_size){
				SetMessage("NURBS KOD_ERR:Intersection points exceeded the allocated array length");
				allansnum = KOD_ERR;
				goto EXIT;
			}
		}
	}

EXIT:
	free(N);
	free(A);
	free(B);
	FreeMatrix(coef,nurb->M[1]);

	return allansnum;
}

// V方向のアイソパラ曲線を指定した分割数で生成し，各曲線とNURBS曲面との交点を算出する
// 引数  *nurb:NURBS曲面  pt:平面上の一点  nvec:平面の法線ベクトル  v_divnum:vパラメータ分割数
//       *ans:算出された交点のu,vパラメータ値をそれぞれans.x,ans.yに格納,  ans_size:ansの配列長
// 返値  交点の個数(交点の数がans_sizeを超えた：KOD_ERR)
int NURBS_Func::CalcIntersecPtsPlaneV(NURBSS *nurbss,Coord pt,Coord nvec,int v_divnum,Coord *ans,int ans_size)
{
	double v_const;			// 定数と置いたときのvパラメータ
	int ansnum;				// 1つのアイソパラ曲線と曲面の交点群の数
	int allansnum=0;		// 全交点数をカウント
	int ansbufsize = (nurbss->M[0]-1)*((nurbss->K[0]>nurbss->K[1]?nurbss->K[0]:nurbss->K[1])-nurbss->M[0]+1);	// 1つのアイソパラ曲線と曲面の交点群を格納する配列の配列長
	double *ansbuf;			// 1つのアイソパラ曲線と曲面の交点群を格納する配列
	NURBSC nurbsc;			// 1つのアイソパラ曲線

	ansbuf = NewVector(ansbufsize);

	// vパラメータを区間内で分割し、各vパラメータ上のNURBS曲線C(u)と平面(pt,nvec)との交点を求める
	for(int v=0;v<=v_divnum;v++){
		v_const = (nurbss->V[1] - nurbss->V[0])*(double)v/(double)v_divnum;		// 適当なv方向パラメータを設定
		GenIsoparamCurveV(nurbss,v_const,&nurbsc);								// v_constでのアイソパラ曲線を取得
		ansnum = CalcIntersecCurve(&nurbsc,pt,nvec,v_divnum,ansbuf,ansbufsize);	// アイソパラ曲線と曲面の交点群を算出
		if(ansnum == KOD_ERR)	goto EXIT;
		for(int i=0;i<ansnum;i++){
			ans[allansnum+i] = SetCoord(ansbuf[i],v_const,0);					// 解を登録
		}
		allansnum += ansnum;
	}

EXIT:
	FreeVector(ansbuf);
	return allansnum;
}

// U方向のアイソパラ曲線を指定した分割数で生成し，各曲線とNURBS曲面との交点を算出する
// 引数  *nurb:NURBS曲面  pt:平面上の一点  nvec:平面の法線ベクトル  u_divnum:uパラメータ分割数
//       *ans:算出された交点のu,vパラメータ値をそれぞれans.x,ans.yに格納,  ans_size:ansの配列長
// 返値  交点の個数(交点の数がans_sizeを超えた：KOD_ERR)
int NURBS_Func::CalcIntersecPtsPlaneU(NURBSS *nurbss,Coord pt,Coord nvec,int u_divnum,Coord *ans,int ans_size)
{
	double u_const;			// 定数と置いたときのvパラメータ
	int ansnum;				// 1つのアイソパラ曲線と曲面の交点群の数
	int allansnum=0;		// 全交点数をカウント
	int ansbufsize = (nurbss->M[0]-1)*((nurbss->K[0]>nurbss->K[1]?nurbss->K[0]:nurbss->K[1])-nurbss->M[0]+1);	// 1つのアイソパラ曲線と曲面の交点群を格納する配列の配列長
	double *ansbuf;			// 1つのアイソパラ曲線と曲面の交点群を格納する配列
	NURBSC nurbsc;			// 1つのアイソパラ曲線


	ansbuf = NewVector(ansbufsize);

	// uパラメータを区間内で分割し、各uパラメータ上のアイソパラ曲線C(v)と平面(pt,nvec)との交点を求める
	for(int u=0;u<=u_divnum;u++){
		u_const = (nurbss->U[1] - nurbss->U[0])*(double)u/(double)u_divnum;		// 適当なu方向パラメータを設定
		GenIsoparamCurveU(nurbss,u_const,&nurbsc);								// u_constでのアイソパラ曲線を取得
		ansnum = CalcIntersecCurve(&nurbsc,pt,nvec,u_divnum,ansbuf,ansbufsize);	// アイソパラ曲線と曲面の交点群を算出
		if(ansnum == KOD_ERR)	goto EXIT;
		for(int i=0;i<ansnum;i++){
			ans[allansnum+i] = SetCoord(u_const,ansbuf[i],0);					// 解を登録
		}
		allansnum += ansnum;
	}

EXIT:
	FreeVector(ansbuf);
	return allansnum;
}

// NURBS曲面と平面との交点群を幾何学的に求める(補助平面を用いた解法)
// 引数  *nurb:NURBS曲面  pt:平面上の一点  nf:平面の法線ベクトル　u_divnum:uパラメータ分割数　v_divnum:vパラメータ分割数
//       *ans:算出された交点のu,vパラメータ値をそれぞれans.x,ans.yに格納,  ans_size:ansの配列長
// 返値  交点の個数(交点の数がans_sizeを超えた：ERR)
int NURBS_Func::CalcIntersecPtsPlaneGeom(NURBSS *nurb,Coord pt,Coord nf,int u_divnum,int v_divnum,Coord *ans,int ans_size)
{
	Coord init_pt;
	int ansnum=0;
	double pcolor[3] = {0,1,1};		// 表示の色

	for(int u=0;u<u_divnum;u++){
		for(int v=0;v<v_divnum;v++){
			double u0 = nurb->U[0] + (nurb->U[1] - nurb->U[0])*(double)u/(double)u_divnum;
			double v0 = nurb->V[0] + (nurb->V[1] - nurb->V[0])*(double)v/(double)v_divnum;
			for(int i=0;i<LOOPCOUNTMAX;i++){
				Coord p0 = CalcNurbsSCoord(nurb,u0,v0);						// S(u0,v0)となる点(初期点)の座標
//				DrawPoint(p0,1,5,pcolor);				
				Coord su = CalcDiffuNurbsS(nurb,u0,v0);						// 点S(u0,v0)のu偏微分(基本ベクトル)
				Coord sv = CalcDiffvNurbsS(nurb,u0,v0);						// 点S(u0,v0)のv偏微分(基本ベクトル)
				if(ZoroCoord(su) == KOD_FALSE || ZoroCoord(sv) == KOD_FALSE) break;
				Coord nt = DivCoord(CalcOuterProduct(su,sv),CalcEuclid(CalcOuterProduct(su,sv)));	// 点S(u0,v0)の単位法線ベクトル
				Coord nn = DivCoord(CalcOuterProduct(nf,nt),CalcEuclid(CalcOuterProduct(nf,nt)));	// 平面Fと平面Ftに直交する平面Fnの単位法線ベクトル
				if(ZoroCoord(nt) == KOD_FALSE || ZoroCoord(nn) == KOD_FALSE) break;
				double df = CalcInnerProduct(pt,nf);						// 原点から平面Fまでの距離
				double dt = CalcInnerProduct(p0,nt);						// 原点から平面Ftまでの距離
				double dn = CalcInnerProduct(p0,nn);						// 原点から平面Fnまでの距離
				Coord cross_ntn = CalcOuterProduct(nt,nn);					// 単位法線ベクトルnt,nnのベクトル積
				Coord cross_nnf = CalcOuterProduct(nn,nf);					// 単位法線ベクトルnn,nfのベクトル積
				Coord cross_nft = CalcOuterProduct(nf,nt);					// 単位法線ベクトルnf,ntのベクトル積
				Coord nume_p1_sub =  AddCoord(MulCoord(cross_ntn,df),MulCoord(cross_nnf,dt));	// 3平面F,Ft,Fnの交点p1の分子の最初の2項を計算
				Coord nume_p1 = AddCoord(nume_p1_sub,MulCoord(cross_nft,dn));			// p1の分子を算出
				double denom_p1 = CalcScalarTriProduct(nf,nt,nn);				// p1の分母を算出
				Coord p1 = DivCoord(nume_p1,denom_p1);						// p1を算出
				Coord deltap = SubCoord(p1,p0);							// 点p1と点p0の距離を算出
				double deltap_dis = CalcEuclid(deltap);						// Δpの距離を算出
				double tri_su = CalcScalarTriProduct(su,sv,nf);
				double tri_sv = CalcScalarTriProduct(su,sv,nf);
				if(CheckZero(tri_sv,HIGH_ACCURACY) || CheckZero(tri_su,HIGH_ACCURACY)) break;
				double du = CalcScalarTriProduct(deltap,sv,nf)/CalcScalarTriProduct(su,sv,nf);	// 新しい点s(u0+du,v0+dv)を与えるためのduを算出
				double dv = -CalcScalarTriProduct(deltap,su,nf)/CalcScalarTriProduct(su,sv,nf);	// 新しい点s(u0+du,v0+dv)を与えるためのdvを算出
//				fprintf(stderr,"delta = %lf\n",deltap_dis);
//				fprintf(stderr,"du = %lf dv = %lf\n",du,dv);
				u0 += du;									// 新しい点のuパラメータを得る
				v0 += dv;									// 新しい点のvパラメータを得る
				if(u0 < nurb->U[0] || u0 > nurb->U[1] || v0 < nurb->V[0] || v0 > nurb->V[1]){	// 追跡点がパラメータ領域外に出た
				//	fprintf(stderr,"NURBS ERROR:曲面Rのパラメータが領域外に出ました\n");
					break;
				}
				if(deltap_dis < CONVERG_GAP){							// Δpが収束したら
					// fprintf(stderr,"   %d:%lf,%lf\n",ansnum,u0,v0);	// debug
					ans[ansnum] = SetCoord(u0,v0,0);					// 解として登録
					ansnum++;								// 解をカウント
					//if(ansnum == ans_size)					// 解の数が制限を越えた
						//return ansnum;
					break;
				}
			}
		}
	}
	
	return ansnum;
}


// NURBS曲面と平面との交点群を交点追跡法にて求める
// 引数　NURBSS:NURBS曲面  pt:平面上の1点  nvec:平面の法線ベクトル  ds:交線(交点群)の粗さ(密0.1〜2疎)  initdivnum:初期点探索の荒さ(密10〜1疎)
//       ans:解  ans_size:解の数   method:交点算出時の数値解析法の選択(Runge-Kutta or Bulirsch-Stoer)
// 返値　KOD_FALSE:NURBS曲面と平面が交差していない　KOD_ERR:特異点または発散により処理を中断
int NURBS_Func::CalcIntersecPtsPlaneSearch(NURBSS *nurb,Coord pt,Coord nvec,double ds,int initdivnum,Coord *ans,int ans_size,int method)
{
	int loop_count=0;		// 収束計算のループ数
	int pcount=0;
	int anscount=0;
	Coord oldp;
	Coord newp;
	Coord init_pt[INTERSECPTNUMMAX];		// 初期点(u,vパラメータ値)
	Coord init_pt_Coord[INTERSECPTNUMMAX];	// 初期点(x,y,z座標値)
	bool  init_pt_flag[INTERSECPTNUMMAX];	// 各初期点を通り終えたかを判別するフラグ
	bool  init_allpt_flag=KOD_FALSE;			// 初期点を全て通り終えたかを判別するフラグ
	int   init_pt_num=0;						// 初期点の数
	int   init_pt_flag_count=0;
	double u,v;								// 交線追跡中のu,vパラメータ中間値
	double dist;							// ループ脱出用に追跡点間距離を閾値とする
	int loopbreak_flag = KOD_FALSE;				// 初期点一致フラグ
	int  search_flag = KOD_TRUE;				// 交線追跡方向フラグ(KOD_TRUE:順方向,KOD_FALSE:逆方向)
	int  inverse_flag = KOD_FALSE;				// 交線追跡方向逆転フラグ
	double color[3] = {0,1,1};

	// 初期点通過判別フラグを初期化
	for(int i=0;i<INTERSECPTNUMMAX;i++){
		init_pt_flag[i] = KOD_FALSE;
	}
	init_pt_flag[0] = KOD_TRUE;

	// まず交線追跡法の初期点として交点をいくつか求める
	init_pt_num = CalcIntersecPtsPlaneGeom(nurb,pt,nvec,initdivnum,initdivnum,init_pt,INTERSECPTNUMMAX);

	if(init_pt_num<=0){
		init_pt_num = CalcIntersecPtsPlaneU(nurb,pt,nvec,initdivnum,init_pt,INTERSECPTNUMMAX);		// 交点が見つからなかった場合はサーチ法を変えて再トライ
	}
	if(init_pt_num<=0)
		init_pt_num = CalcIntersecPtsPlaneV(nurb,pt,nvec,initdivnum,init_pt,INTERSECPTNUMMAX);		// それでも交点が見つからなかった場合は方向を変えてサーチ

	init_pt_num = CheckTheSamePoints(init_pt,init_pt_num);		// 同一点は除去する
	if(!init_pt_num){		// 見つからない場合は、交差していないとみなす
		SetMessage("NURBS KOD_ERROR:Init intersection point is noexistence");
		return KOD_FALSE;					
	}
	else if(init_pt_num == KOD_ERR) return KOD_ERR;			// init_pt_numがinit_ptの配列長を超えた

	for(int i=0;i<init_pt_num;i++){
		init_pt_Coord[i] = CalcNurbsSCoord(nurb,init_pt[i].x,init_pt[i].y);		// 交点のuvパラメータをxyz座標値に変換したものを保持しておく
		//fprintf(stderr,"%d:%lf,%lf,%lf\n",i,init_pt_Coord[i].x,init_pt_Coord[i].y,init_pt_Coord[i].z);	// debug
		//DrawPoint(init_pt_Coord[i],1,5,color);	// debug
	}

	// 初期点を全て通過するまで交線追跡法を繰り返す
	while(init_allpt_flag == KOD_FALSE){
		//fprintf(stderr,"Init%d:%lf,%lf,%lf\n",pcount+1,init_pt_Coord[pcount].x,init_pt_Coord[pcount].y,init_pt_Coord[pcount].z);		// debug
		// 交線追跡のための始点(u,v)をセット
		u = oldp.x = init_pt[pcount].x;
		v = oldp.y = init_pt[pcount].y;
		ans[anscount] = SetCoord(init_pt[pcount]);
		anscount++;
		init_pt_flag[pcount] = KOD_TRUE;
		init_pt_flag_count++;
		if(init_pt_flag_count == init_pt_num && init_pt_num > 1)	break;

		if(inverse_flag == KOD_TRUE){	// 逆方向への交線追跡も終了していたら
			inverse_flag = KOD_FALSE;	// 交線追跡方向を順方向に戻す
		}
		// 交線追跡開始
		loop_count = 0;
		while(loop_count < ans_size){
			// 順方向に交線追跡
			//fprintf(stderr,"%d:%lf,%lf--->",loop_count,u,v);
			if(inverse_flag == KOD_FALSE){
				if(method == RUNGE_KUTTA)	search_flag = SearchIntersectPt_RKM(nurb,pt,nvec,ds,&u,&v,FORWARD);	// 順方向の交点算出
				else	search_flag = SearchIntersectPt_BS(nurb,pt,nvec,ds,&u,&v,FORWARD);
				if(search_flag != KOD_TRUE){						// 順方向追跡に失敗した場合は
					inverse_flag = KOD_TRUE;						// 逆方向追跡フラグをON
					u = init_pt[pcount].x;							// 交点追跡の初期点をセットしなおす
					v = init_pt[pcount].y;
				}
			}
			// 逆方向追跡フラグがONなら
			if(inverse_flag == KOD_TRUE){
				if(method == RUNGE_KUTTA)	search_flag = SearchIntersectPt_RKM(nurb,pt,nvec,ds,&u,&v,INVERSE);	// 逆方向の交点算出
				else	search_flag = SearchIntersectPt_BS(nurb,pt,nvec,ds,&u,&v,INVERSE);
 			}

			if(search_flag == KOD_ERR){					// 特異点検出などにより処理を継続できない場合
				//return KOD_ERR;
				//return anscount;
				SetMessage("NURBS_FUNC CAUTION: Singler point was ditected.");
				break;
			}
			else if(search_flag == KOD_FALSE){
				break;
			}
			//fprintf(stderr,"%lf,%lf\n",u,v);
			// とりあえず何らかの解が得られた(この時点ではu,v範囲外であっても通す)
			newp.x = u;								
			newp.y = v;

			// 初期点が交線追跡法によって全て通過したか調べる
			for(int i=0;i<init_pt_num;i++){
				// 新たに算出された交点と1つ前の交点を対角とする立方体の中に初期点が含まれていたら
				if(CalcDistance(init_pt_Coord[i],CalcNurbsSCoord(nurb,u,v)) < 1.2*ds){
					if(loop_count && i==pcount && inverse_flag == KOD_FALSE){	// 閉ループに対して一周して戻ってきた場合はループを抜ける
						loopbreak_flag = KOD_TRUE;	
						//fprintf(stderr,"%d loop break OK\n",i);		// debug
						break;					
					}
					if(init_pt_flag[i] == KOD_FALSE && search_flag == KOD_TRUE){		// まだ通過していない初期点で交点もu,v範囲内だったら
						init_pt_flag[i] = KOD_TRUE;					// 通過したこととして登録
						init_pt_flag_count++;						// 通過済み初期点数をカウントアップ
						//fprintf(stderr,"%d OK\n",i);				// debug
					}
				}
			}

			// 閉ループに対して一周してきた場合はループを抜ける
			if(loopbreak_flag == KOD_TRUE){
				loopbreak_flag = KOD_FALSE;
				break;
			}

			// debug
			//Coord buf = CalcNurbsSCoord(nurb,newp.x,newp.y);
			//fprintf(stderr,"   %d:%lf,%lf\n",anscount+1,buf.x,buf.y);

			ans[anscount] = SetCoord(newp);	// 得られたu,vを交線(交点群)として登録
			anscount++;				// 交点群の数をインクリメント

			// 交点の数が指定サイズを超えた場合はそこまでで強制リターン
			if(anscount >= ans_size){
				SetMessage("NURBS KOD_ERROR:Intersection points exceeded the allocated array length");
				SetMessage("There is a possibility that you set large ds."); 
				return anscount;
			}

			oldp = SetCoord(newp);	// このループで算出された交点は次のループでは1個前の交点となる

			loop_count++;		// ループ回数をインクリメント
		}// 交線追跡ここまで

		// 残った点があれば、別の交線があるので、その点を始点として再度交線追跡を行う
		init_allpt_flag = KOD_TRUE;
		for(int i=0;i<init_pt_num;i++){
			//fprintf(stderr,"%d,",i);			// debug
			if(init_pt_flag[i] == KOD_FALSE){
				init_allpt_flag = KOD_FALSE;
				pcount = i;
				break;
			}
		}
		//fprintf(stderr,"loop count:%d\n",loop_count);	// debug
	}

	return anscount;
}

// Bulirsch-Stoer法により交点を収束させる(NURBS曲面と平面)
// 返値　　分割数列のインデックス
int NURBS_Func::SearchIntersectPt_BS(NURBSS *S,Coord pt,Coord nvec,double H,double *u0,double *v0,int direction)
{
	// 引数指定ミス
	if(direction != FORWARD && direction != INVERSE){
		SetMessage("NURBS ERROR: selected wrong direction");
		return KOD_ERR;
	}

	int    n[11] = {2,4,6,8,12,16,24,32,48,64,96};		// B-S法の分割数群を指定
	Coord  z[97];							// 修正中点法の中間値を格納(z.x = u, z.y = v)
	Coord  f;								// f.x = fu(u,v), f.y = fv(u,v)
	Coord  D[11][11],C[11][11],P[11];		// B-S法の中間パラメータ
	double h[11];							// B-S法の刻み幅
	Coord  R;								// h=0の外挿値
	int    conv_flag = KOD_FALSE;			// 収束フラグ

	// 各分割数における刻み幅を求めておく
	for(int i=0;i<11;i++)
		h[i] = H/n[i];
	
	// 刻み幅を小さい方から順に変更しながら、B-S法による外挿値を計算していく
	for(int i=0;i<11;i++){
		// まず、u(s+H)の値を修正中点法により計算する
		z[0] = SetCoord(*u0,*v0,1);										// z0とz1の算出は別処理
		if(GetSIPParam1(S,*u0,*v0,pt,nvec,direction,&f) == KOD_FALSE)	// z0での微分方程式の右辺を計算
			return KOD_FALSE;
		z[1] = AddCoord2D(z[0],MulCoord2D(f,h[i]));							// z0とz1の算出は別処理
		for(int j=1;j<n[i];j++){
			if(GetSIPParam1(S,z[j].x,z[j].y,pt,nvec,direction,&f) == KOD_FALSE)	// zjでの微分方程式の右辺を計算
				return KOD_FALSE;
			z[j+1] = AddCoord2D(z[j-1],MulCoord2D(f,2*h[i]));				// z2〜znまでを算出
		}
		if(GetSIPParam1(S,z[n[i]].x,z[n[i]].y,pt,nvec,direction,&f) == KOD_FALSE)	// znでの微分方程式の右辺を計算
			return KOD_FALSE;
		C[0][i] = DivCoord(AddCoord2D(AddCoord2D(z[n[i]],z[n[i]-1]),MulCoord2D(f,h[i])),2);		// u(s+H)
		D[0][i] = SetCoord(C[0][i]);
		if(i==0) continue;

		// B-S法の差分表を順次求めていく
		R = SetCoord(D[0][i]);
		for(int k=i-1;k>=0;k--){
			int m = i-k;
			double x1 = h[k]*h[k];
			double x2 = h[k+m]*h[k+m];
			double x = x1/x2;
			Coord denom = SubCoord2D(MulCoord2D(D[k][m-1],x),C[k+1][m-1]);
			C[k][m] = DivCoord2D(MulCoord2D(MulCoord2D(D[k][m-1],SubCoord2D(C[k+1][m-1],D[k][m-1])),x),denom);	// 有理式による外挿
			D[k][m] = DivCoord2D(MulCoord2D(C[k][m-1],SubCoord2D(C[k+1][m-1],D[k][m-1])),denom);				// 有理式による外挿
			//C[k][m] = MulCoord2D(SubCoord2D(C[k+1][m-1],D[k][m-1]),x1/(x1-x2));								// 多項式による外挿
			//D[k][m] = MulCoord2D(SubCoord2D(C[k+1][m-1],D[k][m-1]),x2/(x1-x2));								// 多項式による外挿
			R = AddCoord2D(R,D[k][m]);		// 外挿値
		}
		 //fprintf(stderr,"%d,%lf (%lf,%lf)\n",i,h[i],D[0][i].x,D[0][i].y);			// debug

		// D[0][i-1]が所定の閾値よりも小さくなったか、ステップサイズが最小になった場合、そのときの外挿値を解として演算処理を終了する
		if(i > 0 && CalcEuclid2D((D[0][i].x-D[0][i-1].x),(D[0][i].y-D[0][i-1].y)) < APPROX_ZERO_L || i==10){
			*u0 = R.x;
			*v0 = R.y;
			conv_flag = KOD_TRUE;
			break;
		}
	}

	return conv_flag;
}

// 曲面と平面の交点を表す微分方程式の右辺の値を得る
int NURBS_Func::GetSIPParam1(NURBSS *S,double u,double v,Coord pt,Coord nvec,int direction,Coord *f)
{
	NURBS_Func NFunc;

	Coord Su = CalcDiffuNurbsS(S,u,v);
	Coord Sv = CalcDiffvNurbsS(S,u,v);
	double fu = CalcInnerProduct(nvec,Su);	// nf・Su
	double fv = CalcInnerProduct(nvec,Sv);	// nf・Sv
	double E = CalcInnerProduct(Su,Su);		// 1次規格量
	double F = CalcInnerProduct(Su,Sv);		// 1次規格量
	double G = CalcInnerProduct(Sv,Sv);		// 1次規格量
		double f__ = E*fv*fv - 2*F*fu*fv + G*fu*fu;
		if(f__==0.0){
			SetMessage("NURBS_Func CAUTION:The process is stoped by detecting singular point.");
			return KOD_FALSE;				
		}
		double f_ = 1/sqrt(f__);
		*f = SetCoord(f_*fv*(double)direction, -f_*fu*(double)direction, 0);

	return KOD_TRUE;
}

// 4次のルンゲクッタ法により交点を導出(NURBS曲面と平面)
// du(s)/ds = g(u,v),   dv(s)/ds = h(u,v)
// u(s+delta) = u(s) + (p1+2p2+2p3+p4)/6
// v(s+delta) = v(s) + (q1+2q2+2q3+q4)/6
// p1 = delta*g(u,v),   q1 = delta*h(u,v)
// p2 = delta*g(u+p1/2,v+q1/2),   q2 = delta*h(u+p1/2,v+q1/2)
// p3 = delta*g(u+p2/2,v+q2/2),   q3 = delta*h(u+p2/2,v+q2/2)
// p4 = delta*g(u+p3,v+q3),   q4 = delta*h(u+p3,v+q3)
int NURBS_Func::SearchIntersectPt_RKM(NURBSS *S,Coord pt,Coord n,double delta,double *u,double *v,int direction)
{
	double u0 = *u;
	double v0 = *v;
	double p[4],q[4];

	for(int i=0;i<4;i++){
		if(i==1 || i==2){
			*u = u0 + p[i-1]/2;
			*v = v0 + q[i-1]/2;
		}
		else if(i==3){
			*u = u0 + p[i-1];
			*v = v0 + q[i-1];
		}
		Coord Su = CalcDiffuNurbsS(S,*u,*v);
		Coord Sv = CalcDiffvNurbsS(S,*u,*v);
		double fu = CalcInnerProduct(n,Su);
		double fv = CalcInnerProduct(n,Sv);
		double fuu = fu*fu;
		double fuv = fu*fv;
		double fvv = fv*fv;
		if(CheckZero(fu,HIGH_ACCURACY) == KOD_TRUE && CheckZero(fv,HIGH_ACCURACY) == KOD_TRUE){			// 特異点
			//SetMessage("NURBS KOD_ERROR:The process is stoped by detected singular point.");
			return KOD_ERR;				
		}
		double E = CalcInnerProduct(Su,Su);		// 1次規格量
		double F = CalcInnerProduct(Su,Sv);		// 1次規格量
		double G = CalcInnerProduct(Sv,Sv);		// 1次規格量
		double f_ = 1/sqrt(E*fvv - 2*F*fuv + G*fuu);
		p[i] = -delta*fv*f_*(double)direction;
		q[i] = delta*fu*f_*(double)direction;
	}
	*u = u0+(p[0]+2*p[1]+2*p[2]+p[3])/6;
	*v = v0+(q[0]+2*q[1]+2*q[2]+q[3])/6;

	if(*u < S->U[0] || *u > S->U[1] || *v < S->V[0] || *v > S->V[1])	// パラメータ範囲外
		return KOD_FALSE;

	return KOD_TRUE;
}

// ニュートン法により交点を真値に収束させる(NURBS曲面と平面)
int NURBS_Func::SearchIntersectPt(NURBSS *nurb,Coord pt,Coord nvec,double ds,double *u,double *v,int direction)
{
	double d = CalcInnerProduct(pt,nvec);	// 原点から平面までの距離

	// まず初期値としてのdu,dvを求める
	Coord pu = CalcDiffuNurbsS(nurb,*u,*v);
	Coord pv = CalcDiffvNurbsS(nurb,*u,*v);
	double phi = CalcInnerProduct(nvec,CalcNurbsSCoord(nurb,*u,*v));
	double phi_u = CalcInnerProduct(nvec,pu);
	double phi_v = CalcInnerProduct(nvec,pv);
	double E = CalcInnerProduct(pu,pu);
	double F = CalcInnerProduct(pu,pv);
	double G = CalcInnerProduct(pv,pv);
	double f = sqrt(E*phi_v*phi_v - 2*F*phi_u*phi_v + G*phi_u*phi_u); 
	//fprintf(stderr,"%lf , %lf\n",phi_u,phi_v);
	if(CheckZero(phi_u,MID_ACCURACY) == KOD_TRUE && CheckZero(phi_v,MID_ACCURACY) == KOD_TRUE){			// 特異点
		//SetMessage("NURBS KOD_ERROR:The process is stoped by detected singular point.");
		return KOD_ERR;				
	}

	// 交線追跡順方向の場合
	if(direction == FORWARD){
		f = 1/f;
	}
	// 交線追跡逆方向の場合
	else if(direction == INVERSE){
		f = -1/f;
	}

	double du = -f*phi_v*ds;		// 初期値
	double dv = f*phi_u*ds;			// 初期値

	// ニュートン法を用いてu,vを真値に収束させる
	int k=0;
	if(fabs(dv) > fabs(du)){				// dv>duの場合はdvを定数として固定する
		while(!CheckZero(du,MID_ACCURACY)){		// duが収束するまで繰り返し計算
			phi = CalcInnerProduct(nvec,CalcNurbsSCoord(nurb,*u,*v));
			phi_u = CalcInnerProduct(nvec,CalcDiffuNurbsS(nurb,*u,*v));
			phi_v = CalcInnerProduct(nvec,CalcDiffvNurbsS(nurb,*u,*v));
			du = (d-phi-phi_v*dv)/phi_u;
			*u += du;
			if(!CheckRange(nurb->U[0],nurb->U[1],*u,0) || k > LOOPCOUNTMAX){
				//SetMessage("NURBS KOD_ERROR:fail to calculate convergence");
				return KOD_FALSE;
			}
			k++;
		}
		*v += dv;
		if(!CheckRange(nurb->V[0],nurb->V[1],*v,0)){
			return KOD_FALSE;
		}
	}
	else{									// dv<duの場合はduを定数として固定する
		while(!CheckZero(dv,MID_ACCURACY)){		// dvが収束するまで繰り返し計算
			phi = CalcInnerProduct(nvec,CalcNurbsSCoord(nurb,*u,*v));
			phi_u = CalcInnerProduct(nvec,CalcDiffuNurbsS(nurb,*u,*v));
			phi_v = CalcInnerProduct(nvec,CalcDiffvNurbsS(nurb,*u,*v));
			dv = (d-phi-phi_u*du)/phi_v;
			*v += dv;
			if(!CheckRange(nurb->V[0],nurb->V[1],*v,0) || k>LOOPCOUNTMAX){
				//SetMessage("NURBS KOD_ERROR:fail to calculate convergence");
				return KOD_FALSE;
			}
			k++;
		}
		*u += du;
		if(!CheckRange(nurb->U[0],nurb->U[1],*u,0))
			return KOD_FALSE;
	}
	return KOD_TRUE;
}


// NURBS曲面とNURBS曲線との交点群を交線追跡法で求める
// 引数  *NurbsS,*NurbsC:NURBS曲面とNURBS曲線へのポインタ   Divnum:初期点サーチ時の曲線分割数   *ans:解  ans_size:ans配列の配列長
// 返値  交点の数
int NURBS_Func::CalcIntersecPtsNurbsSNurbsC(NURBSS *NurbsS,NURBSC *NurbsC,int Divnum,Coord *ans,int ans_size)
{
	Coord d = SetCoord(100,100,100);		// NURBS曲線S(u,v)の微小変化量(du,dv)、直線N(t)の微小変化量dtを格納
	Coord F,Fu,Fv,Ft;						// F(u,v,t) = S(u,v) - N(t)    Fu = dF/du     Fv = dF/dv     Ft = dF/dt
	double u = NurbsS->U[0];				// NURBS曲面S(u,v)のuパラメータの現在値
	double v = NurbsS->V[0];				// NURBS曲面S(u,v)のvパラメータの現在値
	double t = NurbsC->V[0];				// NURBS曲線C(t)のtパラメータ
	Matrix A = NewMatrix(3,3);				// Fu,Fv,Ftを構成する3x3行列
	Matrix A_ = NewMatrix(3,3);				// Aの逆行列を格納
	bool flag = false;						// 収束フラグ
	double dt = (NurbsC->V[1] - NurbsC->V[0])/(double)Divnum;	// 収束演算用のtパラメータのインターバル値
	int loopcount = 0;						// 収束計算回数
	int anscount = 0;						// 算出された交点の数

	// t loop
	for(int i=0;i<Divnum;i++){
		t = NurbsC->V[0] + (double)i*dt;	// ステップパラメータtの初期値をセット
		u = NurbsS->U[0];					// ステップパラメータuの初期値をセット
		v = NurbsS->V[0];					// ステップパラメータvの初期値をセット
		flag = false;						// 収束フラグをOFF
		loopcount = 0;						// ループカウント初期化
		// 直線の微小変化量dt(=d.z)がAPPROX_ZEROを下回るまでニュートン法による収束計算を行う
		while(loopcount < LOOPCOUNTMAX){
			F = SubCoord(CalcNurbsSCoord(NurbsS,u,v),CalcNurbsCCoord(NurbsC,t));	// F(u,v,t) = S(u,v) - C(t)
			Fu = CalcDiffuNurbsS(NurbsS,u,v);			// Fu = dF/du = dS/du
			Fv = CalcDiffvNurbsS(NurbsS,u,v);			// Fv = dF/dv = dS/dv
			Ft = CalcDiffNurbsC(NurbsC,t);				// Ft = dF/dt = dC/dt
			A[0][0] = Fu.x;				// Fu,Fv,Ftを3x3行列Aに代入
			A[0][1] = Fv.x;				//     |Fu.x Fv.x Ft.x|       |du|       |F.x|
			A[0][2] = Ft.x;				// A = |Fu.y Fv.y Ft.y| , d = |dv| , F = |F.y|
			A[1][0] = Fu.y;				//     |Fu.z Fv.z Ft.z|       |dt|       |F.z|
			A[1][1] = Fv.y;
			A[1][2] = Ft.y;				// A・d = F   --->   d = A_・F
			A[2][0] = Fu.z;
			A[2][1] = Fv.z;
			A[2][2] = Ft.z;	
			if(MatInv3(A,A_) == KOD_FALSE)	break;		// 逆行列を求める
			d = MulCoord(MulMxCoord(A_,F),-1);			// dを算出

			if(fabs(d.x) <= APPROX_ZERO && fabs(d.y) <= APPROX_ZERO && fabs(d.z) <= APPROX_ZERO){	// 真値に収束したらloopを抜ける
				flag = true;		// 収束フラグtrue
				break;
			}

			// 真値に達していなかったらu,v,tを更新
			u += d.x;
			v += d.y;
			t += d.z;

			if(u < NurbsS->U[0] || u > NurbsS->U[1] || v < NurbsS->V[0] || v > NurbsS->V[1] || t < NurbsC->V[0] || t > NurbsC->V[1]){	// u,vのどちらかが発散したらloopを抜ける
				flag = false;		// 収束フラグfalse
				break;
			}

			loopcount++;
		}// end of while

		// 収束していたら解として登録
		if(flag == true){
			ans[anscount] = SetCoord(u,v,t);
			anscount++;
			if(anscount == ans_size){
				SetMessage("NURBS_Func ERROR: Ans_size overflow");
				return KOD_ERR;
			}
		}
	}// end of i loop

	FreeMatrix(A,3);
	FreeMatrix(A_,3);

	anscount = CheckTheSamePoints(ans,anscount);		// 同一点は除去する

	return anscount;
}

// NURBS曲面S(u,v)とNURBS曲面R(w,t)の交線(交点群)を交点追跡法にて求める
// 引数　nurbsS:NURBS曲面S(u,v) nurbsR:NURBS曲面R(w,t) div:初期点サーチ時の曲面分割数  ds:交線(交点群)の粗さ(密0.1〜2疎)  ans:解  ans_size:ans配列の配列長
// 返値　FALSE:NURBS曲面同士が交差していない　ERR:特異点または発散により処理を中断
int NURBS_Func::CalcIntersecPtsNurbsSSearch(NURBSS *nurbR,NURBSS *nurbS,int div,double ds,Coord *ansR,Coord *ansS,int ans_size)
{
	int ans_count=0;		// 追跡点の総数
	int loop_count=0;		// 収束計算のループ数
	int pnow=0;
	Coord init_pt_R[INTERSECPTNUMMAX];		// 初期点(u,vパラメータ値)
	Coord init_pt_S[INTERSECPTNUMMAX];		// 初期点(u,vパラメータ値)
	Coord init_pt_Coord_R[INTERSECPTNUMMAX];	// 初期点(x,y,z座標値)
	Coord init_pt_Coord_S[INTERSECPTNUMMAX];
	int  init_pt_flag[INTERSECPTNUMMAX];		// 各初期点を通り終えたかを判別するフラグ
	int  init_allpt_flag=KOD_FALSE;			// 初期点を全て通り終えたかを判別するフラグ
	int   init_pt_num = 0;				// 初期点の数
	int  conform_flag = KOD_FALSE;			// 初期点一致フラグ
	int  search_flag = KOD_TRUE;			// 交線追跡方向フラグ(KOD_TRUE:順方向,KOD_FALSE:逆方向)
	int  inverse_flag = KOD_FALSE;			// 交線追跡方向逆転フラグ
	double u,v,w,t;					// 交線追跡中のu,vパラメータ中間値
//	FILE *fp=fopen("debug.csv","w");
//	double color[3] = {0,1,1};
	
	// 初期点通過判別フラグを初期化
//	init_pt_flag[0] = KOD_TRUE;
	for(int i=0;i<INTERSECPTNUMMAX;i++){
		init_pt_flag[i] = KOD_FALSE;
	}
	init_pt_flag[0] = KOD_TRUE;

	// 交線追跡するための初期点となる点をいくつか探す
	// ※注意:　複数の交線ループがある場合、全ての交線ループ上の初期点を見つけなければならない
	//　　　　　そのため、あまり分割数が少ないと一部の交線ループ上に交線(交点群)が生成されなくなる場合がある
	init_pt_num = CalcIntersecPtsNurbsSGeom(nurbR,nurbS,div,div,init_pt_R,init_pt_S,INTERSECPTNUMMAX);
	//if(!init_pt_num){
	//	init_pt_num = CalcIntersecPtsNurbsSGeom(nurbR,nurbS,5,5,init_pt_R,init_pt_S,INTERSECPTNUMMAX);
	//}
	//if(!init_pt_num){
	//	init_pt_num = CalcIntersecPtsNurbsSGeom(nurbR,nurbS,7,7,init_pt_R,init_pt_S,INTERSECPTNUMMAX);
	//}
	//if(!init_pt_num){
	//	init_pt_num = CalcIntersecPtsNurbsSGeom(nurbR,nurbS,10,10,init_pt_R,init_pt_S,INTERSECPTNUMMAX);
	//}
	if(!init_pt_num){		// それでも見つからない場合は、交差していないとみなす
		return KOD_FALSE;					
	}
	
	for(int i=0;i<init_pt_num;i++){
		init_pt_Coord_R[i] = CalcNurbsSCoord(nurbR,init_pt_R[i].x,init_pt_R[i].y);		// 交点のuvパラメータをxyz座標値に変換したものを保持しておく
		init_pt_Coord_S[i] = CalcNurbsSCoord(nurbS,init_pt_S[i].x,init_pt_S[i].y);		// 交点のuvパラメータをxyz座標値に変換したものを保持しておく
	//	DrawPoint(init_pt_Coord_R[i],1,5,color);
	//	DrawPoint(init_pt_Coord_S[i],1,5,color);
	}
	ansR[ans_count] = SetCoord(init_pt_R[0]);
	ansS[ans_count] = SetCoord(init_pt_S[0]);
	
	// 初期点を全て通過するまで交線追跡法を繰り返す
	while(init_allpt_flag == KOD_FALSE){
		// 交線追跡のための始点R(w,t),S(u,v)をセット
		w = ansR[ans_count].x = init_pt_R[pnow].x;
		t = ansR[ans_count].y = init_pt_R[pnow].y;
		u = ansS[ans_count].x = init_pt_S[pnow].x;
		v = ansS[ans_count].y = init_pt_S[pnow].y;
 		if(inverse_flag == KOD_FALSE){		// 追跡方向が順方向から逆方向に変わるとき以外
			ans_count++;			// 解をカウント
			init_pt_flag[pnow] = KOD_TRUE;	// 初期点通過フラグを立てる
		}
		else if(inverse_flag == KOD_TRUE)		// 追跡方向が順方向から逆方向に変わるとき
			inverse_flag = KOD_FALSE;		// 追跡方向(順から逆)フラグを元に戻す
		
		// 交線追跡開始
		while(1){
			// 追跡方向が順方向の場合
			if(search_flag == KOD_TRUE){
				search_flag = SearchIntersectPt(nurbR,nurbS,ds,&w,&t,&u,&v,FORWARD);	// 順方向に交線追跡
				if(search_flag != KOD_TRUE)						// uvパラメータ外に出たら
 					inverse_flag = KOD_TRUE;						// 追跡方向(順から逆)フラグを立てる
			}
			// 追跡方向が逆方向の場合
			else if(search_flag == KOD_FALSE){
				int flag = SearchIntersectPt(nurbR,nurbS,ds,&w,&t,&u,&v,INVERSE);
				if(flag == KOD_FALSE)	// uvパラメータ外に出たら
					search_flag = KOD_TRUE;						// 追跡方向フラグを順方向に
 			}
			// 特異点検出などにより処理を継続できない場合
			else if(search_flag == KOD_ERR){
				return KOD_ERR;
			}

			Coord pr = CalcNurbsSCoord(nurbR,w,t);			// 得られたu,vをxyz座標値に変換
			Coord ps = CalcNurbsSCoord(nurbS,u,v);			// 得られたu,vをxyz座標値に変換
			double distr = CalcDistance(init_pt_Coord_R[pnow],pr);	// 得られたxyz座標値と初期点との距離を算出
			double dists = CalcDistance(init_pt_Coord_S[pnow],ps);	// 得られたxyz座標値と初期点との距離を算出
			
			// 交点の個数がリミットを越えたら
			if(ans_count >= ans_size-1){
				SetMessage("NURBS KOD_ERROR:Intersection points exceeded the allocated array length");
				return ans_count;
			}

			// 最初に求めた初期点が交線追跡法によって全て通過したか調べる
			for(int i=0;i<init_pt_num;i++){
				if(CalcDistance(init_pt_Coord_R[i],pr) < ds){
					if(init_pt_flag[i] == KOD_TRUE && i < pnow){
						conform_flag = KOD_TRUE;
						break;
					}
					init_pt_flag[i] = KOD_TRUE;
				}
			}
			
			// u,vが取り得るパラメータ範囲（0〜1）を超えた場合または、１周して戻ってきた場合はループを抜ける
			if(!CheckRange(nurbR->U[0],nurbR->U[1],w,0) || !CheckRange(nurbR->V[0],nurbR->V[1],t,0) || (distr < ds/2 && loop_count > 0)){
				break;
			}
			
			if(!CheckRange(nurbS->U[0],nurbS->U[1],u,0) || !CheckRange(nurbS->V[0],nurbS->V[1],v,0) || (dists < ds/2 && loop_count > 0)){
				break;
			}
			
			// 得られたu,vを交線(交点群)として登録
			ansR[ans_count] = SetCoord(w,t,0);
			ansS[ans_count] = SetCoord(u,v,0);
			ans_count++;

			if(conform_flag == KOD_TRUE){
				conform_flag = KOD_FALSE;
				break;
			}

			loop_count++;		// ループ回数をインクリメント

 		}// 交線追跡ここまで

		// 残った点があれば、別の交線があるので、その点を始点として再度交線追跡を行う
		if(search_flag == KOD_TRUE){
			init_allpt_flag = KOD_TRUE;
			for(int i=0;i<init_pt_num;i++){
				if(init_pt_flag[i] == KOD_FALSE){
					init_allpt_flag = KOD_FALSE;
					pnow = i;
					break;
				}
			}
		}
	}
	
	//fclose(fp);
	return ans_count;
}

// NURBS曲面S(u,v)とNURBS曲面R(w,t)の交線(交点群)を幾何学的に求める(補助平面を用いた解法)
// 引数  *nurbS:NURBS曲面S(u,v) *nurbR:NURBS曲面R(w,t) u_divnum:uパラメータ分割数　v_divnum:vパラメータ分割数
//       *ans:算出された交点のu,vパラメータ値をそれぞれans.x,ans.yに格納,  ans_size:ansの配列長
// 返値  交点の個数(交点の数がans_sizeを超えた：ERR)
int NURBS_Func::CalcIntersecPtsNurbsSGeom(NURBSS *nurbR,NURBSS *nurbS,int u_divnum,int v_divnum,Coord *ansR,Coord *ansS,int ans_size)
{
	int ansnum=0;
	
	// 各曲面を指定の分割数でuv分割し、それらの点における補助平面を生成して交線上の任意の1点に収束させる
	for(int w=0;w<u_divnum;w++){
		for(int t=0;t<v_divnum;t++){
			for(int u=0;u<u_divnum;u++){
				for(int v=0;v<v_divnum;v++){
					// 各曲面に分割点を生成する
					double w0 = nurbR->U[0] + (nurbR->U[1] - nurbR->U[0])*(double)w/(double)u_divnum;
					double t0 = nurbR->V[0] + (nurbR->V[1] - nurbR->V[0])*(double)t/(double)v_divnum;
					double u0 = nurbS->U[0] + (nurbS->U[1] - nurbS->U[0])*(double)u/(double)u_divnum;
					double v0 = nurbS->V[0] + (nurbS->V[1] - nurbS->V[0])*(double)v/(double)v_divnum;
					for(int i=0;i<10;i++){
						// 各種パラメータを算出する
						Coord p0 = CalcNurbsSCoord(nurbR,w0,t0);					// R(w0,t0)となる点(初期点)の座標
						Coord q0 = CalcNurbsSCoord(nurbS,u0,v0);					// S(u0,v0)となる点(初期点)の座標
						Coord rw = CalcDiffuNurbsS(nurbR,w0,t0);					// 点R(w0,t0)のu偏微分(基本ベクトル)
						Coord rt = CalcDiffvNurbsS(nurbR,w0,t0);					// 点R(w0,t0)のv偏微分(基本ベクトル)
						double rwt = CalcEuclid(CalcOuterProduct(rw,rt));
						if(rwt==0.0) break;
						Coord np = DivCoord(CalcOuterProduct(rw,rt),rwt);			// 点R(w0,t0)の単位法線ベクトル
						Coord su = CalcDiffuNurbsS(nurbS,u0,v0);					// 点S(u0,v0)のu偏微分(基本ベクトル)
						Coord sv = CalcDiffvNurbsS(nurbS,u0,v0);					// 点S(u0,v0)のv偏微分(基本ベクトル)
						double suv = CalcEuclid(CalcOuterProduct(su,sv));
						if(suv==0.0) break;
						Coord nq = DivCoord(CalcOuterProduct(su,sv),suv);			// 点S(u0,v0)の単位法線ベクトル
						double npq = CalcEuclid(CalcOuterProduct(np,nq));
						if(npq==0.0) break;
						Coord nn = DivCoord(CalcOuterProduct(np,nq),npq);			// 平面Fpと平面Fqに直交する平面Fnの単位法線ベクトル
						double dp = CalcInnerProduct(p0,np);						// 原点から平面Fpまでの距離
						double dq = CalcInnerProduct(q0,nq);						// 原点から平面Fqまでの距離
						double dn = CalcInnerProduct(p0,nn);						// 原点から平面Fnまでの距離
						Coord cross_nqn = CalcOuterProduct(nq,nn);					// 単位法線ベクトルnq,nnのベクトル積
						Coord cross_nnp = CalcOuterProduct(nn,np);					// 単位法線ベクトルnn,npのベクトル積
						Coord cross_npq = CalcOuterProduct(np,nq);					// 単位法線ベクトルnp,nqのベクトル積
						Coord nume_p_sub =  AddCoord(MulCoord(cross_nqn,dp),MulCoord(cross_nnp,dq));	// 3平面Fp,Fq,Fnの交点pの分子の最初の2項を計算
						Coord nume_p = AddCoord(nume_p_sub,MulCoord(cross_npq,dn));			// pの分子を算出
						double denom_p = CalcScalarTriProduct(np,nq,nn);			// pの分母を算出
						Coord p = DivCoord(nume_p,denom_p);							// pを算出
						Coord deltap0 = SubCoord(p,p0);								// 点pと点p0の差ベクトルを算出
						Coord deltaq0 = SubCoord(p,q0);								// 点pと点q0の差ベクトルを算出
						Coord rw_sub = CalcOuterProduct(rw,np);						// 基本ベクトルrwと法線ベクトルnpに直交するベクトル
						Coord rt_sub = CalcOuterProduct(rt,np);						// 基本ベクトルrtと法線ベクトルnpに直交するベクトル
						Coord su_sub = CalcOuterProduct(su,nq);						// 基本ベクトルsuと法線ベクトルnqに直交するベクトル
						Coord sv_sub = CalcOuterProduct(sv,nq);						// 基本ベクトルsvと法線ベクトルnqに直交するベクトル
						double dw = CalcInnerProduct(rt_sub,deltap0)/CalcInnerProduct(rt_sub,rw);	// 新しい点r(w0+dw,t0+dt)を与えるためのdwを算出
						double dt = CalcInnerProduct(rw_sub,deltap0)/CalcInnerProduct(rw_sub,rt);	// 新しい点r(w0+dw,t0+dt)を与えるためのdtを算出
						double du = CalcInnerProduct(sv_sub,deltaq0)/CalcInnerProduct(sv_sub,su);	// 新しい点r(w0+dw,t0+dt)を与えるためのdwを算出
						double dv = CalcInnerProduct(su_sub,deltaq0)/CalcInnerProduct(su_sub,sv);	// 新しい点r(w0+dw,t0+dt)を与えるためのdtを算出
						w0 += dw;									// 新しい点のwパラメータを得る
						t0 += dt;									// 新しい点のtパラメータを得る
						u0 += du;									// 新しい点のuパラメータを得る
						v0 += dv;									// 新しい点のvパラメータを得る
						
						// 曲面の範囲外に出てしまったらループを抜ける
						if(!CheckRange(nurbR->U[0],nurbR->U[1],w0,1) || !CheckRange(nurbR->V[0],nurbR->V[1],t0,1)){
							break;
						}
						if(!CheckRange(nurbS->U[0],nurbS->U[1],u0,1) || !CheckRange(nurbS->V[0],nurbS->V[1],v0,1)){
							break;
						}
						
						Coord deltapq = SubCoord(p0,q0);						// 点p0と点q0の差ベクトルを算出
						double deltapq_dis = CalcEuclid(deltapq);					// |p0-q0|の距離を算出

						// 十分収束したら解を登録する
						if(deltapq_dis < CONVERG_GAP){								
							if(!ansnum || (!CheckZero(ansR[ansnum-1].x-w0,MID_ACCURACY) && !CheckZero(ansR[ansnum-1].y-t0,MID_ACCURACY))){// 直前に算出した解と同一の解でなければ
								ansR[ansnum] = SetCoord(w0,t0,0);						// 解として登録
								ansS[ansnum] = SetCoord(u0,v0,0);
								ansnum++;								// 解をカウント
								if(ansnum == ans_size)					// 解の数が制限を越えた
									return ansnum;
							}
							break;
						}
					}
				}
			}
		}
	}
	return ansnum;
}


// ニュートン法により交点を真値に収束させる(NURBS曲面同士)
int NURBS_Func::SearchIntersectPt(NURBSS *nurbR,NURBSS *nurbS,double ds,double *w,double *t,double *u,double *v,int direction)
{
	double **J;
	double D[3];
	double ans[3];
	int flag = KOD_TRUE;

	// メモリ確保
	if((J = NewMatrix(3,3)) == NULL){
		SetMessage("NURBS ERROR: fail to malloc\n");
		return KOD_ERR;
	}

	// まず初期値としてのdw,dt,du,dvを求める
	Coord r = CalcNurbsSCoord(nurbR,*w,*t);					// 点R(w,t)のNURBS曲面の座標値を求める
	Coord s = CalcNurbsSCoord(nurbS,*u,*v);					// 点S(u,v)のNURBS曲面の座標値を求める
	Coord rw = CalcDiffuNurbsS(nurbR,*w,*t);					// 点R(w,t)のu偏微分(基本ベクトル)
	Coord rt = CalcDiffvNurbsS(nurbR,*w,*t);					// 点R(w,t)のv偏微分(基本ベクトル)
	Coord su = CalcDiffuNurbsS(nurbS,*u,*v);					// 点S(u,v)のu偏微分(基本ベクトル)
	Coord sv = CalcDiffvNurbsS(nurbS,*u,*v);					// 点S(u,v)のv偏微分(基本ベクトル)
	Coord n1 = DivCoord(CalcOuterProduct(rw,rt),CalcEuclid(CalcOuterProduct(rw,rt)));	// 点R(w0,t0)の単位法線ベクトル
	Coord n2 = DivCoord(CalcOuterProduct(su,sv),CalcEuclid(CalcOuterProduct(su,sv)));	// 点S(u0,v0)の単位法線ベクトル
	double f1 = CalcInnerProduct(n2,rt);
	double g1 = CalcInnerProduct(n2,rw);
	double f2 = CalcInnerProduct(n1,sv);
	double g2 = CalcInnerProduct(n1,su);
	double E1 = CalcInnerProduct(rw,rw);
	double F1 = CalcInnerProduct(rw,rt);
	double G1 = CalcInnerProduct(rt,rt);
	double E2 = CalcInnerProduct(su,su);
	double F2 = CalcInnerProduct(su,sv);
	double G2 = CalcInnerProduct(sv,sv);
	double phi1 = sqrt(E1*f1*f1 - 2*F1*f1*g1 + G1*g1*g1);
	double phi2 = sqrt(E2*f2*f2 - 2*F2*f2*g2 + G2*g2*g2);
	if(!phi1 && !phi2){			// 特異点
		SetMessage("NURBS KOD_ERROR:The process is stoped by detected singular point.");
		FreeMatrix(J,3);
		return KOD_ERR;				
	}
	
	// 交線追跡順方向の場合
	if(direction == FORWARD){
		phi1 = 1/phi1;
		phi2 = -1/phi2;
	}
	// 交線追跡逆方向の場合
	else if(direction == INVERSE){
		phi1 = -1/phi1;
		phi2 = 1/phi2;
	}
	
	// 差分パラメータの初期値を設定
	double dw = -f1*phi1*ds;		
	double dt = g1*phi1*ds;		
	double du = -f2*phi2*ds;
	double dv = g2*phi2*ds;
	double sort[4] = {fabs(dw),fabs(dt),fabs(du),fabs(dv)};	// ソート用変数を用意
	BubbleSort(sort,4);					// 昇順にソート
	double max_delta = sort[3];				// 各パラメータの中で最大値を得る

	// ニュートン法を用いてw,t,u,vを真値に収束させる
	int k=0;	// 収束計算回数を初期化
	// dw,dt,du,dvの絶対値中でdwが最大の時、dwを定数として固定する
	if(max_delta == fabs(dw)){
		while(fabs(dt) > CONVERG_GAP || fabs(du) > CONVERG_GAP || fabs(dv) > CONVERG_GAP){	
			r = CalcNurbsSCoord(nurbR,*w,*t);						// 点R(w,t)のNURBS曲面の座標値を求める
			s = CalcNurbsSCoord(nurbS,*u,*v);						// 点S(u,v)のNURBS曲面の座標値を求める
			rw = CalcDiffuNurbsS(nurbR,*w,*t);						// 点R(w,t)のu偏微分(基本ベクトル)
			rt = CalcDiffvNurbsS(nurbR,*w,*t);						// 点R(w,t)のv偏微分(基本ベクトル)
			su = CalcDiffuNurbsS(nurbS,*u,*v);						// 点S(u,v)のu偏微分(基本ベクトル)
			sv = CalcDiffvNurbsS(nurbS,*u,*v);						// 点S(u,v)のv偏微分(基本ベクトル)
			
			// 3連立方程式を解くために各パラメータを配列に格納する
			double sol[3] = {s.x-r.x-rw.x*dw, s.y-r.y-rw.y*dw, s.z-r.z-rw.z*dw};
			double coef[3][3] = {{rt.x,-su.x,-sv.x},{rt.y,-su.y,-sv.y},{rt.z,-su.z,-sv.z}};
			
			for(int i=0;i<3;i++){
				D[i] = sol[i];
				ans[i] = 0;
				for(int j=0;j<3;j++){
					J[i][j] = coef[i][j];
				}
			}

			// 連立方程式を解き、パラメータを更新
			Gauss(3,J,D,ans);
			dt = ans[0];
			du = ans[1];
			dv = ans[2];
			*t += dt;
			*u += du;
			*v += dv;
			
			// uvパラメータ範囲外に出たら
			if(!CheckRange(nurbR->V[0],nurbR->V[1],*t,0) || !CheckRange(nurbS->U[0],nurbS->U[1],*u,0) || !CheckRange(nurbS->V[0],nurbS->V[1],*v,0) || k>LOOPCOUNTMAX){
				flag = KOD_FALSE;
				goto EXIT;
			}
			k++;
		}
		*w += dw;	// 収束したら固定していたパラメータを更新
		if(!CheckRange(nurbR->U[0],nurbR->U[1],*w,0)){
			flag = KOD_FALSE;
			goto EXIT;
		}
	}
	
	// dw,dt,du,dvの絶対値中でdtが最大の時、dtを定数として固定する
	else if(max_delta == fabs(dt)){	
		while(fabs(dw) > CONVERG_GAP || fabs(du) > CONVERG_GAP || fabs(dv) > CONVERG_GAP){	
			r = CalcNurbsSCoord(nurbR,*w,*t);					// 点R(w,t)のNURBS曲面の座標値を求める
			s = CalcNurbsSCoord(nurbS,*u,*v);					// 点S(u,v)のNURBS曲面の座標値を求める
			rw = CalcDiffuNurbsS(nurbR,*w,*t);					// 点R(w,t)のu偏微分(基本ベクトル)
			rt = CalcDiffvNurbsS(nurbR,*w,*t);					// 点R(w,t)のv偏微分(基本ベクトル)
			su = CalcDiffuNurbsS(nurbS,*u,*v);					// 点S(u,v)のu偏微分(基本ベクトル)
			sv = CalcDiffvNurbsS(nurbS,*u,*v);					// 点S(u,v)のv偏微分(基本ベクトル)
			
			// 3連立方程式を解くために各パラメータを配列に格納する
			double sol[3] = {s.x-r.x-rt.x*dt, s.y-r.y-rt.y*dt, s.z-r.z-rt.z*dt};
			double coef[3][3] = {{rw.x,-su.x,-sv.x},{rw.y,-su.y,-sv.y},{rw.z,-su.z,-sv.z}};
			for(int i=0;i<3;i++){
				D[i] = sol[i];
				ans[i] = 0;
				for(int j=0;j<3;j++){
					J[i][j] = coef[i][j];
				}
			}
			
			// 連立方程式を解き、パラメータを更新
			Gauss(3,J,D,ans);
			dw = ans[0];
			du = ans[1];
			dv = ans[2];
			*w += dw;
			*u += du;
			*v += dv;
				
			// uvパラメータ範囲外に出たら
			if(!CheckRange(nurbR->U[0],nurbR->U[1],*w,0) || !CheckRange(nurbS->U[0],nurbS->U[1],*u,0) || !CheckRange(nurbS->V[0],nurbS->V[1],*v,0) || k>LOOPCOUNTMAX){
				flag = KOD_FALSE;
				goto EXIT;
			}
			k++;
		}
		*t += dt;	// 収束したら固定していたパラメータを更新
		if(!CheckRange(nurbR->V[0],nurbR->V[1],*t,0)){
			flag = KOD_FALSE;
			goto EXIT;
		}
	}
			
	// dw,dt,du,dvの絶対値中でduが最大の時、duを定数として固定する
	else if(max_delta == fabs(du)){	
		while(fabs(dw) > CONVERG_GAP || fabs(dt) > CONVERG_GAP || fabs(dv) > CONVERG_GAP){	
			r = CalcNurbsSCoord(nurbR,*w,*t);					// 点R(w,t)のNURBS曲面の座標値を求める
			s = CalcNurbsSCoord(nurbS,*u,*v);					// 点S(u,v)のNURBS曲面の座標値を求める
			rw = CalcDiffuNurbsS(nurbR,*w,*t);					// 点R(w,t)のu偏微分(基本ベクトル)
			rt = CalcDiffvNurbsS(nurbR,*w,*t);					// 点R(w,t)のv偏微分(基本ベクトル)
			su = CalcDiffuNurbsS(nurbS,*u,*v);					// 点S(u,v)のu偏微分(基本ベクトル)
			sv = CalcDiffvNurbsS(nurbS,*u,*v);					// 点S(u,v)のv偏微分(基本ベクトル)
			
			// 3連立方程式を解くために各パラメータを配列に格納する
			double sol[3] = {s.x-r.x+su.x*du, s.y-r.y+su.y*du, s.z-r.z+su.z*du};
			double coef[3][3] = {{rw.x,rt.x,-sv.x},{rw.y,rt.y,-sv.y},{rw.z,rt.z,-sv.z}};
			for(int i=0;i<3;i++){
				D[i] = sol[i];
				ans[i] = 0;
				for(int j=0;j<3;j++){
					J[i][j] = coef[i][j];
				}
			}
			
			// 連立方程式を解き、パラメータを更新
			Gauss(3,J,D,ans);
			dw = ans[0];
			dt = ans[1];
			dv = ans[2];
			*w += dw;
			*t += dt;
			*v += dv;
			
			// uvパラメータ範囲外に出たら
			if(!CheckRange(nurbR->U[0],nurbR->U[1],*w,0) || !CheckRange(nurbR->V[0],nurbR->V[1],*t,0) || !CheckRange(nurbS->V[0],nurbS->V[1],*v,0) || k>LOOPCOUNTMAX){
				flag = KOD_FALSE;
				goto EXIT;
			}
			k++;
		}
		*u += du;	// 収束したら固定していたパラメータを更新
		if(!CheckRange(nurbS->U[0],nurbS->U[1],*u,0)){
			flag = KOD_FALSE;
			goto EXIT;
		}
	}
	
	// dw,dt,du,dvの絶対値中でdvが最大の時、dvを定数として固定する
	else if(max_delta == fabs(dv)){	
		while(fabs(dt) > CONVERG_GAP || fabs(dw) > CONVERG_GAP || fabs(du) > CONVERG_GAP){	
			r = CalcNurbsSCoord(nurbR,*w,*t);					// 点R(w,t)のNURBS曲面の座標値を求める
			s = CalcNurbsSCoord(nurbS,*u,*v);					// 点S(u,v)のNURBS曲面の座標値を求める
			rw = CalcDiffuNurbsS(nurbR,*w,*t);					// 点R(w,t)のu偏微分(基本ベクトル)
			rt = CalcDiffvNurbsS(nurbR,*w,*t);					// 点R(w,t)のv偏微分(基本ベクトル)
			su = CalcDiffuNurbsS(nurbS,*u,*v);					// 点S(u,v)のu偏微分(基本ベクトル)
			sv = CalcDiffvNurbsS(nurbS,*u,*v);					// 点S(u,v)のv偏微分(基本ベクトル)
			
			// 3連立方程式を解くために各パラメータを配列に格納する
			double sol[3] = {s.x-r.x+sv.x*dv, s.y-r.y+sv.y*dv, s.z-r.z+sv.z*dv};
			double coef[3][3] = {{rw.x,rt.x,-su.x},{rw.y,rt.y,-su.y},{rw.z,rt.z,-su.z}};
			for(int i=0;i<3;i++){
				D[i] = sol[i];
				ans[i] = 0;
				for(int j=0;j<3;j++){
					J[i][j] = coef[i][j];
				}
			}
			
			// 連立方程式を解き、パラメータを更新
			Gauss(3,J,D,ans);
			dw = ans[0];
			dt = ans[1];
			du = ans[2];
			*w += dw;
			*t += dt;
			*u += du;
			
			// uvパラメータ範囲外に出たら
			if(!CheckRange(nurbR->U[0],nurbR->U[1],*w,0) || !CheckRange(nurbR->V[0],nurbR->V[1],*t,0) || !CheckRange(nurbS->U[0],nurbS->U[1],*u,0) || k>LOOPCOUNTMAX){
				flag = KOD_FALSE;
				goto EXIT;
			}
			k++;
		}
		*v += dv;	// 収束したら固定していたパラメータを更新
		if(!CheckRange(nurbS->V[0],nurbS->V[1],*v,0)){
			flag = KOD_FALSE;
			goto EXIT;
		}
	}

EXIT:
	// メモリ解放
	FreeMatrix(J,3);
	
	return flag;
}


// NURBS曲線C(u) or C(v)の係数を求める
void NURBS_Func::GetNurbsSCoef(int M,double **coef,double *a,Coord *b,int i,Coord *P,double *Q)
{
	for(int k=0;k<M;k++){
		Q[k] = 0;
		InitCoord(&P[k]);
		for(int j=0;j<M;j++){
			Q[k] += coef[j][k]*a[i+j];
			P[k] = AddCoord(P[k],MulCoord(b[i+j],coef[j][k]));
		}
	}
}


// NURBS曲線同士の交点を求める(ニュートン法)
// 引数   *NurbA, *NurbB: NURBS曲線     Divnum:初期値設定用の分割数     *ans:交点格納用配列   ans_size:ansの配列長
int NURBS_Func::CalcIntersecPtsNurbsCNurbsCParam(NURBSC *NurbA,NURBSC *NurbB,int Divnum,Coord *ans,int ans_size)
{
	double t = NurbA->V[0];		// 現在のNurbAのパラメータ値
	double u = 0;				// 現在のNurbBのパラメータ値
	double dt = 0;				// ニュートン法によるtパラメータの更新量
	double du = 0;				// ニュートン法によるuパラメータの更新量
	Coord F;					// ニュートン法の対象とする方程式(F(t,u) = NurbA(t) - NurbB(u))
	Coord Ft;					// Fのtによる微分値
	Coord Fu;					// Fのuによる微分値
	double d = (NurbA->V[1] - NurbA->V[0])/(double)Divnum;	// 初期点の増分値
	int loopcount = 0;			// ループ回数
	bool flag = false;			// 収束フラグ
	int anscount = 0;			// 交点の数をカウント
	Matrix A = NewMatrix(2,2);	// Ft,Fuを成分ごとに格納した行列
	Matrix A_ = NewMatrix(2,2);	// Aの逆行列を格納
	

	for(int i=0;i<Divnum;i++){
		flag = false;
		loopcount = 0;
		t = NurbA->V[0] + (double)i*d;		// 初期値更新
		while(loopcount < LOOPCOUNTMAX){
			F = SubCoord(CalcNurbsCCoord(NurbA,t),CalcNurbsCCoord(NurbB,u));
			Ft = CalcDiffNurbsC(NurbA,t);
			Fu = CalcDiffNurbsC(NurbB,u);
			A[0][0] = Ft.x;
			A[0][1] = Fu.x;
			A[1][0] = Ft.y;
			A[1][1] = Fu.y;
			MatInv2(A,A_);
			dt = -(A_[0][0]*F.x + A_[0][1]*F.y);
			du = -(A_[1][0]*F.x + A_[1][1]*F.y);

			if(CheckZero(dt,HIGH_ACCURACY) == KOD_TRUE && CheckZero(du,HIGH_ACCURACY) == KOD_TRUE){		// 更新値が閾値以下になったら、whileを抜け、解として登録
				flag = true;
				break;
			}
			t += dt;		// パラメータ値更新
			u += du;
			if(t < NurbA->V[0] || t > NurbA->V[1] || u < NurbB->V[0] || u > NurbB->V[1]){		// パラメータ範囲を超えたら、whileを抜け、次の初期値へ移行
				flag = false;
				break;
			}
			loopcount++;
		}// end of wihle
		if(flag == true){
			ans[anscount].x = t;		// 解として登録
			ans[anscount].y = u;
			anscount++;
			if(anscount == ans_size){	// 解の個数がans_sizeを超えたら、ERRをリターン
				SetMessage("NURBS_Func ERROR: Ans_size overflow");
				return KOD_ERR;
			}
		}
	}// end of i loop

	anscount = CheckTheSamePoints2D(ans,anscount);		// 同一点は除去する

	FreeMatrix(A,2);
	FreeMatrix(A_,2);

	return anscount;


}


// NURBS曲線と平面との交点を求める(ニュートン法)
// 引数  *nurb:NURBS曲線  pt:平面上の一点  nvec:平面の法線ベクトル   Divnum:NURBS曲線のパラメータ分割数  *ans:算出された交点のtパラメータ値を格納,  ans_size:ansの配列長
// 返値  交点の個数     KOD_ERR:交点の数がans_sizeを超えた
// F(t) = nvec・(C(t)-pt) = 0をニュートン法を用いて求める
// 交点は最大で(M-1)*(K-M+1)点得られる.  (例：4階でコントロールポイントの数8個の場合、(4-1)*(8-4+1)=15点)
// よって引数*ansは(M-1)*(K-M+1)個の配列を用意することが望ましい.
int NURBS_Func::CalcIntersecCurve(NURBSC *nurb,Coord pt,Coord nvec,int Divnum,double *ans,int ans_size)
{
	double t = nurb->V[0];		// 現在のNURBS曲線のパラメータ値
	double d = 0;				// ニュートン法によるパラメータの更新量
	double F;					// ニュートン法の対象とする方程式
	double Ft;					// Fのtによる微分値
	double dt = (nurb->V[1] - nurb->V[0])/(double)Divnum;	// 初期点の増分値
	int loopcount = 0;			// ループ回数
	bool flag = false;			// 収束フラグ
	int anscount = 0;			// 交点の数をカウント
	

	for(int i=0;i<Divnum;i++){
		flag = false;
		loopcount = 0;
		t = nurb->V[0] + (double)i*dt;		// 初期値更新
		while(loopcount < LOOPCOUNTMAX){
			F = CalcInnerProduct(nvec,SubCoord(CalcNurbsCCoord(nurb,t),pt));
			Ft = CalcInnerProduct(nvec,CalcDiffNurbsC(nurb,t));
			d = -F/Ft;		// 更新値
			if(CheckZero(d,MID_ACCURACY) == KOD_TRUE){		// 更新値が閾値以下になったら、whileを抜け、解として登録
				flag = true;
				break;
			}
			t += d;		// パラメータ値更新
			if(t < nurb->V[0] || t > nurb->V[1]){		// パラメータ範囲を超えたら、whileを抜け、次の初期値へ移行
				flag = false;
				break;
			}
			loopcount++;
		}// end of wihle
		if(flag == true){
			ans[anscount] = t;		// 解として登録
			anscount++;
			if(anscount == ans_size){	// 解の個数がans_sizeを超えたら、ERRをリターン
				SetMessage("NURBS_Func ERROR: Ans_size overflow");
				return KOD_ERR;
			}
		}
	}// end of i loop

	anscount = CheckTheSamePoints(ans,anscount);		// 同一点は除去する

	return anscount;
}


// NURBS曲線と平面との交点を求める(3次まで対応)
// 引数  *nurb:NURBS曲線  pt:平面上の一点  nvec:平面の法線ベクトル  *ans:算出された交点のtパラメータ値を格納,  ans_size:ansの配列長
// 返値  交点の個数
// 交点は最大で(M-1)*(K-M+1)点得られる.  (例：4階でコントロールポイントの数8個の場合、(4-1)*(8-4+1)=15点)
// よって引数*ansは(M-1)*(K-M+1)個の配列を用意することが望ましい.
int NURBS_Func::CalcIntersecCurve3(NURBSC *nurb,Coord pt,Coord nvec,double *ans,int ans_size)
{
	double **coef;
	double Q[4];	// NURBS曲線の分母の係数
	Coord  P[4];	// NURBS曲線の分子の係数
	double a[4];
	double t[3];
	int ansnum;
	int k=0;

	if((coef = NewMatrix(nurb->M,nurb->M)) == NULL){
		SetMessage("NURBS KOD_ERROR: CalcIntersecPlane3()");
		return KOD_ERR;
	}

	// 1本のNURBS曲線はK-M+1本の曲線から構成される。それぞれの構成曲線に対して方程式を導出し、解を得る。
	for(int i=0;i<nurb->K-nurb->M+1;i++){
		if(nurb->M-1 == 3){			// 3次			
			GetBSplCoef3(nurb->M,nurb->K,i,nurb->T,coef);	// 各コントロールポイントにおける3次Bスプライン基底関数の係数(coef)を求める
		}
		else if(nurb->M-1 == 2){	// 2次
			GetBSplCoef2(nurb->M,nurb->K,i,nurb->T,coef);	// 各コントロールポイントにおける2次Bスプライン基底関数の係数を求める
		}
		else if(nurb->M-1 == 1){	// 1次	
			GetBSplCoef1(nurb->M,nurb->K,i,nurb->T,coef);	// 各コントロールポイントにおける1次Bスプライン基底関数の係数を求める
		}
		else{
			char mes[256];
			sprintf(mes,"NURBS KOD_ERROR:Ther order of equation is unsupported. (order = %d)",nurb->M-1);
			SetMessage(mes);
			goto EXIT;
		}
		GetNurbsCCoef(nurb,coef,i,P,Q);						// NURBS曲線の係数(P,Q)を求める
		GetIntersecEquation(nurb->M,P,Q,pt,nvec,a);			// NURBS曲線と平面の交線導出用方程式を得る
		ansnum = CalcEquation(a,t,nurb->M-1);					// 方程式を解き、交点のパラメータ値を得る

		for(int j=0;j<ansnum;j++){
			if(t[j] >= nurb->T[i+nurb->M-1] && t[j] <= nurb->T[i+nurb->M]){	// ノットベクトルの値と適合するもののみ解として抽出
				if(k == ans_size){
					SetMessage("NURBS KOD_ERROR:Intersection points exceeded the allocated array length");
					goto EXIT;
				}
				ans[k] = t[j];		// 解を取得
				k++;				// 解の数をインクリメント
			}
		}
	}

	return k;

EXIT:
	FreeMatrix(coef,nurb->M);
	return KOD_ERR;
}

// 3次方程式までを解く
int NURBS_Func::CalcEquation(double *a,double *t,int M)
{
	int flag;

	if(M == 3)		flag = CalcCubicEquation(a,t);
	else if(M == 2)	flag = CalcQuadraticEquation(a,t);
	else if(M == 1) flag = CalcLinearEquation(a,t);
	else			return KOD_ERR;

	return flag;
}

// NURBS曲線と平面の交線導出用方程式を得る
void NURBS_Func::GetIntersecEquation(int M,Coord *P,double *Q,Coord pt,Coord nvec,double *a)
{
	for(int i=0;i<M;i++){
		a[i] = (Q[i]*pt.x-P[i].x)*nvec.x + (Q[i]*pt.y-P[i].y)*nvec.y + (Q[i]*pt.z-P[i].z)*nvec.z;
	}
}

// NURBS曲線の係数を求める(最高3次)
int NURBS_Func::GetNurbsCCoef(NURBSC *nurb,double **coef,int i,Coord *P,double *Q)
{
	for(int j=0;j<nurb->M;j++){
		InitCoord(&P[j]);
		Q[j] = 0;
	}

	for(int j=0;j<nurb->M;j++){
		for(int k=0;k<nurb->M;k++){
			Q[j] += coef[k][j]*nurb->W[i+k];
			P[j] = AddCoord(P[j],MulCoord(nurb->cp[i+k],coef[k][j]*nurb->W[i+k]));
		}
	}
	
	return KOD_TRUE;
}

// 3次のBスプライン曲線の各係数を求める
// 引数 M:階数  K:コントロールポイントの数  i:注目中のコントロールポイント *t:ノットベクトル列  *coef:算出される係数を格納
// coef[j][0]t^3 + coef[j][1]t^2 + coef[j][2]t + coef[j][3]   (Nj,4)
int NURBS_Func::GetBSplCoef3(int M,int K,int i,double *t,double **coef)
{
	double bunbo[8];
	double t10,t20,t21,t30,t31,t32,t41,t42,t43;

	for(int j=0;j<4;j++){

		t10 = t[i+j+1] - t[i+j];
		t20 = t[i+j+2] - t[i+j];
		t21 = t[i+j+2] - t[i+j+1];
		t30 = t[i+j+3] - t[i+j];
		t31 = t[i+j+3] - t[i+j+1];
		t32 = t[i+j+3] - t[i+j+2];
		t41 = t[i+j+4] - t[i+j+1];
		t42 = t[i+j+4] - t[i+j+2];
		t43 = t[i+j+4] - t[i+j+3];

		bunbo[0] = t30*t20*t10;
		bunbo[1] = t30*t20*t21;
		bunbo[2] = t30*t31*t21;
		bunbo[3] = t30*t31*t32;
		bunbo[4] = t41*t31*t21;
		bunbo[5] = t41*t31*t32;
		bunbo[6] = t41*t42*t32;
		bunbo[7] = t41*t42*t43;

		double coef_sub[8][4] = 
		{{1,-3*t[i+j],3*t[i+j]*t[i+j],-t[i+j]*t[i+j]*t[i+j]},
		{-1,t[i+j+2]+2*t[i+j],-2*t[i+j]*t[i+j+2]-t[i+j]*t[i+j],t[i+j]*t[i+j]*t[i+j+2]},
		{-1,t[i+j+3]+t[i+j+1]+t[i+j],-(t[i+j+1]+t[i+j])*t[i+j+3]-t[i+j]*t[i+j+1],t[i+j]*t[i+j+1]*t[i+j+3]},
		{1,-2*t[i+j+3]-t[i+j],t[i+j+3]*t[i+j+3]+2*t[i+j]*t[i+j+3],-t[i+j]*t[i+j+3]*t[i+j+3]},
		{-1,t[i+j+4]+2*t[i+j+1],-2*t[i+j+1]*t[i+j+4]-t[i+j+1]*t[i+j+1],t[i+j+1]*t[i+j+1]*t[i+j+4]},
		{1,-t[i+j+4]-t[i+j+3]-t[i+j+1],(t[i+j+3]+t[i+j+1])*t[i+j+4]+t[i+j+1]*t[i+j+3],-t[i+j+1]*t[i+j+3]*t[i+j+4]},
		{1,-2*t[i+j+4]-t[i+j+2],t[i+j+4]*t[i+j+4]+2*t[i+j+2]*t[i+j+4],-t[i+j+2]*t[i+j+4]*t[i+j+4]},
		{-1,3*t[i+j+4],-3*t[i+j+4]*t[i+j+4],t[i+j+4]*t[i+j+4]*t[i+j+4]}};

		for(int p=0;p<8;p++){
			if(bunbo[p] != 0){
				for(int q=0;q<4;q++){
					coef_sub[p][q] /= bunbo[p];
				}
			}
			else{
				for(int q=0;q<4;q++){
					coef_sub[p][q] = 0;
				}
			}
		}

		InitVector(coef[j],4);
		for(int k=0;k<4;k++){
			if(j==0)
				coef[0][k] += coef_sub[7][k];
			else if(j==1)
				coef[1][k] += coef_sub[3][k] + coef_sub[5][k] + coef_sub[6][k];
			else if(j==2)
				coef[2][k] += coef_sub[1][k] + coef_sub[2][k] + coef_sub[4][k];
			else
				coef[3][k] += coef_sub[0][k];
		}
	}

	return KOD_TRUE;
}


// 2次のBスプライン曲線の各係数を求める
// 引数 M:階数  K:コントロールポイントの数  i:注目中のコントロールポイント *t:ノットベクトル列  *coef:算出される係数を格納
// coef[j][0]t^2 + coef[j][1]t + coef[j][2]
int NURBS_Func::GetBSplCoef2(int M,int K,int i,double *t,double **coef)
{
	double t20,t10,t21,t31,t32;
	double bunbo[4];

	for(int j=0;j<3;j++){

		t20 = t[i+j+2] - t[i+j];
		t10 = t[i+j+1] - t[i+j];
		t21 = t[i+j+2] - t[i+j+1];
		t31 = t[i+j+3] - t[i+j+1];
		t32 = t[i+j+3] - t[i+j+2];

		bunbo[0] = t20*t10;
		bunbo[1] = t20*t21;
		bunbo[2] = t31*t21;
		bunbo[3] = t31*t32;

		double coef_sub[4][3] = {{1,-2*t[i+j],t[i+j]*t[i+j]},{-1,t[i+j]+t[i+j+2],-t[i+j]*t[i+j+2]},
		{-1,t[i+j+1]+t[i+j+3],-t[i+j+1]*t[i+j+3]},{1,-2*t[i+j+3],t[i+j+3]*t[i+j+3]}};

		for(int p=0;p<4;p++){
			if(bunbo[p] != 0){
				for(int q=0;q<3;q++){
					coef_sub[p][q] /= bunbo[p];
				}
			}
			else{
				for(int q=0;q<3;q++){
					coef_sub[p][q] = 0;
				}
			}
		}

		InitVector(coef[j],3);
		for(int k=0;k<3;k++){
			if(j==0)
				coef[0][k] += coef_sub[3][k];
			else if(j==1)
				coef[1][k] += coef_sub[1][k] + coef_sub[2][k];
			else
				coef[2][k] += coef_sub[0][k];
		}
	}

	return KOD_TRUE;
}

// 1次のBスプライン曲線の各係数を求める
// 引数 M:階数  K:コントロールポイントの数  i:注目中のコントロールポイント *t:ノットベクトル列  *coef:算出される係数を格納
// coef[j][0]t + coef[j][1]
int NURBS_Func::GetBSplCoef1(int M,int K,int i,double *t,double **coef)
{
	double bunbo[2];

	for(int j=0;j<2;j++){

		bunbo[0] = t[i+j+1] - t[i+j];
		bunbo[1] = t[i+j+2] - t[i+j+1];

		double coef_sub[2][2] = {{1,-t[i+j]},{-1,t[i+j+2]}};

		for(int p=0;p<2;p++){
			if(bunbo[p] != 0){
				for(int q=0;q<2;q++){
					coef_sub[p][q] /= bunbo[p];
				}
			}
			else{
				for(int q=0;q<2;q++){
					coef_sub[p][q] = 0;
				}
			}
		}

		InitVector(coef[j],2);
		for(int k=0;k<2;k++){
			if(j==0)
				coef[0][k] += coef_sub[1][k];
			else
				coef[1][k] += coef_sub[0][k];
		}
	}

	return KOD_TRUE;
}


// NURBS曲面のシフト
// 引数  *nurbs:変更されるNURBS曲面  shift:シフト量
void NURBS_Func::ShiftNurbsS(NURBSS *nurbs,Coord shift)
{
	for(int i=0;i<nurbs->K[0];i++){
		for(int j=0;j<nurbs->K[1];j++){
			nurbs->cp[i][j] = AddCoord(nurbs->cp[i][j],shift);
		}
	}
}

// NURBS曲線のシフト
// 引数  *nurbs:変更されるNURBS曲線  shift:シフト量
void NURBS_Func::ShiftNurbsC(NURBSC *nurbs,Coord shift)
{
	for(int i=0;i<nurbs->K;i++){
		nurbs->cp[i] = AddCoord(nurbs->cp[i],shift);
	}
}

// NURBS曲面をDベクトル回りにdeg(°)だけ回転させる
// 引数　*nurbs:変更されるNURBS曲面　axis:回転軸の単位ベクトル　deg:角度(degree)
void NURBS_Func::RotNurbsS(NURBSS *nurbs,Coord axis,double deg)
{
	double rad;			// ラジアン格納用
	QUATERNION QFunc;	// クォータニオン関連の関数を集めたクラスのオブジェクトを生成
	Quat StartQ;		// 回転前の座標を格納するクォータニオン
	Quat RotQ;			// 回転クォータニオン
	Quat ConjuQ;		// 共役クォータニオン
	Quat TargetQ;		// 回転後の座標を格納するクォータニオン
	
	for(int i=0;i<nurbs->K[0];i++){			// u方向のコントロールポイント分ループ
		for(int j=0;j<nurbs->K[1];j++){		// v方向のコントロールポイント分ループ
			StartQ = QFunc.QInit(1,nurbs->cp[i][j].x,nurbs->cp[i][j].y,nurbs->cp[i][j].z);		// NURBS曲面を構成するcpの座標を登録
			rad = DegToRad(deg);										// degreeからradianに変換
			RotQ = QFunc.QGenRot(rad,axis.x,axis.y,axis.z);				// 回転クォータニオンに回転量を登録(ここの数字をいじれば任意に回転できる)
			ConjuQ = QFunc.QConjugation(RotQ);							// RotQの共役クォータニオンを登録
			TargetQ = QFunc.QMult(QFunc.QMult(RotQ,StartQ),ConjuQ);		// 回転させる
			nurbs->cp[i][j] = SetCoord(TargetQ.x,TargetQ.y,TargetQ.z);	// 回転後の座標を登録
		}
	}
}

// NURBS曲面をDベクトル回りにdeg(°)だけ回転させる
// 引数　*nurbs:変更されるNURBS曲線　axis:回転軸の単位ベクトル　deg:角度(degree)
void NURBS_Func::RotNurbsC(NURBSC *nurbs,Coord axis,double deg)
{
	double rad;			// ラジアン格納用
	QUATERNION QFunc;	// クォータニオン関連の関数を集めたクラスのオブジェクトを生成
	Quat StartQ;		// 回転前の座標を格納するクォータニオン
	Quat RotQ;			// 回転クォータニオン
	Quat ConjuQ;		// 共役クォータニオン
	Quat TargetQ;		// 回転後の座標を格納するクォータニオン
	
	for(int i=0;i<nurbs->K;i++){		// コントロールポイント分ループ
		StartQ = QFunc.QInit(1,nurbs->cp[i].x,nurbs->cp[i].y,nurbs->cp[i].z);		// NURBS曲面を構成するcpの座標を登録
		rad = DegToRad(deg);									// degreeからradianに変換
		RotQ = QFunc.QGenRot(rad,axis.x,axis.y,axis.z);			// 回転クォータニオンに回転量を登録(ここの数字をいじれば任意に回転できる)
		ConjuQ = QFunc.QConjugation(RotQ);						// RotQの共役クォータニオンを登録
		TargetQ = QFunc.QMult(QFunc.QMult(RotQ,StartQ),ConjuQ);	// 回転させる
		nurbs->cp[i] = SetCoord(TargetQ.x,TargetQ.y,TargetQ.z);	// 回転後の座標を登録
	}
}

// NURBS曲面の倍率を変更する
// 引数  *nurbs:変更されるNURBS曲面  ratio:倍率
void NURBS_Func::ChRatioNurbsS(NURBSS *nurbs,Coord ratio)
{
	for(int i=0;i<nurbs->K[0];i++){
		for(int j=0;j<nurbs->K[1];j++){
			nurbs->cp[i][j] = MulCoord(nurbs->cp[i][j],ratio);
		}
	}
}

// NURBS曲線の倍率を変更する
// 引数  *nurbs:変更されるNURBS曲線  ratio:倍率
void NURBS_Func::ChRatioNurbsC(NURBSC *nurbs,Coord ratio)
{
	for(int i=0;i<nurbs->K;i++){
		nurbs->cp[i] = MulCoord(nurbs->cp[i],ratio);
	}
}

// NURBS曲面間のコントロールポイントの代入
// 引数  *nurbs:代入される方  Nurbs:代入元
int NURBS_Func::SetCPNurbsS(NURBSS *nurbs,NURBSS Nurbs)
{
	if(nurbs->K[0] != Nurbs.K[0] || nurbs->K[1] != Nurbs.K[1]){
		SetMessage("NURBS KOD_ERROR:Control point count is different");
		return KOD_ERR;
	}

	for(int i=0;i<Nurbs.K[0];i++){
		for(int j=0;j<Nurbs.K[1];j++){
			nurbs->cp[i][j] = SetCoord(Nurbs.cp[i][j]);
		}
	}

	return KOD_TRUE;
}

// 与えられた点列を補間するn階のNURBS曲線を生成する
// 端末条件を与えないバージョン
// 引数   *Nurbs:生成されるNURBS曲線のアドレス   *P:点列   PNum:点列の数   M:階数
int NURBS_Func::GenInterpolatedNurbsC1(NURBSC *Nurbs,Coord *P,int PNum,int M)
{
	int retflag = KOD_TRUE;

	if(PNum <= 1){			// 与えられた点が1個未満の場合は、NURBS曲線を生成できない
		SetMessage("NURBS KOD_ERROR:Few Point. You should set over 2 points at least");
		return KOD_ERR;
	}
	if(PNum == 2 || PNum == 3)	M = PNum;	// 与えられた点が2個か3個の場合は、階数を強制的に2か3にする

	int K = PNum;			// コントロールポイントの数
	int N = M+K;			// ノットベクトルの数
	int prop[4] = {0,0,1,0};// パラメータ
	double V[2] = {0,1};	// ノットベクトルの開始値,終了値

	Vector T_ = NewVector(K);	// 通過点上の曲線パラメータ
	Vector T = NewVector(N);	// ノットベクトル
	Matrix B = NewMatrix(K,K);	// Bスプライン基底関数行列
	Matrix B_ = NewMatrix(K,K);	// Bスプライン基底関数行列の逆行列格納用
	Vector W = NewVector(K);	// 重み
	Coord *Q = NewCoord1(K);	// コントロールポイント

	// 通過点上の曲線パラメータを得る
	GetCurveKnotParam2(P,PNum,T_);

	// ノットベクトルを得る
	GetInterpolatedKnot(T_,N,K,M,T);

	// Bスプライン基底関数行列を生成
	for(int i=0;i<K;i++){
		for(int j=0;j<K;j++){
			B[i][j] = CalcBSbasis(T_[i],T,N,j,M);
		}
	}

	// Bスプライン基底関数行列の逆行列を求める
	double det = Gauss(K,B,P,Q);
	if(det == 0){
		SetMessage("NURBS ERROR:Determinant is 0");
		retflag = KOD_ERR;
		goto EXIT;
	}

	// コントロールポイントと重みを得る
	for(int i=0;i<K;i++){
		//MulMxVec(B_,K,K,P,Q);
		W[i] = 1.0;
	}

	// NURBS曲線を生成する
	if(M == 2)
		GenNurbsC(Nurbs,K,M,N,T,W,P,V,prop,0);
	else
		GenNurbsC(Nurbs,K,M,N,T,W,Q,V,prop,0);

EXIT:
	FreeVector(T_);
	FreeVector(T);
	FreeVector(W);
	FreeMatrix(B,K);
	FreeMatrix(B_,K);
	FreeCoord1(Q);

	return retflag;
}

// 与えられた点列を補間するn階のNURBS曲線を生成する
// 端末条件:始点とC2で一致
// 引数   *Nurbs:生成されるNURBS曲線のアドレス   *P_:通過点列   PNum:通過点列の数   M:階数
// P_[0]とP_[PNum-1]は一致していること
// 返値   KOD_TRUE:正常終了
//		  KOD_FALSE:点列の始点と終点が一致していない
//		  KOD_ERR:点列の数が1個未満
int NURBS_Func::GenInterpolatedNurbsC2(NURBSC *Nurbs,Coord *P_,int PNum,int M)
{
	if(DiffCoord(P_[0],P_[PNum-1]) == KOD_FALSE){
		SetMessage("NURBS KOD_ERROR:Given points P0 and Pn are not unmuched");
		return KOD_FALSE;
	}
	if(PNum <= 1){			// 与えられた点が1個未満の場合は、NURBS曲線を生成できない
		SetMessage("NURBS KOD_ERROR:Few Point. You should set over 2 points at least");
		return KOD_ERR;
	}
	if(PNum == 2 || PNum == 3)	M = PNum;	// 与えられた点が2個か3個の場合は、階数を強制的に2か3にする

	int K = PNum+M-2;			// コントロールポイントの数
	int N = M+K;				// ノットベクトルの数
	int prop[4] = {0,0,1,0};	// パラメータ
	double V[2] = {0,1};		// ノットベクトルの開始値,終了値

	Vector T_ = NewVector(PNum);	// 通過点上の曲線パラメータ
	Vector T = NewVector(N);		// ノットベクトル
	Coord *P = NewCoord1(N);		// 通過点列を格納
	Coord *Q = NewCoord1(K);		// コントロールポイント
	Matrix B = NewMatrix(K,K);		// Bスプライン基底関数行列
	Vector W = NewVector(K);		// 重み

	// 通過点列ベクトルを生成
	for(int i=0;i<PNum;i++){
		P[i] = SetCoord(P_[i]);
	}
	P[PNum] = SetCoord(0,0,0);
	P[PNum+1] = SetCoord(0,0,0);

	// 通過点上の曲線パラメータを得る
	GetCurveKnotParam1(P_,PNum,T_);

	// ノットベクトルを得る
	for(int i=0;i<N;i++){
		if(i < M)	T[i] = 0;
		else if(i >= K)	T[i] = 1;
		else{
			T[i] = T_[i-M+1];
		}
	}

	// Bスプライン基底関数行列を生成
	for(int i=0;i<K;i++){
		for(int j=0;j<K;j++){
			B[i][j] = 0;
			if(i<K-2 && (j==i || j==i+1 || j==i+2))
				B[i][j] = CalcBSbasis(T_[i],T,N,j,M);
		}
	}
	B[K-2][0] = CalcDiffBSbasis(T_[0],T,N,0,M);
	B[K-2][1] = CalcDiffBSbasis(T_[0],T,N,1,M);
	B[K-2][K-2] = -CalcDiffBSbasis(T_[PNum-1],T,N,K-2,M);
	B[K-2][K-1] = -CalcDiffBSbasis(T_[PNum-1],T,N,K-1,M);
	B[K-1][0] = CalcDiffBSbasisN(T_[0],T,N,0,M,2);
	B[K-1][1] = CalcDiffBSbasisN(T_[0],T,N,1,M,2);
	B[K-1][2] = CalcDiffBSbasisN(T_[0],T,N,2,M,2);
	B[K-1][K-3] = -CalcDiffBSbasisN(T_[PNum-1],T,N,K-3,M,2);
	B[K-1][K-2] = -CalcDiffBSbasisN(T_[PNum-1],T,N,K-2,M,2);
	B[K-1][K-1] = -CalcDiffBSbasisN(T_[PNum-1],T,N,K-1,M,2);

	// コントロールポイントを得る
	Gauss(K,B,P,Q);

	//for(int i=0;i<K;i++)
	//	fprintf(stderr,"%lf,%lf,%lf\n",Q[i].x,Q[i].y,Q[i].z);

	// 重みを得る
	for(int i=0;i<K;i++){
		W[i] = 1.0;
	}

	// NURBS曲線を生成する
	if(M == 2)
		GenNurbsC(Nurbs,K,M,N,T,W,P,V,prop,0);
	else
		GenNurbsC(Nurbs,K,M,N,T,W,Q,V,prop,0);


	FreeVector(T_);
	FreeVector(T);
	FreeVector(W);
	FreeMatrix(B,K);
	FreeCoord1(Q);
	FreeCoord1(P);
}


// 与えられた点列を近似するn階のNURBS曲線を生成する
// 引数   *Nurbs:生成されるNURBS曲線のアドレス   *P:点列   PNum:点列の数   M:階数
int NURBS_Func::GenApproximationNurbsC(NURBSC *Nurbs,Coord *P,int PNum,int M)
{
	if(PNum <= 1){			// 与えられた点が1個未満の場合は、NURBS曲線を生成できない
		SetMessage("NURBS KOD_ERROR:Few Point. You should set over 2 points at least");
		return KOD_ERR;
	}

	int K = SetApproximationCPnum(PNum);		// 与えられた点列からコントロールポイントの数を決める(コントロールポイントの数で近似される曲線が変わる)
	int Nnum = M+K;					// ノットベクトルの数
	int prop[4] = {0,0,1,0};		// パラメータ
	double V[2] = {0,1};			// ノットベクトルの開始値,終了値

	Vector T_ = NewVector(PNum);	// 通過点上の曲線パラメータ
	Vector T = NewVector(Nnum);		// ノットベクトル
	Coord *Q = NewCoord1(K);		// コントロールポイント
	Vector W = NewVector(K);		// 重み

	GetCurveKnotParam1(P,PNum,T_);		// 通過点上の曲線パラメータを得る

	GetApproximatedKnot(T_,PNum,M,K,T);	// ノットベクトルを設定する

	CalcApproximationCP_LSM(P,T_,T,PNum,Nnum,M,K,Q);	// 最小2乗法で近似コントロールポイントを求める

	for(int i=0;i<K;i++){	// 重みは1で固定
		W[i] = 1;
	}

	GenNurbsC(Nurbs,K,M,Nnum,T,W,Q,V,prop,0);	// NURBS曲線生成

	FreeCoord1(Q);
	FreeVector(T);
	FreeVector(T_);
	FreeVector(W);

	return KOD_TRUE;
}

// コントロールポイントからNURBS曲線を生成する
// ノットベクトルは等間隔に設定される
// 重みは全て1とする
// 引数   *Nurbs:生成されるNURBS曲線のアドレス   *P:点列   PNum:点列の数   M:階数
int NURBS_Func::GenNurbsCfromCP(NURBSC *Nurbs,Coord *P,int PNum,int M)
{
	if(PNum <= 1){			// 与えられた点が1個未満の場合は、NURBS曲線を生成できない
		SetMessage("NURBS KOD_ERROR:Few Point. You should set over 2 points at least");
		return KOD_ERR;
	}

	int Nnum = M+PNum;				// ノットベクトルの数
	int prop[4] = {0,0,1,0};		// パラメータ
	double V[2] = {0,1};			// ノットベクトルの開始値,終了値
	Vector T = NewVector(Nnum);		// ノットベクトル
	Vector W = NewVector(PNum);		// 重み

	GetEqIntervalKont(PNum,M,T);	// ノットベクトルを得る

	for(int i=0;i<PNum;i++){	// 重みは1で固定
		W[i] = 1;
	}

	GenNurbsC(Nurbs,PNum,M,Nnum,T,W,P,V,prop,0);	// NURBS曲線生成

	FreeVector(T);
	FreeVector(W);

	return KOD_TRUE;
}

// 折れ線を生成する
// 引数   *Nurbs:生成されるNURBS曲線のアドレス   *P:コントロールポイント   PNum:コントロールポイントの数
int NURBS_Func::GenPolygonalLine(NURBSC *Nurbs,Coord *P,int PNum)
{
	if(PNum <= 1){			// 与えられた点が1個未満の場合は、NURBS曲線を生成できない
		SetMessage("NURBS KOD_ERROR:Few Point. You should set over 2 points at least");
		return KOD_ERR;
	}

	int M=2;					// 階数2
	int K=PNum;					// コントロールポイントの数
	int N=PNum+2;				// ノットベクトルの数
	int prop[4] = {0,0,1,0};	// パラメータ
	double V[2] = {0,1};		// ノットベクトルの開始値,終了値
	Vector T = NewVector(N);	// ノットベクトル
	Vector W = NewVector(K);	// 重み

	// ノットベクトルを求める
	T[0] = T[1] = 0.0;
	T[K] = T[K+1] = 1.0;
	double d_sum=0;
	for(int i=1;i<K;i++)
		d_sum += CalcDistance(P[i],P[i-1]);
	for(int i=2;i<K;i++){
		double d = CalcDistance(P[i-1],P[i-2]);
		T[i] = T[i-1] + d/d_sum;
	}

	// ウェイト
	for(int i=0;i<K;i++){
		W[i] = 1.0;
	}

	// NURBS曲線を生成する
	GenNurbsC(Nurbs,K,M,N,T,W,P,V,prop,0);

	FreeVector(T);
	FreeVector(W);

	return KOD_TRUE;
}

// 与えられた点列を補間するn階NURBS曲面を生成する
// 端末条件を与えないバージョン
// 引数   *Nurbs:生成されるNURBS曲面のアドレス   **P:与えられた点列   PNum_u,PNum_v:点の数　 Mu,Mv:階数
int NURBS_Func::GenInterpolatedNurbsS1(NURBSS *Nurbs,Coord **P,int PNum_u,int PNum_v,int Mu,int Mv)
{
	if(PNum_u <= 1 || PNum_v <= 1){			// 与えられた点が各方向で1個未満の場合は、NURBS曲面を生成できない
		SetMessage("NURBS ERROR:Few Point. You should set over 2 points at least");
		return KOD_ERR;
	}
	if(PNum_u == 2 || PNum_u == 3)	Mu = PNum_u;	// u方向に与えられた点が2個か3個の場合は、u方向の階数を強制的に2か3にする
	if(PNum_v == 2 || PNum_v == 3)	Mv = PNum_v;	// v方向に与えられた点が2個か3個の場合は、v方向の階数を強制的に2か3にする

	int K[2] = {PNum_u,PNum_v};		// コントロールポイントの数
	int N[2] = {Mu+PNum_u,Mv+PNum_v};	// ノットベクトルの数
	int prop[5] = {0,0,1,0,0};		// パラメータ
	double U[2] = {0,1};			// u方向ノットベクトルの開始値、終了値
	double V[2] = {0,1};			// v方向ノットベクトルの開始値、終了値

	Vector S_ = NewVector(K[0]);		// u方向の通過点上の曲線パラメータ
	Vector S = NewVector(N[0]);			// u方向のノットベクトル
	Vector T_ = NewVector(K[1]);		// v方向の通過点上の曲線パラメータ
	Vector T = NewVector(N[1]);			// v方向のノットベクトル
	Matrix Bu = NewMatrix(K[0],K[0]);	// u方向のBスプライン基底関数行列
	Matrix Bu_ = NewMatrix(K[0],K[0]);	// u方向のBスプライン基底関数行列の逆行列格納用
	Matrix Bv = NewMatrix(K[1],K[1]);	// v方向のBスプライン基底関数行列
	Matrix Bv_ = NewMatrix(K[1],K[1]);	// v方向のBスプライン基底関数行列の逆行列格納用
	Matrix W = NewMatrix(K[0],K[1]);	// 重み
	Coord **PT = NewCoord2(K[1],K[0]);	// 転置した点列P
	Coord **R = NewCoord2(K[0],K[1]);	// アイソパラ曲線のコントロールポイント
	Coord **RT = NewCoord2(K[1],K[0]);	// 転置したコントロールポイントR
	Coord **Q = NewCoord2(K[0],K[1]);	// NURBS曲面のコントロールポイント


	GetSurfaceKnotParam(S_,T_,P,PNum_u,PNum_v);		// 補間曲面用u,vパラメータを得る

	GetInterpolatedKnot(S_,N[0],K[0],Mu,S);			// ノットベクトルSを得る

	GetInterpolatedKnot(T_,N[1],K[1],Mv,T);			// ノットベクトルTを得る

	// u方向のBスプライン基底関数行列を生成
	for(int i=0;i<K[0];i++){
		for(int j=0;j<K[0];j++){
			Bu[i][j] = CalcBSbasis(S_[i],S,N[0],j,Mu);
		}
	}

	// v方向のBスプライン基底関数行列を生成
	for(int i=0;i<K[1];i++){
		for(int j=0;j<K[1];j++){
			Bv[i][j] = CalcBSbasis(T_[i],T,N[1],j,Mv);
		}
	}

	// u方向のBスプライン基底関数行列の逆行列を求める
	MatInv(K[0],Bu,Bu_);

	// v方向のBスプライン基底関数行列の逆行列を求める
	MatInv(K[1],Bv,Bv_);

	// アイソパラ曲線のコントロールポイントを得る
	TranMx(P,K[0],K[1],PT);
	for(int i=0;i<K[1];i++){
		MulMxVec(Bu_,K[0],K[0],PT[i],RT[i]);
	}

	// NURBS曲面のコントロールポイントを得る
	TranMx(RT,K[1],K[0],R);
	for(int i=0;i<K[0];i++){
 		MulMxVec(Bv_,K[1],K[1],R[i],Q[i]);
 	}

	// 重みを得る
	for(int i=0;i<K[0];i++){
		for(int j=0;j<K[1];j++){
			W[i][j] = 1;
		}
	}

	// NURBS曲面を生成する
	if(Mu == 2 && Mv == 2)
		GenNurbsS(Nurbs,Mu,Mv,K[0],K[1],S,T,W,P,U[0],U[1],V[0],V[1]);
	else
		GenNurbsS(Nurbs,Mu,Mv,K[0],K[1],S,T,W,Q,U[0],U[1],V[0],V[1]);

	FreeVector(S_);
	FreeVector(S);
	FreeVector(T_);
	FreeVector(T);
	FreeMatrix(Bu,K[0]);
	FreeMatrix(Bu_,K[0]);
	FreeMatrix(Bv,K[1]);
	FreeMatrix(Bv_,K[1]);
	FreeMatrix(W,K[0]);
	FreeCoord2(PT,K[1]);
	FreeCoord2(R,K[0]);
	FreeCoord2(RT,K[1]);
	FreeCoord2(Q,K[0]);

	return KOD_TRUE;
}

// 与えられた点列を近似するn階のNURBS曲面を生成する
// 引数   *Nurbs:生成されるNURBS曲面のアドレス   **P:与えられた点列   PNum_u,PNum_v:点の数　 Mu,Mv:階数
int NURBS_Func::GenApproximationNurbsS(NURBSS *Nurbs,Coord **P,int PNum_u,int PNum_v,int Mu,int Mv)
{
	if(PNum_u <= 1 || PNum_v <= 1){			// 与えられた点が各方向で1個未満の場合は、NURBS曲面を生成できない
		SetMessage("NURBS ERROR:Few Point. You should set over 2 points at least");
		return KOD_ERR;
	}
	if(PNum_u == 2 || PNum_u == 3)	Mu = PNum_u;	// u方向に与えられた点が2個か3個の場合は、u方向の階数を強制的に2か3にする
	if(PNum_v == 2 || PNum_v == 3)	Mv = PNum_v;	// v方向に与えられた点が2個か3個の場合は、v方向の階数を強制的に2か3にする

	// 与えられた点列からコントロールポイントの数を決める
	int K[2];
	K[0] = SetApproximationCPnum(PNum_u);
	K[1] = SetApproximationCPnum(PNum_v);

	int N[2] = {Mu+K[0],Mv+K[1]};	// ノットベクトルの数
	int prop[5] = {0,0,1,0,0};		// パラメータ
	double U[2] = {0,1};			// u方向ノットベクトルの開始値、終了値
	double V[2] = {0,1};			// v方向ノットベクトルの開始値、終了値

	Vector S_ = NewVector(PNum_u);		// u方向の通過点上の曲線パラメータ
	Vector S = NewVector(N[0]);			// u方向のノットベクトル
	Vector T_ = NewVector(PNum_v);		// v方向の通過点上の曲線パラメータ
	Vector T = NewVector(N[1]);			// v方向のノットベクトル
	Coord **Q1 = NewCoord2(PNum_u,K[1]);	// NURBS曲面のコントロールポイント
	Coord **Q2 = NewCoord2(K[1],PNum_u);	
	Coord **Q3 = NewCoord2(K[1],K[0]);
	Coord **Q4 = NewCoord2(K[0],K[1]);
	Coord **P_ = NewCoord2(K[1],K[0]);
	Matrix W = NewMatrix(K[0],K[1]);	// 重み

	GetSurfaceKnotParam(S_,T_,P,PNum_u,PNum_v);		// 補間曲面用u,vパラメータを得る

	GetApproximatedKnot(S_,PNum_u,Mu,K[0],S);		// ノットベクトルSを設定する
	GetApproximatedKnot(T_,PNum_v,Mv,K[1],T);		// ノットベクトルTを設定する

	// v方向の点列から近似NURBS曲線をPNum_u個作成する
	for(int i=0;i<PNum_u;i++){
		CalcApproximationCP_LSM(P[i],T_,T,PNum_v,N[1],Mv,K[1],Q1[i]);	// 最小2乗法で近似コントロールポイントを求める
	}
	TranMx(Q1,PNum_u,K[1],Q2);					// Qの転置

	for(int i=0;i<K[1];i++){
		CalcApproximationCP_LSM(Q2[i],S_,S,PNum_u,N[0],Mu,K[0],Q3[i]);	// 最小2乗法で近似コントロールポイントを求める
	}
	TranMx(Q3,K[1],K[0],Q4);					// Qの転置

	// 重みを得る
	for(int i=0;i<K[0];i++){
		for(int j=0;j<K[1];j++){
			W[i][j] = 1;
		}
	}

	// NURBS曲面を生成する
	if(Mu == 2 && Mv == 2)
		GenNurbsS(Nurbs,Mu,Mv,K[0],K[1],S,T,W,P,U[0],U[1],V[0],V[1]);
	else
		GenNurbsS(Nurbs,Mu,Mv,K[0],K[1],S,T,W,Q4,U[0],U[1],V[0],V[1]);

	FreeVector(S);
	FreeVector(T);
	FreeVector(S_);
	FreeVector(T_);
	FreeCoord2(Q1,K[1]);
	FreeCoord2(Q2,PNum_u);
	FreeCoord2(Q3,K[0]);
	FreeCoord2(Q4,K[1]);
	FreeCoord2(P_,K[0]);
	FreeMatrix(W,K[1]);


	return KOD_TRUE;
}

// 与えられたコントロールポイントからn階のNURBS曲面を生成する
// ノットベクトルは等間隔に設定される
// 重みは全て1とする
// 引数   *Nurbs:生成されるNURBS曲面のアドレス   **P:与えられたコントロールポイント列   PNum_u,PNum_v:点の数　 Mu,Mv:階数
int NURBS_Func::GenNurbsSfromCP(NURBSS *Nurbs,Coord **P,int PNum_u,int PNum_v,int Mu,int Mv)
{
	if(PNum_u <= 1 || PNum_v <= 1){			// 与えられた点が各方向で1個未満の場合は、NURBS曲面を生成できない
		SetMessage("NURBS ERROR:Few Point. You should set over 2 points at least");
		return KOD_ERR;
	}
	if(PNum_u == 2 || PNum_u == 3)	Mu = PNum_u;	// u方向に与えられた点が2個か3個の場合は、u方向の階数を強制的に2か3にする
	if(PNum_v == 2 || PNum_v == 3)	Mv = PNum_v;	// v方向に与えられた点が2個か3個の場合は、v方向の階数を強制的に2か3にする

	int K[2] = {PNum_u,PNum_v};			// コントロールポイントの数		
	int N[2] = {Mu+K[0],Mv+K[1]};		// ノットベクトルの数
	int prop[5] = {0,0,1,0,0};			// パラメータ
	double U[2] = {0,1};				// u方向ノットベクトルの開始値、終了値
	double V[2] = {0,1};				// v方向ノットベクトルの開始値、終了値
	Vector S = NewVector(N[0]);			// u方向のノットベクトル
	Vector T = NewVector(N[1]);			// v方向のノットベクトル
	Matrix W = NewMatrix(K[0],K[1]);	// 重み

	GetEqIntervalKont(K[0],Mu,S);		// u方向ノットベクトルを得る
	GetEqIntervalKont(K[1],Mv,T);		// v方向ノットベクトルを得る

	// 重みを得る
	for(int i=0;i<K[0];i++){
		for(int j=0;j<K[1];j++){
			W[i][j] = 1;
		}
	}

	GenNurbsS(Nurbs,Mu,Mv,K[0],K[1],S,T,W,P,U[0],U[1],V[0],V[1]);		// NURBS曲面を生成する

	FreeVector(S);
	FreeVector(T);
	FreeMatrix(W,K[0]);

	return KOD_TRUE;
}

// 折れ面を生成する
// 引数   *Nurbs:生成されるNURBS曲面のアドレス   **P:コントロールポイント   PNum_u,PNum_v:コントロールポイントの数
int NURBS_Func::GenPolygonalSurface(NURBSS *Nurbs,Coord **P,int PNum_u,int PNum_v)
{
	int Mu=2;						// 階数2
	int Mv=2;
	int K[2] = {PNum_u,PNum_v};		// コントロールポイントの数
	int N[2] = {PNum_u+2,PNum_v+2};	// ノットベクトルの数
	int prop[4] = {0,0,1,0};		// パラメータ
	double U[2] = {0,1};			// u方向ノットベクトルの開始値、終了値
	double V[2] = {0,1};			// v方向ノットベクトルの開始値、終了値
	Vector S = NewVector(N[0]);			// u方向のノットベクトル
	Vector T = NewVector(N[1]);			// v方向のノットベクトル
	Matrix W = NewMatrix(K[0],K[1]);	// 重み

	// u方向ノットベクトルを求める
	Vector du_sum = NewVector(K[1]);
	InitVector(du_sum,K[1]);
	for(int i=0;i<K[1];i++){
		for(int j=1;j<K[0];j++){
			du_sum[i] += CalcEuclid(SubCoord(P[j][i],P[j-1][i]));
		}
	}
	S[0] = S[1] = 0.0;
	for(int i=2;i<K[0];i++){
		S[i] = 0;
		for(int j=0;j<K[1];j++){
			double du = CalcEuclid(SubCoord(P[i-1][j],P[i-2][j]));
			S[i] += S[i-1] + du/du_sum[j];
		}
		S[i] /= K[1];
	}
	S[K[0]] = S[K[0]+1] = 1.0;
	
	// v方向ノットベクトルを求める
	Vector dv_sum = NewVector(K[0]);
	InitVector(dv_sum,K[0]);
	for(int i=0;i<K[0];i++){
		for(int j=1;j<K[1];j++){
			dv_sum[i] += CalcEuclid(SubCoord(P[i][j],P[i][j-1]));
		}
	}
	T[0] = T[1] = 0.0;
	for(int i=2;i<K[1];i++){
		T[i] = 0;
		for(int j=0;j<K[0];j++){
			double dv = CalcEuclid(SubCoord(P[j][i-1],P[j][i-2]));
			T[i] += T[i-1] + dv/dv_sum[j];
		}
		T[i] /= K[0];
	}
	T[K[1]] = T[K[1]+1] = 1.0;

	// ウェイト
	for(int i=0;i<K[0];i++){
		for(int j=0;j<K[1];j++){
			W[i][j] = 1.0;
		}
	}

	// NURBS曲面を生成する
	GenNurbsS(Nurbs,Mu,Mv,K[0],K[1],S,T,W,P,U[0],U[1],V[0],V[1]);

	FreeVector(S);
	FreeVector(T);
	FreeMatrix(W,K[1]);
	FreeVector(du_sum);
	FreeVector(dv_sum);

	return KOD_TRUE;
}

// NURBS曲面と直線の交点を算出
// 引数    *Nurb:NURBS曲面S(u,v)へのポインタ,  r:直線N(t)上の1点,  p:直線N(t)の方向,  DivNum:NURBS曲面の分割数,
//         *ans:交点のu,v,tパラメータ格納用配列, anssize:ansの配列長
//         Divnumが大きいほど、交点算出の取りこぼしは少なくなる
//         anssizeはDivNum*DivNum以上にするのが好ましい.
// 返値    交点の数,   KOD_ERR:交点の数が指定した配列長を超えた
int NURBS_Func::CalcuIntersecPtNurbsLine(NURBSS *Nurb,Coord r,Coord p,int Divnum,Coord *ans,int anssize)
{
	Coord d = SetCoord(100,100,100);		// NURBS曲線S(u,v)の微小変化量(du,dv)、直線N(t)の微小変化量dtを格納
	Coord F,Fu,Fv,Ft;						// F(u,v,t) = S(u,v) - N(t)    Fu = dF/du     Fv = dF/dv     Ft = dF/dt
	double u = Nurb->U[0];					// NURBS曲面S(u,v)のuパラメータの現在値
	double v = Nurb->V[0];					// NURBS曲面S(u,v)のvパラメータの現在値
	double t = 0;							// 直線N(t)のtパラメータ
	Matrix A = NewMatrix(3,3);				// Fu,Fv,Ftを構成する3x3行列
	Matrix A_ = NewMatrix(3,3);				// Aの逆行列を格納
	int flag = KOD_FALSE;						// 収束フラグ
	double dv = (Nurb->V[1] - Nurb->V[0])/(double)Divnum;	// 収束演算用のvパラメータのインターバル値
	double du = (Nurb->U[1] - Nurb->U[0])/(double)Divnum;	// 収束演算用のuパラメータのインターバル値
	int loopcount = 0;						// 収束計算回数
	int anscount = 0;						// 算出された交点の数

	// u loop
	for(int i=0;i<Divnum;i++){
		// v loop
		for(int j=0;j<Divnum;j++){
			u = Nurb->U[0] + (double)i*du;			// ステップパラメータuの初期値をセット
			v = Nurb->V[0] + (double)j*dv;		// ステップパラメータvの初期値をセット
			t = 0;								// ステップパラメータtの初期値をセット
			flag = KOD_FALSE;						// 収束フラグをOFF
			loopcount = 0;						// ループカウント初期化
			// 直線の微小変化量dt(=d.z)がAPPROX_ZEROを下回るまでニュートン法による収束計算を行う
			while(loopcount < LOOPCOUNTMAX){
				F = SubCoord(CalcNurbsSCoord(Nurb,u,v),AddCoord(r,MulCoord(p,t)));	// F(u,v,t) = S(u,v) - N(t) = S(u,v) - (r+tp)
				Fu = CalcDiffuNurbsS(Nurb,u,v);			// Fu = dF/du = dS/du
				Fv = CalcDiffvNurbsS(Nurb,u,v);			// Fv = dF/dv = dS/dv
				Ft = MulCoord(p,-1);					// Ft = dF/dt = -dN/dt = -p
				A[0][0] = Fu.x;				// Fu,Fv,Ftを3x3行列Aに代入
				A[0][1] = Fv.x;				//     |Fu.x Fv.x Ft.x|       |du|       |F.x|
				A[0][2] = Ft.x;				// A = |Fu.y Fv.y Ft.y| , d = |dv| , F = |F.y|
				A[1][0] = Fu.y;				//     |Fu.z Fv.z Ft.z|       |dt|       |F.z|
				A[1][1] = Fv.y;
				A[1][2] = Ft.y;				// A・d = F   --->   d = A_・F
				A[2][0] = Fu.z;
				A[2][1] = Fv.z;
				A[2][2] = Ft.z;	
				//fprintf(stderr,"   %lf,%lf,%lf,%lf,%lf\n",u,v,Fu.x,Fu.y,Fu.z);
				if(MatInv3(A,A_) == KOD_FALSE){		// 逆行列を求める
					flag = KOD_ERR;
					break;		
				}
				d = MulCoord(MulMxCoord(A_,F),-1);			// dを算出
				
				if(fabs(d.x) <= APPROX_ZERO && fabs(d.y) <= APPROX_ZERO && fabs(d.z) <= APPROX_ZERO){	// 真値に収束したらloopを抜ける
					flag = KOD_TRUE;		// 収束フラグtrue
					break;
				}

				// 真値に達していなかったらu,v,tを更新
				u += d.x;
				v += d.y;
				t += d.z;

				//if(u < Nurb->U[0] || u > Nurb->U[1] || v < Nurb->V[0] || v > Nurb->V[1]){	// u,vのどちらかが発散したらloopを抜ける
				//	flag = KOD_FALSE;		// 収束フラグfalse
				//	break;
				//}

				loopcount++;
			}// end of while

			// LOOPCOUNTMAX回ループしても収束していなかったら警告
			if(loopcount == LOOPCOUNTMAX)
				SetMessage("NURBS_Func ERROR: fail to converge");

			// 収束していたら解として登録
			if(flag == KOD_TRUE){
				ans[anscount] = SetCoord(u,v,t);
				anscount++;
				if(anscount > anssize){
					SetMessage("NURBS_Func ERROR: Ans_size overflow");
					return KOD_ERR;
				}
			}
		}// end of j loop
	}// end of i loop

	FreeMatrix(A,3);
	FreeMatrix(A_,3);

	anscount = CheckTheSamePoints(ans,anscount);		// 同一点は除去する

	return anscount;
}

// NURBS曲面S上における空間上の1点Pからの最近傍点Qを求める(ニュートン法)
//
// 直線の方程式L(t) = P + tN
// NはS上の法線ベクトルと一致するからN=Su×Sv
// 方程式：S(u,v) = P + tN(u,v)
// F(u,v,t) = S(u,v) - P - tN(u,v)   として、ニュートン法を用いる
// Fu = Su - tNu	Fv = Sv - tNv	Ft = -N
// |Fu.x Fv.x Ft.x||du|    |F.x|
// |Fu.y Fv.y Ft.y||dv| = -|F.y|    =>     dF・d = -F     =>     d = -F・dF^-1  
// |Fu.z Fv.z Ft.z||dt|    |F.z|
// 
// Divnum：ニュートン法初期値指定用の曲面分割数
// Q：解（S上の点をu,v,tパラメータでCoord構造体に格納）
int NURBS_Func::CalcIntersecPtNurbsPt(NURBSS *S,Coord P,int Divnum,Coord *Q)
{
	Matrix dF = NewMatrix(3,3);		// Fu,Fv,Ftを構成する3x3行列
	Matrix dF_ = NewMatrix(3,3);	// dFの逆行列を格納
	Coord F,Fu,Fv,Ft;				// F(u,v,t) = S(u,v) - P - t・N(u,v)	ニュートン法にかける関数
	Coord N,Nu,Nv;					// N(u,v):S(u,v)上の法線ベクトル
	Coord d = InitCoord();			// ニュートン法によって更新されるステップサイズパラメータ
	int loopcount=0;				// while()ループのカウント
	double u,v,t;					// u,v,tの現在値
	double dv = (S->V[1] - S->V[0])/(double)Divnum;	// 収束演算用のvパラメータのインターバル値
	double du = (S->U[1] - S->U[0])/(double)Divnum;	// 収束演算用のuパラメータのインターバル値
	int flag = KOD_FALSE;			// while()抜け用判別フラグ

	// 各初期値に対してニュートン法適用
	for(int i=0;i<Divnum;i++){
		for(int j=0;j<Divnum;j++){
			u = S->U[0] + (double)i*du;			// ステップパラメータuの初期値をセット
			v = S->V[0] + (double)j*dv;			// ステップパラメータvの初期値をセット
			t = 0;								// ステップパラメータtの初期値をセット
			loopcount = 0;

			// 収束計算
			while(loopcount < LOOPCOUNTMAX){
				N = CalcNormVecOnNurbsS(S,u,v);									// S(u,v)上の法線ベクトルN(u,v)を算出
				Nu = CalcDiffuNormVecOnNurbsS(S,u,v);								// N(u,v)のu方向偏微分
				Nv = CalcDiffvNormVecOnNurbsS(S,u,v);								// N(u,v)のv方向偏微分
				F = SubCoord(SubCoord(CalcNurbsSCoord(S,u,v),P),MulCoord(N,t));	// ニュートン法にかける関数
				Fu = SubCoord(CalcDiffuNurbsS(S,u,v),MulCoord(Nu,t));				// Fのu方向偏微分
				Fv = SubCoord(CalcDiffvNurbsS(S,u,v),MulCoord(Nv,t));				// Fのv方向偏微分
				Ft = MulCoord(N,-1);												// Fのt方向偏微分
				dF[0][0] = Fu.x;		// 3x3マトリックスにFu,Fv,Ftを代入
				dF[0][1] = Fv.x;
				dF[0][2] = Ft.x;
				dF[1][0] = Fu.y;
				dF[1][1] = Fv.y;
				dF[1][2] = Ft.y;
				dF[2][0] = Fu.z;
				dF[2][1] = Fv.z;
				dF[2][2] = Ft.z;

				if((flag = MatInv3(dF,dF_)) == KOD_FALSE){		// 逆行列算出 detが0なら次の初期値へ
					//fprintf(stderr,"%d:det = 0\n",loopcount);	
					break;
				}

				d = MulCoord(MulMxCoord(dF_,F),-1);		// ステップサイズパラメータの更新値を算出

				if(fabs(d.x) <= APPROX_ZERO && fabs(d.y) <= APPROX_ZERO && fabs(d.z) <= APPROX_ZERO){	// 真値に収束したらloopを抜ける
					flag = KOD_TRUE;		// 収束フラグtrue
					break;
				}

				// 真値に達していなかったらu,v,tを更新
				u += d.x;
				v += d.y;
				t += d.z;
				//fprintf(stderr,"%d:%lf,%lf,%lf\n",loopcount,u,v,t);

				loopcount++;
			}// end of while

			// 収束していたら
			if(flag == KOD_TRUE){
				*Q = SetCoord(u,v,t);
				fprintf(stderr,"%lf,%lf\n",u,v);
				FreeMatrix(dF,3);
				FreeMatrix(dF_,3);
				return KOD_TRUE;
			}
		}// end of loop j
	}// end of loop i

	FreeMatrix(dF,3);
	FreeMatrix(dF_,3);

	return KOD_FALSE;		// 収束しなかった
}


// 注目中のNURBS曲面上の1点(u,v)がトリミング領域内にあるのかを判定する
// 返値  KOD_TRUE:面上  KOD_ONEDGE:エッジ上  KOD_FALSE:面外   KOD_ERR:エラー
int NURBS_Func::DetermPtOnTRMSurf(TRMS *Trim,double u,double v)
{
	int flag;

	// 外周トリム
	if(Trim->n1){
		flag = DetermPtOnTRMSurf_sub(Trim->pTO,u,v);
		if(flag == KOD_ERR)
			return KOD_ERR;
		else if(flag == KOD_FALSE)		// 外
			return KOD_FALSE;
		else if(flag == KOD_ONEDGE)		// エッジ上
			return KOD_ONEDGE;
	}

	// 内周トリム
	if(Trim->n2){
		for(int i=0;i<Trim->n2;i++){		// 内周のトリミング領域全てに対して
			flag = DetermPtOnTRMSurf_sub(Trim->pTI[i],u,v);
			if(flag == KOD_ERR)
				return KOD_ERR;
			else if(flag == KOD_TRUE)	// 内
				return KOD_FALSE;
		}
	}

	return KOD_TRUE;
}

// 面上線のタイプが複合曲線の場合のトリミング領域内外判定
int NURBS_Func::DetermPtOnTRMSurf_sub(CONPS *Conps,double u,double v)
{
	// 面上線が複合曲線になっていること
	if(Conps->BType != COMPOSITE_CURVE){
		SetMessage("NURBS_Func ERROR:TRIM未実装!");
		return KOD_ERR;
	}

	COMPC *CompC=(COMPC *)Conps->pB;	// NURBS曲面のパラメータ空間上に構成されている複合曲線へのポインタを取り出す

	double ent_dev=0;
	NURBSC *NurbsC;			// トリム境界線(NURBS曲線)のポインタを作業用に格納
	Coord *P;				// トリム境界線上に生成した点(多角形近似用の点)を格納
	Coord TargetPoint = SetCoord(u,v,0);		// ターゲットとなる面上の点(u,v)をCoordに格納
	int trm_flag = KOD_FALSE;	// トリミング領域内外判定用フラグ
	int divnum = TRM_BORDERDIVNUM;
	int ptnum=0;

	// メモリ確保
	if((P = (Coord *)malloc(sizeof(Coord)*(CompC->N*TRM_BORDERDIVNUM))) == NULL){
		return KOD_ERR;
	}
	
	// トリム境界線上に点を生成（トリム境界線を多角形近似）
	for(int i=0;i<CompC->N;i++){
		// トリム境界線がNURBS曲線で構成されている
		if(CompC->DEType[i] == NURBS_CURVE){
			NurbsC = (NURBSC *)CompC->pDE[i];	// 注目中のNurbs曲線のポインタを取得
			if(NurbsC->K == 2 && CompC->DegeFlag == KOD_TRUE)	divnum = 2;		// コントロールポイントが2つの場合は直線なので、分割点を生成しなくてもよくする
			else divnum = TRM_BORDERDIVNUM;
			for(int j=0;j<divnum-1;j++){
				ent_dev = NurbsC->T[NurbsC->M-1]+(NurbsC->T[NurbsC->K]-NurbsC->T[NurbsC->M-1])*(double)j/((double)divnum-1);	// 分割点tを求める
				P[ptnum] = CalcNurbsCCoord(NurbsC,ent_dev);	// NURBS曲面のパラメータ空間内のNURBS曲線の分割点tの座標値(u,v)を得る
				ptnum++;
			}
		}
		// それ以外
		else{
			SetMessage("NURBS_Func ERROR:トリム境界線がNURBS曲線以外で構成されています.未実装!");
			goto EXIT;
		}
	}
	
	trm_flag = IsPointInPolygon(TargetPoint,P,ptnum);		// 内外判定

EXIT:
	// メモリ解放
	free(P);
	
	return trm_flag;
}

// NURBS曲線C(t)の指定パラメータ区間[a,b]の線分長Lを求める
// L = S|C'(t)|dt	(Sは積分記号)
// 積分は数値積分(ガウス-ルジャンドルの10分点)を用いる
double NURBS_Func::CalcNurbsCLength(NURBSC *Nurb,double a,double b)
{
	// 分点
	double g[10] = {	0.076526521133497333, 0.227785851141645078,
							0.373706088715419561, 0.510867001950827098,
							0.636053680726515025, 0.746331906460150793,
							0.839116971822218823, 0.912234428251325906,
							0.963971927277913791, 0.993128599185094925};
	// 重み
	double w[10] = {	0.152753387130725851, 0.149172986472603747,
							0.142096109318382051, 0.131688638449176627,
							0.118194531961518417, 0.101930119817240435,
							0.083276741576704749, 0.062672048334109064,
							0.040601429800386941, 0.017614007139152118};
	double A = (b+a)/2;
	double B = (b-a)/2;
	double len=0;

	for(int i=0;i<10;i++){
		double xi = A + B*g[i];
		double xi_= A - B*g[i];
		len += w[i]*(CalcEuclid(CalcDiffNurbsC(Nurb,xi))+CalcEuclid(CalcDiffNurbsC(Nurb,xi_)));
	}

	return(B*len);
}
// NURBS曲線C(t)の全区間の線分長Lを求める
// L = S|C'(t)|dt	(Sは積分記号)
// 積分は数値積分(ガウス-ルジャンドルの10分点)を用いる
double NURBS_Func::CalcNurbsCLength(NURBSC *Nurb)
{
	// 分点
	double g[10] = {	0.076526521133497333, 0.227785851141645078,
							0.373706088715419561, 0.510867001950827098,
							0.636053680726515025, 0.746331906460150793,
							0.839116971822218823, 0.912234428251325906,
							0.963971927277913791, 0.993128599185094925};
	// 重み
	double w[10] = {	0.152753387130725851, 0.149172986472603747,
							0.142096109318382051, 0.131688638449176627,
							0.118194531961518417, 0.101930119817240435,
							0.083276741576704749, 0.062672048334109064,
							0.040601429800386941, 0.017614007139152118};
	double A = (Nurb->V[1]+Nurb->V[0])/2;
	double B = (Nurb->V[1]-Nurb->V[0])/2;
	double len=0;

	for(int i=0;i<10;i++){
		double xi = A + B*g[i];
		double xi_= A - B*g[i];
		len += w[i]*(CalcEuclid(CalcDiffNurbsC(Nurb,xi))+CalcEuclid(CalcDiffNurbsC(Nurb,xi_)));
	}

	return(B*len);
}


// 指定した間隔でNURBS曲線上の座標値を出力する
// 引数
// *Nurb:NURBSへのポインタ  D:間隔  *Pts:出力される座標値を格納
// 返値   点数
int NURBS_Func::CalcDeltaPtsOnNurbsC(NURBSC *Nurb,double D,Coord *Pts)
{
	double L = CalcNurbsCLength(Nurb);		// NURBS曲線の線分長を得る
	double T = Nurb->V[1] - Nurb->V[0];	// パラメトリック空間内での線分長を得る
	double t = T/L*D;
	double now=0;	// パラメトリック空間内での現在の注目点

	int i=0;
	while(now < Nurb->V[1]){
		Pts[i] = CalcNurbsCCoord(Nurb,Nurb->V[0]+now);
		now += t;
		i++;
	}

	Pts[i] = CalcNurbsCCoord(Nurb,Nurb->V[1]);
	i++;

	return i;
}

// 同一点を除去する
// 引数   *P:点群(変更後の点群もここに格納される)   N:点数
// 返値   変更後の点数
int NURBS_Func::CheckTheSamePoints(Coord *P,int N)
{
	Coord *Q = NewCoord1(N);

	for(int i=0;i<N;i++)
		P[i].dmy = KOD_FALSE;

	int k=0;
	for(int i=0;i<N;i++){
		if(P[i].dmy == KOD_FALSE){
			Q[k] = SetCoord(P[i]);
			for(int j=0;j<N;j++){
				if(DiffCoord(Q[k],P[j]) == KOD_TRUE){
					P[j].dmy = KOD_TRUE;
				}
			}
			k++;
		}
	}

	for(int i=0;i<k;i++)
		P[i] = SetCoord(Q[i]);

	FreeCoord1(Q);

	return k;
}
int NURBS_Func::CheckTheSamePoints(double *P,int N)
{
	double *Q = NewVector(N);
	bool *flag = (bool *)malloc(sizeof(bool)*N);

	for(int i=0;i<N;i++)
		flag[i] = false;

	int k=0;
	for(int i=0;i<N;i++){
		if(flag[i] == false){
			Q[k] = P[i];
			for(int j=0;j<N;j++){
				if(CheckZero(Q[k]-P[j],MID_ACCURACY) == KOD_TRUE){
					flag[j] = true;
				}
			}
			k++;
		}
	}

	for(int i=0;i<k;i++)
		P[i] = Q[i];

	FreeVector(Q);
	free(flag);

	return k;
}

// 2次元平面内の同一点を除去する
// 座標値はCoordのxとyで与えるものとする
// 引数   *P:点群(変更後の点群もここに格納される)   N:点数
// 返値   変更後の点数
int NURBS_Func::CheckTheSamePoints2D(Coord *P,int N)
{
	Coord *Q = NewCoord1(N);

	for(int i=0;i<N;i++)
		P[i].dmy = KOD_FALSE;

	int k=0;
	for(int i=0;i<N;i++){
		if(P[i].dmy == KOD_FALSE){
			Q[k] = SetCoord(P[i]);
			for(int j=0;j<N;j++){
				if(DiffCoord(Q[k],P[j]) == KOD_TRUE){
					P[j].dmy = KOD_TRUE;
				}
			}
			k++;
		}
	}

	for(int i=0;i<k;i++)
		P[i] = SetCoord(Q[i]);

	FreeCoord1(Q);

	return k;
}


// NURBS曲線の指定した方向における極値の座標値を得る
// 引数    *C：極値座標を求めたいNURBS曲線へのポインタ   nf:方向ベクトル     *pt:得られた極値のNurbs曲線パラメータ値列    ptnum:*ptの配列長
// 返値    得られた極値パラメータの数   KOD_FALSE:得られなかった      KOD_ERR:極値パラメータの数がptnumを超えた
int NURBS_Func::CalcExtremumNurbsC(NURBSC *C,Coord nf,double *pt,int ptnum)
{
	int anscount=0;			// 極値の数

	// NURBS曲線のパラメータ区間をCONVDIVNUMで区切り、それぞれに対してニュートン法による収束計算を行う
	for(int i=0;i<=CONVDIVNUM;i++){
		double t = C->V[0] + (C->V[1] - C->V[0])/CONVDIVNUM*(double)i;	// 探索開始パラメータ値
		double dt=0;					// ニュートン法用の増分値
		int lpcount=0;					// 収束計算回数
		bool flag = false;				// 例外フラグ

		// 収束計算
		while(lpcount < LOOPCOUNTMAX){
			double f_ = CalcInnerProduct(nf,CalcDiffNurbsC(C,t));
			double f__ = CalcInnerProduct(nf,CalcDiff2NurbsC(C,t));
			if(f__ == 0.0)	break;
			dt = f_/f__;

			if(CheckZero(dt,MID_ACCURACY)){			// 収束した
				flag = true;
				break;
			}
			t -= dt;	// ニュートンパラメータ更新
			if(t < C->V[0] || t > C->V[1])	break;		// 範囲外に出た
			lpcount++;
		}// End while

		// 収束していたら
		if(flag == true){
			pt[anscount] = t;	// 解として登録
			anscount++;
			if(anscount == ptnum){
				SetMessage("NURBS_ERROR:range over");
				return KOD_ERR;
			}
		}

	}// End for i

	anscount = CheckTheSamePoints(pt,anscount);		// 同一点を除去する

	return anscount;
}


// 極地探索線を得る
// 極地探索線は2つ得られる
int NURBS_Func::CalcExtSearchCurve(NURBSS *S,Coord n,Coord pt,double ds,NURBSC *C1,NURBSC *C2)
{
	// 工事中
	return KOD_TRUE;
}

// 極地傾斜線を得る
// 極地傾斜線は2つ得られる
int NURBS_Func::CalcExtGradCurve(NURBSS *S,Coord n,Coord pt,double ds,NURBSC *C1,NURBSC *C2)
{
	// 工事中
	return KOD_TRUE;
}

// NURBS曲面を平面でトリムする
int NURBS_Func::TrimNurbsSPlane(TRMS *Trm,Coord pt,Coord nvec)
{
	Coord t[2000];					// 解
	int   num;						// 解の数
	double pcolor[3] = {0,1,0};		// 表示の色
	double tcolor[3] = {1,0,0};


	num = CalcIntersecPtsPlaneSearch(Trm->pts,pt,nvec,0.5,5,t,2000,RUNGE_KUTTA);		// NURBS曲面と平面との交点群を交線追跡法で求める
	
	// パラメトリック領域内で直線近似(最小2乗法で近似直線の係数2つを求める)
	Matrix A = NewMatrix(2,2);
	Matrix A_ = NewMatrix(2,2);
	Vector B = NewVector(2);
	Vector B_ = NewVector(2);
	InitMatrix(A,2,2);
	InitVector(B,2);
	for(int i=0;i<num;i++){
		A[0][0] += t[i].x*t[i].x;
		A[0][1] += t[i].x;
		B[0] += t[i].x*t[i].y;
		B[1] += t[i].y;
	}
	A[1][0] = A[0][1];
	A[1][1] = (double)num;
	MatInv2(A,A_);
	MulMxVec(A_,2,2,B,2,B_);		// 直線の係数がB_に格納される。y = B_[0]x + B_[1]

	// 端点抽出
	// パラメトリック領域内のU-Vの範囲を決める4点から得られる4本の直線と、さっき求めた近似直線との交点4つを求める
	Coord P[4];
	P[0] = TrimNurbsSPlaneSub1(B_[0],B_[1],Trm->pts->U[0],Trm->pts->V[0],Trm->pts->U[1],Trm->pts->V[0]);
	P[1] = TrimNurbsSPlaneSub1(B_[0],B_[1],Trm->pts->U[1],Trm->pts->V[0],Trm->pts->U[1],Trm->pts->V[1]);
	P[2] = TrimNurbsSPlaneSub1(B_[0],B_[1],Trm->pts->U[1],Trm->pts->V[1],Trm->pts->U[0],Trm->pts->V[1]);
	P[3] = TrimNurbsSPlaneSub1(B_[0],B_[1],Trm->pts->U[0],Trm->pts->V[1],Trm->pts->U[0],Trm->pts->V[0]);
	// 得られた4つの交点Pから、U-V範囲内にある2点を抽出
	Coord Q[2];
	int j=0;
	for(int i=0;i<4;i++){
		if(P[i].x >= Trm->pts->U[0] && P[i].x <= Trm->pts->U[1] && P[i].y >= Trm->pts->V[0] && P[i].y <= Trm->pts->V[1]){
			Q[j] = SetCoord(P[i]);
			j++;
		}
	}
	// 得られた2つの点QからNURBS曲線(直線)を生成
	double T[4] = {0,0,1,1};
	double W[2] = {1,1};
	double V[2] = {0,1};
	int prop[4] = {0,0,1,0};
	Coord cp[2];
	InitCoord(&cp[0]);
	InitCoord(&cp[1]);
	//GenNurbsC(&body->CompC[i].DegeNurbs,2,2,4,T,W,cp,V,prop,1);	

	// すでに登録されている外周トリム曲線と、新たに導出した外周トリム用直線Qから、新たな閉曲線を形成
	

	FILE *fp = fopen("Debug.csv","w");
	for(int i=0;i<num;i++){
		Coord p = CalcNurbsSCoord(Trm->pts,t[i].x,t[i].y);			// 交点をパラメータ値から座標値へ変換
		DrawPoint(p,1,3,pcolor);			// 交点を描画
		fprintf(fp,"%lf,%lf\n",t[i].x,t[i].y);
	}
	fclose(fp);

	return KOD_TRUE;
}

// TrimNurbsSPlaneのサブ関数(2直線の交点をもとめる)
Coord NURBS_Func::TrimNurbsSPlaneSub1(double a,double b,double x0,double y0,double x1,double y1)
{
	Coord c;

	if(x1-x0 == 0.0){
		c.x = x0;
		c.y = a*x0+b;
		return c;
	}

	double p = (y1-y0)/(x1-x0);
	double q = (x1*y0-x0*y1)/(x1-x0);
	c.x = (q-b)/(a-p);
	c.y = (p*b-q*a)/(p-a);

	return c;
}

// 各通過点の曲線パラメータを算出(コード長の比から算出)
// 引数		*P:通過点列   PNum:通過点列の数    T_:曲線パラメータを格納
void NURBS_Func::GetCurveKnotParam1(Coord *P,int PNum,Vector T_)
{
	double d_sum=0;
	for(int i=1;i<PNum;i++){
		d_sum += CalcEuclid(SubCoord(P[i],P[i-1]));
	}
	T_[0] = 0;
	T_[PNum-1] = 1;
	for(int i=1;i<PNum-1;i++){
		double d = CalcEuclid(SubCoord(P[i],P[i-1]));
		T_[i] = T_[i-1] + d/d_sum;
	}
}

// 各通過点の曲線パラメータを算出(コード長の平方根の比から算出)
// 引数		*P:通過点列   PNum:通過点列の数    T_:曲線パラメータを格納
void NURBS_Func::GetCurveKnotParam2(Coord *P,int PNum,Vector T_)
{
	double d_sum=0;
	for(int i=1;i<PNum;i++){
		d_sum += sqrt(CalcEuclid(SubCoord(P[i],P[i-1])));
	}
	T_[0] = 0;
	T_[PNum-1] = 1;
	for(int i=1;i<PNum-1;i++){
		double d = sqrt(CalcEuclid(SubCoord(P[i],P[i-1])));
		T_[i] = T_[i-1] + d/d_sum;
	}
}

// 補間曲面用u,vパラメータを得る
void NURBS_Func::GetSurfaceKnotParam(Vector S,Vector T,Coord **P,int uNum,int vNum)
{
	// u方向の通過点上の曲線パラメータを得る
	Vector du_sum = NewVector(vNum);
	InitVector(du_sum,vNum);
	for(int i=0;i<vNum;i++){
		for(int j=1;j<uNum;j++){
			du_sum[i] += CalcEuclid(SubCoord(P[j][i],P[j-1][i]));
		}
	}
	S[0] = 0;
	for(int i=1;i<uNum-1;i++){
		S[i] = 0;
		for(int j=0;j<vNum;j++){
			double du = CalcEuclid(SubCoord(P[i][j],P[i-1][j]));
			S[i] += S[i-1] + du/du_sum[j];
		}
		S[i] /= (double)vNum;
	}
	S[uNum-1] = 1;
	
	// v方向の通過点上の曲線パラメータを得る
	Vector dv_sum = NewVector(uNum);
	InitVector(dv_sum,uNum);
	for(int i=0;i<uNum;i++){
		for(int j=1;j<vNum;j++){
			dv_sum[i] += CalcEuclid(SubCoord(P[i][j],P[i][j-1]));
		}
	}
	T[0] = 0;
	for(int i=1;i<vNum-1;i++){
		T[i] = 0;
		for(int j=0;j<uNum;j++){
			double dv = CalcEuclid(SubCoord(P[j][i],P[j][i-1]));
			T[i] += T[i-1] + dv/dv_sum[j];
		}
		T[i] /= (double)uNum;
	}
	T[vNum-1] = 1;

	FreeVector(du_sum);
	FreeVector(dv_sum);
}

// 曲線/曲面パラメータから等間隔なノットベクトルを算出
// 引数   K:コントロールポイントの数  M:階数   T:格納するノットベクトル列
void NURBS_Func::GetEqIntervalKont(int K,int M,Vector T)
{
	for(int i=0;i<M;i++)
		T[i] = 0;
	for(int i=M;i<K;i++)
		T[i] = ((double)i-(double)M+1)/((double)K-(double)M+1)*NORM_KNOT_VAL;
	for(int i=K;i<K+M;i++)
		T[i] = NORM_KNOT_VAL;
}

// 曲線/曲面パラメータから補間用ノットベクトルを算出
// 引数   T_:曲線パラメータ列  N:ノットベクトルの数  K:コントロールポイントの数  M:階数   T:格納するノットベクトル列
void NURBS_Func::GetInterpolatedKnot(Vector T_,int N,int K,int M,Vector T)
{
	for(int i=0;i<M;i++)
		T[i] = 0;
	for(int j=1;j<K-M+1;j++){
		double d=0;
		for(int i=j;i<j+M-1;i++){
			d += T_[i];
		}
		T[j+M-1] = d/((double)M-1);
	}
	for(int i=K;i<K+M;i++)
		T[i] = 1;
}

// 曲線/曲面パラメータから近似用ノットベクトルを算出
// 引数   T_:曲線パラメータ列  N:曲線パラメータの数  M:階数  K:コントロールポイントの数  T:格納するノットベクトル列
void NURBS_Func::GetApproximatedKnot(Vector T_,int N,int M,int K,Vector T)
{
	for(int i=0;i<M;i++)	T[i] = 0;
	for(int i=K;i<K+M;i++)	T[i] = 1;
	double d = (double)N/(double)(K-M+1);
	for(int j=1;j<K-M+1;j++){
		int i = (int)(j*d);
		double a = (double)j*d - (double)i;
		T[j+M-1] = (1-a)*T_[i-1] + a*T_[i];
		T[j+M-1] += 0.0001;					// 肝!  TとT_が同値になると、最小２乗法がうまくいかないので、便宜的に同値にならないようにしている。
	}
}

// 最小2乗法で近似コントロールポイントを求める
// 引数    *P:通過点列  T_:曲線パラメータ列  T:ノットベクトル  Pnum:曲線パラメータの数  Nnum:ノットベクトルの数  
//         M:階数  K:コントロールポイントの数   *Q:算出されたコントロールポイント列
void NURBS_Func::CalcApproximationCP_LSM(Coord *P,Vector T_,Vector T,int Pnum,int Nnum,int M,int K,Coord *Q)
{
	Matrix N = NewMatrix(Pnum-2,K-2);
	for(int i=0;i<Pnum-2;i++){
		for(int j=0;j<K-2;j++){
			N[i][j] =  CalcBSbasis(T_[i+1],T,Nnum,j+1,M);
		}
	}
	
	Coord *R = NewCoord1(K-2);
	for(int i=0;i<K-2;i++){
		InitCoord(&R[i]);
		for(int j=0;j<Pnum-2;j++){
			Coord NP0 = MulCoord(P[0],CalcBSbasis(T_[j+1],T,Nnum,0,M));
			Coord NPN = MulCoord(P[Pnum-1],CalcBSbasis(T_[j+1],T,Nnum,K-1,M));
			Coord R_ = SubCoord(P[j+1],AddCoord(NP0,NPN));
			R[i] = AddCoord(R[i],MulCoord(R_,N[j][i]));
		}
	}

	Matrix N_ = NewMatrix(K-2,K-2);			// (NTN)^-1
	Matrix NTN = NewMatrix(K-2,K-2);		// NT*N
	Matrix NT = NewMatrix(K-2,Pnum-2);		// Nの転置行列NT
	TranMx(N,Pnum-2,K-2,NT);				// calc NT
	MulMxMx(NT,K-2,Pnum-2,N,Pnum-2,K-2,NTN);// calc NTN

	Coord *Q_ = NewCoord1(K-2);
	Gauss(K-2,NTN,R,Q_);

	// コントロールポイント
	Q[0] = SetCoord(P[0]);
	Q[K-1] = SetCoord(P[Pnum-1]);
	for(int i=1;i<K-1;i++){
		Q[i] = SetCoord(Q_[i-1]);
	}

	FreeCoord1(Q_);
	FreeMatrix(N_,K-2);
	FreeMatrix(NTN,K-2);
	FreeMatrix(NT,K-2);
	FreeCoord1(R);
	FreeMatrix(N,Pnum-2);
}

// 点列数から生成するコントロールポイント数を算定する
// 勘です。
int NURBS_Func::SetApproximationCPnum(int PNum)
{
	if(PNum < 5)		// 勘
		return PNum;
	else if(PNum < 10)	// 勘
		return PNum-1;
	else 
		return PNum/2;	// 勘
}