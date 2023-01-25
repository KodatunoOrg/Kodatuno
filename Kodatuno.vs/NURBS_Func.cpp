#include "stdafx.h"
#include "BODY.h"
#include "NURBS_Func.h"
#include "Quaternion.h"

#if defined(_DEBUG) && defined(_MSC_VER)
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////////////////////
// Function: CalcBSbasis
// Bスプライン基底関数を計算し、計算結果を返す
static	double CalcBSbasis(double,double [],int,int,int);			

// Function: CalcDiffBSbasis
// Bスプライン基底関数の1階微分係数を求める
static	double CalcDiffBSbasis(double,double [],int,int,int);		

// Function: CalcDiffBSbasisN
// Bスプライン基底関数のN階微分係数を求める
static	double CalcDiffBSbasisN(double,double [],int,int,int,int);	

// Function: GetBSplCoef3
// 3次のBスプライン曲線の各係数を求める　(at^3 + bt^2 + ct + dの係数a,b,c,dを返す)
static	int GetBSplCoef3(int,int,int,double *,double **);			

// Function: GetBSplCoef2
// 2次のBスプライン曲線の各係数を求める　(at^2 + bt + cの係数a,b,cを返す)
static	int GetBSplCoef2(int,int,int,double *,double **);			

// Function: GetBSplCoef1
// 1次のBスプライン曲線の各係数を求める　(at + bの係数a,bを返す)
static	int GetBSplCoef1(int,int,int,double *,double **);	

// Function: TrimNurbsSPlaneSub1
// (private)TrimNurbsSPlaneのサブ関数(2直線の交点をもとめる)
static	Coord TrimNurbsSPlaneSub1(double,double,double,double,double,double); 

// Function: GetIntersecEquation
// (private)NURBS曲線と平面の交線導出用3次方程式を得る
static	void GetIntersecEquation(int,Coord *,double *,Coord,Coord,double *);		

// Function: CalcEquation
// (private)3次方程式までを判別して解く
static	int CalcEquation(double *,double *,int);					

// Function: GetNurbsSCoef
// (private)NURBS曲面においてuまたはvを固定した場合に得られるNURBS曲線C(u) or C(v)の分母分子の係数を求める
static	void GetNurbsSCoef(int,double **,double *,Coord *,int,Coord *,double *);	

// Function: GetMinDistance
// (private)最小距離を持つ座標値を返す
static	Coord GetMinDistance(Coord,Coord *,int);						

// Function: CheckClossedPoints
// (private)指定した点が他の2点を対角とする立方体の中に存在するかを調べる
static	int CheckClossedPoints(Coord,Coord,Coord);				

// Function: ChangeKnotVecRange
// ノットベクトルのパラメータ定義域を変更する
static	void ChangeKnotVecRange(Vector,int,int,int,double,double);	

// Function: CalcApproximationCP_LSM
// (private)最小2乗法で近似コントロールポイントを求める
static	void CalcApproximationCP_LSM(Coord *,Vector,Vector,int,int,int,int,Coord *);	

// Function: GetEqIntervalKont
// 曲線/曲面パラメータから等間隔なノットベクトルを算出
static	void GetEqIntervalKont(int,int,Vector);						

// Function: SetApproximationCPnum
// (private)点列数から生成するコントロールポイント数を算定する
static	int SetApproximationCPnum(int);									

// Function: GetApproximatedKnot
// (private)曲線/曲面パラメータから近似用ノットベクトルを算出
static	void GetApproximatedKnot(Vector,int,int,int,Vector);			

// Function: GetInterpolatedKnot
// (private)曲線/曲面パラメータから補間用ノットベクトルを算出
static	void GetInterpolatedKnot(Vector,int,int,int,Vector);			

// Function: GetSurfaceKnotParam
// (private)各通過点の曲面パラメータを算出
static	void GetSurfaceKnotParam(Vector,Vector,Coord **,int,int);		

// Function: GetCurveKnotParam1
// (private)各通過点の曲線パラメータを算出(コード長の比から算出)
static	void GetCurveKnotParam1(Coord *,int,Vector);					

// Function: GetCurveKnotParam2
// (private)各通過点の曲線パラメータを算出(コード長の平方根の比から算出)
static	void GetCurveKnotParam2(Coord *,int,Vector);					

//////////////////////////////////////////////////////////////////////////////////////
// Function: New_NurbsC
// Nurbs曲線のメモリー確保
//
// Parameters: 
// *nurb - メモリー確保するNurbs曲線へのポインタ
// K - コントロールポイントの数
// N - ノットベクトルの数
//
// Return:
// 成功：KOD_TRUE, 失敗：KOD_ERR
int NURBS_Func::New_NurbsC(NURBSC *nurb,int K, int N)
{
//	if((nurb->T = (double *)malloc(sizeof(double)*N)) == NULL)
	nurb->T = new double[N];
//	nurb->T = boost::shared_array<double>( new double[N] );
	if ( !nurb->T )
		return KOD_ERR;
//	if((nurb->W = (double *)malloc(sizeof(double)*K)) == NULL){
	nurb->W = new double[K];
//	nurb->W = boost::shared_array<double>( new double[K] );
	if ( !nurb->W ) {
//		free(nurb->T);
		delete[]	nurb->T;
		return KOD_ERR;
	}
//	if((nurb->cp = (Coord *)malloc(sizeof(Coord)*K)) == NULL){
	nurb->cp = new Coord[K];
//	nurb->cp = boost::shared_array<Coord>( new Coord[K] );
	if ( !nurb->cp ) {
//		free(nurb->T);
//		free(nurb->W);
		delete[]	nurb->T;
		delete[]	nurb->W;
		return KOD_ERR;
	}

	return KOD_TRUE;
}

// Function: Free_NurbsC_1DArray
// NURBS曲線配列のメモリー解放
//
// Parameters: 
// *a - メモリーを解放するNurbs曲線へのポインタ
// num - メモリーを解放するNURBS曲線の数
void NURBS_Func::Free_NurbsC_1DArray(NURBSC *a,int num)
{
	for(int i=0;i<num;i++)
		Free_NurbsC(&a[i]);
}

// Function: Free_NurbsC
// 1本のNURBS曲線のメモリー解放
//
// Parameters: 
// *a - メモリーを解放するNurbs曲線へのポインタ
void NURBS_Func::Free_NurbsC(NURBSC *a)
{
//	free(a->T);
//	free(a->W);
//	free(a->cp);
//	delete	a->T;	a->T = NULL;
//	delete	a->W;	a->W = NULL;
//	delete	a->cp;	a->cp = NULL;
// /*
	if ( a->T ) {
		delete[]	a->T;
		a->T = NULL;
	}
	if ( a->W ) {
		delete[]	a->W;
		a->W = NULL;
	}
	if ( a->cp ) {
		delete[]	a->cp;
		a->cp = NULL;
	}
// */
//	a->T.reset();
//	a->W.reset();
//	a->cp.reset();
}

// Function: New_NurbsS
// NURBS曲面のメモリー確保
//
// Parameters: 
// *nurb - メモリー確保するNurbs曲面へのポインタ
// K[2] - u,vコントロールポイントの数
// N[2] - u,vノットベクトルの数
//
// Return:
// 成功：KOD_TRUE, 失敗：KOD_ERR
int NURBS_Func::New_NurbsS(NURBSS *nurb,int K[2],int N[2])
{
	int KOD_ERRflag = 0;

//	if((nurb->S = (double *)malloc(sizeof(double)*N[0])) == NULL)
	nurb->S = new double[N[0]];
	if ( !nurb->S )
		goto EXIT;
	KOD_ERRflag++;	// 1
//	if((nurb->T = (double *)malloc(sizeof(double)*N[1])) == NULL)
	nurb->T = new double[N[1]];
	if ( !nurb->T )
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
//		free(nurb->T);
		delete[]	nurb->T;	nurb->T = NULL;
		KOD_ERRflag--;
	}
	if(KOD_ERRflag == 1){
//		free(nurb->S);
		delete[]	nurb->S;	nurb->S = NULL;
	}

	return KOD_ERR;
}

// Function: Free_NurbsS_1DArray
// NURBS曲面配列のメモリー解放
// 
// Parameters:
// *a - メモリーを解放するNurbs曲面へのポインタ
// num - メモリーを解放するNURBS曲面の数
void NURBS_Func::Free_NurbsS_1DArray(NURBSS *a,int num)
{
	for(int i=0;i<num;i++)
		Free_NurbsS(&a[i]);
}


// Function: Free_NurbsS
// 1つのNURBS曲面のメモリー解放
//
// Parameters: 
// *a - メモリーを解放するNurbs曲面へのポインタ
void NURBS_Func::Free_NurbsS(NURBSS *a)
{
//	free(a->S);
//	free(a->T);
	if ( a->S ) {
		delete[]	a->S;
		a->S = NULL;
	}
	if ( a->T ) {
		delete[]	a->T;
		a->T = NULL;
	}
	FreeMatrix(a->W,a->K[0]);
	FreeCoord2(a->cp,a->K[0]);
}

// Function: New_TrmS
// トリム面のメモリー確保
//
// Parameters: 
// *trms - メモリー確保するトリム面へのポインタ
// num - メモリー確保するトリム面の数
//
// return:
// 成功：KOD_TRUE, 失敗：KOD_ERR
int NURBS_Func::New_TrmS(TRMS *trms,int num)
{
//	if((trms->pTI = (CONPS **)malloc(sizeof(CONPS *)*num)) == NULL){
	trms->pTI = new CONPS*[num];
	if ( !trms->pTI ) {
		return KOD_ERR;
	}

	return KOD_TRUE;
}

// Function: Free_TrmS_1DArray
// トリム面配列のメモリー解放
//
// Parameters: 
// *a - メモリーを解放するトリム面配列へのポインタ
// num - メモリーを解放するトリム面の数
void NURBS_Func::Free_TrmS_1DArray(TRMS *a,int num)
{
	for(int i=0;i<num;i++)
		Free_TrmS(&a[i]);
}

// Function: Free_TrmS
// トリム面のメモリー解放
//
// Parameters: 
// *a - メモリーを解放するトリム面へのポインタ
void NURBS_Func::Free_TrmS(TRMS *a)
{
//	free(a->pTI);
	if ( a->pTI ) {
		delete[]	a->pTI;
		a->pTI = NULL;
	}
}

// Function: New_CompC
// 複合曲線のメモリー確保
//
// Parameters: 
// *compc - メモリー確保する複合曲線へのポインタ
// num - メモリー確保する複合曲線の数
//
// return:
// 成功：KOD_TRUE, 失敗：KOD_ERR
int NURBS_Func::New_CompC(COMPC *compc,int num)
{
//	if((compc->DEType = (int *)malloc(sizeof(int)*num)) == NULL){
	compc->DEType = new int[num];
	if ( !compc->DEType ) {
		return KOD_ERR;
	}
	
//	if((compc->pDE = (COMPELEM **)malloc(sizeof(COMPELEM *)*num)) == NULL){
	compc->pDE = new COMPELEM[num];
	if ( !compc->pDE ) {
//		free(compc->DEType);
		delete[]	compc->DEType;
		return KOD_ERR;
	}

	compc->N = num;

	return KOD_TRUE;
}

// Function: Free_CompC_1DArray
// 複合曲線配列のメモリー解放
//
// Parameters:
// *a - メモリーを解放する複合曲線配列へのポインタ
// num - メモリーを解放する複合曲線の数
void NURBS_Func::Free_CompC_1DArray(COMPC *a,int num)
{
	for(int i=0;i<num;i++)
		Free_CompC(&a[i]);
}

// Function: Free_CompC
// 複合曲線のメモリー解放
//
// Parameters:
// *a - メモリーを解放する複合曲線へのポインタ
void NURBS_Func::Free_CompC(COMPC *a)
{
//	free(a->DEType);
//	free(a->pDE);
	if ( a->DEType ) {
		delete[]	a->DEType;
		a->DEType = NULL;
	}
	if ( a->pDE ) {
		delete[]	a->pDE;
		a->pDE = NULL;
	}
}

// Function: GenNurbsC
// 1つのNurbs曲線を生成する
//
// Parameters:
// *Nurbs - 生成するNURBS曲線へのポインタ
// K - コントロールポイントの数
// M - 階数
// N - ノットベクトルの数
// T[] - ノットベクトル列
// W[] - ウェイト列
// cp[] - コントロールポイント列
// V[2] - ノットベクトルの範囲
// prop[4] - プロパティ(BODY.h参照)
// euflag - ディレクトリ部 Entity Use Flag の値(0:幾何要素 5:2Dパラメトリック要素)
// 
// return:
// KOD_TRUE
int NURBS_Func::GenNurbsC(NURBSC *Nurbs,int K,int M,int N,double T[],double W[],Coord cp[],double V[],int prop[],int euflag)
{
	int i;

	Nurbs->K = K;
	Nurbs->M = M;
	Nurbs->N = N;
	Nurbs->V[0] = V[0];
	Nurbs->V[1] = V[1];
//	Nurbs->T = (double *)malloc(sizeof(double)*Nurbs->N);
//	Nurbs->W = (double *)malloc(sizeof(double)*Nurbs->K);
//	Nurbs->cp = (Coord *)malloc(sizeof(Coord)*Nurbs->K);
	Nurbs->T = new double[Nurbs->N];
	Nurbs->W = new double[Nurbs->K];
	Nurbs->cp = new Coord[Nurbs->K];
//	Nurbs->T = boost::shared_array<double>( new double[Nurbs->N] );
//	Nurbs->W = boost::shared_array<double>( new double[Nurbs->K] );
//	Nurbs->cp = boost::shared_array<Coord>( new Coord[Nurbs->K] );
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

// Function: GenNurbsC
// 1つのNurbs曲線を生成する(NURBS曲線のコピー)(オーバーロード)
//
// Parameters:
// *Nurbs - 新たに生成するNURBS曲線
// nurb - 代入元
// 
// return:
// KOD_TRUE
//int NURBS_Func::GenNurbsC(NURBSC *Nurbs,NURBSC nurb)
int NURBS_Func::GenNurbsC(NURBSC *Nurbs,NURBSC* nurb)
{
	int i;

	Nurbs->K = nurb->K;
	Nurbs->M = nurb->M;
	Nurbs->N = nurb->N;
	Nurbs->V[0] = nurb->V[0];
	Nurbs->V[1] = nurb->V[1];
	
//	Nurbs->T = (double *)malloc(sizeof(double)*Nurbs->N);
//	Nurbs->W = (double *)malloc(sizeof(double)*Nurbs->K);
//	Nurbs->cp = (Coord *)malloc(sizeof(Coord)*Nurbs->K);
	Nurbs->T = new double[Nurbs->N];
	Nurbs->W = new double[Nurbs->K];
	Nurbs->cp = new Coord[Nurbs->K];
//	Nurbs->T = boost::shared_array<double>( new double[Nurbs->N] );
//	Nurbs->W = boost::shared_array<double>( new double[Nurbs->K] );
//	Nurbs->cp = boost::shared_array<Coord>( new Coord[Nurbs->K] );
	for(i=0;i<nurb->N;i++){
		Nurbs->T[i] = nurb->T[i];
	}
	for(i=0;i<nurb->K;i++){
		Nurbs->W[i] = nurb->W[i];
		Nurbs->cp[i] = SetCoord(nurb->cp[i]);
	}

	return KOD_TRUE;
}

// Function: DelNurbsC
// GenNurbsC()によって生成されたNURBS曲線を削除する
// 
// Parameters:
// *Nurbs - 新たに生成するNURBS曲線へのポインタ
void NURBS_Func::DelNurbsC(NURBSC *Nurbs)
{
	NURBS_Func hbody;
	hbody.Free_NurbsC(Nurbs);
}

// Function: GenNurbsS
// 1つのNURBS曲面を生成する
//
// Parameters:
// Mu,Mv - 階数
// Ku,Kv - コントロールポイントの数  
// *S,*T - u,v方向ノットベクトル列
// **W - ウエイト
// **Cp - コントロールポイント  
// U_s,U_e,V_s,V_e - u方向ノットベクトルの開始値,終了値
// 
// return:
// 成功：KOD_TRUE, 失敗：KOD_ERR
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
        GuiIF.SetMessage("NURBS_Func KOD_ERROR:fail to allocate memory");
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

// Function: GenNurbsS
// 1つのNurbs曲面を生成する(NURBS曲面のコピー)
//
// Parameters:
// *Nurbs - 新たに生成するNURBS曲面へのポインタ
// nurb - 代入元
// 
// return:
// 成功：KOD_TRUE, 失敗：KOD_ERR
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
        GuiIF.SetMessage("NURBS_Func KOD_ERROR:fail to allocate memory");
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

// Function: GenRotNurbsS
// NurbsCを原点を通るAxis回りにdegだけ回転させた回転サーフェスNurbsSを生成する
//
// Parameter:
// *NurbsS - 生成される回転サーフェス(NURBS曲面)へのポインタ
// NurbsC - 基線となるNURBS曲線
// Axis - 回転軸ベクトル
// deg - 回転角度（deg)
//
// Return:
// 成功：KOD_TRUE, 失敗：KOD_ERR
//int NURBS_Func::GenRotNurbsS(NURBSS *NurbsS,NURBSC NurbsC,Coord Axis,double deg)
int NURBS_Func::GenRotNurbsS(NURBSS *NurbsS,const NURBSC& NurbsC,Coord Axis,double deg)
{
    Axis = NormalizeVec(Axis);		// 正規化

    // 回転角度によって，いくつのセグメントで円弧を生成するか判断する
    // 回転角度が180度未満の場合，1セグメントで円弧を表現する
    if(fabs(deg) < 180 ){
        double S[6] = {0,0,0,1,1,1};	// u方向ノットベクトル
        double **W;	// ウエイト
        Coord **Cp;	// コントロールポイント
        double rad = DegToRad(deg);
        if((W = NewMatrix(3,NurbsC.K)) == NULL){
            GuiIF.SetMessage("NURBS_Func KOD_ERROR:fail to allocate memory");
            return KOD_ERR;
        }
        if((Cp = NewCoord2(3,NurbsC.K)) == NULL){
            GuiIF.SetMessage("NURBS_Func KOD_ERROR:fail to allocate memory");
            FreeMatrix(W,3);
            return KOD_ERR;
        }
        for(int i=0;i<3;i++){
            for(int j=0;j<NurbsC.K;j++){
                Coord Q_ = CalcRotVec(NurbsC.cp[j],Axis,(double)i*rad/2);	// 元々のNURBS曲線上のコントロールポイントをAxis周りに0,deg/2,deg度回転
                Coord P = CalcNormalLine(NurbsC.cp[j],SetCoord(0,0,0),Axis);	// Axis上の回転中心の座標
                Coord PQ_ = SubCoord(Q_,P);	// PQ_ベクトルを生成
                if(i%2 == 0){		// i=0,2のとき
                    W[i][j] = NurbsC.W[j];
                    Cp[i][j] = SetCoord(Q_);
                }
                else{
                    W[i][j] = NurbsC.W[j]*cos(rad/2);
                    Cp[i][j] = AddCoord(MulCoord(PQ_,1/cos(rad/2)),P);
                }
            }
        }
		GenNurbsS(NurbsS,3,NurbsC.M,3,NurbsC.K,S,NurbsC.T,W,Cp,0,1,0,1);		// NURBS曲面生成
//		GenNurbsS(NurbsS,3,NurbsC.M,3,NurbsC.K,S,NurbsC.T.get(),W,Cp,0,1,0,1);		// NURBS曲面生成

        FreeMatrix(W,3);
        FreeCoord2(Cp,3);
    }

    // 回転角度が270未満の場合，2セグメントで円弧を表現する
    else if(fabs(deg) < 270){
        double S[8] = {0,0,0,0.5,0.5,1,1,1};
        double **W;	// ウエイト
        Coord **Cp;	// コントロールポイント
        double rad = DegToRad(deg);
        if((W = NewMatrix(5,NurbsC.K)) == NULL){
            GuiIF.SetMessage("NURBS_Func KOD_ERROR:fail to allocate memory");
            return KOD_ERR;
        }
        if((Cp = NewCoord2(5,NurbsC.K)) == NULL){
            GuiIF.SetMessage("NURBS_Func KOD_ERROR:fail to allocate memory");
            FreeMatrix(W,5);
            return KOD_ERR;
        }
        for(int i=0;i<5;i++){
            for(int j=0;j<NurbsC.K;j++){
                Coord Q_ = CalcRotVec(NurbsC.cp[j],Axis,(double)i*rad/4);	// 元々のNURBS曲線上のコントロールポイントをAxis周りに0,deg/2,deg度回転
                Coord P = CalcNormalLine(NurbsC.cp[j],SetCoord(0,0,0),Axis);	// Axis上の回転中心の座標
                Coord PQ_ = SubCoord(Q_,P);	// PQ_ベクトルを生成
                if(i%2 ==  1){	// i=1,3のとき
                    W[i][j] = NurbsC.W[j]*cos(rad/4);
                    Cp[i][j] = AddCoord(MulCoord(PQ_,1/cos(rad/4)),P);
                }
                else{		// i=0,2,4のとき
                    W[i][j] = NurbsC.W[j];
                    Cp[i][j] = SetCoord(Q_);
                }
            }
        }
		GenNurbsS(NurbsS,3,NurbsC.M,5,NurbsC.K,S,NurbsC.T,W,Cp,0,1,0,1);		// NURBS曲面生成
//		GenNurbsS(NurbsS,3,NurbsC.M,5,NurbsC.K,S,NurbsC.T.get(),W,Cp,0,1,0,1);		// NURBS曲面生成

        FreeMatrix(W,5);
        FreeCoord2(Cp,5);
    }

    // 回転角度が360度未満の場合，3セグメントで円弧を表現する
    else if(fabs(deg) < 360){
        double S[10] = {0,0,0,0.33,0.33,0.66,0.66,1,1,1};
        double **W;	// ウエイト
        Coord **Cp;	// コントロールポイント
        double rad = DegToRad(deg);
        if((W = NewMatrix(7,NurbsC.K)) == NULL){
            GuiIF.SetMessage("NURBS_Func KOD_ERROR:fail to allocate memory");
            return KOD_ERR;
        }
        if((Cp = NewCoord2(7,NurbsC.K)) == NULL){
            GuiIF.SetMessage("NURBS_Func KOD_ERROR:fail to allocate memory");
            FreeMatrix(W,7);
            return KOD_ERR;
        }
        for(int i=0;i<7;i++){
            for(int j=0;j<NurbsC.K;j++){
                Coord Q_ = CalcRotVec(NurbsC.cp[j],Axis,(double)i*rad/6);	// 元々のNURBS曲線上のコントロールポイントをAxis周りに0,deg/2,deg度回転
                Coord P = CalcNormalLine(NurbsC.cp[j],SetCoord(0,0,0),Axis);	// Axis上の回転中心の座標
                Coord PQ_ = SubCoord(Q_,P);	// PQ_ベクトルを生成
                if(i%2 ==  0){	// i=0,2,4,6のとき
                    W[i][j] = NurbsC.W[j];
                    Cp[i][j] = SetCoord(Q_);
                }
                else{		// i=1,3,5のとき
                    W[i][j] = NurbsC.W[j]*cos(rad/6);
                    Cp[i][j] = AddCoord(MulCoord(PQ_,1/cos(rad/6)),P);
                }
            }
        }
		GenNurbsS(NurbsS,3,NurbsC.M,7,NurbsC.K,S,NurbsC.T,W,Cp,0,1,0,1);		// NURBS曲面生成
//		GenNurbsS(NurbsS,3,NurbsC.M,7,NurbsC.K,S,NurbsC.T.get(),W,Cp,0,1,0,1);		// NURBS曲面生成
#ifdef _DEBUG
		NurbsS->DebugForNurbsS();
#endif
        FreeMatrix(W,7);
        FreeCoord2(Cp,7);
    }
    // 360度以上
    else{
        // NurbsSを生成
        double S[12] = {0,0,0,0.25,0.25,0.5,0.5,0.75,0.75,1,1,1};		// u方向ノットベクトル
        double **W;			// ウエイト
        Coord  **Cp;		// コントロールポイント
        if((W = NewMatrix(9,NurbsC.K)) == NULL){
            GuiIF.SetMessage("NURBS_Func KOD_ERROR:fail to allocate memory");
            return KOD_ERR;
        }
        if((Cp = NewCoord2(9,NurbsC.K)) == NULL){
            GuiIF.SetMessage("NURBS_Func KOD_ERROR:fail to allocate memory");
            FreeMatrix(W,9);
            return KOD_ERR;
        }
        for(int i=0;i<9;i++){		// u方向
            for(int j=0;j<NurbsC.K;j++){		// v方向
                Coord Q_ = CalcRotVec(NurbsC.cp[j],Axis,(double)i*PAI/4);		// 元々のNURBS曲線上のコントロールポイントをAxis周りに45度回転
                Coord P = CalcNormalLine(NurbsC.cp[j],SetCoord(0,0,0),Axis);	// Axis上の回転中心の座標
                Coord PQ_ = SubCoord(Q_,P);										// PQ_ベクトルを生成
                if(i%2 == 0){													// i=0,2,4,6のとき
                    W[i][j] = NurbsC.W[j];										// ウエイト
                    Cp[i][j] = SetCoord(Q_);									// Q_がそのままコントロールポイントになる
                }
                else{															// i=1,3,5,7のとき
                    W[i][j] = NurbsC.W[j]*cos(PAI/4);							// ウエイト計算
                    Cp[i][j] = AddCoord(MulCoord(PQ_,1/cos(PAI/4)),P);			// コントロールポイント計算
                }
            }
        }

		GenNurbsS(NurbsS,3,NurbsC.M,9,NurbsC.K,S,NurbsC.T,W,Cp,0,1,0,1);		// NURBS曲面生成
//		GenNurbsS(NurbsS,3,NurbsC.M,9,NurbsC.K,S,NurbsC.T.get(),W,Cp,0,1,0,1);		// NURBS曲面生成

        FreeMatrix(W,9);
        FreeCoord2(Cp,9);
    }
    return KOD_TRUE;
}

// Function: GenSweepNurbsS
// 1つのNURBS曲線からある軸方向にある距離だけスイープさせたスイープサーフェスを生成する
//
// Parameters:
// *NurbsS - 生成されるスイープサーフェス(NURBS曲面)へのポインタ
// NurbsC - 基線となるNURBS曲線
// Axis - スイープ方向ベクトル
// Len - スイープ距離
//
// Return:
// 成功：KOD_TRUE, 失敗：KOD_ERR
//int NURBS_Func::GenSweepNurbsS(NURBSS *NurbsS,NURBSC NurbsC,Coord Axis,double Len)
int NURBS_Func::GenSweepNurbsS(NURBSS *NurbsS,const NURBSC& NurbsC,Coord Axis,double Len)
{
	Axis = NormalizeVec(Axis);		// 正規化

	// NurbsSを生成
	double T[4] = {0,0,1,1};		// v方向ノットベクトル
	double **W;			// ウエイト
	Coord  **Cp;		// コントロールポイント
	if((W = NewMatrix(NurbsC.K,2)) == NULL){
        GuiIF.SetMessage("NURBS_Func KOD_ERROR:fail to allocate memory");
		return KOD_ERR;
	}
	if((Cp = NewCoord2(NurbsC.K,2)) == NULL){
        GuiIF.SetMessage("NURBS_Func KOD_ERROR:fail to allocate memory");
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
//	GenNurbsS(NurbsS,NurbsC.M,2,NurbsC.K,2,NurbsC.T.get(),T,W,Cp,0,1,NurbsC.V[0],NurbsC.V[1]);	// NURBS曲面生成

	FreeMatrix(W,2);
	FreeCoord2(Cp,2);

	return KOD_TRUE;
}

// Function: GenIsoparamCurveU
// NURBS曲面上のu方向パラメータ値を固定したときのアイソパラメトリックNURBS曲線を生成
//
// Parameters:
// *P - アイソパラメトリック曲線生成元のNURBS曲面   
// u - u方向の固定パラメータ   
// *C - 生成されたアイソパラメトリック曲線
//
// Return:
// 成功：KOD_TRUE, 失敗：KOD_ERR（引数uが*Pのuパラメータ範囲外）
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

// Function: GenIsoparamCurveV
// NURBS曲面上のv方向パラメータ値を固定したときのアイソパラメトリックNURBS曲線を生成
//
// Parameters:
// *S - アイソパラメトリック曲線生成元のNURBS曲面   
// v - v方向の固定パラメータ   
// *C - 生成されたアイソパラメトリック曲線
//
// Return:
// 成功：KOD_TRUE, 失敗：KOD_ERR（引数vが*Pのuパラメータ範囲外）
int NURBS_Func::GenIsoparamCurveV(NURBSS *P,double v,NURBSC *C)
{
	if(v < P->V[0] || v > P->V[1])	return KOD_ERR;

	double V[2] = {P->U[0],P->U[1]};	// ノットベクトルの範囲
	int prop[4] = {0,0,1,0};		// パラメータ

	Coord *Q = NewCoord1(P->K[0]);	// コントロールポイント
	double *W = NewVector(P->K[0]);	// ウェイト

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

// Function: DelNurbsS
// GenNurbsS()によって生成されたNURBS曲面を削除する
// 
// Parameters:
// *Nurbs - 削除するNURBS曲面へのポインタ
void NURBS_Func::DelNurbsS(NURBSS *Nurbs)
{
	NURBS_Func hbody;
	hbody.Free_NurbsS(Nurbs);
}

// Function: GenTrimdNurbsS
// トリム面を有するNURBS曲面をコピーする
//
// Parameters:
// *TNurbs - 生成されるトリム面へのポインタ
// tnurb - コピー元のトリム面
//
// Return:
// KOD_TRUE
int NURBS_Func::GenTrimdNurbsS(TRIMD_NURBSS *TNurbs,TRIMD_NURBSS  tnurb)
{
	NURBSS *nurbsS;
	NURBSC *nurbsC;
	CONPS *conps_o,*conps_i;
	COMPC *compc_o,*compc_i;
	int curve_num=0;

//	nurbsS = (NURBSS *)malloc(sizeof(NURBSS));		// NURBS曲面のメモリー確保
//	conps_o = (CONPS *)malloc(sizeof(CONPS));		// 外側トリムを構成する面上線のメモリー確保
//	compc_o = (COMPC *)malloc(sizeof(COMPC));		// 外側トリムを構成する複合曲線のメモリー確保
	nurbsS = new NURBSS[1];		// NURBS曲面のメモリー確保
	conps_o = new CONPS[1];		// 外側トリムを構成する面上線のメモリー確保
	compc_o = new COMPC[1];		// 外側トリムを構成する複合曲線のメモリー確保

	// トリム面を構成するNURBS曲線の総数をカウント
	for(int i=0;i<tnurb.n2;i++){
//		for(int j=0;j<tnurb.pTI[i]->pB->CompC.N;j++){
		for(int j=0;j<tnurb.pTI[i]->pB.CompC->N;j++){
			curve_num++;
		}
	}
//	curve_num += tnurb.pTO->pB->CompC.N;
	curve_num += tnurb.pTO->pB.CompC->N;

//	nurbsC = (NURBSC *)malloc(sizeof(NURBSC)*curve_num);	// トリム面を構成するNURBS曲線の数だけNURBS曲線のメモリーを確保
	nurbsC = new NURBSC[curve_num];	// トリム面を構成するNURBS曲線の数だけNURBS曲線のメモリーを確保

	GenNurbsS(nurbsS,*tnurb.pts);							// 新たなNURBS曲面を1つ得る
	TNurbs->pts = nurbsS;									// NURBS曲面をトリム面に関連付ける

	New_TrmS(TNurbs,tnurb.n2);						// トリム面のメモリー確保

//	conps_i = (CONPS *)malloc(sizeof(CONPS)*tnurb.n2);		// 内側を構成する面上線のメモリー確保
//	compc_i = (COMPC *)malloc(sizeof(COMPC)*tnurb.n2);		// 内側を構成する複合曲線のメモリー確保
	conps_i = new CONPS[tnurb.n2];		// 内側を構成する面上線のメモリー確保
	compc_i = new COMPC[tnurb.n2];		// 内側を構成する複合曲線のメモリー確保

	// NURBS曲線をトリム部分を構成するNURBS曲線に関連付ける
	// 外周トリム
	TNurbs->pTO = conps_o;
//	New_CompC(compc_o,tnurb.pTO->pB->CompC.N);
	New_CompC(compc_o,tnurb.pTO->pB.CompC->N);
//	for(int i=0;i<tnurb.pTO->pB->CompC.N;i++){
//		GenNurbsC(&nurbsC[i],tnurb.pTO->pB->CompC.pDE[i]->NurbsC);
//		compc_o->pDE[i] = (COMPELEM *)(&nurbsC[i]);
//		compc_o->DEType[i] = tnurb.pTO->pB->CompC.DEType[i];
	for(int i=0;i<tnurb.pTO->pB.CompC->N;i++){
		GenNurbsC(&nurbsC[i],tnurb.pTO->pB.CompC->pDE[i].NurbsC);
		compc_o->pDE[i].NurbsC = &(nurbsC[i]);
		compc_o->DEType[i] = tnurb.pTO->pB.CompC->DEType[i];
	}
//	TNurbs->pTO->pB = (CURVE *)compc_o;
	TNurbs->pTO->pB.CompC = compc_o;
	TNurbs->pTO->BType = tnurb.pTO->BType;
//	TNurbs->pTO->pB->CompC.DegeFlag = tnurb.pTO->pB->CompC.DegeFlag;
//	TNurbs->pTO->pB->CompC.DegeNurbs = tnurb.pTO->pB->CompC.DegeNurbs;
	TNurbs->pTO->pB.CompC->DegeFlag = tnurb.pTO->pB.CompC->DegeFlag;
	TNurbs->pTO->pB.CompC->DegeNurbs = tnurb.pTO->pB.CompC->DegeNurbs;

	// 内周トリム
	curve_num = 0;
	for(int i=0;i<tnurb.n2;i++){
		TNurbs->pTI[i] = &(conps_i[i]);
//		New_CompC(&compc_i[i],tnurb.pTI[i]->pB->CompC.N);
		New_CompC(&compc_i[i],tnurb.pTI[i]->pB.CompC->N);
//		for(int j=0;j<tnurb.pTI[i]->pB->CompC.N;j++){
//			GenNurbsC(&nurbsC[tnurb.pTO->pB->CompC.N+curve_num],tnurb.pTI[i]->pB->CompC.pDE[j]->NurbsC);
//			compc_i[i].pDE[j] = (COMPELEM *)(&nurbsC[tnurb.pTO->pB->CompC.N+curve_num]);
//			compc_i[i].DEType[j] = tnurb.pTI[i]->pB->CompC.DEType[j];
		for(int j=0;j<tnurb.pTI[i]->pB.CompC->N;j++){
			GenNurbsC(&nurbsC[tnurb.pTO->pB.CompC->N+curve_num],tnurb.pTI[i]->pB.CompC->pDE[j].NurbsC);
			compc_i[i].pDE[j].NurbsC = &(nurbsC[tnurb.pTO->pB.CompC->N+curve_num]);
			compc_i[i].DEType[j] = tnurb.pTI[i]->pB.CompC->DEType[j];
			curve_num++;
		}
//		TNurbs->pTI[i]->pB = (CURVE *)(&(compc_i[i]));
		TNurbs->pTI[i]->pB.CompC = &(compc_i[i]);
		TNurbs->pTI[i]->BType = tnurb.pTI[i]->BType;
//		TNurbs->pTI[i]->pB->CompC.DegeFlag = tnurb.pTI[i]->pB->CompC.DegeFlag;
//		TNurbs->pTI[i]->pB->CompC.DegeNurbs = tnurb.pTI[i]->pB->CompC.DegeNurbs;
		TNurbs->pTI[i]->pB.CompC->DegeFlag = tnurb.pTI[i]->pB.CompC->DegeFlag;
		TNurbs->pTI[i]->pB.CompC->DegeNurbs = tnurb.pTI[i]->pB.CompC->DegeNurbs;
	}

	TNurbs->n1 = tnurb.n1;
	TNurbs->n2 = tnurb.n2;

	return KOD_TRUE;
}

// Function: DelTrimdNurbsS
// GenTrimdNurbsS()によって生成されたトリム面を削除する
//
// Parameters:
// *TNurbs - 削除するトリム面へのポインタ
//
// Return:
// KOD_TRUE
int NURBS_Func::DelTrimdNurbsS(TRIMD_NURBSS *TNurbs)
{
	NURBS_Func hbody;
	int curve_num = 0;

	// トリム面を構成する全てのNURBS曲線の本数を調べる
	for(int i=0;i<TNurbs->n2;i++){
//		for(int j=0;j<TNurbs->pTI[i]->pB->CompC.N;j++){
		for(int j=0;j<TNurbs->pTI[i]->pB.CompC->N;j++){
			curve_num++;
		}
	}
//	curve_num += TNurbs->pTO->pB->CompC.N;
	curve_num += TNurbs->pTO->pB.CompC->N;

//	hbody.Free_NurbsC_1DArray((NURBSC *)TNurbs->pTO->pB->CompC.pDE[0],curve_num);		// トリム面を構成する全てのNURBS曲線パラメータのメモリー解放
	hbody.Free_NurbsC_1DArray(TNurbs->pTO->pB.CompC->pDE[0].NurbsC,curve_num);		// トリム面を構成する全てのNURBS曲線パラメータのメモリー解放

	hbody.Free_NurbsS(TNurbs->pts);						// トリム面を構成するNURBS曲面パラメータのメモリー解放
//	free(TNurbs->pts);								// トリム面を構成するNURBS曲面のメモリー解放
	delete[]	TNurbs->pts;								// トリム面を構成するNURBS曲面のメモリー解放

//	hbody.Free_NurbsC(&TNurbs->pTO->pB->CompC.DegeNurbs);	// トリム面外周を構成する複合曲線を構成する縮退用NURBS曲線のメモリー解放
//	hbody.Free_CompC((COMPC *)TNurbs->pTO->pB);			// トリム面外周を構成する複合曲線を構成するNURBS曲線のメモリー解放
//	free(TNurbs->pTO->pB);							// トリム面外周を構成する複合曲線のメモリー解放
//	free(TNurbs->pTO);								// トリム面外周を構成する面上線のメモリー解放
	hbody.Free_NurbsC(&TNurbs->pTO->pB.CompC->DegeNurbs);	// トリム面外周を構成する複合曲線を構成する縮退用NURBS曲線のメモリー解放
	hbody.Free_CompC(TNurbs->pTO->pB.CompC);			// トリム面外周を構成する複合曲線を構成するNURBS曲線のメモリー解放
	delete[]	TNurbs->pTO->pB.CompC;							// トリム面外周を構成する複合曲線のメモリー解放
	delete[]	TNurbs->pTO;								// トリム面外周を構成する面上線のメモリー解放

	for(int i=0;i<TNurbs->n2;i++){
//		hbody.Free_NurbsC(&TNurbs->pTI[i]->pB->CompC.DegeNurbs);	// トリム面内周を構成する複合曲線を構成する縮退用NURBS曲線のメモリー解放
//		hbody.Free_CompC((COMPC *)TNurbs->pTI[i]->pB);	// トリム面内周を構成する複合曲線を構成するNURBS曲線のメモリー解放
//		free(TNurbs->pTI[i]->pB);					// トリム面内周を構成する複合曲線のメモリー解放
		hbody.Free_NurbsC(&TNurbs->pTI[i]->pB.CompC->DegeNurbs);	// トリム面内周を構成する複合曲線を構成する縮退用NURBS曲線のメモリー解放
		hbody.Free_CompC(TNurbs->pTI[i]->pB.CompC);	// トリム面内周を構成する複合曲線を構成するNURBS曲線のメモリー解放
		delete[]	TNurbs->pTI[i]->pB.CompC;					// トリム面内周を構成する複合曲線のメモリー解放
	}
	hbody.Free_TrmS(TNurbs);								// トリム面パラメータのメモリー解放

	return KOD_TRUE;
}

// Function: CalcNurbsCCoord
// 指定したノットtでのNURBS曲線の座標値を求める
//
// Parameters:
// *NurbsC - 対象とするNURBS曲線へのポインタ
// t - ノット値
//
// Return:
// 座標値
//Coord NURBS_Func::CalcNurbsCCoord(NURBSC *NurbsC,double t)
Coord NURBSC::CalcNurbsCCoord(double t)
{
	Coord p;
	Coord bscpw;
	double bsw=0;
	double bs=0;
	int i;

	InitCoord(&bscpw);	// 初期化

	for(i=0;i<K;i++){
		bs = CalcBSbasis(t,T,N,i,M);		// Bスプライン基底関数を求める
//		bs = CalcBSbasis(t,T.get(),N,i,M);		// Bスプライン基底関数を求める
		bsw += bs*W[i];							// 分母
		bscpw = AddCoord(bscpw,MulCoord(cp[i],bs*W[i]));	// 分子
	}
	
	p = DivCoord(bscpw,bsw);	// 座標値を求める

	return p;
}

// Function: CalcNurbsCCoords
// 指定したノットt群でのNURBS曲線の座標値を求める
//
// Parameters:
// *NurbsS - NURBS曲面へのポインタ   
// Ptnum - 求める点群の数   
// *T - tパラメータ群を格納した配列
// *Pt - 実座標値を格納
//void NURBS_Func::CalcNurbsCCoords(NURBSC *NurbsC,int Ptnum,double *T,Coord *Pt)
void NURBSC::CalcNurbsCCoords(int Ptnum,double *T,Coord *Pt)
{
	for(int i=0;i<Ptnum;i++){
		Pt[i] = CalcNurbsCCoord(T[i]);
	}
}

// Function: CalcNurbsSCoord
// 指定したノットu,vでのNURBS曲面の座標値を求める
//
// Parameters:
// *NurbsS - 対象とするNURBS曲面へのポインタ
// div_u - u方向ノット値
// div_v - v方向ノット値
//
// Return:
// 座標値
//Coord NURBS_Func::CalcNurbsSCoord(NURBSS *NurbsS,double div_u,double div_v)
Coord NURBSS::CalcNurbsSCoord(double div_u,double div_v)
{
	int i,j;
	double bs_u,bs_v;		// u,v方向Bスプライン基底関数
	double bsw=0;			// 分母
	Coord bscpw;			// 分子

	InitCoord(&bscpw);		// 初期化

	for(i=0;i<K[0];i++){
		bs_u = CalcBSbasis(div_u,S,N[0],i,M[0]);			// u方向Bスプライン基底関数を求める
		for(j=0;j<K[1];j++){
			bs_v = CalcBSbasis(div_v,T,N[1],j,M[1]);		// v方向Bスプライン基底関数を求める
			bsw += bs_u*bs_v*W[i][j];
			bscpw = AddCoord(bscpw,MulCoord(cp[i][j],bs_u*bs_v*W[i][j]));
		}
	}
	//if(fabs(bsw) < APPROX_ZERO)
		//return(SetCoord(0,0,0));

	return(DivCoord(bscpw,bsw));
}

// Function: CalcNurbsSCoords
// 指定したノットu,v群でのNURBS曲面の座標値群を求める
//
// Parameters:
// *NurbsS - NURBS曲面へのポインタ   
// Ptnum - 求める点群の数   
// *UV - u,vパラメータ群を格納したCoord型配列(UV[].xにu方向、UV[].ｙにV方向のパラメータを格納しておくこと)
// *Pt - 実座標値を格納
//void NURBS_Func::CalcNurbsSCoords(NURBSS *NurbsS,int Ptnum,Coord *UV,Coord *Pt)
void NURBSS::CalcNurbsSCoords(int Ptnum,Coord *UV,Coord *Pt)
{
	for(int i=0;i<Ptnum;i++){
		Pt[i] = CalcNurbsSCoord(UV[i].x,UV[i].y);
	}
}

// Function: CalcBSbasis
// Bスプライン基底関数を計算し、計算結果を返す
//
// Parameters:
// t - ノット　
// knot[] - ノットベクトル  
// N - ノットベクトルの数  
// I - Bspl基底関数下添字の1つ目(0～)  
// M - 階数(Bspl基底関数下添字の2つ目)  
//
// Return:
// 計算結果
//double NURBS_Func::CalcBSbasis(double t, double knot[],int N,int I,int M)
double CalcBSbasis(double t, double knot[],int N,int I,int M)
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

// Function: CalcDiffBSbasis
// Bスプライン基底関数の1階微分係数を求める
//
// Parameters:
// t - ノット　
// knot[] - ノットベクトル  
// N - ノットベクトルの数  
// I - 注目中のコントロールポイント  
// M - 階数
//
// Return:
// 計算結果
//double NURBS_Func::CalcDiffBSbasis(double t,double knot[],int N,int I,int M)
double CalcDiffBSbasis(double t,double knot[],int N,int I,int M)
{
	double n1 = knot[I+M-1]-knot[I];
	double n2 = knot[I+M]-knot[I+1];

	if(n1 != 0.0) n1 = (M-1)/n1*CalcBSbasis(t,knot,N,I,M-1);
	
	if(n2 != 0.0) n2 = (M-1)/n2*CalcBSbasis(t,knot,N,I+1,M-1);
	
	return(n1-n2);
}

// Function: CalcDiffBSbasisN
// Bスプライン基底関数のN階微分係数を求める
//
// Parameters:
// t - ノット　
// knot[] - ノットベクトル  
// N - ノットベクトルの数  
// I - 注目中のコントロールポイント  
// M - 階数  
// Dn - 微分階数
//
// Return:
// 計算結果
//double NURBS_Func::CalcDiffBSbasisN(double t,double knot[],int N,int I,int M, int Dn)
double CalcDiffBSbasisN(double t,double knot[],int N,int I,int M, int Dn)
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

// Fucntion:CalcDiffNurbsC
// NURBS曲線の1階微分係数を求める
// 
// Paramters:
// *NurbsC - NURBS曲線へのポインタ
// t - ノット値
//
// Return:
// 計算結果
//Coord NURBS_Func::CalcDiffNurbsC(NURBSC *NurbsC,double t)
Coord NURBSC::CalcDiffNurbsC(double t)
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
	for(i=0;i<K;i++){
		bs = CalcBSbasis(t,T,N,i,M);
		diff_bs = CalcDiffBSbasis(t,T,N,i,M);
//		bs = CalcBSbasis(t,T.get(),N,i,M);
//		diff_bs = CalcDiffBSbasis(t,T.get(),N,i,M);

		Ft = AddCoord(Ft,MulCoord(cp[i],bs*W[i]));
		diff_Ft = AddCoord(diff_Ft,MulCoord(cp[i],diff_bs*W[i]));

		Gt += bs*W[i];
		diff_Gt += diff_bs*W[i];
	}
	if(fabs(Gt) < APPROX_ZERO)	return(SetCoord(0,0,0));

	// 1階微分を求める
	p = SubCoord(DivCoord(diff_Ft,Gt),DivCoord(MulCoord(Ft,diff_Gt),Gt*Gt));

	return p;
}

// Function: CalcDiff2NurbsC
// NURBS曲線の2階微分係数を求める
// 
// Paramters:
// *NurbsC - NURBS曲線へのポインタ
// t - ノット値
//
// Return:
// 計算結果
//Coord NURBS_Func::CalcDiff2NurbsC(NURBSC *NurbsC,double t)
Coord NURBSC::CalcDiff2NurbsC(double t)
{
	double w0=0;
	double w1=0;
	double w2=0;
	Coord  A2=SetCoord(0,0,0);
	Coord  P0=SetCoord(0,0,0);
	Coord  P1=SetCoord(0,0,0);

	P0 = CalcNurbsCCoord(t);
	P1 = CalcDiffNurbsC(t);

	for(int i=0;i<K;i++){
		w0 += CalcBSbasis(t,T,N,i,M) * W[i];
		w1 += CalcDiffBSbasis(t,T,N,i,M) * W[i];
		w2 += CalcDiffBSbasisN(t,T,N,i,M,2) * W[i];
		A2 = AddCoord(A2,MulCoord(cp[i],CalcDiffBSbasisN(t,T,N,i,M,2) * W[i]));
//		w0 += CalcBSbasis(t,T.get(),N,i,M) * W[i];
//		w1 += CalcDiffBSbasis(t,T.get(),N,i,M) * W[i];
//		w2 += CalcDiffBSbasisN(t,T.get(),N,i,M,2) * W[i];
//		A2 = AddCoord(A2,MulCoord(cp[i],CalcDiffBSbasisN(t,T.get(),N,i,M,2) * W[i]));
	}

	return DivCoord(SubCoord(A2,AddCoord(MulCoord(P1,2*w1),MulCoord(P0,2*w2))),w0);
}

// Function: CalcDiffNNurbsC
// NURBS曲線のr階微分係数を求める
// 
// Paramters:
// *NurbsC - NURBS曲線へのポインタ
// r - 微分階数
// t - ノット値
//
// Return:
// 計算結果
//Coord NURBS_Func::CalcDiffNNurbsC(NURBSC *NurbsC,int r,double t)
Coord NURBSC::CalcDiffNNurbsC(int r,double t)
{
	if(!r)
		return CalcNurbsCCoord(t);

	Coord Ar = InitCoord();
	double W = 0;
	for(int i=0;i<K;i++){
		double bsr = CalcDiffBSbasisN(t,T,N,i,M,r);
//		double bsr = CalcDiffBSbasisN(t,T.get(),N,i,M,r);
		Ar = AddCoord(Ar,MulCoord(cp[i],bsr*this->W[i]));
		W += this->W[i]*CalcBSbasis(t,T,N,i,M);
//		W += this->W[i]*CalcBSbasis(t,T.get(),N,i,M);
	}

	Coord Br = InitCoord();
	for(int i=1;i<=r;i++){
		double Wi = 0;
		for(int j=0;j<K;j++){
			double bsi = CalcDiffBSbasisN(t,T,N,j,M,i);
//			double bsi = CalcDiffBSbasisN(t,T.get(),N,j,M,i);
			Wi += bsi*this->W[j];
		}
		if(Wi == 0.0)  return(InitCoord());
		Br = AddCoord(Br,MulCoord(CalcDiffNNurbsC(r-i,t),(double)nCr(r,i)*Wi));	// 回帰
	}

	return (DivCoord(SubCoord(Ar,Br),W));
}

// Function: CalcDiffuNurbsS
// NURBS曲面のu方向の1階微分係数を得る
//
// Parameters:
// *NurbsS - NURBS曲面へのポインタ
// div_u - u方向ノット値
// div_v - v方向ノット値
// 
// Return:
// 計算結果
//Coord NURBS_Func::CalcDiffuNurbsS(NURBSS *NurbsS,double div_u,double div_v)
Coord NURBSS::CalcDiffuNurbsS(double div_u,double div_v)
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

	for(i=0;i<K[0];i++){
		bs_u = CalcBSbasis(div_u,S,N[0],i,M[0]);				// u方向Bスプライン基底関数を求める
		diff_bs_u = CalcDiffBSbasis(div_u,S,N[0],i,M[0]);	// u方向Bスプライン基底関数の1階微分を求める
		for(j=0;j<K[1];j++){
			bs_v = CalcBSbasis(div_v,T,N[1],j,M[1]);			// v方向Bスプライン基底関数を求める
			Ft = AddCoord(Ft,MulCoord(cp[i][j],bs_u*bs_v*W[i][j]));
			diff_Ft = AddCoord(diff_Ft,MulCoord(cp[i][j],diff_bs_u*bs_v*W[i][j]));
			Gt += bs_u*bs_v*W[i][j];
			diff_Gt += diff_bs_u*bs_v*W[i][j];
		}
	}

	if(fabs(Gt) < APPROX_ZERO_H)	return(SetCoord(0,0,0));

	// 1階微分を求める
	p = SubCoord(DivCoord(diff_Ft,Gt),DivCoord(MulCoord(Ft,diff_Gt),Gt*Gt));

	return p;
}

// Function: CalcDiffvNurbsS
// NURBS曲面のv方向の1階微分係数を得る
//
// Parameters:
// *NurbsS - NURBS曲面へのポインタ
// div_u - u方向ノット値
// div_v - v方向ノット値
// 
// Return:
// 計算結果
//Coord NURBS_Func::CalcDiffvNurbsS(NURBSS *NurbsS,double div_u,double div_v)
Coord NURBSS::CalcDiffvNurbsS(double div_u,double div_v)
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

	for(i=0;i<K[0];i++){
		bs_u = CalcBSbasis(div_u,S,N[0],i,M[0]);				// u方向Bスプライン基底関数を求める
		for(j=0;j<K[1];j++){
			bs_v = CalcBSbasis(div_v,T,N[1],j,M[1]);				// v方向Bスプライン基底関数を求める
			diff_bs_v = CalcDiffBSbasis(div_v,T,N[1],j,M[1]);	// v方向Bスプライン基底関数の1階微分を求める
			Ft = AddCoord(Ft,MulCoord(cp[i][j],bs_u*bs_v*W[i][j]));
			diff_Ft = AddCoord(diff_Ft,MulCoord(cp[i][j],bs_u*diff_bs_v*W[i][j]));
			Gt += bs_u*bs_v*W[i][j];
			diff_Gt += bs_u*diff_bs_v*W[i][j];
		}
	}

	if(fabs(Gt) < APPROX_ZERO_H)	return(SetCoord(0,0,0));

	// 1階微分を求める
	p = SubCoord(DivCoord(diff_Ft,Gt),DivCoord(MulCoord(Ft,diff_Gt),Gt*Gt));

	return p;
}

// Function: CalcDiffNNurbsS
// NURBS曲面の各方向を任意階微分したときの微分係数を求める
//
// Parameters:
// *S - NURBS曲面へのポインタ   
// k - u方向の微分階数    
// l - v方向の微分階数   
// u,v - u方向v方向それぞれのパラメータ
// 
// Return:
// 計算結果
//Coord NURBS_Func::CalcDiffNNurbsS(NURBSS *S,int k,int l,double u,double v)
Coord NURBSS::CalcDiffNNurbsS(int k,int l,double u,double v)
{
	double w = CalcDiffNurbsSDenom(0,0,u,v);
	Coord  A = CalcDiffNurbsSNumer(k,l,u,v);
	Coord  B = SetCoord(0,0,0);
	Coord  C = SetCoord(0,0,0);
	Coord  D = SetCoord(0,0,0);

	if(!k && !l)
		return(CalcNurbsSCoord(u,v));
		
	for(int i=1;i<=k;i++)
		B = AddCoord(B,MulCoord(CalcDiffNNurbsS(k-i,l,u,v),nCr(k,i)*CalcDiffNurbsSDenom(i,0,u,v)));
	for(int j=1;j<=l;j++)
		C = AddCoord(C,MulCoord(CalcDiffNNurbsS(k,l-j,u,v),nCr(l,j)*CalcDiffNurbsSDenom(0,j,u,v)));
	for(int i=1;i<=k;i++){
		for(int j=1;j<=l;j++){
			D = AddCoord(D,MulCoord(CalcDiffNNurbsS(k-i,l-j,u,v),nCr(l,j)*CalcDiffNurbsSDenom(i,j,u,v)));
		}
		D = MulCoord(D,nCr(k,i));
	}
	return(DivCoord(SubCoord(A,AddCoord(B,AddCoord(C,D))),w));

}

// Function: CalcDiffNurbsSDenom
// (private)NURBS曲面分母の各方向を任意階微分したときの微分係数を求める
//
// Parameters:
// *S - NURBS曲面へのポインタ   
// k - u方向の微分階数    
// l - v方向の微分階数   
// u,v - u方向v方向それぞれのパラメータ
// 
// Return:
// 計算結果
//double NURBS_Func::CalcDiffNurbsSDenom(NURBSS *S,int k,int l,double u,double v)
double NURBSS::CalcDiffNurbsSDenom(int k,int l,double u,double v)
{
	double w=0;
	for(int i=0;i<K[0];i++){
		double Nk = CalcDiffBSbasisN(u,S,N[0],i,M[0],k);		// u方向のk階微分
		for(int j=0;j<K[1];j++){
			double Nl = CalcDiffBSbasisN(v,T,N[1],j,M[1],l);	// v方向のl階微分
			w += Nk*Nl*W[i][j];
		}
	}
	return w;
}

// Function: CalcDiffNurbsSNumer
// (private)NURBS曲面分子の各方向を任意階微分したときの微分係数を求める
//
// Parameters:
// *S - NURBS曲面へのポインタ   
// k - u方向の微分階数    
// l - v方向の微分階数   
// u,v - u方向v方向それぞれのパラメータ
// 
// Return:
// 計算結果
//Coord NURBS_Func::CalcDiffNurbsSNumer(NURBSS *S,int k,int l,double u,double v)
Coord NURBSS::CalcDiffNurbsSNumer(int k,int l,double u,double v)
{
	Coord A=SetCoord(0,0,0);
	for(int i=0;i<K[0];i++){
		double Nk = CalcDiffBSbasisN(u,S,N[0],i,M[0],k);		// u方向のk階微分
		for(int j=0;j<K[1];j++){
			double Nl = CalcDiffBSbasisN(v,T,N[1],j,M[1],l);	// v方向のl階微分
			A = AddCoord(A,MulCoord(cp[i][j],Nk*Nl*W[i][j]));
		}
	}
	return A;
}

// Function: CalcNormVecOnNurbsS
// NURBS曲面上の(u,v)における法線ベクトルをもとめる
// 
// Parameters:
// *nurb - NURBS曲面へのポインタ
// u - u方向ノット値
// v - v方向ノット値
//
// Retrurn:
// 計算結果
//Coord NURBS_Func::CalcNormVecOnNurbsS(NURBSS *nurb,double u,double v)
Coord NURBSS::CalcNormVecOnNurbsS(double u,double v)
{
	Coord a = CalcDiffuNurbsS(u,v);
	Coord b = CalcDiffvNurbsS(u,v);

	return(NormalizeVec(CalcOuterProduct(a,b)));
}

// Function: CalcTanVecOnNurbsC
// NURBS曲線上のtにおける単位接ベクトルをもとめる
//
// Parameters:
// *C - NURBS曲線へのポインタ
// t - ノット値
//
// Retrurn:
// 計算結果
//Coord NURBS_Func::CalcTanVecOnNurbsC(NURBSC *C,double t)
Coord NURBSC::CalcTanVecOnNurbsC(double t)
{
    return NormalizeVec(CalcDiffNurbsC(t));
}

// Function: CalcDiffuNormVecOnNurbsS
// NURBS曲面上の(u,v)における法線ベクトルのu方向1階微分をもとめる
// Nu = Suu×Sv + Su×Suv
//
// Parameters:
// *nurb - NURBS曲面へのポインタ
// u - u方向ノット値
// v - v方向ノット値
//
// Retrurn:
// 計算結果
//Coord NURBS_Func::CalcDiffuNormVecOnNurbsS(NURBSS *nurb,double u,double v)
Coord NURBSS::CalcDiffuNormVecOnNurbsS(double u,double v)
{
	Coord Suu = CalcDiffNNurbsS(2,0,u,v);
	Coord Suv = CalcDiffNNurbsS(1,1,u,v);
	Coord Su = CalcDiffuNurbsS(u,v);
	Coord Sv = CalcDiffvNurbsS(u,v);

	return (NormalizeVec(AddCoord(CalcOuterProduct(Suu,Sv),CalcOuterProduct(Su,Suv))));
}

// Function: CalcDiffvNormVecOnNurbsS
// NURBS曲面上の(u,v)における法線ベクトルのv方向1階微分をもとめる
// Nv = Suv×Sv + Su×Svv
// 
// Parameters:
// *nurb - NURBS曲面へのポインタ
// u - u方向ノット値
// v - v方向ノット値
//
// Retrurn:
// 計算結果
//Coord NURBS_Func::CalcDiffvNormVecOnNurbsS(NURBSS *nurb,double u,double v)
Coord NURBSS::CalcDiffvNormVecOnNurbsS(double u,double v)
{
	Coord Suv = CalcDiffNNurbsS(1,1,u,v);
	Coord Svv = CalcDiffNNurbsS(0,2,u,v);
	Coord Su = CalcDiffuNurbsS(u,v);
	Coord Sv = CalcDiffvNurbsS(u,v);

	return (NormalizeVec(AddCoord(CalcOuterProduct(Suv,Sv),CalcOuterProduct(Su,Svv))));
}

// Function: CalcMeanCurvature
// NURBS曲面上の(u,v)における平均曲率を求める
// 
// Parameters:
// *nurb - NURBS曲面へのポインタ
// u - u方向ノット値
// v - v方向ノット値
//
// Retrurn:
// 計算結果
//double NURBS_Func::CalcMeanCurvature(NURBSS *nurb,double u,double v)
double NURBSS::CalcMeanCurvature(double u,double v)
{
	Coord du = CalcDiffuNurbsS(u,v);			// u方向1階微分
	Coord dv = CalcDiffvNurbsS(u,v);			// v方向1階微分
	double E = CalcInnerProduct(du,du);				// 第1基本量
	double F = CalcInnerProduct(du,dv);				// 第1基本量
	double G = CalcInnerProduct(dv,dv);				// 第1基本量
	Coord duu = CalcDiffNNurbsS(2,0,u,v);		// u方向2階微分
	Coord dvv = CalcDiffNNurbsS(0,2,u,v);		// v方向2階微分
	Coord duv = CalcDiffNNurbsS(1,1,u,v);		// u,v方向各1階微分
	Coord n = CalcNormVecOnNurbsS(u,v);		// 法線ベクトル
	double L = CalcInnerProduct(duu,n);				// 第2基本量
	double M = CalcInnerProduct(duv,n);				// 第2基本量
	double N = CalcInnerProduct(dvv,n);				// 第2基本量
	double H = -(G*L+E*N-2*F*M)/(E*G-F*F)/2;		// 平均曲率

	return H;
}
/*	-- Move to SFQuant.cpp
// Function: CalcMeanCurvature
// NURBS曲面上の(u,v)における平均曲率を求める（オーバーロード）
// 
// Parameters:
// q - 曲面の基本量をセットにした構造体
//
// Retrurn:
// 計算結果
double NURBS_Func::CalcMeanCurvature(SFQuant q)
{
	return -(q.G*q.L+q.E*q.N-2*q.F*q.M)/(q.E*q.G-q.F*q.F)/2;		// 平均曲率
}
*/
// Function: CalcMeanCurvatureNormVec
// NURBS曲面上の(u,v)における平均曲率法線ベクトルをもとめる
//
// Parameters:
// *nurb - NURBS曲面へのポインタ
// u - u方向ノット値
// v - v方向ノット値
//
// Retrurn:
// 計算結果
//Coord NURBS_Func::CalcMeanCurvatureNormVec(NURBSS *nurb,double u,double v)
Coord NURBSS::CalcMeanCurvatureNormVec(double u,double v)
{
	Coord n = CalcNormVecOnNurbsS(u,v);				// 法線ベクトル
	Coord Hn = MulCoord(n,CalcMeanCurvature(u,v));		// 平均曲率法線ベクトル

	return Hn;
}

// Function: CalcGaussCurvature
// NURBS曲面上の(u,v)におけるガウス曲率を求める
//
// Parameters:
// *nurb - NURBS曲面へのポインタ
// u - u方向ノット値
// v - v方向ノット値
//
// Retrurn:
// 計算結果
//double NURBS_Func::CalcGaussCurvature(NURBSS *nurb,double u,double v)
double NURBSS::CalcGaussCurvature(double u,double v)
{
	Coord du = CalcDiffuNurbsS(u,v);			// u方向1階微分
	Coord dv = CalcDiffvNurbsS(u,v);			// v方向1階微分
	double E = CalcInnerProduct(du,du);				// 第1基本量
	double F = CalcInnerProduct(du,dv);				// 第1基本量
	double G = CalcInnerProduct(dv,dv);				// 第1基本量
	Coord duu = CalcDiffNNurbsS(2,0,u,v);		// u方向2階微分
	Coord dvv = CalcDiffNNurbsS(0,2,u,v);		// v方向2階微分
	Coord duv = CalcDiffNNurbsS(1,1,u,v);		// u,v方向各1階微分
	Coord n = CalcNormVecOnNurbsS(u,v);		// 法線ベクトル
	double L = CalcInnerProduct(duu,n);				// 第2基本量
	double M = CalcInnerProduct(duv,n);				// 第2基本量
	double N = CalcInnerProduct(dvv,n);				// 第2基本量
	double K = (L*N-M*M)/(E*G-F*F);					// ガウス曲率

	return K;
}
/*	-- Move to SFQuant.cpp
// Function: CalcGaussCurvature
// NURBS曲面上の(u,v)におけるガウス曲率を求める（オーバーロード）
//
// Parameters:
// q - 曲面の基本量をセットにした構造体
//
// Retrurn:
// 計算結果
double NURBS_Func::CalcGaussCurvature(SFQuant q)
{
	return (q.L*q.N-q.M*q.M)/(q.E*q.G-q.F*q.F);					// ガウス曲率
}
*/
// Function: CalcGaussCurvatureNormVec
// NURBS曲面上の(u,v)におけるガウス曲率法線ベクトルをもとめる
//
// Parameters:
// *nurb - NURBS曲面へのポインタ
// u - u方向ノット値
// v - v方向ノット値
//
// Retrurn:
// 計算結果
//Coord NURBS_Func::CalcGaussCurvatureNormVec(NURBSS *nurb,double u,double v)
Coord NURBSS::CalcGaussCurvatureNormVec(double u,double v)
{
	SFQuant q(this,u,v);
//	return MulCoord(q.n,CalcGaussCurvature(q));	// ガウス曲率法線ベクトル
	return MulCoord(q.n,q.CalcGaussCurvature());	// ガウス曲率法線ベクトル
}

// Function: CalcCurvatureNurbsC
// NURBS曲線の曲率を求める
//
// Parameters:
// *C - NURBS曲線へのポインタ
// t - ノット値
//
// Retrurn:
// 計算結果
//double NURBS_Func::CalcCurvatureNurbsC(NURBSC *C,double t)
double NURBSC::CalcCurvatureNurbsC(double t)
{
	Coord p_ = CalcDiffNurbsC(t);
	Coord p__ = CalcDiff2NurbsC(t);

	return(CalcEuclid(CalcOuterProduct(p_,p__))/pow(CalcEuclid(p_),3));
}

// Function: DetectInterfereNurbsS
// NURBS曲面S(u,v)とNURBS曲面R(w,t)の干渉を検出する(トリム無)
// 
// Parameters:
// *nurbS - NURBS曲面S(u,v) 
// *nurbR - NURBS曲面R(w,t) 
// divnum - パラメータ分割数(初期点の数)
// 
// Return:
// 干渉有:KOD_TRUE, 干渉無:KOD_FALSE
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
//						Coord p0 = CalcNurbsSCoord(nurbR,w0,t0);					// R(w0,t0)となる点(初期点)の座標
//						Coord q0 = CalcNurbsSCoord(nurbS,u0,v0);					// S(u0,v0)となる点(初期点)の座標
//						Coord rw = CalcDiffuNurbsS(nurbR,w0,t0);					// 点R(w0,t0)のu偏微分(基本ベクトル)
//						Coord rt = CalcDiffvNurbsS(nurbR,w0,t0);					// 点R(w0,t0)のv偏微分(基本ベクトル)
						Coord p0 = nurbR->CalcNurbsSCoord(w0,t0);					// R(w0,t0)となる点(初期点)の座標
						Coord q0 = nurbS->CalcNurbsSCoord(u0,v0);					// S(u0,v0)となる点(初期点)の座標
						Coord rw = nurbR->CalcDiffuNurbsS(w0,t0);					// 点R(w0,t0)のu偏微分(基本ベクトル)
						Coord rt = nurbR->CalcDiffvNurbsS(w0,t0);					// 点R(w0,t0)のv偏微分(基本ベクトル)
						double rwt = CalcEuclid(CalcOuterProduct(rw,rt));
						if(rwt==0.0) break;
						Coord np = DivCoord(CalcOuterProduct(rw,rt),rwt);			// 点R(w0,t0)の単位法線ベクトル
//						Coord su = CalcDiffuNurbsS(nurbS,u0,v0);					// 点S(u0,v0)のu偏微分(基本ベクトル)
//						Coord sv = CalcDiffvNurbsS(nurbS,u0,v0);					// 点S(u0,v0)のv偏微分(基本ベクトル)
						Coord su = nurbS->CalcDiffuNurbsS(u0,v0);					// 点S(u0,v0)のu偏微分(基本ベクトル)
						Coord sv = nurbS->CalcDiffvNurbsS(u0,v0);					// 点S(u0,v0)のv偏微分(基本ベクトル)
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
                            //GuiIF.SetMessage("Interference with the NURBS surface was detected");
							return KOD_TRUE;
						}
					}
				}
			}
		}
	}
	
    //GuiIF.SetMessage("Interference with the NURBS surface was not detected");
	return KOD_FALSE;
}

// Function: DetectInterfereTrmS
// NURBS曲面S(u,v)とNURBS曲面R(w,t)の干渉を検出する(トリム有)
// 
// Parameters:
// *tNurbS - NURBS曲面S(u,v)(トリム有) 
// *tNurbR - NURBS曲面R(w,t)(トリム有) 
// divnum - パラメータ分割数(初期点の数)
//
// Return:
// 干渉有:KOD_TRUE, 干渉無:KOD_FALSE
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
//						Coord p0 = CalcNurbsSCoord(tNurbR->pts,w0,t0);					// R(w0,t0)となる点(初期点)の座標
//						Coord q0 = CalcNurbsSCoord(tNurbS->pts,u0,v0);					// S(u0,v0)となる点(初期点)の座標
//						Coord rw = CalcDiffuNurbsS(tNurbR->pts,w0,t0);					// 点R(w0,t0)のu偏微分(基本ベクトル)
//						Coord rt = CalcDiffvNurbsS(tNurbR->pts,w0,t0);					// 点R(w0,t0)のv偏微分(基本ベクトル)
						Coord p0 = tNurbR->pts->CalcNurbsSCoord(w0,t0);					// R(w0,t0)となる点(初期点)の座標
						Coord q0 = tNurbS->pts->CalcNurbsSCoord(u0,v0);					// S(u0,v0)となる点(初期点)の座標
						Coord rw = tNurbR->pts->CalcDiffuNurbsS(w0,t0);					// 点R(w0,t0)のu偏微分(基本ベクトル)
						Coord rt = tNurbR->pts->CalcDiffvNurbsS(w0,t0);					// 点R(w0,t0)のv偏微分(基本ベクトル)
						double rwt = CalcEuclid(CalcOuterProduct(rw,rt));
						if(rwt==0.0) break;
						Coord np = DivCoord(CalcOuterProduct(rw,rt),rwt);				// 点R(w0,t0)の単位法線ベクトル
//						Coord su = CalcDiffuNurbsS(tNurbS->pts,u0,v0);					// 点S(u0,v0)のu偏微分(基本ベクトル)
//						Coord sv = CalcDiffvNurbsS(tNurbS->pts,u0,v0);					// 点S(u0,v0)のv偏微分(基本ベクトル)
						Coord su = tNurbS->pts->CalcDiffuNurbsS(u0,v0);					// 点S(u0,v0)のu偏微分(基本ベクトル)
						Coord sv = tNurbS->pts->CalcDiffvNurbsS(u0,v0);					// 点S(u0,v0)のv偏微分(基本ベクトル)
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
                                //GuiIF.SetMessage("Interference with the trimmed NURBS surface was detected");
								return KOD_TRUE;
							}
						}
					}
				}
			}
		}
	}
	
    //GuiIF.SetMessage("Interference with the trimmed NURBS surface was not detected");
	return KOD_FALSE;
}

// Function: CalcIntersecPtsPlaneV3
// 3次以下のNURBS曲面と平面との交点群を代数解法にて求める(vパラメータ分割)
// 
// Parameters:
// *nurb - NURBS曲面  
// pt - 平面上の一点  
// nvec - 平面の法線ベクトル  
// v_divnum - vパラメータ分割数
// *ans - 算出された交点のu,vパラメータ値をそれぞれans.x,ans.yに格納
// ans_size - ansの配列長
//
// Return:
// 交点の個数(交点の数がans_sizeを超えた場合：KOD_ERR)
//int NURBS_Func::CalcIntersecPtsPlaneV3(NURBSS *nurb,Coord pt,Coord nvec,int v_divnum,Coord *ans,int ans_size)
int NURBSS::CalcIntersecPtsPlaneV3(Coord pt,Coord nvec,int v_divnum,Coord *ans,int ans_size)
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

//	N = (double *)malloc(sizeof(double)*nurb->K[1]);
//	A = (double *)malloc(sizeof(double)*nurb->K[0]);
//	B = (Coord *)malloc(sizeof(Coord)*nurb->K[0]);
	N = new double[K[1]];
	A = new double[K[0]];
	B = new Coord[K[0]];
	coef = NewMatrix(M[0],M[0]);

	// vパラメータを区間内で分割し、各vパラメータ上のNURBS曲線C(u)と平面(pt,nvec)との交点を求める
	for(int v=0;v<=v_divnum;v++){
		v_const = (V[1] - V[0])*(double)v/(double)v_divnum;		// 適当なv方向パラメータを設定
		for(int i=0;i<K[1];i++){
			N[i] = CalcBSbasis(v_const,T,N[0],i,M[1]);		// v_const時のBスプライン基底関数を求める
		}
		for(int i=0;i<K[0];i++){
			A[i] = 0;
			InitCoord(&B[i]);
			for(int j=0;j<K[1];j++){
				A[i] += N[j]*W[i][j];			// v_const上のNURBS曲線C(u)の分母の係数
				B[i] = AddCoord(B[i],MulCoord(cp[i][j],N[j]*W[i][j]));		// v_const上のNURBS曲線C(u)の分子の係数
			}
		}
		for(int i=0;i<K[0]-M[0]+1;i++){						// i番目の曲線に対して
			InitMatrix(coef,M[0],M[0]);
			InitVector(a,4);
			InitCoord(P,4);
			InitVector(Q,4);
			InitVector(t,3);
			if(M[0]-1 == 3){										// 3次
				GetBSplCoef3(M[0],K[0],i,S,coef);		// 3次のBスプライン基底関数の係数を求める
			}
			else if(M[0]-1 == 2){									// 2次
				GetBSplCoef2(M[0],K[0],i,S,coef);		// 2次のBスプライン基底関数の係数を求める
			}
			else if(M[0]-1 == 1){									// 1次
				GetBSplCoef1(M[0],K[0],i,S,coef);		// 1次のBスプライン基底関数の係数を求める
			}
			GetNurbsSCoef(M[0],coef,A,B,i,P,Q);					// 固定されたvパラメータ上のNURBS曲線C(u)の係数を求める
			GetIntersecEquation(M[0],P,Q,pt,nvec,a);				// 方程式を導出
			ansnum = CalcEquation(a,t,M[0]-1);					// 方程式を解く
			int hitnum = 0;						// 条件に適合する解の数をカウントする
			for(int j=0;j<ansnum;j++){			// 3つの解それぞれに対して
				if(t[j] >= S[i+M[0]-1] && t[j] <= S[i+M[0]]){	// 注目中のノットベクトルの範囲内なら
					ans[allansnum+hitnum] = SetCoord(t[j],v_const,0);		// 解として登録
					hitnum++;
				}
			}
			allansnum += hitnum;				// 条件適合解の数だけ総解数をカウントアップ
			if(allansnum >= ans_size){
                GuiIF.SetMessage("NURBS KOD_ERR:Intersection points exceeded the allocated array length");
				allansnum = KOD_ERR;
				goto EXIT;
			}
		}
	}

EXIT:
//	free(N);
//	free(A);
//	free(B);
	delete[]	N;
	delete[]	A;
	delete[]	B;
	FreeMatrix(coef,M[0]);

	return allansnum;
}

// Function: CalcIntersecPtsPlaneU3
// 3次以下のNURBS曲面と平面との交点群を代数解法にて求める(uパラメータ分割)
//
// Parameters:
// *nurb - NURBS曲面  
// pt - 平面上の一点  
// nvec - 平面の法線ベクトル  
// u_divnum - uパラメータ分割数
// *ans - 算出された交点のu,vパラメータ値ををそれぞれans.x,ans.yに格納
// ans_size - ansの配列長
//
// Return:
// 交点の個数(交点の数がans_sizeを超えた：KOD_ERR)
//int NURBS_Func::CalcIntersecPtsPlaneU3(NURBSS *nurb,Coord pt,Coord nvec,int u_divnum,Coord *ans,int ans_size)
int NURBSS::CalcIntersecPtsPlaneU3(Coord pt,Coord nvec,int u_divnum,Coord *ans,int ans_size)
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

//	N = (double *)malloc(sizeof(double)*nurb->K[0]);
//	A = (double *)malloc(sizeof(double)*nurb->K[1]);
//	B = (Coord *)malloc(sizeof(Coord)*nurb->K[1]);
	N = new double[K[0]];
	A = new double[K[1]];
	B = new Coord[K[1]];
	coef = NewMatrix(M[1],M[1]);

	// uパラメータを区間内で分割し、各uパラメータ上のNURBS曲線C(v)と平面(pt,nvec)との交点を求める
	for(int u=0;u<=u_divnum;u++){
		u_const = (U[1] - U[0])*(double)u/(double)u_divnum;		// 適当なu方向パラメータを設定
		for(int i=0;i<K[0];i++){
			N[i] = CalcBSbasis(u_const,S,N[0],i,M[0]);		// u_const時のBスプライン基底関数を求める
		}
		for(int j=0;j<K[1];j++){
			A[j] = 0;
			InitCoord(&B[j]);
			for(int i=0;i<K[0];i++){
				A[j] += N[i]*W[i][j];			// u_const上のNURBS曲線C(v)の分母の係数
				B[j] = AddCoord(B[j],MulCoord(cp[i][j],N[i]*W[i][j]));		// u_const上のNURBS曲線C(v)の分子の係数
			}
		}
		for(int i=0;i<K[1]-M[1]+1;i++){						// i番目の曲線に対して
			if(M[1]-1 == 3){										// 3次
				GetBSplCoef3(M[1],K[1],i,T,coef);		// 3次のBスプライン基底関数の係数を求める
			}
			else if(M[1]-1 == 2){									// 2次
				GetBSplCoef2(M[1],K[1],i,T,coef);		// 2次のBスプライン基底関数の係数を求める
			}
			else if(M[1]-1 == 1){									// 1次
				GetBSplCoef1(M[1],K[1],i,T,coef);		// 1次のBスプライン基底関数の係数を求める
			}
			GetNurbsSCoef(M[1],coef,A,B,i,P,Q);					// 固定されたuパラメータ上のNURBS曲線C(v)の係数を求める
			GetIntersecEquation(M[1],P,Q,pt,nvec,a);				// 方程式を導出
			ansnum = CalcEquation(a,t,M[1]-1);					// 方程式を解く

			int hitnum = 0;						// 条件に適合する解の数をカウントする
			for(int j=0;j<ansnum;j++){			// 3つの解それぞれに対して
				if(t[j] >= T[i+M[1]-1] && t[j] <= T[i+M[1]]){	// 注目中のノットベクトルの範囲内なら
					ans[allansnum+hitnum] = SetCoord(u_const,t[j],0);		// 解として登録
					hitnum++;
				}
			}
			allansnum += hitnum;				// 条件適合解の数だけ総解数をカウントアップ
			if(allansnum >= ans_size){
                GuiIF.SetMessage("NURBS KOD_ERR:Intersection points exceeded the allocated array length");
				allansnum = KOD_ERR;
				goto EXIT;
			}
		}
	}

EXIT:
//	free(N);
//	free(A);
//	free(B);
	delete[]	N;
	delete[]	A;
	delete[]	B;
	FreeMatrix(coef,M[1]);

	return allansnum;
}

// Function: CalcIntersecPtsPlaneV
// V方向のアイソパラ曲線を指定した分割数で生成し，各曲線とNURBS曲面との交点を算出する
//
// Parameters:
// *nurb - NURBS曲面  
// pt - 平面上の一点  
// nvec - 平面の法線ベクトル  
// v_divnum - vパラメータ分割数
// *ans - 算出された交点のu,vパラメータ値をそれぞれans.x,ans.yに格納
// ans_size - ansの配列長
//
// Return:
// 交点の個数(交点の数がans_sizeを超えた：KOD_ERR)
//int NURBS_Func::CalcIntersecPtsPlaneV(NURBSS *nurbss,Coord pt,Coord nvec,int v_divnum,Coord *ans,int ans_size)
int NURBSS::CalcIntersecPtsPlaneV(Coord pt,Coord nvec,int v_divnum,Coord *ans,int ans_size)
{
	double v_const;			// 定数と置いたときのvパラメータ
	int ansnum;				// 1つのアイソパラ曲線と曲面の交点群の数
	int allansnum=0;		// 全交点数をカウント
	int ansbufsize = 2*(M[0]-1)*((K[0]>K[1]?K[0]:K[1])-M[0]+1);	// 1つのアイソパラ曲線と曲面の交点群を格納する配列の配列長
	double *ansbuf;			// 1つのアイソパラ曲線と曲面の交点群を格納する配列
	NURBSC nurbsc;			// 1つのアイソパラ曲線

	ansbuf = NewVector(ansbufsize);

	// vパラメータを区間内で分割し、各vパラメータ上のNURBS曲線C(u)と平面(pt,nvec)との交点を求める
	for(int v=0;v<=v_divnum;v++){
		v_const = (V[1] - V[0])*(double)v/(double)v_divnum;			// 適当なv方向パラメータを設定
//		ansnum = CalcIntersecIsparaCurveU(nurbss,v_const,pt,nvec,v_divnum,ansbuf,ansbufsize);		// アイソパラ曲線と曲面の交点群を算出
		ansnum = CalcIntersecIsparaCurveU(v_const,pt,nvec,v_divnum,ansbuf,ansbufsize);		// アイソパラ曲線と曲面の交点群を算出
		if(ansnum == KOD_ERR)	goto EXIT;
		for(int i=0;i<ansnum;i++){
//			Coord a = CalcNurbsSCoord(nurbss,ansbuf[i],v_const);
			Coord a = CalcNurbsSCoord(ansbuf[i],v_const);
			ans[allansnum+i] = SetCoord(ansbuf[i],v_const,0);					// 解を登録
		}
		allansnum += ansnum;
	}

EXIT:
	FreeVector(ansbuf);
	return allansnum;
}

// Function: CalcIntersecPtsPlaneU
// U方向のアイソパラ曲線を指定した分割数で生成し，各曲線とNURBS曲面との交点を算出する
//
// Parameters:
// *nurb - NURBS曲面  
// pt - 平面上の一点  
// nvec - 平面の法線ベクトル  
// u_divnum - uパラメータ分割数
// *ans - 算出された交点のu,vパラメータ値をそれぞれans.x,ans.yに格納
// ans_size - ansの配列長
//
// Return:
// 交点の個数(交点の数がans_sizeを超えた：KOD_ERR)
//int NURBS_Func::CalcIntersecPtsPlaneU(NURBSS *nurbss,Coord pt,Coord nvec,int u_divnum,Coord *ans,int ans_size)
int NURBSS::CalcIntersecPtsPlaneU(Coord pt,Coord nvec,int u_divnum,Coord *ans,int ans_size)
{
	double u_const;			// 定数と置いたときのvパラメータ
	int ansnum;				// 1つのアイソパラ曲線と曲面の交点群の数
	int allansnum=0;		// 全交点数をカウント
	int ansbufsize = 2*(M[0]-1)*((K[0]>K[1]?K[0]:K[1])-M[0]+1);	// 1つのアイソパラ曲線と曲面の交点群を格納する配列の配列長
	double *ansbuf;			// 1つのアイソパラ曲線と曲面の交点群を格納する配列
	NURBSC nurbsc;			// 1つのアイソパラ曲線

	ansbuf = NewVector(ansbufsize);

	// uパラメータを区間内で分割し、各uパラメータ上のアイソパラ曲線C(v)と平面(pt,nvec)との交点を求める
	for(int u=0;u<=u_divnum;u++){
		u_const = (U[1] - U[0])*(double)u/(double)u_divnum;			// 適当なu方向パラメータを設定
//		ansnum = CalcIntersecIsparaCurveV(nurbss,u_const,pt,nvec,u_divnum,ansbuf,ansbufsize);		// アイソパラ曲線と曲面の交点群を算出
		ansnum = CalcIntersecIsparaCurveV(u_const,pt,nvec,u_divnum,ansbuf,ansbufsize);		// アイソパラ曲線と曲面の交点群を算出
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

// Function:
// NURBS曲面と平面との交点群を幾何学的に求める(補助平面を用いた解法)
//
// Parameters:
// *nurb - NURBS曲面  
// pt - 平面上の一点  
// nf - 平面の法線ベクトル　
// u_divnum - uパラメータ分割数　
// v_divnum - vパラメータ分割数
// *ans - 算出された交点のu,vパラメータ値をそれぞれans.x,ans.yに格納
// ans_size - ansの配列長
//
// Return:
// 交点の個数(交点の数がans_sizeを超えた：ERR)
//int NURBS_Func::CalcIntersecPtsPlaneGeom(NURBSS *nurb,Coord pt,Coord nf,int u_divnum,int v_divnum,Coord *ans,int ans_size)
int NURBSS::CalcIntersecPtsPlaneGeom(Coord pt,Coord nf,int u_divnum,int v_divnum,Coord *ans,int ans_size)
{
//	Coord init_pt;
	int ansnum=0;

	for(int u=0;u<=u_divnum;u++){
		for(int v=0;v<=v_divnum;v++){
			double u0 = U[0] + (U[1] - U[0])*(double)u/(double)u_divnum;
			double v0 = V[0] + (V[1] - V[0])*(double)v/(double)v_divnum;
			for(int i=0;i<LOOPCOUNTMAX;i++){
//				Coord p0 = CalcNurbsSCoord(nurb,u0,v0);						// S(u0,v0)となる点(初期点)の座標
//				Coord su = CalcDiffuNurbsS(nurb,u0,v0);						// 点S(u0,v0)のu偏微分(基本ベクトル)
//				Coord sv = CalcDiffvNurbsS(nurb,u0,v0);						// 点S(u0,v0)のv偏微分(基本ベクトル)
				Coord p0 = CalcNurbsSCoord(u0,v0);						// S(u0,v0)となる点(初期点)の座標
				Coord su = CalcDiffuNurbsS(u0,v0);						// 点S(u0,v0)のu偏微分(基本ベクトル)
				Coord sv = CalcDiffvNurbsS(u0,v0);						// 点S(u0,v0)のv偏微分(基本ベクトル)
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
				Coord nume_p1 = AddCoord(nume_p1_sub,MulCoord(cross_nft,dn));					// p1の分子を算出
				double denom_p1 = CalcScalarTriProduct(nf,nt,nn);			// p1の分母を算出
				Coord p1 = DivCoord(nume_p1,denom_p1);						// p1を算出
				Coord deltap = SubCoord(p1,p0);								// 点p1と点p0の距離を算出
				double deltap_dis = CalcEuclid(deltap);						// Δpの距離を算出
				double tri_su = CalcScalarTriProduct(su,sv,nf);
				double tri_sv = CalcScalarTriProduct(su,sv,nf);
				if(CheckZero(tri_sv,HIGH_ACCURACY) || CheckZero(tri_su,HIGH_ACCURACY)) break;
				double du = CalcScalarTriProduct(deltap,sv,nf)/CalcScalarTriProduct(su,sv,nf);	// 新しい点s(u0+du,v0+dv)を与えるためのduを算出
				double dv = -CalcScalarTriProduct(deltap,su,nf)/CalcScalarTriProduct(su,sv,nf);	// 新しい点s(u0+du,v0+dv)を与えるためのdvを算出
				u0 += du;													// 新しい点のuパラメータを得る
				v0 += dv;													// 新しい点のvパラメータを得る
				if(u0 < U[0] || u0 > U[1] || v0 < V[0] || v0 > V[1]){	// 追跡点がパラメータ領域外に出た
				//	fprintf(stderr,"NURBS ERROR:曲面Rのパラメータが領域外に出ました\n");
					break;
				}
				if(deltap_dis < APPROX_ZERO_H){//CONVERG_GAP){								// Δpが収束したら
					// fprintf(stderr,"   %d:%lf,%lf\n",ansnum,u0,v0);		// debug
					ans[ansnum] = SetCoord(u0,v0,0);						// 解として登録
					ansnum++;												// 解をカウント
					//if(ansnum == ans_size)								// 解の数が制限を越えた
						//return ansnum;
					break;
				}
			}
		}
	}
	
	return ansnum;
}

// Function: CalcIntersecPtsOffsetPlaneSearch
// オフセットNURBS曲面と平面との交点群を交点追跡法にて求める
//
// Parameters:
// *nurb - NURBS曲面  
// os - オフセット量  
// pt - 平面上の1点  
// nvec - 平面の法線ベクトル  
// ds - 交線(交点群)の粗さ(密0.1～2疎)  
// initdivnum - 初期点探索の荒さ(密10～1疎)
// *ans - 解格納用配列  
// ans_size - 解の数(ansの配列長)
//
// Return:
// KOD_FALSE:NURBS曲面と平面が交差していない　KOD_ERR:特異点または発散により処理を中断
//int NURBS_Func::CalcIntersecPtsOffsetPlaneSearch(NURBSS *nurb,double os,Coord pt,Coord nvec,double ds,int initdivnum,Coord *ans,int ans_size)
int NURBSS::CalcIntersecPtsOffsetPlaneSearch(double os,Coord pt,Coord nvec,double ds,int initdivnum,Coord *ans,int ans_size)
{
	pt.dmy = os;
//	return CalcIntersecPtsPlaneSearch(nurb,pt,nvec,ds,initdivnum,ans,ans_size,CALC_OFFSET);
	return CalcIntersecPtsPlaneSearch(pt,nvec,ds,initdivnum,ans,ans_size,CALC_OFFSET);
}

// 平面とオフセットNURBS曲面との交点を補助平面を用いて数点求める
//int NURBS_Func::CalcIntersecPtsOffsetPlaneGeom(NURBSS *S,double d,Coord pt,Coord nf,int divnum,Coord *ans,int ans_size)
int NURBSS::CalcIntersecPtsOffsetPlaneGeom(double d,Coord pt,Coord nf,int divnum,Coord *ans,int ans_size)
{
	int ansnum = 0;

	for(int u=0;u<=divnum;u++){
		for(int v=0;v<=divnum;v++){
			double u0 = U[0] + (U[1] - U[0])*(double)u/(double)divnum;
			double v0 = V[0] + (V[1] - V[0])*(double)v/(double)divnum;
			for(int i=0;i<LOOPCOUNTMAX;i++){
//				Coord Su = CalcDiffuNurbsS(S,u0,v0);
//				Coord Sv = CalcDiffvNurbsS(S,u0,v0);
				Coord Su = CalcDiffuNurbsS(u0,v0);
				Coord Sv = CalcDiffvNurbsS(u0,v0);
				SFQuant sfq(this,u0,v0);					// S(u0,v0)上の曲面基本量を得る
				double H = sfq.E*sfq.G-sfq.F*sfq.F;
				double H2 = H*H;
				if(CheckZero(H,HIGH_ACCURACY) == KOD_TRUE){		// 0割り禁止
                    //GuiIF.SetMessage("NURBS KOD_ERROR:The process is stoped by detecting singular point.");
					//return KOD_ERR;		
					break;
				}
				Coord nu = AddCoord(MulCoord(Su,(sfq.M*sfq.F-sfq.L*sfq.G)/H2),MulCoord(Sv,(sfq.L*sfq.F-sfq.M*sfq.E)/H2));	// Sの法線ベクトルｎのu方向微分
				Coord nv = AddCoord(MulCoord(Su,(sfq.N*sfq.F-sfq.M*sfq.G)/H2),MulCoord(Sv,(sfq.M*sfq.F-sfq.N*sfq.E)/H2));	// Sの法線ベクトルｎのv方向微分
				Coord Su_ = AddCoord(Su,MulCoord(nu,d));	// Sのオフセット曲面S_のu方向微分
				Coord Sv_ = AddCoord(Sv,MulCoord(nv,d));	// Sのオフセット曲面S_のv方向微分
//				Coord nt = CalcNormVecOnNurbsS(S,u0,v0);
				Coord nt = CalcNormVecOnNurbsS(u0,v0);
				Coord nn = DivCoord(CalcOuterProduct(nf,nt),CalcEuclid(CalcOuterProduct(nf,nt)));	// 平面Fと平面Ftに直交する平面Fnの単位法線ベクトル
//				Coord p0 = AddCoord(CalcNurbsSCoord(S,u0,v0),MulCoord(nt,d));						// S(u0,v0)の座標値
				Coord p0 = AddCoord(CalcNurbsSCoord(u0,v0),MulCoord(nt,d));						// S(u0,v0)の座標値
				double d = nf&pt;
				double dt = nt&p0;
				double dn = nn&p0;
				Coord p1 = ((((nt&&nn)*d) + ((nn&&nf)*dt)) + ((nf&&nt)*dn))/CalcScalarTriProduct(nf,nt,nn);
				Coord dp = p1 - p0;
				double denom = CalcScalarTriProduct(Su_,Sv_,nf);
				double du = CalcScalarTriProduct(dp,Sv_,nf)/denom;
				double dv = CalcScalarTriProduct(dp,Su_,nf)/denom;
				u0 += du;
				v0 += dv;
				if(CalcEuclid(dp) < CONVERG_GAP){
					ans[ansnum].x = u0;
					ans[ansnum].y = v0;
					ansnum++;
					break;
				}
			}
		}
	}

	return ansnum;
}

// Function: CalcIntersecPtsPlaneSearch
// NURBS曲面と平面との交点群を交点追跡法にて求める
//
// Parameters:
// *nurb - NURBS曲面  
// pt - 平面上の1点  
// nvec - 平面の法線ベクトル  
// ds - 交線(交点群)の粗さ(密0.1～2疎)  
// initdivnum - 初期点探索の荒さ(密10～1疎)
// *ans - 解格納用配列  
// ans_size - 解の数(ansの配列長)
// method:交点算出時の数値解析法の選択(RUNGE_KUTTA or BULIRSH_STOER)
//
// Return:
// 返値　KOD_FALSE:NURBS曲面と平面が交差していない　KOD_ERR:特異点または発散により処理を中断
//int NURBS_Func::CalcIntersecPtsPlaneSearch(NURBSS *nurb,Coord pt,Coord nvec,double ds,int initdivnum,Coord *ans,int ans_size,int method)
int NURBSS::CalcIntersecPtsPlaneSearch(Coord pt,Coord nvec,double ds,int initdivnum,Coord *ans,int ans_size,int method)
{
	int loop_count=0;		// 収束計算のループ数
	int pcount=0;
	int anscount=0;
	Coord oldp;
	Coord newp;
	Coord init_pt[INTERSECPTNUMMAX];		// 初期点(u,vパラメータ値)
	Coord init_pt_buf[INTERSECPTNUMMAX];	// 初期点仮置きバッファ(u,vパラメータ値)
	Coord init_pt_Coord[INTERSECPTNUMMAX];	// 初期点(x,y,z座標値)
	bool  init_pt_flag[INTERSECPTNUMMAX];	// 各初期点を通り終えたかを判別するフラグ
	bool  init_allpt_flag=KOD_FALSE;			// 初期点を全て通り終えたかを判別するフラグ
	int   init_pt_num=0;						// 初期点の数
	int   init_pt_flag_count=0;
	double u,v;								// 交線追跡中のu,vパラメータ中間値
//	double dist;							// ループ脱出用に追跡点間距離を閾値とする
	int loopbreak_flag = KOD_FALSE;				// 初期点一致フラグ
	int  search_flag = KOD_TRUE;				// 交線追跡方向フラグ(KOD_TRUE:順方向,KOD_FALSE:逆方向)
	int  inverse_flag = KOD_FALSE;				// 交線追跡方向逆転フラグ
	double color[3] = {0,1,1};
//	double *hdid,*hnext;

	//FILE *fp = fopen("debug.csv","w");

	// 初期点通過判別フラグを初期化
	for(int i=0;i<INTERSECPTNUMMAX;i++){
		init_pt_flag[i] = KOD_FALSE;
	}
	init_pt_flag[0] = KOD_TRUE;

	// まず交線追跡法の初期点として交点をいくつか求める
	if(method == CALC_OFFSET) {
//		init_pt_num = CalcIntersecPtsOffsetPlaneGeom(nurb,pt.dmy,pt,nvec,initdivnum,init_pt,INTERSECPTNUMMAX);
		init_pt_num = CalcIntersecPtsOffsetPlaneGeom(pt.dmy,pt,nvec,initdivnum,init_pt,INTERSECPTNUMMAX);
	}
	else{
		// 初期点を2方向でサーチ
//		init_pt_num = CalcIntersecPtsPlaneU(nurb,pt,nvec,initdivnum,init_pt,INTERSECPTNUMMAX);
//		int num = CalcIntersecPtsPlaneV(nurb,pt,nvec,initdivnum,init_pt_buf,INTERSECPTNUMMAX);
		init_pt_num = CalcIntersecPtsPlaneU(pt,nvec,initdivnum,init_pt,INTERSECPTNUMMAX);
		int num = CalcIntersecPtsPlaneV(pt,nvec,initdivnum,init_pt_buf,INTERSECPTNUMMAX);
		init_pt_num = CatCoord(init_pt,init_pt_buf,INTERSECPTNUMMAX,init_pt_num,num);
		if(!init_pt_num) {
//			init_pt_num = CalcIntersecPtsPlaneGeom(nurb,pt,nvec,initdivnum,initdivnum,init_pt,INTERSECPTNUMMAX);	// 解が得られなかったら，サーチ法を変え再トライ
			init_pt_num = CalcIntersecPtsPlaneGeom(pt,nvec,initdivnum,initdivnum,init_pt,INTERSECPTNUMMAX);	// 解が得られなかったら，サーチ法を変え再トライ
		}
	}
	init_pt_num = CheckTheSamePoints(init_pt,init_pt_num);		// 同一点は除去する
	if(!init_pt_num){		// 見つからない場合は、交差していないとみなす
        GuiIF.SetMessage("NURBS KOD_ERROR:Init intersection point is noexistence");
		return KOD_FALSE;					
	}
	else if(init_pt_num == KOD_ERR) return KOD_ERR;			// init_pt_numがinit_ptの配列長を超えた

	for(int i=0;i<init_pt_num;i++){
//		init_pt_Coord[i] = CalcNurbsSCoord(nurb,init_pt[i].x,init_pt[i].y);		// 交点のuvパラメータをxyz座標値に変換したものを保持しておく
		init_pt_Coord[i] = CalcNurbsSCoord(init_pt[i].x,init_pt[i].y);		// 交点のuvパラメータをxyz座標値に変換したものを保持しておく
		//fprintf(stderr,"%d,%lf,%lf,%lf,%lf,%lf\n",i,init_pt[i].x,init_pt[i].y,init_pt_Coord[i].x,init_pt_Coord[i].y,init_pt_Coord[i].z);	// debug
		//DrawPoint(init_pt_Coord[i],1,3,color);	// debug
	}

	// 初期点を全て通過するまで交線追跡法を繰り返す
	while(init_allpt_flag == KOD_FALSE){
		//fprintf(stderr,"Init%d,%lf,%lf,%lf,%lf,%lf\n",pcount+1,init_pt[pcount].x,init_pt[pcount].y,init_pt_Coord[pcount].x,init_pt_Coord[pcount].y,init_pt_Coord[pcount].z);		// debug
		// 交線追跡のための始点(u,v)をセット
		u = oldp.x = init_pt[pcount].x;
		v = oldp.y = init_pt[pcount].y;
		ans[anscount] = SetCoord(init_pt[pcount]);
		anscount++;
		init_pt_flag[pcount] = KOD_TRUE;
		init_pt_flag_count++;
		//if(init_pt_flag_count == init_pt_num && init_pt_num > 1)	break;

		if(inverse_flag == KOD_TRUE){	// 逆方向への交線追跡も終了していたら
			inverse_flag = KOD_FALSE;	// 交線追跡方向を順方向に戻す
		}

		// 交線追跡開始
		loop_count = 0;
		while(loop_count < ans_size){
			// 順方向に交線追跡
			if(inverse_flag == KOD_FALSE){
//				if(method == RUNGE_KUTTA)	search_flag = SearchIntersectPt_RKM(nurb,pt,nvec,ds,&u,&v,FORWARD);	// 順方向の交点算出
//				else if(method == BULIRSH_STOER)	search_flag = SearchIntersectPt_BS(nurb,pt,nvec,ds,&u,&v,FORWARD);
//				else search_flag = SearchIntersectPt_OS(nurb,pt,nvec,ds,&u,&v,FORWARD);
				if(method == RUNGE_KUTTA)	search_flag = SearchIntersectPt_RKM(pt,nvec,ds,&u,&v,FORWARD);	// 順方向の交点算出
				else if(method == BULIRSH_STOER)	search_flag = SearchIntersectPt_BS(pt,nvec,ds,&u,&v,FORWARD);
				else search_flag = SearchIntersectPt_OS(pt,nvec,ds,&u,&v,FORWARD);
				if(search_flag == KOD_ERR){							// 順方向追跡に失敗した場合は
					inverse_flag = KOD_TRUE;						// 逆方向追跡フラグをON
					//fprintf(stderr,"a,%d,%d,%lf,%lf\n",search_flag,inverse_flag,u,v);	// for debug	
					u = init_pt[pcount].x;							// 交点追跡の初期点をセットしなおす
					v = init_pt[pcount].y;
				}
				//fprintf(stderr,"e,%d,%d,%lf,%lf\n",search_flag,inverse_flag,u,v);	// for debug
			}
			// 逆方向追跡フラグがONなら
			if(inverse_flag == KOD_TRUE){
//				if(method == RUNGE_KUTTA)	search_flag = SearchIntersectPt_RKM(nurb,pt,nvec,ds,&u,&v,INVERSE);	// 逆方向の交点算出
//				else if(method == BULIRSH_STOER)	search_flag = SearchIntersectPt_BS(nurb,pt,nvec,ds,&u,&v,INVERSE);
//				else search_flag = SearchIntersectPt_OS(nurb,pt,nvec,ds,&u,&v,INVERSE);
				if(method == RUNGE_KUTTA)	search_flag = SearchIntersectPt_RKM(pt,nvec,ds,&u,&v,INVERSE);	// 逆方向の交点算出
				else if(method == BULIRSH_STOER)	search_flag = SearchIntersectPt_BS(pt,nvec,ds,&u,&v,INVERSE);
				else search_flag = SearchIntersectPt_OS(pt,nvec,ds,&u,&v,INVERSE);
				if(search_flag == KOD_ERR){					// 特異点検出により処理を継続できない場合
					//fprintf(stderr,"b,%d,%d,%lf,%lf\n",search_flag,inverse_flag,u,v);	// for debug
					GuiIF.SetMessage("NURBS_FUNC CAUTION: Singler point was ditected.");
					break;
				}
				//fprintf(stderr,"f,%d,%d,%lf,%lf\n",search_flag,inverse_flag,u,v);	// for debug
			}

			// パラメータ範囲外の場合
			if(search_flag == KOD_FALSE){
//				newp = CalcIntersecPtsPlaneSearch_Sub(nurb,u,v,pt,nvec);		// 面から飛び出した(u,v)を参考に面のエッジ部(new_u,new_v)を得る
				newp = CalcIntersecPtsPlaneSearch_Sub(u,v,pt,nvec);		// 面から飛び出した(u,v)を参考に面のエッジ部(new_u,new_v)を得る
				//fprintf(stderr,"c,%d,%d,%.12lf,%.12lf\n",search_flag,inverse_flag,newp.x,newp.y);	// for debug
				ans[anscount] = SetCoord(newp);		// 得られたu,vを交線(交点群)として登録
				anscount++;							// 交点群の数をインクリメント
				// 初期点が交線追跡法によって全て通過したか調べる
				for(int i=0;i<init_pt_num;i++){
					if(CheckClossedPoints(oldp,newp,init_pt[i]) == KOD_TRUE){ // 新たに算出された交点と1つ前の交点を対角とする立方体の中に初期点が含まれていたら
						if(init_pt_flag[i] == KOD_FALSE){		// まだ通過していない初期点で交点もu,v範囲内だったら
							init_pt_flag[i] = KOD_TRUE;			// 通過したこととして登録
							init_pt_flag_count++;				// 通過済み初期点数をカウントアップ
							//fprintf(stderr,"%d OK!\n",i);			// debug
						}
					}
				}
				if(inverse_flag == KOD_FALSE){		// 今が順方向なら
					inverse_flag = KOD_TRUE;		// 次のサーチは逆方向にする
					u = init_pt[pcount].x;			// 交点追跡の初期点をセットしなおす
					v = init_pt[pcount].y;
					oldp = SetCoord(init_pt[pcount]);
					continue;						// 逆方向ループへ
				}
				break;								// 今が逆方向ならループ終わり
			}

			// 例外なしで解が得られた
			else{
//				Coord cd = CalcNurbsSCoord(nurb,u,v);
				Coord cd = CalcNurbsSCoord(u,v);
				//fprintf(stderr,"d,%d,%d,%.12lf,%.12lf,%lf,%lf,%lf,%d\n",search_flag,inverse_flag,u,v,cd.x,cd.y,cd.z,anscount);			// for debug
				newp.x = u;					
				newp.y = v;
			}

			// 初期点が交線追跡法によって全て通過したか調べる
			for(int i=0;i<init_pt_num;i++){
				// 新たに算出された交点と1つ前の交点を対角とする立方体の中に初期点が含まれていたら
				if(int asdf = CheckClossedPoints(oldp,newp,init_pt[i]) == KOD_TRUE){
					if(loop_count && i==pcount && inverse_flag == KOD_FALSE){	// 閉ループに対して一周して戻ってきた場合はループを抜ける
						loopbreak_flag = KOD_TRUE;	
						//fprintf(fp,"%d loop break OK\n",i);		// debug
						break;					
					}
					if(init_pt_flag[i] == KOD_FALSE && search_flag == KOD_TRUE){		// まだ通過していない初期点で交点もu,v範囲内だったら
						init_pt_flag[i] = KOD_TRUE;					// 通過したこととして登録
						init_pt_flag_count++;						// 通過済み初期点数をカウントアップ
						//fprintf(fp,"%d OK\n",i);				// debug
					}
				}
			}

			// 閉ループに対して一周してきた場合はループを抜ける
			if(loopbreak_flag == KOD_TRUE){
				loopbreak_flag = KOD_FALSE;
				break;
			}

			ans[anscount] = SetCoord(newp);	// 得られたu,vを交線(交点群)として登録
			anscount++;				// 交点群の数をインクリメント

			// 交点の数が指定サイズを超えた場合はそこまでで強制リターン
			if(anscount >= ans_size){
                GuiIF.SetMessage("NURBS KOD_ERROR:Intersection points exceeded the allocated array length");
                GuiIF.SetMessage("There is a possibility that you set large ds.");
				return anscount;
			}

			oldp = SetCoord(newp);	// このループで算出された交点は次のループでは1個前の交点となる

			loop_count++;		// ループ回数をインクリメント
		}// 交線追跡ここまで

		// 残った点があれば、別の交線があるので、その点を始点として再度交線追跡を行う
		init_allpt_flag = KOD_TRUE;
		for(int i=0;i<init_pt_num;i++){
			//fprintf(fp,"%d,",i);			// debug
			if(init_pt_flag[i] == KOD_FALSE){
				init_allpt_flag = KOD_FALSE;
				pcount = i;
				break;
			}
		}
		//fprintf(stderr,"%d:loop count:%d\n",init_allpt_flag,loop_count);	// debug
	}
//	anscount = RemoveTheSamePoints(nurb,ans,anscount);
	anscount = RemoveTheSamePoints(ans,anscount);
	//anscount = CheckTheSamePoints(ans,anscount);

	//fclose(fp);

	return anscount;
}

// Function: CheckClossedPoints
// (private)指定した点が他の2点を対角とする立方体の中に存在するかを調べる
// 
// Parameters:
// A - 対角上の1点
// B - 対角上の1点
// P - 指定点
// 
// Return:
// 存在する：KOD_TRUE,  存在しない：KOD_FALSE
//int NURBS_Func::CheckClossedPoints(Coord A,Coord B,Coord P)
int CheckClossedPoints(Coord A,Coord B,Coord P)
{
	int ap = LOW_LOW_ACCURACY;

	if((CheckMag(A.x,P.x,ap) != KOD_LARGE) && (CheckMag(P.x,B.x,ap) != KOD_LARGE) && 
		(CheckMag(A.y,P.y,ap) != KOD_LARGE) && (CheckMag(P.y,B.y,ap) != KOD_LARGE))
		return KOD_TRUE;

	else if((CheckMag(A.x,P.x,ap) != KOD_LARGE) && (CheckMag(P.x,B.x,ap) != KOD_LARGE) && 
		(CheckMag(B.y,P.y,ap) != KOD_LARGE) && (CheckMag(P.y,A.y,ap) != KOD_LARGE))
		return KOD_TRUE;

	else if((CheckMag(B.x,P.x,ap) != KOD_LARGE) && (CheckMag(P.x,A.x,ap) != KOD_LARGE) && 
		(CheckMag(A.y,P.y,ap) != KOD_LARGE) && (CheckMag(P.y,B.y,ap) != KOD_LARGE))
		return KOD_TRUE;

	else if((CheckMag(B.x,P.x,ap) != KOD_LARGE) && (CheckMag(P.x,A.x,ap) != KOD_LARGE) && 
		(CheckMag(B.y,P.y,ap) != KOD_LARGE) && (CheckMag(P.y,A.y,ap) != KOD_LARGE))
		return KOD_TRUE;


	return KOD_FALSE;
}

// Function: CalcIntersecPtsPlaneSearch_Sub
// (private)平面とNURBS曲面との交点群を求める関数CalcIntersecPtsPlaneSearch()のサブ関数．
// 面から飛び出した(u,v)を参考に面のエッジ部における交点(new_u,new_v)を得る
//
// Parameters:
// *nurb - NURBS曲面へのポインタ
// u,v - 曲面存在領域外の(u, v)値 
// pt - 平面上の1点
// nvec - 平面の法線ベクトル
// 
// Return:
// エッジ部上の交点の(u, v)座標値（Coord.xにu，Coord.yにvがそれぞれ格納される）
//Coord NURBS_Func::CalcIntersecPtsPlaneSearch_Sub(NURBSS *nurb,double u, double v,Coord pt,Coord nvec)
Coord NURBSS::CalcIntersecPtsPlaneSearch_Sub(double u, double v,Coord pt,Coord nvec)
{
	Coord old = SetCoord(u,v,0);
	Coord min;
	double a[INTERSECPTNUMMAX];
	Coord cod_a[INTERSECPTNUMMAX];
	int n;
	bool uflag = false;
	bool vflag = false;

	// どこを飛び出したか調べる
	if(u < U[0]){
		uflag = true;
		u = U[0];			// エッジをuとする
	}
	else if(u > U[1]){
		uflag = true;
		u = U[1];
	}

	if(v < V[0]){
		vflag = true;
		v = V[0];
	}
	else if(v > V[1]){
		vflag = true;
		v = V[1];
		//fprintf(stderr,"a\n");
	}

	if(uflag == true && vflag == false){
//		n = CalcIntersecIsparaCurveV(nurb,u,pt,nvec,5,a,INTERSECPTNUMMAX);	// uを固定したアイソパラ曲線に対して平面との交点を得る
		n = CalcIntersecIsparaCurveV(u,pt,nvec,5,a,INTERSECPTNUMMAX);	// uを固定したアイソパラ曲線に対して平面との交点を得る
		for(int i=0;i<n;i++)
			cod_a[i] = SetCoord(u,a[i],0);
	}
	else if(uflag == false && vflag == true){
//		n = CalcIntersecIsparaCurveU(nurb,v,pt,nvec,5,a,INTERSECPTNUMMAX);	// vを固定したアイソパラ曲線に対して平面との交点を得る
		n = CalcIntersecIsparaCurveU(v,pt,nvec,5,a,INTERSECPTNUMMAX);	// vを固定したアイソパラ曲線に対して平面との交点を得る
		for(int i=0;i<n;i++)
			cod_a[i] = SetCoord(a[i],v,0);
	}
	else if(uflag == true && vflag == true){
//		n = CalcIntersecIsparaCurveV(nurb,u,pt,nvec,5,a,INTERSECPTNUMMAX);		// uを固定したアイソパラ曲線に対して平面との交点を得る
		n = CalcIntersecIsparaCurveV(u,pt,nvec,5,a,INTERSECPTNUMMAX);		// uを固定したアイソパラ曲線に対して平面との交点を得る
		if(n > 0){
			for(int i=0;i<n;i++)
				cod_a[i] = SetCoord(u,a[i],0);
		}
		if(n <= 0){
//			n = CalcIntersecIsparaCurveU(nurb,v,pt,nvec,5,a,INTERSECPTNUMMAX);	// vを固定したアイソパラ曲線に対して平面との交点を得る
			n = CalcIntersecIsparaCurveU(v,pt,nvec,5,a,INTERSECPTNUMMAX);	// vを固定したアイソパラ曲線に対して平面との交点を得る
			for(int i=0;i<n;i++)
				cod_a[i] = SetCoord(a[i],v,0);
		}
	}

	min = GetMinDistance(old,cod_a,n);

	return min;
}

// Function: SearchIntersectPt_BS
// (private)Bulirsch-Stoer法により交点を収束させる(NURBS曲面と平面)
// 
// Parameters:
// *S - 1つ目の対象となるNURBS曲面
// pt - 平面上の1点
// nvec - 平面の法線ベクトル
// H - BS法のデフォルトの刻み幅
// *u0 - 得られた交点のuパラメータ
// *v0 - 得られた交点のvパラメータ
// direction - 追跡方向を表すフラグ（FORWARD or INVERSE)
// 
// Return:
// 収束した：KOD_TRUE, パラメータ範囲外：KOD_FALSE，失敗：KOD_ERR
//int NURBS_Func::SearchIntersectPt_BS(NURBSS *S,Coord pt,Coord nvec,double H,double *u0,double *v0,int direction)
int NURBSS::SearchIntersectPt_BS(Coord pt,Coord nvec,double H,double *u0,double *v0,int direction)
{
	// 引数指定ミス
	if(direction != FORWARD && direction != INVERSE){
		GuiIF.SetMessage("NURBS ERROR: selected wrong direction");
		return KOD_ERR;
	}

	int    n[BS_DIV] = {2,4,6,8,12,16,24,32,48,64,96};	// B-S法の分割数群を指定
	Coord  z[97];										// 修正中点法の中間値を格納(z.x = u, z.y = v)
	Coord  f;											// f.x = fu(u,v), f.y = fv(u,v)
	Coord  D[BS_DIV][BS_DIV],C[BS_DIV][BS_DIV];			// B-S法の中間パラメータ
	double h[BS_DIV];									// B-S法の刻み幅
	Coord wek,wek_;										// h=0の外挿値

	for(int lpnum=0;lpnum<4;lpnum++){

		// 各分割数における刻み幅を求めておく
		for(int i=0;i<BS_DIV;i++)
			h[i] = H/n[i];

		// 刻み幅を小さい方から順に変更しながら、B-S法による外挿値を計算していく
		for(int i=0;i<BS_DIV;i++){
			bool  divzero_flag = false;							// ゼロ割監視フラグ

			// まず、u(s+H), v(s+H)の値を修正中点法により計算する
			z[0] = SetCoord(*u0,*v0,1);										// z0とz1の算出は別処理
			if(GetSIPParam1(*u0,*v0,pt,nvec,direction,&f) == KOD_ERR){	// z0での微分方程式の右辺を計算
				break;
			}
			z[1] = AddCoord2D(z[0],MulCoord2D(f,h[i]));							// z0とz1の算出は別処理
			for(int j=1;j<n[i];j++){
				if(GetSIPParam1(z[j].x,z[j].y,pt,nvec,direction,&f) == KOD_ERR){	// zjでの微分方程式の右辺を計算
					wek = SetCoord(z[j]);
					divzero_flag = true;
					break;
				}
				z[j+1] = AddCoord2D(z[j-1],MulCoord2D(f,2*h[i]));				// z2～znまでを算出
			}
			if(divzero_flag == true)	break;						// ゼロ割になる場合はbreakし，次のステップ幅へ
			if(GetSIPParam1(z[n[i]].x,z[n[i]].y,pt,nvec,direction,&f) == KOD_ERR){	// znでの微分方程式の右辺を計算
				wek = SetCoord(z[n[i]]);
				break;
			}
			C[i][0] = DivCoord(AddCoord2D(AddCoord2D(z[n[i]],z[n[i]-1]),MulCoord2D(f,h[i])),2);		// u(s+H)
			D[i][0] = SetCoord(C[i][0]);
			if(i==0){
				wek_ = SetCoord(D[i][0]);
				continue;
			}

			// B-S法の差分表を順次求めていく
			wek = SetCoord(D[i][0]);
			for(int k=1;k<=i;k++){
				double xa = h[i-k]*h[i-k];
				double xb = h[i]*h[i];
				double x = xa/xb;
				Coord denom = SubCoord2D(MulCoord2D(D[i-k][k-1],x),C[i-k+1][k-1]);
				C[i-k][k] = DivCoord2D(MulCoord2D(MulCoord2D(D[i-k][k-1],SubCoord2D(C[i-k+1][k-1],D[i-k][k-1])),x),denom);
				D[i-k][k] = DivCoord2D(MulCoord2D(C[i-k][k-1],SubCoord2D(C[i-k+1][k-1],D[i-k][k-1])),denom);
				//fprintf(stderr,"    %lf,%lf\n",D[i-k][k-1].y,C[i-k+1][k-1].y);
				wek = AddCoord2D(wek,D[i-k][k]);
			}
			if(fabs(wek.x-wek_.x) < APPROX_ZERO_L && fabs(wek.y-wek_.y) < APPROX_ZERO_L){
				*u0 = wek.x;
				*v0 = wek.y;
				return KOD_TRUE;
			}

			wek_ = SetCoord(wek);
		}
	
		// ここまで来た場合，刻み幅Hを1/4とし再トライ
		H *= 0.25;
		
		if(lpnum==3){
			*u0 = wek.x;
			*v0 = wek.y;
		}
	}

	// ここまで来た場合，最後に算出された(*u0,*v0)が範囲外ならKOD_FALSEをリターン
	if(*u0 < U[0] || *u0 > U[1] || *v0 < V[0] || *v0 > V[1]){
		return KOD_FALSE;
	}
	// それ以外は特異点としてKOD_ERRをリターン
	return KOD_ERR;
}

// Function: GetSIPParam1
// (private)SearchIntersectPt_BS()のサブ関数．曲面と平面の交点を表す微分方程式の右辺の値を得る
//
// Parameters:
// *S - NURBS曲面へのポインタ
// u,v - 注目中のNURBS曲面パラメータ 
// pt - 平面上の一点
// nvec - 平面の法線ベクトル
// direction - 追跡方向を表すフラグ（FORWARD or INVERSE)
// *f - 計算結果 
//
// Return:
// 成功：KOD_TRUE, 失敗：KOD_ERR
//int NURBS_Func::GetSIPParam1(NURBSS *S,double u,double v,Coord pt,Coord nvec,int direction,Coord *f)
int NURBSS::GetSIPParam1(double u,double v,Coord pt,Coord nvec,int direction,Coord *f)
{
//	NURBS_Func NFunc;

//	Coord Su = CalcDiffuNurbsS(S,u,v);
//	Coord Sv = CalcDiffvNurbsS(S,u,v);
	Coord Su = CalcDiffuNurbsS(u,v);
	Coord Sv = CalcDiffvNurbsS(u,v);
	double fu = CalcInnerProduct(nvec,Su);	// nf・Su
	double fv = CalcInnerProduct(nvec,Sv);	// nf・Sv
	if(CheckZero(fu,HIGH_ACCURACY) == KOD_TRUE && CheckZero(fv,HIGH_ACCURACY) == KOD_TRUE){			// 特異点
		//GuiIF.SetMessage("NURBS KOD_ERROR:The process is stoped by detected singular point.");
		return KOD_ERR;				
	}
	double E = CalcInnerProduct(Su,Su);		// 1次規格量
	double F = CalcInnerProduct(Su,Sv);		// 1次規格量
	double G = CalcInnerProduct(Sv,Sv);		// 1次規格量
	double f_ = 1/sqrt(E*fv*fv - 2*F*fu*fv + G*fu*fu);
	*f = SetCoord(f_*fv*(double)direction, -f_*fu*(double)direction, 0);

	return KOD_TRUE;
}

// Function: SearchIntersectPt_RKM
// (private)4次のルンゲクッタ法により交点を導出(NURBS曲面と平面)
// >du(s)/ds = g(u,v),   dv(s)/ds = h(u,v)
// >u(s+delta) = u(s) + (p1+2p2+2p3+p4)/6
// >v(s+delta) = v(s) + (q1+2q2+2q3+q4)/6
// >p1 = delta*g(u,v),   q1 = delta*h(u,v)
// >p2 = delta*g(u+p1/2,v+q1/2),   q2 = delta*h(u+p1/2,v+q1/2)
// >p3 = delta*g(u+p2/2,v+q2/2),   q3 = delta*h(u+p2/2,v+q2/2)
// >p4 = delta*g(u+p3,v+q3),   q4 = delta*h(u+p3,v+q3)
// 
// Parameters:
// *S - NURBS曲面へのポインタ
// pt - 平面上の一点
// n - 平面の法線ベクトル
// delta - 解追跡の刻み幅
// *u,*v - 解
// direction - 追跡方向を表すフラグ（FORWARD or INVERSE)
// 
// Return:
// 成功：KOD_TRUE, 失敗：KOD_ERR
//int NURBS_Func::SearchIntersectPt_RKM(NURBSS *S,Coord pt,Coord n,double delta,double *u,double *v,int direction)
int NURBSS::SearchIntersectPt_RKM(Coord pt,Coord n,double delta,double *u,double *v,int direction)
{
	double u0 = *u;
	double v0 = *v;
	double p[4]={0,0,0,0};
	double q[4]={0,0,0,0};

	for(int i=0;i<4;i++){
		if(i==1 || i==2){
			*u = u0 + p[i-1]/2;
			*v = v0 + q[i-1]/2;
		}
		else if(i==3){
			*u = u0 + p[i-1];
			*v = v0 + q[i-1];
		}
		if(*u < U[0] || *u > U[1] || *v < V[0] || *v > V[1])	// パラメータ範囲外
			return KOD_FALSE;

//		Coord Su = CalcDiffuNurbsS(S,*u,*v);
//		Coord Sv = CalcDiffvNurbsS(S,*u,*v);
		Coord Su = CalcDiffuNurbsS(*u,*v);
		Coord Sv = CalcDiffvNurbsS(*u,*v);
		double fu = CalcInnerProduct(n,Su);
		double fv = CalcInnerProduct(n,Sv);
		double fuu = fu*fu;
		double fuv = fu*fv;
		double fvv = fv*fv;
		if(CheckZero(fu,LOW_ACCURACY) == KOD_TRUE && CheckZero(fv,LOW_ACCURACY) == KOD_TRUE){			// 特異点
            //GuiIF.SetMessage("NURBS KOD_ERROR:The process is stoped by detected singular point.");
			return KOD_ERR;				
		}
		double E = CalcInnerProduct(Su,Su);		// 1次規格量
		double F = CalcInnerProduct(Su,Sv);		// 1次規格量
		double G = CalcInnerProduct(Sv,Sv);		// 1次規格量
		double denom = sqrt(E*fvv - 2*F*fuv + G*fuu);
		if(CheckZero(denom,LOW_ACCURACY) == KOD_TRUE)	return KOD_ERR;		// 特異点
		double f_ = 1/denom;
		p[i] = -delta*fv*f_*(double)direction;
		q[i] = delta*fu*f_*(double)direction;
	}
	*u = u0+(p[0]+2*p[1]+2*p[2]+p[3])/6;
	*v = v0+(q[0]+2*q[1]+2*q[2]+q[3])/6;

	if(*u < U[0] || *u > U[1] || *v < V[0] || *v > V[1])	// パラメータ範囲外
		return KOD_FALSE;

	return KOD_TRUE;
}

// Function: SearchIntersectPt_OS
// (private)4次のルンゲクッタ法により交点を導出(オフセットNURBS曲面と平面)
// >du(s)/ds = g(u,v),   dv(s)/ds = h(u,v)
// >u(s+delta) = u(s) + (p1+2p2+2p3+p4)/6
// >v(s+delta) = v(s) + (q1+2q2+2q3+q4)/6
// >p1 = delta*g(u,v),   q1 = delta*h(u,v)
// >p2 = delta*g(u+p1/2,v+q1/2),   q2 = delta*h(u+p1/2,v+q1/2)
// >p3 = delta*g(u+p2/2,v+q2/2),   q3 = delta*h(u+p2/2,v+q2/2)
// >p4 = delta*g(u+p3,v+q3),   q4 = delta*h(u+p3,v+q3)
// 
// Parameters:
// *S - NURBS曲面へのポインタ
// pt - 平面上の一点
// n - 平面の法線ベクトル
// delta - 解追跡の刻み幅
// *u,*v - 解
// direction - 追跡方向を表すフラグ（FORWARD or INVERSE)
// 
// Return:
// 成功：KOD_TRUE, パラメータ範囲外：KOD_FALSE, 失敗：KOD_ERR
//int NURBS_Func::SearchIntersectPt_OS(NURBSS *S,Coord pt,Coord n,double delta,double *u,double *v,int direction)
int NURBSS::SearchIntersectPt_OS(Coord pt,Coord n,double delta,double *u,double *v,int direction)
{
	double u0 = *u;
	double v0 = *v;
	double p[4]={0,0,0,0};
	double q[4]={0,0,0,0};
	double d = pt.dmy;

	for(int i=0;i<4;i++){
		if(i==1 || i==2){
			*u = u0 + p[i-1]/2;
			*v = v0 + q[i-1]/2;
		}
		else if(i==3){
			*u = u0 + p[i-1];
			*v = v0 + q[i-1];
		}

//		Coord Su = CalcDiffuNurbsS(S,*u,*v);
//		Coord Sv = CalcDiffvNurbsS(S,*u,*v);
		Coord Su = CalcDiffuNurbsS(*u,*v);
		Coord Sv = CalcDiffvNurbsS(*u,*v);

		SFQuant sfq(this,*u,*v);
		double H = sfq.E*sfq.G-sfq.F*sfq.F;
		if(CheckZero(H,HIGH_ACCURACY) == KOD_TRUE){			// 特異点
			//GuiIF.SetMessage("NURBS KOD_ERROR:The process is stoped by detected singular point.");
			return KOD_ERR;				
		}
		Coord nu = MulCoord(Su,(sfq.N*sfq.F-sfq.M*sfq.G)/(H*H));
		Coord nv = MulCoord(Sv,(sfq.L*sfq.F-sfq.M*sfq.E)/(H*H));

		double fut = CalcInnerProduct(n,AddCoord(Su,MulCoord(nu,d)));
		double fvt = CalcInnerProduct(n,AddCoord(Sv,MulCoord(nv,d)));
		
		double fuut = fut*fut;
		double fuvt = fut*fvt;
		double fvvt = fvt*fvt;
		if(CheckZero(fut,HIGH_ACCURACY) == KOD_TRUE && CheckZero(fvt,HIGH_ACCURACY) == KOD_TRUE){			// 特異点
			//GuiIF.SetMessage("NURBS KOD_ERROR:The process is stoped by detected singular point.");
			return KOD_ERR;				
		}
//		double Kg = CalcGaussCurvature(sfq);
//		double Km = CalcMeanCurvature(sfq);
		double Kg = sfq.CalcGaussCurvature();
		double Km = sfq.CalcMeanCurvature();
		double nunu = -Kg*sfq.E+2*Km*sfq.L;
		double nunv = -Kg*sfq.G+2*Km*sfq.N;
		double nvnv = -Kg*sfq.F+2*Km*sfq.M;
		double Et = sfq.E-2*sfq.L*d+nunu*d*d;		// 1次規格量
		double Ft = sfq.F-2*sfq.M*d+nunv*d*d;		// 1次規格量
		double Gt = sfq.G-2*sfq.N*d+nvnv*d*d;		// 1次規格量
		double denom = Et*fvvt - 2*Ft*fuvt + Gt*fuut;
		if(denom <= 0)
			return KOD_ERR;
		double gt_ = 1/sqrt(denom);
		p[i] = -delta*fvt*gt_*(double)direction;
		q[i] = delta*fut*gt_*(double)direction;
	}
	*u = u0+(p[0]+2*p[1]+2*p[2]+p[3])/6;
	*v = v0+(q[0]+2*q[1]+2*q[2]+q[3])/6;
	
	if(*u < U[0] || *u > U[1] || *v < V[0] || *v > V[1])	// パラメータ範囲外
		return KOD_FALSE;

	return KOD_TRUE;
}

// Function: SearchIntersectPt
// (private)ニュートン法により交点を真値に収束させる(NURBS曲面と平面)
// 
// Parameters:
// *nurb - NURBS曲面へのポインタ
// pt - 平面上の一点
// nvec - 平面の法線ベクトル
// ds - 解追跡の刻み幅
// *u,*v - 解
// direction - 追跡方向を表すフラグ（FORWARD or INVERSE)
//
// Return:
// 成功：KOD_TURE, パラメータ範囲外：KOD_FALSE, 失敗(特異点につきゼロ割)：KOD_ERR
//int NURBS_Func::SearchIntersectPt(NURBSS *nurb,Coord pt,Coord nvec,double ds,double *u,double *v,int direction)
int NURBSS::SearchIntersectPt(Coord pt,Coord nvec,double ds,double *u,double *v,int direction)
{
	double d = CalcInnerProduct(pt,nvec);	// 原点から平面までの距離

	// まず初期値としてのdu,dvを求める
//	Coord pu = CalcDiffuNurbsS(nurb,*u,*v);
//	Coord pv = CalcDiffvNurbsS(nurb,*u,*v);
//	double phi = CalcInnerProduct(nvec,CalcNurbsSCoord(nurb,*u,*v));
	Coord pu = CalcDiffuNurbsS(*u,*v);
	Coord pv = CalcDiffvNurbsS(*u,*v);
	double phi = CalcInnerProduct(nvec,CalcNurbsSCoord(*u,*v));
	double phi_u = CalcInnerProduct(nvec,pu);
	double phi_v = CalcInnerProduct(nvec,pv);
	double E = CalcInnerProduct(pu,pu);
	double F = CalcInnerProduct(pu,pv);
	double G = CalcInnerProduct(pv,pv);
	double f = sqrt(E*phi_v*phi_v - 2*F*phi_u*phi_v + G*phi_u*phi_u); 
	//fprintf(stderr,"%lf , %lf\n",phi_u,phi_v);
	if(CheckZero(phi_u,MID_ACCURACY) == KOD_TRUE && CheckZero(phi_v,MID_ACCURACY) == KOD_TRUE){			// 特異点
        //GuiIF.SetMessage("NURBS KOD_ERROR:The process is stoped by detected singular point.");
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
//			phi = CalcInnerProduct(nvec,CalcNurbsSCoord(nurb,*u,*v));
//			phi_u = CalcInnerProduct(nvec,CalcDiffuNurbsS(nurb,*u,*v));
//			phi_v = CalcInnerProduct(nvec,CalcDiffvNurbsS(nurb,*u,*v));
			phi = CalcInnerProduct(nvec,CalcNurbsSCoord(*u,*v));
			phi_u = CalcInnerProduct(nvec,CalcDiffuNurbsS(*u,*v));
			phi_v = CalcInnerProduct(nvec,CalcDiffvNurbsS(*u,*v));
			du = (d-phi-phi_v*dv)/phi_u;
			*u += du;
			if(!CheckRange(U[0],U[1],*u,0) || k > LOOPCOUNTMAX){
                //GuiIF.SetMessage("NURBS KOD_ERROR:fail to calculate convergence");
				return KOD_FALSE;
			}
			k++;
		}
		*v += dv;
		if(!CheckRange(V[0],V[1],*v,0)){
			return KOD_FALSE;
		}
	}
	else{									// dv<duの場合はduを定数として固定する
		while(!CheckZero(dv,MID_ACCURACY)){		// dvが収束するまで繰り返し計算
//			phi = CalcInnerProduct(nvec,CalcNurbsSCoord(nurb,*u,*v));
//			phi_u = CalcInnerProduct(nvec,CalcDiffuNurbsS(nurb,*u,*v));
//			phi_v = CalcInnerProduct(nvec,CalcDiffvNurbsS(nurb,*u,*v));
			phi = CalcInnerProduct(nvec,CalcNurbsSCoord(*u,*v));
			phi_u = CalcInnerProduct(nvec,CalcDiffuNurbsS(*u,*v));
			phi_v = CalcInnerProduct(nvec,CalcDiffvNurbsS(*u,*v));
			dv = (d-phi-phi_u*du)/phi_v;
			*v += dv;
			if(!CheckRange(V[0],V[1],*v,0) || k>LOOPCOUNTMAX){
                //GuiIF.SetMessage("NURBS KOD_ERROR:fail to calculate convergence");
				return KOD_FALSE;
			}
			k++;
		}
		*u += du;
		if(!CheckRange(U[0],U[1],*u,0))
			return KOD_FALSE;
	}
	return KOD_TRUE;
}

// Function: CalcIntersecPtsNurbsSNurbsC
// NURBS曲面とNURBS曲線との交点群を交線追跡法で求める
//
// Parameters:
// *NurbsS, *NurbsC - NURBS曲面とNURBS曲線へのポインタ   
// Divnum - 初期点サーチ時の曲線分割数   
// *ans - 解  
// ans_size - ans配列の配列長
//
// Return:
// 交点の数（解の数がansのサイズを超えた場合：KOD_ERR）
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
//			F = SubCoord(CalcNurbsSCoord(NurbsS,u,v),CalcNurbsCCoord(NurbsC,t));	// F(u,v,t) = S(u,v) - C(t)
//			Fu = CalcDiffuNurbsS(NurbsS,u,v);			// Fu = dF/du = dS/du
//			Fv = CalcDiffvNurbsS(NurbsS,u,v);			// Fv = dF/dv = dS/dv
//			Ft = CalcDiffNurbsC(NurbsC,t);				// Ft = dF/dt = dC/dt
			F = SubCoord(NurbsS->CalcNurbsSCoord(u,v),NurbsC->CalcNurbsCCoord(t));	// F(u,v,t) = S(u,v) - C(t)
			Fu = NurbsS->CalcDiffuNurbsS(u,v);			// Fu = dF/du = dS/du
			Fv = NurbsS->CalcDiffvNurbsS(u,v);			// Fv = dF/dv = dS/dv
			Ft = NurbsC->CalcDiffNurbsC(t);				// Ft = dF/dt = dC/dt
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
                GuiIF.SetMessage("NURBS_Func ERROR: Ans_size overflow");
				return KOD_ERR;
			}
		}
	}// end of i loop

	FreeMatrix(A,3);
	FreeMatrix(A_,3);

	anscount = CheckTheSamePoints(ans,anscount);		// 同一点は除去する

	return anscount;
}

// Function: CalcIntersecPtsNurbsSSearch
// NURBS曲面S(u,v)とNURBS曲面R(w,t)の交線(交点群)を交点追跡法にて求める
// 
// Parameters:
// nurbsS - NURBS曲面S(u,v) 
// nurbsR - NURBS曲面R(w,t) 
// div - 初期点サーチ時の曲面分割数  
// ds - 交線(交点群)の粗さ(密0.1～2疎)  
// ans - 解  
// ans_size - ans配列の配列長
//
// Return:
// 交点の数（NURBS曲面同士が交差していない：KOD_FALSE，特異点または発散により処理を中断：KOD_ERR）
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
//		init_pt_Coord_R[i] = CalcNurbsSCoord(nurbR,init_pt_R[i].x,init_pt_R[i].y);		// 交点のuvパラメータをxyz座標値に変換したものを保持しておく
//		init_pt_Coord_S[i] = CalcNurbsSCoord(nurbS,init_pt_S[i].x,init_pt_S[i].y);		// 交点のuvパラメータをxyz座標値に変換したものを保持しておく
		init_pt_Coord_R[i] = nurbR->CalcNurbsSCoord(init_pt_R[i].x,init_pt_R[i].y);		// 交点のuvパラメータをxyz座標値に変換したものを保持しておく
		init_pt_Coord_S[i] = nurbS->CalcNurbsSCoord(init_pt_S[i].x,init_pt_S[i].y);		// 交点のuvパラメータをxyz座標値に変換したものを保持しておく
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

//			Coord pr = CalcNurbsSCoord(nurbR,w,t);			// 得られたu,vをxyz座標値に変換
//			Coord ps = CalcNurbsSCoord(nurbS,u,v);			// 得られたu,vをxyz座標値に変換
			Coord pr = nurbR->CalcNurbsSCoord(w,t);			// 得られたu,vをxyz座標値に変換
			Coord ps = nurbS->CalcNurbsSCoord(u,v);			// 得られたu,vをxyz座標値に変換
			double distr = CalcDistance(init_pt_Coord_R[pnow],pr);	// 得られたxyz座標値と初期点との距離を算出
			double dists = CalcDistance(init_pt_Coord_S[pnow],ps);	// 得られたxyz座標値と初期点との距離を算出
			
			// 交点の個数がリミットを越えたら
			if(ans_count >= ans_size-1){
                GuiIF.SetMessage("NURBS KOD_ERROR:Intersection points exceeded the allocated array length");
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
			
			// u,vが取り得るパラメータ範囲（0～1）を超えた場合または、１周して戻ってきた場合はループを抜ける
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

// Function: CalcIntersecPtsNurbsSGeom
// NURBS曲面S(u,v)とNURBS曲面R(w,t)の交線(交点群)を幾何学的に求める(補助平面を用いた解法)
//
// Parameters:
// *nurbS - NURBS曲面S(u,v) 
// *nurbR - NURBS曲面R(w,t) 
// u_divnum - uパラメータ分割数　
// v_divnum - vパラメータ分割数
// *ans - 算出された交点のu,vパラメータ値をそれぞれans.x,ans.yに格納
// ans_size - ansの配列長
//
// Return:
// 交点の個数
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
//						Coord p0 = CalcNurbsSCoord(nurbR,w0,t0);					// R(w0,t0)となる点(初期点)の座標
//						Coord q0 = CalcNurbsSCoord(nurbS,u0,v0);					// S(u0,v0)となる点(初期点)の座標
//						Coord rw = CalcDiffuNurbsS(nurbR,w0,t0);					// 点R(w0,t0)のu偏微分(基本ベクトル)
//						Coord rt = CalcDiffvNurbsS(nurbR,w0,t0);					// 点R(w0,t0)のv偏微分(基本ベクトル)
						Coord p0 = nurbR->CalcNurbsSCoord(w0,t0);					// R(w0,t0)となる点(初期点)の座標
						Coord q0 = nurbS->CalcNurbsSCoord(u0,v0);					// S(u0,v0)となる点(初期点)の座標
						Coord rw = nurbR->CalcDiffuNurbsS(w0,t0);					// 点R(w0,t0)のu偏微分(基本ベクトル)
						Coord rt = nurbR->CalcDiffvNurbsS(w0,t0);					// 点R(w0,t0)のv偏微分(基本ベクトル)
						double rwt = CalcEuclid(CalcOuterProduct(rw,rt));
						if(rwt==0.0) break;
						Coord np = DivCoord(CalcOuterProduct(rw,rt),rwt);			// 点R(w0,t0)の単位法線ベクトル
//						Coord su = CalcDiffuNurbsS(nurbS,u0,v0);					// 点S(u0,v0)のu偏微分(基本ベクトル)
//						Coord sv = CalcDiffvNurbsS(nurbS,u0,v0);					// 点S(u0,v0)のv偏微分(基本ベクトル)
						Coord su = nurbS->CalcDiffuNurbsS(u0,v0);					// 点S(u0,v0)のu偏微分(基本ベクトル)
						Coord sv = nurbS->CalcDiffvNurbsS(u0,v0);					// 点S(u0,v0)のv偏微分(基本ベクトル)
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

// Function: SearchIntersectPt
// (private)ニュートン法により交点を真値に収束させる(NURBS曲面同士)
// 
// Parameters:
// *nurbR - NURBS曲面S(u,v)
// *nurbS - NURBS曲面R(w,t) 
// ds - 解追跡時の刻み幅
// *w,*t,*u,*v - 解
// direction - 追跡方向を表すフラグ（FORWARD or INVERSE)
//
// Return:
// 収束した：KOD_TRUE, パラメータ範囲外：KOD_FALSE, 特異点検出：KOD_ERR
int NURBS_Func::SearchIntersectPt(NURBSS *nurbR,NURBSS *nurbS,double ds,double *w,double *t,double *u,double *v,int direction)
{
	double **J;
	double D[3];
	double ans[3];
	int flag = KOD_TRUE;

	// メモリ確保
	if((J = NewMatrix(3,3)) == NULL){
        GuiIF.SetMessage("NURBS ERROR: fail to malloc\n");
		return KOD_ERR;
	}

	// まず初期値としてのdw,dt,du,dvを求める
//	Coord r = CalcNurbsSCoord(nurbR,*w,*t);					// 点R(w,t)のNURBS曲面の座標値を求める
//	Coord s = CalcNurbsSCoord(nurbS,*u,*v);					// 点S(u,v)のNURBS曲面の座標値を求める
//	Coord rw = CalcDiffuNurbsS(nurbR,*w,*t);					// 点R(w,t)のu偏微分(基本ベクトル)
//	Coord rt = CalcDiffvNurbsS(nurbR,*w,*t);					// 点R(w,t)のv偏微分(基本ベクトル)
//	Coord su = CalcDiffuNurbsS(nurbS,*u,*v);					// 点S(u,v)のu偏微分(基本ベクトル)
//	Coord sv = CalcDiffvNurbsS(nurbS,*u,*v);					// 点S(u,v)のv偏微分(基本ベクトル)
	Coord r = nurbR->CalcNurbsSCoord(*w,*t);					// 点R(w,t)のNURBS曲面の座標値を求める
	Coord s = nurbS->CalcNurbsSCoord(*u,*v);					// 点S(u,v)のNURBS曲面の座標値を求める
	Coord rw = nurbR->CalcDiffuNurbsS(*w,*t);					// 点R(w,t)のu偏微分(基本ベクトル)
	Coord rt = nurbR->CalcDiffvNurbsS(*w,*t);					// 点R(w,t)のv偏微分(基本ベクトル)
	Coord su = nurbS->CalcDiffuNurbsS(*u,*v);					// 点S(u,v)のu偏微分(基本ベクトル)
	Coord sv = nurbS->CalcDiffvNurbsS(*u,*v);					// 点S(u,v)のv偏微分(基本ベクトル)
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
        GuiIF.SetMessage("NURBS KOD_ERROR:The process is stoped by detected singular point.");
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
//			r = CalcNurbsSCoord(nurbR,*w,*t);						// 点R(w,t)のNURBS曲面の座標値を求める
//			s = CalcNurbsSCoord(nurbS,*u,*v);						// 点S(u,v)のNURBS曲面の座標値を求める
//			rw = CalcDiffuNurbsS(nurbR,*w,*t);						// 点R(w,t)のu偏微分(基本ベクトル)
//			rt = CalcDiffvNurbsS(nurbR,*w,*t);						// 点R(w,t)のv偏微分(基本ベクトル)
//			su = CalcDiffuNurbsS(nurbS,*u,*v);						// 点S(u,v)のu偏微分(基本ベクトル)
//			sv = CalcDiffvNurbsS(nurbS,*u,*v);						// 点S(u,v)のv偏微分(基本ベクトル)
			r = nurbR->CalcNurbsSCoord(*w,*t);						// 点R(w,t)のNURBS曲面の座標値を求める
			s = nurbS->CalcNurbsSCoord(*u,*v);						// 点S(u,v)のNURBS曲面の座標値を求める
			rw = nurbR->CalcDiffuNurbsS(*w,*t);						// 点R(w,t)のu偏微分(基本ベクトル)
			rt = nurbR->CalcDiffvNurbsS(*w,*t);						// 点R(w,t)のv偏微分(基本ベクトル)
			su = nurbS->CalcDiffuNurbsS(*u,*v);						// 点S(u,v)のu偏微分(基本ベクトル)
			sv = nurbS->CalcDiffvNurbsS(*u,*v);						// 点S(u,v)のv偏微分(基本ベクトル)
			
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
//			r = CalcNurbsSCoord(nurbR,*w,*t);					// 点R(w,t)のNURBS曲面の座標値を求める
//			s = CalcNurbsSCoord(nurbS,*u,*v);					// 点S(u,v)のNURBS曲面の座標値を求める
//			rw = CalcDiffuNurbsS(nurbR,*w,*t);					// 点R(w,t)のu偏微分(基本ベクトル)
//			rt = CalcDiffvNurbsS(nurbR,*w,*t);					// 点R(w,t)のv偏微分(基本ベクトル)
//			su = CalcDiffuNurbsS(nurbS,*u,*v);					// 点S(u,v)のu偏微分(基本ベクトル)
//			sv = CalcDiffvNurbsS(nurbS,*u,*v);					// 点S(u,v)のv偏微分(基本ベクトル)
			r = nurbR->CalcNurbsSCoord(*w,*t);					// 点R(w,t)のNURBS曲面の座標値を求める
			s = nurbS->CalcNurbsSCoord(*u,*v);					// 点S(u,v)のNURBS曲面の座標値を求める
			rw = nurbR->CalcDiffuNurbsS(*w,*t);					// 点R(w,t)のu偏微分(基本ベクトル)
			rt = nurbR->CalcDiffvNurbsS(*w,*t);					// 点R(w,t)のv偏微分(基本ベクトル)
			su = nurbS->CalcDiffuNurbsS(*u,*v);					// 点S(u,v)のu偏微分(基本ベクトル)
			sv = nurbS->CalcDiffvNurbsS(*u,*v);					// 点S(u,v)のv偏微分(基本ベクトル)
			
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
//			r = CalcNurbsSCoord(nurbR,*w,*t);					// 点R(w,t)のNURBS曲面の座標値を求める
//			s = CalcNurbsSCoord(nurbS,*u,*v);					// 点S(u,v)のNURBS曲面の座標値を求める
//			rw = CalcDiffuNurbsS(nurbR,*w,*t);					// 点R(w,t)のu偏微分(基本ベクトル)
//			rt = CalcDiffvNurbsS(nurbR,*w,*t);					// 点R(w,t)のv偏微分(基本ベクトル)
//			su = CalcDiffuNurbsS(nurbS,*u,*v);					// 点S(u,v)のu偏微分(基本ベクトル)
//			sv = CalcDiffvNurbsS(nurbS,*u,*v);					// 点S(u,v)のv偏微分(基本ベクトル)
			r = nurbR->CalcNurbsSCoord(*w,*t);					// 点R(w,t)のNURBS曲面の座標値を求める
			s = nurbS->CalcNurbsSCoord(*u,*v);					// 点S(u,v)のNURBS曲面の座標値を求める
			rw = nurbR->CalcDiffuNurbsS(*w,*t);					// 点R(w,t)のu偏微分(基本ベクトル)
			rt = nurbR->CalcDiffvNurbsS(*w,*t);					// 点R(w,t)のv偏微分(基本ベクトル)
			su = nurbS->CalcDiffuNurbsS(*u,*v);					// 点S(u,v)のu偏微分(基本ベクトル)
			sv = nurbS->CalcDiffvNurbsS(*u,*v);					// 点S(u,v)のv偏微分(基本ベクトル)
			
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
//			r = CalcNurbsSCoord(nurbR,*w,*t);					// 点R(w,t)のNURBS曲面の座標値を求める
//			s = CalcNurbsSCoord(nurbS,*u,*v);					// 点S(u,v)のNURBS曲面の座標値を求める
//			rw = CalcDiffuNurbsS(nurbR,*w,*t);					// 点R(w,t)のu偏微分(基本ベクトル)
//			rt = CalcDiffvNurbsS(nurbR,*w,*t);					// 点R(w,t)のv偏微分(基本ベクトル)
//			su = CalcDiffuNurbsS(nurbS,*u,*v);					// 点S(u,v)のu偏微分(基本ベクトル)
//			sv = CalcDiffvNurbsS(nurbS,*u,*v);					// 点S(u,v)のv偏微分(基本ベクトル)
			r = nurbR->CalcNurbsSCoord(*w,*t);					// 点R(w,t)のNURBS曲面の座標値を求める
			s = nurbS->CalcNurbsSCoord(*u,*v);					// 点S(u,v)のNURBS曲面の座標値を求める
			rw = nurbR->CalcDiffuNurbsS(*w,*t);					// 点R(w,t)のu偏微分(基本ベクトル)
			rt = nurbR->CalcDiffvNurbsS(*w,*t);					// 点R(w,t)のv偏微分(基本ベクトル)
			su = nurbS->CalcDiffuNurbsS(*u,*v);					// 点S(u,v)のu偏微分(基本ベクトル)
			sv = nurbS->CalcDiffvNurbsS(*u,*v);					// 点S(u,v)のv偏微分(基本ベクトル)
			
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



// Function: GetNurbsSCoef
// (private)CalcIntersecPtsPlaneU/V3()のサブ関数．NURBS曲線C(u) or C(v)の係数を求める
//
// Parameters:
// M - 階数
// **coef - Bスプライン基底関数の係数 
// *a,*b - u/vを固定した時のNURBS曲線C(v)/C(u)の分母/分子の係数 
// i - 曲線の番号
// *P, *Q - 固定されたパラメータにおけるNURBS曲面の係数(P,Q) 
//void NURBS_Func::GetNurbsSCoef(int M,double **coef,double *a,Coord *b,int i,Coord *P,double *Q)
void GetNurbsSCoef(int M,double **coef,double *a,Coord *b,int i,Coord *P,double *Q)
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

// Function: CalcIntersecPtsNurbsCNurbsCParam
// (x,y)平面上のNURBS曲線同士の交点を求める(ニュートン法)
// 
// Parameters:
// *NurbA, *NurbB - NURBS曲線
// Divnum - 初期値設定用の分割数     
// *ans - 交点格納用配列   
// ans_size - ansの配列長
// 
// Return:
// 解の個数（ans_sizeを超えたら，KOD_ERR）
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
//			F = SubCoord(CalcNurbsCCoord(NurbA,t),CalcNurbsCCoord(NurbB,u));
//			Ft = CalcDiffNurbsC(NurbA,t);
//			Fu = CalcDiffNurbsC(NurbB,u);
			F = SubCoord(NurbA->CalcNurbsCCoord(t),NurbB->CalcNurbsCCoord(u));
			Ft = NurbA->CalcDiffNurbsC(t);
			Fu = NurbB->CalcDiffNurbsC(u);
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
                GuiIF.SetMessage("NURBS_Func ERROR: Ans_size overflow");
				return KOD_ERR;
			}
		}
	}// end of i loop

	anscount = CheckTheSamePoints2D(ans,anscount);		// 同一点は除去する

	FreeMatrix(A,2);
	FreeMatrix(A_,2);

	return anscount;
}

// Function: ClacIntersecPtsNurbsCLine
// 2次元NURBS曲線と直線との交点を求める
//
// Parameters:
// *C - NURBS曲線
// P - 直線上の1点
// r - 直線の方向ベクトル
// *t1 - 交点におけるNURBS曲線パラメータ
// *t2 - 交点における直線パラメータ
//
// return:
// 交点の有無（KOD_TRUE：交点あり， KOD_FALSE：交点なし）
//int NURBS_Func::ClacIntersecPtsNurbsCLine(NURBSC *C, Coord P, Coord r, double *t1,double *t2)
int NURBSC::ClacIntersecPtsNurbsCLine(Coord P, Coord r, double *t1,double *t2)
{
    Matrix A = NewMatrix(2,2);
    Matrix A_ = NewMatrix(2,2);
    bool conv_flag = false;

    *t1 = (V[0]+V[1])/2;
    *t2 = 0;

    while(1){
//      Coord Ct = CalcDiffNurbsC(C,*t1);
        Coord Ct = CalcDiffNurbsC(*t1);
        Coord Lt = SetCoord(r);
//      Coord B = SubCoord(AddCoord(P,MulCoord(r,*t2)),CalcNurbsCCoord(C,*t1));
        Coord B = SubCoord(AddCoord(P,MulCoord(r,*t2)),CalcNurbsCCoord(*t1));
        A[0][0] = Ct.x;
        A[1][0] = Ct.y;
        A[0][1] = -Lt.x;
        A[1][1] = -Lt.y;
        double det = MatInv2(A,A_);
        if(det == 0) break;
        double dt1 = A_[0][0]*B.x + A_[0][1]*B.y;
        double dt2 = A_[1][0]*B.x + A_[1][1]*B.y;
        //fprintf(stderr,"    %lf,%lf,%lf,%lf,%lf\n",*t1,*t2,dt1,dt2,det);		// fro debug
        if(CheckZero(dt1,LOW_ACCURACY) == KOD_TRUE && CheckZero(dt2,LOW_ACCURACY) == KOD_TRUE){		// 交点に収束したらwhile break
            conv_flag = true;
            break;
        }
        else{
            *t1 += dt1/3;
            *t2 += dt2/3;
        }
        if(*t1 < V[0] || *t1 > V[1])	// 現在注目中のエッジの範囲を超えたらbreak
            break;
    }

    FreeMatrix(A,2);
    FreeMatrix(A_,2);

    if(conv_flag == true)
        return KOD_TRUE;
    else
        return KOD_ERR;
}

// Function: ClacIntersecPtsNurbsCLineSeg
// 2次元NURBS曲線と線分との交点を求める
//
// Parameters:
// *C - NURBS曲線
// P - 線分上の1点
// r - 線分の方向ベクトル
// ts - 線分の始点パラメータ
// te - 線分の終点パラメータ
// *t1 - 交点におけるNURBS曲線パラメータ
// *t2 - 交点における直線パラメータ
//
// return:
// 交点の有無（KOD_TRUE：交点あり， KOD_FALSE：交点なし）
//int NURBS_Func::ClacIntersecPtsNurbsCLineSeg(NURBSC *C, Coord P, Coord r, double ts, double te, double *t1,double *t2)
int NURBSC::ClacIntersecPtsNurbsCLineSeg(Coord P, Coord r, double ts, double te, double *t1,double *t2)
{
//	if(ClacIntersecPtsNurbsCLine(C,P,r,t1,t2) == KOD_TRUE){
	if(ClacIntersecPtsNurbsCLine(P,r,t1,t2) == KOD_TRUE){
        if(*t2 >= ts && *t2 <= te){
            return KOD_TRUE;
        }
        else{
            return KOD_FALSE;
        }
    }

    return KOD_FALSE;
}

// Function: CalcIntersecCurve
// NURBS曲線と平面との交点を求める(ニュートン法)
// F(t) = nvec・(C(t)-pt) = 0をニュートン法を用いて求める．
// 交点は最大で(M-1)*(K-M+1)点得られる.  (例：4階でコントロールポイントの数8個の場合、(4-1)*(8-4+1)=15点)．
// よって引数*ansは(M-1)*(K-M+1)個の配列を用意することが望ましい.
//
// Parameters:
// *nurb - NURBS曲線  
// pt - 平面上の一点  
// nvec - 平面の法線ベクトル   
// Divnum - NURBS曲線のパラメータ分割数  
// *ans - 算出された交点のtパラメータ値を格納
// ans_size - ansの配列長
// LoD - 詳細度（ニュートン法の更新パラメータを足しこむときに，LoDで割ることで，急激なパラメータ変更を避ける．通常は1でよいが，解が得られない場合は値を大きくする．2とか3とか）
//
// Return:
// 交点の個数（KOD_ERR：交点の数がans_sizeを超えた）
//int NURBS_Func::CalcIntersecCurve(NURBSC *nurb,Coord pt,Coord nvec,int Divnum,double *ans,int ans_size,int LoD)
int NURBSC::CalcIntersecCurve(Coord pt,Coord nvec,int Divnum,double *ans,int ans_size,int LoD)
{
	double t = V[0];		// 現在のNURBS曲線のパラメータ値
	double d = 0;				// ニュートン法によるパラメータの更新量
	double F;					// ニュートン法の対象とする方程式
	double Ft;					// Fのtによる微分値
	double dt = (V[1] - V[0])/(double)Divnum;	// 初期点の増分値
	int loopcount = 0;			// ループ回数
	bool flag = false;			// 収束フラグ
	int anscount = 0;			// 交点の数をカウント

	if(!LoD){
		GuiIF.SetMessage("NURBS_Func ERROR: LoD is changed 0 to 1");
		LoD = 1;
	}

	for(int i=0;i<=Divnum;i++){
		flag = false;
		loopcount = 0;
		t = V[0] + (double)i*dt;		// 初期値更新
		while(loopcount < LOOPCOUNTMAX){
//			F = CalcInnerProduct(nvec,SubCoord(CalcNurbsCCoord(nurb,t),pt));
//			Ft = CalcInnerProduct(nvec,CalcDiffNurbsC(nurb,t));
			F = CalcInnerProduct(nvec,SubCoord(CalcNurbsCCoord(t),pt));
			Ft = CalcInnerProduct(nvec,CalcDiffNurbsC(t));
			d = -F/Ft;		// 更新値
			//fprintf(stderr,"   %d:%.14lf,%lf\n",i,d,t);	// for debug
			if(CheckZero(d,HIGH_ACCURACY) == KOD_TRUE){		// 更新値が閾値以下になったら、whileを抜け、解として登録
				flag = true;
				break;
			}
			t += d/(double)LoD;		// パラメータ値更新
			
			if(t < V[0] || t > V[1]){		// パラメータ範囲を超えたら、whileを抜け、次の初期値へ移行
				flag = false;
				break;
			}
			loopcount++;
		}// end of wihle
		if(flag == true){
			if(anscount == ans_size){	// 解の個数がans_sizeを超えたら、ERRをリターン
				GuiIF.SetMessage("NURBS_Func ERROR: Ans_size overflow");
				return KOD_ERR;
			}
			ans[anscount] = t;		// 解として登録
			anscount++;
		}
	}// end of i loop

	anscount = CheckTheSamePoints(ans,anscount);		// 同一点は除去する

	return anscount;
}

// Function: CalcIntersecIsparaCurveU
// NURBS曲面のU方向アイソパラ曲線(Vパラメータを固定)と平面との交点を求める(ニュートン法)
// F(t) = nvec・(C(t)-pt) = 0をニュートン法を用いて求める
// 交点は最大で(M-1)*(K-M+1)点得られる.  (例：4階でコントロールポイントの数8個の場合、(4-1)*(8-4+1)=15点)
// よって引数*ansは(M-1)*(K-M+1)個の配列を用意することが望ましい.
//
// Parameters:
// *nurb - NURBS曲面  
// V - vの固定値  
// pt - 平面上の一点  
// nvec - 平面の法線ベクトル  
// Divnum - NURBS曲線のパラメータ分割数  
// *ans - 算出された交点のtパラメータ値を格納  
// ans_size - ansの配列長
//
// Return:
// 交点の個数（KOD_ERR:交点の数がans_sizeを超えた）
//int NURBS_Func::CalcIntersecIsparaCurveU(NURBSS *nurb,double V,Coord pt,Coord nvec,int Divnum,double *ans,int ans_size)
int NURBSS::CalcIntersecIsparaCurveU(double V,Coord pt,Coord nvec,int Divnum,double *ans,int ans_size)
{
	double d = 0;				// ニュートン法によるパラメータの更新量
	double F;					// ニュートン法の対象とする方程式
	double Fu;					// Fのuによる微分値
	int loopcount = 0;			// ループ回数
	bool flag = false;			// 収束フラグ
	int anscount = 0;			// 交点の数をカウント
	double u = U[0];		// 現在のNURBS曲線のパラメータ値
	double du = (U[1] - U[0])/(double)Divnum;	// 初期点の増分値

	for(int i=0;i<=Divnum;i++){
		flag = false;
		loopcount = 0;
		u = U[0] + (double)i*du;		// 初期値更新
		while(loopcount < LOOPCOUNTMAX){
//			F = CalcInnerProduct(nvec,SubCoord(CalcNurbsSCoord(nurb,u,V),pt));
//			Fu = CalcInnerProduct(nvec,CalcDiffuNurbsS(nurb,u,V));
			F = CalcInnerProduct(nvec,SubCoord(CalcNurbsSCoord(u,V),pt));
			Fu = CalcInnerProduct(nvec,CalcDiffuNurbsS(u,V));
			if(CheckZero(Fu,MID_ACCURACY) == KOD_TRUE)	break;
			d = -F/Fu;		// 更新値
			if(CheckZero(d,MID_ACCURACY) == KOD_TRUE){		// 更新値が閾値以下になったら、whileを抜け、解として登録
				flag = true;
				break;
			}
			u += d;		// パラメータ値更新
			if(u < U[0] || u > U[1]){		// パラメータ範囲を超えたら、whileを抜け、次の初期値へ移行
				flag = false;
				break;
			}
			loopcount++;
		}// end of wihle
		if(flag == true){
			anscount = CheckTheSamePoints(ans,anscount);		// 同一点は除去する
			if(anscount == ans_size){	// 解の個数がans_sizeを超えたら、ERRをリターン
				GuiIF.SetMessage("NURBS_Func ERROR: Ans_size overflow");
				return KOD_ERR;
			}
			ans[anscount] = u;		// 解として登録
			anscount++;
		}
	}// end of i loop

	anscount = CheckTheSamePoints(ans,anscount);		// 同一点は除去する

	return anscount;
}

// Function: CalcIntersecIsparaCurveV
// NURBS曲面のV方向アイソパラ曲線(Uパラメータを固定)と平面との交点を求める(ニュートン法)
// F(t) = nvec・(C(t)-pt) = 0をニュートン法を用いて求める
// 交点は最大で(M-1)*(K-M+1)点得られる.  (例：4階でコントロールポイントの数8個の場合、(4-1)*(8-4+1)=15点)
// よって引数*ansは(M-1)*(K-M+1)個の配列を用意することが望ましい.
//
// Parameters:
// *nurb - NURBS曲面  
// U - uの固定値   
// pt - 平面上の一点  
// nvec - 平面の法線ベクトル
// Divnum - NURBS曲線のパラメータ分割数  
// *ans - 算出された交点のtパラメータ値を格納
// ans_size - ansの配列長
//
// Return:
// 交点の個数（KOD_ERR:交点の数がans_sizeを超えた）
//int NURBS_Func::CalcIntersecIsparaCurveV(NURBSS *nurb,double U,Coord pt,Coord nvec,int Divnum,double *ans,int ans_size)
int NURBSS::CalcIntersecIsparaCurveV(double U,Coord pt,Coord nvec,int Divnum,double *ans,int ans_size)
{
	double d = 0;				// ニュートン法によるパラメータの更新量
	double F;					// ニュートン法の対象とする方程式
	double Fv;					// Fのvによる微分値
	int loopcount = 0;			// ループ回数
	bool flag = false;			// 収束フラグ
	int anscount = 0;			// 交点の数をカウント
	double v = V[0];		// 現在のNURBS曲線のパラメータ値
	double dv = (V[1] - V[0])/(double)Divnum;	// 初期点の増分値

	for(int i=0;i<=Divnum;i++){
		flag = false;
		loopcount = 0;
		v = V[0] + (double)i*dv;		// 初期値更新
		while(loopcount < LOOPCOUNTMAX){
//			F = CalcInnerProduct(nvec,SubCoord(CalcNurbsSCoord(nurb,U,v),pt));
//			Fv = CalcInnerProduct(nvec,CalcDiffvNurbsS(nurb,U,v));
			F = CalcInnerProduct(nvec,SubCoord(CalcNurbsSCoord(U,v),pt));
			Fv = CalcInnerProduct(nvec,CalcDiffvNurbsS(U,v));
			if(CheckZero(Fv,MID_ACCURACY) == KOD_TRUE)	break;
			d = -F/Fv;		// 更新値
			if(CheckZero(d,MID_ACCURACY) == KOD_TRUE){		// 更新値が閾値以下になったら、whileを抜け、解として登録
				flag = true;
				break;
			}
			//fprintf(stderr,"   %lf,%lf,%lf,%lf\n",v,d,F,Fv); //for debug
			v += d;		// パラメータ値更新
			if(v < V[0] || v > V[1]){		// パラメータ範囲を超えたら、whileを抜け、次の初期値へ移行
				flag = false;
				break;
			}
			loopcount++;
		}// end of wihle
		if(flag == true){
			anscount = CheckTheSamePoints(ans,anscount);		// 同一点は除去する
			if(anscount == ans_size){	// 解の個数がans_sizeを超えたら、ERRをリターン
				GuiIF.SetMessage("NURBS_Func ERROR: Ans_size overflow");
				return KOD_ERR;
			}
			ans[anscount] = v;		// 解として登録
			anscount++;
		}
	}// end of i loop

	anscount = CheckTheSamePoints(ans,anscount);		// 同一点は除去する

	return anscount;
}

// Function: CalcIntersecCurve3 
// NURBS曲線と平面との交点を求める(3次まで対応)
// 交点は最大で(M-1)*(K-M+1)点得られる.  (例：4階でコントロールポイントの数8個の場合、(4-1)*(8-4+1)=15点)
// よって引数*ansは(M-1)*(K-M+1)個の配列を用意することが望ましい.
// 
// Parameters:
// *nurb - NURBS曲線  
// pt - 平面上の一点  
// nvec - 平面の法線ベクトル  
// *ans - 算出された交点のtパラメータ値を格納
// ans_size - ansの配列長
//
// Return:
// 交点の個数（曲線次数が3次以上：KOD_ERR）
//int NURBS_Func::CalcIntersecCurve3(NURBSC *nurb,Coord pt,Coord nvec,double *ans,int ans_size)
int NURBSC::CalcIntersecCurve3(Coord pt,Coord nvec,double *ans,int ans_size)
{
	double **coef;
	double Q[4];	// NURBS曲線の分母の係数
	Coord  P[4];	// NURBS曲線の分子の係数
	double a[4];
	double t[3];
	int ansnum;
	int k=0;

	if((coef = NewMatrix(M,M)) == NULL){
        GuiIF.SetMessage("NURBS KOD_ERROR: CalcIntersecPlane3()");
		return KOD_ERR;
	}

	// 1本のNURBS曲線はK-M+1本の曲線から構成される。それぞれの構成曲線に対して方程式を導出し、解を得る。
	for(int i=0;i<K-M+1;i++){
		if(M-1 == 3){			// 3次			
			GetBSplCoef3(M,K,i,T,coef);	// 各コントロールポイントにおける3次Bスプライン基底関数の係数(coef)を求める
//			GetBSplCoef3(M,K,i,T.get(),coef);	// 各コントロールポイントにおける3次Bスプライン基底関数の係数(coef)を求める
		}
		else if(M-1 == 2){	// 2次
			GetBSplCoef2(M,K,i,T,coef);	// 各コントロールポイントにおける2次Bスプライン基底関数の係数を求める
//			GetBSplCoef2(M,K,i,T.get(),coef);	// 各コントロールポイントにおける2次Bスプライン基底関数の係数を求める
		}
		else if(M-1 == 1){	// 1次	
			GetBSplCoef1(M,K,i,T,coef);	// 各コントロールポイントにおける1次Bスプライン基底関数の係数を求める
//			GetBSplCoef1(M,K,i,T.get(),coef);	// 各コントロールポイントにおける1次Bスプライン基底関数の係数を求める
		}
		else{
			char mes[256];
			sprintf(mes,"NURBS KOD_ERROR:Ther order of equation is unsupported. (order = %d)",M-1);
            GuiIF.SetMessage(mes);
			goto EXIT;
		}
		GetNurbsCCoef(coef,i,P,Q);						// NURBS曲線の係数(P,Q)を求める
		GetIntersecEquation(M,P,Q,pt,nvec,a);			// NURBS曲線と平面の交線導出用方程式を得る
		ansnum = CalcEquation(a,t,M-1);					// 方程式を解き、交点のパラメータ値を得る

		for(int j=0;j<ansnum;j++){
			if(t[j] >= T[i+M-1] && t[j] <= T[i+M]){	// ノットベクトルの値と適合するもののみ解として抽出
				if(k == ans_size){
                    GuiIF.SetMessage("NURBS KOD_ERROR:Intersection points exceeded the allocated array length");
					goto EXIT;
				}
				ans[k] = t[j];		// 解を取得
				k++;				// 解の数をインクリメント
			}
		}
	}

	return k;

EXIT:
	FreeMatrix(coef,M);
	return KOD_ERR;
}

// Function: CalcEquation
// (private)CalcIntersecCurve3(), CalcIntersecPtsPlaneU/V3()のサブ関数．3次方程式までを解く
// 
// Parameters:
// *a - 係数行列
// *t - 解
// M - 次数
//
// Return:
// 解の個数（解がなかった場合 or 次数が3,2,1のいずれかでない：KOD_ERR）
//int NURBS_Func::CalcEquation(double *a,double *t,int M)
int CalcEquation(double *a,double *t,int M)
{
	int flag;

	if(M == 3)		flag = CalcCubicEquation(a,t);
	else if(M == 2)	flag = CalcQuadraticEquation(a,t);
	else if(M == 1) flag = CalcLinearEquation(a,t);
	else			return KOD_ERR;

	return flag;
}

// Function: GetIntersecEquation
// (private)CalcIntersecCurve3(), CalcIntersecPtsPlaneU/V3()のサブ関数．NURBS曲線と平面の交線導出用方程式を得る
// 
// Parameters:
// M - 階数 
// *P, *Q - NURBS曲線の係数（P,Q)
// pt - 平面上の一点
// nvec - 平面の法線ベクトル 
// *a - 結果 
//void NURBS_Func::GetIntersecEquation(int M,Coord *P,double *Q,Coord pt,Coord nvec,double *a)
void GetIntersecEquation(int M,Coord *P,double *Q,Coord pt,Coord nvec,double *a)
{
	for(int i=0;i<M;i++){
		a[i] = (Q[i]*pt.x-P[i].x)*nvec.x + (Q[i]*pt.y-P[i].y)*nvec.y + (Q[i]*pt.z-P[i].z)*nvec.z;
	}
}

// Function: GetNurbsCCoef
// (private)CalcIntersecCurve3()のサブ関数．NURBS曲線の係数を求める(最高3次)
// 
// Parameters:
// *nurb - 対象となるNURBS曲線 
// **coef - Bスプライン基底関数の係数 
// i - 曲線の番号 
// *P, *Q - NURBS曲線の係数(P,Q) 
//
// Return:
// 成功：KOD_TRUE, 失敗：KOD_ERR
//int NURBS_Func::GetNurbsCCoef(NURBSC *nurb,double **coef,int i,Coord *P,double *Q)
int NURBSC::GetNurbsCCoef(double **coef,int i,Coord *P,double *Q)
{
	for(int j=0;j<M;j++){
		InitCoord(&P[j]);
		Q[j] = 0;
	}

	for(int j=0;j<M;j++){
		for(int k=0;k<M;k++){
			Q[j] += coef[k][j]*W[i+k];
			P[j] = AddCoord(P[j],MulCoord(cp[i+k],coef[k][j]*W[i+k]));
		}
	}
	
	return KOD_TRUE;
}

// Function: GetBSplCoef3
// 3次のBスプライン曲線の各係数を求める.
//
// coef[j][0]t^3 + coef[j][1]t^2 + coef[j][2]t + coef[j][3]   (Nj,4)
// 
// Parameters:
// M - 階数  
// K - コントロールポイントの数  
// i - 注目中のコントロールポイント 
// *t - ノットベクトル列  
// *coef - 算出される係数を格納
//
// Return:
// KOD_TRUE
//int NURBS_Func::GetBSplCoef3(int M,int K,int i,double *t,double **coef)
int GetBSplCoef3(int M,int K,int i,double *t,double **coef)
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

// Function: GetBSplCoef2
// 2次のBスプライン曲線の各係数を求める
//
// coef[j][0]t^2 + coef[j][1]t + coef[j][2]
//
// Parameters:
// M - 階数  
// K - コントロールポイントの数  
// i - 注目中のコントロールポイント 
// *t - ノットベクトル列  
// *coef - 算出される係数を格納
//
// Return:
// KOD_TRUE
//int NURBS_Func::GetBSplCoef2(int M,int K,int i,double *t,double **coef)
int GetBSplCoef2(int M,int K,int i,double *t,double **coef)
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

// Function: GetBSplCoef1
// 1次のBスプライン曲線の各係数を求める
//
// coef[j][0]t + coef[j][1]
//
// Parameters:
// M - 階数  
// K - コントロールポイントの数  
// i - 注目中のコントロールポイント 
// *t - ノットベクトル列  
// *coef - 算出される係数を格納
//
// Return:
// KOD_TRUE
//int NURBS_Func::GetBSplCoef1(int M,int K,int i,double *t,double **coef)
int GetBSplCoef1(int M,int K,int i,double *t,double **coef)
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

// Function: ShiftNurbsS
// NURBS曲面のシフト
//
// Parameters:
// *nurbs - 変更されるNURBS曲面  
// shift - シフト量
//void NURBS_Func::ShiftNurbsS(NURBSS *nurbs,Coord shift)
void NURBSS::ShiftNurbsS(Coord shift)
{
	for(int i=0;i<K[0];i++){
		for(int j=0;j<K[1];j++){
			cp[i][j] = AddCoord(cp[i][j],shift);
		}
	}
}

// Function: ShiftNurbsC
// NURBS曲線のシフト
// 
// Parameters:
// *nurbs - 変更されるNURBS曲線  
// shift - シフト量
//void NURBS_Func::ShiftNurbsC(NURBSC *nurbs,Coord shift)
void NURBSC::ShiftNurbsC(Coord shift)
{
	for(int i=0;i<K;i++){
		cp[i] = AddCoord(cp[i],shift);
	}
}

// Function: RotNurbsS
// NURBS曲面をDベクトル回りにdeg(°)だけ回転させる
//
// Parameters:
// *nurbs - 変更されるNURBS曲面　
// axis - 回転軸の単位ベクトル　
// deg - 角度(degree)
//void NURBS_Func::RotNurbsS(NURBSS *nurbs,Coord axis,double deg)
void NURBSS::RotNurbsS(Coord axis,double deg)
{
	double rad;			// ラジアン格納用
	QUATERNION QFunc;	// クォータニオン関連の関数を集めたクラスのオブジェクトを生成
	Quat StartQ;		// 回転前の座標を格納するクォータニオン
	Quat RotQ;			// 回転クォータニオン
	Quat ConjuQ;		// 共役クォータニオン
	Quat TargetQ;		// 回転後の座標を格納するクォータニオン
	
	for(int i=0;i<K[0];i++){			// u方向のコントロールポイント分ループ
		for(int j=0;j<K[1];j++){		// v方向のコントロールポイント分ループ
			StartQ = QFunc.QInit(1,cp[i][j].x,cp[i][j].y,cp[i][j].z);		// NURBS曲面を構成するcpの座標を登録
			rad = DegToRad(deg);										// degreeからradianに変換
			RotQ = QFunc.QGenRot(rad,axis.x,axis.y,axis.z);				// 回転クォータニオンに回転量を登録(ここの数字をいじれば任意に回転できる)
			ConjuQ = QFunc.QConjugation(RotQ);							// RotQの共役クォータニオンを登録
			TargetQ = QFunc.QMult(QFunc.QMult(RotQ,StartQ),ConjuQ);		// 回転させる
			cp[i][j] = SetCoord(TargetQ.x,TargetQ.y,TargetQ.z);	// 回転後の座標を登録
		}
	}
}

// Function: RotNurbsC
// NURBS曲面をDベクトル回りにdeg(°)だけ回転させる
//
// Parameters:
// *nurbs - 変更されるNURBS曲線　
// axis - 回転軸の単位ベクトル　
// deg - 角度(degree)
//void NURBS_Func::RotNurbsC(NURBSC *nurbs,Coord axis,double deg)
void NURBSC::RotNurbsC(Coord axis,double deg)
{
	double rad;			// ラジアン格納用
	QUATERNION QFunc;	// クォータニオン関連の関数を集めたクラスのオブジェクトを生成
	Quat StartQ;		// 回転前の座標を格納するクォータニオン
	Quat RotQ;			// 回転クォータニオン
	Quat ConjuQ;		// 共役クォータニオン
	Quat TargetQ;		// 回転後の座標を格納するクォータニオン
	
	for(int i=0;i<K;i++){		// コントロールポイント分ループ
		StartQ = QFunc.QInit(1,cp[i].x,cp[i].y,cp[i].z);		// NURBS曲面を構成するcpの座標を登録
		rad = DegToRad(deg);									// degreeからradianに変換
		RotQ = QFunc.QGenRot(rad,axis.x,axis.y,axis.z);			// 回転クォータニオンに回転量を登録(ここの数字をいじれば任意に回転できる)
		ConjuQ = QFunc.QConjugation(RotQ);						// RotQの共役クォータニオンを登録
		TargetQ = QFunc.QMult(QFunc.QMult(RotQ,StartQ),ConjuQ);	// 回転させる
		cp[i] = SetCoord(TargetQ.x,TargetQ.y,TargetQ.z);	// 回転後の座標を登録
	}
}

// Function: ChRatioNurbsS
// NURBS曲面の倍率を変更する
//
// Parameters:
// *nurbs - 変更されるNURBS曲面  
// ratio - 倍率
//void NURBS_Func::ChRatioNurbsS(NURBSS *nurbs,Coord ratio)
void NURBSS::ChRatioNurbsS(Coord ratio)
{
	for(int i=0;i<K[0];i++){
		for(int j=0;j<K[1];j++){
			cp[i][j] = MulCoord(cp[i][j],ratio);
		}
	}
}

// Function: ChRatioNurbsC
// NURBS曲線の倍率を変更する
//
// Parameters:
// *nurbs - 変更されるNURBS曲線  
// ratio - 倍率
//void NURBS_Func::ChRatioNurbsC(NURBSC *nurbs,Coord ratio)
void NURBSC::ChRatioNurbsC(Coord ratio)
{
	for(int i=0;i<K;i++){
		cp[i] = MulCoord(cp[i],ratio);
	}
}

// Function: SetCPNurbsS
// NURBS曲面nurbsのコントロールポイントを，NURBS曲面Nurbsのコントロールポイントに置き換える
//
// Parameters:
// *nurbs - 置換されるNURBS曲面  
// Nurbs - 代入元のNURBS曲面
//
// Return:
// 正常終了：KOD_TRUE, 両曲面のコントロールポイント数が一致していない：KOD_ERR
int NURBS_Func::SetCPNurbsS(NURBSS *nurbs,NURBSS Nurbs)
{
	if(nurbs->K[0] != Nurbs.K[0] || nurbs->K[1] != Nurbs.K[1]){
        GuiIF.SetMessage("NURBS KOD_ERROR:Control point count is different");
		return KOD_ERR;
	}

	for(int i=0;i<Nurbs.K[0];i++){
		for(int j=0;j<Nurbs.K[1];j++){
			nurbs->cp[i][j] = SetCoord(Nurbs.cp[i][j]);
		}
	}

	return KOD_TRUE;
}

// Function: GenInterpolatedNurbsC1
// 与えられた点列を補間するn階のNURBS曲線を生成する.
// 端末条件を与えないバージョン
//
// Parameters:
// *Nurbs - 生成されるNURBS曲線のアドレス   
// *P - 点列   
// PNum - 点列の数   
// M - 階数
//
// Return:
// 正常終了：KOD_TRUE, 与えられた点列が1個未満：KOD_ERR, 計算過程でゼロ割が発生：KOD_ERR
int NURBS_Func::GenInterpolatedNurbsC1(NURBSC *Nurbs,Coord *P,int PNum,int M)
{
	int retflag = KOD_TRUE;

	if(PNum <= 1){			// 与えられた点が1個未満の場合は、NURBS曲線を生成できない
        GuiIF.SetMessage("NURBS KOD_ERROR:Few Point. You should set over 2 points at least");
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
	for(int i=0;i<PNum;i++)
		P[i].dmy = T_[i];

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
        GuiIF.SetMessage("NURBS ERROR:Determinant is 0");
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

// Function: GenInterpolatedNurbsC2
// 与えられた点列を補間するn階のNURBS曲線を生成する．
// 端末条件:始点とC2で一致
//
// Parameters:
// *Nurbs - 生成されるNURBS曲線のアドレス   
// *P_ - 通過点列（P_[0]とP_[PNum-1]は一致していること）
// PNum - 通過点列の数   
// M - 階数
//
// Return:
// KOD_TRUE:正常終了, KOD_FALSE:点列の始点と終点が一致していない, KOD_ERR:点列の数が1個未満
int NURBS_Func::GenInterpolatedNurbsC2(NURBSC *Nurbs,Coord *P_,int PNum,int M)
{
	if(DiffCoord(P_[0],P_[PNum-1]) == KOD_FALSE){
        GuiIF.SetMessage("NURBS KOD_ERROR:Given points P0 and Pn are not unmuched");
		return KOD_FALSE;
	}
	if(PNum <= 1){			// 与えられた点が1個未満の場合は、NURBS曲線を生成できない
        GuiIF.SetMessage("NURBS KOD_ERROR:Few Point. You should set over 2 points at least");
		return KOD_ERR;
	}
	if(PNum == 2 || PNum == 3)	M = PNum;	// 与えられた点が2個か3個の場合は、階数を強制的に2か3にする

	int K = PNum+2;				// コントロールポイントの数
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
	
	return KOD_TRUE;
}

// Function: GenApproximationNurbsC
// 与えられた点列を近似するn階のNURBS曲線を生成する
//
// Parameters:
// *Nurbs - 生成されるNURBS曲線のアドレス   
// *P - 点列   
// PNum - 点列の数   
// M - 階数
//
// Return:
// 正常終了：KOD_TRUE, 与えられた点が1個未満：KOD_ERR
int NURBS_Func::GenApproximationNurbsC(NURBSC *Nurbs,Coord *P,int PNum,int M)
{
	if(PNum <= 1){			// 与えられた点が1個未満の場合は、NURBS曲線を生成できない
        GuiIF.SetMessage("NURBS KOD_ERROR:Few Point. You should set over 2 points at least");
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

// Function: GenNurbsCfromCP
// コントロールポイントからNURBS曲線を生成する
//
// ノットベクトルは等間隔に設定される
//
// 重みは全て1とする
//
// Parameters:
// *Nurbs - 生成されるNURBS曲線のアドレス   
// *P - 点列   
// PNum - 点列の数   
// M - 階数
// 正常終了：KOD_TRUE, 与えられた点が1個未満：KOD_ERR
int NURBS_Func::GenNurbsCfromCP(NURBSC *Nurbs,Coord *P,int PNum,int M)
{
	if(PNum <= 1){			// 与えられた点が1個未満の場合は、NURBS曲線を生成できない
        GuiIF.SetMessage("NURBS KOD_ERROR:Few Point. You should set over 2 points at least");
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

// Function: GenPolygonalLine
// 折れ線(NURBS曲線)を生成する
// 
// Parameters:
// *Nurbs - 生成されるNURBS曲線のアドレス   
// *P - コントロールポイント   
// PNum - コントロールポイントの数
//
// Return:
// 正常終了：KOD_TRUE, 与えられた点が1個未満：KOD_ERR
int NURBS_Func::GenPolygonalLine(NURBSC *Nurbs,Coord *P,int PNum)
{
	if(PNum <= 1){			// 与えられた点が1個未満の場合は、NURBS曲線を生成できない
        GuiIF.SetMessage("NURBS KOD_ERROR:Few Point. You should set over 2 points at least");
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

// Function: GenInterpolatedNurbsS1
// 与えられた点列を補間するn階NURBS曲面を生成する．
// 端末条件を与えないバージョン
//
// Parameters:
// *Nurbs - 生成されるNURBS曲面のアドレス   
// **P - 与えられた点列   
// PNum_u,PNum_v - 点の数　 
// Mu,Mv - 階数
//
// Return:
// 正常終了：KOD_TRUE, 与えられた点が1個未満：KOD_ERR
int NURBS_Func::GenInterpolatedNurbsS1(NURBSS *Nurbs,Coord **P,int PNum_u,int PNum_v,int Mu,int Mv)
{
	if(PNum_u <= 1 || PNum_v <= 1){			// 与えられた点が各方向で1個未満の場合は、NURBS曲面を生成できない
        GuiIF.SetMessage("NURBS ERROR:Few Point. You should set over 2 points at least");
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

// Function: GenApproximationNurbsS
// 与えられた点列を近似するn階のNURBS曲面を生成する
//
// Parameters:
// *Nurbs - 生成されるNURBS曲面のアドレス   
// **P - 与えられた点列   
// PNum_u,PNum_v - 点の数　 
// Mu,Mv - 階数
//
// Return:
// 正常終了：KOD_TRUE, 与えられた点が1個未満：KOD_ERR
int NURBS_Func::GenApproximationNurbsS(NURBSS *Nurbs,Coord **P,int PNum_u,int PNum_v,int Mu,int Mv)
{
	if(PNum_u <= 1 || PNum_v <= 1){			// 与えられた点が各方向で1個未満の場合は、NURBS曲面を生成できない
        GuiIF.SetMessage("NURBS ERROR:Few Point. You should set over 2 points at least");
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
	FreeCoord2(Q1,PNum_u);
	FreeCoord2(Q2,K[1]);
	FreeCoord2(Q3,K[1]);
	FreeCoord2(Q4,K[0]);
	FreeCoord2(P_,K[1]);
	FreeMatrix(W,K[0]);

	return KOD_TRUE;
}

// Function: GenNurbsSfromCP
// 与えられたコントロールポイントからn階のNURBS曲面を生成する
//
// ノットベクトルは等間隔に設定される
//
// 重みは全て1とする
//
// Parameters:
// *Nurbs - 生成されるNURBS曲面のアドレス   
// **P - 与えられたコントロールポイント列   
// PNum_u,PNum_v - 点の数　 
// Mu,Mv - 階数
//
// Return:
// 正常終了：KOD_TRUE, 与えられた点が1個未満：KOD_ERR
int NURBS_Func::GenNurbsSfromCP(NURBSS *Nurbs,Coord **P,int PNum_u,int PNum_v,int Mu,int Mv)
{
	if(PNum_u <= 1 || PNum_v <= 1){			// 与えられた点が各方向で1個未満の場合は、NURBS曲面を生成できない
        GuiIF.SetMessage("NURBS ERROR:Few Point. You should set over 2 points at least");
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

// Function: 
// 折れ面(NURBS曲面)を生成するGenPolygonalSurface
//
// Parameters:
// *Nurbs - 生成されるNURBS曲面のアドレス   
// **P - コントロールポイント   
// PNum_u,PNum_v - コントロールポイントの数
//
// Return:
// KOD_TRUE
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
	FreeMatrix(W,K[0]);
	FreeVector(du_sum);
	FreeVector(dv_sum);

	return KOD_TRUE;
}

// Function: ConnectNurbsSU
// 2枚のNURBS曲面を連結する(U方向に長くなる)(S1_U1とS2_U0を連結)
//
// Parameters:
// *S1 - 面1
// *S2 - 面2
// *S_ - 連結後の面を格納
//
// Return:
// 成功：KOD_TRUE,  失敗：KOD_FALSE
int NURBS_Func::ConnectNurbsSU(NURBSS *S1,NURBSS *S2,NURBSS *S_)			
{
	// 連結されるエッジのV方向コントロールポイントの数が全て等しいこと
	if(S1->K[1] != S2->K[1]){
		fprintf(stderr,"ERROR: Number of control point on V direction is not equal.");
		return KOD_ERR;
	}
	// 連結されるエッジのV方向コントロールポイントが全て等しいこと
	for(int i=0;i<S1->K[1];i++){
		if(DiffCoord(S1->cp[S1->K[0]-1][i],S2->cp[0][i]) == KOD_FALSE){
			fprintf(stderr,"ERROR: Knot value on V direction is not equal.");
			return KOD_ERR;
		}
	}
	// 両曲面の階数がU,V共に等しいこと
	if(S1->M[0] != S2->M[0] || S1->M[1] != S2->M[1]){
		fprintf(stderr,"ERROR: Rank is not equal.");
		return KOD_ERR;
	}

	int K[2],N[2];

	K[0] = S1->K[0] + S2->K[0] - 1;				// S_のU方向コントロールポイントの数
	K[1] = S1->K[1];							// S_のV方向コントロールポイントの数
	N[0] = S1->N[0] + S2->N[0] - S2->M[0] - 1;	// S_のU方向ノットベクトルの数
	N[1] = S1->N[1];							// S_のV方向ノットベクトルの数

	New_NurbsS(S_,K,N);							// S_内のメモリー確保

	SetKnotVecSU_ConnectS(S1,S2,S_);			// S_のu方向ノット定義域を指定

	SetCPSU_ConnectS(S1,S2,S_);					// S_のu方向コントロールポイントとウェイトを指定

	S_->M[0] = S1->M[0];						// S_の階数を指定
	S_->M[1] = S1->M[1];

	return KOD_TRUE;
}

// Function: ConnectNurbsSV
// 2枚のNURBS曲面を連結する(V方向に長くなる)(S1_V1とS2_V0を連結)
//
// Parameters:
// *S1 - 面1
// *S2 - 面2
// *S_ - 連結後の面を格納
//
// Return:
// 成功：KOD_TRUE,  失敗：KOD_FALSE
int NURBS_Func::ConnectNurbsSV(NURBSS *S1,NURBSS *S2,NURBSS *S_)			
{
	// 連結されるエッジのU方向コントロールポイントの数が全て等しいこと
	if(S1->K[0] != S2->K[0]){
		fprintf(stderr,"ERROR: Number of control point on U direction is not equal.");
		return KOD_ERR;
	}
	// 連結されるエッジのU方向コントロールポイントが全て等しいこと
	for(int i=0;i<S1->K[0];i++){
		if(DiffCoord(S1->cp[i][S1->K[0]-1],S2->cp[i][0]) == KOD_FALSE){
			fprintf(stderr,"ERROR: Knot value on U direction is not equal.");
			return KOD_ERR;
		}
	}
	// 両曲面の階数がU,V共に等しいこと
	if(S1->M[0] != S2->M[0] || S1->M[1] != S2->M[1]){
		fprintf(stderr,"ERROR: Rank is not equal.");
		return KOD_ERR;
	}

	int K[2],N[2];

	K[0] = S1->K[0];							// S_のU方向コントロールポイントの数
	K[1] = S1->K[1] + S2->K[1] - 1;				// S_のV方向コントロールポイントの数
	N[0] = S1->N[0];							// S_のU方向ノットベクトルの数
	N[1] = S1->N[1] + S2->N[1] - S2->M[1] - 1;	// S_のV方向ノットベクトルの数

	New_NurbsS(S_,K,N);							// S_内のメモリー確保

	SetKnotVecSV_ConnectS(S1,S2,S_);			// S_のv方向ノット定義域を指定

	SetCPSV_ConnectS(S1,S2,S_);					// S_のv方向コントロールポイントとウェイトを指定

	S_->M[0] = S1->M[0];						// S_の階数を指定
	S_->M[1] = S1->M[1];

	return KOD_TRUE;
}

// Function: SetCPSU_ConnectS
// (private)ConnectNurbsSU()のサブ関数．S_のu方向コントロールポイントとウェイトを指定
//
// Parameters:
// *S1 - 面1
// *S2 - 面2
// *S_ - 連結後の面を格納
void NURBS_Func::SetCPSU_ConnectS(NURBSS *S1,NURBSS *S2,NURBSS *S_)
{
	S_->K[0] = S1->K[0] + S2->K[0] - 1;
	S_->K[1] = S1->K[1];

	for(int i=0;i<S1->K[0];i++){
		for(int j=0;j<S_->K[1];j++){
			S_->cp[i][j] = SetCoord(S1->cp[i][j]);
			S_->W[i][j] = S1->W[i][j];
		}
	}
	for(int i=1;i<S2->K[0];i++){
		for(int j=0;j<S_->K[1];j++){
			S_->cp[S1->K[0]+i-1][j] = SetCoord(S2->cp[i][j]);
			S_->W[S1->K[0]+i-1][j] = S2->W[i][j];
		}
	}
}

// Function: SetKnotVecSU_ConnectS
// (private)ConnectNurbsSU()のサブ関数．S_のu方向ノット定義域を指定
//
// Parameters:
// *S1 - 面1
// *S2 - 面2
// *S_ - 連結後の面を格納
void NURBS_Func::SetKnotVecSU_ConnectS(NURBSS *S1,NURBSS *S2,NURBSS *S_)
{
	// V方向
	S_->N[1] = S1->N[1];				// S_のV方向ノットベクトルの数
	CopyVector(S1->T,S1->N[1],S_->T);	// S_のV方向ノットベクトル(V方向はS1のをそのまま流用)
	S_->V[0] = S1->V[0];				// S_のV方向ノットベクトルの範囲
	S_->V[1] = S1->V[1];

	// U方向
	// コード長を調べる
	double us=0,ue=NORM_KNOT_VAL,uc=0;		// U方向開始，終了，連結部ノットベクトル
	double l1=0,l2=0;						// 各曲面のU方向ノットベクトルのコード長
	for(int i=0;i<S1->N[0]-1;i++) {
//		l1 += CalcDistance(CalcNurbsSCoord(S1,S1->S[i+1],S1->T[0]),CalcNurbsSCoord(S1,S1->S[i],S1->T[0]));	// S1のコード長
		l1 += CalcDistance(S1->CalcNurbsSCoord(S1->S[i+1],S1->T[0]),S1->CalcNurbsSCoord(S1->S[i],S1->T[0]));	// S1のコード長
	}
	for(int i=0;i<S2->N[0]-1;i++) {
//		l2 += CalcDistance(CalcNurbsSCoord(S2,S2->S[i+1],S2->T[0]),CalcNurbsSCoord(S2,S2->S[i],S2->T[0]));	// S2のコード長
		l2 += CalcDistance(S2->CalcNurbsSCoord(S2->S[i+1],S2->T[0]),S2->CalcNurbsSCoord(S2->S[i],S2->T[0]));	// S2のコード長
	}
	uc = l1/(l1+l2);	// 結合点のノットベクトル値

	// S_のノットベクトル範囲を得る
	Vector U1 = NewVector(S1->N[0]);	
	Vector U2 = NewVector(S2->N[0]);	
	CopyVector(S1->S,S1->N[0],U1);		// S1のノットベクトルをU1にコピー
	CopyVector(S2->S,S2->N[0],U2);		// S2のノットベクトルをU2にコピー
	ChangeKnotVecRange(U1,S1->N[0],S1->M[0],S1->K[0],us,uc);	// S1(U1)のノットベクトルの範囲を変更
	ChangeKnotVecRange(U2,S2->N[0],S2->M[0],S2->K[0],uc,ue);	// S2(U2)のノットベクトルの範囲を変更
	S_->U[0] = us;						// S_のU方向ノットベクトルの範囲
	S_->U[1] = ue;
	S_->N[0] = S1->N[0] + S2->N[0] - S2->M[0] - 1;	// S_のノットベクトル数

	// S_のノットベクトルを得る
	for(int i=0;i<S1->K[0];i++)
		S_->S[i] = U1[i];
	for(int i=1;i<S2->N[0];i++)
		S_->S[S1->K[0]+i-1] = U2[i];

	FreeVector(U1);
	FreeVector(U2);
}

// Function: SetCPSV_ConnectS
// (private)ConnectNurbsSV()のサブ関数．S_のv方向コントロールポイントとウェイトを指定
//
// Parameters:
// *S1 - 面1
// *S2 - 面2
// *S_ - 連結後の面を格納
void NURBS_Func::SetCPSV_ConnectS(NURBSS *S1,NURBSS *S2,NURBSS *S_)
{
	S_->K[0] = S1->K[0];
	S_->K[1] = S1->K[1] + S2->K[1] - 1;

	for(int i=0;i<S_->K[0];i++){
		for(int j=0;j<S1->K[1];j++){
			S_->cp[i][j] = SetCoord(S1->cp[i][j]);
			S_->W[i][j] = S1->W[i][j];
		}
	}
	for(int i=0;i<S_->K[0];i++){
		for(int j=1;j<S2->K[1];j++){
			S_->cp[i][S1->K[1]+j-1] = SetCoord(S2->cp[i][j]);
			S_->W[i][S1->K[1]+j-1] = S2->W[i][j];
		}
	}
}

// Function: SetKnotVecSV_ConnectS
// (private)ConnectNurbsSV()のサブ関数．S_のv方向ノット定義域を指定
//
// Parameters:
// *S1 - 面1
// *S2 - 面2
// *S_ - 連結後の面を格納
void NURBS_Func::SetKnotVecSV_ConnectS(NURBSS *S1,NURBSS *S2,NURBSS *S_)
{
	// U方向
	S_->N[0] = S1->N[0];				// S_のU方向ノットベクトルの数
	CopyVector(S1->S,S1->N[0],S_->S);	// S_のU方向ノットベクトル(U方向はS1のをそのまま流用)
	S_->U[0] = S1->U[0];				// S_のU方向ノットベクトルの範囲
	S_->U[1] = S1->U[1];

	// V方向
	// コード長を調べる
	double vs=0,ve=NORM_KNOT_VAL,vc=0;		// U方向開始，終了，連結部ノットベクトル
	double l1=0,l2=0;						// 各曲面のU方向ノットベクトルのコード長
	for(int i=0;i<S1->N[1]-1;i++) {
//		l1 += CalcDistance(CalcNurbsSCoord(S1,S1->S[0],S1->T[i+1]),CalcNurbsSCoord(S1,S1->S[0],S1->T[i]));	// S1のコード長
		l1 += CalcDistance(S1->CalcNurbsSCoord(S1->S[0],S1->T[i+1]),S1->CalcNurbsSCoord(S1->S[0],S1->T[i]));	// S1のコード長
	}
	for(int i=0;i<S2->N[1]-1;i++) {
//		l2 += CalcDistance(CalcNurbsSCoord(S2,S2->S[0],S2->T[i+1]),CalcNurbsSCoord(S2,S2->S[0],S2->T[i]));	// S2のコード長
		l2 += CalcDistance(S2->CalcNurbsSCoord(S2->S[0],S2->T[i+1]),S2->CalcNurbsSCoord(S2->S[0],S2->T[i]));	// S2のコード長
	}
	vc = l1/(l1+l2);	// 結合点のノットベクトル値

	// S_のノットベクトル範囲を得る
	Vector V1 = NewVector(S1->N[1]);	
	Vector V2 = NewVector(S2->N[1]);	
	CopyVector(S1->T,S1->N[1],V1);		// S1のノットベクトルをV1にコピー
	CopyVector(S2->T,S2->N[1],V2);		// S2のノットベクトルをV2にコピー
	ChangeKnotVecRange(V1,S1->N[1],S1->M[1],S1->K[1],vs,vc);	// S1(V1)のノットベクトルの範囲を変更
	ChangeKnotVecRange(V2,S2->N[1],S2->M[1],S2->K[1],vc,ve);	// S2(V2)のノットベクトルの範囲を変更
	S_->V[0] = vs;						// S_のV方向ノットベクトルの範囲
	S_->V[1] = ve;
	S_->N[1] = S1->N[1] + S2->N[1] - S2->M[1] - 1;	// S_のノットベクトル数

	// S_のノットベクトルを得る
	for(int i=0;i<S1->K[1];i++)
		S_->T[i] = V1[i];
	for(int i=1;i<S2->N[1];i++)
		S_->T[S1->K[1]+i-1] = V2[i];

	FreeVector(V1);
	FreeVector(V2);
}

// Function: CalcuIntersecPtNurbsLine
// NURBS曲面と直線の交点を算出
//
// Parameters:
// *Nurb - NURBS曲面S(u,v)へのポインタ
// r - 直線N(t)上の1点
// p - 直線N(t)の方向
// DivNum - NURBS曲面の分割数
// *ans - 交点のu,v,tパラメータ格納用配列
// anssize - ansの配列長
//
// Divnumが大きいほど、交点算出の取りこぼしは少なくなる．
//
// anssizeはDivNum*DivNum以上にするのが好ましい.
//
// LoD - 詳細度（ニュートン法の更新パラメータを足しこむときに，LoDで割ることで，急激なパラメータ変更を避ける．通常は1でよいが，解が得られない場合は値を大きくする．2とか3とか）
//
// Return:
// 交点の数,   KOD_ERR:交点の数が指定した配列長を超えた
//int NURBS_Func::CalcuIntersecPtNurbsLine(NURBSS *Nurb,Coord r,Coord p,int Divnum,Coord *ans,int anssize,int LoD)
int NURBSS::CalcuIntersecPtNurbsLine(Coord r,Coord p,int Divnum,Coord *ans,int anssize,int LoD)
{
	Coord d = SetCoord(100,100,100);		// NURBS曲線S(u,v)の微小変化量(du,dv)、直線N(t)の微小変化量dtを格納
	Coord F,Fu,Fv,Ft;						// F(u,v,t) = S(u,v) - N(t)    Fu = dF/du     Fv = dF/dv     Ft = dF/dt
	double u = U[0];					// NURBS曲面S(u,v)のuパラメータの現在値
	double v = V[0];					// NURBS曲面S(u,v)のvパラメータの現在値
	double t = 0;							// 直線N(t)のtパラメータ
	Matrix A = NewMatrix(3,3);				// Fu,Fv,Ftを構成する3x3行列
	Matrix A_ = NewMatrix(3,3);				// Aの逆行列を格納
	int flag = KOD_FALSE;						// 収束フラグ
	double dv = (V[1] - V[0])/(double)Divnum;	// 収束演算用のvパラメータのインターバル値
	double du = (U[1] - U[0])/(double)Divnum;	// 収束演算用のuパラメータのインターバル値
	int loopcount = 0;						// 収束計算回数
	int anscount = 0;						// 算出された交点の数

	// u loop
	for(int i=0;i<Divnum;i++){
		// v loop
		for(int j=0;j<Divnum;j++){
			u = U[0] + (double)i*du;			// ステップパラメータuの初期値をセット
			v = V[0] + (double)j*dv;		// ステップパラメータvの初期値をセット
			t = 0;								// ステップパラメータtの初期値をセット
			flag = KOD_FALSE;						// 収束フラグをOFF
			loopcount = 0;						// ループカウント初期化
			// 直線の微小変化量dt(=d.z)がAPPROX_ZEROを下回るまでニュートン法による収束計算を行う
			while(loopcount < LOOPCOUNTMAX){
//				F = SubCoord(CalcNurbsSCoord(Nurb,u,v),AddCoord(r,MulCoord(p,t)));	// F(u,v,t) = S(u,v) - N(t) = S(u,v) - (r+tp)
//				Fu = CalcDiffuNurbsS(Nurb,u,v);			// Fu = dF/du = dS/du
//				Fv = CalcDiffvNurbsS(Nurb,u,v);			// Fv = dF/dv = dS/dv
				F = SubCoord(CalcNurbsSCoord(u,v),AddCoord(r,MulCoord(p,t)));	// F(u,v,t) = S(u,v) - N(t) = S(u,v) - (r+tp)
				Fu = CalcDiffuNurbsS(u,v);			// Fu = dF/du = dS/du
				Fv = CalcDiffvNurbsS(u,v);			// Fv = dF/dv = dS/dv
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
				u += d.x/(double)LoD;
				v += d.y/(double)LoD;
				t += d.z/(double)LoD;

				//if(u < Nurb->U[0] || u > Nurb->U[1] || v < Nurb->V[0] || v > Nurb->V[1]){	// u,vのどちらかが発散したらloopを抜ける
				//	flag = KOD_FALSE;		// 収束フラグfalse
				//	break;
				//}

				loopcount++;
			}// end of while

			// LOOPCOUNTMAX回ループしても収束していなかったら警告
			if(loopcount == LOOPCOUNTMAX)
                GuiIF.SetMessage("NURBS_Func ERROR: fail to converge");

			// 収束していたら解として登録
			if(flag == KOD_TRUE){
				ans[anscount] = SetCoord(u,v,t);
				anscount++;
				if(anscount > anssize){
                    GuiIF.SetMessage("NURBS_Func ERROR: Ans_size overflow");
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

// Function: CalcIntersecPtNurbsPt
// 空間上の1点PからNURBS曲面S上の最近傍点Qを求める(ニュートン法)
//
// >直線の方程式L(t) = P + tN
// >NはS上の法線ベクトルと一致するからN=Su×Sv
// >方程式：S(u,v) = P + tN(u,v)
// >F(u,v,t) = S(u,v) - P - tN(u,v)   として、ニュートン法を用いる
// >Fu = Su - tNu	Fv = Sv - tNv	Ft = -N
// >|Fu.x Fv.x Ft.x||du|    |F.x|
// >|Fu.y Fv.y Ft.y||dv| = -|F.y|    =>     dF・d = -F     =>     d = -F・dF^-1  
// >|Fu.z Fv.z Ft.z||dt|    |F.z|
// 
// Parameters:
// *S - NURBS曲面
// P - 空間上の1点
// Divnum - ニュートン法初期値指定用の曲面分割数
// LoD - ニュートンパラメータ更新時のステップサイズ(1～)
// Q - 解（S上の点をu,v,tパラメータでCoord構造体に格納）
//
// Return:
// KOD_TRUE：収束した    KOD_FALSE:収束しなかった
//int NURBS_Func::CalcIntersecPtNurbsPt(NURBSS *S,Coord P,int Divnum,int LoD,Coord *Q)
int NURBSS::CalcIntersecPtNurbsPt(Coord P,int Divnum,int LoD,Coord *Q)
{
	Matrix dF = NewMatrix(3,3);		// Fu,Fv,Ftを構成する3x3行列
	Matrix dF_ = NewMatrix(3,3);	// dFの逆行列を格納
	Coord F,Fu,Fv,Ft;				// F(u,v,t) = S(u,v) - P - t・N(u,v)	ニュートン法にかける関数
	Coord N,Nu,Nv;					// N(u,v):S(u,v)上の法線ベクトル
	Coord d = InitCoord();			// ニュートン法によって更新されるステップサイズパラメータ
	int loopcount=0;				// while()ループのカウント
	double u,v,t;					// u,v,tの現在値
	double dv = (V[1] - V[0])/(double)Divnum;	// 収束演算用のvパラメータのインターバル値
	double du = (U[1] - U[0])/(double)Divnum;	// 収束演算用のuパラメータのインターバル値
	int flag = KOD_FALSE;			// while()抜け用判別フラグ
	Coord *Q_ = NewCoord1(Divnum*Divnum);			// 解の一時格納用

	// 各初期値に対してニュートン法適用
	for(int i=0;i<Divnum;i++){
		for(int j=0;j<Divnum;j++){
			u = U[0] + (double)i*du;			// ステップパラメータuの初期値をセット
			v = V[0] + (double)j*dv;			// ステップパラメータvの初期値をセット
			t = 0;								// ステップパラメータtの初期値をセット
			loopcount = 0;
			flag = KOD_FALSE;

			// 収束計算
			while(loopcount < LOOPCOUNTMAX){
//				N = CalcNormVecOnNurbsS(S,u,v);									// S(u,v)上の法線ベクトルN(u,v)を算出
//				Nu = CalcDiffuNormVecOnNurbsS(S,u,v);								// N(u,v)のu方向偏微分
//				Nv = CalcDiffvNormVecOnNurbsS(S,u,v);								// N(u,v)のv方向偏微分
//				F = SubCoord(SubCoord(CalcNurbsSCoord(S,u,v),P),MulCoord(N,t));	// ニュートン法にかける関数
//				Fu = SubCoord(CalcDiffuNurbsS(S,u,v),MulCoord(Nu,t));				// Fのu方向偏微分
//				Fv = SubCoord(CalcDiffvNurbsS(S,u,v),MulCoord(Nv,t));				// Fのv方向偏微分
				N = CalcNormVecOnNurbsS(u,v);									// S(u,v)上の法線ベクトルN(u,v)を算出
				Nu = CalcDiffuNormVecOnNurbsS(u,v);								// N(u,v)のu方向偏微分
				Nv = CalcDiffvNormVecOnNurbsS(u,v);								// N(u,v)のv方向偏微分
				F = SubCoord(SubCoord(CalcNurbsSCoord(u,v),P),MulCoord(N,t));	// ニュートン法にかける関数
				Fu = SubCoord(CalcDiffuNurbsS(u,v),MulCoord(Nu,t));				// Fのu方向偏微分
				Fv = SubCoord(CalcDiffvNurbsS(u,v),MulCoord(Nv,t));				// Fのv方向偏微分
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
					//fprintf(stderr,"%d:det = 0\n",loopcount);	// debug
					break;
				}

				d = MulCoord(MulMxCoord(dF_,F),-1);		// ステップサイズパラメータの更新値を算出

				if(fabs(d.x) <= APPROX_ZERO_L && fabs(d.y) <= APPROX_ZERO_L && fabs(d.z) <= APPROX_ZERO_L){	// 真値に収束したらloopを抜ける
					flag = KOD_TRUE;		// 収束フラグtrue
					break;
				}

				// 真値に達していなかったらu,v,tを更新
				u += d.x/(double)LoD;
				v += d.y/(double)LoD;
				t += d.z/(double)LoD;
				//fprintf(stderr,"%d:%lf,%lf,%lf,%lf,%lf,%lf\n",loopcount,u,v,t,d.x,d.y,d.z);	// debug

				loopcount++;
			}// end of while

			if(flag == KOD_TRUE)	Q_[i*Divnum+j] = SetCoord(u,v,t);		// 収束していたら

			else Q_[i*Divnum+j] = SetCoord(KOD_ERR,KOD_ERR,KOD_ERR);		// 収束していなかったら

		}// end of loop j
	}// end of loop i

	flag = GetMinDist(P,Q_,Divnum*Divnum,Q);		// 極小解にならないよう，全ての解のうち，距離が最小のものを真の解として選び出す

	FreeMatrix(dF,3);
	FreeMatrix(dF_,3);
	FreeCoord1(Q_);

	return flag;
}

// Function: CalcIntersecPtNurbsPt
// 空間上の1点PからNURBS曲線C上の最近傍点Q(曲線パラメータ)を求める(ニュートン法)
// 
// >F(t) = (P-C'(t))･C'(t) = 0
// >F'(t)dt = -F(t)
// >F'(t) = -|C'(t)|^2 + (P+C(t))･C"(t)
//
// Parameters:
// *C - NURBS曲線
// P - 空間上の1点
// Divnum - ニュートン法初期値指定用の曲線分割数
// LoD - ニュートンパラメータ更新時のステップサイズ(1～)
// Q - 解（C上の点をtパラメータで格納）
// 
// Return:
// KOD_TRUE：収束した    KOD_FALSE:収束しなかった
//int NURBS_Func::CalcIntersecPtNurbsPt(NURBSC *C,Coord P,int Divnum,int LoD,double *Q)
int NURBSC::CalcIntersecPtNurbsPt(Coord P,int Divnum,int LoD,double *Q)
{
	double *t_buf = NewVector(Divnum);					// 収束解格納用バッファ
	double *dist_buf = NewVector(Divnum);				// 各tでの距離格納用バッファ
	double delta = (V[1] - V[0])/(double)Divnum;	// 収束演算用のtパラメータのインターバル値

	for(int i=0;i<Divnum;i++){
		double t = V[0] + (double)i*delta;	// tの初期値をセット
		int loopcount = 0;
		while(loopcount < LOOPCOUNTMAX){
//			Coord Ct = CalcNurbsCCoord(C,t);
//			Coord C_ = CalcDiffNurbsC(C,t);
//			Coord C__ = CalcDiff2NurbsC(C,t);
			Coord Ct = CalcNurbsCCoord(t);
			Coord C_ = CalcDiffNurbsC(t);
			Coord C__ = CalcDiff2NurbsC(t);
			double a = CalcInnerProduct(P,C_);
			double b = CalcInnerProduct(Ct,C_);
			double c = CalcInnerProduct(C_,C_);
			double d = CalcInnerProduct(SubCoord(P,Ct),C__);
			if(fabs(d-c) <= APPROX_ZERO)	break;			// 分母がゼロなら次の初期点へ
			double dt = (b-a)/(d-c);
			t += dt/(double)LoD;				// t更新
			if(fabs(dt) <= APPROX_ZERO_L){	// 収束していたら解を保持し次のtへ
				t_buf[i] = t;
//				dist_buf[i] = CalcDistance(CalcNurbsCCoord(C,t),P);	// PQ間距離を得る
				dist_buf[i] = CalcDistance(CalcNurbsCCoord(t),P);	// PQ間距離を得る
				break;
			}
			loopcount++;
			t_buf[i] = dist_buf[i] = -1;		// 収束していなかったら，エラーフラグとして-1を代入
		}
	}

	// 得られた解から，PQ間の距離が最も短いものを選択
	bool flag = false;
	double min = 1E+308;
	for(int i=0;i<Divnum;i++){
		if(dist_buf[i] > 0 && dist_buf[i] < min){
			min = dist_buf[i];
			*Q = t_buf[i];
			flag = true;
		}
	}

	FreeVector(t_buf);
	FreeVector(dist_buf);
	
	return flag == true ? KOD_TRUE : KOD_FALSE;
}

// Function: GetMinDist
// (private)CalcIntersecPtNurbsPt()のサブ関数．最小距離を調べる
//
// Parameters:
// *S - NURBS曲面へのポインタ
// P - 空間上の1点
// *Q - 曲面上の点群(u,vパラメータで指定)
// N - 点数
// *Ans - 最小距離を持つ曲面上の点
//
// Return:
// 成功：KOD_TRUE, 失敗：KOD_ERR
//int NURBS_Func::GetMinDist(NURBSS *S,Coord P,Coord *Q,int N,Coord *Ans)
int NURBSS::GetMinDist(Coord P,Coord *Q,int N,Coord *Ans)
{
	double min = 1.0E+12;
	int flag = KOD_FALSE;

	for(int i=0;i<N;i++){
		if(Q[i].z == KOD_ERR)	continue;
//		Coord Q_ = CalcNurbsSCoord(S,Q[i].x,Q[i].y);
		Coord Q_ = CalcNurbsSCoord(Q[i].x,Q[i].y);
		double d = CalcDistance(Q_,P);
		if(d < min){
			min = d;
			*Ans = SetCoord(Q[i]);
		}
		flag = KOD_TRUE;
	}

	return flag;
}

// Function: DetermPtOnTRMSurf
// 注目中のNURBS曲面上の1点(u,v)がトリミング領域内にあるのかを判定する
// 
// Parameters:
// *Trim - トリム曲面
// u,v - トリム曲面上の1点(u, v)
//
// Return:
// KOD_TRUE:面上  KOD_ONEDGE:エッジ上  KOD_FALSE:面外   KOD_ERR:エラー
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

// Function: DetermPtOnTRMSurf_sub
// (private)DetermPtOnTRMSurf()のサブ関数．面上線のタイプが複合曲線の場合のトリミング領域内外判定
//
// Parameter:
// *Conps - 複合曲線
// u,v - トリム曲面上の1点(u, v)
// 
// Return:
// KOD_TRUE:面上  KOD_ONEDGE:エッジ上  KOD_FALSE:面外   KOD_ERR:エラー
int NURBS_Func::DetermPtOnTRMSurf_sub(CONPS *Conps,double u,double v)
{
	// 面上線が複合曲線になっていること
	if(Conps->BType != COMPOSITE_CURVE){
        GuiIF.SetMessage("NURBS_Func ERROR:TRIM未実装!");
		return KOD_ERR;
	}

//	COMPC *CompC=(COMPC *)Conps->pB;	// NURBS曲面のパラメータ空間上に構成されている複合曲線へのポインタを取り出す
	COMPC *CompC=Conps->pB.CompC;	// NURBS曲面のパラメータ空間上に構成されている複合曲線へのポインタを取り出す
	Coord *P;							// トリム境界線上に生成した点(多角形近似用の点)を格納
	int ptnum;							// トリム境界線を点群近似したときの点数

	// メモリ確保
//	if((P = (Coord *)malloc(sizeof(Coord)*(CompC->N*TRM_BORDERDIVNUM))) == NULL){
	P = new Coord[CompC->N*TRM_BORDERDIVNUM];
	if ( !P ) {
		return KOD_ERR;
	}

	// トリム境界線を点群Pで近似
	if((ptnum = ApproxTrimBorder(CompC,P)) == KOD_ERR){
			GuiIF.SetMessage("NURBS_Func ERROR:トリム境界線がNURBS曲線以外で構成されています.未実装!");
			FreeCoord1(P);
			return KOD_ERR;
	}
	
	int trm_flag = KOD_FALSE;							// トリミング領域内外判定用フラグ
	Coord TargetPoint = SetCoord(u,v,0);				// ターゲットとなる面上の点(u,v)をCoordに格納
	trm_flag = IsPointInPolygon(TargetPoint,P,ptnum);	// 内外判定

	return trm_flag;
}

// Function: GetPtsOnOuterTRMSurf
// 外周トリム面内の点のみ残す
//
// Parameters:
// *Trm - トリム面へのポインタ    
// *Pt - 判別対象の(u,v)群      
// N - (u,v)群の数
//
// Return:
// 残った点の数　(外周トリムが存在しない：KOD_FALSE)
int NURBS_Func::GetPtsOnOuterTRMSurf(TRMS *Trm,Coord *Pt,int N)
{
	// 外周トリムが存在しない場合は0をリターン
	if(!Trm->n1)
		return KOD_FALSE;

//	COMPC *CompC = (COMPC *)Trm->pTO->pB;	// NURBS曲面のパラメータ空間上に構成されている複合曲線へのポインタを取り出す
	COMPC *CompC = Trm->pTO->pB.CompC;	// NURBS曲面のパラメータ空間上に構成されている複合曲線へのポインタを取り出す
	Coord *P;								// トリム境界線上に生成した点(多角形近似用の点)を格納
	int ptnum;								// トリム境界線を点群近似したときの点数

	// メモリ確保
//	if((P = (Coord *)malloc(sizeof(Coord)*(CompC->N*TRM_BORDERDIVNUM))) == NULL){
	P = new Coord[CompC->N*TRM_BORDERDIVNUM];
	if ( !P ) {
		return KOD_ERR;
	}

	// トリム境界線を点群Pで近似
	if((ptnum = ApproxTrimBorder(CompC,P)) == KOD_ERR){
			GuiIF.SetMessage("NURBS_Func ERROR:トリム境界線がNURBS曲線以外で構成されています.未実装!");
			FreeCoord1(P);
			return KOD_ERR;
	}

	Coord *ans = NewCoord1(N);		// 残す点の格納先
	int trm_flag = KOD_FALSE;		// トリミング領域内外判定用フラグ
	int n=0;

	// 外側トリムの内側だけを残す
	for(int i=0;i<N;i++){
		trm_flag = IsPointInPolygon(Pt[i],P,ptnum);		// 内外判定
		if(trm_flag > 0){
			ans[n] = SetCoord(Pt[i]);
			n++;
		}
	}
	CopyCoord(ans,n,Pt);

	FreeCoord1(ans);
	FreeCoord1(P);

	return n;
}

// Function: GetPtsOnInnerTRMSurf
// 内周トリム面外の点のみ残す
//
// Parameters:
// *Trm - トリム面へのポインタ    
// *Pt - 判別対象の(u,v)群      
// N - (u,v)群の数
//
// Retrun:
// 残った点の数　(内周トリムが存在しない：KOD_FALSE)
int NURBS_Func::GetPtsOnInnerTRMSurf(TRMS *Trm,Coord *Pt,int N)
{
	// 内周トリムが存在しない場合は0をリターン
	if(!Trm->n2){
		return KOD_FALSE;
	}

	COMPC *CompC;				// NURBS曲面のパラメータ空間上に構成されている複合曲線へのポインタを取り出す
	Coord *P;					// トリム境界線上に生成した点(多角形近似用の点)を格納
	int ptnum;					// トリム境界線を点群近似したときの点数
	Coord *ans = NewCoord1(N);	// 残す点の格納先
	int trm_flag = KOD_FALSE;	// トリミング領域内外判定用フラグ
	int N_ = N;

	// 内周トリムの数だけループ
	for(int k=0;k<Trm->n2;k++){

//		CompC = (COMPC *)Trm->pTI[k]->pB;	
		CompC = Trm->pTI[k]->pB.CompC;	

		// メモリ確保
//		if((P = (Coord *)malloc(sizeof(Coord)*(CompC->N*TRM_BORDERDIVNUM))) == NULL){
		P = new Coord[CompC->N*TRM_BORDERDIVNUM];
		if ( !P ) {
			return KOD_ERR;
		}

		// トリム境界線を点群Pで近似
		if((ptnum = ApproxTrimBorder(CompC,P)) == KOD_ERR){
			GuiIF.SetMessage("NURBS_Func ERROR:トリム境界線がNURBS曲線以外で構成されています.未実装!");
			FreeCoord1(P);
			return KOD_ERR;
		}

		// 内側トリムの外側だけを残す
		int n=0;
		for(int i=0;i<N_;i++){
			trm_flag = IsPointInPolygon(Pt[i],P,ptnum);		// 内外判定
			if(trm_flag == KOD_FALSE || trm_flag == KOD_ONEDGE){
				ans[n] = SetCoord(Pt[i]);
				n++;
			}
		}
		CopyCoord(ans,n,Pt);		// ans -> Pt
		N_ = n;

		FreeCoord1(P);
	}

	FreeCoord1(ans);

	return N_;
}

// Function: GetPtsOnInnerOuterTRMSurf
// 内外周トリム面内の点のみ残す
//
// Parameters:
// *Trm - トリム面へのポインタ    
// *Pt - 判別対象の(u,v)群      
// N - (u,v)群の数
//
// Return:
// 残った点の数　(内周トリムが存在しない：KOD_FALSE)
int NURBS_Func::GetPtsOnInnerOuterTRMSurf(TRMS *Trm,Coord *Pt,int N)
{
	int n=0;

	n = GetPtsOnInnerTRMSurf(Trm,Pt,N);		// 内周トリム

	if(n == KOD_FALSE)
		n = N;

	n = GetPtsOnOuterTRMSurf(Trm,Pt,n);		// 外周トリム

	return n;
}

// Function: ApproxTrimBorder
// (private)トリム境界線を点群で近似する
//
// Parameters:
// *CompC - トリム境界線を構成する複合曲線へのポインタ
// *P - 近似された点群を格納するためのCoord配列
//
// Return:
// 近似した点群の数（トリム境界線がNURBS曲線以外で構成されていた場合は，KOD_ERR）
int NURBS_Func::ApproxTrimBorder(COMPC *CompC,Coord *P)
{
	double ent_dev=0;				// 分割点パラメータ
	NURBSC *NurbsC;					// トリム境界線(NURBS曲線)のポインタを作業用に格納
	int trm_flag = KOD_FALSE;		// トリミング領域内外判定用フラグ
	int divnum = TRM_BORDERDIVNUM;	// 各境界線の分割数
	int ptnum=0;					// 全体の点数をカウント

	// トリム境界線上に点を生成（トリム境界線を多角形近似）
	for(int i=0;i<CompC->N;i++){
		// トリム境界線がNURBS曲線で構成されている
		if(CompC->DEType[i] == NURBS_CURVE){
//			NurbsC = (NURBSC *)CompC->pDE[i];	// 注目中のNurbs曲線のポインタを取得
			NurbsC = CompC->pDE[i].NurbsC;	// 注目中のNurbs曲線のポインタを取得
			if(NurbsC->K == 2 && CompC->DegeFlag == KOD_TRUE)	divnum = 2;		// コントロールポイントが2つの場合は直線なので、分割点を生成しなくてもよくする
			else divnum = TRM_BORDERDIVNUM;
			for(int j=0;j<divnum-1;j++){
				ent_dev = NurbsC->T[NurbsC->M-1]+(NurbsC->T[NurbsC->K]-NurbsC->T[NurbsC->M-1])*(double)j/((double)divnum-1);	// 分割点tを求める
//				P[ptnum] = CalcNurbsCCoord(NurbsC,ent_dev);	// NURBS曲面のパラメータ空間内のNURBS曲線の分割点tの座標値(u,v)を得る
				P[ptnum] = NurbsC->CalcNurbsCCoord(ent_dev);	// NURBS曲面のパラメータ空間内のNURBS曲線の分割点tの座標値(u,v)を得る
				ptnum++;
			}
		}
		// それ以外
		else{
			GuiIF.SetMessage("NURBS_Func ERROR:トリム境界線がNURBS曲線以外で構成されています.未実装!");
			return KOD_ERR;
		}
	}

	return ptnum;
}

// Function: CalcDeltaPtsOnNurbsC
// 指定した分割数でNURBS曲線上の座標値を出力する
// 
// Parameters:
// *Nurb - NURBSへのポインタ  
// D - 分割数  
// *Pts - 出力される座標値を格納
//
// Return:
// 点数
//int NURBS_Func::CalcDeltaPtsOnNurbsC(NURBSC *Nurb,int D,Coord *Pts)
int NURBSC::CalcDeltaPtsOnNurbsC(int D,Coord *Pts)
{
	double T = (V[1] - V[0])/D;	// パラメトリック空間内での線分長を得る

	for(int i=0;i<=D;i++){
		Pts[i] = CalcNurbsCCoord(V[0] + T*(double)i);
	}

	return D+2;
}

// Function: CalcDeltaPtsOnNurbsC
// 指定した間隔でNURBS曲線上の座標値を出力する
//
// Parameters:
// *Nurb - NURBS曲線へのポインタ  
// D - 間隔  
// *Pts - 出力される座標値を格納
//
// Return:
// 点数（Dが0，あるいは指定したNURBS曲線の全長より長かった場合は，KOD_ERR）
//int NURBS_Func::CalcDeltaPtsOnNurbsC(NURBSC *Nurb,double D,Coord *Pts)
int NURBSC::CalcDeltaPtsOnNurbsC(double D,Coord *Pts)
{
	if(D == 0){
		GuiIF.SetMessage("NURBS_Func ERROR: Set Correct Interval Value");
		return KOD_ERR;
	}

//	double L = CalcNurbsCLength(Nurb);		// NURBS曲線の線分長を得る
	double L = CalcNurbsCLength();		// NURBS曲線の線分長を得る
	if(D > L){
		GuiIF.SetMessage("NURBS_Func ERROR: Arc Length > Whole Lenth of the Curve");
	}
	//fprintf(stderr,"L = %lf\n",L);		// debug
	//fprintf(stderr,"D = %lf\n",D);		// debug

	int k=1;			// 分割カウンタ
	double t = (V[1] - V[0])/(L/D);	// tの初期値をセット

	while(t <= V[1]){
//		t = CalcParamLengthOnNurbsC(Nurb,(double)k*D,t);	// 解を探索
//		Pts[k-1] = CalcNurbsCCoord(Nurb,t);		// 解を登録
		t = CalcParamLengthOnNurbsC((double)k*D,t);	// 解を探索
		Pts[k-1] = CalcNurbsCCoord(t);		// 解を登録
		k++;
		t = k*(V[1] - V[0])/(L/D);	// 次のtの初期値をセット
	}

	return k-1;
}

// Function: CalcParamLengthOnNurbsC
// NURBS曲線において一端からの指定距離におけるパラメータ値を返す
//
// Parameters:
// *C - NURBS曲線
// L - 指定距離
// Init_t - 解探索の初期パラメータ
//
// Return:
// 指定距離におけるパラメータ値
//double NURBS_Func::CalcParamLengthOnNurbsC(NURBSC *C,double L,double Init_t)
double NURBSC::CalcParamLengthOnNurbsC(double L,double Init_t)
{
	double dt = 1E+12;			// ステップサイズパラメータの初期値
	double t = Init_t;
	int count = 0;

	while(fabs(dt) > APPROX_ZERO){
//		dt = (L - CalcNurbsCLength(C,0,t))/CalcEuclid(CalcDiffNurbsC(C,t))/2;		// ニュートン法による収束計算
		dt = (L - CalcNurbsCLength(0,t))/CalcEuclid(CalcDiffNurbsC(t))/2;		// ニュートン法による収束計算
		t += dt;
		if(count > LOOPCOUNTMAX || t > V[1]){
			GuiIF.SetMessage("NURBS_Func ERROR: Cannot find a anser");
			break;
		}
		//fprintf(stderr,"%d:  t = %lf,    dt = %lf\n",k,t,dt);	// debug
	}

	return t;
}

// Function: CalcDeltaPtsOnNurbsS
// 指定した分割数でNURBS曲面上の座標値を求める
// 
// Parameters:
// *S - NURBSSへのポインタ  
// Du,Dv - u方向，v方向の分割数  
// **Pts - 出力される座標値を格納
//
// Return:
// 点数
//int NURBS_Func::CalcDeltaPtsOnNurbsS(NURBSS *S,int Du,int Dv,Coord **Pts)
int NURBSS::CalcDeltaPtsOnNurbsS(int Du,int Dv,Coord **Pts)
{
	double u_val = (U[1] - U[0])/Du;		// パラメトリック空間内でのu方向線分長を得る
	double v_val = (V[1] - V[0])/Dv;		// パラメトリック空間内でのv方向線分長を得る

	// u方向，v方向の各分割点における座標値を求める
	int num=0;
	for(int i=0;i<=Du;i++){
		for(int j=0;j<=Dv;j++){
//			Pts[i][j] = CalcNurbsSCoord(S,S->U[0]+u_val*i,S->V[0]+v_val*j);	// 指定した(u,v)の座標値を求める
			Pts[i][j] = CalcNurbsSCoord(U[0]+u_val*i,V[0]+v_val*j);	// 指定した(u,v)の座標値を求める
			num++;
		}
	}
	
	return num;
}

// Funciton: RemoveTheSamePoints
// (private)同一点を除去する
//
// Parameters:
// *S - 曲面 
// *Q - 曲面上の(u,v)パラメータ群(変更後の点群もここに格納される)   
// N - 点数
//
// Return:
// 変更後の点数
//int NURBS_Func::RemoveTheSamePoints(NURBSS *S,Coord *Q,int N)
int NURBSS::RemoveTheSamePoints(Coord *Q,int N)
{
	Coord *P = NewCoord1(N);

	for(int i=0;i<N;i++){
//		P[i] = CalcNurbsSCoord(S,Q[i].x,Q[i].y);
		P[i] = CalcNurbsSCoord(Q[i].x,Q[i].y);
		P[i].dmy = KOD_FALSE;
	}
	for(int i=0;i<N;i++){
		if(P[i].dmy == KOD_FALSE){
			for(int j=i+1;j<N;j++){
				if(DiffCoord(P[i],P[j],1.0e-3) == KOD_TRUE){
					P[j].dmy = KOD_TRUE;
				}
			}
		}
	}
	int k=0;
	for(int i=0;i<N;i++){
		if(P[i].dmy != KOD_TRUE){
			Q[k] = SetCoord(Q[i]);
			k++;
		}
	}

	return k;
}

// Function: CalcExtremumNurbsC
// NURBS曲線の指定した方向における極値の座標値を得る
//
// Parameters:
// *C - 極値座標を求めたいNURBS曲線へのポインタ   
// nf - 方向ベクトル     
// *pt - 得られた極値のNurbs曲線パラメータ値列    
// ptnum - *ptの配列長
//
// Return:
// 得られた極値パラメータの数（KOD_FALSE:得られなかった, KOD_ERR:極値パラメータの数がptnumを超えた）
//int NURBS_Func::CalcExtremumNurbsC(NURBSC *C,Coord nf,double *pt,int ptnum)
int NURBSC::CalcExtremumNurbsC(Coord nf,double *pt,int ptnum)
{
	int anscount=0;			// 極値の数

	// NURBS曲線のパラメータ区間をCONVDIVNUMで区切り、それぞれに対してニュートン法による収束計算を行う
	for(int i=0;i<=CONVDIVNUM;i++){
		double t = V[0] + (V[1] - V[0])/CONVDIVNUM*(double)i;	// 探索開始パラメータ値
		double dt=0;					// ニュートン法用の増分値
		int lpcount=0;					// 収束計算回数
		bool flag = false;				// 例外フラグ

		// 収束計算
		while(lpcount < LOOPCOUNTMAX){
//			double f_ = CalcInnerProduct(nf,CalcDiffNurbsC(C,t));
//			double f__ = CalcInnerProduct(nf,CalcDiff2NurbsC(C,t));
			double f_ = CalcInnerProduct(nf,CalcDiffNurbsC(t));
			double f__ = CalcInnerProduct(nf,CalcDiff2NurbsC(t));
			if(f__ == 0.0)	break;
			dt = f_/f__;

			if(CheckZero(dt,MID_ACCURACY)){			// 収束した
				flag = true;
				break;
			}
			t -= dt;	// ニュートンパラメータ更新
			if(t < V[0] || t > V[1])	break;		// 範囲外に出た
			lpcount++;
		}// End while

		// 収束していたら
		if(flag == true){
			pt[anscount] = t;	// 解として登録
			anscount++;
			if(anscount == ptnum){
                GuiIF.SetMessage("NURBS_ERROR:range over");
				return KOD_ERR;
			}
		}

	}// End for i

	anscount = CheckTheSamePoints(pt,anscount);		// 同一点を除去する

	return anscount;
}

// Function: CalcExtSearchCurve
// （準備中）極値探索線を得る
// 
// Parameters:
// *S - 対象とするNURBS曲線
// n - 法線ベクトル
// pt - 
// ds - 極値探索線を追跡する際の刻み幅
// *C1 - 得られた極値探索線（NURBS曲線）
// *C2 -  得られた極値探索線（NURBS曲線）（極地探索線は2つ得られる）
//
// Return:
// KOD_TRUE
int NURBS_Func::CalcExtSearchCurve(NURBSS *S,Coord n,Coord pt,double ds,NURBSC *C1,NURBSC *C2)
{
	// 工事中
	return KOD_TRUE;
}

// Function: CalcExtGradCurve
// （準備中）極値傾斜線を得る
//
// Parameters:
// *S - 対象とするNURBS曲線
// n - 法線ベクトル
// pt - 
// ds - 極値傾斜線を追跡する際の刻み幅
// *C1 - 得られた極値傾斜線（NURBS曲線）
// *C2 -  得られた極値傾斜線（NURBS曲線）（極値傾斜線は2つ得られる）
//
// Return:
// KOD_TRUE
int NURBS_Func::CalcExtGradCurve(NURBSS *S,Coord n,Coord pt,double ds,NURBSC *C1,NURBSC *C2)
{
	// 工事中
	return KOD_TRUE;
}

// Funciton: TrimNurbsSPlane
// NURBS曲面を平面でトリムする
//
// Parameters:
// *Trm - トリム面（トリムされた面もここに入る）
// pt - 平面上の1点
// nvec - 平面の法線ベクトル
//
// Return:
// KOD_TRUE
int NURBS_Func::TrimNurbsSPlane(TRMS *Trm,Coord pt,Coord nvec)
{
	Coord t[2000];					// 解
	int   num;						// 解の数
	double pcolor[3] = {0,1,0};		// 表示の色
	double tcolor[3] = {1,0,0};


//	num = CalcIntersecPtsPlaneSearch(Trm->pts,pt,nvec,0.5,5,t,2000,RUNGE_KUTTA);		// NURBS曲面と平面との交点群を交線追跡法で求める
	num = Trm->pts->CalcIntersecPtsPlaneSearch(pt,nvec,0.5,5,t,2000,RUNGE_KUTTA);		// NURBS曲面と平面との交点群を交線追跡法で求める
	
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
//		Coord p = CalcNurbsSCoord(Trm->pts,t[i].x,t[i].y);			// 交点をパラメータ値から座標値へ変換
		Coord p = Trm->pts->CalcNurbsSCoord(t[i].x,t[i].y);			// 交点をパラメータ値から座標値へ変換
		DrawPoint(p,1,3,pcolor);			// 交点を描画
		fprintf(fp,"%lf,%lf\n",t[i].x,t[i].y);
	}
	fclose(fp);

	return KOD_TRUE;
}

// Function: SearchExtremum_BS
// Bulirsch-Stoer法により極地探索を行う(微分方程式：du(s)/ds = fu(u,v) と、dv(s)/ds = fv(u,v)の解探索)
// 
// Parameters:
// *S - 極値探索されるNURBS曲面へのポインタ
// nf - 平面の法線ベクトル
// u0,v0 - 開始点
// H - 探索幅
// param - u方向の1階微分が0となる極値の探索(PARAM_U) or v方向探索(PARAM_V)の選択
// direction - 順方向探索(FORWARD) or逆方向探索(INVERSE)
// *ans - 更新されたu,vパラメータ(ans.x = u, ans.y = v)
//
// Return:
// KOD_TRUE:正常終了,  KOD_FALSE:特異点により処理を中断,  KOD_ERR:パラメータの指定ミスにより処理を中断
//int NURBS_Func::SearchExtremum_BS(NURBSS *S,Coord nf,double u0,double v0,double H,int param,int direction,Coord *ans)
int NURBSS::SearchExtremum_BS(Coord nf,double u0,double v0,double H,int param,int direction,Coord *ans)
{
	// 引数指定ミス
	if(direction != FORWARD && direction != INVERSE){
        GuiIF.SetMessage("NURBS ERROR: selected wrong direction");
		return KOD_ERR;
	}

	int    n[11] = {2,4,6,8,12,16,24,32,48,64,96};		// B-S法の分割数群を指定
	Coord  z[97];							// 修正中点法の中間値を格納(z.x = u, z.y = v)
	Coord  f;								// f.x = fu(u,v), f.y = fv(u,v)
	Coord  D[10][10],C[10][10],P[11];		// B-S法の中間パラメータ
	double h[11];							// B-S法の刻み幅
	Coord  R;								// h=0の外挿値
	int    conv_flag = KOD_FALSE;			// 収束フラグ

	// 各分割数における刻み幅を求めておく
	for(int i=0;i<11;i++)
		h[i] = H/n[i];
	
	// 刻み幅を小さい方から順に変更しながら、B-S法による外挿値を計算していく
	for(int i=0;i<11;i++){

		// まず、u(s+H)の値を修正中点法により計算する
		z[0] = SetCoord(u0,v0,0);										// z0とz1の算出は別処理
		if(GetSECParam1(u0,v0,nf,param,direction,&f) == KOD_FALSE)	// z0での微分方程式の右辺を計算
			return KOD_FALSE;
			//fprintf(stderr,"f%d=(%lf,%lf)\n",i,f.x,f.y);
		z[1] = AddCoord(z[0],MulCoord(f,h[i]));							// z0とz1の算出は別処理
		for(int j=1;j<n[i];j++){
			if(GetSECParam1(z[j].x,z[j].y,nf,param,direction,&f) == KOD_FALSE)	// zjでの微分方程式の右辺を計算
				return KOD_FALSE;
			z[j+1] = AddCoord(z[j-1],MulCoord(f,2*h[i]));				// z2～znまでを算出
		}
		if(GetSECParam1(z[n[i]].x,z[n[i]].y,nf,param,direction,&f) == KOD_FALSE)	// znでの微分方程式の右辺を計算
			return KOD_FALSE;
		P[i] = DivCoord(AddCoord(AddCoord(z[n[i]],z[n[i]-1]),MulCoord(f,h[i])),2);		// u(s+H)
			//fprintf(stderr,"P%d=(%lf,%lf)\n",i,P[i].x,P[i].y);

		// B-S法の差分表を順次求めていく
		if(i > 0)	R = SetCoord(P[i-1]);
		for(int k=i-1;k>=0;k--){
			double x1 = h[k]*h[k];
			double x2 = h[k+1]*h[k+1];
			if(k == i-1){
				C[k][i-1-k] = MulCoord(SubCoord(P[k+1],P[k]),x1/(x1-x2));
				D[k][i-1-k] = MulCoord(SubCoord(P[k+1],P[k]),x2/(x1-x2));
			}
			else{
				C[k][i-1-k] = MulCoord(SubCoord(C[k+1][i-2-k],D[k][i-2-k]),x1/(x1-x2));
				D[k][i-1-k] = MulCoord(SubCoord(C[k+1][i-2-k],D[k][i-2-k]),x2/(x1-x2));
			}
			R = AddCoord(R,D[k][i-1-k]);		// 外挿値
			//fprintf(stderr,"%d,D%d=(%lf,%lf)\n",i,k,D[k][i-1-k].x,D[k][i-1-k].y);
		}

		// fprintf(stderr,"%d,%lf,%.16lf\n",i,h[i],CalcEuclid2D(D[0][i-1].x,D[0][i-1].y));

		// D[0][i-1]が所定の閾値よりも小さくなったら、そのときの外挿値を解として演算処理を終了する
		if(i > 0 && CalcEuclid2D(D[0][i-1].x,D[0][i-1].y) < APPROX_ZERO_L){
			ans->x = R.x;
			ans->y = R.y;
			conv_flag = KOD_TRUE;
			break;
		}
	}

	return conv_flag;
}

// Function: GetSECParam1
// (private)極値探索線Sub関数1
// 
// Parameters:
// *S - NURBS曲面
// u, v - 注目中の(u, v)パラメータ
// nf - 平面の法線ベクトル
// param - u方向の1階微分が0となる極値の探索(PARAM_U) or v方向探索(PARAM_V)の選択
// direction - 順方向探索(FORWARD) or逆方向探索(INVERSE)
// *f - f.x = fu(u,v), f.y = fv(u,v)
// 
// Return:
// 成功：KOD_TURE, 特異点につき処理を中断した：KOD_FALSE
//int NURBS_Func::GetSECParam1(NURBSS *S,double u,double v,Coord nf,int param,int direction,Coord *f)
int NURBSS::GetSECParam1(double u,double v,Coord nf,int param,int direction,Coord *f)
{
//	double fuu = CalcInnerProduct(nf,CalcDiffNNurbsS(S,2,0,u,v));	// nf・Suu
//	double fuv = CalcInnerProduct(nf,CalcDiffNNurbsS(S,1,1,u,v));	// nf・Suv
//	double fvv = CalcInnerProduct(nf,CalcDiffNNurbsS(S,0,2,u,v));	// nf・Svv
//	Coord Su = CalcDiffuNurbsS(S,u,v);		// 曲面のu方向1階微分
//	Coord Sv = CalcDiffvNurbsS(S,u,v);		// 曲面のv方向1階微分
	double fuu = CalcInnerProduct(nf,CalcDiffNNurbsS(2,0,u,v));	// nf・Suu
	double fuv = CalcInnerProduct(nf,CalcDiffNNurbsS(1,1,u,v));	// nf・Suv
	double fvv = CalcInnerProduct(nf,CalcDiffNNurbsS(0,2,u,v));	// nf・Svv
	Coord Su = CalcDiffuNurbsS(u,v);		// 曲面のu方向1階微分
	Coord Sv = CalcDiffvNurbsS(u,v);		// 曲面のv方向1階微分
	double E = CalcInnerProduct(Su,Su);		// 1次規格量
	double F = CalcInnerProduct(Su,Sv);		// 1次規格量
	double G = CalcInnerProduct(Sv,Sv);		// 1次規格量
	if(param == PARAM_U){
		double f__ = E*fvv*fvv - 2*F*fuv*fvv + G*fuv*fuv;
		if(f__==0.0){
            GuiIF.SetMessage("NURBS KOD_ERROR:The process is stoped by detecting singular point.");
			return KOD_FALSE;				
		}
		double f_ = 1/sqrt(f__);
		*f = SetCoord(-f_*fvv*(double)direction,f_*fuv*(double)direction,0);
	}
	else if(param == PARAM_V){
		double f__ = E*fuv*fuv - 2*F*fuv*fuu + G*fuu*fuu; 
		if(f__==0.0){
            GuiIF.SetMessage("NURBS KOD_ERROR:The process is stoped by detecting singular point.");
			return KOD_FALSE;				
		}
		double f_ = 1/sqrt(f__);
		*f = SetCoord(-f_*fuv*(double)direction,f_*fuu*(double)direction,0);
	}

	return KOD_TRUE;
}

// Function: TrimNurbsSPlaneSub1
// (private)TrimNurbsSPlaneのサブ関数(2D上の2直線の交点をもとめる)
//
// Parameters:
// a,b - 1つ目の直線の係数
// x0, y0, x1, y1 - 2つ目の直線が通る2点
//
// Return:
// 交点の2D座標値
//Coord NURBS_Func::TrimNurbsSPlaneSub1(double a,double b,double x0,double y0,double x1,double y1)
Coord TrimNurbsSPlaneSub1(double a,double b,double x0,double y0,double x1,double y1)
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

// Function: GetCurveKnotParam1
// (private)各通過点の曲線パラメータを算出(コード長の比から算出)
//
// Parameters:
// *P - 通過点列   
// PNum - 通過点列の数    
// T_ - 曲線パラメータを格納
//void NURBS_Func::GetCurveKnotParam1(Coord *P,int PNum,Vector T_)
void GetCurveKnotParam1(Coord *P,int PNum,Vector T_)
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

// Function: GetCurveKnotParam2
// (private)各通過点の曲線パラメータを算出(コード長の平方根の比から算出)
//
// Parameters:
// *P - 通過点列   
// PNum - 通過点列の数    
// T_ - 曲線パラメータを格納
//void NURBS_Func::GetCurveKnotParam2(Coord *P,int PNum,Vector T_)
void GetCurveKnotParam2(Coord *P,int PNum,Vector T_)
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

// Funciton: GetSurfaceKnotParam
// (private)補間曲面用u,vパラメータを得る
// 
// Parameters:
// S - u方向曲線パラメータ
// T - v方向曲線パラメータ 
// **P - 与えられた点列
// uNum, vNum - u方向，v方向の点列数
//void NURBS_Func::GetSurfaceKnotParam(Vector S,Vector T,Coord **P,int uNum,int vNum)
void GetSurfaceKnotParam(Vector S,Vector T,Coord **P,int uNum,int vNum)
{
	double d;
	Matrix p_ = NewMatrix(uNum,vNum);

	// u方向の通過点上の曲線パラメータを得る
	for(int j=0;j<vNum;j++){
		d = 0;
		for(int i=1;i<uNum;i++){
			d += CalcDistance(P[i][j],P[i-1][j]);
		}
		for(int i=0;i<uNum;i++){
			if(i==0)
				p_[i][j] = 0;
			else if(i==uNum-1)
				p_[i][j] = 1;
			else
				p_[i][j] = p_[i-1][j] + CalcDistance(P[i][j],P[i-1][j])/d;
		}
	}
	for(int i=0;i<uNum;i++){
		S[i] = 0;
		for(int j=0;j<vNum;j++){
			S[i] += p_[i][j];
		}
		S[i] /= (double)vNum;
	}

	// v方向の通過点上の曲線パラメータを得る
	for(int i=0;i<uNum;i++){
		d = 0;
		for(int j=1;j<vNum;j++){
			d += CalcDistance(P[i][j],P[i][j-1]);
		}
		for(int j=0;j<vNum;j++){
			if(j==0)
				p_[i][j] = 0;
			else if(j==vNum-1)
				p_[i][j] = 1;
			else
				p_[i][j] = p_[i][j-1] + CalcDistance(P[i][j],P[i][j-1])/d;
		}
	}
	for(int j=0;j<vNum;j++){
		T[j] = 0;
		for(int i=0;i<uNum;i++){
			T[j] += p_[i][j];
		}
		T[j] /= (double)uNum;
	}
	
	FreeMatrix(p_,uNum);
}

// Function: GetEqIntervalKont
// (private)曲線/曲面パラメータから等間隔なノットベクトルを算出
// 
// Parameters:
// K - コントロールポイントの数  
// M - 階数   
// T - 格納するノットベクトル列
//void NURBS_Func::GetEqIntervalKont(int K,int M,Vector T)
void GetEqIntervalKont(int K,int M,Vector T)
{
	for(int i=0;i<M;i++)
		T[i] = 0;
	for(int i=M;i<K;i++)
		T[i] = ((double)i-(double)M+1)/((double)K-(double)M+1)*NORM_KNOT_VAL;
	for(int i=K;i<K+M;i++)
		T[i] = NORM_KNOT_VAL;
}

// Function: GetInterpolatedKnot
// (private)曲線/曲面パラメータから補間用ノットベクトルを算出
// 
// Parameters:
// T_ - 曲線パラメータ列  
// N - ノットベクトルの数  
// K - コントロールポイントの数  
// M - 階数   
// T - 格納するノットベクトル列
//void NURBS_Func::GetInterpolatedKnot(Vector T_,int N,int K,int M,Vector T)
void GetInterpolatedKnot(Vector T_,int N,int K,int M,Vector T)
{
	for(int i=0;i<M;i++)
		T[i] = 0;

	// T_を参考にする
	for(int j=1;j<K-M+1;j++){
		double d=0;
		for(int i=j;i<j+M-1;i++){
			d += T_[i];
		}
		T[j+M-1] = d/((double)M-1);
	}

	// 等間隔に設定
	//for(int i=M;i<K;i++)
	//	T[i] = ((double)i-(double)M+1)/((double)K-(double)M+1);

	for(int i=K;i<K+M;i++)
		T[i] = 1;
}

// Function: GetApproximatedKnot
// (private)曲線/曲面パラメータから近似用ノットベクトルを算出
// 
// Parameters:
// T_ - 曲線パラメータ列  
// N - 曲線パラメータの数  
// M - 階数  
// K - コントロールポイントの数  
// T - 格納するノットベクトル列
//void NURBS_Func::GetApproximatedKnot(Vector T_,int N,int M,int K,Vector T)
void GetApproximatedKnot(Vector T_,int N,int M,int K,Vector T)
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

// Function: ChangeKnotVecRange
// (private)曲線/曲面パラメータの定義域を変更する
// 
// Parameters:
// T - 変更したいノットベクトル列
// N - Tの配列長
// M - 階数
// K - コントロールポイントの数
// Tst - 開始ノットベクトル
// Te - 終了ノットベクトル
//void NURBS_Func::ChangeKnotVecRange(Vector T, int N, int M, int K, double Ts, double Te)
void ChangeKnotVecRange(Vector T, int N, int M, int K, double Ts, double Te)
{
	Vector T_ = NewVector(N);
	
	for(int i=0;i<N;i++)
		T_[i] = (Te-Ts)/(T[K]-T[M-1])*T[i] + (Ts*T[K]-Te*T[M-1])/(T[K]-T[M-1]);

	for(int i=0;i<N;i++)
		T[i] = T_[i];

	FreeVector(T_);
}

// Function: CalcApproximationCP_LSM
// (private)最小2乗法で近似コントロールポイントを求める
// 
// Parameters:
// *P - 通過点列  
// T_ - 曲線パラメータ列  
// T - ノットベクトル  
// Pnum - 曲線パラメータの数  
// Nnum - ノットベクトルの数  
// M - 階数  
// K - コントロールポイントの数   
// *Q - 算出されたコントロールポイント列
//void NURBS_Func::CalcApproximationCP_LSM(Coord *P,Vector T_,Vector T,int Pnum,int Nnum,int M,int K,Coord *Q)
void CalcApproximationCP_LSM(Coord *P,Vector T_,Vector T,int Pnum,int Nnum,int M,int K,Coord *Q)
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

// Function: SetApproximationCPnum
// (private)点列数から生成するコントロールポイント数を算定する（勘です。）
// 
// Parameters:
// PNum - 点列数
//
// Return:
// コントロールポイントの数
//int NURBS_Func::SetApproximationCPnum(int PNum)
int SetApproximationCPnum(int PNum)
{
	if(PNum < 5)		// 勘
		return PNum;
	else if(PNum < 10)	// 勘
		return PNum-1;
	else 
		return PNum/2;	// 勘
}
#ifdef _DEBUG
// Function: DebugForNurbsC
// NURBS曲線情報をデバッグプリント
//
// Parameters:
// *nurbs - デバッグするNURBS曲線
//void NURBS_Func::DebugForNurbsC(NURBSC *nurbs)
void NURBSC::DebugForNurbsC(void)
{
	fprintf(stderr,"Cp num: %d\n",K);
	fprintf(stderr,"Rank: %d\n",M);
	fprintf(stderr,"Knot num: %d\n",N);
	fprintf(stderr,"Knot range: %lf - %lf\n",V[0], V[1]);

	// コントロールポイント
	fprintf(stderr,"Control Point\n");
	for(int i=0;i<K;i++){
		fprintf(stderr,"#%d: (%lf,%lf,%lf)\t",i+1,cp[i].x,cp[i].y,cp[i].z);
	}
	fprintf(stderr,"\n");

	// ノットシーケンス
	fprintf(stderr,"Knot Vector\t");
	for(int i=0;i<K+M;i++){
		fprintf(stderr,"#%d: %lf\t",i+1,T[i]);
	}
	fprintf(stderr,"\n");

	// ウェイト
	fprintf(stderr,"Weight\n");
	for(int i=0;i<K;i++){
		fprintf(stderr,"#%d: %lf\t",i+1,W[i]);
	}
}

// Function: DebugForNurbsS
// NURBS曲面情報をデバッグプリント
//
// Parameters:
// *nurbs - デバッグするNURBS曲面
//void NURBS_Func::DebugForNurbsS(NURBSS *nurbs)
void NURBSS::DebugForNurbsS(void)
{
	fprintf(stderr,"Cp num: %d-%d\n",K[0],K[1]);
	fprintf(stderr,"Rank: %d-%d\n",M[0],M[1]);
	fprintf(stderr,"Knot num: %d-%d\n",N[0],N[1]);
	fprintf(stderr,"Knot range: (%lf - %lf),(%lf - %lf)\n",U[0],U[1],V[0],V[1]);

	// コントロールポイント
	fprintf(stderr,"Control Point\n");
	for(int i=0;i<K[0];i++){
		for(int j=0;j<K[1];j++){
			fprintf(stderr,"#(%d-%d): (%lf,%lf,%lf)\t",i+1,j+1,cp[i][j].x,cp[i][j].y,cp[i][j].z);
		}
	}
	fprintf(stderr,"\n");

	// U方向ノットシーケンス
	fprintf(stderr,"U Knot Vector\t");
	for(int i=0;i<K[0]+M[0];i++){
		fprintf(stderr,"#%d: %lf\t",i+1,S[i]);
	}
	fprintf(stderr,"\n");

	// V方向ノットシーケンス
	fprintf(stderr,"V Knot Vector\t");
	for(int i=0;i<K[1]+M[1];i++){
		fprintf(stderr,"#%d: %lf\t",i+1,T[i]);
	}
	fprintf(stderr,"\n");

	// ウェイト
	//fprintf(stderr,"Weight\n");
	//for(int i=0;i<nurbs->K[0];i++){
	//	for(int j=0;j<nurbs->K[1];j++){
	//		fprintf(stderr,"#(%d-%d): %lf\t",i+1,j+1,nurbs->W[i][j]);
	//	}
	//}
}
#endif
// Function: CalcNurbsCLength
// NURBS曲線C(t)の指定パラメータ区間[a,b]の線分長Lを求める
//
// L = S|C'(t)|dt	(Sは積分記号)
//
// 積分は数値積分(ガウス-ルジャンドルの80分点)を用いる
//
// Parameters:
// *Nurb - 対象となるNURBS曲線
// a, b - 指定パラメータ区間[a,b]
//
// Return:
// 線分長
//double NURBS_Func::CalcNurbsCLength(NURBSC *Nurb,double a,double b)
double NURBSC::CalcNurbsCLength(double a,double b)
{
	double g[80] = {-0.9995538226516306298800804990945671849917
		,-0.997649864398237688899494208183122985331
		,-0.994227540965688277892063503664911698088
		,-0.989291302499755531026503167136631385282
		,-0.982848572738629070418288027709116473568
		,-0.974909140585727793385645230069136276245
		,-0.965485089043799251452273155671454998502
		,-0.954590766343634905493481517021029508783
		,-0.942242761309872674752266004500001735070
		,-0.928459877172445795953045959075453133792
		,-0.913263102571757654164733656150947478111
		,-0.896675579438770683194324071967395986307
		,-0.878722567678213828703773343639124407935
		,-0.859431406663111096977192123491656492839
		,-0.838831473580255275616623043902867064793
		,-0.816954138681463470371124994012295707742
		,-0.793832717504605449948639311738454358610
		,-0.769502420135041373865616068749026083985
		,-0.744000297583597272316540527930913673808
		,-0.717365185362099880254068258293815278566
		,-0.689637644342027600771207612438935266089
		,-0.660859898986119801735967122844317234805
		,-0.631075773046871966247928387289336863089
		,-0.600330622829751743154746299164006848430
		,-0.568671268122709784725485786624827158742
		,-0.536145920897131932019857253125400904911
		,-0.502804111888784987593672750367568003564
		,-0.468696615170544477036078364935808657294
		,-0.433875370831756093062386700363181958302
		,-0.398393405881969227024379642517533757117
		,-0.362304753499487315619043286358963588017
		,-0.325664370747701914619112943627358695037
		,-0.288528054884511853109139301434713898496
		,-0.250952358392272120493158816035004797363
		,-0.212994502857666132572388538666321823094
		,-0.174712291832646812559339048011286195718
		,-0.136164022809143886559241078000717067933
		,-0.097408398441584599063278450104936902017
		,-0.058504437152420668628993321883417794425
		,-0.019511383256793997654351234107454547933
		,0.0195113832567939976543512341074545479335
		,0.0585044371524206686289933218834177944254
		,0.0974083984415845990632784501049369020170
		,0.1361640228091438865592410780007170679331
		,0.1747122918326468125593390480112861957188
		,0.2129945028576661325723885386663218230948
		,0.2509523583922721204931588160350047973630
		,0.2885280548845118531091393014347138984964
		,0.3256643707477019146191129436273586950370
		,0.3623047534994873156190432863589635880171
		,0.3983934058819692270243796425175337571172
		,0.4338753708317560930623867003631819583021
		,0.4686966151705444770360783649358086572940
		,0.5028041118887849875936727503675680035649
		,0.5361459208971319320198572531254009049117
		,0.5686712681227097847254857866248271587420
		,0.6003306228297517431547462991640068484301
		,0.6310757730468719662479283872893368630891
		,0.6608598989861198017359671228443172348051
		,0.6896376443420276007712076124389352660897
		,0.7173651853620998802540682582938152785668
		,0.7440002975835972723165405279309136738087
		,0.7695024201350413738656160687490260839854
		,0.7938327175046054499486393117384543586106
		,0.8169541386814634703711249940122957077428
		,0.8388314735802552756166230439028670647936
		,0.8594314066631110969771921234916564928399
		,0.8787225676782138287037733436391244079359
		,0.8966755794387706831943240719673959863073
		,0.9132631025717576541647336561509474781115
		,0.9284598771724457959530459590754531337922
		,0.9422427613098726747522660045000017350708
		,0.9545907663436349054934815170210295087836
		,0.9654850890437992514522731556714549985029
		,0.9749091405857277933856452300691362762450
		,0.9828485727386290704182880277091164735687
		,0.9892913024997555310265031671366313852822
		,0.9942275409656882778920635036649116980888
		,0.9976498643982376888994942081831229853311
		,0.9995538226516306298800804990945671849917
	};
	double w[80] = {0.00114495000318694153454417194131563611869939240558
		,0.0026635335895126816692935358316684554657445542424
		,0.0041803131246948952367393042016813513235494973731
		,0.0056909224514031986492691071171620184769252638347
		,0.0071929047681173127526755708679565074765070381923
		,0.0086839452692608584264094522040342813524060429550
		,0.0101617660411030645208318503524069436640457818796
		,0.0116241141207978269164667699954326348595131815029
		,0.0130687615924013392937868258970563403104186343824
		,0.0144935080405090761169620745834605500559568721551
		,0.0158961835837256880449029092291785257709720926057
		,0.0172746520562693063585842071312909998003110293040
		,0.0186268142082990314287354141521572090084477663361
		,0.0199506108781419989288919287151135633605010642850
		,0.0212440261157820063887107372506131285464689242433
		,0.0225050902463324619262215896861687390205795883718
		,0.0237318828659301012931925246135684162923425291083
		,0.0249225357641154911051178470032198023571024898755
		,0.0260752357675651179029687436002692871256974758292
		,0.0271882275004863806744187066805442598298771757001
		,0.0282598160572768623967531979650145302942654983731
		,0.0292883695832678476927675860195791396612074311446
		,0.0302723217595579806612200100909011747473420675596
		,0.0312101741881147016424428667206035518659997208202
		,0.0321004986734877731480564902872506960895167638325
		,0.0329419393976454013828361809019595361280270376927
		,0.0337332149846115228166751630642387284458265038481
		,0.0344731204517539287943642267310298320767807967429
		,0.0351605290447475934955265923886968812291624523105
		,0.0357943939534160546028615888161544542402361352305
		,0.0363737499058359780439649910465228136600628217876
		,0.0368977146382760088391509965734052192685681011318
		,0.0373654902387304900267053770578386691648069079494
		,0.0377763643620013974897749764263210547707019240195
		,0.0381297113144776383442067915657362019141439239065
		,0.0384249930069594231852124363294901384310218762709
		,0.0386617597740764633270771102671566912609009278398
		,0.0388396510590519689317741826687871658908802293404
		,0.0389583959627695311986255247722608223149320115862
		,0.0390178136563066548112804392527540483295504740296
		,0.0390178136563066548112804392527540483295504740296
		,0.0389583959627695311986255247722608223149320115862
		,0.0388396510590519689317741826687871658908802293404
		,0.0386617597740764633270771102671566912609009278398
		,0.0384249930069594231852124363294901384310218762709
		,0.0381297113144776383442067915657362019141439239065
		,0.0377763643620013974897749764263210547707019240195
		,0.0373654902387304900267053770578386691648069079494
		,0.0368977146382760088391509965734052192685681011318
		,0.0363737499058359780439649910465228136600628217876
		,0.0357943939534160546028615888161544542402361352305
		,0.0351605290447475934955265923886968812291624523105
		,0.0344731204517539287943642267310298320767807967429
		,0.0337332149846115228166751630642387284458265038481
		,0.0329419393976454013828361809019595361280270376927
		,0.0321004986734877731480564902872506960895167638325
		,0.0312101741881147016424428667206035518659997208202
		,0.0302723217595579806612200100909011747473420675596
		,0.0292883695832678476927675860195791396612074311446
		,0.0282598160572768623967531979650145302942654983731
		,0.0271882275004863806744187066805442598298771757001
		,0.0260752357675651179029687436002692871256974758292
		,0.0249225357641154911051178470032198023571024898755
		,0.0237318828659301012931925246135684162923425291083
		,0.0225050902463324619262215896861687390205795883718
		,0.0212440261157820063887107372506131285464689242433
		,0.0199506108781419989288919287151135633605010642850
		,0.0186268142082990314287354141521572090084477663361
		,0.0172746520562693063585842071312909998003110293040
		,0.0158961835837256880449029092291785257709720926057
		,0.0144935080405090761169620745834605500559568721551
		,0.0130687615924013392937868258970563403104186343824
		,0.0116241141207978269164667699954326348595131815029
		,0.0101617660411030645208318503524069436640457818796
		,0.0086839452692608584264094522040342813524060429550
		,0.0071929047681173127526755708679565074765070381923
		,0.0056909224514031986492691071171620184769252638347
		,0.0041803131246948952367393042016813513235494973731
		,0.0026635335895126816692935358316684554657445542424
		,0.0011449500031869415345441719413156361186993924055
	};

	double A = (b+a)/2;
	double B = (b-a)/2;
	double len=0;

	for(int i=0;i<80;i++){
		double xi = A+B*g[i];
		len += w[i]*(CalcEuclid(CalcDiffNurbsC(xi)));
	}
	return(B*len);
}

// Function: CalcNurbsCLength
// NURBS曲線C(t)の全区間の線分長Lを求める
//
// L = S|C'(t)|dt	(Sは積分記号)
//
// 積分は数値積分(ガウス-ルジャンドルの80分点)を用いる
//
// Parameters:
// *Nurb - 対象となるNURBS曲線
//
// Return:
// 線分長
//double NURBS_Func::CalcNurbsCLength(NURBSC *Nurb)
double NURBSC::CalcNurbsCLength(void)
{
	double g[80] = {-0.9995538226516306298800804990945671849917
		,-0.997649864398237688899494208183122985331
		,-0.994227540965688277892063503664911698088
		,-0.989291302499755531026503167136631385282
		,-0.982848572738629070418288027709116473568
		,-0.974909140585727793385645230069136276245
		,-0.965485089043799251452273155671454998502
		,-0.954590766343634905493481517021029508783
		,-0.942242761309872674752266004500001735070
		,-0.928459877172445795953045959075453133792
		,-0.913263102571757654164733656150947478111
		,-0.896675579438770683194324071967395986307
		,-0.878722567678213828703773343639124407935
		,-0.859431406663111096977192123491656492839
		,-0.838831473580255275616623043902867064793
		,-0.816954138681463470371124994012295707742
		,-0.793832717504605449948639311738454358610
		,-0.769502420135041373865616068749026083985
		,-0.744000297583597272316540527930913673808
		,-0.717365185362099880254068258293815278566
		,-0.689637644342027600771207612438935266089
		,-0.660859898986119801735967122844317234805
		,-0.631075773046871966247928387289336863089
		,-0.600330622829751743154746299164006848430
		,-0.568671268122709784725485786624827158742
		,-0.536145920897131932019857253125400904911
		,-0.502804111888784987593672750367568003564
		,-0.468696615170544477036078364935808657294
		,-0.433875370831756093062386700363181958302
		,-0.398393405881969227024379642517533757117
		,-0.362304753499487315619043286358963588017
		,-0.325664370747701914619112943627358695037
		,-0.288528054884511853109139301434713898496
		,-0.250952358392272120493158816035004797363
		,-0.212994502857666132572388538666321823094
		,-0.174712291832646812559339048011286195718
		,-0.136164022809143886559241078000717067933
		,-0.097408398441584599063278450104936902017
		,-0.058504437152420668628993321883417794425
		,-0.019511383256793997654351234107454547933
		,0.0195113832567939976543512341074545479335
		,0.0585044371524206686289933218834177944254
		,0.0974083984415845990632784501049369020170
		,0.1361640228091438865592410780007170679331
		,0.1747122918326468125593390480112861957188
		,0.2129945028576661325723885386663218230948
		,0.2509523583922721204931588160350047973630
		,0.2885280548845118531091393014347138984964
		,0.3256643707477019146191129436273586950370
		,0.3623047534994873156190432863589635880171
		,0.3983934058819692270243796425175337571172
		,0.4338753708317560930623867003631819583021
		,0.4686966151705444770360783649358086572940
		,0.5028041118887849875936727503675680035649
		,0.5361459208971319320198572531254009049117
		,0.5686712681227097847254857866248271587420
		,0.6003306228297517431547462991640068484301
		,0.6310757730468719662479283872893368630891
		,0.6608598989861198017359671228443172348051
		,0.6896376443420276007712076124389352660897
		,0.7173651853620998802540682582938152785668
		,0.7440002975835972723165405279309136738087
		,0.7695024201350413738656160687490260839854
		,0.7938327175046054499486393117384543586106
		,0.8169541386814634703711249940122957077428
		,0.8388314735802552756166230439028670647936
		,0.8594314066631110969771921234916564928399
		,0.8787225676782138287037733436391244079359
		,0.8966755794387706831943240719673959863073
		,0.9132631025717576541647336561509474781115
		,0.9284598771724457959530459590754531337922
		,0.9422427613098726747522660045000017350708
		,0.9545907663436349054934815170210295087836
		,0.9654850890437992514522731556714549985029
		,0.9749091405857277933856452300691362762450
		,0.9828485727386290704182880277091164735687
		,0.9892913024997555310265031671366313852822
		,0.9942275409656882778920635036649116980888
		,0.9976498643982376888994942081831229853311
		,0.9995538226516306298800804990945671849917
	};
	double w[80] = {0.00114495000318694153454417194131563611869939240558
		,0.0026635335895126816692935358316684554657445542424
		,0.0041803131246948952367393042016813513235494973731
		,0.0056909224514031986492691071171620184769252638347
		,0.0071929047681173127526755708679565074765070381923
		,0.0086839452692608584264094522040342813524060429550
		,0.0101617660411030645208318503524069436640457818796
		,0.0116241141207978269164667699954326348595131815029
		,0.0130687615924013392937868258970563403104186343824
		,0.0144935080405090761169620745834605500559568721551
		,0.0158961835837256880449029092291785257709720926057
		,0.0172746520562693063585842071312909998003110293040
		,0.0186268142082990314287354141521572090084477663361
		,0.0199506108781419989288919287151135633605010642850
		,0.0212440261157820063887107372506131285464689242433
		,0.0225050902463324619262215896861687390205795883718
		,0.0237318828659301012931925246135684162923425291083
		,0.0249225357641154911051178470032198023571024898755
		,0.0260752357675651179029687436002692871256974758292
		,0.0271882275004863806744187066805442598298771757001
		,0.0282598160572768623967531979650145302942654983731
		,0.0292883695832678476927675860195791396612074311446
		,0.0302723217595579806612200100909011747473420675596
		,0.0312101741881147016424428667206035518659997208202
		,0.0321004986734877731480564902872506960895167638325
		,0.0329419393976454013828361809019595361280270376927
		,0.0337332149846115228166751630642387284458265038481
		,0.0344731204517539287943642267310298320767807967429
		,0.0351605290447475934955265923886968812291624523105
		,0.0357943939534160546028615888161544542402361352305
		,0.0363737499058359780439649910465228136600628217876
		,0.0368977146382760088391509965734052192685681011318
		,0.0373654902387304900267053770578386691648069079494
		,0.0377763643620013974897749764263210547707019240195
		,0.0381297113144776383442067915657362019141439239065
		,0.0384249930069594231852124363294901384310218762709
		,0.0386617597740764633270771102671566912609009278398
		,0.0388396510590519689317741826687871658908802293404
		,0.0389583959627695311986255247722608223149320115862
		,0.0390178136563066548112804392527540483295504740296
		,0.0390178136563066548112804392527540483295504740296
		,0.0389583959627695311986255247722608223149320115862
		,0.0388396510590519689317741826687871658908802293404
		,0.0386617597740764633270771102671566912609009278398
		,0.0384249930069594231852124363294901384310218762709
		,0.0381297113144776383442067915657362019141439239065
		,0.0377763643620013974897749764263210547707019240195
		,0.0373654902387304900267053770578386691648069079494
		,0.0368977146382760088391509965734052192685681011318
		,0.0363737499058359780439649910465228136600628217876
		,0.0357943939534160546028615888161544542402361352305
		,0.0351605290447475934955265923886968812291624523105
		,0.0344731204517539287943642267310298320767807967429
		,0.0337332149846115228166751630642387284458265038481
		,0.0329419393976454013828361809019595361280270376927
		,0.0321004986734877731480564902872506960895167638325
		,0.0312101741881147016424428667206035518659997208202
		,0.0302723217595579806612200100909011747473420675596
		,0.0292883695832678476927675860195791396612074311446
		,0.0282598160572768623967531979650145302942654983731
		,0.0271882275004863806744187066805442598298771757001
		,0.0260752357675651179029687436002692871256974758292
		,0.0249225357641154911051178470032198023571024898755
		,0.0237318828659301012931925246135684162923425291083
		,0.0225050902463324619262215896861687390205795883718
		,0.0212440261157820063887107372506131285464689242433
		,0.0199506108781419989288919287151135633605010642850
		,0.0186268142082990314287354141521572090084477663361
		,0.0172746520562693063585842071312909998003110293040
		,0.0158961835837256880449029092291785257709720926057
		,0.0144935080405090761169620745834605500559568721551
		,0.0130687615924013392937868258970563403104186343824
		,0.0116241141207978269164667699954326348595131815029
		,0.0101617660411030645208318503524069436640457818796
		,0.0086839452692608584264094522040342813524060429550
		,0.0071929047681173127526755708679565074765070381923
		,0.0056909224514031986492691071171620184769252638347
		,0.0041803131246948952367393042016813513235494973731
		,0.0026635335895126816692935358316684554657445542424
		,0.0011449500031869415345441719413156361186993924055
	};

	double A = (V[1]+V[0])/2;
	double B = (V[1]-V[0])/2;
	double len=0;

	for(int i=0;i<80;i++){
		double xi = A+B*g[i];
		len += w[i]*(CalcEuclid(CalcDiffNurbsC(xi)));
	}
	return(B*len);
}

// Function: GetMinDistance
// (private)最小距離を持つ座標値を返す
//
// Parameters:
// a - 対象とする1点
// *b - 探索する点群
// n - 点群の数
//
// Return:
// 最小距離となる点b_min
//Coord NURBS_Func::GetMinDistance(Coord a,Coord *b,int n)
Coord GetMinDistance(Coord a,Coord *b,int n)
{
	if(!n)	return SetCoord(0,0,0);

	Vector d = NewVector(n);

	for(int i=0;i<n;i++){
		d[i] = CalcDistance(a,b[i]);
	}

	double min = 1.0E+12;
	int min_num;
	for(int i=0;i<n;i++){
		if(d[i] < min){
			min = d[i];
			min_num = i;
		}
	}

	FreeVector(d);

	return b[min_num];
}

// Function: DivNurbsC
// NURBS曲線を指定した位置（端からの弧長）で分割する
//
// Parameters:
// *C0 - 分割するNURBS曲線へのポインタ        
// *C1 - 分割されたNURBS曲線へのポインタ
// *C2 - 分割されたNURBS曲線へのポインタ     
// L - 端からの弧長
//
// Return:
// 成功：KOD_TRUE, 失敗：KOD_FALSE 
int NURBS_Func::DivNurbsC(NURBSC *C0, NURBSC *C1, NURBSC *C2, double L)
{
//	double dLEN = CalcNurbsCLength(C0);					// NURBS曲線の線分長を得る
	double dLEN = C0->CalcNurbsCLength();					// NURBS曲線の線分長を得る
	double t_init = (C0->V[1] - C0->V[0])*L/dLEN;		// tの初期値をセット
//	double t = CalcParamLengthOnNurbsC(C0,L,t_init);	// 分割点パラメータ値取得
	double t = C0->CalcParamLengthOnNurbsC(L,t_init);	// 分割点パラメータ値取得

	int iKOD = DivNurbsCParam(C0,C1,C2,t);		// 分割

	return iKOD;

}

// Function: DivNurbsCParam
// NURBS曲線を指定したパラメータ値で分割する
//
// Parameters:
// *C0 - 分割するNURBS曲線へのポインタ        
// *C1 - 分割されたNURBS曲線へのポインタ
// *C2 - 分割されたNURBS曲線へのポインタ    
// t - 分割位置を表す曲線パラメータ
//
// Return:
// 成功：KOD_TRUE, 失敗：KOD_FALSE 
int NURBS_Func::DivNurbsCParam(NURBSC *C0, NURBSC *C1, NURBSC *C2, double t)
{
	// tパラメータが適正範囲か
	if(t <= C0->T[0] || t >= C0->T[C0->N-1]){
		GuiIF.SetMessage("NURBS_Func ERROR: Wrong Curve Parameter is set.");
		return KOD_ERR;
	}

	int deg = C0->M - 1;		// 多重度

	// 分割の下準備
	// 分割用曲線C0_を準備する
	NURBSC C0_;
	C0_.K = C0->K + deg;
	C0_.N = C0->M + C0_.K;
	New_NurbsC(&C0_,C0_.K,C0_.N);

	// C0のノットベクトルにtと同じ値がある場合は，多重度を1つ落とす
	for(int i=0;i<C0->N;i++){
		if(t == C0->T[i])	deg--;
	}

	// 分割位置パラメータtをC0_に挿入する
	int k = InsertNewKnotOnNurbsC(C0,&C0_,t,deg);

	// 2本の分割曲線を生成
	int N1 = k+1;
	int K1 = N1 - C0->M;
	int N2 = C0_.N - k + deg+1;
	int K2 = N2 - C0->M;

	Vector T1 = NewVector(N1);
	Vector W1 = NewVector(K1);
	Coord  *cp1 = NewCoord1(K1);
	Vector T2 = NewVector(N2);
	Vector W2 = NewVector(K2);
	Coord  *cp2 = NewCoord1(K2);

	// ノットベクトル，コントロールポイント，ウェイトをC1,C2に分配
	for(int i=0;i<N1-1;i++)
		T1[i] = C0_.T[i];
	T1[N1-1] = t;
	for(int i=0;i<K1;i++){
		cp1[i] = SetCoord(C0_.cp[i]);
		W1[i] = C0_.W[i];
	}
	for(int i=0;i<C0->M;i++)
		T2[i] = t;
	for(int i=C0->M;i<N2;i++)
		T2[i] = C0_.T[k+i-C0->M];
	for(int i=0;i<K2;i++){
		cp2[i] = SetCoord(C0_.cp[i+K1-1]);
		W2[i] = C0_.W[i+K1-1];
	}

	// debug
	//fprintf(stderr,"C0:%d,%d\n",C0->K,C0->N);
	//fprintf(stderr,"C0_:%d,%d\n",C0_.K,C0_.N);
	//fprintf(stderr,"C1:%d,%d\n",K1,N1);
	//fprintf(stderr,"C2:%d,%d\n",K2,N2);
	//fprintf(stderr,"\n");
	//for(int i=0;i<C0_.N;i++)
	//	fprintf(stderr,"%d:%lf\n",i+1,C0_.T[i]);
	//fprintf(stderr,"\n");
	//for(int i=0;i<N1;i++)
	//	fprintf(stderr,"%d:%lf\n",i+1,T1[i]);
	//fprintf(stderr,"\n");
	//for(int i=0;i<N2;i++)
	//	fprintf(stderr,"%d:%lf\n",i+1,T2[i]);

	// ノットの範囲を0-1に変更
	ChangeKnotVecRange(T1,N1,C0->M,K1,0,1);
	ChangeKnotVecRange(T2,N2,C0->M,K2,0,1);

	// C1,C2生成
	GenNurbsC(C1,K1,C0->M,N1,T1,W1,cp1,C0->V,C0->prop,0);
	GenNurbsC(C2,K2,C0->M,N2,T2,W2,cp2,C0->V,C0->prop,0);
	
	FreeVector(T1);
	FreeVector(W1);
	FreeCoord1(cp1);
	FreeVector(T2);
	FreeVector(W2);
	FreeCoord1(cp2);

	return KOD_TRUE;
}

// Function: ConnectNurbsC
// 2本のNURBS曲線を連結する
//
// Parameter:
// *C1 - 曲線1
// *C2 - 曲線2
// *C_ - 連結後の曲線を格納
//
// Return:
// 成功：KOD_TRUE,  失敗：KOD_FALSE
int NURBS_Func::ConnectNurbsC(NURBSC *C1,NURBSC *C2,NURBSC *C_)
{
	int flag = -1;		// 連結位置判別用フラグ

	// 2曲線の連結位置を調べ，連結点がC1(1), C2(0)となるようどちらかの曲線を調整する
	if(DiffCoord(C1->cp[0],C2->cp[0]) == KOD_TRUE){
//		ReverseNurbsC(C1);				// C1の向きを反転する
		C1->ReverseNurbsC();				// C1の向きを反転する
	}
	else if(DiffCoord(C1->cp[0],C2->cp[C2->K-1]) == KOD_TRUE){
		NURBSC *C;
		C = C2;
		C2 = C1;
		C1 = C;
	}
	else if(DiffCoord(C1->cp[C1->K-1],C2->cp[0]) == KOD_TRUE){
		// このケースはOK．特に調整必要なし
	}
	else if(DiffCoord(C1->cp[C1->K-1],C2->cp[C2->K-1]) == KOD_TRUE){
//		ReverseNurbsC(C2);				// C2の向きを反転する
		C2->ReverseNurbsC();				// C2の向きを反転する
	}
	else{
		GuiIF.SetMessage("NURBS_Func ERROR: Two Curves don't share the same coordinate value.");
		return KOD_ERR;
	}

	// 2曲線の階数が等しいこと
	if(C1->M != C2->M){
		GuiIF.SetMessage("NURBS_Func ERROR: Two Curves don't have the same rank.");
		return KOD_ERR;
	}

	int K = C1->K + C2->K - 1;				// C_のコントロールポイントの数
	int N = C1->N + C2->N - C2->M - 1;		// C_のノットベクトルの数

	New_NurbsC(C_,K,N);						// C_内のメモリー確保

	SetKnotVecC_ConnectC(C1,C2,C_);			// C_のノット定義域を指定

	SetCPC_ConnectC(C1,C2,C_);				// C_のコントロールポイントとウェイトを指定

	//for(int i=0;i<C_->N;i++)
	//	fprintf(stderr,"%d,%lf\n",i+1,C_->T[i]);
	//fprintf(stderr,"\n");
	//for(int i=0;i<C_->K;i++)
	//	fprintf(stderr,"%d,%lf,%lf,%lf,%lf\n",i+1,C_->cp[i].x,C_->cp[i].y,C_->cp[i].z,C_->W[i]);

	C_->M = C1->M;							// C_の階数を指定

	for(int i=0;i<4;i++)
		C_->prop[i] = C1->prop[i];
	C_->EntUseFlag = C1->EntUseFlag;

	return KOD_TRUE;
}

// Function: ReverseNurbsC
// NURBS曲線のノットベクトル向きを反転する
//
// Parameters:
// *C - NURBS曲線 
//void NURBS_Func::ReverseNurbsC(NURBSC *C)
void NURBSC::ReverseNurbsC(void)
{
	Reverse(W,K);
	Reverse(cp,K);
	Reverse(T,N);
//	Reverse(W.get(),K);
//	Reverse(cp.get(),K);
//	Reverse(T.get(),N);
	for(int i=0;i<N;i++)
		T[i] *= -1;
	ChangeKnotVecRange(T,N,M,K,0,1);
//	ChangeKnotVecRange(T.get(),N,M,K,0,1);
}

// Function: SetKnotVecC_ConnectC
// (private)2本の曲線を繋げたときのノットベクトルを設定する
// 
// Parameters:
// *C1, *Cs - 連結する2つのNURBS曲線
// *C_ - 連結後のNURBS曲線
void NURBS_Func::SetKnotVecC_ConnectC(NURBSC *C1,NURBSC *C2,NURBSC *C_)
{
	// コード長を調べる
	double s=0,e=NORM_KNOT_VAL,c=0;			// 開始，終了，連結部ノットベクトル
	double l1=0,l2=0;						// 各曲線のノットベクトルのコード長
	for(int i=0;i<C1->N-1;i++) {
//		l1 += CalcDistance(CalcNurbsCCoord(C1,C1->T[i+1]),CalcNurbsCCoord(C1,C1->T[i]));	// C1のコード長
		l1 += CalcDistance(C1->CalcNurbsCCoord(C1->T[i+1]),C1->CalcNurbsCCoord(C1->T[i]));	// C1のコード長
	}
	for(int i=0;i<C2->N-1;i++) {
//		l2 += CalcDistance(CalcNurbsCCoord(C2,C2->T[i+1]),CalcNurbsCCoord(C2,C2->T[i]));	// C2のコード長
		l2 += CalcDistance(C2->CalcNurbsCCoord(C2->T[i+1]),C2->CalcNurbsCCoord(C2->T[i]));	// C2のコード長
	}
	c = l1/(l1+l2);	// 結合点のノットベクトル値

	// C_のノットベクトル範囲を得る
	Vector T1 = NewVector(C1->N);	
	Vector T2 = NewVector(C2->N);	
	CopyVector(C1->T,C1->N,T1);		// C1のノットベクトルをT1にコピー
	CopyVector(C2->T,C2->N,T2);		// C2のノットベクトルをT2にコピー
//	CopyVector(C1->T.get(),C1->N,T1);		// C1のノットベクトルをT1にコピー
//	CopyVector(C2->T.get(),C2->N,T2);		// C2のノットベクトルをT2にコピー
	ChangeKnotVecRange(T1,C1->N,C1->M,C1->K,s,c);	// C1(T1)のノットベクトルの範囲を変更
	ChangeKnotVecRange(T2,C2->N,C2->M,C2->K,c,e);	// C2(U2)のノットベクトルの範囲を変更
	C_->V[0] = s;						// C_のノットベクトルの範囲
	C_->V[1] = e;
	C_->N = C1->N + C2->N - C2->M - 1;	// C_のノットベクトル数

	// C_のノットベクトルを得る
	for(int i=0;i<C1->K;i++)
		C_->T[i] = T1[i];
	for(int i=1;i<C2->N;i++)
		C_->T[C1->K+i-1] = T2[i];

	FreeVector(T1);
	FreeVector(T2);
}

// Function: SetCPC_ConnectC
// (private)2本の曲線を繋げたときのコントロールポイントとウェイトを設定する
// 
// Parameters:
// *C1, *C2 - 連結する2つのNURBS曲線
// *C_ - 連結後のNURBS曲線
void NURBS_Func::SetCPC_ConnectC(NURBSC *C1,NURBSC *C2,NURBSC *C_)
{
	C_->K = C1->K + C2->K - 1;

	for(int i=0;i<C1->K;i++){
		C_->cp[i] = SetCoord(C1->cp[i]);
		C_->W[i] = C1->W[i];
	}
	for(int i=1;i<C2->K;i++){
		C_->cp[C1->K+i-1] = SetCoord(C2->cp[i]);
		C_->W[C1->K+i-1] = C2->W[i];
	}
}

// Function: InsertNewKnotOnNurbsC
// (private)NURBS曲線に新たなノットを挿入する
//
// Parameters:
// *C - 元のNURBS曲線  
// *C_ - 挿入対象のNURBS曲線     
// t - 挿入するノット     
// deg - 多重度
//
// Return:
// 新たなノットベクトル列におけるtの挿入位置
int NURBS_Func::InsertNewKnotOnNurbsC(NURBSC *C,NURBSC *C_,double t,int deg)
{
	int k=0;					// tの挿入位置
	int m = C->M;				// 階数
	int n = C->K;				// コントロールポイントの数

	double *T_buf = NewVector(C->K+C->M+deg);	// ノットベクトル一時格納用バッファ
	Coord *cp_buf = NewCoord1(C->K+deg);			// コントロールポイント一時格納用バッファ
	double *W_buf = NewVector(C->K+deg);		// ウェイト一時格納用バッファ
	//double T_buf[C->K+C->M+deg];
	//Coord  cp_buf[C->K+deg];
	//double W_buf[C->K+deg];


	// C_に元のNURBS曲線のT,cp,Wを初期値として代入
	for(int i=0;i<m+n;i++)
		C_->T[i] = C->T[i];
	for(int i=0;i<n;i++)
		C_->cp[i] = SetCoord(C->cp[i]);
	for(int i=0;i<n;i++)
		C_->W[i] = C->W[i];

	// 多重度分，tの挿入を繰り返す
	for(int count=0;count<deg;count++){
		// 各bufにC_のT,cp,Wを代入
		for(int i=0;i<n+m;i++)
			T_buf[i] = C_->T[i];
		for(int i=0;i<n;i++)
			cp_buf[i] = SetCoord(C_->cp[i]);
		for(int i=0;i<n;i++)
			W_buf[i] = C_->W[i];

		// tの挿入位置kを調べる
		k=0;
		for(int i=0;i<n+m-1;i++){
			if(t >= T_buf[i] && t < T_buf[i+1]){
				k = i;
				break;
			}
		}

		// C_のノットベクトルを更新
		for(int i=0;i<=k;i++)
			C_->T[i] = T_buf[i];
		C_->T[k+1] = t;
		for(int i=k+2;i<=n+m;i++)
			C_->T[i] = T_buf[i-1];

		// コントロールポイントとウェイトの更新
		for(int i=0;i<=k-m+1;i++){
			C_->cp[i] = SetCoord(cp_buf[i]);
			C_->W[i] = W_buf[i];
		}
		for(int i=k-m+2;i<=k;i++){
			double a = (t-T_buf[i])/(T_buf[i+m-1]-T_buf[i]);
			C_->cp[i] = AddCoord(MulCoord(cp_buf[i-1],1-a),MulCoord(cp_buf[i],a));
			C_->W[i] = (1-a)*W_buf[i-1] + a*W_buf[i];
		}
		for(int i=k+1;i<=n;i++){
			C_->cp[i] = SetCoord(cp_buf[i-1]);
			C_->W[i] = W_buf[i-1];
		}

		n++;
	}

	FreeVector(T_buf);
	FreeVector(W_buf);
	FreeCoord1(cp_buf);

	return k+2;
}
