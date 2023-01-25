/***************
* BODY�`��R�A *
****************/

#include "stdafx.h"
#include "Describe_BODY.h"

#if defined(_DEBUG) && defined(_MSC_VER)
#define new DEBUG_NEW
#endif

GLUnurbsObj *Describe_BODY::NurbsSurf;		// NURBS�Ȗʗp�I�u�W�F�N�g
GLUnurbsObj *Describe_BODY::NurbsCurve;		// NURBS�Ȑ��p�I�u�W�F�N�g

// Function: Describe_BODY
// �R���X�g���N�^. NURBS�`��X�e�[�^�X�̐ݒ�
Describe_BODY::Describe_BODY()
{
	SetNurbsStat();
}

// Function: ~Describe_BODY
// �f�X�g���N�^�D�X�P���g���ł��D
Describe_BODY::~Describe_BODY()
{
}

// Function: DrawBody
// Body��`��
//
// Parameters:
// *Body - �`�悷��BODY�ւ̃|�C���^
void Describe_BODY::DrawBody(BODY *Body)
{
	for(int i=0;i<ALL_ENTITY_TYPE_NUM;i++){
/*
		if(i == _CIRCLE_ARC){						// �~�E�~��
			// �~�E�~�ʂ�NRBS�Ȑ��ɕϊ������
			//Draw_CircleArc();
		}
		else if(i == _CONIC_ARC){					// �~���Ȑ�
			//Draw_ConicArc();
		}
		else if(i == _LINE){						// ����
			// ������NURBS�Ȑ��ɕϊ������
			//Draw_Line();
		}
		else if(i == _NURBSC){						// NURBS�Ȑ�
			Draw_NurbsCurves(Body);
		}
		else if(i == _NURBSS){
			Draw_NurbsSurfaces(Body);
		}
		else if(i == _TRIMMED_SURFACE){				// �g������(NURBS�Ȗ�)
			Draw_TrimSurfes(Body);
		}
*/
		switch ( i ) {
		case _NURBSC:
			Draw_NurbsCurves(Body);
			break;
		case _NURBSS:
			Draw_NurbsSurfaces(Body);
			break;
		case _TRIMMED_SURFACE:
			Draw_TrimSurfes(Body);
			break;
		}
	}
}

// Function: DrawLine
// �����̕`��
//
// Parameters:
// Line - �`�悷������\����
//void Describe_BODY::DrawLine(LINE_ Line)
void Describe_BODY::DrawLine(const LINE_& Line)
{
	glLineWidth(1);

	glBegin(GL_LINE_STRIP);
	glVertex3d(Line.cp[0].x,Line.cp[0].y,Line.cp[0].z);	// �n�_
	glVertex3d(Line.cp[1].x,Line.cp[1].y,Line.cp[1].z);	// �I�_
	glEnd();

}

// Function: DrawCircleArc
// �~�E�~�ʂ̕`��
//
// Parameters:
// Cira - �`�悷��~�E�~�ʍ\����
//void Describe_BODY::DrawCircleArc(CIRA Cira)
void Describe_BODY::DrawCircleArc(const CIRA& Cira)
{
	double delta = Cira.t[1] - Cira.t[0];
	if(Cira.t[1] < Cira.t[0])
		delta += 360;
	int d = (int)fabs(delta);

	for(int i=0;i<d;i++){
		double sth = (Cira.t[0] + delta/(double)d*(double)i)*PAI/180;
		double eth = (Cira.t[0] + delta/(double)d*(double)(i+1))*PAI/180;
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
// NURBS�Ȑ��̕`��
//
// Parameters:
// NurbsC - �`�悷��NURBS�Ȑ��\����
//void Describe_BODY::DrawNurbsCurve(NURBSC NurbsC)
void Describe_BODY::DrawNurbsCurve(const NURBSC& NurbsC)
{
	int i,j;
	static GLfloat	uKnot[KNOTNUMMAX];					// NURBS�`��p�o�b�t�@
	static GLfloat	CCtlp[CTLPNUMMAX][4];				// NURBS�`��p�o�b�t�@

	for(i=0;i<NurbsC.K;i++){			// �R���g���[���|�C���g���o��
		CCtlp[i][0] = NurbsC.cp[i].x*NurbsC.W[i];
		CCtlp[i][1] = NurbsC.cp[i].y*NurbsC.W[i];
		CCtlp[i][2] = NurbsC.cp[i].z*NurbsC.W[i];
		CCtlp[i][3] = NurbsC.W[i];
	}

	for(j=0;j<NurbsC.N;j++){			// �m�b�g�x�N�g�����o��
		uKnot[j] = NurbsC.T[j];
	}

	glDisable(GL_LIGHTING);
	gluBeginCurve(NurbsCurve);
	gluNurbsCurve(NurbsCurve,NurbsC.N,uKnot,4,&CCtlp[0][0],NurbsC.M,GL_MAP1_VERTEX_4);	// �m�b�g�x�N�g���̒l�͈̔͂�0�`1�łȂ��ƁA
	gluEndCurve(NurbsCurve);															// "�m�b�g�����X�v���C�����߂�葽������܂��Ɠ{����"
	glFlush();																			// �m�b�g�x�N�g���̐��K�����K�v(pp111)
	glEnable(GL_LIGHTING);

}

// Function: DrawTrimdNurbsSurfe
// �g�����ʂ�����NURBS�Ȗʂ�`�悷��
//
// Parameters:
// *NurbsS - �`�悷��NURBS�Ȗʂ̃|�C���^
void Describe_BODY::DrawTrimdNurbsSurfe(NURBSS *NurbsS)
{
	int j,k;
	static GLfloat	uKnot[KNOTNUMMAX];					// NURBS�`��p�o�b�t�@
	static GLfloat	vKnot[KNOTNUMMAX];					// NURBS�`��p�o�b�t�@
	static GLfloat	SCtlp[CTLPNUMMAX][CTLPNUMMAX][4];	// NURBS�`��p�o�b�t�@

	//NURBS_Func NFunc;					// for debug
	//NFunc.DebugForNurbsS(NurbsS);		// for debug

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT,NurbsS->Dstat.Color);
	for(k=0;k<NurbsS->K[1];k++){
		for(j=0;j<NurbsS->K[0];j++){
			SCtlp[j][k][0] = NurbsS->cp[j][k].x*NurbsS->W[j][k];	// �R���g���[���|�C���g���o��
			SCtlp[j][k][1] = NurbsS->cp[j][k].y*NurbsS->W[j][k];
			SCtlp[j][k][2] = NurbsS->cp[j][k].z*NurbsS->W[j][k];
			SCtlp[j][k][3] = NurbsS->W[j][k];
		}
	}
	for(j=0;j<NurbsS->N[0];j++){
		uKnot[j] = NurbsS->S[j];		// u�m�b�g�x�N�g�����o��
		//fprintf(stderr,"U:%d-%.12lf\n",j+1,uKnot[j]);
	}
	for(j=0;j<NurbsS->N[1];j++){
		vKnot[j] = NurbsS->T[j];		// v�m�b�g�x�N�g�����o��
		//fprintf(stderr,"V:%d-%.12lf\n",j+1,vKnot[j]);
	}

	// NURBS�Ȗʂ̕`��
	gluNurbsSurface(NurbsSurf,(GLdouble)NurbsS->N[0],uKnot,(GLdouble)NurbsS->N[1],vKnot,CTLPNUMMAX*4,4,&SCtlp[0][0][0],NurbsS->M[0],NurbsS->M[1],GL_MAP2_VERTEX_4);
}

// Function: DrawNurbsSurfe
// NURBS�Ȗʂ̕`��(�g�����Ȃ�)
//
// Parameters:
// NurbsS - �`�悷��NURBS�Ȗʍ\����
//void Describe_BODY::DrawNurbsSurfe(NURBSS NurbsS)
void Describe_BODY::DrawNurbsSurfe(const NURBSS& NurbsS)
{
	int j,k;
	static GLfloat	uKnot[KNOTNUMMAX];					// NURBS�`��p�o�b�t�@
	static GLfloat	vKnot[KNOTNUMMAX];					// NURBS�`��p�o�b�t�@
	static GLfloat	SCtlp[CTLPNUMMAX][CTLPNUMMAX][4];	// NURBS�`��p�o�b�t�@

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT,NurbsS.Dstat.Color);
	for(k=0;k<NurbsS.K[1];k++){
		for(j=0;j<NurbsS.K[0];j++){
			SCtlp[j][k][0] = NurbsS.cp[j][k].x*NurbsS.W[j][k];	// �R���g���[���|�C���g���o��
			SCtlp[j][k][1] = NurbsS.cp[j][k].y*NurbsS.W[j][k];
			SCtlp[j][k][2] = NurbsS.cp[j][k].z*NurbsS.W[j][k];
			SCtlp[j][k][3] = NurbsS.W[j][k];
		}
	}
	for(j=0;j<NurbsS.N[0];j++){
		uKnot[j] = NurbsS.S[j];		// u�m�b�g�x�N�g�����o��
	}
	for(j=0;j<NurbsS.N[1];j++){
		vKnot[j] = NurbsS.T[j];		// v�m�b�g�x�N�g�����o��
	}

	// NURBS�Ȗʂ̕`��
	gluBeginSurface(NurbsSurf);
	gluNurbsSurface(NurbsSurf,NurbsS.N[0],uKnot,NurbsS.N[1],vKnot,CTLPNUMMAX*4,4,&SCtlp[0][0][0],NurbsS.M[0],NurbsS.M[1],GL_MAP2_VERTEX_4);
	gluEndSurface(NurbsSurf);

}

// Function: DrawCompositeCurve
// �����Ȑ��̕`��
//
// Parameters:
// *CompC - �`�悷�镡���Ȑ��ւ̃|�C���^
void Describe_BODY::DrawCompositeCurve(COMPC *CompC)
{
	int i;

	for(i=0;i<CompC->N;i++){
		if(CompC->DEType[i] == NURBS_CURVE){
//			DrawNurbsCurve_Param((NURBSC *)CompC->pDE[i]);	// NURBS�Ȑ�
			DrawNurbsCurve_Param(CompC->pDE[i].NurbsC);	// NURBS�Ȑ�
		}
		//else if(CompC->DEType[i] == CIRCLE_ARC){
		//	glDraw_CircleArc_Param((CIRA *)CompC->pDE[i]);		// �~�E�~��
		//}
		//else if(CompC->DEType[i] == CONIC_ARC){
		//	glDraw_ConicArc_Param((CONA *)CompC->pDE[i]);		// �~���Ȑ�
		//}
		//else if(CompC->DEType[i] == LINE){
		//	glDraw_Line_Param((LINE_ *)CompC->pDE[i]);			// ����
		//}
	}

	if(CompC->DegeFlag == KOD_FALSE)
		DrawNurbsCurve_Param(&CompC->DegeNurbs);		// �k�ނ�����ꍇ�A�k�ޗpNurbs�Ȑ����g�����G���e�B�e�B�Ƃ��Ēǉ�
}

// Function: DrawCurveOnParamSurfe
// �ʏ���̕`��
//
// Parameters:
// *ConpS - �`�悷��ʏ���ւ̃|�C���^
void Describe_BODY::DrawCurveOnParamSurfe(CONPS *ConpS)
{
	// 2D�p�����g���b�N�Ȑ�
	if(ConpS->BType == COMPOSITE_CURVE){
//		DrawCompositeCurve((COMPC *)ConpS->pB);	// �����Ȑ�
		DrawCompositeCurve(ConpS->pB.CompC);	// �����Ȑ�
	}
//	else if(ConpS->BType == NURBS_SURFACE){
//		glDraw_NurbsCurve(ConpS->pB);		// NURBS�Ȑ�
//	}
//	else if(ConpS->BType == CIRCLE_ARC){
//		glDraw_CircleArc(ConpS->pB);		// �~�E�~��
//	}
//	else if(ConpS->BType == CONIC_ARC){
//		glDraw_ConicArc();					// �~���Ȑ�
//	}
}

// Function: DrawTrimdSurf
// �g�����ʂ̕`��
//
// Parameters:
// TrmS - �`�悷��g�����ʍ\����
//void Describe_BODY::DrawTrimdSurf(TRMS TrmS)
void Describe_BODY::DrawTrimdSurf(const TRMS& TrmS)
{
	gluBeginSurface(NurbsSurf);

	DrawTrimdNurbsSurfe(TrmS.pts);				// NURBS�Ȗʂ̕`��

	// �O���g����(�����v���ł��邱��)
	gluBeginTrim(NurbsSurf);
	DrawCurveOnParamSurfe(TrmS.pTO);			// �ʏ��
	gluEndTrim(NurbsSurf);

	// �����g����(���v���ł��邱��)
	for(int j=0;j<TrmS.n2;j++){
		gluBeginTrim(NurbsSurf);
		DrawCurveOnParamSurfe(TrmS.pTI[j]);		// �ʏ��
		gluEndTrim(NurbsSurf);
	}

	gluEndSurface(NurbsSurf);

}

// Function: DrawNurbsCurve_Param
// 2D�p�����g���b�NNURBS�Ȑ��v�f�̕`��
//
// Parameters:
// *NurbsC - �`�悷��2D�p�����g���b�NNURBS�Ȑ��̃|�C���^
void Describe_BODY::DrawNurbsCurve_Param(NURBSC *NurbsC)
{
	int i;
	static GLfloat	uKnot[KNOTNUMMAX];					// NURBS�`��p�o�b�t�@
	static GLfloat	CCtlp[CTLPNUMMAX][4];				// NURBS�`��p�o�b�t�@

	for(i=0;i<NurbsC->K;i++){			// �R���g���[���|�C���g���o��
		CCtlp[i][0] = NurbsC->cp[i].x*NurbsC->W[i];
		CCtlp[i][1] = NurbsC->cp[i].y*NurbsC->W[i];
		CCtlp[i][2] = NurbsC->W[i];
	}
	for(i=0;i<NurbsC->N;i++){			// �m�b�g�x�N�g�����o��
		uKnot[i] = NurbsC->T[i];
	}

	// �g�����ʂ��\������NURBS�Ȑ����w��
	gluNurbsCurve(NurbsSurf,NurbsC->N,uKnot,4,&CCtlp[0][0],NurbsC->M,GLU_MAP1_TRIM_3);

}

// Function: Draw_Lines
// BODY�Ɋ܂܂�������S�ĕ`��
//
// Parameters:
// *Body - BODY�ւ̃|�C���^
void Describe_BODY::Draw_Lines(BODY *Body)
{
	for(int i=0;i<Body->TypeNum[_LINE];i++){
		glColor3f(Body->Line[i].Dstat.Color[0],Body->Line[i].Dstat.Color[1],Body->Line[i].Dstat.Color[2]);
		if(Body->Line[i].EntUseFlag == GEOMTRYELEM){				// IGES�f�B���N�g������"Entity Use Flag"��0�̏ꍇ�͎��ۂ̃��f���v�f�Ƃ��ĕ`�悷��
			DrawLine(Body->Line[i]);
		}
	}
}

// Function: Draw_CircleArcs
// BODY�Ɋ܂܂��~�C�~�ʂ�S�ĕ`��
//
// Parameters:
// *Body - BODY�ւ̃|�C���^
void Describe_BODY::Draw_CircleArcs(BODY *Body)
{
	for(int i=0;i<Body->TypeNum[_CIRCLE_ARC];i++){
		glColor3f(Body->CirA[i].Dstat.Color[0],Body->CirA[i].Dstat.Color[1],Body->CirA[i].Dstat.Color[2]);
		if(Body->CirA[i].EntUseFlag == GEOMTRYELEM){				// IGES�f�B���N�g������"Entity Use Flag"��0�̏ꍇ�͎��ۂ̃��f���v�f�Ƃ��ĕ`�悷��
			DrawCircleArc(Body->CirA[i]);
		}
	}
}

// Function: Draw_NurbsCurves
// BODY�Ɋ܂܂��NURBS�Ȑ���S�ĕ`��
//
// Parameters:
// *Body - BODY�ւ̃|�C���^
void Describe_BODY::Draw_NurbsCurves(BODY *Body)
{
	for(int i=0;i<Body->TypeNum[_NURBSC];i++){
		glColor3f(Body->NurbsC[i].Dstat.Color[0],Body->NurbsC[i].Dstat.Color[1],Body->NurbsC[i].Dstat.Color[2]);
		if(Body->NurbsC[i].EntUseFlag == GEOMTRYELEM){				// IGES�f�B���N�g������"Entity Use Flag"��0�̏ꍇ�͎��ۂ̃��f���v�f�Ƃ��ĕ`�悷��
			DrawNurbsCurve(Body->NurbsC[i]);						// �`��
		}
	}
}

// Function: Draw_NurbsSurfaces
// BODY�Ɋ܂܂��NURBS�Ȗʂ�S�ĕ`��
//
// Parameters:
// *Body - BODY�ւ̃|�C���^
void Describe_BODY::Draw_NurbsSurfaces(BODY *Body)
{
	for(int i=0;i<Body->TypeNum[_NURBSS];i++){
		if(Body->NurbsS[i].TrmdSurfFlag == KOD_TRUE)	// �g�����ʂƂ���NURBS�Ȗʂ��o�^����Ă���Ȃ�
			continue;		// �`�悵�Ȃ�
		else{
			DrawNurbsSurfe(Body->NurbsS[i]);	// NURBS�Ȗʕ`��
		}
	}
}
		
// Function: Draw_TrimSurfes
// BODY�Ɋ܂܂��g�����ʂ�S�ĕ`��
//
// Parameters:
// *Body - BODY�ւ̃|�C���^
void Describe_BODY::Draw_TrimSurfes(BODY *Body)
{
	for(int i=0;i<Body->TypeNum[_TRIMMED_SURFACE];i++){
		DrawTrimdSurf(Body->TrmS[i]);
	}
}

// Function: DrawMesh
// ���b�V���̕`��
//
// Parameters:
// *mesh - Mesh�N���X�̃I�u�W�F�N�g�ւ̃|�C���^
// flag - KOD_TRUE�F�X�P���g���\���D KOD_FALSE�F�ʕ\��
void Describe_BODY::DrawMesh(MESH *mesh,int flag)
{
	//mesh->Face.setSentinel(0);
	for(int i=0;i<mesh->FaceNum;i++){
		HEface *f = (HEface *)mesh->Face.getData(i);	// i�Ԗڂ�Face���X�g�̎��̂𓾂�
		//HEface *f = (HEface *)mesh->Face.getSentinelData();
		glPushName(f->index);		// �t�@�Z�b�g1��1���ɃZ���N�V�����ԍ������蓖�Ă�
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
// NURBS�`��X�e�[�^�X�̐ݒ�
void Describe_BODY::SetNurbsStat()
{
	NurbsCurve = gluNewNurbsRenderer();
	gluNurbsProperty(NurbsCurve,GLU_SAMPLING_TOLERANCE,20);	
#ifdef _GLUfuncptr
    gluNurbsCallback(NurbsCurve, GLU_ERROR, (_GLUfuncptr)NURBS_Err);	// NURBS�֘A�̃G���[�̃R�[���o�b�N�֐���o�^
#else
    gluNurbsCallback(NurbsCurve, GLU_ERROR, (void (CALLBACK *) (void))NURBS_Err);	// NURBS�֘A�̃G���[�̃R�[���o�b�N�֐���o�^
#endif


    NurbsSurf = gluNewNurbsRenderer();
    gluNurbsProperty(NurbsSurf,GLU_SAMPLING_TOLERANCE,20);
#ifdef _GLUfuncptr
    gluNurbsCallback(NurbsSurf, GLU_ERROR, (_GLUfuncptr)NURBS_Err);	// NURBS�֘A�̃G���[�̃R�[���o�b�N�֐���o�^
#else
    gluNurbsCallback(NurbsSurf, GLU_ERROR, (void (CALLBACK *) (void))NURBS_Err);	// NURBS�֘A�̃G���[�̃R�[���o�b�N�֐���o�^
#endif
}

// Function: SetNurbsSProperty
// NURBS�Ȗʂ̕`��`����ύX����
void Describe_BODY::SetNurbsSProperty(GLenum prop,GLfloat val)
{
	gluNurbsProperty(NurbsSurf,prop,val);
}

// Function: SetNurbsSTolerance
// NURBS�Ȗ�/�Ȑ��̑e�����w��
//
// Parameters:
// t - �g�������X�l�DgluNurbsProperty()�֐���Property��GLU_SAMPLING_TOLERANCE���w�肵���ꍇ��value�l������. �l���������قǊ��炩�ȕ`��ƂȂ�.�f�t�H���g�ł�20���w�肳��Ă���.
void Describe_BODY::SetNurbsSTolerance(GLfloat t)
{
	gluNurbsProperty(NurbsSurf,GLU_SAMPLING_TOLERANCE,t);
	gluNurbsProperty(NurbsCurve,GLU_SAMPLING_TOLERANCE,t);
}

// Function: NURBS_Err
// NURBS�t�@���N�V�����G���[�̃R�[���o�b�N��o�^
// 
// Parameters:
// error_code - OpenGL���񋟂���NURBS�`��֐����Ŕ��������G���[�R�[�h
void Describe_BODY::NURBS_Err(GLenum error_code)
{
	fprintf(stderr,"%s\n",gluErrorString(error_code));
	getchar();
	//exit(1);
}

// �~�E�~�ʂ̕`��
void Describe_BODY::DrawCircleArc()
{
	// ������
}

// �~���Ȑ��̕`��
void Describe_BODY::DrawConicArc()
{
	// ������
}

// 2D�p�����g���b�N�~�v�f�̕`��
void Describe_BODY::DrawCircleArc_Param(CIRA *CirA)
{
	// ������
}

// 2D�p�����g���b�N�~���Ȑ��v�f�̕`��
void Describe_BODY::DrawConicArc_Param(CONA *ConA)
{
	// ������
}

// 2D�p�����g���b�N�����v�f�̕`��
void Describe_BODY::DrawLine_Param(LINE_ *Line)
{
	// ������
}
