/***************
* BODY�`��R�A *
****************/

#include "Describe_BODY.h"

// �R���X�g���N�^
Describe_BODY::Describe_BODY()
{
	SetNurbsStat();
}

// �f�X�g���N�^
Describe_BODY::~Describe_BODY()
{
}

// Body��`��
void Describe_BODY::DrawBody(BODY *Body)
{
	for(int i=0;i<ALL_ENTITY_TYPE_NUM;i++){
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
	}
}

// �����̕`��
void Describe_BODY::DrawLine(LINE_ Line)
{
	glLineWidth(1);

	glBegin(GL_LINE_STRIP);
	glVertex3d(Line.cp[0].x,Line.cp[0].y,Line.cp[0].z);	// �n�_
	glVertex3d(Line.cp[1].x,Line.cp[1].y,Line.cp[1].z);	// �I�_
	glEnd();

}

// NURBS�Ȑ��̕`��
void Describe_BODY::DrawNurbsCurve(NURBSC NurbsC)
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


// NURBS�Ȗʂ̕`��(�g��������)
void Describe_BODY::DrawTrimdNurbsSurfe(NURBSS *NurbsS)
{
	int j,k;
	static GLfloat	uKnot[KNOTNUMMAX];					// NURBS�`��p�o�b�t�@
	static GLfloat	vKnot[KNOTNUMMAX];					// NURBS�`��p�o�b�t�@
	static GLfloat	SCtlp[CTLPNUMMAX][CTLPNUMMAX][4];	// NURBS�`��p�o�b�t�@

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

// NURBS�Ȗʂ̕`��(�g�����Ȃ�)
void Describe_BODY::DrawNurbsSurfe(NURBSS NurbsS)
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

// �����Ȑ��̕`��
void Describe_BODY::DrawCompositeCurve(COMPC *CompC)
{
	int i;

	for(i=0;i<CompC->N;i++){
		if(CompC->DEType[i] == NURBS_CURVE){
			DrawNurbsCurve_Param((NURBSC *)CompC->pDE[i]);	// NURBS�Ȑ�
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

// �ʏ���̕`��
void Describe_BODY::DrawCurveOnParamSurfe(CONPS *ConpS)
{
	// 2D�p�����g���b�N�Ȑ�
	if(ConpS->BType == COMPOSITE_CURVE){
		DrawCompositeCurve((COMPC *)ConpS->pB);	// �����Ȑ�
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

// �g�����ʂ̕`��
void Describe_BODY::DrawTrimdSurf(TRMS TrmS)
{
	gluBeginSurface(NurbsSurf);

	DrawTrimdNurbsSurfe(TrmS.pts);				// NURBS�Ȗʂ̕`��

	// �O���g����
	gluBeginTrim(NurbsSurf);
	DrawCurveOnParamSurfe(TrmS.pTO);			// �ʏ��
	gluEndTrim(NurbsSurf);

	// �����g����
	for(int j=0;j<TrmS.n2;j++){
		gluBeginTrim(NurbsSurf);
		DrawCurveOnParamSurfe(TrmS.pTI[j]);		// �ʏ��
		gluEndTrim(NurbsSurf);
	}

	gluEndSurface(NurbsSurf);

}

// 2D�p�����g���b�NNURBS�Ȑ��v�f�̕`��
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

// BODY�Ɋ܂܂��NURBS�Ȑ���S�ĕ`��
void Describe_BODY::Draw_NurbsCurves(BODY *Body)
{
	for(int i=0;i<Body->TypeNum[_NURBSC];i++){
		glColor3f(Body->NurbsC[i].Dstat.Color[0],Body->NurbsC[i].Dstat.Color[1],Body->NurbsC[i].Dstat.Color[2]);
		if(Body->NurbsC[i].EntUseFlag == GEOMTRYELEM){				// IGES�f�B���N�g������"Entity Use Flag"��0�̏ꍇ�͎��ۂ̃��f���v�f�Ƃ��ĕ`�悷��
			DrawNurbsCurve(Body->NurbsC[i]);						// �`��
		}
	}
}

// BODY�Ɋ܂܂��NURBS�Ȗʂ�S�ĕ`��
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
		

// BODY�Ɋ܂܂��g�����ʂ�S�ĕ`��
void Describe_BODY::Draw_TrimSurfes(BODY *Body)
{
	for(int i=0;i<Body->TypeNum[_TRIMMED_SURFACE];i++){
		DrawTrimdSurf(Body->TrmS[i]);
	}
}


// NURBS�`��X�e�[�^�X�̐ݒ�
void Describe_BODY::SetNurbsStat()
{
	NurbsCurve = gluNewNurbsRenderer();
	gluNurbsProperty(NurbsCurve,GLU_SAMPLING_TOLERANCE,20);	
	gluNurbsCallback(NurbsCurve, GLU_ERROR, (void (CALLBACK *) (void))NURBS_Err);	// NURBS�֘A�̃G���[�̃R�[���o�b�N�֐���o�^

	NurbsSurf = gluNewNurbsRenderer();
	gluNurbsProperty(NurbsSurf,GLU_SAMPLING_TOLERANCE,20);
	gluNurbsCallback(NurbsSurf, GLU_ERROR, (void (CALLBACK *) (void))NURBS_Err);	// NURBS�֘A�̃G���[�̃R�[���o�b�N�֐���o�^
}

// NURBS�Ȗʂ̕`��`����ύX����
void Describe_BODY::SetNurbsSProperty(GLenum prop,GLfloat val)
{
	gluNurbsProperty(NurbsSurf,prop,val);
}


// NURBS�Ȗ�/�Ȑ��̑e�����w��
void Describe_BODY::SetNurbsSTolerance(GLfloat t)
{
	gluNurbsProperty(NurbsSurf,GLU_SAMPLING_TOLERANCE,t);
	gluNurbsProperty(NurbsCurve,GLU_SAMPLING_TOLERANCE,t);
}

// NURBS�t�@���N�V�����G���[�̃R�[���o�b�N��o�^
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
