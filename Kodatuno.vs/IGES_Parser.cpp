/*************************
* IGES�t�@�C����ǂݍ��� *
**************************/

#include "stdafx.h"
#include "IGES_Parser.h"

#if defined(_DEBUG) && defined(_MSC_VER)
#define new DEBUG_NEW
#endif

// Function: IGES_Parser_Main
// IGES�p�[�T�[�̃��C��
//
// Parameters:
// *body - ���̂��\������G���e�B�e�B�̏W���I�u�W�F�N�g�ւ̃|�C���^
// TypeNum[] - �e�G���e�B�e�B�̐����i�[�����
//
// Return:
// KOD_TRUE:�ǂݍ��ݐ���	KOD_ERR:���s
int IGES_PARSER::IGES_Parser_Main(BODY *body,const char *IGES_fname)
{
	FILE *fp;
	GlobalParam gpara;				// �O���[�o�����̃p�����[�^���i�[
	DirectoryParam *dpara;			// �f�B���N�g�����̃p�����[�^���i�[
	char mes[256];					// ���b�Z�[�W�_���v�pstring
	int  line[SECTION_NUM];			// �e�Z�N�V�������̃��C�������i�[
	int  flag = 0;
	int  i;

	// IGES�t�@�C���I�[�v��
	if((fp = fopen(IGES_fname,"r")) == NULL){
		sprintf(mes,"KOD_ERROR: Cannot open %s",IGES_fname);
		GuiIF.SetMessage(mes);
		return(KOD_ERR);
	}
//	sprintf(mes,"Open %s",IGES_fname);
//	GuiIF.SetMessage(mes);

	// �e�Z�N�V�����̍s�������炩���ߎ擾
	GetSectionLine(fp,line);

	// DirectoryParam�̃������[�m��
	line[SECTION_DIRECTORY] /= 2;		// �f�B���N�g�����́A2�s��1�̃V�[�P���X���\������̂�2�Ŋ��������̂��f�B���N�g�����̃��C�����Ƃ���
//	dpara = (DirectoryParam *)malloc(sizeof(DirectoryParam)*line[SECTION_DIRECTORY]);
	dpara = new DirectoryParam[line[SECTION_DIRECTORY]];
	if(dpara == NULL){
		GuiIF.SetMessage("KOD_ERROR: fail to allocate DirectoryParam");
		return(KOD_ERR);
	}

	// IGES�t�@�C���ǂݍ���(�e�Z�N�V�������ɏ���)
	for(i=0;i<SECTION_NUM;i++){
		if(i == SECTION_START){					// �X�^�[�g���ǂݍ���
			flag = GetStartSection(fp,line[i]);
		}
		else if(i == SECTION_GLOBAL){			// �O���[�o�����ǂݍ���
			flag = GetGlobalSection(fp,&gpara,line[i]);
		}
		else if(i == SECTION_DIRECTORY){		// �f�B���N�g�����ǂݍ���
			flag = GetDirectorySection(fp,dpara,body->TypeNum,line[i]);
		}
		else if(i == SECTION_PARAMETER){		// �p�����[�^���ǂݍ���
			body->NewBodyElem();				// BODY�\���̓��̊e�G���e�B�e�B�̃������[�m��
			flag = GetParameterSection(fp,dpara,*body,line[SECTION_DIRECTORY]);
		}
		else if(i == SECTION_TERMINATE){		// �^�[�~�l�[�g���ǂݍ���
			flag = GetTerminateSection(fp);
		}
		if(flag == KOD_ERR){
			body->DelBodyElem(TypeCount);		// ���܂Ŋm�ۂ������̃������[�����
			return(KOD_ERR);
		}
	}

	ChangeEntityforNurbs(dpara,*body,line[SECTION_DIRECTORY]);	// �����\����S��NURBS�ɕύX����

	flag = SearchMaxCoord(body,body->TypeNum);		// ���̂̍ő���W�l��T��(�����\���ł̕\���{�������肷�邽��)

	fclose(fp);

//	free(dpara);
	delete[]	dpara;

	return flag;
}

// Function: Optimize4OpenGL
// �ǂݍ���IGES�t�@�C����OpenGL�p�ɍœK������
//
// Parameters:
// *body - BODY�ւ̃|�C���^
//
// Return:
// KOD_TURE
int IGES_PARSER::Optimize4OpenGL(BODY *body)
{
	ExpandKnotRange(body);		// �m�b�g�x�N�g���͈̔͂�OpenGL�̎d�l�ɉ����čœK��
	CheckCWforTrim(body);		// �g�����Ȑ������v���A�����v����OpenGL�̎d�l�ɉ����ĕύX
	CheckDegenracy(body);		// �k��(2D�p�����g���b�N�Ȑ��̎n�_�ƏI�_����v���Ă��邩)�̃`�F�b�N
	ModifyParamConect(body);	// �p�����g���b�N���ʓ��̃g�����Ȑ����m�̂Ȃ�����`�F�b�N�A�C������

	return KOD_TRUE;
}

// Function: CheckDegenracy
// �k�ރ`�F�b�N
//
// Parameters:
// *body - BODY�ւ̃|�C���^
//
// Return:
// KOD_TRUE
int IGES_PARSER::CheckDegenracy(BODY *body)
{
	int flag;
	NURBS_Func NFunc;

	// �k�ޗpNurbs�Ȑ��𕡍��Ȑ��̐���������
	if(body->TypeNum[_COMPOSITE_CURVE]){
		double T[4] = {0,0,NORM_KNOT_VAL,NORM_KNOT_VAL};
		double W[2] = {1,1};
		double V[2] = {0,NORM_KNOT_VAL};
		int prop[4] = {0,0,1,0};
		Coord cp[2];
		InitCoord(&cp[0]);
		InitCoord(&cp[1]);

		for(int i=0;i<body->TypeNum[_COMPOSITE_CURVE];i++){
			 NFunc.GenNurbsC(&body->CompC[i].DegeNurbs,2,2,4,T,W,cp,V,prop,1);	// �k�ޗpNurbs�Ȑ��𕡍��Ȑ��̃G���e�B�e�B��������������

			// �e�����Ȑ���NURBS�Ȑ��݂̂ō\������Ă��肩��2D�p�����g���b�N�v�f�ł��邩�̃`�F�b�N
			flag = 0;
			for(int j=0;j<body->CompC[i].N;j++){
//				if(body->CompC[i].DEType[j] == NURBS_CURVE && body->CompC[i].pDE[j]->NurbsC.EntUseFlag == PARAMETRICELEM){
				if(body->CompC[i].DEType[j] == NURBS_CURVE && body->CompC[i].pDE[j].NurbsC->EntUseFlag == PARAMETRICELEM){
					flag++;				
				}
			}

			// NURBS�Ȑ��ō\������Ă��镡���Ȑ��ɑ΂��āA�n�_�ƏI�_�̍��W�l���r
			if(flag == body->CompC[i].N){
				Coord s,e;
//				s = NFunc.CalcNurbsCCoord(&body->CompC[i].pDE[0]->NurbsC,body->CompC[i].pDE[0]->NurbsC.V[0]);					// �n�_
//				e = NFunc.CalcNurbsCCoord(&body->CompC[i].pDE[body->CompC[i].N-1]->NurbsC,body->CompC[i].pDE[body->CompC[i].N-1]->NurbsC.V[1]);	// �I�_
				s = body->CompC[i].pDE[0].NurbsC->CalcNurbsCCoord(body->CompC[i].pDE[0].NurbsC->V[0]);					// �n�_
				e = body->CompC[i].pDE[body->CompC[i].N-1].NurbsC->CalcNurbsCCoord(body->CompC[i].pDE[body->CompC[i].N-1].NurbsC->V[1]);	// �I�_
				if(DiffCoord(s,e,1.0E-5) == KOD_FALSE){				// �n�_���I�_
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

	return KOD_TRUE;
}

// Function: ModifyParamConect
// �p�����g���b�N���ʓ��̃g�����Ȑ����m�̂Ȃ�����`�F�b�N�A�C������
//
// Parameters:
// *body - BODY�ւ̃|�C���^
//
// Return:
// KOD_TRUE
int IGES_PARSER::ModifyParamConect(BODY *body)
{
	NURBSC *bc,*nc;

	// �g�����Ȗ�
	for(int i=0;i<body->TypeNum[_TRIMMED_SURFACE];i++){
		// �O���g����
//		for(int j=1;j<body->TrmS[i].pTO->pB->CompC.N;j++){
//			bc = (NURBSC *)body->TrmS[i].pTO->pB->CompC.pDE[j-1];
//			nc = (NURBSC *)body->TrmS[i].pTO->pB->CompC.pDE[j];
		for(int j=1;j<body->TrmS[i].pTO->pB.CompC->N;j++){
			bc = body->TrmS[i].pTO->pB.CompC->pDE[j-1].NurbsC;
			nc = body->TrmS[i].pTO->pB.CompC->pDE[j].NurbsC;
			if(DiffCoord2D(bc->cp[bc->K-1],nc->cp[0]) == KOD_FALSE)
				nc->cp[0] = SetCoord(bc->cp[bc->K-1]);
		}
		// �����g����
		for(int j=0;j<body->TrmS[i].n2;j++){
//			for(int k=1;k<body->TrmS[i].pTI[j]->pB->CompC.N;k++){
//				bc = (NURBSC *)body->TrmS[i].pTI[j]->pB->CompC.pDE[k-1];
//				nc = (NURBSC *)body->TrmS[i].pTI[j]->pB->CompC.pDE[k];
			for(int k=1;k<body->TrmS[i].pTI[j]->pB.CompC->N;k++){
				bc = body->TrmS[i].pTI[j]->pB.CompC->pDE[k-1].NurbsC;
				nc = body->TrmS[i].pTI[j]->pB.CompC->pDE[k].NurbsC;
				if(DiffCoord2D(bc->cp[bc->K-1],nc->cp[0]) == KOD_FALSE)
					nc->cp[0] = SetCoord(bc->cp[bc->K-1]);
			}
		}
	}

	return KOD_TRUE;
}

// Function: ChangeKnotVecRange
// �m�b�g�x�N�g���͈̔͂�0�`val�ɂ���D
//
// Parameters:
// Range[] - �m�b�g�x�N�g���͈̔�
// Knot[] - �m�b�g�x�N�g��
// N - �m�b�g�x�N�g���̐�
// M - �K��
// K - �R���g���[���|�C���g��
// val - �m�b�g�x�N�g���͈̔͂̏���l
//
// Return:
// KOD_TRUE
int IGES_PARSER::ChangeKnotVecRange(double Range[],double Knot[],int N,int M,int K,double val)
{
	double _t[KNOTNUMMAX];
	for(int i=0;i<N;i++){
		_t[i] = ChangeKnot(Knot[i],Knot[M-1],Knot[K],val);
	}
	for(int i=0;i<N;i++){
		Knot[i] = _t[i];
	}
	Range[0] = 0;
	Range[1] = val;

	return KOD_TRUE;
}

// Function: ChangeKnot
// �m�b�g�x�N�g���͈͕̔ύX�֐�ChangeKnotVecRange()�̃T�u�֐�
//
// Parameters:
// Knot - ���ڒ��̃m�b�g
// M_ - Knot[�K��-1]
// K_ - Knot[K]
// val - �m�b�g�x�N�g���͈̔͂̏���l
//
// Return:
// �͈͕ύX��̃m�b�g�l
double IGES_PARSER::ChangeKnot(double Knot,double M_,double K_,double val)
{
	return val*(Knot - M_)/(K_-M_);
}

// Fucntion: NormalizeKnotRange
// >�w�肵��BODY�ɑ�����S�Ă�NURBS�Ȑ�/�Ȗʂ̃m�b�g�x�N�g����0-val�͈̔͂ɕύX������
// >0.0001�ȉ����炢�̔����ω���OpenGL���F�����Ȃ����߁A�m�b�g�Ԋu���L�����A0.0001�ȏ�̊Ԋu
// >�ɂȂ�悤�ɂ���B�����������_�ł͑S�Ẵm�b�g�Ԋu���ꗥ0�`NORM_KNOT_VAL���Ă���A���ʂł���B
// >����m�b�g�Ԋu��0.0001�ȉ��̏ꍇ�̂݃m�b�g�Ԋu���L����悤�ɂ���ׂ��B(2011/10)
// >�ׂ荇���m�b�g�x�N�g���̍���MIN_KNOT_RANGE�ȏ�ɂȂ�悤�͈͂�ύX����
//
// Parameters:
// *body - BODY�ւ̃|�C���^
// val - �m�b�g�x�N�g���͈̔͂̏���l
//
// Return:
// KOD_TRUE
int IGES_PARSER::NormalizeKnotRange(BODY *body,double val)
{
	// �g������
	for(int i=0;i<body->TypeNum[_TRIMMED_SURFACE];i++){
		int M0 = body->TrmS[i].pts->M[0];
		int M1 = body->TrmS[i].pts->M[1];
		int K0 = body->TrmS[i].pts->K[0];
		int K1 = body->TrmS[i].pts->K[1];
		// �g�����ʂ̃p�����g���b�N���ʂɂ�����O���g�����Ȑ��̕ύX
//		for(int j=0;j<body->TrmS[i].pTO->pB->CompC.N;j++){
//			NURBSC *nc = (NURBSC *)body->TrmS[i].pTO->pB->CompC.pDE[j];
		for(int j=0;j<body->TrmS[i].pTO->pB.CompC->N;j++){
			NURBSC *nc = body->TrmS[i].pTO->pB.CompC->pDE[j].NurbsC;
			for(int k=0;k<nc->K;k++){	// �p�����g���b�N���ʏ��NURBS�Ȑ��̃R���g���[���|�C���g���m�b�g�̕ύX�ɍ��킹�ĕύX
				nc->cp[k].x = ChangeKnot(nc->cp[k].x,body->TrmS[i].pts->S[M0-1],body->TrmS[i].pts->S[K0],val);
				nc->cp[k].y = ChangeKnot(nc->cp[k].y,body->TrmS[i].pts->T[M1-1],body->TrmS[i].pts->T[K1],val);
			}
			ChangeKnotVecRange(nc->V,nc->T,nc->N,nc->M,nc->K,val);
//			ChangeKnotVecRange(nc->V,nc->T.get(),nc->N,nc->M,nc->K,val);
		}
		// �g�����ʂ̃p�����g���b�N���ʂɂ���������g�����Ȑ��̕ύX
		for(int j=0;j<body->TrmS[i].n2;j++){
//			for(int k=0;k<body->TrmS[i].pTI[j]->pB->CompC.N;k++){
//				NURBSC *nc = (NURBSC *)body->TrmS[i].pTI[j]->pB->CompC.pDE[k];
			for(int k=0;k<body->TrmS[i].pTI[j]->pB.CompC->N;k++){
				NURBSC *nc = body->TrmS[i].pTI[j]->pB.CompC->pDE[k].NurbsC;
				for(int l=0;l<nc->K;l++){
					nc->cp[l].x = ChangeKnot(nc->cp[l].x,body->TrmS[i].pts->S[M0-1],body->TrmS[i].pts->S[K0],val);
					nc->cp[l].y = ChangeKnot(nc->cp[l].y,body->TrmS[i].pts->T[M1-1],body->TrmS[i].pts->T[K1],val);
				}
				ChangeKnotVecRange(nc->V,nc->T,nc->N,nc->M,nc->K,val);
//				ChangeKnotVecRange(nc->V,nc->T.get(),nc->N,nc->M,nc->K,val);
			}
		}
		// �m�b�g�x�N�g���͈̔͂�ύX����
		ChangeKnotVecRange(body->TrmS[i].pts->U,body->TrmS[i].pts->S,body->TrmS[i].pts->N[0],M0,K0,val);
		ChangeKnotVecRange(body->TrmS[i].pts->V,body->TrmS[i].pts->T,body->TrmS[i].pts->N[1],M1,K1,val);
	}

	// NURBS�Ȑ�
	for(int i=0;i<body->TypeNum[_NURBSC];i++){
		if(body->NurbsC[i].EntUseFlag == 5) continue;	// ����ԏ�̋Ȑ��̂ݕύX
		ChangeKnotVecRange(body->NurbsC[i].V,body->NurbsC[i].T,body->NurbsC[i].N,body->NurbsC[i].M,body->NurbsC[i].K,val);
//		ChangeKnotVecRange(body->NurbsC[i].V,body->NurbsC[i].T.get(),body->NurbsC[i].N,body->NurbsC[i].M,body->NurbsC[i].K,val);
	}

	return KOD_TRUE;
}

// Function: SearchMinVecRange
// �m�b�g�x�N�g���񂩂�ׂ荇���m�b�g�x�N�g���̍ŏ��l��T�����Ԃ�
//
// Parameters:
// Knot[] - �m�b�g�x�N�g��
// M - �K��
// K - �R���g���[���|�C���g�̐� 
// 
// Return:
// �ŏ��l
double IGES_PARSER::SearchMinVecRange(double Knot[],int M,int K)
{
	double min = 1.0E+6;
	for(int i=M;i<=K;i++){
		double d = Knot[i]-Knot[i-1];
		if(!CheckZero(d,MID_ACCURACY) && d < min){
			min = d;
		}
	}

	return min;
}

// Function: ExpandKnotRange
// �ׂ荇���m�b�g�x�N�g���̍���MIN_KNOT_RANGE�ȏ�ɂȂ�悤�͈͂�ύX����
//
// Parameters:
// *body - BODY�ւ̃|�C���^
// 
// Return:
// KOD_TRUE
int IGES_PARSER::ExpandKnotRange(BODY *body)
{
	NormalizeKnotRange(body,NORM_KNOT_VAL);		// �m�b�g��0-1�ɐ��K��
	
	double min;

	// �g������
	for(int i=0;i<body->TypeNum[_TRIMMED_SURFACE];i++){
		int M0 = body->TrmS[i].pts->M[0];
		int M1 = body->TrmS[i].pts->M[1];
		int K0 = body->TrmS[i].pts->K[0];
		int K1 = body->TrmS[i].pts->K[1];

		double uval = NORM_KNOT_VAL;
		double vval = NORM_KNOT_VAL;
		min = SearchMinVecRange(body->TrmS[i].pts->S,M0,K0);	// u�����m�b�g�x�N�g���̍ŏ������W�𒲂ׂ�
		if(min < MIN_KNOT_RANGE) {
			uval = MIN_KNOT_RANGE/min;			// �ŏ������W��MIN_KNOT_RANGE�ɂȂ�{���𓾂�
		}

		min = SearchMinVecRange(body->TrmS[i].pts->T,M1,K1);	// v�����m�b�g�x�N�g���̍ŏ������W�𒲂ׂ�
		if(min < MIN_KNOT_RANGE){
			vval = MIN_KNOT_RANGE/min;			// �ŏ������W��MIN_KNOT_RANGE�ɂȂ�{���𓾂�
		}

		// �g�����ʂ̃p�����g���b�N���ʂɂ�����O���g�����Ȑ��̕ύX
//		for(int j=0;j<body->TrmS[i].pTO->pB->CompC.N;j++){
//			NURBSC *nc = (NURBSC *)body->TrmS[i].pTO->pB->CompC.pDE[j];
		for(int j=0;j<body->TrmS[i].pTO->pB.CompC->N;j++){
			NURBSC *nc = body->TrmS[i].pTO->pB.CompC->pDE[j].NurbsC;
			for(int k=0;k<nc->K;k++){	// �p�����g���b�N���ʏ��NURBS�Ȑ��̃R���g���[���|�C���g���m�b�g�̕ύX�ɍ��킹�ĕύX
				nc->cp[k].x = ChangeKnot(nc->cp[k].x,body->TrmS[i].pts->S[M0-1],body->TrmS[i].pts->S[K0],uval);
				nc->cp[k].y = ChangeKnot(nc->cp[k].y,body->TrmS[i].pts->T[M1-1],body->TrmS[i].pts->T[K1],vval);
			}
			ChangeKnotVecRange(nc->V,nc->T,nc->N,nc->M,nc->K,NORM_KNOT_VAL);
//			ChangeKnotVecRange(nc->V,nc->T.get(),nc->N,nc->M,nc->K,NORM_KNOT_VAL);
		}
		// �g�����ʂ̃p�����g���b�N���ʂɂ���������g�����Ȑ��̕ύX
		for(int j=0;j<body->TrmS[i].n2;j++){
//			for(int k=0;k<body->TrmS[i].pTI[j]->pB->CompC.N;k++){
//				NURBSC *nc = (NURBSC *)body->TrmS[i].pTI[j]->pB->CompC.pDE[k];
			for(int k=0;k<body->TrmS[i].pTI[j]->pB.CompC->N;k++){
				NURBSC *nc = body->TrmS[i].pTI[j]->pB.CompC->pDE[k].NurbsC;
				for(int l=0;l<nc->K;l++){
					nc->cp[l].x = ChangeKnot(nc->cp[l].x,body->TrmS[i].pts->S[M0-1],body->TrmS[i].pts->S[K0],uval);
					nc->cp[l].y = ChangeKnot(nc->cp[l].y,body->TrmS[i].pts->T[M1-1],body->TrmS[i].pts->T[K1],vval);
				}
				ChangeKnotVecRange(nc->V,nc->T,nc->N,nc->M,nc->K,NORM_KNOT_VAL);
//				ChangeKnotVecRange(nc->V,nc->T.get(),nc->N,nc->M,nc->K,NORM_KNOT_VAL);
			}
		}
		// �m�b�g�x�N�g���͈̔͂�ύX����
		ChangeKnotVecRange(body->TrmS[i].pts->U,body->TrmS[i].pts->S,body->TrmS[i].pts->N[0],M0,K0,uval);
		ChangeKnotVecRange(body->TrmS[i].pts->V,body->TrmS[i].pts->T,body->TrmS[i].pts->N[1],M1,K1,vval);
	}

	// NURBS�Ȑ�
	for(int i=0;i<body->TypeNum[_NURBSC];i++){
		if(body->NurbsC[i].EntUseFlag == 5) continue;	// ����ԏ�̋Ȑ��̂ݕύX
		ChangeKnotVecRange(body->NurbsC[i].V,body->NurbsC[i].T,body->NurbsC[i].N,body->NurbsC[i].M,body->NurbsC[i].K,NORM_KNOT_VAL);
//		ChangeKnotVecRange(body->NurbsC[i].V,body->NurbsC[i].T.get(),body->NurbsC[i].N,body->NurbsC[i].M,body->NurbsC[i].K,NORM_KNOT_VAL);
	}

	return KOD_TRUE;
}

// Function: CheckCWforTrim
// �g�����Ɏg���Ă��镡���Ȑ�����Ȃ鑽�p�`�����v��肩�����v��肩�𒲂ׁA�O���g�����͔����v���A�����g�����͎��v����ɂȂ�悤�ɕύX����
//
// Parameters:
// *body - BODY�ւ̃|�C���^
// 
// Return:
// KOD_TRUE
int IGES_PARSER::CheckCWforTrim(BODY *body)
{
	Coord *p;
	int flag;

	// �g������
	for(int i=0;i<body->TypeNum[_TRIMMED_SURFACE];i++){
//		int otrmnum = body->TrmS[i].pTO->pB->CompC.N;
		int otrmnum = body->TrmS[i].pTO->pB.CompC->N;

		if(otrmnum > 2){
			// �g�����ʂ̃p�����g���b�N���ʂɂ�����O���g�����Ȑ��̕ύX
			p = NewCoord1(otrmnum);

			// �O���g�������\������eNURBS�Ȑ��̎n�_�����o��
			for(int j=0;j<otrmnum;j++){
//				NURBSC *nc = (NURBSC *)body->TrmS[i].pTO->pB->CompC.pDE[j];
				NURBSC *nc = body->TrmS[i].pTO->pB.CompC->pDE[j].NurbsC;
				p[j] = SetCoord(nc->cp[0]);		
			}
			flag = DiscriminateCW2D(p,otrmnum);	// ���v�E�����v����𒲂ׂ�

			// �O���g���������v��肾������A�����v���ɕύX����
			if(flag == KOD_FALSE){
				for(int j=0;j<otrmnum;j++){
//					NURBSC *nc = (NURBSC *)body->TrmS[i].pTO->pB->CompC.pDE[j];
					NURBSC *nc = body->TrmS[i].pTO->pB.CompC->pDE[j].NurbsC;
					Reverse(nc->cp,nc->K);		// �R���g���[���|�C���g��̔��]
//					Reverse(nc->cp.get(),nc->K);		// �R���g���[���|�C���g��̔��]
					// �m�b�g�x�N�g����𔽓]
					for(int k=0;k<nc->N;k++){
						nc->T[k] *= -1;
						nc->T[k] += nc->V[0]+nc->V[1];
					}
					Reverse(nc->T,nc->N);
//					Reverse(nc->T.get(),nc->N);
				}
				// COMPELEM�𔽓]
//				ReverseCOMPELEM(&body->TrmS[i].pTO->pB->CompC);
				ReverseCOMPELEM(body->TrmS[i].pTO->pB.CompC);
			}

			FreeCoord1(p);
			// �O���g���������܂�
		}

		// �g�����ʂ̃p�����g���b�N���ʂɂ���������g�����Ȑ��̕ύX
		for(int j=0;j<body->TrmS[i].n2;j++){
//			otrmnum = body->TrmS[i].pTI[j]->pB->CompC.N;
			otrmnum = body->TrmS[i].pTI[j]->pB.CompC->N;

			if(otrmnum > 2){
				p = NewCoord1(otrmnum);

				// �����g�������\������eNURBS�Ȑ��̎n�_�����o��
				for(int k=0;k<otrmnum;k++){
//					NURBSC *nc = (NURBSC *)body->TrmS[i].pTI[j]->pB->CompC.pDE[k];
					NURBSC *nc = body->TrmS[i].pTI[j]->pB.CompC->pDE[k].NurbsC;
					p[k] = SetCoord(nc->cp[0]);
				}
				flag = DiscriminateCW2D(p,otrmnum);	// ���v�E�����v����𒲂ׂ�

				// �����g�����������v��肾������A���v���ɕύX����
				if(flag == KOD_TRUE){
					for(int k=0;k<otrmnum;k++){
//						NURBSC *nc = (NURBSC *)body->TrmS[i].pTI[j]->pB->CompC.pDE[k];
						NURBSC *nc = body->TrmS[i].pTI[j]->pB.CompC->pDE[k].NurbsC;
						Reverse(nc->cp,nc->K);		// �R���g���[���|�C���g��̔��]
//						Reverse(nc->cp.get(),nc->K);		// �R���g���[���|�C���g��̔��]
						// �m�b�g�x�N�g����𔽓]
						for(int l=0;l<nc->N;l++){
							nc->T[l] *= -1;
							nc->T[l] += nc->V[0]+nc->V[1];
						}
						Reverse(nc->T,nc->N);
//						Reverse(nc->T.get(),nc->N);
					}
					// COMPELEM�𔽓]
//					ReverseCOMPELEM(&body->TrmS[i].pTI[j]->pB->CompC);
					ReverseCOMPELEM(body->TrmS[i].pTI[j]->pB.CompC);
				}

				FreeCoord1(p);
			}
		}
	}

	return KOD_TRUE;
}

// Function: ReverseCOMPELEM
// COMPELEM�z��𔽓]
//
// Parameters:
// *CompC - COMPC�z��
void IGES_PARSER::ReverseCOMPELEM(COMPC *CompC)
{
	int i,j;
//	COMPELEM *tmp;
	COMPELEM tmp;

	for(i=0,j=CompC->N-1;i<j;i++,j--){
		tmp = CompC->pDE[i];
		CompC->pDE[i] = CompC->pDE[j];
		CompC->pDE[j] = tmp;
	}
}

// Function: ChangeEntityforNurbs
// NURBS�Ȑ��ȊO�̃G���e�B�e�B��NURBS�Ȑ��ɕϊ����A�ϊ��s�񂪂���΍��W�ϊ����{��
//
// Parameters:
// *dpara - �f�B���N�g�����̃p�����[�^�\���̂ւ̃|�C���^
// body - BODY�\����  
// dline - �f�B���N�g�����̃��C���� 
//
// Return:
// KOD_TRUE:����	KOD_ERR:���s
int IGES_PARSER::ChangeEntityforNurbs(DirectoryParam *dpara,BODY body,int dline)
{
	bool flag;

	for(int i=0;i<dline;i++){
		flag = KOD_FALSE;
		// �~/�~��->NURBS�Ȑ�
		if(dpara[i].entity_type == CIRCLE_ARC){
			if(body.GetNurbsCFromCirA(TypeCount[_NURBSC],dpara[i].entity_count) == KOD_ERR) return KOD_ERR;		// �~/�~�ʃp�����[�^����NURBS�Ȑ��p�����[�^�𓾂�
			InitDisplayStat(&body.NurbsC[TypeCount[_NURBSC]].Dstat);			// �\�������̏�����
			flag = KOD_TRUE;
		}
		// ����->NURBS�Ȑ�
		else if(dpara[i].entity_type == LINE){
			if(body.GetNurbsCFromLine(TypeCount[_NURBSC],dpara[i].entity_count) == KOD_ERR) return KOD_ERR;		// �����p�����[�^����NURBS�Ȑ��p�����[�^�𓾂�
			InitDisplayStat(&body.NurbsC[TypeCount[_NURBSC]].Dstat);			// �\�������̏�����
			flag = KOD_TRUE;
		}
		// �~/�~�ʁA�����ȊO�̋Ȑ��G���e�B�e�B�����݂���ꍇ�́A�V�K�ɏ�����ǉ����Ă�������

		// �ϊ��s�񉉎Z
		if(flag == KOD_TRUE){												// NURBS�ϊ����ꂽ�G���e�B�e�B�ɑ΂���
			if(dpara[i].p_tm){												// �ϊ��s�񂪑��݂���ꍇ
				for(int j=0;j<TypeCount[_TRANSFORMATION_MATRIX];j++){		// �S�Ă̕ϊ��s��^�C�v�𒲂ׂ�
					if(body.TMat[j].pD == dpara[i].p_tm){					// �ΏۂƂȂ�ϊ��s��^�C�v�ւ̃|�C���^
						if(TransformNurbsC(TypeCount[_NURBSC],j,body) == KOD_ERR) return KOD_ERR;	// NURBS�Ȑ������W�ϊ�����
					}
				}
			}
			TypeCount[_NURBSC]++;											// NURBSC�̐����C���N�������g
		}
	}

	return KOD_TRUE;
}

// Function: GetParameterSection
// �p�����[�^���̏���ǂݍ���
//
// Parameters:
// *fp - �ǂݍ���IGES�t�@�C���ւ̃|�C���^  
// *dpara - �f�B���N�g�����̃p�����[�^�\���̂ւ̃|�C���^
// body - BODY�\���� 
// dline - �f�B���N�g�����̃��C���� 
//
// Return:
// KOD_TRUE:����	KOD_ERR:���s
int IGES_PARSER::GetParameterSection(FILE *fp,DirectoryParam *dpara,BODY body,int dline)
{
	int i,j;
	char str[COLUMN_MAX*5000];	// ������o�b�t�@(5000�s���m��)
	char *p;					// ������擪���ʗp�|�C���^
	int  pD;					// �f�B���N�g�����ւ̋t�|�C���^�̒l

	// TypeCount�̏�����
	for(i=0;i<ALL_ENTITY_TYPE_NUM;i++)
		TypeCount[i] = 0;

	// �S�G���e�B�e�B�̃p�����[�^�����ꂼ��̃G���e�B�e�B�\���̂Ɋi�[���Ă���
	for(i=0;i<dline;i++){
		// �f�B���N�g����14�t�B�[���h�̏������ɁAstr�Ɋe�p�����[�^���̃��C���������q�����킹��
		strcpy(str,"");
		for(j=0;j<dpara[i].param_line_count;j++){
			fgets(buf,COLUMN_MAX_,fp);
			if((p = strchr(buf,';')) == NULL){
				p = strchr(buf,' ');
			}
			else{
				buf[p-buf] = '\0';		// ���R�[�h�f���~�^���o���́A���R�[�h�f���~�^�����I�[�����ɂ���
			}
			strncat(str,buf,p-buf+1);	// ��������e�p�����[�^���̃��C�������q�����킹�Ă���
		}
		p = &buf[COL_P_DIRECTORY];		// �f�B���N�g�����ւ̋t�|�C���^�̒l�����炩���ߒ��ׂĂ����i�֋X��j
		sscanf(p,"%d",&pD);

		// str�𕪉����e�G���e�B�e�B�\���̂ɏ��𖄂߂Ă���
		// ���̃G���e�B�e�B��ǉ�����ꍇ�͈ȉ��ɃR�[�h��ǉ�����

		// �~�E�~��(NURBS�Ȑ��Ƃ��ẴG���e�B�e�B���������ɓ���)
		if(dpara[i].entity_type == CIRCLE_ARC){							
			if(GetCirAPara(str,pD,dpara,body) == KOD_ERR)  return KOD_ERR;					// �~/�~�ʃp�����[�^�̎擾
			body.CirA[TypeCount[_CIRCLE_ARC]].EntUseFlag = dpara[i].useflag_stat;	// �f�B���N�g�����̏��"Entity Use Flag"�𓾂�
			dpara[i].entity_count = TypeCount[_CIRCLE_ARC];							// dpara��body���֘A�t����
			TypeCount[_CIRCLE_ARC]++;					// �~�E�~�ʃ^�C�v�̐����C���N�������g
		}
		// �����Ȑ�
		else if(dpara[i].entity_type == COMPOSITE_CURVE){					
			if(GetCompCPara(str,pD,dpara,dline,body) == KOD_ERR)  return KOD_ERR;
			dpara[i].entity_count = TypeCount[_COMPOSITE_CURVE];					// dpara��body���֘A�t����
			TypeCount[_COMPOSITE_CURVE]++;				// �����Ȑ��^�C�v�̐����C���N�������g
		}
		// �~���Ȑ�
		else if(dpara[i].entity_type == CONIC_ARC){											
			if(GetConAPara(str,pD,dpara,body) == KOD_ERR)  return KOD_ERR;
			TypeCount[_CONIC_ARC]++;					// �~���Ȑ��^�C�v�̐����C���N�������g
		}
		// ����(NURBS�Ȑ��Ƃ��ẴG���e�B�e�B���������ɓ���)
		else if(dpara[i].entity_type == LINE){									
			if(GetLinePara(str,pD,dpara,body) == KOD_ERR)  return KOD_ERR;				// �����p�����[�^�̎擾
			body.Line[TypeCount[_LINE]].EntUseFlag = dpara[i].useflag_stat;		// �f�B���N�g�����̏��"Entity Use Flag"�𓾂�(LINE)
			dpara[i].entity_count = TypeCount[_LINE];							// dpara��body���֘A�t����
			TypeCount[_LINE]++;							// �����^�C�v�̐����C���N�������g
		}
		// �ϊ��s��
		else if(dpara[i].entity_type == TRANSFORMATION_MATRIX){			
			if(GetTMatPara(str,pD,dpara,body) == KOD_ERR)  return KOD_ERR;
			dpara[i].entity_count = TypeCount[_TRANSFORMATION_MATRIX];			// dpara��body���֘A�t����
			TypeCount[_TRANSFORMATION_MATRIX]++;		// �ϊ��s��^�C�v�̐����C���N�������g
		}
		// NURBS�Ȑ�
		else if(dpara[i].entity_type == NURBS_CURVE){		
			if(GetNurbsCPara(str,pD,dpara,body) == KOD_ERR)  return KOD_ERR;
			body.NurbsC[TypeCount[_NURBSC]].EntUseFlag = dpara[i].useflag_stat;	// �f�B���N�g�����̏��"Entity Use Flag"�𓾂�
			body.NurbsC[TypeCount[_NURBSC]].OriginEnt = NURBS_CURVE;			// ������NURBS�Ȑ��v�f�ł��邱�Ƃ𖾎�
			body.NurbsC[TypeCount[_NURBSC]].pOriginEnt = NULL;					// �Q�ƌ���NULL
			dpara[i].entity_count = TypeCount[_NURBSC];							// dpara��body���֘A�t����
			TypeCount[_NURBSC]++;		// NURBS�Ȑ��^�C�v�̐����C���N�������g
		}
		// NURBS�Ȗ�
		else if(dpara[i].entity_type == NURBS_SURFACE){		
			if(GetNurbsSPara(str,pD,dpara,body) == KOD_ERR)  return KOD_ERR;
			dpara[i].entity_count = TypeCount[_NURBSS];							// dpara��body���֘A�t����
			TypeCount[_NURBSS]++;		// NURBS�Ȗʃ^�C�v�̐����C���N�������g
		}
		// �ʏ��
		else if(dpara[i].entity_type == CURVE_ON_PARAMETRIC_SURFACE){	
			if(GeConpSPara(str,pD,dpara,dline,body) == KOD_ERR)  return KOD_ERR;
			dpara[i].entity_count = TypeCount[_CURVE_ON_PARAMETRIC_SURFACE];	// dpara��body���֘A�t����
			TypeCount[_CURVE_ON_PARAMETRIC_SURFACE]++;	// �ʏ���^�C�v�̐����C���N�������g
		}
		// �g������	
		else if(dpara[i].entity_type == TRIMMED_SURFACE){				
			if(GetTrmSPara(str,pD,dpara,body) == KOD_ERR)  return KOD_ERR;
			dpara[i].entity_count = TypeCount[_TRIMMED_SURFACE];				// dpara��body���֘A�t����
			TypeCount[_TRIMMED_SURFACE]++;				// �g�����ʃ^�C�v�̐����C���N�������g
		}
		// �T�|�[�g���Ă��Ȃ�Entity Type�̏ꍇ
		else{
		//	char mes[256];
			//sprintf(mes,"Entity Type #%d:Unsupported",dpara[i].entity_type);
		//	GuiIF.SetMessage(mes);
			continue;
		}
	}

	return KOD_TRUE;
}

// Function: GetCirAPara
// Type100 �~�E�~�ʂ�ǂݍ���
//
// Parameters:
// str[] - ������o�b�t�@
// pD - �f�B���N�g�����ւ̋t�|�C���^�̒l  
// *dpara - �f�B���N�g�����̃p�����[�^�\���̂ւ̃|�C���^ 
// body - BODY�\���� 
//
// Return:
// KOD_TRUE
int IGES_PARSER::GetCirAPara(char str[],int pD,DirectoryParam *dpara,BODY body)
{
//	int i;
	char *p;
	double x[3],y[3];

	p = str;

	body.CirA[TypeCount[_CIRCLE_ARC]].zt = CatchStringD(&p);		// Z�������̐[��
	x[0] = CatchStringD(&p);							// ���S���WX
	body.CirA[TypeCount[_CIRCLE_ARC]].cp[0].x = x[0];
	y[0] = CatchStringD(&p);							// ���S���WY
	body.CirA[TypeCount[_CIRCLE_ARC]].cp[0].y = y[0];
	x[1] = CatchStringD(&p);							// �n�_X
	body.CirA[TypeCount[_CIRCLE_ARC]].cp[1].x = x[1];
	y[1] = CatchStringD(&p);							// �n�_Y
	body.CirA[TypeCount[_CIRCLE_ARC]].cp[1].y = y[1];
	x[2] = CatchStringD(&p);							// �I�_X
	body.CirA[TypeCount[_CIRCLE_ARC]].cp[2].x = x[2];
	y[2] = CatchStringD(&p);							// �I�_Y
	body.CirA[TypeCount[_CIRCLE_ARC]].cp[2].y = y[2];

	body.CirA[TypeCount[_CIRCLE_ARC]].R = sqrt((x[1]-x[0])*(x[1]-x[0])+(y[1]-y[0])*(y[1]-y[0]));	// ���a�Z�o

	body.CirA[TypeCount[_CIRCLE_ARC]].pD = pD;		// �f�B���N�g�����ւ̋t�|�C���^�̒l

	InitDisplayStat(&body.CirA[TypeCount[_CIRCLE_ARC]].Dstat);	// �\�������̏�����

	return KOD_TRUE;
}

// Function: GetConAPara
// Type104 �~���Ȑ��̓ǂݍ���(������)
// 
// Parameters:
// str[] - ������o�b�t�@
// pD - �f�B���N�g�����ւ̋t�|�C���^�̒l  
// *dpara - �f�B���N�g�����̃p�����[�^�\���̂ւ̃|�C���^ 
// body - BODY�\���� 
//
// Return:
// KOD_TRUE
int IGES_PARSER::GetConAPara(char str[],int pD,DirectoryParam *dpara,BODY body)
{
	GuiIF.SetMessage("Type104:Unmounted");
	return KOD_TRUE;
}

// Function: GetLinePara
// Type110 �����̓ǂݍ���
//
// Parameters:
// str[] - ������o�b�t�@
// pD - �f�B���N�g�����ւ̋t�|�C���^�̒l  
// *dpara - �f�B���N�g�����̃p�����[�^�\���̂ւ̃|�C���^ 
// body - BODY�\���� 
//
// Return:
// KOD_TRUE
int IGES_PARSER::GetLinePara(char str[],int pD,DirectoryParam *dpara,BODY body)
{
	char *p;

	p = str;

	body.Line[TypeCount[_LINE]].cp[0].x = CatchStringD(&p);		// �n�_��X���W
	body.Line[TypeCount[_LINE]].cp[0].y = CatchStringD(&p);		// �n�_��Y���W
	body.Line[TypeCount[_LINE]].cp[0].z = CatchStringD(&p);		// �n�_��Z���W
	body.Line[TypeCount[_LINE]].cp[1].x = CatchStringD(&p);		// �I�_��X���W
	body.Line[TypeCount[_LINE]].cp[1].y = CatchStringD(&p);		// �I�_��Y���W
	body.Line[TypeCount[_LINE]].cp[1].z = CatchStringD(&p);		// �I�_��Z���W

	body.Line[TypeCount[_LINE]].pD = pD;		// �f�B���N�g�����ւ̋t�|�C���^�̒l

	InitDisplayStat(&body.Line[TypeCount[_LINE]].Dstat);	// �\�������̏�����

	return KOD_TRUE;
}

// Function: GetTMatPara
// Type124 �ϊ��s��̓ǂݍ���
//
// Parameters:
// str[] - ������o�b�t�@
// pD - �f�B���N�g�����ւ̋t�|�C���^�̒l  
// *dpara - �f�B���N�g�����̃p�����[�^�\���̂ւ̃|�C���^ 
// body - BODY�\���� 
//
// Return:
// KOD_TRUE
int IGES_PARSER::GetTMatPara(char str[],int pD,DirectoryParam *dpara,BODY body)
{
	int i,j;
	char *p;
	
	p = str;
	for(i=0;i<3;i++){
		for(j=0;j<4;j++){
			if(j != 3){
				body.TMat[TypeCount[_TRANSFORMATION_MATRIX]].R[i][j] = CatchStringD(&p);	// 3�~3��]�s�񐬕�
			}
			else{
				body.TMat[TypeCount[_TRANSFORMATION_MATRIX]].T[i] = CatchStringD(&p);		// ���i�x�N�g������
			}
		}
	}
	
	body.TMat[TypeCount[_TRANSFORMATION_MATRIX]].pD = pD;		// DE���ւ̋t�|�C���^�̒l
	
	return KOD_TRUE;
}

// Function: GetNurbsCPara
// Type126 NRBS�Ȑ��̓ǂݍ���
//
// Parameters:
// str[] - ������o�b�t�@
// pD - �f�B���N�g�����ւ̋t�|�C���^�̒l  
// *dpara - �f�B���N�g�����̃p�����[�^�\���̂ւ̃|�C���^ 
// body - BODY�\���� 
//
// Return:
// KOD_TRUE:����	KOD_ERR:�������[�m�ۂɎ��s
int IGES_PARSER::GetNurbsCPara(char str[],int pD,DirectoryParam *dpara,BODY body)
{
	char *p;
	int i=0;

	p = str;
	body.NurbsC[TypeCount[_NURBSC]].K = CatchStringI(&p) + 1;		// ���a�L���̏㑤�Y���i�R���g���[���|�C���g-1�j�̒l
	body.NurbsC[TypeCount[_NURBSC]].M = CatchStringI(&p) + 1;		// ���֐��̊K��
	body.NurbsC[TypeCount[_NURBSC]].N = body.NurbsC[TypeCount[_NURBSC]].K + body.NurbsC[TypeCount[_NURBSC]].M;	// �m�b�g�x�N�g���̐�
	for(i=0;i<4;i++){	// �u�[���A���^�v���p�e�B4��
		body.NurbsC[TypeCount[_NURBSC]].prop[i] = CatchStringI(&p);
	}

	// �������[�m��
	if(NFunc.New_NurbsC(&body.NurbsC[TypeCount[_NURBSC]],body.NurbsC[TypeCount[_NURBSC]].K,body.NurbsC[TypeCount[_NURBSC]].N) == KOD_ERR){
		GuiIF.SetMessage("PARAMETER SECTION KOD_ERROR:fail to allocate memory");
		return KOD_ERR;
	}

	for(i=0;i<body.NurbsC[TypeCount[_NURBSC]].N;i++){
		body.NurbsC[TypeCount[_NURBSC]].T[i] = CatchStringD(&p);	// �m�b�g�x�N�g���̒l
	}
	for(i=0;i<body.NurbsC[TypeCount[_NURBSC]].K;i++){				// Weight�̒l
		body.NurbsC[TypeCount[_NURBSC]].W[i] = CatchStringD(&p);
	}
	for(i=0;i<body.NurbsC[TypeCount[_NURBSC]].K;i++){				// �R���g���[���|�C���g�̍��W�l
		body.NurbsC[TypeCount[_NURBSC]].cp[i].x = CatchStringD(&p);
		body.NurbsC[TypeCount[_NURBSC]].cp[i].y = CatchStringD(&p);
		body.NurbsC[TypeCount[_NURBSC]].cp[i].z = CatchStringD(&p);
	}
	body.NurbsC[TypeCount[_NURBSC]].V[0] = CatchStringD(&p);		// �p�����[�^�͈̔�
	body.NurbsC[TypeCount[_NURBSC]].V[1] = CatchStringD(&p);

	// �@���x�N�g���͋L�q����Ă���ꍇ�Ƃ���Ă��Ȃ��ꍇ������悤�Ȃ̂ŁA�L�q����Ă���ꍇ�̂ݓǂݍ���
	if(strchr(p,',') != NULL){
		body.NurbsC[TypeCount[_NURBSC]].norm.x = CatchStringD(&p);	// �@���x�N�g��
		body.NurbsC[TypeCount[_NURBSC]].norm.y = CatchStringD(&p);
		body.NurbsC[TypeCount[_NURBSC]].norm.z = CatchStringD(&p);
	}

	body.NurbsC[TypeCount[_NURBSC]].pD = pD;		// DE���ւ̋t�|�C���^�̒l

	InitDisplayStat(&body.NurbsC[TypeCount[_NURBSC]].Dstat);	// �\�������̏�����

	return KOD_TRUE;
}

// Function: GetNurbsSPara
// Type128 NURBS�Ȗʂ̓ǂݍ���
//
// Parameters:
// str[] - ������o�b�t�@
// pD - �f�B���N�g�����ւ̋t�|�C���^�̒l  
// *dpara - �f�B���N�g�����̃p�����[�^�\���̂ւ̃|�C���^ 
// body - BODY�\���� 
//
// Return:
// KOD_TRUE:����	KOD_ERR:�������[�m�ۂɎ��s
int IGES_PARSER::GetNurbsSPara(char str[],int pD,DirectoryParam *dpara,BODY body)
{
	char *p;
	int i=0,j=0;

	p = str;

	body.NurbsS[TypeCount[_NURBSS]].K[0] = CatchStringI(&p) + 1;	// u�����R���g���[���|�C���g�̐�
	body.NurbsS[TypeCount[_NURBSS]].K[1] = CatchStringI(&p) + 1;	// v�����R���g���[���|�C���g�̐�
	body.NurbsS[TypeCount[_NURBSS]].M[0] = CatchStringI(&p) + 1;	// ���֐���u�����K��
	body.NurbsS[TypeCount[_NURBSS]].M[1] = CatchStringI(&p) + 1;	// ���֐���v�����K��
	body.NurbsS[TypeCount[_NURBSS]].N[0] = body.NurbsS[TypeCount[_NURBSS]].K[0] + body.NurbsS[TypeCount[_NURBSS]].M[0];	// u�����m�b�g�x�N�g���̐�
	body.NurbsS[TypeCount[_NURBSS]].N[1] = body.NurbsS[TypeCount[_NURBSS]].K[1] + body.NurbsS[TypeCount[_NURBSS]].M[1];	// v�����m�b�g�x�N�g���̐�
	for(i=0;i<5;i++){
		body.NurbsS[TypeCount[_NURBSS]].prop[i] = CatchStringI(&p);	// �u�[���A���^�v���p�e�B5��
	}

	// �������[�m��
	if(NFunc.New_NurbsS(&body.NurbsS[TypeCount[_NURBSS]],body.NurbsS[TypeCount[_NURBSS]].K,body.NurbsS[TypeCount[_NURBSS]].N) == KOD_ERR){
		GuiIF.SetMessage("PARAMETER SECTION KOD_ERROR:fail to allocate memory");
		return KOD_ERR;
	}
	
	for(i=0;i<body.NurbsS[TypeCount[_NURBSS]].N[0];i++){
		body.NurbsS[TypeCount[_NURBSS]].S[i] = CatchStringD(&p);	// u�����m�b�g�x�N�g��
	}
	for(i=0;i<body.NurbsS[TypeCount[_NURBSS]].N[1];i++){
		body.NurbsS[TypeCount[_NURBSS]].T[i] = CatchStringD(&p);	// v�����m�b�g�x�N�g��
	}
	for(i=0;i<body.NurbsS[TypeCount[_NURBSS]].K[1];i++){
		for(j=0;j<body.NurbsS[TypeCount[_NURBSS]].K[0];j++){
			body.NurbsS[TypeCount[_NURBSS]].W[j][i] = CatchStringD(&p);	//  u����Weight
		}
	}
	for(i=0;i<body.NurbsS[TypeCount[_NURBSS]].K[1];i++){
		for(j=0;j<body.NurbsS[TypeCount[_NURBSS]].K[0];j++){
			body.NurbsS[TypeCount[_NURBSS]].cp[j][i].x = CatchStringD(&p);	// �R���g���[���|�C���gX
			body.NurbsS[TypeCount[_NURBSS]].cp[j][i].y = CatchStringD(&p);	// �R���g���[���|�C���gY
			body.NurbsS[TypeCount[_NURBSS]].cp[j][i].z = CatchStringD(&p);	// �R���g���[���|�C���gZ
		}
	}
	body.NurbsS[TypeCount[_NURBSS]].U[0] = CatchStringD(&p);			// u�����̊J�n�l
	body.NurbsS[TypeCount[_NURBSS]].U[1] = CatchStringD(&p);			// u�����̏I���l
	body.NurbsS[TypeCount[_NURBSS]].V[0] = CatchStringD(&p);			// v�����̊J�n�l
	body.NurbsS[TypeCount[_NURBSS]].V[1] = CatchStringD(&p);			// v�����̏I���l

	body.NurbsS[TypeCount[_NURBSS]].pD = pD;		// DE���ւ̋t�|�C���^�̒l

	body.NurbsS[TypeCount[_NURBSS]].TrmdSurfFlag = KOD_FALSE;	// �Ƃ肠�����g��������Ă��Ȃ��Ɨ��ʂƂ��Ă���(Type144��ǂ݂Ɍ������Ƃ��ɕύX�����)

	body.ChangeStatColor(body.NurbsS[TypeCount[_NURBSS]].Dstat.Color,0.2,0.2,0.2,0.5);	// �Ȗʂ̐F��ݒ�

	return KOD_TRUE;
}

// Function: GetCompCPara
// Type102 �����Ȑ��̓ǂݍ���
//
// Parameters:
// str[] - ������o�b�t�@
// pD - �f�B���N�g�����ւ̋t�|�C���^�̒l  
// *dpara - �f�B���N�g�����̃p�����[�^�\���̂ւ̃|�C���^ 
// body - BODY�\���� 
//
// Return:
// KOD_TRUE:����	KOD_ERR:�������[�m�ۂɎ��s
int IGES_PARSER::GetCompCPara(char str[],int pD,DirectoryParam *dpara,int dline,BODY body)
{
	char *p;
	int  pdnum;		// DE���̃V�[�P���X�i���o�[�擾�p
	int  i;

	p = str;

	body.CompC[TypeCount[_COMPOSITE_CURVE]].N = CatchStringI(&p);	// �����Ȑ��̍\���v�f��

	// �����Ȑ��̃������[���m��
	if(NFunc.New_CompC(&body.CompC[TypeCount[_COMPOSITE_CURVE]],body.CompC[TypeCount[_COMPOSITE_CURVE]].N) == KOD_ERR){
		GuiIF.SetMessage("PARAMETER SECTION KOD_ERROR:fail to allocate memory");
		return KOD_ERR;
	}

	for(i=0;i<body.CompC[TypeCount[_COMPOSITE_CURVE]].N;i++){		// �\���v�f��DE���ւ̃|�C���^�l
		pdnum = CatchStringI(&p);		// �e�\���v�f��DE���̃V�[�P���X�i���o�[�𓾂�
		body.CompC[TypeCount[_COMPOSITE_CURVE]].DEType[i] = SearchEntType(dpara,pdnum,dline);		// pdnum�������G���e�B�e�B�^�C�v�𔻕�
//		body.CompC[TypeCount[_COMPOSITE_CURVE]].pDE[i] = (COMPELEM *)GetDEPointer(pdnum,body);		// pdnum�������\���̂̃|�C���^�𓾂�
		body.CompC[TypeCount[_COMPOSITE_CURVE]].pDE[i].substitution = GetDEPointer(pdnum,body);		// pdnum�������\���̂̃|�C���^�𓾂�
	}

	body.CompC[TypeCount[_COMPOSITE_CURVE]].pD = pD;		// DE���ւ̋t�|�C���^�̒l

	return KOD_TRUE;
}

// Function: GeConpSPara
// Type142 �ʏ���̓ǂݍ���
//
// Parameters:
// str[] - ������o�b�t�@
// pD - �f�B���N�g�����ւ̋t�|�C���^�̒l  
// *dpara - �f�B���N�g�����̃p�����[�^�\���̂ւ̃|�C���^ 
// body - BODY�\���� 
//
// Return:
// KOD_TRUE
int IGES_PARSER::GeConpSPara(char str[],int pD,DirectoryParam *dpara,int dline,BODY body)
{
	char *p;
	int pdnum;		// DE���̃V�[�P���X�i���o�[�擾�p

	p = str;

	body.ConpS[TypeCount[_CURVE_ON_PARAMETRIC_SURFACE]].crtn = CatchStringI(&p);	// �ʏ�����ǂ̂悤�ɍ��ꂽ����\��

	pdnum = CatchStringI(&p);			// Curve�����Surface��DE���̃V�[�P���X�i���o�[�𓾂�
	body.ConpS[TypeCount[_CURVE_ON_PARAMETRIC_SURFACE]].SType = SearchEntType(dpara,pdnum,dline);	// pdnum�������G���e�B�e�B�^�C�v�𔻕�
	body.ConpS[TypeCount[_CURVE_ON_PARAMETRIC_SURFACE]].pS = (NURBSS *)GetDEPointer(pdnum,body);		// pdnum�������\���̂̃|�C���^�𓾂�

	pdnum = CatchStringI(&p);			// Surface�̃p�����[�^��Ԃɂ�����curve���`����Entity��DE���̃V�[�P���X�i���o�[�𓾂�
	body.ConpS[TypeCount[_CURVE_ON_PARAMETRIC_SURFACE]].BType = SearchEntType(dpara,pdnum,dline);	// pdnum�������G���e�B�e�B�^�C�v�𔻕�
//	body.ConpS[TypeCount[_CURVE_ON_PARAMETRIC_SURFACE]].pB = (CURVE *)GetDEPointer(pdnum,body);		// pdnum�������\���̂̃|�C���^�𓾂�(���p��)
	body.ConpS[TypeCount[_CURVE_ON_PARAMETRIC_SURFACE]].pB.substitution = GetDEPointer(pdnum,body);		// pdnum�������\���̂̃|�C���^�𓾂�(���p��)

	pdnum = CatchStringI(&p);			// Curve C��DE���ւ̃|�C���^
	body.ConpS[TypeCount[_CURVE_ON_PARAMETRIC_SURFACE]].CType = SearchEntType(dpara,pdnum,dline);	// pdnum�������G���e�B�e�B�^�C�v�𔻕�
//	body.ConpS[TypeCount[_CURVE_ON_PARAMETRIC_SURFACE]].pC = (CURVE *)GetDEPointer(pdnum,body);		// pdnum�������\���̂̃|�C���^�𓾂�(���p��)
	body.ConpS[TypeCount[_CURVE_ON_PARAMETRIC_SURFACE]].pC.substitution = GetDEPointer(pdnum,body);		// pdnum�������\���̂̃|�C���^�𓾂�(���p��)

	body.ConpS[TypeCount[_CURVE_ON_PARAMETRIC_SURFACE]].pref = CatchStringI(&p);	// ���葤�V�X�e���ō̂��Ă����\����\���t���O

	body.ConpS[TypeCount[_CURVE_ON_PARAMETRIC_SURFACE]].pD = pD;	// DE���̃V�[�P���X�i���o�[�𓾂�

	return KOD_TRUE;
}

// Function: GetTrmSPara
// Type144 �g�����ʂ̓ǂݍ���
//
// Parameters:
// str[] - ������o�b�t�@
// pD - �f�B���N�g�����ւ̋t�|�C���^�̒l  
// *dpara - �f�B���N�g�����̃p�����[�^�\���̂ւ̃|�C���^ 
// body - BODY�\���� 
//
// Return:
// KOD_TRUE:����	KOD_ERR:�������[�m�ۂɎ��s
int IGES_PARSER::GetTrmSPara(char str[],int pD,DirectoryParam *dpara,BODY body)
{
	char *p;
	int  i;
	int  pdnum;		// DE���̃V�[�P���X�i���o�[�擾�p

	p = str;
	
	pdnum = CatchStringI(&p);		// �g���������Surface Entity��DE���̒l���擾
	body.TrmS[TypeCount[_TRIMMED_SURFACE]].pts = (NURBSS *)GetDEPointer(pdnum,body);		// �g���������Surface Entity�ւ̃|�C���^���擾
	body.TrmS[TypeCount[_TRIMMED_SURFACE]].pts->TrmdSurfFlag = KOD_TRUE;		// �g�����ʂƂ��Ă�NURBS�Ȗʂł��邱�Ƃ�����
	body.TrmS[TypeCount[_TRIMMED_SURFACE]].n1 = CatchStringI(&p);		// �O�F�O����D�̋��E�ƈ�v���Ă���@�P�F����ȊO
	body.TrmS[TypeCount[_TRIMMED_SURFACE]].n2 = CatchStringI(&p);		// Trimmed Surface�̓����̒P���Ȑ��̐�

	pdnum = CatchStringI(&p);		// Trimmed Surface�̊O���̒P���Ȑ��̐�
	body.TrmS[TypeCount[_TRIMMED_SURFACE]].pTO = (CONPS *)GetDEPointer(pdnum,body); // �P���Ȑ��\���̂ւ̃|�C���^���擾

	// �P���Ȑ�N2�̐������������[�m��
	if((NFunc.New_TrmS(&body.TrmS[TypeCount[_TRIMMED_SURFACE]],body.TrmS[TypeCount[_TRIMMED_SURFACE]].n2)) == KOD_ERR){
		GuiIF.SetMessage("PARAMETER SECTION KOD_ERROR:fail to allocate memory");
		return KOD_ERR;
	}

	for(i=0;i<body.TrmS[TypeCount[_TRIMMED_SURFACE]].n2;i++){
		pdnum = CatchStringI(&p);	// Trimmed Surface�̓����̒P���Ȑ���DE���̒l���擾
		body.TrmS[TypeCount[_TRIMMED_SURFACE]].pTI[i] = (CONPS *)GetDEPointer(pdnum,body);	// �P���Ȑ��\���̂ւ̃|�C���^���擾
	}

	body.TrmS[TypeCount[_TRIMMED_SURFACE]].pD = pD;		// DE���̃V�[�P���X�i���o�[�𓾂�

	return KOD_TRUE;
}

// Function: GetDirectorySection
// �f�B���N�g�����ǂݍ���
//
// Parameters:
// *fp - �ǂݍ���IGES�t�@�C���ւ̃|�C���^
// *dpara - �f�B���N�g�����̃p�����[�^�\���̂ւ̃|�C���^ 
// TypeNum[] - BODY���\������e�G���e�B�e�B�̐� 
// dline - �f�B���N�g�����̃��C���� 
//
// Return:
// KOD_TRUE:����	KOD_ERR:���s
int IGES_PARSER::GetDirectorySection(FILE *fp,DirectoryParam *dpara,int TypeNum[],int dline)
{
	int i,j;
	char *p;						// ������擪���ʗp�|�C���^
	char str[COLUMN_MAX*2+1];		// 2�s���i1�G���e�B�e�B���j�̕�����z��
	char field[FIELD_NUM+1];		// 8������1�t�B�[���h
	char dmy;

	for(i=0;i<ALL_ENTITY_TYPE_NUM;i++){
		TypeNum[i] = 0;			// ������
	}

	for(i=0;i<dline;i++){
		strcpy(str,"");				// str������

		if(fgets(buf,COLUMN_MAX_,fp) == NULL){		// i�Ԗڂ̃G���e�B�e�B��1�s�ڂ�ǂݍ���
			GuiIF.SetMessage("DIRECTORY SECTION KOD_ERROR: fail to read this file");
			exit(KOD_ERR);
		}
		strncat(str,buf,COLUMN_MAX);
		if(fgets(buf,COLUMN_MAX_,fp) == NULL){		// i�Ԗڂ̃G���e�B�e�B��2�s�ڂ�ǂݍ���
			GuiIF.SetMessage("DIRECTORY SECTION KOD_ERROR: fail to read this file");
			exit(KOD_ERR);
		}
		strncat(str,buf,COLUMN_MAX);				// �ǂݍ���2�s��str�ɑS�Ċi�[�����

		p = str;									// p���܂�str�̐擪�ɂ���
		for(j=0;j<DIRECTORYPARANUM;j++){			// �f�B���N�g�����̃p�����[�^�̐����������[�v
			strncpy(field,p,8);						// p�̐擪8������field�֊i�[
			field[FIELD_NUM] = '\0';				// �ꉞ�A�I�[������field�̂��K�ɂ��Ă���
			p += FIELD_NUM;							// p�����̃t�B�[���h�̐擪�ֈړ�
			// �f�B���N�g�����̏�񂪕K�v�ȏꍇ�͈ȉ��ɃR�[�h��ǉ�����
			if(j == ENTITY_TYPE_NUM){					// �v�f�ԍ����擾
				dpara[i].entity_type = atoi(field);
				GetType(dpara[i].entity_type,TypeNum);	// �G���e�B�e�B�^�C�v�̐������Z
			}
			else if(j == PARAM_DATA){					// �p�����[�^���ւ̃|�C���^���擾
				dpara[i].p_param = atoi(field);
			}
			else if(j == TRAN_MATRIX){					// �}�g���b�N�X�ւ̃|�C���^���擾
				dpara[i].p_tm = atoi(field);
			}
			else if(j == STATUS_NUM){					// �X�e�[�^�X���擾
				GetStatusNumber(field,&dpara[i]);
			}
			else if(j == SEQUENCE_NUM){					// �V�[�P���X�ԍ����擾
				sscanf(field,"%c %d",&dmy,&dpara[i].seq_num);
			}
			else if(j == PARAM_LINE_COUNT){				// �p�����[�^���̃��C����
				dpara[i].param_line_count = atoi(field);
			}
		}
	}
	
	return KOD_TRUE;
}

// Function: GetType
// �e�G���e�B�e�B�^�C�v�̐����擾����
//
// Parameters:
// type - �G���e�B�e�B�̃^�C�v
// entitynum[] - �G���e�B�e�B�̐� 
void IGES_PARSER::GetType(int type,int entitynum[])
{
	int i;

	// �����܂��͉~�E�~�ʃG���e�B�e�B�̏ꍇ��NURBS�Ȑ��G���e�B�e�B�������ɃC���N�������g
	if(type == LINE || type == CIRCLE_ARC){
		entitynum[_NURBSC]++;
	}

	for(i=0;i<ALL_ENTITY_TYPE_NUM;i++){
		if(type == entity[i]){
			entitynum[i]++;		// i��enum�^EntityTyp�ɑΉ� ex) entitynum[10]��_NURBSC�i�L��B�X�v���C���Ȑ��j�̐���\��
		}
	}
}

// Function: GetStatusNumber
// DE#9(�X�e�[�^�X)���̓ǂݍ���
//
// Parameters:
// field[] - �t�B�[���h
// *dpara - �f�B���N�g�����̃p�����[�^�\���̂ւ̃|�C���^
void IGES_PARSER::GetStatusNumber(char field[],DirectoryParam *dpara)
{
	char str[3]="";
	char *p;

	p = field;
	strncpy(str,p,2);
	sscanf(str,"%d",&dpara->blank_stat);
	p += 2;
	strncpy(str,p,2);
	sscanf(str,"%d",&dpara->subordinate_stat);
	p += 2;
	strncpy(str,p,2);
	sscanf(str,"%d",&dpara->useflag_stat);
	p += 2;
}

// Function: GetGlobalSection
// �O���[�o�����ǂݍ���
//
// Parameters:
// *fp - �ǂݍ���IGES�t�@�C���ւ̃|�C���^
// *gpara - �O���[�o�����̃p�����[�^�\���̂ւ̃|�C���^
// gline - �O���[�o�����̃��C����
//
// Return:
// KOD_TRUE:����	KOD_ERR:���s
int IGES_PARSER::GetGlobalSection(FILE *fp,GlobalParam *gpara,int gline)
{
	char *str;					// �O���[�o����������S�Ă��i�[����
	char para_delim = ',';		// �p�����[�^�f���~�^
	char record_delim = ';';	// ���R�[�h�f���~�^
//	int  para_length;			// �e�p�����[�^�̕�����̒������i�[
	char *p,*p_;
	int  i;

	// �O���[�o�����̃��C����*COL_CHAR���������[�m��
//	str = (char *)malloc(sizeof(char) * gline*COL_CHAR);
	str = new char[gline*COL_CHAR];
	if(str == NULL){
		GuiIF.SetMessage("GLOBAL SECTION KOD_ERROR: faile to allocate memory");
		exit(KOD_ERR);
	}

	strcpy(str,"");			// str������
	for(i=0;i<gline;i++){
		if(fgets(buf,COLUMN_MAX_,fp) == NULL){
			GuiIF.SetMessage("GLOBAL SECTION KOD_ERROR: faile to read this file");
			exit(KOD_ERR);
		}
		strncat(str,buf,COL_CHAR-1);	// str�ɃZ�N�V�������ʕ����܂ł̕�������������Ă���
	}

	// �p�����[�^�f���~�^�ƃ��R�[�h�f���~�^�̃`�F�b�N(�C���x���^�Ή��̂��߁C�R�����g�A�E�g�i2015/0706�j)
	//sscanf(str,"%dH%c",&para_length,&para_delim);		// �p�����[�^�f���~�^���擾
	//fprintf(stderr,"%d,%c\n",para_length,para_delim);
	//if(para_delim != ','){
	//	GuiIF.SetMessage("GLOBAL SECTION KOD_ERROR: The parameter delimiter is not governed by and construed for JAMA-IS");
	//	exit(KOD_ERR);
	//}
	//else{
	//	sscanf(str,"%dH[^,],[^,],%dH%c",&para_length,&para_length,&record_delim);		// ���R�[�h�f���~�^���擾
	//	if(record_delim != ';'){
	//		GuiIF.SetMessage("GLOBAL SECTION KOD_ERROR: The record delimiter is not governed by and construed for JAMA-IS");
	//		exit(KOD_ERR);
	//	}
	//}

	// �C���x���^�Ή��D�p�����[�^�f���~�^�ƃ��R�[�h�f���~�^��ǂݔ�΂�
	p = p_ = str;
	p = strchr(p_,',');
	strncpy(str,p_,p-p_);
	str[p-p_] = '\0';
	if(strchr(str,'H') == NULL){
		p += 2;
	}
	else{
		p += 2;
		p = strchr(p,',');
		p++;
	}
	p_ = p;

	for(i=3;i<GLOBALPARAMNUM;i++){		// 2�̃f���~�^�𔲂������c��̃p�����[�^�𒀎��ǂݍ���
		if((p = strchr(p_,',')) == NULL){
			GuiIF.SetMessage("GLOBAL SECTION KOD_ERROR: Low parameter count of global section");
			exit(KOD_ERR);
		}
		strncpy(str,p_,p-p_);
		str[p-p_] = '\0';

		// �O���[�o�����̏�񂪕K�v�ȏꍇ�͈ȉ��ɃR�[�h���L��
		if(i == MODEL_SCALE){				// ���f���X�P�[���ǂݍ���
			gpara->scale = atof(str);
			fprintf(stderr,"%lf\n",gpara->scale);	// debug
		}
		else if(i == UNIT_FLAG){			// �P�ʃt���O�ǂݍ���
			gpara->unit_flag = atoi(str);
			fprintf(stderr,"%d\n",gpara->unit_flag);	// debug
		}
		else if(i == MODEL_SPACE_SIZE){		// ���f����Ԃ̑傫���ǂݍ���
			gpara->space_size = atof(str);
			fprintf(stderr,"%lf\n",gpara->space_size);	// debug
		}
		p++;
		p_ = p;
	}

//	free(str);		// �������[�J��
	delete[]	str;

	return KOD_TRUE;
}

// Function: GetStartSection
// �X�^�[�g���ǂݍ���
//
// Parameter: 
// *fp - �ǂݍ���IGES�t�@�C���ւ̃|�C���^
// sline - �X�^�[�g���̃��C���� 
//
// Return:
// KOD_TRUE:����	KOD_ERR:���s
int IGES_PARSER::GetStartSection(FILE *fp,int sline)
{
	int i;

	for(i=0;i<sline;i++){
		if(fgets(buf,COLUMN_MAX_,fp) == NULL){
			GuiIF.SetMessage("START SECTION KOD_ERROR:fail to read this file");
			exit(KOD_ERR);
		}

		// �X�^�[�g���̏�񂪕K�v�ȏꍇ�͈ȉ��ɃR�[�h��ǉ�����
	}

	return KOD_TRUE;
}

// Function: GetTerminateSection
// �^�[�~�l�[�g���ǂݍ���
//
// Parameter: 
// *fp - �ǂݍ���IGES�t�@�C���ւ̃|�C���^
//
// Return:
// KOD_TRUE
int IGES_PARSER::GetTerminateSection(FILE *fp)
{
	return KOD_TRUE;
}

// �e�Z�N�V�����̃��C�����𒲂ׂ�
void IGES_PARSER::GetSectionLine(FILE *fp,int line[])
{
	line[0] = line[1] = line[2] = line[3] = line[4] = 0;	// ������

	while(fgets(buf,COLUMN_MAX_,fp)){
		if(buf[COL_CHAR-1] == 'S'){
			line[SECTION_START]++;
		}
		else if(buf[COL_CHAR-1] == 'G'){
			line[SECTION_GLOBAL]++;
		}
		else if(buf[COL_CHAR-1] == 'D'){
			line[SECTION_DIRECTORY]++;
		}
		else if(buf[COL_CHAR-1] == 'P'){
			line[SECTION_PARAMETER]++;
		}
		else if(buf[COL_CHAR-1] == 'T'){
			line[SECTION_TERMINATE]++;
		}
		else{							// �ǂ̃Z�N�V�����ɂ������Ȃ����������o
			GuiIF.SetMessage("KOD_ERROR: IGES FORMAT");
			exit(KOD_ERR);
		}
	}
	fseek(fp,0L,SEEK_SET);				// �t�@�C���擪�ɖ߂�

}

// Funciton: CatchStringI
// �J���}�܂ł̐��l��ǂݍ���ŕԂ�(int)
//
// Parameters:
// **p - ������ւ̃|�C���^
//
// Return:
// �J���}�܂ł̐��l 
int IGES_PARSER::CatchStringI(char **p)
{
	int a;

	if((*p = strchr(*p,',')) == NULL){
		GuiIF.SetMessage("KOD_ERROR:No governed by and construed for JAMA-IS");
		exit(KOD_ERR);
	}

	(*p)++;
	sscanf(*p,"%d[^,],",&a);

	return a;
}

// Funciton: CatchStringD
// �J���}�܂ł̐��l��ǂݍ���ŕԂ�(double)
//
// Parameters:
// **p - ������ւ̃|�C���^
//
// Return:
// �J���}�܂ł̐��l 
double IGES_PARSER::CatchStringD(char **p)
{
	double a;

	if((*p = strchr(*p,',')) == NULL){
		GuiIF.SetMessage("KOD_ERROR:No governed by and construed for JAMA-IS");
		exit(KOD_ERR);
	}

	(*p)++;
	sscanf(*p,"%lf[^,],",&a);

	return a;
}

// Funciton: GetDEPointer
// DE���ւ̃|�C���^���������ۂ̍\���̂ւ̃|�C���^��Ԃ�
//
// Parameters:
// TypeNum - �G���e�B�e�B�̃^�C�v�̐� 
// body - BODY�N���X�ւ̃C���X�^���X
//
// Return:
// DE���ւ̃|�C���^���������ۂ̍\���̂ւ̃|�C���^��void�^�ŕԂ�
void *IGES_PARSER::GetDEPointer(int TypeNum,BODY body)
{
	int i,j;

	for(i=0;i<ALL_ENTITY_TYPE_NUM;i++){
		for(j=0;j<TypeCount[i];j++){
			if(i==_CIRCLE_ARC && body.CirA[j].pD == TypeNum){
				return &body.CirA[j];
			}
			else if(i==_COMPOSITE_CURVE && body.CompC[j].pD == TypeNum){
				return &body.CompC[j];
			}
			else if(i==_CONIC_ARC && body.ConA[j].pD == TypeNum){
				return &body.ConA[j];
			}
			else if(i==_LINE && body.Line[j].pD == TypeNum){
				return &body.Line[j];
			}
			else if(i==_TRANSFORMATION_MATRIX && body.TMat[j].pD == TypeNum){
				return &body.TMat[j];
			}
			else if(i==_NURBSC && body.NurbsC[j].pD == TypeNum){
				return &body.NurbsC[j];
			}
			else if(i==_NURBSS && body.NurbsS[j].pD == TypeNum){
				return &body.NurbsS[j];
			}
			else if(i==_CURVE_ON_PARAMETRIC_SURFACE && body.ConpS[j].pD == TypeNum){
				return &body.ConpS[j];
			}
			else if(i==_TRIMMED_SURFACE && body.TrmS[j].pD == TypeNum){
				return &body.TrmS[j];
			}
		}
	}

	return NULL;
}

// Funciton: SearchEntType
// DE���ւ̃|�C���^�̒l����G���e�B�e�B�̃^�C�v�𒲂ׂĕԂ�
//
// Parameters:
// *dpara - �f�B���N�g�����̃p�����[�^�\���̂ւ̃|�C���^
// pdnum - DE���̃V�[�P���X�i���o�[
// dline - �f�B���N�g�����̃��C����  
//
// Return: 
// �G���e�B�e�B�^�C�v(DE���̃V�[�P���X�i���o�[�ƈ�v�����������ꍇ��KOD_ERR)
int IGES_PARSER::SearchEntType(DirectoryParam *dpara,int pdnum,int dline)
{
	int i;

	for(i=0;i<dline;i++){
		if(dpara[i].seq_num == pdnum){
			return dpara[i].entity_type;
		}
	}

	return KOD_ERR;
}

// Funciton: SearchMaxCoord
// �S�ẴG���e�B�e�B�ɂ�������W�l�̍ő�l�𒲂ׂ�
//
// Parameters:
// *body - BODY�\���̂ւ̃|�C���^ 
// TypeNum[] - �G���e�B�e�B�^�C�v�̐�
//
// Return: 
// KOD_TRUE:����	KOD_ERR:���s
int IGES_PARSER::SearchMaxCoord(BODY *body,int TypeNum[])
{
	int i,j;
	int temp=0;
	int bufnum=0;
	double *CoordBuf;
	
	// #100(�~�A�~��)�A#110(����)�A#126(NURBS�Ȑ�)�̃R���g���[���|�C���g�̍��W�l�̌��𓾂�
	for(i=0;i<TypeNum[_NURBSC];i++){
		bufnum += 3*body->NurbsC[i].K;
	}
	
	// �������m��
//	if((CoordBuf = (double*)malloc(sizeof(double)*bufnum)) == NULL){
	CoordBuf = new double[bufnum];
	if ( !CoordBuf ) {
		GuiIF.SetMessage("SEARCH MAXCOORD KOD_ERROR:fail to allocate memory");
		return KOD_ERR;
	}

	// #100(�~�A�~��)�A#110(����)�A#126(NURBS�Ȑ�)�̃R���g���[���|�C���g�̍��W�l�𓾂�
	for(i=0;i<TypeNum[_NURBSC];i++){
		for(j=0;j<body->NurbsC[i].K;j++){
			CoordBuf[temp*3] = fabs(body->NurbsC[i].cp[j].x);	// �R���g���[���|�C���gX
			CoordBuf[temp*3+1] = fabs(body->NurbsC[i].cp[j].y);	// �R���g���[���|�C���gY
			CoordBuf[temp*3+2] = fabs(body->NurbsC[i].cp[j].z);	// �R���g���[���|�C���gZ
			temp++;
			// Add by K.Magara
			// NCVC�p�̃f�[�^�ݒ�
			body->minmaxCoord[0].SetMinCoord(body->NurbsC[i].cp[j]);
			body->minmaxCoord[1].SetMaxCoord(body->NurbsC[i].cp[j]);
		}
	}
	
	qsort(CoordBuf,bufnum,sizeof(double),QCmp);	// �S�Ă̍��W�l���N�C�b�N�\�[�g�ɂ��~���Ƀ\�[�g
	body->MaxCoord = CoordBuf[0];				// �ł��傫�����W�l�𓾂�

	// ���������
//	free(CoordBuf);
	delete[]	CoordBuf;
	
	return KOD_TRUE;
}

// Funciton: IGES_PARSER
// �R���X�g���N�^
IGES_PARSER::IGES_PARSER()
{
		entity[0] = CIRCLE_ARC;							// �~/�~��
		entity[1] = COMPOSITE_CURVE;					// �����Ȑ�
		entity[2] = CONIC_ARC;							// �~���Ȑ�
		entity[3] = COPIOUS_DATA;						// �L�ӓ_��
		entity[4] = PLANE;								// ����
		entity[5] = LINE;								// ����
		entity[6] = PARAMETRIC_SPLINE_CURVE;			// �p�����g���b�N�X�v���C���Ȑ�
		entity[7] = PARAMETRIC_SPLINE_SURFACE;			// �p�����g���b�N�X�v���C���Ȗ�
		entity[8] = POINT;								// �_
		entity[9] = TRANSFORMATION_MATRIX;				// �ϊ��s��
		entity[10] = NURBS_CURVE;						// �L��B�X�v���C���Ȑ�
		entity[11] = NURBS_SURFACE;						// �L��B�X�v���C���Ȗ�
		entity[12] = CURVE_ON_PARAMETRIC_SURFACE; 		// �ʏ��
		entity[13] = TRIMMED_SURFACE;					// �g������
		entity[14] = SUBFIGURE_DEFINITION;				// �q�}�̒�`
		entity[15] = ASSOCIATIVITY_INSTANCE;			// �O���[�v
		entity[16] = DRAWING;							// �}��
		entity[17] = PROPERTY;							// �}�ʃT�C�Y
		entity[18] = SINGULAR_SUBFIGURE_INSTANCE;		// �q�}�̎Q��
		entity[19] = VIEW;								// ���ۖ�
}

// Funciton: InitDisplayStat
// �e�G���e�B�e�B�̕\��������ݒ�
//
// Parameters:
// *Dstat - �G���e�B�e�B�̕\������ 
void IGES_PARSER::InitDisplayStat(DispStat *Dstat)
{
	// ���F��ݒ�
	Dstat->Color[0] = 1.0;
	Dstat->Color[1] = 1.0;
	Dstat->Color[2] = 1.0;
	Dstat->Color[3] = 0.5;

	// �\��������ǉ�����ꍇ�͈ȉ��ɒǉ��̃R�[�h���L�q
}

// Funciton: TransformNurbsC
// Type124(�ϊ��s��)��p����NURBS�Ȑ������W�ϊ�����
//
// Parameters:
// NurbsCount - NURBS�Ȑ��̐� 
// TMp - �ϊ��s��̐� 
// body - BODY�\���� 
//
// Return:
// KOD_TRUE
int IGES_PARSER::TransformNurbsC(int NurbsCount,int TMp,BODY body)	
{
	int i;

	for(i=0;i<body.NurbsC[NurbsCount].K;i++){
		body.NurbsC[NurbsCount].cp[i] = MulFrameCoord(body.TMat[TMp].R,body.TMat[TMp].T,body.NurbsC[NurbsCount].cp[i]);	
	}
	
	return KOD_TRUE;
}
