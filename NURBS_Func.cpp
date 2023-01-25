#include "NURBS_Func.h"


// 1つのNurbs曲線を生成する
int NURBS_Func::GenNurbsC(NURBSC *Nurbs,int K,int M,int N,double T[],double W[],Coord cp[])
{
	int i;

	Nurbs->K = K;
	Nurbs->M = M;
	Nurbs->N = N;
	Nurbs->T = (double *)malloc(sizeof(double)*Nurbs->N);
	Nurbs->W = (double *)malloc(sizeof(double)*Nurbs->K);
	Nurbs->cp = (Coord *)malloc(sizeof(Coord)*Nurbs->K);
	for(i=0;i<N;i++){
		Nurbs->T[i] = T[i];
	}
	for(i=0;i<K;i++){
		Nurbs->W[i] = W[i];
		Nurbs->cp[i] = SetCoord(cp[i]);
	}

	return TRUE;
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

	return TRUE;
}

// GenNurbsC()によって生成されたNURBS曲線を削除する
void NURBS_Func::DelNurbsC(NURBSC *Nurbs)
{
	Free_NurbsC(Nurbs);
}

// 1つのNurbs曲面を生成する(NURBS曲面のコピー)
// 引数　*Nurbs:新たに生成するNURBS曲面　nurb:代入元
int NURBS_Func::GenNurbsS(NURBSS *Nurbs,NURBSS nurb)
{
	int errflag=0;
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
	if(New_NurbsS(Nurbs,Nurbs->K,Nurbs->N) == ERR){
		fprintf(stderr,"NURBS_Func ERROR:メモリーの確保に失敗しました\n");
		return ERR;
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

	return TRUE;
}

// GenNurbsS()によって生成されたNURBS曲面を削除する
void NURBS_Func::DelNurbsS(NURBSS *Nurbs)
{
	Free_NurbsS(Nurbs);
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

	New_TrmS(TNurbs,tnurb.n2);								// トリム面のメモリー確保

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

	return TRUE;
}

// GenTrimdNurbsS()によって生成されたトリム面を削除する
int NURBS_Func::DelTrimdNurbsS(TRIMD_NURBSS *TNurbs)
{
	int curve_num = 0;


	// トリム面を構成する全てのNURBS曲線の本数を調べる
	for(int i=0;i<TNurbs->n2;i++){
		for(int j=0;j<TNurbs->pTI[i]->pB->CompC.N;j++){
			curve_num++;
		}
	}
	curve_num += TNurbs->pTO->pB->CompC.N;

	Free_NurbsC_1DArray((NURBSC *)TNurbs->pTO->pB->CompC.pDE[0],curve_num);		// トリム面を構成する全てのNURBS曲線パラメータのメモリー解放

	Free_NurbsS(TNurbs->pts);						// トリム面を構成するNURBS曲面パラメータのメモリー解放
	free(TNurbs->pts);								// トリム面を構成するNURBS曲面のメモリー解放

	Free_NurbsC(&TNurbs->pTO->pB->CompC.DegeNurbs);	// トリム面外周を構成する複合曲線を構成する縮退用NURBS曲線のメモリー解放
	Free_CompC((COMPC *)TNurbs->pTO->pB);			// トリム面外周を構成する複合曲線を構成するNURBS曲線のメモリー解放
	free(TNurbs->pTO->pB);							// トリム面外周を構成する複合曲線のメモリー解放
	free(TNurbs->pTO);								// トリム面外周を構成する面上線のメモリー解放

	for(int i=0;i<TNurbs->n2;i++){
		Free_NurbsC(&TNurbs->pTI[i]->pB->CompC.DegeNurbs);	// トリム面内周を構成する複合曲線を構成する縮退用NURBS曲線のメモリー解放
		Free_CompC((COMPC *)TNurbs->pTI[i]->pB);	// トリム面内周を構成する複合曲線を構成するNURBS曲線のメモリー解放
		free(TNurbs->pTI[i]->pB);					// トリム面内周を構成する複合曲線のメモリー解放
	}
	Free_TrmS(TNurbs);								// トリム面パラメータのメモリー解放

	return TRUE;
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
		bs = CalcBSbasis(t,NurbsC->T,i,NurbsC->M);		// Bスプライン基底関数を求める
		bsw += bs*NurbsC->W[i];							// 分母
		bscpw = AddCoord(bscpw,MulCoord(NurbsC->cp[i],bs*NurbsC->W[i]));	// 分子
	}
	
	p = DivCoord(bscpw,bsw);	// 座標値を求める

	return p;
}


// 指定したノットtでのNURBS曲面の座標値を求める
Coord NURBS_Func::CalcNurbsSCoord(NURBSS *NurbsS,double div_u,double div_v)
{
	int i,j;
	double bs_u,bs_v;		// u,v方向Bスプライン基底関数
	double bsw=0;			// 分母
	Coord bscpw;			// 分子
	Coord p;

	InitCoord(&bscpw);		// 初期化

	for(i=0;i<NurbsS->K[0];i++){
		bs_u = CalcBSbasis(div_u,NurbsS->S,i,NurbsS->M[0]);			// u方向Bスプライン基底関数を求める
		for(j=0;j<NurbsS->K[1];j++){
			bs_v = CalcBSbasis(div_v,NurbsS->T,j,NurbsS->M[1]);		// v方向Bスプライン基底関数を求める
			bsw += bs_u*bs_v*NurbsS->W[i][j];
			bscpw = AddCoord(bscpw,MulCoord(NurbsS->cp[i][j],bs_u*bs_v*NurbsS->W[i][j]));
		}
	}

	p = DivCoord(bscpw,bsw);

	return p;
}

// Bスプライン基底関数を計算し、計算結果を返す
// t:ノット　knot[]:ノットベクトル  span:注目中のコントロールポイント  order:階数
double NURBS_Func::CalcBSbasis(double t, double knot[],int span,int order)
{
	double n1=0.0;
	double n2=0.0;

	// 階数(order)が1の時
	if( order == 1 ){
		if( knot[span] <= t && t <= knot[span+1] ){
			return 1.0;
		}
		else{
			return 0.0;
		}
	}

	// それ以外の時
	else{
		n1 = knot[ span + order - 1 ] - knot[span];
		if( n1 != 0.0 ){
			n1 = ( t - knot[span] ) / n1 * CalcBSbasis( t, knot, span, order - 1 );
		}

		n2 = knot[ span + order ] - knot[ span + 1 ];
		if( n2 != 0.0 ){
			n2 = ( knot[ span + order ] - t ) / n2 * CalcBSbasis( t, knot, span+1, order-1 );
		}

		return( n1 + n2 );
	}
}

// Bスプライン基底関数の1階微分係数を求める
// t:ノット　knot[]:ノットベクトル  span:注目中のコントロールポイント  order:階数
double NURBS_Func::CalcDiffBSbasis(double t,double knot[],int span,int order)
{
	double n1 = 0.0, n2 = 0.0;
	
	n1 = knot[span + order - 1] - knot[span];
	if( n1 != 0.0 ){
		n1 = (order - 1) / n1 * CalcBSbasis( t, knot, span, order-1 );
	}
	n2 = knot[ span + order ] - knot[ span + 1 ];
	if( n2 != 0.0 ){
		n2 = (order - 1) / n2 * CalcBSbasis( t, knot, span+1, order-1 );
	}
	
	return( n1 - n2 );
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
		bs = CalcBSbasis(t,NurbsC->T,i,NurbsC->M);
		diff_bs = CalcDiffBSbasis(t,NurbsC->T,i,NurbsC->M);

		Ft = AddCoord(Ft,MulCoord(NurbsC->cp[i],bs*NurbsC->W[i]));
		diff_Ft = AddCoord(diff_Ft,MulCoord(NurbsC->cp[i],diff_bs*NurbsC->W[i]));

		Gt += bs*NurbsC->W[i];
		diff_Gt += diff_bs*NurbsC->W[i];
	}

	// 1階微分を求める
	p = SubCoord(DivCoord(diff_Ft,Gt),DivCoord(MulCoord(Ft,diff_Gt),Gt*Gt));

	return p;
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
		bs_u = CalcBSbasis(div_u,NurbsS->S,i,NurbsS->M[0]);				// u方向Bスプライン基底関数を求める
		diff_bs_u = CalcDiffBSbasis(div_u,NurbsS->S,i,NurbsS->M[0]);	// u方向Bスプライン基底関数の1階微分を求める
		for(j=0;j<NurbsS->K[1];j++){
			bs_v = CalcBSbasis(div_v,NurbsS->T,j,NurbsS->M[1]);			// v方向Bスプライン基底関数を求める
			Ft = AddCoord(Ft,MulCoord(NurbsS->cp[i][j],bs_u*bs_v*NurbsS->W[i][j]));
			diff_Ft = AddCoord(diff_Ft,MulCoord(NurbsS->cp[i][j],diff_bs_u*bs_v*NurbsS->W[i][j]));
			Gt += bs_u*bs_v*NurbsS->W[i][j];
			diff_Gt += diff_bs_u*bs_v*NurbsS->W[i][j];
		}
	}

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
		bs_u = CalcBSbasis(div_u,NurbsS->S,i,NurbsS->M[0]);				// u方向Bスプライン基底関数を求める
		for(j=0;j<NurbsS->K[1];j++){
			bs_v = CalcBSbasis(div_v,NurbsS->T,j,NurbsS->M[1]);				// v方向Bスプライン基底関数を求める
			diff_bs_v = CalcDiffBSbasis(div_v,NurbsS->T,j,NurbsS->M[1]);	// v方向Bスプライン基底関数の1階微分を求める
			Ft = AddCoord(Ft,MulCoord(NurbsS->cp[i][j],bs_u*bs_v*NurbsS->W[i][j]));
			diff_Ft = AddCoord(diff_Ft,MulCoord(NurbsS->cp[i][j],bs_u*diff_bs_v*NurbsS->W[i][j]));
			Gt += bs_u*bs_v*NurbsS->W[i][j];
			diff_Gt += bs_u*diff_bs_v*NurbsS->W[i][j];
		}
	}

	// 1階微分を求める
	p = SubCoord(DivCoord(diff_Ft,Gt),DivCoord(MulCoord(Ft,diff_Gt),Gt*Gt));

	return p;
}

// NURBS曲面上の(u,v)における法線ベクトルをもとめる
Coord NURBS_Func::CalcNormVecOnNurbsS(NURBSS *nurb,double u,double v)
{
	Coord a = CalcDiffuNurbsS(nurb,u,v);
	Coord b = CalcDiffvNurbsS(nurb,u,v);

	return(NormalizeVec(CalcOuterProduct(a,b)));
}

// 3次以下のNURBS曲面と平面との交点群を代数解法にて求める(vパラメータ分割)
// 引数  *nurb:NURBS曲面  pt:平面上の一点  nvec:平面の法線ベクトル  v_divnum:vパラメータ分割数
//       *ans:算出された交点のu,vパラメータ値をそれぞれans.x,ans.yに格納,  ans_size:ansの配列長
// 返値  交点の個数(交点の数がans_sizeを超えた：ERR)
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
			N[i] = CalcBSbasis(v_const,nurb->T,i,nurb->M[1]);		// v_const時のBスプライン基底関数を求める
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
				fprintf(stderr,"NURBS ERR:交点の数が確保された配列長を超えました\n");
				allansnum = ERR;
				goto EXIT;
			}
		}
	}

EXIT:
	free(N);
	free(A);
	free(B);
	FreeMatrix(coef);

	return allansnum;
}

// 3次以下のNURBS曲面と平面との交点群を代数解法にて求める(uパラメータ分割)
// 引数  *nurb:NURBS曲面  pt:平面上の一点  nvec:平面の法線ベクトル  u_divnum:uパラメータ分割数
//       *ans:算出された交点のu,vパラメータ値ををそれぞれans.x,ans.yに格納,  ans_size:ansの配列長
// 返値  交点の個数(交点の数がans_sizeを超えた：ERR)
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
			N[i] = CalcBSbasis(u_const,nurb->S,i,nurb->M[0]);		// u_const時のBスプライン基底関数を求める
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
				fprintf(stderr,"NURBS ERR:交点の数が確保された配列長を超えました\n");
				allansnum = ERR;
				goto EXIT;
			}
		}
	}

EXIT:
	free(N);
	free(A);
	free(B);
	FreeMatrix(coef);

	return allansnum;
}

// 3次以下のNURBS曲面と平面との交点群を交点追跡法にて求める
// 引数　NURBSS:NURBS曲面  pt:平面上に1点  nvec:平面の法線ベクトル  ds:交線(交点群)の粗さ(密0.1～2疎)  ans:解  ans_size:解の数
// 返値　FALSE:NURBS曲面と平面が交差していない　ERR:特異点または発散により処理を中断
int NURBS_Func::CalcIntersecPtsPlaneSearch3(NURBSS *nurb,Coord pt,Coord nvec,double ds,Coord *ans,int ans_size)
{
	int ans_count=0;		// 追跡点の総数
	int loop_count=0;		// 収束計算のループ数
	int pnow=0;
	Coord init_pt[INTERSECPTNUMMAX];		// 初期点(u,vパラメータ値)
	Coord init_pt_Coord[INTERSECPTNUMMAX];	// 初期点(x,y,z座標値)
	bool  init_pt_flag[INTERSECPTNUMMAX];	// 各初期点を通り終えたかを判別するフラグ
	bool  init_allpt_flag=false;			// 初期点を全て通り終えたかを判別するフラグ
	int   init_pt_num;						// 初期点の数
	double u,v;								// 交線追跡中のu,vパラメータ中間値
	double d = CalcInnerProduct(pt,nvec);	// 原点から平面までの距離
	double dist;							// ループ脱出用に追跡点間距離を閾値とする
	//FILE *fp=fopen("debug.csv","w");
	//double color[3] = {1,0,0};

	// 初期点通過判別フラグを初期化
	init_pt_flag[0] = true;
	for(int i=1;i<INTERSECPTNUMMAX;i++){
		init_pt_flag[i] = false;
	}

	// まず交線追跡法の初期点として交点をいくつか求める
	init_pt_num = CalcIntersecPtsPlaneV3(nurb,pt,nvec,10,init_pt,INTERSECPTNUMMAX);
	if(!init_pt_num){
		init_pt_num = CalcIntersecPtsPlaneV3(nurb,pt,nvec,100,init_pt,INTERSECPTNUMMAX);	// 交点が見つからなかった場合は分割数を細かくしてもう一度サーチ
	}
	if(!init_pt_num){
		init_pt_num = CalcIntersecPtsPlaneU3(nurb,pt,nvec,10,init_pt,INTERSECPTNUMMAX);		// それでも交点が見つからなかった場合は方向を変えてサーチ
	}
	if(!init_pt_num){
		init_pt_num = CalcIntersecPtsPlaneU3(nurb,pt,nvec,100,init_pt,INTERSECPTNUMMAX);	// それでも交点が見つからなかった場合は分割数を細かくしてもう一度サーチ
	}
	if(!init_pt_num){		// それでも見つからない場合は、交差していないとみなす
		fprintf(stderr,"NURBS ERROR:交点が得られませんでした\n");
		return FALSE;					
	}
	else if(init_pt_num == ERR) return ERR;			// init_pt_numがinit_ptの配列長を超えた

	for(int i=0;i<init_pt_num;i++){
		init_pt_Coord[i] = CalcNurbsSCoord(nurb,init_pt[i].x,init_pt[i].y);		// 交点のuvパラメータをxyz座標値に変換したものを保持しておく
		//fprintf(fp,"%lf,%lf\n",init_pt_Coord[i]);
		//DrawPoint(init_pt_Coord[i],1,3,color);
	}

	ans[ans_count] = SetCoord(init_pt[0]);
	ans_count++;

	// 初期点を全て通過するまで交線追跡法を繰り返す
	while(init_allpt_flag == false){

		// 交線追跡のための始点(u,v)をセット
		u = ans[ans_count].x = init_pt[pnow].x;
		v = ans[ans_count].y = init_pt[pnow].y;
		init_pt_flag[pnow] = true;

		// 交線追跡開始
		loop_count = 0;
		while(1){
			// まず初期値としてのdu,dvを求める
			Coord pu = CalcDiffuNurbsS(nurb,u,v);
			Coord pv = CalcDiffvNurbsS(nurb,u,v);
			double phi = CalcInnerProduct(nvec,CalcNurbsSCoord(nurb,u,v));
			double phi_u = CalcInnerProduct(nvec,pu);
			double phi_v = CalcInnerProduct(nvec,pv);
			double E = CalcInnerProduct(pu,pu);
			double F = CalcInnerProduct(pu,pv);
			double G = CalcInnerProduct(pv,pv);
			double f = sqrt(E*phi_v*phi_v - 2*F*phi_u*phi_v + G*phi_u*phi_u); 
			if(!f){			// 特異点
				fprintf(stderr,"NURBS ERROR:特異点検出により処理を中断します\n");
				return ERR;				
			}
			double f1 = 1/f;
			double f2 = -f1;
			double du = -f2*phi_v*ds;		// 初期値
			double dv = f2*phi_u*ds;		// 初期値

			// ニュートン法を用いてu,vを真値に収束させる
			int k=0;
			if(fabs(dv) > fabs(du)){				// dv>duの場合はdvを定数として固定する
				while(fabs(du) > CONVERG_GAP){		// duが収束するまで繰り返し計算
					phi = CalcInnerProduct(nvec,CalcNurbsSCoord(nurb,u,v));
					phi_u = CalcInnerProduct(nvec,CalcDiffuNurbsS(nurb,u,v));
					phi_v = CalcInnerProduct(nvec,CalcDiffvNurbsS(nurb,u,v));
					du = (d-phi-phi_v*dv)/phi_u;
					u += du;
					if(u < nurb->U[0] || u > nurb->U[1] || k > LOOPCOUNTMAX){
						fprintf(stderr,"NURBS ERROR:収束計算に失敗しました\n");
						return ERR;
					}
					k++;
				}
				v += dv;
			}
			else{									// dv<duの場合はduを定数として固定する
				while(fabs(dv) > CONVERG_GAP){		// dvが収束するまで繰り返し計算
					phi = CalcInnerProduct(nvec,CalcNurbsSCoord(nurb,u,v));
					phi_u = CalcInnerProduct(nvec,CalcDiffuNurbsS(nurb,u,v));
					phi_v = CalcInnerProduct(nvec,CalcDiffvNurbsS(nurb,u,v));
					dv = (d-phi-phi_u*du)/phi_v;
					v += dv;
					if(v < nurb->V[0] || v > nurb->V[1] || k>LOOPCOUNTMAX){
						fprintf(stderr,"NURBS ERROR:収束計算に失敗しました\n");
						return ERR;
					}
					k++;
				}
				u += du;
			}

			Coord p = CalcNurbsSCoord(nurb,u,v);			// 得られたu,vをxyz座標値に変換
			dist = CalcEuclidPnts3D(init_pt_Coord[pnow],p);	// 得られたxyz座標値と初期点との距離を算出

			//fprintf(fp,"%d,%lf,%lf,%lf,%lf\n",ans_count,p.x,p.y,u,v);

			ans_count++;		// 交点群の数をインクリメント

			if(ans_count == ans_size){
				fprintf(stderr,"NURBS ERROR:交点の数が確保された配列長を超えました\n");
				return ans_count;
			}

			// 最初に求めた初期点が交線追跡法によって全て通過したか調べる
			for(int i=0;i<init_pt_num;i++){
				if(init_pt_flag[i] == false){
					if(CalcEuclidPnts3D(init_pt_Coord[i],p) < ds)
						init_pt_flag[i] = true;
					else
						init_pt_flag[i] = false;
				}
			}

			// u,vが取り得るパラメータ範囲（0～1）を超えた場合または、１周して戻ってきた場合はループを抜ける
			if(u < nurb->U[0] || u > nurb->U[1] || v < nurb->V[0] || v > nurb->V[1] || (dist < ds/2 && loop_count > 0))
				break;

			// 得られたu,vを交線(交点群)として登録
			ans[ans_count].x = u;
			ans[ans_count].y = v;

			loop_count++;		// ループ回数をインクリメント

		}// 交線追跡ここまで

		// 残った点があれば、別の交線があるので、その点を始点として再度交線追跡を行う
		init_allpt_flag = true;
		for(int i=0;i<init_pt_num;i++){
			if(init_pt_flag[i] == false){
				init_allpt_flag = false;
				pnow = i;
				break;
			}
		}
	}

	//fclose(fp);

	return ans_count;
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
		fprintf(stderr,"NURBS ERROR: CalcIntersecPlane3() \n");
		return ERR;
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
			fprintf(stderr,"NURBS ERROR:次数未対応(次数 = %d)\n",nurb->M-1);
			goto EXIT;
		}
		GetNurbsCCoef(nurb,coef,i,P,Q);						// NURBS曲線の係数(P,Q)を求める
		GetIntersecEquation(nurb->M,P,Q,pt,nvec,a);			// NURBS曲線と平面の交線導出用方程式を得る
		ansnum = CalcEquation(a,t,nurb->M-1);					// 方程式を解き、交点のパラメータ値を得る

		for(int j=0;j<ansnum;j++){
			if(t[j] >= nurb->T[i+nurb->M-1] && t[j] <= nurb->T[i+nurb->M]){	// ノットベクトルの値と適合するもののみ解として抽出
				if(k == ans_size){
					fprintf(stderr,"NURBS ERROR:交点の数が確保された配列長を超えました\n");
					goto EXIT;
				}
				ans[k] = t[j];		// 解を取得
				k++;				// 解の数をインクリメント
			}
		}
	}

	return k;

EXIT:
	FreeMatrix(coef);
	return ERR;
}

// 3次方程式までを解く
int NURBS_Func::CalcEquation(double *a,double *t,int M)
{
	int flag;

	if(M == 3)		flag = CalcCubicEquation(a,t);
	else if(M == 2)	flag = CalcQuadraticEquation(a,t);
	else if(M == 1) flag = CalcLinearEquation(a,t);
	else			return ERR;

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
	
	return TRUE;
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

	return TRUE;
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

	return TRUE;
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

	return TRUE;
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
		fprintf(stderr,"NURBS ERROR:コントロールポイントの数が異なります\n");
		return ERR;
	}

	for(int i=0;i<Nurbs.K[0];i++){
		for(int j=0;j<Nurbs.K[1];j++){
			nurbs->cp[i][j] = SetCoord(Nurbs.cp[i][j]);
		}
	}

	return TRUE;
}