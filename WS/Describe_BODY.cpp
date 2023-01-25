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

// �k�ރ`�F�b�N
void Describe_BODY::CheckDegenracy(BODY *body)
{
	int flag;
	NURBS_Func NFunc;

	// �k�ޗpNurbs�Ȑ��𕡍��Ȑ��̐���������
	if(body->TypeNum[_COMPOSITE_CURVE]){
		double T[4] = {0,0,1,1};
		double W[2] = {1,1};
		double V[2] = {0,1};
		int prop[4] = {0,0,1,0};
		Coord cp[2];
		InitCoord(&cp[0]);
		InitCoord(&cp[1]);

		for(int i=0;i<body->TypeNum[_COMPOSITE_CURVE];i++){
			 NFunc.GenNurbsC(&body->CompC[i].DegeNurbs,2,2,4,T,W,cp,V,prop,1);	// �k�ޗpNurbs�Ȑ��𕡍��Ȑ��̃G���e�B�e�B��������������

			// �e�����Ȑ���NURBS�Ȑ��݂̂ō\������Ă��肩��2D�p�����g���b�N�v�f�ł��邩�̃`�F�b�N
			flag = 0;
			for(int j=0;j<body->CompC[i].N;j++){
				if(body->CompC[i].DEType[j] == NURBS_CURVE && body->CompC[i].pDE[j]->NurbsC.EntUseFlag == PARAMETRICELEM){
					flag++;				
				}
			}

			// NURBS�Ȑ��ō\������Ă��镡���Ȑ��ɑ΂��āA�n�_�ƏI�_�̍��W�l���r
			if(flag == body->CompC[i].N){
				Coord s,e;
				s = NFunc.CalcNurbsCCoord(&body->CompC[i].pDE[0]->NurbsC,body->CompC[i].pDE[0]->NurbsC.V[0]);					// �n�_
				e = NFunc.CalcNurbsCCoord(&body->CompC[i].pDE[body->CompC[i].N-1]->NurbsC,body->CompC[i].pDE[body->CompC[i].N-1]->NurbsC.V[1]);	// �I�_
				if(DiffCoord(s,e) == KOD_FALSE){				// �n�_���I�_
					body->CompC[i].DegeNurbs.cp[0] = e;
					body->CompC[i].DegeNurbs.cp[1] = s;
					body->CompC[i].DegeFlag = KOD_FALSE;			// �k�ނ���̃t���O�𗧂Ă�
				}
				else{
					body->CompC[i].DegeFlag = KOD_TRUE;				// �k�ނȂ��̃t���O�𗧂Ă�
				}
			}
			else{
				body->CompC[i].DegeFlag = KOD_TRUE;					// �����Ȑ���Nurbs�Ȑ��ō\������Ă��Ȃ��ꍇ���k�ނȂ��̃t���O
			}
		}
	}

}

// NURBS�`��X�e�[�^�X�̐ݒ�
void Describe_BODY::SetNurbsStat()
{
	// Notes(2010/10/26 ktaka)
	// 1.OpenGL���񋟂���NURBS�`��֐���0.0001�ȏ�̐��x��0�Ƃ��Ă��܂��悤���B����āA����ȏ�̐��x�ŕϓ�����
	//   �m�b�g�x�N�g���Ɋւ��Ă͑S�ē����l�ƂȂ��Ă��܂��ANURBS_Err()�R�[���o�b�N���o�͂���Ǝv����B
	//   ��������������@�Ƃ��āA�m�b�g�x�N�g���͈̔͂𐳋K������Ȃǂ���0.0001���傫���ϓ����ɂ���K�v�����邪�A
	//   ���̏ꍇ�A�x�[�X�ƂȂ�NURBS�Ȗʂƃg�������C���̑��֐�������Ă��܂����߁A�ł��Ȃ��B
	//   �iNURBS�Ȗʂ̃m�b�g�x�N�g���𐳋K������ƁA����ɍ��킹�ċȖʂ̃p�����g���b�N���ʏ�̃g�������C�����ω������Ȃ���΂����Ȃ����A
	//     �g�������C�����̂�NURBS�ŕ\������Ă��邽�߁A�p�����g���b�N���ʏ�ŕω������g�������C�����W�l���g�������C���̃R���g���[���|�C���g�ɕϊ��ł��Ȃ��j
	//   �m�b�g�̏���������ǉ����邱�ƂŁA�����x�ȃm�b�g�x�N�g�����폜�ł��Ȃ���
	// 2.gluNewNurbsRenderer()��Nurbs�I�u�W�F�N�g��1�m�ۂ����ꍇ�A����1�̃I�u�W�F�N�g�ɂ���Ē񋟂���郊�\�[�X�ȏ��
	// �@NURBS�����݂���ƁA�o�O��B����āA���̊֐�����gluNewNurbsRenderer()���s�����A�e�`��֐���gluBegin***�`gluEnd***�Ԃ�
	//   �͂��ނ悤��gluNewNurbsRenderer()��gluDeleteNurbsRenderer()���L�q���ANURBS�`��̓x��Nurbs�I�u�W�F�N�g�̊m��-��������������悢�Ǝv����B

	NurbsCurve = gluNewNurbsRenderer();
	gluNurbsProperty(NurbsCurve,GLU_SAMPLING_TOLERANCE,50);	
	gluNurbsCallback(NurbsCurve, GLU_ERROR, (void (CALLBACK *) (void))NURBS_Err);	// NURBS�֘A�̃G���[�̃R�[���o�b�N�֐���o�^

	NurbsSurf = gluNewNurbsRenderer();
	gluNurbsProperty(NurbsSurf,GLU_SAMPLING_TOLERANCE,50);
	gluNurbsCallback(NurbsSurf, GLU_ERROR, (void (CALLBACK *) (void))NURBS_Err);	// NURBS�֘A�̃G���[�̃R�[���o�b�N�֐���o�^
}

// NURBS�Ȗʂ̕`��`����ύX����
void Describe_BODY::SetNurbsSProperty(GLenum prop,GLfloat val)
{
	gluNurbsProperty(NurbsSurf,prop,val);
}


// NURBS�Ȗʂ̑e�����w��
void Describe_BODY::SetNurbsSTolerance(GLfloat t)
{
	gluNurbsProperty(NurbsSurf,GLU_SAMPLING_TOLERANCE,t);
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
