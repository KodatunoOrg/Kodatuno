/***************
* BODY描画コア *
****************/

#include "stdafx.h"			// Add by K.Magara
#include "Describe_BODY.h"

GLUnurbsObj *Describe_BODY::NurbsSurf;		// NURBS曲面用オブジェクト
GLUnurbsObj *Describe_BODY::NurbsCurve;		// NURBS曲線用オブジェクト

// Function: Describe_BODY
// コンストラクタ. NURBS描画ステータスの設定
Describe_BODY::Describe_BODY()
{
	SetNurbsStat();
}

// Function: ~Describe_BODY
// デストラクタ．スケルトンです．
Describe_BODY::~Describe_BODY()
{
}

// Function: DrawBody
// Bodyを描画
//
// Parameters:
// *Body - 描画するBODYへのポインタ
void Describe_BODY::DrawBody(BODY *Body)
{
	for(int i=0;i<ALL_ENTITY_TYPE_NUM;i++){
		if(i == _CIRCLE_ARC){						// 円・円弧
			// 円・円弧はNRBS曲線に変換される
			//Draw_CircleArc();
		}
		else if(i == _CONIC_ARC){					// 円錐曲線
			//Draw_ConicArc();
		}
		else if(i == _LINE){						// 線分
			// 線分はNURBS曲線に変換される
			//Draw_Line();
		}
		else if(i == _NURBSC){						// NURBS曲線
			Draw_NurbsCurves(Body);
		}
		else if(i == _NURBSS){
			Draw_NurbsSurfaces(Body);
		}
		else if(i == _TRIMMED_SURFACE){				// トリム面(NURBS曲面)
			Draw_TrimSurfes(Body);
		}
	}
}

// Function: DrawLine
// 線分の描画
//
// Parameters:
// Line - 描画する線分構造体
//void Describe_BODY::DrawLine(LINE_ Line)
void Describe_BODY::DrawLine(const LINE_& Line)
{
	glLineWidth(1);

	glBegin(GL_LINE_STRIP);
	glVertex3d(Line.cp[0].x,Line.cp[0].y,Line.cp[0].z);	// 始点
	glVertex3d(Line.cp[1].x,Line.cp[1].y,Line.cp[1].z);	// 終点
	glEnd();

}

// Function: DrawCircleArc
// 円・円弧の描画
//
// Parameters:
// Cira - 描画する円・円弧構造体
//void Describe_BODY::DrawCircleArc(CIRA Cira)
void Describe_BODY::DrawCircleArc(const CIRA& Cira)
{
	double delta = Cira.t[1] - Cira.t[0];
	if(Cira.t[1] < Cira.t[0])
		delta += 360;
	int d = (int)fabs(delta);

	for(int i=0;i<d;i++){
		double sth = (Cira.t[0] + delta/(double)d*(double)i)*PI/180;
		double eth = (Cira.t[0] + delta/(double)d*(double)(i+1))*PI/180;
		double sx = Cira.R*cos(sth) + Cira.cp[0].x;
		double sy = Cira.R*sin(sth) + Cira.cp[0].y;
		double ex = Cira.R*cos(eth) + Cira.cp[0].x;
		double ey = Cira.R*sin(eth) + Cira.cp[0].y;
		glBegin(GL_LINES);
			glVertex3d(sx,sy,0);
			glVertex3d(ex,ey,0);
		glEnd();
	}

}

// Function: DrawNurbsCurve
// NURBS曲線の描画
//
// Parameters:
// NurbsC - 描画するNURBS曲線構造体
//void Describe_BODY::DrawNurbsCurve(NURBSC NurbsC)
void Describe_BODY::DrawNurbsCurve(const NURBSC& NurbsC)
{
	int i,j;
	static GLfloat	uKnot[KNOTNUMMAX];					// NURBS描画用バッファ
	static GLfloat	CCtlp[CTLPNUMMAX][4];				// NURBS描画用バッファ

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
	gluNurbsCurve(NurbsCurve,NurbsC.N,uKnot,4,&CCtlp[0][0],NurbsC.M,GL_MAP1_VERTEX_4);	// ノットベクトルの値の範囲が0～1でないと、
	gluEndCurve(NurbsCurve);															// "ノット数がスプライン命令より多くありますと怒られる"
	glFlush();																			// ノットベクトルの正規化が必要(pp111)
	glEnable(GL_LIGHTING);

}

// Function: DrawTrimdNurbsSurfe
// トリム面を持つNURBS曲面を描画する
//
// Parameters:
// *NurbsS - 描画するNURBS曲面のポインタ
void Describe_BODY::DrawTrimdNurbsSurfe(NURBSS *NurbsS)
{
	int j,k;
	static GLfloat	uKnot[KNOTNUMMAX];					// NURBS描画用バッファ
	static GLfloat	vKnot[KNOTNUMMAX];					// NURBS描画用バッファ
	static GLfloat	SCtlp[CTLPNUMMAX][CTLPNUMMAX][4];	// NURBS描画用バッファ

	//NURBS_Func NFunc;					// for debug
	//NFunc.DebugForNurbsS(NurbsS);		// for debug

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
		//fprintf(stderr,"U:%d-%.12lf\n",j+1,uKnot[j]);
	}
	for(j=0;j<NurbsS->N[1];j++){
		vKnot[j] = NurbsS->T[j];		// vノットベクトル取り出し
		//fprintf(stderr,"V:%d-%.12lf\n",j+1,vKnot[j]);
	}

	// NURBS曲面の描画
	gluNurbsSurface(NurbsSurf,(GLdouble)NurbsS->N[0],uKnot,(GLdouble)NurbsS->N[1],vKnot,CTLPNUMMAX*4,4,&SCtlp[0][0][0],NurbsS->M[0],NurbsS->M[1],GL_MAP2_VERTEX_4);
}

// Function: DrawNurbsSurfe
// NURBS曲面の描画(トリムなし)
//
// Parameters:
// NurbsS - 描画するNURBS曲面構造体
//void Describe_BODY::DrawNurbsSurfe(NURBSS NurbsS)
void Describe_BODY::DrawNurbsSurfe(const NURBSS& NurbsS)
{
	int j,k;
	static GLfloat	uKnot[KNOTNUMMAX];					// NURBS描画用バッファ
	static GLfloat	vKnot[KNOTNUMMAX];					// NURBS描画用バッファ
	static GLfloat	SCtlp[CTLPNUMMAX][CTLPNUMMAX][4];	// NURBS描画用バッファ

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

// Function: DrawCompositeCurve
// 複合曲線の描画
//
// Parameters:
// *CompC - 描画する複合曲線へのポインタ
void Describe_BODY::DrawCompositeCurve(COMPC *CompC)
{
	int i;

	for(i=0;i<CompC->N;i++){
		if(CompC->DEType[i] == NURBS_CURVE){
//			DrawNurbsCurve_Param((NURBSC *)CompC->pDE[i]);	// NURBS曲線
			DrawNurbsCurve_Param(CompC->pDE[i].NurbsC);	// NURBS曲線
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

	if(CompC->DegeFlag == KOD_FALSE)
		DrawNurbsCurve_Param(&CompC->DegeNurbs);		// 縮退がある場合、縮退用Nurbs曲線をトリムエンティティとして追加
}

// Function: DrawCurveOnParamSurfe
// 面上線の描画
//
// Parameters:
// *ConpS - 描画する面上線へのポインタ
void Describe_BODY::DrawCurveOnParamSurfe(CONPS *ConpS)
{
	// 2Dパラメトリック曲線
	if(ConpS->BType == COMPOSITE_CURVE){
//		DrawCompositeCurve((COMPC *)ConpS->pB);	// 複合曲線
		DrawCompositeCurve(ConpS->pB.CompC);	// 複合曲線
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

// Function: DrawTrimdSurf
// トリム面の描画
//
// Parameters:
// TrmS - 描画するトリム面構造体
//void Describe_BODY::DrawTrimdSurf(TRMS TrmS)
void Describe_BODY::DrawTrimdSurf(const TRMS& TrmS)
{
	gluBeginSurface(NurbsSurf);

	DrawTrimdNurbsSurfe(TrmS.pts);				// NURBS曲面の描画

	// 外周トリム(反時計回りであること)
	gluBeginTrim(NurbsSurf);
	DrawCurveOnParamSurfe(TrmS.pTO);			// 面上線
	gluEndTrim(NurbsSurf);

	// 内周トリム(時計回りであること)
	for(int j=0;j<TrmS.n2;j++){
		gluBeginTrim(NurbsSurf);
		DrawCurveOnParamSurfe(TrmS.pTI[j]);		// 面上線
		gluEndTrim(NurbsSurf);
	}

	gluEndSurface(NurbsSurf);

}

// Function: DrawNurbsCurve_Param
// 2DパラメトリックNURBS曲線要素の描画
//
// Parameters:
// *NurbsC - 描画する2DパラメトリックNURBS曲線のポインタ
void Describe_BODY::DrawNurbsCurve_Param(NURBSC *NurbsC)
{
	int i;
	static GLfloat	uKnot[KNOTNUMMAX];					// NURBS描画用バッファ
	static GLfloat	CCtlp[CTLPNUMMAX][4];				// NURBS描画用バッファ

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

// Function: Draw_Lines
// BODYに含まれる線分を全て描画
//
// Parameters:
// *Body - BODYへのポインタ
void Describe_BODY::Draw_Lines(BODY *Body)
{
	for(int i=0;i<Body->TypeNum[_LINE];i++){
		glColor3f(Body->Line[i].Dstat.Color[0],Body->Line[i].Dstat.Color[1],Body->Line[i].Dstat.Color[2]);
        // IGESディレクトリ部の"Entity Use Flag"が0かつ，"Blank Status"が0の場合は実際のモデル要素として描画する
        if(Body->Line[i].EntUseFlag == GEOMTRYELEM && Body->Line[i].BlankStat == DISPLAY){
			DrawLine(Body->Line[i]);
		}
	}
}

// Function: Draw_CircleArcs
// BODYに含まれる円，円弧を全て描画
//
// Parameters:
// *Body - BODYへのポインタ
void Describe_BODY::Draw_CircleArcs(BODY *Body)
{
	for(int i=0;i<Body->TypeNum[_CIRCLE_ARC];i++){
		glColor3f(Body->CirA[i].Dstat.Color[0],Body->CirA[i].Dstat.Color[1],Body->CirA[i].Dstat.Color[2]);
        // IGESディレクトリ部の"Entity Use Flag"が0かつ，"Blank Status"が0の場合は実際のモデル要素として描画する
        if(Body->CirA[i].EntUseFlag == GEOMTRYELEM && Body->CirA[i].BlankStat == DISPLAY){
			DrawCircleArc(Body->CirA[i]);
		}
	}
}

// Function: Draw_NurbsCurves
// BODYに含まれるNURBS曲線を全て描画
//
// Parameters:
// *Body - BODYへのポインタ
void Describe_BODY::Draw_NurbsCurves(BODY *Body)
{
	for(int i=0;i<Body->TypeNum[_NURBSC];i++){
		glColor3f(Body->NurbsC[i].Dstat.Color[0],Body->NurbsC[i].Dstat.Color[1],Body->NurbsC[i].Dstat.Color[2]);
        // IGESディレクトリ部の"Entity Use Flag"が0かつ，"Blank Status"が0の場合は実際のモデル要素として描画する
        if(Body->NurbsC[i].EntUseFlag == GEOMTRYELEM && Body->NurbsC[i].BlankStat == DISPLAY){
			DrawNurbsCurve(Body->NurbsC[i]);						// 描画
		}
	}
}

// Function: Draw_NurbsSurfaces
// BODYに含まれるNURBS曲面を全て描画
//
// Parameters:
// *Body - BODYへのポインタ
void Describe_BODY::Draw_NurbsSurfaces(BODY *Body)
{
	for(int i=0;i<Body->TypeNum[_NURBSS];i++){
		if(Body->NurbsS[i].TrmdSurfFlag == KOD_TRUE)	// トリム面としてNURBS曲面が登録されているなら
			continue;		// 描画しない
		else{
			DrawNurbsSurfe(Body->NurbsS[i]);	// NURBS曲面描画
		}
	}
}
		
// Function: Draw_TrimSurfes
// BODYに含まれるトリム面を全て描画
//
// Parameters:
// *Body - BODYへのポインタ
void Describe_BODY::Draw_TrimSurfes(BODY *Body)
{
	for(int i=0;i<Body->TypeNum[_TRIMMED_SURFACE];i++){
		DrawTrimdSurf(Body->TrmS[i]);
	}
}

// Function: DrawMesh
// メッシュの描画
//
// Parameters:
// *mesh - Meshクラスのオブジェクトへのポインタ
// flag - KOD_TRUE：スケルトン表示． KOD_FALSE：面表示
void Describe_BODY::DrawMesh(MESH *mesh,int flag)
{
	//mesh->Face.setSentinel(0);
	for(int i=0;i<mesh->FaceNum;i++){
		HEface *f = (HEface *)mesh->Face.getData(i);	// i番目のFaceリストの実体を得る
		//HEface *f = (HEface *)mesh->Face.getSentinelData();
		glPushName(f->index);		// ファセット1枚1枚にセレクション番号を割り当てる
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT,f->Dstat.Color);
		HEedge *e = f->edge;
		if(flag == KOD_TRUE)	glBegin(GL_LINE_LOOP);
		else	glBegin(GL_TRIANGLES);
		glNormal3d(f->norm.x,f->norm.y,f->norm.z);
		for(int j=0;j<f->vertnum;j++){
			glVertex3d(e->vert->cod.x,e->vert->cod.y,e->vert->cod.z);
			e = e->ne;
		}
		glEnd();
		glPopName();
		//mesh->Face.shiftSentinel(1);
	}
	glFlush();

}

// Function: SetNurbsStat
// NURBS描画ステータスの設定
void Describe_BODY::SetNurbsStat()
{
	NurbsCurve = gluNewNurbsRenderer();
	gluNurbsProperty(NurbsCurve,GLU_SAMPLING_TOLERANCE,20);	
#ifdef _GLUfuncptr
    gluNurbsCallback(NurbsCurve, GLU_ERROR, (_GLUfuncptr)NURBS_Err);	// NURBS関連のエラーのコールバック関数を登録
#else
    gluNurbsCallback(NurbsCurve, GLU_ERROR, (void (CALLBACK *) (void))NURBS_Err);	// NURBS関連のエラーのコールバック関数を登録
#endif


    NurbsSurf = gluNewNurbsRenderer();
    gluNurbsProperty(NurbsSurf,GLU_SAMPLING_TOLERANCE,20);
#ifdef _GLUfuncptr
    gluNurbsCallback(NurbsSurf, GLU_ERROR, (_GLUfuncptr)NURBS_Err);	// NURBS関連のエラーのコールバック関数を登録
#else
    gluNurbsCallback(NurbsSurf, GLU_ERROR, (void (CALLBACK *) (void))NURBS_Err);	// NURBS関連のエラーのコールバック関数を登録
#endif
}

// Function: SetNurbsSProperty
// NURBS曲面の描画形式を変更する
void Describe_BODY::SetNurbsSProperty(GLenum prop,GLfloat val)
{
	gluNurbsProperty(NurbsSurf,prop,val);
}

// Function: SetNurbsSTolerance
// NURBS曲面/曲線の粗さを指定
//
// Parameters:
// t - トレランス値．gluNurbsProperty()関数のPropertyにGLU_SAMPLING_TOLERANCEを指定した場合のvalue値を示す. 値が小さいほど滑らかな描画となる.デフォルトでは20が指定されている.
void Describe_BODY::SetNurbsSTolerance(GLfloat t)
{
	gluNurbsProperty(NurbsSurf,GLU_SAMPLING_TOLERANCE,t);
	gluNurbsProperty(NurbsCurve,GLU_SAMPLING_TOLERANCE,t);
}

// Function: NURBS_Err
// NURBSファンクションエラーのコールバックを登録
// 
// Parameters:
// error_code - OpenGLが提供するNURBS描画関数内で発生したエラーコード
void Describe_BODY::NURBS_Err(GLenum error_code)
{
	fprintf(stderr,"%s\n",gluErrorString(error_code));
	getchar();
	//exit(1);
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
