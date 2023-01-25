/***************
* BODY描画コア *
****************/

#include "Describe_BODY.h"

GLUnurbsObj *Describe_BODY::NurbsSurf;		// NURBS曲面用オブジェクト
GLUnurbsObj *Describe_BODY::NurbsCurve;		// NURBS曲線用オブジェクト
int Describe_BODY::DrawBODYFlag;			// BODY描画フラグ
int Describe_BODY::DrawSurfFlag;			// Surface描画フラグ
int Describe_BODY::DrawBODY1st;
GLfloat Describe_BODY::uKnot[KNOTNUMMAX];
GLfloat Describe_BODY::vKnot[KNOTNUMMAX];
GLfloat Describe_BODY::CCtlp[CTLPNUMMAX][4];
GLfloat Describe_BODY::SCtlp[CTLPNUMMAX][CTLPNUMMAX][4];

// コンストラクタ
Describe_BODY::Describe_BODY()
{
	//uKnot = (GLfloat *)malloc(sizeof(GLfloat)*KNOTNUMMAX);
	//vKnot = (GLfloat *)malloc(sizeof(GLfloat)*KNOTNUMMAX);

	//CCtlp = (GLfloat **)malloc(sizeof(GLfloat *)*CTLPNUMMAX);
	//SCtlp = (GLfloat ***)malloc(sizeof(GLfloat **)*CTLPNUMMAX);
	//for(int i=0;i<CTLPNUMMAX;i++){
	//	CCtlp[i] = (GLfloat *)malloc(sizeof(GLfloat)*4);
	//	SCtlp[i] = (GLfloat **)malloc(sizeof(GLfloat *)*CTLPNUMMAX);
	//	for(int j=0;j<CTLPNUMMAX;j++){
	//		SCtlp[i][j] = (GLfloat *)malloc(sizeof(GLfloat)*4);
	//	}
	//}
}

// デストラクタ
Describe_BODY::~Describe_BODY()
{
	//free(uKnot);
	//free(vKnot);

	//for(int i=0;i<CTLPNUMMAX;i++){
	//	for(int j=0;j<CTLPNUMMAX;j++){
	//		free(SCtlp[i][j]);
	//	}
	//	free(CCtlp[i]);
	//	free(SCtlp[i]);
	//}
	//free(CCtlp);
	//free(SCtlp);
}


// 線分の描画
void Describe_BODY::DrawLine(LINE_ Line)
{
	glLineWidth(1);

	glBegin(GL_LINE_STRIP);
	glVertex3d(Line.cp[0].x,Line.cp[0].y,Line.cp[0].z);	// 始点
	glVertex3d(Line.cp[1].x,Line.cp[1].y,Line.cp[1].z);	// 終点
	glEnd();

}

// NURBS曲線の描画
void Describe_BODY::DrawNurbsCurve(NURBSC NurbsC)
{
	int i,j;

	for(i=0;i<NurbsC.K;i++){			// コントロールポイント取り出し
		CCtlp[i][0] = NurbsC.cp[i].x*NurbsC.W[i];
		CCtlp[i][1] = NurbsC.cp[i].y*NurbsC.W[i];
		CCtlp[i][2] = NurbsC.cp[i].z*NurbsC.W[i];
		CCtlp[i][3] = NurbsC.W[i];
	}
	for(j=0;j<NurbsC.N;j++){			// ノットベクトル取り出し
		uKnot[j] = NurbsC.T[j];
	}

	glDisable(GL_LIGHTING);
	gluBeginCurve(NurbsCurve);
	gluNurbsCurve(NurbsCurve,NurbsC.N,uKnot,4,&CCtlp[0][0],NurbsC.M,GL_MAP1_VERTEX_4);
	gluEndCurve(NurbsCurve);
	glFlush();
	glEnable(GL_LIGHTING);

}


// NURBS曲面の描画(トリムあり)
void Describe_BODY::DrawTrimdNurbsSurfe(NURBSS *NurbsS)
{
	int j,k;

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT,NurbsS->Dstat.Color);
	for(k=0;k<NurbsS->K[1];k++){
		for(j=0;j<NurbsS->K[0];j++){
			SCtlp[j][k][0] = NurbsS->cp[j][k].x*NurbsS->W[j][k];	// コントロールポイント取り出し
			SCtlp[j][k][1] = NurbsS->cp[j][k].y*NurbsS->W[j][k];
			SCtlp[j][k][2] = NurbsS->cp[j][k].z*NurbsS->W[j][k];
			SCtlp[j][k][3] = NurbsS->W[j][k];
		}
	}
	for(j=0;j<NurbsS->N[0];j++){
		uKnot[j] = NurbsS->S[j];		// uノットベクトル取り出し
	}
	for(j=0;j<NurbsS->N[1];j++){
		vKnot[j] = NurbsS->T[j];		// vノットベクトル取り出し
	}

	// NURBS曲面の描画
	gluNurbsSurface(NurbsSurf,NurbsS->N[0],uKnot,NurbsS->N[1],vKnot,CTLPNUMMAX*4,4,&SCtlp[0][0][0],NurbsS->M[0],NurbsS->M[1],GL_MAP2_VERTEX_4);

}

// NURBS曲面の描画(トリムなし)
void Describe_BODY::DrawNurbsSurfe(NURBSS NurbsS)
{
	int j,k;

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT,NurbsS.Dstat.Color);
	for(k=0;k<NurbsS.K[1];k++){
		for(j=0;j<NurbsS.K[0];j++){
			SCtlp[j][k][0] = NurbsS.cp[j][k].x*NurbsS.W[j][k];	// コントロールポイント取り出し
			SCtlp[j][k][1] = NurbsS.cp[j][k].y*NurbsS.W[j][k];
			SCtlp[j][k][2] = NurbsS.cp[j][k].z*NurbsS.W[j][k];
			SCtlp[j][k][3] = NurbsS.W[j][k];
		}
	}
	for(j=0;j<NurbsS.N[0];j++){
		uKnot[j] = NurbsS.S[j];		// uノットベクトル取り出し
	}
	for(j=0;j<NurbsS.N[1];j++){
		vKnot[j] = NurbsS.T[j];		// vノットベクトル取り出し
	}

	// NURBS曲面の描画
	gluBeginSurface(NurbsSurf);
	gluNurbsSurface(NurbsSurf,NurbsS.N[0],uKnot,NurbsS.N[1],vKnot,CTLPNUMMAX*4,4,&SCtlp[0][0][0],NurbsS.M[0],NurbsS.M[1],GL_MAP2_VERTEX_4);
	gluEndSurface(NurbsSurf);

}

// 複合曲線の描画
void Describe_BODY::DrawCompositeCurve(COMPC *CompC)
{
	int i;

	for(i=0;i<CompC->N;i++){
		if(CompC->DEType[i] == NURBS_CURVE){
			DrawNurbsCurve_Param((NURBSC *)CompC->pDE[i]);	// NURBS曲線
		}
		//else if(CompC->DEType[i] == CIRCLE_ARC){
		//	glDraw_CircleArc_Param((CIRA *)CompC->pDE[i]);		// 円・円弧
		//}
		//else if(CompC->DEType[i] == CONIC_ARC){
		//	glDraw_ConicArc_Param((CONA *)CompC->pDE[i]);		// 円錐曲線
		//}
		//else if(CompC->DEType[i] == LINE){
		//	glDraw_Line_Param((LINE_ *)CompC->pDE[i]);			// 線分
		//}
	}

	if(CompC->DegeFlag == FALSE)
		DrawNurbsCurve_Param(&CompC->DegeNurbs);		// 縮退がある場合、縮退用Nurbs曲線をトリムエンティティとして追加
}

// 面上線の描画
void Describe_BODY::DrawCurveOnParamSurfe(CONPS *ConpS)
{
	// 2Dパラメトリック曲線
	if(ConpS->BType == COMPOSITE_CURVE){
		DrawCompositeCurve((COMPC *)ConpS->pB);	// 複合曲線
	}
//	else if(ConpS->BType == NURBS_SURFACE){
//		glDraw_NurbsCurve(ConpS->pB);		// NURBS曲線
//	}
//	else if(ConpS->BType == CIRCLE_ARC){
//		glDraw_CircleArc(ConpS->pB);		// 円・円弧
//	}
//	else if(ConpS->BType == CONIC_ARC){
//		glDraw_ConicArc();					// 円錐曲線
//	}
}

// トリム面の描画
void Describe_BODY::DrawTrimdSurf(TRMS TrmS)
{
	gluBeginSurface(NurbsSurf);

	DrawTrimdNurbsSurfe(TrmS.pts);				// NURBS曲面の描画

	// 外周トリム
	gluBeginTrim(NurbsSurf);
	DrawCurveOnParamSurfe(TrmS.pTO);			// 面上線
	gluEndTrim(NurbsSurf);

	// 内周トリム
	for(int j=0;j<TrmS.n2;j++){
		gluBeginTrim(NurbsSurf);
		DrawCurveOnParamSurfe(TrmS.pTI[j]);		// 面上線
		gluEndTrim(NurbsSurf);
	}

	gluEndSurface(NurbsSurf);
}

// 2DパラメトリックNURBS曲線要素の描画
void Describe_BODY::DrawNurbsCurve_Param(NURBSC *NurbsC)
{
	int i;

	for(i=0;i<NurbsC->K;i++){			// コントロールポイント取り出し
		CCtlp[i][0] = NurbsC->cp[i].x*NurbsC->W[i];
		CCtlp[i][1] = NurbsC->cp[i].y*NurbsC->W[i];
		CCtlp[i][2] = NurbsC->W[i];
	}
	for(i=0;i<NurbsC->N;i++){			// ノットベクトル取り出し
		uKnot[i] = NurbsC->T[i];
	}

	// トリム面を構成するNURBS曲線を指定
	gluNurbsCurve(NurbsSurf,NurbsC->N,uKnot,4,&CCtlp[0][0],NurbsC->M,GLU_MAP1_TRIM_3);

}

// 縮退チェック
void Describe_BODY::CheckDegenracy(BODY *body)
{
	int flag;
	NURBS_Func NFunc;

	// 縮退用Nurbs曲線を複合曲線の数だけ生成
	if(body->TypeNum[_COMPOSITE_CURVE]){
		double T[4] = {0,0,1,1};
		double W[2] = {1,1};
		Coord cp[2];
		InitCoord(&cp[0]);
		InitCoord(&cp[1]);

		for(int i=0;i<body->TypeNum[_COMPOSITE_CURVE];i++){
			 NFunc.GenNurbsC(&body->CompC[i].DegeNurbs,2,2,4,T,W,cp);	// 縮退用Nurbs曲線を複合曲線のエンティティ数だけ生成する

			// 各複合曲線がNURBS曲線のみで構成されておりかつ2Dパラメトリック要素であるかのチェック
			flag = 0;
			for(int j=0;j<body->CompC[i].N;j++){
				if(body->CompC[i].DEType[j] == NURBS_CURVE && body->CompC[i].pDE[j]->NurbsC.EntUseFlag == PARAMETRICELEM){
					flag++;				
				}
			}

			// NURBS曲線で構成されている複合曲線に対して、始点と終点の座標値を比較
			if(flag == body->CompC[i].N){
				Coord s,e;
				s = NFunc.CalcNurbsCCoord(&body->CompC[i].pDE[0]->NurbsC,0);					// 始点
				e = NFunc.CalcNurbsCCoord(&body->CompC[i].pDE[body->CompC[i].N-1]->NurbsC,1);	// 終点
				if(DiffCoord(s,e) == FALSE){					// 始点≠終点
					body->CompC[i].DegeNurbs.cp[0] = e;
					body->CompC[i].DegeNurbs.cp[1] = s;
					body->CompC[i].DegeFlag = FALSE;			// 縮退ありのフラグを立てる
				}
				else{
					body->CompC[i].DegeFlag = TRUE;				// 縮退なしのフラグを立てる
				}
			}
			else{
				body->CompC[i].DegeFlag = TRUE;					// 複合曲線がNurbs曲線で構成されていない場合も縮退なしのフラグ
			}
		}
	}

}

// NURBS描画ステータスの設定
void Describe_BODY::SetNurbsStat()
{
	NurbsCurve = gluNewNurbsRenderer();
	gluNurbsProperty(NurbsCurve,GLU_SAMPLING_TOLERANCE,25);	
	gluNurbsCallback(NurbsCurve, GLU_ERROR, (void (CALLBACK *) (void))NURBS_Err);	// NURBS関連のエラーのコールバック関数を登録

	NurbsSurf = gluNewNurbsRenderer();
	gluNurbsProperty(NurbsSurf,GLU_SAMPLING_TOLERANCE,25);
	gluNurbsProperty(NurbsSurf, GLU_DISPLAY_MODE, GLU_FILL);
	gluNurbsCallback(NurbsSurf, GLU_ERROR, (void (CALLBACK *) (void))NURBS_Err);	// NURBS関連のエラーのコールバック関数を登録
}

// NURBSファンクションエラーのコールバックを登録
void Describe_BODY::NURBS_Err(GLenum error_code)
{
	fprintf(stderr,"%s\n",gluErrorString(error_code));
	getchar();
	exit(1);
}

// 円・円弧の描画
void Describe_BODY::DrawCircleArc()
{
	// 未実装
}

// 円錐曲線の描画
void Describe_BODY::DrawConicArc()
{
	// 未実装
}

// 2Dパラメトリック円要素の描画
void Describe_BODY::DrawCircleArc_Param(CIRA *CirA)
{
	// 未実装
}

// 2Dパラメトリック円錐曲線要素の描画
void Describe_BODY::DrawConicArc_Param(CONA *ConA)
{
	// 未実装
}

// 2Dパラメトリック直線要素の描画
void Describe_BODY::DrawLine_Param(LINE_ *Line)
{
	// 未実装
}