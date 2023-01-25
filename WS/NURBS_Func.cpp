#include "NURBS_Func.h"


// 1��Nurbs�Ȑ��𐶐�����
int NURBS_Func::GenNurbsC(NURBSC *Nurbs,int K,int M,int N,double T[],double W[],Coord cp[],double V[],int prop[],int euflag)
{
	int i;

	Nurbs->K = K;
	Nurbs->M = M;
	Nurbs->N = N;
	Nurbs->V[0] = 0;
	Nurbs->V[1] = 1;
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
	InitCurveColor(Nurbs->Dstat.Color);

	return KOD_TRUE;
}

// 1��Nurbs�Ȑ��𐶐�����(NURBS�Ȑ��̃R�s�[)(�I�[�o�[���[�h)
// �����@*Nurbs:�V���ɐ�������NURBS�Ȑ��@nurb:�����
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

// GenNurbsC()�ɂ���Đ������ꂽNURBS�Ȑ����폜����
void NURBS_Func::DelNurbsC(NURBSC *Nurbs)
{
	
	BODY_Handler hbody;
	hbody.Free_NurbsC(Nurbs);
}

// 1��Nurbs�Ȗʂ𐶐�����(NURBS�Ȗʂ̃R�s�[)
// �����@Mu,Mv:�K��  Ku,Kv:�R���g���[���|�C���g�̐�  *S,*T:u,v�����m�b�g�x�N�g��  **W:�E�G�C�g
// **Cp:�R���g���[���|�C���g  U_s,U_e,V_s,V_e:u�����m�b�g�x�N�g���̊J�n�l,�I���l
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

	InitSurfaceColor(Nurbs->Dstat.Color);

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


// 1��Nurbs�Ȗʂ𐶐�����(NURBS�Ȗʂ̃R�s�[)
// �����@*Nurbs:�V���ɐ�������NURBS�Ȗʁ@nurb:�����
int NURBS_Func::GenNurbsS(NURBSS *Nurbs,NURBSS nurb)
{
	BODY_Handler hbody;
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

	// �������[�m��
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

// NurbsC�����_��ʂ�Axis����deg������]��������]�T�[�t�F�XNurbsS�𐶐�����
// Axis�͒P�ʃx�N�g���Ƃ���
int NURBS_Func::GenRotNurbsS(NURBSS *NurbsS,NURBSC NurbsC,Coord Axis,double deg)
{
	// 360�x�ȊO�͖�����
	if(deg != 360){
		SetMessage("Unsupported");
		return KOD_ERR;
	}

	Axis = NormalizeVec(Axis);		// ���K��

	// NurbsS�𐶐�
	double S[12] = {0,0,0,0.25,0.25,0.5,0.5,0.75,0.75,1,1,1};		// u�����m�b�g�x�N�g��
	double **W;			// �E�G�C�g
	Coord  **Cp;		// �R���g���[���|�C���g
	if((W = NewMatrix(9,NurbsC.K)) == NULL){
		SetMessage("NURBS_Func KOD_ERROR:fail to allocate memory");
		return KOD_ERR;
	}
	if((Cp = NewCoord2(9,NurbsC.K)) == NULL){
		SetMessage("NURBS_Func KOD_ERROR:fail to allocate memory");
		FreeMatrix(W,9);
		return KOD_ERR;
	}
	for(int i=0;i<9;i++){		// u����
		for(int j=0;j<NurbsC.K;j++){		// v����
			Coord Q_ = CalcRotVec(NurbsC.cp[j],Axis,(double)i*PI/4);		// ���X��NURBS�Ȑ���̃R���g���[���|�C���g��Axis�����45�x��]
			Coord P = CalcNormalLine(NurbsC.cp[j],SetCoord(0,0,0),Axis);	// Axis��̉�]���S�̍��W
			Coord PQ_ = SubCoord(Q_,P);										// PQ_�x�N�g���𐶐�
			if(i%2 == 0){													// i=0,2,4,6�̂Ƃ�
				W[i][j] = 1;												// �E�G�C�g��1
				Cp[i][j] = SetCoord(Q_);									// Q_�����̂܂܃R���g���[���|�C���g�ɂȂ�
			}
			else{															// i=1,3,5,7�̂Ƃ�
				W[i][j] = cos(PI/4);										// �E�G�C�g�v�Z
				Cp[i][j] = AddCoord(MulCoord(PQ_,1/cos(PI/4)),P);			// �R���g���[���|�C���g�v�Z
			}
		}
	}

	GenNurbsS(NurbsS,3,NurbsC.M,9,NurbsC.K,S,NurbsC.T,W,Cp,0,1,0,1);		// NURBS�Ȗʐ���

	FreeMatrix(W,2);
	FreeCoord2(Cp,2);

	return KOD_TRUE;
}

// 1��NURBS�Ȑ����炠�鎲�����ɂ��鋗�������X�C�[�v�������X�C�[�v�T�[�t�F�X�𐶐�����
int NURBS_Func::GenSweepNurbsS(NURBSS *NurbsS,NURBSC NurbsC,Coord Axis,double Len)
{
	Axis = NormalizeVec(Axis);		// ���K��

	// NurbsS�𐶐�
	double T[4] = {0,0,1,1};		// v�����m�b�g�x�N�g��
	double **W;			// �E�G�C�g
	Coord  **Cp;		// �R���g���[���|�C���g
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
			W[i][j] = NurbsC.W[i];	// �E�G�C�g�v�Z
			if(j==0)
				Cp[i][j] = SetCoord(NurbsC.cp[i]);		// �R���g���[���|�C���g�v�Z
			else
				Cp[i][j] = SetCoord(AddCoord(NurbsC.cp[i],MulCoord(Axis,Len)));		// �R���g���[���|�C���g�v�Z
		}
	}

	GenNurbsS(NurbsS,NurbsC.M,2,NurbsC.K,2,NurbsC.T,T,W,Cp,0,1,NurbsC.V[0],NurbsC.V[1]);	// NURBS�Ȗʐ���

	FreeMatrix(W,2);
	FreeCoord2(Cp,2);

	return KOD_TRUE;
}

// GenNurbsS()�ɂ���Đ������ꂽNURBS�Ȗʂ��폜����
void NURBS_Func::DelNurbsS(NURBSS *Nurbs)
{
	BODY_Handler hbody;
	hbody.Free_NurbsS(Nurbs);
}

// �g�����ʂ�L����NURBS�Ȗʂ𐶐�����
int NURBS_Func::GenTrimdNurbsS(TRIMD_NURBSS *TNurbs,TRIMD_NURBSS  tnurb)
{
	BODY_Handler hbody;
	NURBSS *nurbsS;
	NURBSC *nurbsC;
	CONPS *conps_o,*conps_i;
	COMPC *compc_o,*compc_i;
	int curve_num=0;

	nurbsS = (NURBSS *)malloc(sizeof(NURBSS));		// NURBS�Ȗʂ̃������[�m��
	conps_o = (CONPS *)malloc(sizeof(CONPS));		// �O���g�������\������ʏ���̃������[�m��
	compc_o = (COMPC *)malloc(sizeof(COMPC));		// �O���g�������\�����镡���Ȑ��̃������[�m��

	// �g�����ʂ��\������NURBS�Ȑ��̑������J�E���g
	for(int i=0;i<tnurb.n2;i++){
		for(int j=0;j<tnurb.pTI[i]->pB->CompC.N;j++){
			curve_num++;
		}
	}
	curve_num += tnurb.pTO->pB->CompC.N;

	nurbsC = (NURBSC *)malloc(sizeof(NURBSC)*curve_num);	// �g�����ʂ��\������NURBS�Ȑ��̐�����NURBS�Ȑ��̃������[���m��

	GenNurbsS(nurbsS,*tnurb.pts);							// �V����NURBS�Ȗʂ�1����
	TNurbs->pts = nurbsS;									// NURBS�Ȗʂ��g�����ʂɊ֘A�t����

	hbody.New_TrmS(TNurbs,tnurb.n2);						// �g�����ʂ̃������[�m��

	conps_i = (CONPS *)malloc(sizeof(CONPS)*tnurb.n2);		// �������\������ʏ���̃������[�m��
	compc_i = (COMPC *)malloc(sizeof(COMPC)*tnurb.n2);		// �������\�����镡���Ȑ��̃������[�m��

	// NURBS�Ȑ����g�����������\������NURBS�Ȑ��Ɋ֘A�t����
	// �O���g����
	TNurbs->pTO = conps_o;
	hbody.New_CompC(compc_o,tnurb.pTO->pB->CompC.N);
	for(int i=0;i<tnurb.pTO->pB->CompC.N;i++){
		GenNurbsC(&nurbsC[i],tnurb.pTO->pB->CompC.pDE[i]->NurbsC);
		compc_o->pDE[i] = (COMPELEM *)(&nurbsC[i]);
		compc_o->DEType[i] = tnurb.pTO->pB->CompC.DEType[i];
	}
	TNurbs->pTO->pB = (CURVE *)compc_o;
	TNurbs->pTO->BType = tnurb.pTO->BType;
	TNurbs->pTO->pB->CompC.DegeFlag = tnurb.pTO->pB->CompC.DegeFlag;
	TNurbs->pTO->pB->CompC.DegeNurbs = tnurb.pTO->pB->CompC.DegeNurbs;

	// �����g����
	curve_num = 0;
	for(int i=0;i<tnurb.n2;i++){
		TNurbs->pTI[i] = &(conps_i[i]);
		hbody.New_CompC(&compc_i[i],tnurb.pTI[i]->pB->CompC.N);
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

// GenTrimdNurbsS()�ɂ���Đ������ꂽ�g�����ʂ��폜����
int NURBS_Func::DelTrimdNurbsS(TRIMD_NURBSS *TNurbs)
{
	BODY_Handler hbody;
	int curve_num = 0;

	// �g�����ʂ��\������S�Ă�NURBS�Ȑ��̖{���𒲂ׂ�
	for(int i=0;i<TNurbs->n2;i++){
		for(int j=0;j<TNurbs->pTI[i]->pB->CompC.N;j++){
			curve_num++;
		}
	}
	curve_num += TNurbs->pTO->pB->CompC.N;

	hbody.Free_NurbsC_1DArray((NURBSC *)TNurbs->pTO->pB->CompC.pDE[0],curve_num);		// �g�����ʂ��\������S�Ă�NURBS�Ȑ��p�����[�^�̃������[���

	hbody.Free_NurbsS(TNurbs->pts);						// �g�����ʂ��\������NURBS�Ȗʃp�����[�^�̃������[���
	free(TNurbs->pts);								// �g�����ʂ��\������NURBS�Ȗʂ̃������[���

	hbody.Free_NurbsC(&TNurbs->pTO->pB->CompC.DegeNurbs);	// �g�����ʊO�����\�����镡���Ȑ����\������k�ޗpNURBS�Ȑ��̃������[���
	hbody.Free_CompC((COMPC *)TNurbs->pTO->pB);			// �g�����ʊO�����\�����镡���Ȑ����\������NURBS�Ȑ��̃������[���
	free(TNurbs->pTO->pB);							// �g�����ʊO�����\�����镡���Ȑ��̃������[���
	free(TNurbs->pTO);								// �g�����ʊO�����\������ʏ���̃������[���

	for(int i=0;i<TNurbs->n2;i++){
		hbody.Free_NurbsC(&TNurbs->pTI[i]->pB->CompC.DegeNurbs);	// �g�����ʓ������\�����镡���Ȑ����\������k�ޗpNURBS�Ȑ��̃������[���
		hbody.Free_CompC((COMPC *)TNurbs->pTI[i]->pB);	// �g�����ʓ������\�����镡���Ȑ����\������NURBS�Ȑ��̃������[���
		free(TNurbs->pTI[i]->pB);					// �g�����ʓ������\�����镡���Ȑ��̃������[���
	}
	hbody.Free_TrmS(TNurbs);								// �g�����ʃp�����[�^�̃������[���

	return KOD_TRUE;
}

// �w�肵���m�b�gt�ł�NURBS�Ȑ��̍��W�l�����߂�
Coord NURBS_Func::CalcNurbsCCoord(NURBSC *NurbsC,double t)
{
	Coord p;
	Coord bscpw;
	double bsw=0;
	double bs=0;
	int i;

	InitCoord(&bscpw);	// ������

	for(i=0;i<NurbsC->K;i++){
		bs = CalcBSbasis(t,NurbsC->T,NurbsC->N,i,NurbsC->M);		// B�X�v���C�����֐������߂�
		bsw += bs*NurbsC->W[i];							// ����
		bscpw = AddCoord(bscpw,MulCoord(NurbsC->cp[i],bs*NurbsC->W[i]));	// ���q
	}
	
	p = DivCoord(bscpw,bsw);	// ���W�l�����߂�

	return p;
}

// �w�肵���m�b�gt�Q�ł�NURBS�Ȑ��̍��W�l�����߂�
// ����   *NurbsS:NURBS�Ȗʂւ̃|�C���^   Ptnum:���߂�_�Q�̐�   *T:t�p�����[�^�Q���i�[�����z��
//        *Pt:�����W�l���i�[
void NURBS_Func::CalcNurbsCCoords(NURBSC *NurbsC,int Ptnum,double *T,Coord *Pt)
{
	for(int i=0;i<Ptnum;i++){
		Pt[i] = CalcNurbsCCoord(NurbsC,T[i]);
	}
}

// �w�肵���m�b�gu,v�ł�NURBS�Ȗʂ̍��W�l�����߂�
Coord NURBS_Func::CalcNurbsSCoord(NURBSS *NurbsS,double div_u,double div_v)
{
	int i,j;
	double bs_u,bs_v;		// u,v����B�X�v���C�����֐�
	double bsw=0;			// ����
	Coord bscpw;			// ���q

	InitCoord(&bscpw);		// ������

	for(i=0;i<NurbsS->K[0];i++){
		bs_u = CalcBSbasis(div_u,NurbsS->S,NurbsS->N[0],i,NurbsS->M[0]);			// u����B�X�v���C�����֐������߂�
		for(j=0;j<NurbsS->K[1];j++){
			bs_v = CalcBSbasis(div_v,NurbsS->T,NurbsS->N[1],j,NurbsS->M[1]);		// v����B�X�v���C�����֐������߂�
			bsw += bs_u*bs_v*NurbsS->W[i][j];
			bscpw = AddCoord(bscpw,MulCoord(NurbsS->cp[i][j],bs_u*bs_v*NurbsS->W[i][j]));
		}
	}
	//if(fabs(bsw) < APPROX_ZERO)
		//return(SetCoord(0,0,0));

	return(DivCoord(bscpw,bsw));
}

// �w�肵���m�b�gu,v�Q�ł�NURBS�Ȗʂ̍��W�l�Q�����߂�
// ����   *NurbsS:NURBS�Ȗʂւ̃|�C���^   Ptnum:���߂�_�Q�̐�   *UV:u,v�p�����[�^�Q���i�[����Coord�^�z��(UV[].x��u�����AUV[].����V�����̃p�����[�^���i�[���Ă�������)
//        *Pt:�����W�l���i�[
void NURBS_Func::CalcNurbsSCoords(NURBSS *NurbsS,int Ptnum,Coord *UV,Coord *Pt)
{
	for(int i=0;i<Ptnum;i++){
		Pt[i] = CalcNurbsSCoord(NurbsS,UV[i].x,UV[i].y);
	}
}

// B�X�v���C�����֐����v�Z���A�v�Z���ʂ�Ԃ�
// t:�m�b�g�@knot[]:�m�b�g�x�N�g��  N:�m�b�g�x�N�g���̐�  I:Bspl���֐����Y����1��(0�`)  M:�K��(Bspl���֐����Y����2��)  
// N_I,M(t)
double NURBS_Func::CalcBSbasis(double t, double knot[],int N,int I,int M)
{
	// �K��(order)��1�̎�
	if(M == 1){
		// ���ڒ��̃m�b�g�̒l���m�b�g�x�N�g���̏I�[�l�Ɠ����ꍇ�A���֐���1����肤��͈͂�knot[I+1]���܂ނ悤�ɂ���
		// �������Ȃ��ƁA���̂Ƃ������S�Ă̊��֐��l��0�ɂȂ��Ă��܂��B
		if(t==knot[N-1]){
			if(knot[I] <= t && t <= knot[I+1])	return 1.0;
			else		return 0.0;
		}
		else{
			if(knot[I] <= t && t < knot[I+1])	return 1.0;
			else	return 0.0;
		}
	}

	// ����ȊO�̎�
	else{
		double n1=0.0;
		double n2=0.0;
		double denom;

		denom = knot[I+M-1] - knot[I];	// ����
		if(denom > 0.0){
			n1 = (t-knot[I])/denom * CalcBSbasis(t,knot,N,I,M-1);		// 1����
		}

		denom = knot[I+M] - knot[I+1];
		if(denom > 0.0){
			n2 = (knot[I+M]-t)/denom * CalcBSbasis(t,knot,N,I+1,M-1);	// 2����
		}

		return(n1+n2);
	}
}


// B�X�v���C�����֐���1�K�����W�������߂�
// t:�m�b�g�@knot[]:�m�b�g�x�N�g��  N:�m�b�g�x�N�g���̐�  I:���ڒ��̃R���g���[���|�C���g  M:�K��
double NURBS_Func::CalcDiffBSbasis(double t,double knot[],int N,int I,int M)
{
	double n1 = knot[I+M-1]-knot[I];
	double n2 = knot[I+M]-knot[I+1];

	if(n1 != 0.0) n1 = (M-1)/n1*CalcBSbasis(t,knot,N,I,M-1);
	
	if(n2 != 0.0) n2 = (M-1)/n2*CalcBSbasis(t,knot,N,I+1,M-1);
	
	return(n1-n2);
}

// B�X�v���C�����֐���N�K�����W�������߂�
// t:�m�b�g�@knot[]:�m�b�g�x�N�g��  N:�m�b�g�x�N�g���̐�  I:���ڒ��̃R���g���[���|�C���g  M:�K��  Dn:�����K��
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


// NURBS�Ȑ���1�K�����W�������߂�
Coord NURBS_Func::CalcDiffNurbsC(NURBSC *NurbsC,double t)
{
	Coord Ft,diff_Ft;		// NURBS�Ȑ��̕��q
	double Gt,diff_Gt;		// NURBS�Ȑ��̕���
	double bs,diff_bs;		// B�X�v���C�����֐�
	Coord p;
	int i;

	InitCoord(&Ft);
	InitCoord(&diff_Ft);
	Gt = 0;
	diff_Gt = 0;

	// �e�W���Z�o
	for(i=0;i<NurbsC->K;i++){
		bs = CalcBSbasis(t,NurbsC->T,NurbsC->N,i,NurbsC->M);
		diff_bs = CalcDiffBSbasis(t,NurbsC->T,NurbsC->N,i,NurbsC->M);

		Ft = AddCoord(Ft,MulCoord(NurbsC->cp[i],bs*NurbsC->W[i]));
		diff_Ft = AddCoord(diff_Ft,MulCoord(NurbsC->cp[i],diff_bs*NurbsC->W[i]));

		Gt += bs*NurbsC->W[i];
		diff_Gt += diff_bs*NurbsC->W[i];
	}
	if(fabs(Gt) < APPROX_ZERO)	return(SetCoord(0,0,0));

	// 1�K���������߂�
	p = SubCoord(DivCoord(diff_Ft,Gt),DivCoord(MulCoord(Ft,diff_Gt),Gt*Gt));

	return p;
}

// NURBS�Ȑ���2�K�����W�������߂�
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

// NURBS�Ȗʂ�u������1�K�����W���𓾂�
Coord NURBS_Func::CalcDiffuNurbsS(NURBSS *NurbsS,double div_u,double div_v)
{
	int i,j;
	Coord Ft,diff_Ft;
	double Gt,diff_Gt;
	double bs_u,bs_v;		// u,v����B�X�v���C�����֐�
	double diff_bs_u;
	Coord p;

	InitCoord(&Ft);
	InitCoord(&diff_Ft);
	Gt = 0;
	diff_Gt = 0;

	for(i=0;i<NurbsS->K[0];i++){
		bs_u = CalcBSbasis(div_u,NurbsS->S,NurbsS->N[0],i,NurbsS->M[0]);				// u����B�X�v���C�����֐������߂�
		diff_bs_u = CalcDiffBSbasis(div_u,NurbsS->S,NurbsS->N[0],i,NurbsS->M[0]);	// u����B�X�v���C�����֐���1�K���������߂�
		for(j=0;j<NurbsS->K[1];j++){
			bs_v = CalcBSbasis(div_v,NurbsS->T,NurbsS->N[1],j,NurbsS->M[1]);			// v����B�X�v���C�����֐������߂�
			Ft = AddCoord(Ft,MulCoord(NurbsS->cp[i][j],bs_u*bs_v*NurbsS->W[i][j]));
			diff_Ft = AddCoord(diff_Ft,MulCoord(NurbsS->cp[i][j],diff_bs_u*bs_v*NurbsS->W[i][j]));
			Gt += bs_u*bs_v*NurbsS->W[i][j];
			diff_Gt += diff_bs_u*bs_v*NurbsS->W[i][j];
		}
	}
	if(fabs(Gt) < APPROX_ZERO)	return(SetCoord(0,0,0));

	// 1�K���������߂�
	p = SubCoord(DivCoord(diff_Ft,Gt),DivCoord(MulCoord(Ft,diff_Gt),Gt*Gt));

	return p;
}

// NURBS�Ȗʂ�v������1�K�����W���𓾂�
Coord NURBS_Func::CalcDiffvNurbsS(NURBSS *NurbsS,double div_u,double div_v)
{
	int i,j;
	Coord Ft,diff_Ft;
	double Gt,diff_Gt;
	double bs_u,bs_v;		// u,v����B�X�v���C�����֐�
	double diff_bs_v;
	Coord p;

	InitCoord(&Ft);
	InitCoord(&diff_Ft);
	Gt = 0;
	diff_Gt = 0;

	for(i=0;i<NurbsS->K[0];i++){
		bs_u = CalcBSbasis(div_u,NurbsS->S,NurbsS->N[0],i,NurbsS->M[0]);				// u����B�X�v���C�����֐������߂�
		for(j=0;j<NurbsS->K[1];j++){
			bs_v = CalcBSbasis(div_v,NurbsS->T,NurbsS->N[1],j,NurbsS->M[1]);				// v����B�X�v���C�����֐������߂�
			diff_bs_v = CalcDiffBSbasis(div_v,NurbsS->T,NurbsS->N[1],j,NurbsS->M[1]);	// v����B�X�v���C�����֐���1�K���������߂�
			Ft = AddCoord(Ft,MulCoord(NurbsS->cp[i][j],bs_u*bs_v*NurbsS->W[i][j]));
			diff_Ft = AddCoord(diff_Ft,MulCoord(NurbsS->cp[i][j],bs_u*diff_bs_v*NurbsS->W[i][j]));
			Gt += bs_u*bs_v*NurbsS->W[i][j];
			diff_Gt += bs_u*diff_bs_v*NurbsS->W[i][j];
		}
	}
	if(fabs(Gt) < APPROX_ZERO)	return(SetCoord(0,0,0));

	// 1�K���������߂�
	p = SubCoord(DivCoord(diff_Ft,Gt),DivCoord(MulCoord(Ft,diff_Gt),Gt*Gt));

	return p;
}

// NURBS�Ȗʂ̊e������C�ӊK���������Ƃ��̔����W�������߂�
// ����   *S:NURBS�Ȗʂւ̃|�C���^   k:u�����̔����K��    l:v�����̔����K��   u,v:u����v�������ꂼ��̃p�����[�^
Coord NURBS_Func::CalcDiffNurbsS(NURBSS *S,int k,int l,double u,double v)
{
	double w = CalcDiffNurbsSDenom(S,0,0,u,v);
	Coord  A = CalcDiffNurbsSNumer(S,k,l,u,v);
	Coord  B = SetCoord(0,0,0);
	Coord  C = SetCoord(0,0,0);
	Coord  D = SetCoord(0,0,0);

	if(!k && !l)
		return(CalcNurbsSCoord(S,u,v));
		
	for(int i=1;i<=k;i++)
		B = AddCoord(B,MulCoord(CalcDiffNurbsS(S,k-i,l,u,v),nCr(k,i)*CalcDiffNurbsSDenom(S,i,0,u,v)));
	for(int j=1;j<=l;j++)
		C = AddCoord(C,MulCoord(CalcDiffNurbsS(S,k,l-j,u,v),nCr(l,j)*CalcDiffNurbsSDenom(S,0,j,u,v)));
	for(int i=1;i<=k;i++){
		for(int j=1;j<=l;j++){
			D = AddCoord(D,MulCoord(CalcDiffNurbsS(S,k-i,l-j,u,v),nCr(l,j)*CalcDiffNurbsSDenom(S,i,j,u,v)));
		}
		D = MulCoord(D,nCr(k,i));
	}
	return(DivCoord(SubCoord(A,AddCoord(B,AddCoord(C,D))),w));

}

// NURBS�Ȗʕ���̊e������C�ӊK���������Ƃ��̔����W�������߂�
// ����   *S:NURBS�Ȗʂւ̃|�C���^   k:u�����̔����K��    l:v�����̔����K��   u,v:u����v�������ꂼ��̃p�����[�^
double NURBS_Func::CalcDiffNurbsSDenom(NURBSS *S,int k,int l,double u,double v)
{
	double w=0;
	for(int i=0;i<S->K[0];i++){
		double Nk = CalcDiffBSbasisN(u,S->S,S->N[0],i,S->M[0],k);		// u������k�K����
		for(int j=0;j<S->K[1];j++){
			double Nl = CalcDiffBSbasisN(v,S->T,S->N[1],j,S->M[1],l);	// v������l�K����
			w += Nk*Nl*S->W[i][j];
		}
	}
	return w;
}

// NURBS�Ȗʕ��q�̊e������C�ӊK���������Ƃ��̔����W�������߂�
// ����   *S:NURBS�Ȗʂւ̃|�C���^   k:u�����̔����K��    l:v�����̔����K��   u,v:u����v�������ꂼ��̃p�����[�^
Coord NURBS_Func::CalcDiffNurbsSNumer(NURBSS *S,int k,int l,double u,double v)
{
	Coord A=SetCoord(0,0,0);
	for(int i=0;i<S->K[0];i++){
		double Nk = CalcDiffBSbasisN(u,S->S,S->N[0],i,S->M[0],k);		// u������k�K����
		for(int j=0;j<S->K[1];j++){
			double Nl = CalcDiffBSbasisN(v,S->T,S->N[1],j,S->M[1],l);	// v������l�K����
			A = AddCoord(A,MulCoord(S->cp[i][j],Nk*Nl*S->W[i][j]));
		}
	}
	return A;
}


// NURBS�Ȗʏ��(u,v)�ɂ�����@���x�N�g�������Ƃ߂�
Coord NURBS_Func::CalcNormVecOnNurbsS(NURBSS *nurb,double u,double v)
{
	Coord a = CalcDiffuNurbsS(nurb,u,v);
	Coord b = CalcDiffvNurbsS(nurb,u,v);

	return(NormalizeVec(CalcOuterProduct(a,b)));
}

// NURBS�Ȗ�S(u,v)��NURBS�Ȗ�R(w,t)�̊������o����(�g������)
// ����  *nurbS:NURBS�Ȗ�S(u,v) *nurbR:NURBS�Ȗ�R(w,t) divnum:�p�����[�^������
// �Ԓl  ���̗L��(���L:TRUE ����:FALSE)
int NURBS_Func::DetectInterfereNurbsS(NURBSS *nurbR,NURBSS *nurbS,int divnum)
{
	// �e�Ȗʂ��w��̕�������uv�������A�����̓_�ɂ�����⏕���ʂ𐶐����Č����̔C�ӂ�1�_�Ɏ���������
	for(int w=0;w<divnum;w++){
		for(int t=0;t<divnum;t++){
			for(int u=0;u<divnum;u++){
				for(int v=0;v<divnum;v++){
					// �e�Ȗʂɕ����_�𐶐�����
					double w0 = nurbR->U[0] + (nurbR->U[1] - nurbR->U[0])*(double)w/(double)divnum;
					double t0 = nurbR->V[0] + (nurbR->V[1] - nurbR->V[0])*(double)t/(double)divnum;
					double u0 = nurbS->U[0] + (nurbS->U[1] - nurbS->U[0])*(double)u/(double)divnum;
					double v0 = nurbS->V[0] + (nurbS->V[1] - nurbS->V[0])*(double)v/(double)divnum;
					for(int i=0;i<10;i++){
						// �e��p�����[�^���Z�o����
						Coord p0 = CalcNurbsSCoord(nurbR,w0,t0);					// R(w0,t0)�ƂȂ�_(�����_)�̍��W
						Coord q0 = CalcNurbsSCoord(nurbS,u0,v0);					// S(u0,v0)�ƂȂ�_(�����_)�̍��W
						Coord rw = CalcDiffuNurbsS(nurbR,w0,t0);					// �_R(w0,t0)��u�Δ���(��{�x�N�g��)
						Coord rt = CalcDiffvNurbsS(nurbR,w0,t0);					// �_R(w0,t0)��v�Δ���(��{�x�N�g��)
						Coord np = DivCoord(CalcOuterProduct(rw,rt),CalcEuclid(CalcOuterProduct(rw,rt)));	// �_R(w0,t0)�̒P�ʖ@���x�N�g��
						Coord su = CalcDiffuNurbsS(nurbS,u0,v0);					// �_S(u0,v0)��u�Δ���(��{�x�N�g��)
						Coord sv = CalcDiffvNurbsS(nurbS,u0,v0);					// �_S(u0,v0)��v�Δ���(��{�x�N�g��)
						Coord nq = DivCoord(CalcOuterProduct(su,sv),CalcEuclid(CalcOuterProduct(su,sv)));	// �_S(u0,v0)�̒P�ʖ@���x�N�g��
						Coord nn = DivCoord(CalcOuterProduct(np,nq),CalcEuclid(CalcOuterProduct(np,nq)));	// ����Fp�ƕ���Fq�ɒ������镽��Fn�̒P�ʖ@���x�N�g��
						double dp = CalcInnerProduct(p0,np);						// ���_���畽��Fp�܂ł̋���
						double dq = CalcInnerProduct(q0,nq);						// ���_���畽��Fq�܂ł̋���
						double dn = CalcInnerProduct(p0,nn);						// ���_���畽��Fn�܂ł̋���
						Coord cross_nqn = CalcOuterProduct(nq,nn);					// �P�ʖ@���x�N�g��nq,nn�̃x�N�g����
						Coord cross_nnp = CalcOuterProduct(nn,np);					// �P�ʖ@���x�N�g��nn,np�̃x�N�g����
						Coord cross_npq = CalcOuterProduct(np,nq);					// �P�ʖ@���x�N�g��np,nq�̃x�N�g����
						Coord nume_p_sub =  AddCoord(MulCoord(cross_nqn,dp),MulCoord(cross_nnp,dq));	// 3����Fp,Fq,Fn�̌�_p�̕��q�̍ŏ���2�����v�Z
						Coord nume_p = AddCoord(nume_p_sub,MulCoord(cross_npq,dn));			// p�̕��q���Z�o
						double denom_p = CalcScalarTriProduct(np,nq,nn);				// p�̕�����Z�o
						Coord p = DivCoord(nume_p,denom_p);						// p���Z�o
						Coord deltap0 = SubCoord(p,p0);							// �_p�Ɠ_p0�̍��x�N�g�����Z�o
						Coord deltaq0 = SubCoord(p,q0);							// �_p�Ɠ_q0�̍��x�N�g�����Z�o
						Coord rw_sub = CalcOuterProduct(rw,np);						// ��{�x�N�g��rw�Ɩ@���x�N�g��np�ɒ�������x�N�g��
						Coord rt_sub = CalcOuterProduct(rt,np);						// ��{�x�N�g��rt�Ɩ@���x�N�g��np�ɒ�������x�N�g��
						Coord su_sub = CalcOuterProduct(su,nq);						// ��{�x�N�g��su�Ɩ@���x�N�g��nq�ɒ�������x�N�g��
						Coord sv_sub = CalcOuterProduct(sv,nq);						// ��{�x�N�g��sv�Ɩ@���x�N�g��nq�ɒ�������x�N�g��
						double dw = CalcInnerProduct(rt_sub,deltap0)/CalcInnerProduct(rt_sub,rw);	// �V�����_r(w0+dw,t0+dt)��^���邽�߂�dw���Z�o
						double dt = CalcInnerProduct(rw_sub,deltap0)/CalcInnerProduct(rw_sub,rt);	// �V�����_r(w0+dw,t0+dt)��^���邽�߂�dt���Z�o
						double du = CalcInnerProduct(sv_sub,deltaq0)/CalcInnerProduct(sv_sub,su);	// �V�����_r(w0+dw,t0+dt)��^���邽�߂�dw���Z�o
						double dv = CalcInnerProduct(su_sub,deltaq0)/CalcInnerProduct(su_sub,sv);	// �V�����_r(w0+dw,t0+dt)��^���邽�߂�dt���Z�o
						w0 += dw;									// �V�����_��w�p�����[�^�𓾂�
						t0 += dt;									// �V�����_��t�p�����[�^�𓾂�
						u0 += du;									// �V�����_��u�p�����[�^�𓾂�
						v0 += dv;									// �V�����_��v�p�����[�^�𓾂�
						
						// �Ȗʂ͈̔͊O�ɏo�Ă��܂����烋�[�v�𔲂���
						if(!CheckRange(nurbR->U[0],nurbR->U[1],w0,1) || !CheckRange(nurbR->V[0],nurbR->V[1],t0,1)){
							break;
						}
						if(!CheckRange(nurbS->U[0],nurbS->U[1],u0,1) || !CheckRange(nurbS->V[0],nurbS->V[1],v0,1)){
							break;
						}
						
						Coord deltapq = SubCoord(p0,q0);						// �_p0�Ɠ_q0�̍��x�N�g�����Z�o
						double deltapq_dis = CalcEuclid(deltapq);					// |p0-q0|�̋������Z�o
						
						if(isnan(deltapq_dis)){
							break;	
						}
						
						// �\�������������_�����݂��邽�ߊ��L
						if(deltapq_dis < CONVERG_GAP){
							SetMessage("Interference with the NURBS surface was detected");
							return TRUE;
						}
					}
				}
			}
		}
	}
	
	SetMessage("Interference with the NURBS surface was not detected");
	return FALSE;
}

// NURBS�Ȗ�S(u,v)��NURBS�Ȗ�R(w,t)�̊������o����(�g�����L)
// ����  *tNurbS:NURBS�Ȗ�S(u,v)(�g�����L) *tNurbR:NURBS�Ȗ�R(w,t)(�g�����L) divnum:�p�����[�^������
// �Ԓl  ���̗L��(���L:TRUE ����:FALSE)
int NURBS_Func::DetectInterfereTrmS(TRIMD_NURBSS *tNurbR,TRIMD_NURBSS *tNurbS,int divnum)
{
	int count=0;

	// �e�Ȗʂ��w��̕�������uv�������A�����̓_�ɂ�����⏕���ʂ𐶐����Č����̔C�ӂ�1�_�Ɏ���������
	for(int w=0;w<divnum;w++){
		for(int t=0;t<divnum;t++){
			for(int u=0;u<divnum;u++){
				for(int v=0;v<divnum;v++){
					// �e�Ȗʂɕ����_�𐶐�����
					double w0 = tNurbR->pts->U[0] + (tNurbR->pts->U[1] - tNurbR->pts->U[0])*(double)w/(double)divnum;
					double t0 = tNurbR->pts->V[0] + (tNurbR->pts->V[1] - tNurbR->pts->V[0])*(double)t/(double)divnum;
					double u0 = tNurbS->pts->U[0] + (tNurbS->pts->U[1] - tNurbS->pts->U[0])*(double)u/(double)divnum;
					double v0 = tNurbS->pts->V[0] + (tNurbS->pts->V[1] - tNurbS->pts->V[0])*(double)v/(double)divnum;
					for(int i=0;i<10;i++){
						// �e��p�����[�^���Z�o����
						Coord p0 = CalcNurbsSCoord(tNurbR->pts,w0,t0);					// R(w0,t0)�ƂȂ�_(�����_)�̍��W
						Coord q0 = CalcNurbsSCoord(tNurbS->pts,u0,v0);					// S(u0,v0)�ƂȂ�_(�����_)�̍��W
						Coord rw = CalcDiffuNurbsS(tNurbR->pts,w0,t0);					// �_R(w0,t0)��u�Δ���(��{�x�N�g��)
						Coord rt = CalcDiffvNurbsS(tNurbR->pts,w0,t0);					// �_R(w0,t0)��v�Δ���(��{�x�N�g��)
						Coord np = DivCoord(CalcOuterProduct(rw,rt),CalcEuclid(CalcOuterProduct(rw,rt)));	// �_R(w0,t0)�̒P�ʖ@���x�N�g��
						Coord su = CalcDiffuNurbsS(tNurbS->pts,u0,v0);					// �_S(u0,v0)��u�Δ���(��{�x�N�g��)
						Coord sv = CalcDiffvNurbsS(tNurbS->pts,u0,v0);					// �_S(u0,v0)��v�Δ���(��{�x�N�g��)
						Coord nq = DivCoord(CalcOuterProduct(su,sv),CalcEuclid(CalcOuterProduct(su,sv)));	// �_S(u0,v0)�̒P�ʖ@���x�N�g��
						Coord nn = DivCoord(CalcOuterProduct(np,nq),CalcEuclid(CalcOuterProduct(np,nq)));	// ����Fp�ƕ���Fq�ɒ������镽��Fn�̒P�ʖ@���x�N�g��
						double dp = CalcInnerProduct(p0,np);						// ���_���畽��Fp�܂ł̋���
						double dq = CalcInnerProduct(q0,nq);						// ���_���畽��Fq�܂ł̋���
						double dn = CalcInnerProduct(p0,nn);						// ���_���畽��Fn�܂ł̋���
						Coord cross_nqn = CalcOuterProduct(nq,nn);					// �P�ʖ@���x�N�g��nq,nn�̃x�N�g����
						Coord cross_nnp = CalcOuterProduct(nn,np);					// �P�ʖ@���x�N�g��nn,np�̃x�N�g����
						Coord cross_npq = CalcOuterProduct(np,nq);					// �P�ʖ@���x�N�g��np,nq�̃x�N�g����
						Coord nume_p_sub =  AddCoord(MulCoord(cross_nqn,dp),MulCoord(cross_nnp,dq));	// 3����Fp,Fq,Fn�̌�_p�̕��q�̍ŏ���2�����v�Z
						Coord nume_p = AddCoord(nume_p_sub,MulCoord(cross_npq,dn));			// p�̕��q���Z�o
						double denom_p = CalcScalarTriProduct(np,nq,nn);				// p�̕�����Z�o
						Coord p = DivCoord(nume_p,denom_p);						// p���Z�o
						Coord deltap0 = SubCoord(p,p0);							// �_p�Ɠ_p0�̍��x�N�g�����Z�o
						Coord deltaq0 = SubCoord(p,q0);							// �_p�Ɠ_q0�̍��x�N�g�����Z�o
						Coord rw_sub = CalcOuterProduct(rw,np);						// ��{�x�N�g��rw�Ɩ@���x�N�g��np�ɒ�������x�N�g��
						Coord rt_sub = CalcOuterProduct(rt,np);						// ��{�x�N�g��rt�Ɩ@���x�N�g��np�ɒ�������x�N�g��
						Coord su_sub = CalcOuterProduct(su,nq);						// ��{�x�N�g��su�Ɩ@���x�N�g��nq�ɒ�������x�N�g��
						Coord sv_sub = CalcOuterProduct(sv,nq);						// ��{�x�N�g��sv�Ɩ@���x�N�g��nq�ɒ�������x�N�g��
						double dw = CalcInnerProduct(rt_sub,deltap0)/CalcInnerProduct(rt_sub,rw);	// �V�����_r(w0+dw,t0+dt)��^���邽�߂�dw���Z�o
						double dt = CalcInnerProduct(rw_sub,deltap0)/CalcInnerProduct(rw_sub,rt);	// �V�����_r(w0+dw,t0+dt)��^���邽�߂�dt���Z�o
						double du = CalcInnerProduct(sv_sub,deltaq0)/CalcInnerProduct(sv_sub,su);	// �V�����_r(w0+dw,t0+dt)��^���邽�߂�dw���Z�o
						double dv = CalcInnerProduct(su_sub,deltaq0)/CalcInnerProduct(su_sub,sv);	// �V�����_r(w0+dw,t0+dt)��^���邽�߂�dt���Z�o
						w0 += dw;									// �V�����_��w�p�����[�^�𓾂�
						t0 += dt;									// �V�����_��t�p�����[�^�𓾂�
						u0 += du;									// �V�����_��u�p�����[�^�𓾂�
						v0 += dv;									// �V�����_��v�p�����[�^�𓾂�
						
						// �Ȗʂ͈̔͊O�ɏo�Ă��܂����烋�[�v�𔲂���
						if(!CheckRange(tNurbR->pts->U[0],tNurbR->pts->U[1],w0,1) || !CheckRange(tNurbR->pts->V[0],tNurbR->pts->V[1],t0,1)){
							break;
						}
						if(!CheckRange(tNurbS->pts->U[0],tNurbS->pts->U[1],u0,1) || !CheckRange(tNurbS->pts->V[0],tNurbS->pts->V[1],v0,1)){
							break;
						}
						
						Coord deltapq = SubCoord(p0,q0);						// �_p0�Ɠ_q0�̍��x�N�g�����Z�o
						double deltapq_dis = CalcEuclid(deltapq);					// |p0-q0|�̋������Z�o
						
						if(isnan(deltapq_dis)){
							break;	
						}
				
						// �\�������������_�����݂��邽�ߊ��L
						if(deltapq_dis < CONVERG_GAP){
							if(DetermPtOnTRMSurf(tNurbR,w0,t0) >= KOD_TRUE && DetermPtOnTRMSurf(tNurbS,u0,v0) >= KOD_TRUE){	// �g��������Ȃ����
								SetMessage("Interference with the trimmed NURBS surface was detected");
								return TRUE;
							}
						}
					}
				}
			}
		}
	}
	
	SetMessage("Interference with the trimmed NURBS surface was not detected");
	return FALSE;
}

// 3���ȉ���NURBS�Ȗʂƕ��ʂƂ̌�_�Q��㐔��@�ɂċ��߂�(v�p�����[�^����)
// ����  *nurb:NURBS�Ȗ�  pt:���ʏ�̈�_  nvec:���ʂ̖@���x�N�g��  v_divnum:v�p�����[�^������
//       *ans:�Z�o���ꂽ��_��u,v�p�����[�^�l�����ꂼ��ans.x,ans.y�Ɋi�[,  ans_size:ans�̔z��
// �Ԓl  ��_�̌�(��_�̐���ans_size�𒴂����FKOD_ERR)
int NURBS_Func::CalcIntersecPtsPlaneV3(NURBSS *nurb,Coord pt,Coord nvec,int v_divnum,Coord *ans,int ans_size)
{
	double v_const;			// �萔�ƒu�����Ƃ���v�p�����[�^
	double *N;				// B�X�v���C�����֐��̌v�Z�l���i�[
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

	// v�p�����[�^����ԓ��ŕ������A�ev�p�����[�^���NURBS�Ȑ�C(u)�ƕ���(pt,nvec)�Ƃ̌�_�����߂�
	for(int v=0;v<=v_divnum;v++){
		v_const = (nurb->V[1] - nurb->V[0])*(double)v/(double)v_divnum;		// �K����v�����p�����[�^��ݒ�
		for(int i=0;i<nurb->K[1];i++){
			N[i] = CalcBSbasis(v_const,nurb->T,nurb->N[0],i,nurb->M[1]);		// v_const����B�X�v���C�����֐������߂�
		}
		for(int i=0;i<nurb->K[0];i++){
			A[i] = 0;
			InitCoord(&B[i]);
			for(int j=0;j<nurb->K[1];j++){
				A[i] += N[j]*nurb->W[i][j];			// v_const���NURBS�Ȑ�C(u)�̕���̌W��
				B[i] = AddCoord(B[i],MulCoord(nurb->cp[i][j],N[j]*nurb->W[i][j]));		// v_const���NURBS�Ȑ�C(u)�̕��q�̌W��
			}
		}
		for(int i=0;i<nurb->K[0]-nurb->M[0]+1;i++){						// i�Ԗڂ̋Ȑ��ɑ΂���
			InitMatrix(coef,nurb->M[0],nurb->M[0]);
			InitVector(a,4);
			InitCoord(P,4);
			InitVector(Q,4);
			InitVector(t,3);
			if(nurb->M[0]-1 == 3){										// 3��
				GetBSplCoef3(nurb->M[0],nurb->K[0],i,nurb->S,coef);		// 3����B�X�v���C�����֐��̌W�������߂�
			}
			else if(nurb->M[0]-1 == 2){									// 2��
				GetBSplCoef2(nurb->M[0],nurb->K[0],i,nurb->S,coef);		// 2����B�X�v���C�����֐��̌W�������߂�
			}
			else if(nurb->M[0]-1 == 1){									// 1��
				GetBSplCoef1(nurb->M[0],nurb->K[0],i,nurb->S,coef);		// 1����B�X�v���C�����֐��̌W�������߂�
			}
			GetNurbsSCoef(nurb->M[0],coef,A,B,i,P,Q);					// �Œ肳�ꂽv�p�����[�^���NURBS�Ȑ�C(u)�̌W�������߂�
			GetIntersecEquation(nurb->M[0],P,Q,pt,nvec,a);				// �������𓱏o
			ansnum = CalcEquation(a,t,nurb->M[0]-1);					// ������������
			int hitnum = 0;						// �����ɓK��������̐����J�E���g����
			for(int j=0;j<ansnum;j++){			// 3�̉����ꂼ��ɑ΂���
				if(t[j] >= nurb->S[i+nurb->M[0]-1] && t[j] <= nurb->S[i+nurb->M[0]]){	// ���ڒ��̃m�b�g�x�N�g���͈͓̔��Ȃ�
					ans[allansnum+hitnum] = SetCoord(t[j],v_const,0);		// ���Ƃ��ēo�^
					hitnum++;
				}
			}
			allansnum += hitnum;				// �����K�����̐��������𐔂��J�E���g�A�b�v
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

// 3���ȉ���NURBS�Ȗʂƕ��ʂƂ̌�_�Q��㐔��@�ɂċ��߂�(u�p�����[�^����)
// ����  *nurb:NURBS�Ȗ�  pt:���ʏ�̈�_  nvec:���ʂ̖@���x�N�g��  u_divnum:u�p�����[�^������
//       *ans:�Z�o���ꂽ��_��u,v�p�����[�^�l�������ꂼ��ans.x,ans.y�Ɋi�[,  ans_size:ans�̔z��
// �Ԓl  ��_�̌�(��_�̐���ans_size�𒴂����FKOD_ERR)
int NURBS_Func::CalcIntersecPtsPlaneU3(NURBSS *nurb,Coord pt,Coord nvec,int u_divnum,Coord *ans,int ans_size)
{
	double u_const;			// �萔�ƒu�����Ƃ���v�p�����[�^
	double *N;				// B�X�v���C�����֐��̌v�Z�l���i�[
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

	// u�p�����[�^����ԓ��ŕ������A�eu�p�����[�^���NURBS�Ȑ�C(v)�ƕ���(pt,nvec)�Ƃ̌�_�����߂�
	for(int u=0;u<=u_divnum;u++){
		u_const = (nurb->U[1] - nurb->U[0])*(double)u/(double)u_divnum;		// �K����u�����p�����[�^��ݒ�
		for(int i=0;i<nurb->K[0];i++){
			N[i] = CalcBSbasis(u_const,nurb->S,nurb->N[0],i,nurb->M[0]);		// u_const����B�X�v���C�����֐������߂�
		}
		for(int j=0;j<nurb->K[1];j++){
			A[j] = 0;
			InitCoord(&B[j]);
			for(int i=0;i<nurb->K[0];i++){
				A[j] += N[i]*nurb->W[i][j];			// u_const���NURBS�Ȑ�C(v)�̕���̌W��
				B[j] = AddCoord(B[j],MulCoord(nurb->cp[i][j],N[i]*nurb->W[i][j]));		// u_const���NURBS�Ȑ�C(v)�̕��q�̌W��
			}
		}
		for(int i=0;i<nurb->K[1]-nurb->M[1]+1;i++){						// i�Ԗڂ̋Ȑ��ɑ΂���
			if(nurb->M[1]-1 == 3){										// 3��
				GetBSplCoef3(nurb->M[1],nurb->K[1],i,nurb->T,coef);		// 3����B�X�v���C�����֐��̌W�������߂�
			}
			else if(nurb->M[1]-1 == 2){									// 2��
				GetBSplCoef2(nurb->M[1],nurb->K[1],i,nurb->T,coef);		// 2����B�X�v���C�����֐��̌W�������߂�
			}
			else if(nurb->M[1]-1 == 1){									// 1��
				GetBSplCoef1(nurb->M[1],nurb->K[1],i,nurb->T,coef);		// 1����B�X�v���C�����֐��̌W�������߂�
			}
			GetNurbsSCoef(nurb->M[1],coef,A,B,i,P,Q);					// �Œ肳�ꂽu�p�����[�^���NURBS�Ȑ�C(v)�̌W�������߂�
			GetIntersecEquation(nurb->M[1],P,Q,pt,nvec,a);				// �������𓱏o
			ansnum = CalcEquation(a,t,nurb->M[1]-1);					// ������������

			int hitnum = 0;						// �����ɓK��������̐����J�E���g����
			for(int j=0;j<ansnum;j++){			// 3�̉����ꂼ��ɑ΂���
				if(t[j] >= nurb->T[i+nurb->M[1]-1] && t[j] <= nurb->T[i+nurb->M[1]]){	// ���ڒ��̃m�b�g�x�N�g���͈͓̔��Ȃ�
					ans[allansnum+hitnum] = SetCoord(u_const,t[j],0);		// ���Ƃ��ēo�^
					hitnum++;
				}
			}
			allansnum += hitnum;				// �����K�����̐��������𐔂��J�E���g�A�b�v
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

// NURBS�Ȗʂƕ��ʂƂ̌�_�Q���􉽊w�I�ɋ��߂�(�⏕���ʂ�p������@)
// ����  *nurb:NURBS�Ȗ�  pt:���ʏ�̈�_  nf:���ʂ̖@���x�N�g���@u_divnum:u�p�����[�^�������@v_divnum:v�p�����[�^������
//       *ans:�Z�o���ꂽ��_��u,v�p�����[�^�l�����ꂼ��ans.x,ans.y�Ɋi�[,  ans_size:ans�̔z��
// �Ԓl  ��_�̌�(��_�̐���ans_size�𒴂����FERR)
int NURBS_Func::CalcIntersecPtsPlaneGeom(NURBSS *nurb,Coord pt,Coord nf,int u_divnum,int v_divnum,Coord *ans,int ans_size)
{
	Coord init_pt;
	int ansnum=0;
	double pcolor[3] = {0,1,1};		// �\���̐F

	for(int u=0;u<u_divnum;u++){
		for(int v=0;v<v_divnum;v++){
			double u0 = nurb->U[0] + (nurb->U[1] - nurb->U[0])*(double)u/(double)u_divnum;
			double v0 = nurb->V[0] + (nurb->V[1] - nurb->V[0])*(double)v/(double)v_divnum;
			for(int i=0;i<LOOPCOUNTMAX;i++){
				Coord p0 = CalcNurbsSCoord(nurb,u0,v0);						// S(u0,v0)�ƂȂ�_(�����_)�̍��W
//				DrawPoint(p0,1,5,pcolor);				
				Coord su = CalcDiffuNurbsS(nurb,u0,v0);						// �_S(u0,v0)��u�Δ���(��{�x�N�g��)
				Coord sv = CalcDiffvNurbsS(nurb,u0,v0);						// �_S(u0,v0)��v�Δ���(��{�x�N�g��)
				Coord nt = DivCoord(CalcOuterProduct(su,sv),CalcEuclid(CalcOuterProduct(su,sv)));	// �_S(u0,v0)�̒P�ʖ@���x�N�g��
				Coord nn = DivCoord(CalcOuterProduct(nf,nt),CalcEuclid(CalcOuterProduct(nf,nt)));	// ����F�ƕ���Ft�ɒ������镽��Fn�̒P�ʖ@���x�N�g��
				double df = CalcInnerProduct(pt,nf);						// ���_���畽��F�܂ł̋���
				double dt = CalcInnerProduct(p0,nt);						// ���_���畽��Ft�܂ł̋���
				double dn = CalcInnerProduct(p0,nn);						// ���_���畽��Fn�܂ł̋���
				Coord cross_ntn = CalcOuterProduct(nt,nn);					// �P�ʖ@���x�N�g��nt,nn�̃x�N�g����
				Coord cross_nnf = CalcOuterProduct(nn,nf);					// �P�ʖ@���x�N�g��nn,nf�̃x�N�g����
				Coord cross_nft = CalcOuterProduct(nf,nt);					// �P�ʖ@���x�N�g��nf,nt�̃x�N�g����
				Coord nume_p1_sub =  AddCoord(MulCoord(cross_ntn,df),MulCoord(cross_nnf,dt));	// 3����F,Ft,Fn�̌�_p1�̕��q�̍ŏ���2�����v�Z
				Coord nume_p1 = AddCoord(nume_p1_sub,MulCoord(cross_nft,dn));			// p1�̕��q���Z�o
				double denom_p1 = CalcScalarTriProduct(nf,nt,nn);				// p1�̕�����Z�o
				Coord p1 = DivCoord(nume_p1,denom_p1);						// p1���Z�o
				Coord deltap = SubCoord(p1,p0);							// �_p1�Ɠ_p0�̋������Z�o
				double deltap_dis = CalcEuclid(deltap);						// ��p�̋������Z�o
				if(isnan(deltap_dis))	break;							// nan�����o������for���𔲂���
				double du = CalcScalarTriProduct(deltap,sv,nf)/CalcScalarTriProduct(su,sv,nf);	// �V�����_s(u0+du,v0+dv)��^���邽�߂�du���Z�o
				double dv = -CalcScalarTriProduct(deltap,su,nf)/CalcScalarTriProduct(su,sv,nf);	// �V�����_s(u0+du,v0+dv)��^���邽�߂�dv���Z�o
//				fprintf(stderr,"delta = %lf\n",deltap_dis);
//				fprintf(stderr,"du = %lf dv = %lf\n",du,dv);
				u0 += du;									// �V�����_��u�p�����[�^�𓾂�
				v0 += dv;									// �V�����_��v�p�����[�^�𓾂�
				if(u0 < nurb->U[0] || u0 > nurb->U[1] || v0 < nurb->V[0] || v0 > nurb->V[1]){	// �ǐՓ_���p�����[�^�̈�O�ɏo��
					//fprintf(stderr,"NURBS ERROR:�Ȗ�R�̃p�����[�^���̈�O�ɏo�܂���\n");
					break;
				}
				if(isnan(deltap_dis)){
					//fprintf(stderr,"NURBS ERROR:�_p0�Ɠ_q0�Ԃ̋����Z�o�Ɏ��s���܂���\n");
					break;	
				}
				//fprintf(stderr,"%lf,%lf\n",u0,v0);	// debug
				if(deltap_dis < CONVERG_GAP){							// ��p������������
					//fprintf(stderr,"   %d:%lf,%lf\n",ansnum,u0,v0);	// debug
					ans[ansnum] = SetCoord(u0,v0,0);					// ���Ƃ��ēo�^
					ansnum++;								// �����J�E���g
					//if(ansnum == ans_size)					// ���̐����������z����
						//return ansnum;
					break;
				}
			}
		}
	}
	
	return ansnum;
}

// 3���ȉ���NURBS�Ȗʂƕ��ʂƂ̌�_�Q����_�ǐՖ@�ɂċ��߂�
// �����@NURBSS:NURBS�Ȗ�  pt:���ʏ��1�_  nvec:���ʂ̖@���x�N�g��  ds:���(��_�Q)�̑e��(��0.1�`2�a)  initdivnum:�����_�T���̍r��(��10�`1�a)
//       ans:��  ans_size:���̐�
// �Ԓl�@KOD_FALSE:NURBS�Ȗʂƕ��ʂ��������Ă��Ȃ��@KOD_ERR:���ٓ_�܂��͔��U�ɂ�菈���𒆒f
int NURBS_Func::CalcIntersecPtsPlaneSearch3(NURBSS *nurb,Coord pt,Coord nvec,double ds,int initdivnum,Coord *ans,int ans_size)
{
	int loop_count=0;		// �����v�Z�̃��[�v��
	int pcount=0;
	int anscount=0;
	Coord oldp;
	Coord newp;
	Coord init_pt[INTERSECPTNUMMAX];		// �����_(u,v�p�����[�^�l)
	Coord init_pt_Coord[INTERSECPTNUMMAX];	// �����_(x,y,z���W�l)
	bool  init_pt_flag[INTERSECPTNUMMAX];	// �e�����_��ʂ�I�������𔻕ʂ���t���O
	bool  init_allpt_flag=KOD_FALSE;			// �����_��S�Ēʂ�I�������𔻕ʂ���t���O
	int   init_pt_num;						// �����_�̐�
	int   init_pt_flag_count=0;
	double u,v;								// ����ǐՒ���u,v�p�����[�^���Ԓl
	double dist;							// ���[�v�E�o�p�ɒǐՓ_�ԋ�����臒l�Ƃ���
	int loopbreak_flag = KOD_FALSE;				// �����_��v�t���O
	int  search_flag = KOD_TRUE;				// ����ǐՕ����t���O(KOD_TRUE:������,KOD_FALSE:�t����)
	int  inverse_flag = KOD_FALSE;				// ����ǐՕ����t�]�t���O
	double color[3] = {0,1,1};

	// �����_�ʉߔ��ʃt���O��������
	for(int i=0;i<INTERSECPTNUMMAX;i++){
		init_pt_flag[i] = KOD_FALSE;
	}
	init_pt_flag[0] = KOD_TRUE;

	// �܂�����ǐՖ@�̏����_�Ƃ��Č�_�����������߂�
	init_pt_num = CalcIntersecPtsPlaneGeom(nurb,pt,nvec,initdivnum,initdivnum,init_pt,INTERSECPTNUMMAX);
	init_pt_num = CheckTheSamePoints(init_pt,init_pt_num);		// ����_�͏�������
	if(!init_pt_num){		// ������Ȃ��ꍇ�́A�������Ă��Ȃ��Ƃ݂Ȃ�
		//SetMessage("NURBS KOD_ERROR:Intersection point is noexistence");
		return KOD_FALSE;					
	}
	else if(init_pt_num == KOD_ERR) return KOD_ERR;			// init_pt_num��init_pt�̔z�񒷂𒴂���

	for(int i=0;i<init_pt_num;i++){
		init_pt_Coord[i] = CalcNurbsSCoord(nurb,init_pt[i].x,init_pt[i].y);		// ��_��uv�p�����[�^��xyz���W�l�ɕϊ��������̂�ێ����Ă���
		//fprintf(stderr,"    %d:%lf,%lf\n",i,init_pt_Coord[i].x,init_pt_Coord[i].y);	// debug
		//DrawPoint(init_pt_Coord[i],1,5,color);	// debug
	}

	// �����_��S�Ēʉ߂���܂Ō���ǐՖ@���J��Ԃ�
	while(init_allpt_flag == KOD_FALSE){
		//fprintf(stderr,"Init%d:%lf,%lf,%lf\n",pcount+1,init_pt_Coord[pcount].x,init_pt_Coord[pcount].y,init_pt_Coord[pcount].z);		// debug
		// ����ǐՂ̂��߂̎n�_(u,v)���Z�b�g
		u = oldp.x = init_pt[pcount].x;
		v = oldp.y = init_pt[pcount].y;
		ans[anscount] = SetCoord(init_pt[pcount]);
		anscount++;
		init_pt_flag[pcount] = KOD_TRUE;
		init_pt_flag_count++;
		if(init_pt_flag_count == init_pt_num && init_pt_num > 1)	break;

		if(inverse_flag == KOD_TRUE){	// �t�����ւ̌���ǐՂ��I�����Ă�����
			inverse_flag = KOD_FALSE;	// ����ǐՕ������������ɖ߂�
		}
		// ����ǐՊJ�n
		loop_count = 0;
		while(loop_count < ans_size){
			// �������Ɍ���ǐ�
			//fprintf(stderr,"%d:%lf,%lf--->",loop_count,u,v);
			if(inverse_flag == KOD_FALSE){
				search_flag = SearchIntersectPt_RKM(nurb,pt,nvec,ds,&u,&v,FORWARD);	// �������̌�_�Z�o
				if(search_flag == KOD_FALSE){						// �������ǐՂɎ��s�����ꍇ��
					inverse_flag = KOD_TRUE;						// �t�����ǐՃt���O��ON
					u = init_pt[pcount].x;							// ��_�ǐՂ̏����_���Z�b�g���Ȃ���
					v = init_pt[pcount].y;
				}
			}
			// �t�����ǐՃt���O��ON�Ȃ�
			if(inverse_flag == KOD_TRUE){
				search_flag = SearchIntersectPt_RKM(nurb,pt,nvec,ds,&u,&v,INVERSE);	// �t�����̌�_�Z�o
 			}

			if(search_flag == KOD_ERR){					// ���ٓ_���o�Ȃǂɂ�菈�����p���ł��Ȃ��ꍇ
				//return KOD_ERR;
				//return anscount;
				SetMessage("NURBS ERROR: Singler point was ditected.");
				break;
			}
			else if(search_flag == KOD_FALSE){
				break;
			}
			//fprintf(stderr,"%lf,%lf\n",u,v);
			// �Ƃ肠�������炩�̉�������ꂽ(���̎��_�ł�u,v�͈͊O�ł����Ă��ʂ�)
			newp.x = u;								
			newp.y = v;

			// �����_������ǐՖ@�ɂ���đS�Ēʉ߂��������ׂ�
			for(int i=0;i<init_pt_num;i++){
				// �V���ɎZ�o���ꂽ��_��1�O�̌�_��Ίp�Ƃ��闧���̂̒��ɏ����_���܂܂�Ă�����
				if(CalcDistance(init_pt_Coord[i],CalcNurbsSCoord(nurb,u,v)) < ds){
					if(loop_count && i==pcount && inverse_flag == KOD_FALSE){	// ���[�v�ɑ΂��Ĉ�����Ė߂��Ă����ꍇ�̓��[�v�𔲂���
						loopbreak_flag = KOD_TRUE;	
						//fprintf(stderr,"%d loop break OK\n",i);		// debug
						break;					
					}
					if(init_pt_flag[i] == KOD_FALSE && search_flag == KOD_TRUE){		// �܂��ʉ߂��Ă��Ȃ������_�Ō�_��u,v�͈͓���������
						init_pt_flag[i] = KOD_TRUE;					// �ʉ߂������ƂƂ��ēo�^
						init_pt_flag_count++;						// �ʉߍςݏ����_�����J�E���g�A�b�v
						//fprintf(stderr,"%d OK\n",i);				// debug
					}
				}
			}

			// ���[�v�ɑ΂��Ĉ�����Ă����ꍇ�̓��[�v�𔲂���
			if(loopbreak_flag == KOD_TRUE){
				loopbreak_flag = KOD_FALSE;
				break;
			}

			// debug
			//Coord buf = CalcNurbsSCoord(nurb,newp.x,newp.y);
			//fprintf(stderr,"   %d:%lf,%lf\n",anscount+1,buf.x,buf.y);

			ans[anscount] = SetCoord(newp);	// ����ꂽu,v�����(��_�Q)�Ƃ��ēo�^
			anscount++;				// ��_�Q�̐����C���N�������g

			// ��_�̐����w��T�C�Y�𒴂����ꍇ�͂����܂łŋ������^�[��
			if(anscount >= ans_size){
				SetMessage("NURBS KOD_ERROR:Intersection points exceeded the allocated array length");
				SetMessage("There is a possibility that you set large ds."); 
				return anscount;
			}

			oldp = SetCoord(newp);	// ���̃��[�v�ŎZ�o���ꂽ��_�͎��̃��[�v�ł�1�O�̌�_�ƂȂ�

			loop_count++;		// ���[�v�񐔂��C���N�������g
		}// ����ǐՂ����܂�

		// �c�����_������΁A�ʂ̌��������̂ŁA���̓_���n�_�Ƃ��čēx����ǐՂ��s��
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

// NURBS�Ȗ�S(u,v)��NURBS�Ȗ�R(w,t)�̌��(��_�Q)����_�ǐՖ@�ɂċ��߂�
// �����@nurbsS:NURBS�Ȗ�S(u,v) nurbsR:NURBS�Ȗ�R(w,t) div:�����_�T�[�`���̋Ȗʕ�����  ds:���(��_�Q)�̑e��(��0.1�`2�a)  ans:��  ans_size:���̐�
// �Ԓl�@FALSE:NURBS�Ȗʓ��m���������Ă��Ȃ��@ERR:���ٓ_�܂��͔��U�ɂ�菈���𒆒f
int NURBS_Func::CalcIntersecPtsNurbsSSearch(NURBSS *nurbR,NURBSS *nurbS,int div,double ds,Coord *ansR,Coord *ansS,int ans_size)
{
	int ans_count=0;		// �ǐՓ_�̑���
	int loop_count=0;		// �����v�Z�̃��[�v��
	int pnow=0;
	Coord init_pt_R[INTERSECPTNUMMAX];		// �����_(u,v�p�����[�^�l)
	Coord init_pt_S[INTERSECPTNUMMAX];		// �����_(u,v�p�����[�^�l)
	Coord init_pt_Coord_R[INTERSECPTNUMMAX];	// �����_(x,y,z���W�l)
	Coord init_pt_Coord_S[INTERSECPTNUMMAX];
	int  init_pt_flag[INTERSECPTNUMMAX];		// �e�����_��ʂ�I�������𔻕ʂ���t���O
	int  init_allpt_flag=KOD_FALSE;			// �����_��S�Ēʂ�I�������𔻕ʂ���t���O
	int   init_pt_num = 0;				// �����_�̐�
	int  conform_flag = KOD_FALSE;			// �����_��v�t���O
	int  search_flag = KOD_TRUE;			// ����ǐՕ����t���O(KOD_TRUE:������,KOD_FALSE:�t����)
	int  inverse_flag = KOD_FALSE;			// ����ǐՕ����t�]�t���O
	double u,v,w,t;					// ����ǐՒ���u,v�p�����[�^���Ԓl
//	FILE *fp=fopen("debug.csv","w");
//	double color[3] = {0,1,1};
	
	// �����_�ʉߔ��ʃt���O��������
//	init_pt_flag[0] = KOD_TRUE;
	for(int i=0;i<INTERSECPTNUMMAX;i++){
		init_pt_flag[i] = KOD_FALSE;
	}
	init_pt_flag[0] = KOD_TRUE;

	// ����ǐՂ��邽�߂̏����_�ƂȂ�_���������T��
	// ������:�@�����̌�����[�v������ꍇ�A�S�Ă̌�����[�v��̏����_�������Ȃ���΂Ȃ�Ȃ�
	//�@�@�@�@�@���̂��߁A���܂蕪���������Ȃ��ƈꕔ�̌�����[�v��Ɍ��(��_�Q)����������Ȃ��Ȃ�ꍇ������
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
	if(!init_pt_num){		// ����ł�������Ȃ��ꍇ�́A�������Ă��Ȃ��Ƃ݂Ȃ�
		return KOD_FALSE;					
	}
	
	for(int i=0;i<init_pt_num;i++){
		init_pt_Coord_R[i] = CalcNurbsSCoord(nurbR,init_pt_R[i].x,init_pt_R[i].y);		// ��_��uv�p�����[�^��xyz���W�l�ɕϊ��������̂�ێ����Ă���
		init_pt_Coord_S[i] = CalcNurbsSCoord(nurbS,init_pt_S[i].x,init_pt_S[i].y);		// ��_��uv�p�����[�^��xyz���W�l�ɕϊ��������̂�ێ����Ă���
	//	DrawPoint(init_pt_Coord_R[i],1,5,color);
	//	DrawPoint(init_pt_Coord_S[i],1,5,color);
	}
	ansR[ans_count] = SetCoord(init_pt_R[0]);
	ansS[ans_count] = SetCoord(init_pt_S[0]);
	
	// �����_��S�Ēʉ߂���܂Ō���ǐՖ@���J��Ԃ�
	while(init_allpt_flag == KOD_FALSE){
		// ����ǐՂ̂��߂̎n�_R(w,t),S(u,v)���Z�b�g
		w = ansR[ans_count].x = init_pt_R[pnow].x;
		t = ansR[ans_count].y = init_pt_R[pnow].y;
		u = ansS[ans_count].x = init_pt_S[pnow].x;
		v = ansS[ans_count].y = init_pt_S[pnow].y;
 		if(inverse_flag == KOD_FALSE){		// �ǐՕ���������������t�����ɕς��Ƃ��ȊO
			ans_count++;			// �����J�E���g
			init_pt_flag[pnow] = KOD_TRUE;	// �����_�ʉ߃t���O�𗧂Ă�
		}
		else if(inverse_flag == KOD_TRUE)		// �ǐՕ���������������t�����ɕς��Ƃ�
			inverse_flag = KOD_FALSE;		// �ǐՕ���(������t)�t���O�����ɖ߂�
		
		// ����ǐՊJ�n
		while(1){
			// �ǐՕ������������̏ꍇ
			if(search_flag == KOD_TRUE){
				search_flag = SearchIntersectPt(nurbR,nurbS,ds,&w,&t,&u,&v,FORWARD);	// �������Ɍ���ǐ�
				if(search_flag == KOD_FALSE)						// uv�p�����[�^�O�ɏo����
 					inverse_flag = KOD_TRUE;						// �ǐՕ���(������t)�t���O�𗧂Ă�
			}
			// �ǐՕ������t�����̏ꍇ
			else if(search_flag == KOD_FALSE){
				int flag = SearchIntersectPt(nurbR,nurbS,ds,&w,&t,&u,&v,INVERSE);
				if(flag == KOD_FALSE)	// uv�p�����[�^�O�ɏo����
					search_flag = KOD_TRUE;						// �ǐՕ����t���O����������
 			}
			// ���ٓ_���o�Ȃǂɂ�菈�����p���ł��Ȃ��ꍇ
			else if(search_flag == KOD_ERR){
				return KOD_ERR;
			}

			Coord pr = CalcNurbsSCoord(nurbR,w,t);			// ����ꂽu,v��xyz���W�l�ɕϊ�
			Coord ps = CalcNurbsSCoord(nurbS,u,v);			// ����ꂽu,v��xyz���W�l�ɕϊ�
			double distr = CalcDistance(init_pt_Coord_R[pnow],pr);	// ����ꂽxyz���W�l�Ə����_�Ƃ̋������Z�o
			double dists = CalcDistance(init_pt_Coord_S[pnow],ps);	// ����ꂽxyz���W�l�Ə����_�Ƃ̋������Z�o
			
			// ��_�̌������~�b�g���z������
			if(ans_count >= ans_size-1){
				SetMessage("NURBS KOD_ERROR:Intersection points exceeded the allocated array length");
				return ans_count;
			}

			// �ŏ��ɋ��߂������_������ǐՖ@�ɂ���đS�Ēʉ߂��������ׂ�
			for(int i=0;i<init_pt_num;i++){
				if(CalcDistance(init_pt_Coord_R[i],pr) < ds){
					if(init_pt_flag[i] == KOD_TRUE && i < pnow){
						conform_flag = KOD_TRUE;
						break;
					}
					init_pt_flag[i] = KOD_TRUE;
				}
			}
			
			// u,v����蓾��p�����[�^�͈́i0�`1�j�𒴂����ꍇ�܂��́A�P�����Ė߂��Ă����ꍇ�̓��[�v�𔲂���
			if(!CheckRange(nurbR->U[0],nurbR->U[1],w,0) || !CheckRange(nurbR->V[0],nurbR->V[1],t,0) || (distr < ds/2 && loop_count > 0)){
				break;
			}
			
			if(!CheckRange(nurbS->U[0],nurbS->U[1],u,0) || !CheckRange(nurbS->V[0],nurbS->V[1],v,0) || (dists < ds/2 && loop_count > 0)){
				break;
			}
			
			// ����ꂽu,v�����(��_�Q)�Ƃ��ēo�^
			ansR[ans_count] = SetCoord(w,t,0);
			ansS[ans_count] = SetCoord(u,v,0);
			ans_count++;

			if(conform_flag == KOD_TRUE){
				conform_flag = KOD_FALSE;
				break;
			}

			loop_count++;		// ���[�v�񐔂��C���N�������g

 		}// ����ǐՂ����܂�

		// �c�����_������΁A�ʂ̌��������̂ŁA���̓_���n�_�Ƃ��čēx����ǐՂ��s��
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

// NURBS�Ȗ�S(u,v)��NURBS�Ȗ�R(w,t)�̌��(��_�Q)���􉽊w�I�ɋ��߂�(�⏕���ʂ�p������@)
// ����  *nurbS:NURBS�Ȗ�S(u,v) *nurbR:NURBS�Ȗ�R(w,t) u_divnum:u�p�����[�^�������@v_divnum:v�p�����[�^������
//       *ans:�Z�o���ꂽ��_��u,v�p�����[�^�l�����ꂼ��ans.x,ans.y�Ɋi�[,  ans_size:ans�̔z��
// �Ԓl  ��_�̌�(��_�̐���ans_size�𒴂����FERR)
int NURBS_Func::CalcIntersecPtsNurbsSGeom(NURBSS *nurbR,NURBSS *nurbS,int u_divnum,int v_divnum,Coord *ansR,Coord *ansS,int ans_size)
{
	int ansnum=0;
	
	// �e�Ȗʂ��w��̕�������uv�������A�����̓_�ɂ�����⏕���ʂ𐶐����Č����̔C�ӂ�1�_�Ɏ���������
	for(int w=0;w<u_divnum;w++){
		for(int t=0;t<v_divnum;t++){
			for(int u=0;u<u_divnum;u++){
				for(int v=0;v<v_divnum;v++){
					// �e�Ȗʂɕ����_�𐶐�����
					double w0 = nurbR->U[0] + (nurbR->U[1] - nurbR->U[0])*(double)w/(double)u_divnum;
					double t0 = nurbR->V[0] + (nurbR->V[1] - nurbR->V[0])*(double)t/(double)v_divnum;
					double u0 = nurbS->U[0] + (nurbS->U[1] - nurbS->U[0])*(double)u/(double)u_divnum;
					double v0 = nurbS->V[0] + (nurbS->V[1] - nurbS->V[0])*(double)v/(double)v_divnum;
					for(int i=0;i<10;i++){
						// �e��p�����[�^���Z�o����
						Coord p0 = CalcNurbsSCoord(nurbR,w0,t0);					// R(w0,t0)�ƂȂ�_(�����_)�̍��W
						Coord q0 = CalcNurbsSCoord(nurbS,u0,v0);					// S(u0,v0)�ƂȂ�_(�����_)�̍��W
						Coord rw = CalcDiffuNurbsS(nurbR,w0,t0);					// �_R(w0,t0)��u�Δ���(��{�x�N�g��)
						Coord rt = CalcDiffvNurbsS(nurbR,w0,t0);					// �_R(w0,t0)��v�Δ���(��{�x�N�g��)
						Coord np = DivCoord(CalcOuterProduct(rw,rt),CalcEuclid(CalcOuterProduct(rw,rt)));	// �_R(w0,t0)�̒P�ʖ@���x�N�g��
						Coord su = CalcDiffuNurbsS(nurbS,u0,v0);					// �_S(u0,v0)��u�Δ���(��{�x�N�g��)
						Coord sv = CalcDiffvNurbsS(nurbS,u0,v0);					// �_S(u0,v0)��v�Δ���(��{�x�N�g��)
						Coord nq = DivCoord(CalcOuterProduct(su,sv),CalcEuclid(CalcOuterProduct(su,sv)));	// �_S(u0,v0)�̒P�ʖ@���x�N�g��
						Coord nn = DivCoord(CalcOuterProduct(np,nq),CalcEuclid(CalcOuterProduct(np,nq)));	// ����Fp�ƕ���Fq�ɒ������镽��Fn�̒P�ʖ@���x�N�g��
						double dp = CalcInnerProduct(p0,np);						// ���_���畽��Fp�܂ł̋���
						double dq = CalcInnerProduct(q0,nq);						// ���_���畽��Fq�܂ł̋���
						double dn = CalcInnerProduct(p0,nn);						// ���_���畽��Fn�܂ł̋���
						Coord cross_nqn = CalcOuterProduct(nq,nn);					// �P�ʖ@���x�N�g��nq,nn�̃x�N�g����
						Coord cross_nnp = CalcOuterProduct(nn,np);					// �P�ʖ@���x�N�g��nn,np�̃x�N�g����
						Coord cross_npq = CalcOuterProduct(np,nq);					// �P�ʖ@���x�N�g��np,nq�̃x�N�g����
						Coord nume_p_sub =  AddCoord(MulCoord(cross_nqn,dp),MulCoord(cross_nnp,dq));	// 3����Fp,Fq,Fn�̌�_p�̕��q�̍ŏ���2�����v�Z
						Coord nume_p = AddCoord(nume_p_sub,MulCoord(cross_npq,dn));			// p�̕��q���Z�o
						double denom_p = CalcScalarTriProduct(np,nq,nn);				// p�̕�����Z�o
						Coord p = DivCoord(nume_p,denom_p);						// p���Z�o
						Coord deltap0 = SubCoord(p,p0);							// �_p�Ɠ_p0�̍��x�N�g�����Z�o
						Coord deltaq0 = SubCoord(p,q0);							// �_p�Ɠ_q0�̍��x�N�g�����Z�o
						Coord rw_sub = CalcOuterProduct(rw,np);						// ��{�x�N�g��rw�Ɩ@���x�N�g��np�ɒ�������x�N�g��
						Coord rt_sub = CalcOuterProduct(rt,np);						// ��{�x�N�g��rt�Ɩ@���x�N�g��np�ɒ�������x�N�g��
						Coord su_sub = CalcOuterProduct(su,nq);						// ��{�x�N�g��su�Ɩ@���x�N�g��nq�ɒ�������x�N�g��
						Coord sv_sub = CalcOuterProduct(sv,nq);						// ��{�x�N�g��sv�Ɩ@���x�N�g��nq�ɒ�������x�N�g��
						double dw = CalcInnerProduct(rt_sub,deltap0)/CalcInnerProduct(rt_sub,rw);	// �V�����_r(w0+dw,t0+dt)��^���邽�߂�dw���Z�o
						double dt = CalcInnerProduct(rw_sub,deltap0)/CalcInnerProduct(rw_sub,rt);	// �V�����_r(w0+dw,t0+dt)��^���邽�߂�dt���Z�o
						double du = CalcInnerProduct(sv_sub,deltaq0)/CalcInnerProduct(sv_sub,su);	// �V�����_r(w0+dw,t0+dt)��^���邽�߂�dw���Z�o
						double dv = CalcInnerProduct(su_sub,deltaq0)/CalcInnerProduct(su_sub,sv);	// �V�����_r(w0+dw,t0+dt)��^���邽�߂�dt���Z�o
						w0 += dw;									// �V�����_��w�p�����[�^�𓾂�
						t0 += dt;									// �V�����_��t�p�����[�^�𓾂�
						u0 += du;									// �V�����_��u�p�����[�^�𓾂�
						v0 += dv;									// �V�����_��v�p�����[�^�𓾂�
						
						// �Ȗʂ͈̔͊O�ɏo�Ă��܂����烋�[�v�𔲂���
						if(!CheckRange(nurbR->U[0],nurbR->U[1],w0,1) || !CheckRange(nurbR->V[0],nurbR->V[1],t0,1)){
							break;
						}
						if(!CheckRange(nurbS->U[0],nurbS->U[1],u0,1) || !CheckRange(nurbS->V[0],nurbS->V[1],v0,1)){
							break;
						}
						
						Coord deltapq = SubCoord(p0,q0);						// �_p0�Ɠ_q0�̍��x�N�g�����Z�o
						double deltapq_dis = CalcEuclid(deltapq);					// |p0-q0|�̋������Z�o
						
						if(isnan(deltapq_dis)){
							break;	
						}
						
						// �\���������������o�^����
						if(deltapq_dis < CONVERG_GAP){								
							if(!ansnum || (!CheckZero(ansR[ansnum-1].x-w0,MID_ACCURACY) && !CheckZero(ansR[ansnum-1].y-t0,MID_ACCURACY))){// ���O�ɎZ�o�������Ɠ���̉��łȂ����
								ansR[ansnum] = SetCoord(w0,t0,0);						// ���Ƃ��ēo�^
								ansS[ansnum] = SetCoord(u0,v0,0);
								ansnum++;								// �����J�E���g
								if(ansnum == ans_size)					// ���̐����������z����
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

// �j���[�g���@�ɂ���_��^�l�Ɏ���������(NURBS�Ȗʂƕ���)
int NURBS_Func::SearchIntersectPt(NURBSS *nurb,Coord pt,Coord nvec,double ds,double *u,double *v,int direction)
{
	double d = CalcInnerProduct(pt,nvec);	// ���_���畽�ʂ܂ł̋���

	// �܂������l�Ƃ��Ă�du,dv�����߂�
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
	if(CheckZero(phi_u,MID_ACCURACY) == KOD_TRUE || CheckZero(phi_v,MID_ACCURACY) == KOD_TRUE){			// ���ٓ_
		//SetMessage("NURBS KOD_ERROR:The process is stoped by detected singular point.");
		return KOD_ERR;				
	}

	// ����ǐՏ������̏ꍇ
	if(direction == FORWARD){
		f = 1/f;
	}
	// ����ǐՋt�����̏ꍇ
	else if(direction == INVERSE){
		f = -1/f;
	}

	double du = -f*phi_v*ds;		// �����l
	double dv = f*phi_u*ds;			// �����l

	// �j���[�g���@��p����u,v��^�l�Ɏ���������
	int k=0;
	if(fabs(dv) > fabs(du)){				// dv>du�̏ꍇ��dv��萔�Ƃ��ČŒ肷��
		while(!CheckZero(du,MID_ACCURACY)){		// du����������܂ŌJ��Ԃ��v�Z
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
	else{									// dv<du�̏ꍇ��du��萔�Ƃ��ČŒ肷��
		while(!CheckZero(dv,MID_ACCURACY)){		// dv����������܂ŌJ��Ԃ��v�Z
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

// 4���̃����Q�N�b�^�@�ɂ���_�𓱏o(NURBS�Ȗʂƕ���)
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
		if(CheckZero(fu,MID_ACCURACY) == KOD_TRUE || CheckZero(fv,MID_ACCURACY) == KOD_TRUE){			// ���ٓ_
			//SetMessage("NURBS KOD_ERROR:The process is stoped by detected singular point.");
			return KOD_ERR;				
		}
		double E = CalcInnerProduct(Su,Su);		// 1���K�i��
		double F = CalcInnerProduct(Su,Sv);		// 1���K�i��
		double G = CalcInnerProduct(Sv,Sv);		// 1���K�i��
		double f_ = 1/sqrt(E*fvv - 2*F*fuv + G*fuu);
		p[i] = -delta*fv*f_*(double)direction;
		q[i] = delta*fu*f_*(double)direction;
	}
	*u = u0+(p[0]+2*p[1]+2*p[2]+p[3])/6;
	*v = v0+(q[0]+2*q[1]+2*q[2]+q[3])/6;

	return KOD_TRUE;
}

// �j���[�g���@�ɂ���_��^�l�Ɏ���������(NURBS�Ȗʓ��m)
int NURBS_Func::SearchIntersectPt(NURBSS *nurbR,NURBSS *nurbS,double ds,double *w,double *t,double *u,double *v,int direction)
{
	double **J;
	double D[3];
	double ans[3];
	int flag = KOD_TRUE;

	// �������m��
	if((J = NewMatrix(3,3)) == NULL){
		SetMessage("NURBS ERROR: fail to malloc\n");
		return KOD_ERR;
	}

	// �܂������l�Ƃ��Ă�dw,dt,du,dv�����߂�
	Coord r = CalcNurbsSCoord(nurbR,*w,*t);					// �_R(w,t)��NURBS�Ȗʂ̍��W�l�����߂�
	Coord s = CalcNurbsSCoord(nurbS,*u,*v);					// �_S(u,v)��NURBS�Ȗʂ̍��W�l�����߂�
	Coord rw = CalcDiffuNurbsS(nurbR,*w,*t);					// �_R(w,t)��u�Δ���(��{�x�N�g��)
	Coord rt = CalcDiffvNurbsS(nurbR,*w,*t);					// �_R(w,t)��v�Δ���(��{�x�N�g��)
	Coord su = CalcDiffuNurbsS(nurbS,*u,*v);					// �_S(u,v)��u�Δ���(��{�x�N�g��)
	Coord sv = CalcDiffvNurbsS(nurbS,*u,*v);					// �_S(u,v)��v�Δ���(��{�x�N�g��)
	Coord n1 = DivCoord(CalcOuterProduct(rw,rt),CalcEuclid(CalcOuterProduct(rw,rt)));	// �_R(w0,t0)�̒P�ʖ@���x�N�g��
	Coord n2 = DivCoord(CalcOuterProduct(su,sv),CalcEuclid(CalcOuterProduct(su,sv)));	// �_S(u0,v0)�̒P�ʖ@���x�N�g��
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
	if(!phi1 || !phi2){			// ���ٓ_
		SetMessage("NURBS KOD_ERROR:The process is stoped by detected singular point.");
		FreeMatrix(J,3);
		return KOD_ERR;				
	}
	
	// ����ǐՏ������̏ꍇ
	if(direction == FORWARD){
		phi1 = 1/phi1;
		phi2 = -1/phi2;
	}
	// ����ǐՋt�����̏ꍇ
	else if(direction == INVERSE){
		phi1 = -1/phi1;
		phi2 = 1/phi2;
	}
	
	// �����p�����[�^�̏����l��ݒ�
	double dw = -f1*phi1*ds;		
	double dt = g1*phi1*ds;		
	double du = -f2*phi2*ds;
	double dv = g2*phi2*ds;
	double sort[4] = {fabs(dw),fabs(dt),fabs(du),fabs(dv)};	// �\�[�g�p�ϐ���p��
	BubbleSort(sort,4);					// �����Ƀ\�[�g
	double max_delta = sort[3];				// �e�p�����[�^�̒��ōő�l�𓾂�

	// �j���[�g���@��p����w,t,u,v��^�l�Ɏ���������
	int k=0;	// �����v�Z�񐔂�������
	// dw,dt,du,dv�̐�Βl����dw���ő�̎��Adw��萔�Ƃ��ČŒ肷��
	if(max_delta == fabs(dw)){
		while(fabs(dt) > CONVERG_GAP || fabs(du) > CONVERG_GAP || fabs(dv) > CONVERG_GAP){	
			r = CalcNurbsSCoord(nurbR,*w,*t);						// �_R(w,t)��NURBS�Ȗʂ̍��W�l�����߂�
			s = CalcNurbsSCoord(nurbS,*u,*v);						// �_S(u,v)��NURBS�Ȗʂ̍��W�l�����߂�
			rw = CalcDiffuNurbsS(nurbR,*w,*t);						// �_R(w,t)��u�Δ���(��{�x�N�g��)
			rt = CalcDiffvNurbsS(nurbR,*w,*t);						// �_R(w,t)��v�Δ���(��{�x�N�g��)
			su = CalcDiffuNurbsS(nurbS,*u,*v);						// �_S(u,v)��u�Δ���(��{�x�N�g��)
			sv = CalcDiffvNurbsS(nurbS,*u,*v);						// �_S(u,v)��v�Δ���(��{�x�N�g��)
			
			// 3�A�����������������߂Ɋe�p�����[�^��z��Ɋi�[����
			double sol[3] = {s.x-r.x-rw.x*dw, s.y-r.y-rw.y*dw, s.z-r.z-rw.z*dw};
			double coef[3][3] = {{rt.x,-su.x,-sv.x},{rt.y,-su.y,-sv.y},{rt.z,-su.z,-sv.z}};
			
			for(int i=0;i<3;i++){
				D[i] = sol[i];
				ans[i] = 0;
				for(int j=0;j<3;j++){
					J[i][j] = coef[i][j];
				}
			}

			// �A���������������A�p�����[�^���X�V
			Gauss(3,J,D,ans);
			dt = ans[0];
			du = ans[1];
			dv = ans[2];
			*t += dt;
			*u += du;
			*v += dv;
			
			// uv�p�����[�^�͈͊O�ɏo����
			if(!CheckRange(nurbR->V[0],nurbR->V[1],*t,0) || !CheckRange(nurbS->U[0],nurbS->U[1],*u,0) || !CheckRange(nurbS->V[0],nurbS->V[1],*v,0) || k>LOOPCOUNTMAX){
				flag = KOD_FALSE;
				goto EXIT;
			}
			k++;
		}
		*w += dw;	// ����������Œ肵�Ă����p�����[�^���X�V
		if(!CheckRange(nurbR->U[0],nurbR->U[1],*w,0)){
			flag = KOD_FALSE;
			goto EXIT;
		}
	}
	
	// dw,dt,du,dv�̐�Βl����dt���ő�̎��Adt��萔�Ƃ��ČŒ肷��
	else if(max_delta == fabs(dt)){	
		while(fabs(dw) > CONVERG_GAP || fabs(du) > CONVERG_GAP || fabs(dv) > CONVERG_GAP){	
			r = CalcNurbsSCoord(nurbR,*w,*t);					// �_R(w,t)��NURBS�Ȗʂ̍��W�l�����߂�
			s = CalcNurbsSCoord(nurbS,*u,*v);					// �_S(u,v)��NURBS�Ȗʂ̍��W�l�����߂�
			rw = CalcDiffuNurbsS(nurbR,*w,*t);					// �_R(w,t)��u�Δ���(��{�x�N�g��)
			rt = CalcDiffvNurbsS(nurbR,*w,*t);					// �_R(w,t)��v�Δ���(��{�x�N�g��)
			su = CalcDiffuNurbsS(nurbS,*u,*v);					// �_S(u,v)��u�Δ���(��{�x�N�g��)
			sv = CalcDiffvNurbsS(nurbS,*u,*v);					// �_S(u,v)��v�Δ���(��{�x�N�g��)
			
			// 3�A�����������������߂Ɋe�p�����[�^��z��Ɋi�[����
			double sol[3] = {s.x-r.x-rt.x*dt, s.y-r.y-rt.y*dt, s.z-r.z-rt.z*dt};
			double coef[3][3] = {{rw.x,-su.x,-sv.x},{rw.y,-su.y,-sv.y},{rw.z,-su.z,-sv.z}};
			for(int i=0;i<3;i++){
				D[i] = sol[i];
				ans[i] = 0;
				for(int j=0;j<3;j++){
					J[i][j] = coef[i][j];
				}
			}
			
			// �A���������������A�p�����[�^���X�V
			Gauss(3,J,D,ans);
			dw = ans[0];
			du = ans[1];
			dv = ans[2];
			*w += dw;
			*u += du;
			*v += dv;
				
			// uv�p�����[�^�͈͊O�ɏo����
			if(!CheckRange(nurbR->U[0],nurbR->U[1],*w,0) || !CheckRange(nurbS->U[0],nurbS->U[1],*u,0) || !CheckRange(nurbS->V[0],nurbS->V[1],*v,0) || k>LOOPCOUNTMAX){
				flag = KOD_FALSE;
				goto EXIT;
			}
			k++;
		}
		*t += dt;	// ����������Œ肵�Ă����p�����[�^���X�V
		if(!CheckRange(nurbR->V[0],nurbR->V[1],*t,0)){
			flag = KOD_FALSE;
			goto EXIT;
		}
	}
			
	// dw,dt,du,dv�̐�Βl����du���ő�̎��Adu��萔�Ƃ��ČŒ肷��
	else if(max_delta == fabs(du)){	
		while(fabs(dw) > CONVERG_GAP || fabs(dt) > CONVERG_GAP || fabs(dv) > CONVERG_GAP){	
			r = CalcNurbsSCoord(nurbR,*w,*t);					// �_R(w,t)��NURBS�Ȗʂ̍��W�l�����߂�
			s = CalcNurbsSCoord(nurbS,*u,*v);					// �_S(u,v)��NURBS�Ȗʂ̍��W�l�����߂�
			rw = CalcDiffuNurbsS(nurbR,*w,*t);					// �_R(w,t)��u�Δ���(��{�x�N�g��)
			rt = CalcDiffvNurbsS(nurbR,*w,*t);					// �_R(w,t)��v�Δ���(��{�x�N�g��)
			su = CalcDiffuNurbsS(nurbS,*u,*v);					// �_S(u,v)��u�Δ���(��{�x�N�g��)
			sv = CalcDiffvNurbsS(nurbS,*u,*v);					// �_S(u,v)��v�Δ���(��{�x�N�g��)
			
			// 3�A�����������������߂Ɋe�p�����[�^��z��Ɋi�[����
			double sol[3] = {s.x-r.x+su.x*du, s.y-r.y+su.y*du, s.z-r.z+su.z*du};
			double coef[3][3] = {{rw.x,rt.x,-sv.x},{rw.y,rt.y,-sv.y},{rw.z,rt.z,-sv.z}};
			for(int i=0;i<3;i++){
				D[i] = sol[i];
				ans[i] = 0;
				for(int j=0;j<3;j++){
					J[i][j] = coef[i][j];
				}
			}
			
			// �A���������������A�p�����[�^���X�V
			Gauss(3,J,D,ans);
			dw = ans[0];
			dt = ans[1];
			dv = ans[2];
			*w += dw;
			*t += dt;
			*v += dv;
			
			// uv�p�����[�^�͈͊O�ɏo����
			if(!CheckRange(nurbR->U[0],nurbR->U[1],*w,0) || !CheckRange(nurbR->V[0],nurbR->V[1],*t,0) || !CheckRange(nurbS->V[0],nurbS->V[1],*v,0) || k>LOOPCOUNTMAX){
				flag = KOD_FALSE;
				goto EXIT;
			}
			k++;
		}
		*u += du;	// ����������Œ肵�Ă����p�����[�^���X�V
		if(!CheckRange(nurbS->U[0],nurbS->U[1],*u,0)){
			flag = KOD_FALSE;
			goto EXIT;
		}
	}
	
	// dw,dt,du,dv�̐�Βl����dv���ő�̎��Adv��萔�Ƃ��ČŒ肷��
	else if(max_delta == fabs(dv)){	
		while(fabs(dt) > CONVERG_GAP || fabs(dw) > CONVERG_GAP || fabs(du) > CONVERG_GAP){	
			r = CalcNurbsSCoord(nurbR,*w,*t);					// �_R(w,t)��NURBS�Ȗʂ̍��W�l�����߂�
			s = CalcNurbsSCoord(nurbS,*u,*v);					// �_S(u,v)��NURBS�Ȗʂ̍��W�l�����߂�
			rw = CalcDiffuNurbsS(nurbR,*w,*t);					// �_R(w,t)��u�Δ���(��{�x�N�g��)
			rt = CalcDiffvNurbsS(nurbR,*w,*t);					// �_R(w,t)��v�Δ���(��{�x�N�g��)
			su = CalcDiffuNurbsS(nurbS,*u,*v);					// �_S(u,v)��u�Δ���(��{�x�N�g��)
			sv = CalcDiffvNurbsS(nurbS,*u,*v);					// �_S(u,v)��v�Δ���(��{�x�N�g��)
			
			// 3�A�����������������߂Ɋe�p�����[�^��z��Ɋi�[����
			double sol[3] = {s.x-r.x+sv.x*dv, s.y-r.y+sv.y*dv, s.z-r.z+sv.z*dv};
			double coef[3][3] = {{rw.x,rt.x,-su.x},{rw.y,rt.y,-su.y},{rw.z,rt.z,-su.z}};
			for(int i=0;i<3;i++){
				D[i] = sol[i];
				ans[i] = 0;
				for(int j=0;j<3;j++){
					J[i][j] = coef[i][j];
				}
			}
			
			// �A���������������A�p�����[�^���X�V
			Gauss(3,J,D,ans);
			dw = ans[0];
			dt = ans[1];
			du = ans[2];
			*w += dw;
			*t += dt;
			*u += du;
			
			// uv�p�����[�^�͈͊O�ɏo����
			if(!CheckRange(nurbR->U[0],nurbR->U[1],*w,0) || !CheckRange(nurbR->V[0],nurbR->V[1],*t,0) || !CheckRange(nurbS->U[0],nurbS->U[1],*u,0) || k>LOOPCOUNTMAX){
				flag = KOD_FALSE;
				goto EXIT;
			}
			k++;
		}
		*v += dv;	// ����������Œ肵�Ă����p�����[�^���X�V
		if(!CheckRange(nurbS->V[0],nurbS->V[1],*v,0)){
			flag = KOD_FALSE;
			goto EXIT;
		}
	}

EXIT:
	// ���������
	FreeMatrix(J,3);
	
	return flag;
}


// NURBS�Ȑ�C(u) or C(v)�̌W�������߂�
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

// NURBS�Ȑ��ƕ��ʂƂ̌�_�����߂�(3���܂őΉ�)
// ����  *nurb:NURBS�Ȑ�  pt:���ʏ�̈�_  nvec:���ʂ̖@���x�N�g��  *ans:�Z�o���ꂽ��_��t�p�����[�^�l���i�[,  ans_size:ans�̔z��
// �Ԓl  ��_�̌�
// ��_�͍ő��(M-1)*(K-M+1)�_������.  (��F4�K�ŃR���g���[���|�C���g�̐�8�̏ꍇ�A(4-1)*(8-4+1)=15�_)
// ����Ĉ���*ans��(M-1)*(K-M+1)�̔z���p�ӂ��邱�Ƃ��]�܂���.
int NURBS_Func::CalcIntersecCurve3(NURBSC *nurb,Coord pt,Coord nvec,double *ans,int ans_size)
{
	double **coef;
	double Q[4];	// NURBS�Ȑ��̕���̌W��
	Coord  P[4];	// NURBS�Ȑ��̕��q�̌W��
	double a[4];
	double t[3];
	int ansnum;
	int k=0;

	if((coef = NewMatrix(nurb->M,nurb->M)) == NULL){
		SetMessage("NURBS KOD_ERROR: CalcIntersecPlane3()");
		return KOD_ERR;
	}

	// 1�{��NURBS�Ȑ���K-M+1�{�̋Ȑ�����\�������B���ꂼ��̍\���Ȑ��ɑ΂��ĕ������𓱏o���A���𓾂�B
	for(int i=0;i<nurb->K-nurb->M+1;i++){
		if(nurb->M-1 == 3){			// 3��			
			GetBSplCoef3(nurb->M,nurb->K,i,nurb->T,coef);	// �e�R���g���[���|�C���g�ɂ�����3��B�X�v���C�����֐��̌W��(coef)�����߂�
		}
		else if(nurb->M-1 == 2){	// 2��
			GetBSplCoef2(nurb->M,nurb->K,i,nurb->T,coef);	// �e�R���g���[���|�C���g�ɂ�����2��B�X�v���C�����֐��̌W�������߂�
		}
		else if(nurb->M-1 == 1){	// 1��	
			GetBSplCoef1(nurb->M,nurb->K,i,nurb->T,coef);	// �e�R���g���[���|�C���g�ɂ�����1��B�X�v���C�����֐��̌W�������߂�
		}
		else{
			char mes[256];
			sprintf(mes,"NURBS KOD_ERROR:Ther order of equation is unsupported. (order = %d)",nurb->M-1);
			SetMessage(mes);
			goto EXIT;
		}
		GetNurbsCCoef(nurb,coef,i,P,Q);						// NURBS�Ȑ��̌W��(P,Q)�����߂�
		GetIntersecEquation(nurb->M,P,Q,pt,nvec,a);			// NURBS�Ȑ��ƕ��ʂ̌�����o�p�������𓾂�
		ansnum = CalcEquation(a,t,nurb->M-1);					// �������������A��_�̃p�����[�^�l�𓾂�

		for(int j=0;j<ansnum;j++){
			if(t[j] >= nurb->T[i+nurb->M-1] && t[j] <= nurb->T[i+nurb->M]){	// �m�b�g�x�N�g���̒l�ƓK��������̂̂݉��Ƃ��Ē��o
				if(k == ans_size){
					SetMessage("NURBS KOD_ERROR:Intersection points exceeded the allocated array length");
					goto EXIT;
				}
				ans[k] = t[j];		// �����擾
				k++;				// ���̐����C���N�������g
			}
		}
	}

	return k;

EXIT:
	FreeMatrix(coef,nurb->M);
	return KOD_ERR;
}

// 3���������܂ł�����
int NURBS_Func::CalcEquation(double *a,double *t,int M)
{
	int flag;

	if(M == 3)		flag = CalcCubicEquation(a,t);
	else if(M == 2)	flag = CalcQuadraticEquation(a,t);
	else if(M == 1) flag = CalcLinearEquation(a,t);
	else			return KOD_ERR;

	return flag;
}

// NURBS�Ȑ��ƕ��ʂ̌�����o�p�������𓾂�
void NURBS_Func::GetIntersecEquation(int M,Coord *P,double *Q,Coord pt,Coord nvec,double *a)
{
	for(int i=0;i<M;i++){
		a[i] = (Q[i]*pt.x-P[i].x)*nvec.x + (Q[i]*pt.y-P[i].y)*nvec.y + (Q[i]*pt.z-P[i].z)*nvec.z;
	}
}

// NURBS�Ȑ��̌W�������߂�(�ō�3��)
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

// 3����B�X�v���C���Ȑ��̊e�W�������߂�
// ���� M:�K��  K:�R���g���[���|�C���g�̐�  i:���ڒ��̃R���g���[���|�C���g *t:�m�b�g�x�N�g����  *coef:�Z�o�����W�����i�[
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


// 2����B�X�v���C���Ȑ��̊e�W�������߂�
// ���� M:�K��  K:�R���g���[���|�C���g�̐�  i:���ڒ��̃R���g���[���|�C���g *t:�m�b�g�x�N�g����  *coef:�Z�o�����W�����i�[
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

// 1����B�X�v���C���Ȑ��̊e�W�������߂�
// ���� M:�K��  K:�R���g���[���|�C���g�̐�  i:���ڒ��̃R���g���[���|�C���g *t:�m�b�g�x�N�g����  *coef:�Z�o�����W�����i�[
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

// NURBS�Ȗʂ̃V�t�g
// ����  *nurbs:�ύX�����NURBS�Ȗ�  shift:�V�t�g��
void NURBS_Func::ShiftNurbsS(NURBSS *nurbs,Coord shift)
{
	for(int i=0;i<nurbs->K[0];i++){
		for(int j=0;j<nurbs->K[1];j++){
			nurbs->cp[i][j] = AddCoord(nurbs->cp[i][j],shift);
		}
	}
}

// NURBS�Ȑ��̃V�t�g
// ����  *nurbs:�ύX�����NURBS�Ȑ�  shift:�V�t�g��
void NURBS_Func::ShiftNurbsC(NURBSC *nurbs,Coord shift)
{
	for(int i=0;i<nurbs->K;i++){
		nurbs->cp[i] = AddCoord(nurbs->cp[i],shift);
	}
}

// NURBS�Ȗʂ�D�x�N�g������deg(��)������]������
// �����@*nurbs:�ύX�����NURBS�Ȗʁ@axis:��]���̒P�ʃx�N�g���@deg:�p�x(degree)
void NURBS_Func::RotNurbsS(NURBSS *nurbs,Coord axis,double deg)
{
	double rad;			// ���W�A���i�[�p
	QUATERNION QFunc;	// �N�H�[�^�j�I���֘A�̊֐����W�߂��N���X�̃I�u�W�F�N�g�𐶐�
	Quat StartQ;		// ��]�O�̍��W���i�[����N�H�[�^�j�I��
	Quat RotQ;			// ��]�N�H�[�^�j�I��
	Quat ConjuQ;		// �����N�H�[�^�j�I��
	Quat TargetQ;		// ��]��̍��W���i�[����N�H�[�^�j�I��
	
	for(int i=0;i<nurbs->K[0];i++){			// u�����̃R���g���[���|�C���g�����[�v
		for(int j=0;j<nurbs->K[1];j++){		// v�����̃R���g���[���|�C���g�����[�v
			StartQ = QFunc.QInit(1,nurbs->cp[i][j].x,nurbs->cp[i][j].y,nurbs->cp[i][j].z);		// NURBS�Ȗʂ��\������cp�̍��W��o�^
			rad = DegToRad(deg);										// degree����radian�ɕϊ�
			RotQ = QFunc.QGenRot(rad,axis.x,axis.y,axis.z);				// ��]�N�H�[�^�j�I���ɉ�]�ʂ�o�^(�����̐�����������ΔC�ӂɉ�]�ł���)
			ConjuQ = QFunc.QConjugation(RotQ);							// RotQ�̋����N�H�[�^�j�I����o�^
			TargetQ = QFunc.QMult(QFunc.QMult(RotQ,StartQ),ConjuQ);		// ��]������
			nurbs->cp[i][j] = SetCoord(TargetQ.x,TargetQ.y,TargetQ.z);	// ��]��̍��W��o�^
		}
	}
}

// NURBS�Ȗʂ�D�x�N�g������deg(��)������]������
// �����@*nurbs:�ύX�����NURBS�Ȑ��@axis:��]���̒P�ʃx�N�g���@deg:�p�x(degree)
void NURBS_Func::RotNurbsC(NURBSC *nurbs,Coord axis,double deg)
{
	double rad;			// ���W�A���i�[�p
	QUATERNION QFunc;	// �N�H�[�^�j�I���֘A�̊֐����W�߂��N���X�̃I�u�W�F�N�g�𐶐�
	Quat StartQ;		// ��]�O�̍��W���i�[����N�H�[�^�j�I��
	Quat RotQ;			// ��]�N�H�[�^�j�I��
	Quat ConjuQ;		// �����N�H�[�^�j�I��
	Quat TargetQ;		// ��]��̍��W���i�[����N�H�[�^�j�I��
	
	for(int i=0;i<nurbs->K;i++){		// �R���g���[���|�C���g�����[�v
		StartQ = QFunc.QInit(1,nurbs->cp[i].x,nurbs->cp[i].y,nurbs->cp[i].z);		// NURBS�Ȗʂ��\������cp�̍��W��o�^
		rad = DegToRad(deg);									// degree����radian�ɕϊ�
		RotQ = QFunc.QGenRot(rad,axis.x,axis.y,axis.z);			// ��]�N�H�[�^�j�I���ɉ�]�ʂ�o�^(�����̐�����������ΔC�ӂɉ�]�ł���)
		ConjuQ = QFunc.QConjugation(RotQ);						// RotQ�̋����N�H�[�^�j�I����o�^
		TargetQ = QFunc.QMult(QFunc.QMult(RotQ,StartQ),ConjuQ);	// ��]������
		nurbs->cp[i] = SetCoord(TargetQ.x,TargetQ.y,TargetQ.z);	// ��]��̍��W��o�^
	}
}

// NURBS�Ȗʂ̔{����ύX����
// ����  *nurbs:�ύX�����NURBS�Ȗ�  ratio:�{��
void NURBS_Func::ChRatioNurbsS(NURBSS *nurbs,Coord ratio)
{
	for(int i=0;i<nurbs->K[0];i++){
		for(int j=0;j<nurbs->K[1];j++){
			nurbs->cp[i][j] = MulCoord(nurbs->cp[i][j],ratio);
		}
	}
}

// NURBS�Ȑ��̔{����ύX����
// ����  *nurbs:�ύX�����NURBS�Ȑ�  ratio:�{��
void NURBS_Func::ChRatioNurbsC(NURBSC *nurbs,Coord ratio)
{
	for(int i=0;i<nurbs->K;i++){
		nurbs->cp[i] = MulCoord(nurbs->cp[i],ratio);
	}
}

// NURBS�ȖʊԂ̃R���g���[���|�C���g�̑��
// ����  *nurbs:���������  Nurbs:�����
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

// �^����ꂽ�_����Ԃ���ō�4�K��NURBS�Ȑ��𐶐�����
// �[��������^���Ȃ��o�[�W����
// ����   *Nurbs:���������NURBS�Ȑ��̃A�h���X   *P:�_��   PNum:�_��̐�   M:�K��
int NURBS_Func::GenInterpolatedNurbsC1(NURBSC *Nurbs,Coord *P,int PNum,int M)
{
	int retflag = KOD_TRUE;

	if(PNum <= 1){			// �^����ꂽ�_��1�����̏ꍇ�́ANURBS�Ȑ��𐶐��ł��Ȃ�
		SetMessage("NURBS KOD_ERROR:Few Point. You should set over 2 points at least");
		return KOD_ERR;
	}
	if(PNum == 2 || PNum == 3)	M = PNum;	// �^����ꂽ�_��2��3�̏ꍇ�́A�K���������I��2��3�ɂ���

	int K = PNum;			// �R���g���[���|�C���g�̐�
	int N = M+K;			// �m�b�g�x�N�g���̐�
	int prop[4] = {0,0,1,0};// �p�����[�^
	double V[2] = {0,1};	// �m�b�g�x�N�g���̊J�n�l,�I���l

	Vector T_ = NewVector(K);	// �ʉߓ_��̋Ȑ��p�����[�^
	Vector T = NewVector(N);	// �m�b�g�x�N�g��
	Matrix B = NewMatrix(K,K);	// B�X�v���C�����֐��s��
	Matrix B_ = NewMatrix(K,K);	// B�X�v���C�����֐��s��̋t�s��i�[�p
	Vector W = NewVector(K);	// �d��
	Coord *Q = NewCoord1(K);	// �R���g���[���|�C���g

	// �ʉߓ_��̋Ȑ��p�����[�^�𓾂�
	GetKnotParam2(P,PNum,T_);

	// �m�b�g�x�N�g���𓾂�
	GetKnot(T_,N,K,M,T);

	// B�X�v���C�����֐��s��𐶐�
	for(int i=0;i<K;i++){
		for(int j=0;j<K;j++){
			B[i][j] = CalcBSbasis(T_[i],T,N,j,M);
		}
	}

	// B�X�v���C�����֐��s��̋t�s������߂�
	double det = Gauss(K,B,P,Q);
	if(det == 0){
		SetMessage("NURBS ERROR:Determinant is 0");
		retflag = KOD_ERR;
		goto EXIT;
	}

	// �R���g���[���|�C���g�Əd�݂𓾂�
	for(int i=0;i<K;i++){
		//MulMxVec(B_,K,K,P,Q);
		W[i] = 1.0;
	}

	// NURBS�Ȑ��𐶐�����
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

// �^����ꂽ�_����Ԃ���ō�4�K��NURBS�Ȑ��𐶐�����
// �[������:�n�_��C2�ň�v
// ����   *Nurbs:���������NURBS�Ȑ��̃A�h���X   *P_:�ʉߓ_��   PNum:�ʉߓ_��̐�   M:�K��
// P_[0]��P_[PNum-1]�͈�v���Ă��邱��
// �Ԓl   KOD_TRUE:����I��
//		  KOD_FALSE:�_��̎n�_�ƏI�_����v���Ă��Ȃ�
//		  KOD_ERR:�_��̐���1����
int NURBS_Func::GenInterpolatedNurbsC2(NURBSC *Nurbs,Coord *P_,int PNum,int M)
{
	if(DiffCoord(P_[0],P_[PNum-1]) == KOD_FALSE){
		SetMessage("NURBS KOD_ERROR:Given points P0 and Pn are not unmuched");
		return KOD_FALSE;
	}
	if(PNum <= 1){			// �^����ꂽ�_��1�����̏ꍇ�́ANURBS�Ȑ��𐶐��ł��Ȃ�
		SetMessage("NURBS KOD_ERROR:Few Point. You should set over 2 points at least");
		return KOD_ERR;
	}
	if(PNum == 2 || PNum == 3)	M = PNum;	// �^����ꂽ�_��2��3�̏ꍇ�́A�K���������I��2��3�ɂ���

	int K = PNum+M-2;			// �R���g���[���|�C���g�̐�
	int N = M+K;				// �m�b�g�x�N�g���̐�
	int prop[4] = {0,0,1,0};	// �p�����[�^
	double V[2] = {0,1};		// �m�b�g�x�N�g���̊J�n�l,�I���l

	Vector T_ = NewVector(PNum);	// �ʉߓ_��̋Ȑ��p�����[�^
	Vector T = NewVector(N);		// �m�b�g�x�N�g��
	Coord *P = NewCoord1(N);		// �ʉߓ_����i�[
	Coord *Q = NewCoord1(K);		// �R���g���[���|�C���g
	Matrix B = NewMatrix(K,K);		// B�X�v���C�����֐��s��
	Vector W = NewVector(K);		// �d��

	// �ʉߓ_��x�N�g���𐶐�
	for(int i=0;i<PNum;i++){
		P[i] = SetCoord(P_[i]);
	}
	P[PNum] = SetCoord(0,0,0);
	P[PNum+1] = SetCoord(0,0,0);

	// �ʉߓ_��̋Ȑ��p�����[�^�𓾂�
	GetKnotParam1(P_,PNum,T_);

	// �m�b�g�x�N�g���𓾂�
	for(int i=0;i<N;i++){
		if(i < M)	T[i] = 0;
		else if(i >= K)	T[i] = 1;
		else{
			T[i] = T_[i-M+1];
		}
	}

	// B�X�v���C�����֐��s��𐶐�
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

	// �R���g���[���|�C���g�𓾂�
	Gauss(K,B,P,Q);

	//for(int i=0;i<K;i++)
	//	fprintf(stderr,"%lf,%lf,%lf\n",Q[i].x,Q[i].y,Q[i].z);

	// �d�݂𓾂�
	for(int i=0;i<K;i++){
		W[i] = 1.0;
	}

	// NURBS�Ȑ��𐶐�����
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

// �Ȑ��p�����[�^����m�b�g�x�N�g���𓾂�
// ����   T_:�Ȑ��p�����[�^��  N:�m�b�g�x�N�g���̐�  M:�K��   T:�i�[����m�b�g�x�N�g����
void NURBS_Func::GetKnot(Vector T_,int N,int K,int M,Vector T)
{
	for(int i=0;i<M;i++)
		T[i] = 0;
	for(int j=1;j<K-M+1;j++){
		double d=0;
		for(int i=j;i<j+M-1;i++){
			d += T_[i];
		}
		T[j+M-1] = d/(M-1);
	}
	for(int i=K;i<K+M;i++)
		T[i] = 1;

}

// �e�ʉߓ_�̋Ȑ��p�����[�^���Z�o(�R�[�h���̔䂩��Z�o)
// ����		*P:�ʉߓ_��   PNum:�ʉߓ_��̐�    T_:�Ȑ��p�����[�^���i�[
void NURBS_Func::GetKnotParam1(Coord *P,int PNum,Vector T_)
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

// �e�ʉߓ_�̋Ȑ��p�����[�^���Z�o(�R�[�h���̕������̔䂩��Z�o)
// ����		*P:�ʉߓ_��   PNum:�ʉߓ_��̐�    T_:�Ȑ��p�����[�^���i�[
void NURBS_Func::GetKnotParam2(Coord *P,int PNum,Vector T_)
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


// �^����ꂽ�_����ߎ�����n�K��NURBS�Ȑ��𐶐�����
// ����   *Nurbs:���������NURBS�Ȑ��̃A�h���X   *P:�_��   PNum:�_��̐�   M:�K��
int NURBS_Func::GenApproximationNurbsC(NURBSC *Nurbs,Coord *P,int PNum,int M)
{
	if(PNum <= 1){			// �^����ꂽ�_��1�����̏ꍇ�́ANURBS�Ȑ��𐶐��ł��Ȃ�
		SetMessage("NURBS KOD_ERROR:Few Point. You should set over 2 points at least");
		return KOD_ERR;
	}

	int K = PNum/2;			// �R���g���[���|�C���g�̐�
	int Nnum = M+K;			// �m�b�g�x�N�g���̐�
	int prop[4] = {0,0,1,0};// �p�����[�^
	double V[2] = {0,1};	// �m�b�g�x�N�g���̊J�n�l,�I���l

	Vector T_ = NewVector(PNum);	// �ʉߓ_��̋Ȑ��p�����[�^
	Vector W = NewVector(K);		// �d��

	// �ʉߓ_��̋Ȑ��p�����[�^�𓾂�
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

	// �m�b�g�x�N�g����ݒ肷��
	Vector T = NewVector(Nnum);
	for(int i=0;i<M;i++)	T[i] = 0;
	for(int i=K;i<K+M;i++)	T[i] = 1;
	double d = (double)PNum/(double)(K-M+1);
	for(int j=1;j<K-M+1;j++){
		int i = (int)(j*d);
		double a = (double)j*d - (double)i;
		T[j+M-1] = (1-a)*T_[i-1] + a*T_[i];
		T[j+M-1] += 0.0001;					// ��!  T��T_�����l�ɂȂ�ƁA�ŏ��Q��@�����܂������Ȃ��̂ŁA�֋X�I�ɓ��l�ɂȂ�Ȃ��悤�ɂ��Ă���B
	}


	Matrix N = NewMatrix(PNum-2,K-2);
	for(int i=0;i<PNum-2;i++){
		for(int j=0;j<K-2;j++){
			N[i][j] =  CalcBSbasis(T_[i+1],T,Nnum,j+1,M);
		}
	}
	
	Coord *R = NewCoord1(K-2);
	for(int i=0;i<K-2;i++){
		InitCoord(&R[i]);
		for(int j=0;j<PNum-2;j++){
			Coord NP0 = MulCoord(P[0],CalcBSbasis(T_[j+1],T,Nnum,0,M));
			Coord NPN = MulCoord(P[PNum-1],CalcBSbasis(T_[j+1],T,Nnum,K-1,M));
			Coord R_ = SubCoord(P[j+1],AddCoord(NP0,NPN));
			R[i] = AddCoord(R[i],MulCoord(R_,N[j][i]));
		}
	}

	Matrix N_ = NewMatrix(K-2,K-2);			// (NTN)^-1
	Matrix NTN = NewMatrix(K-2,K-2);		// NT*N
	Matrix NT = NewMatrix(K-2,PNum-2);		// N�̓]�u�s��NT
	TranMx(N,PNum-2,K-2,NT);				// calc NT
	MulMxMx(NT,K-2,PNum-2,N,PNum-2,K-2,NTN);// calc NTN
	Coord *Q_ = NewCoord1(K-2);
	Gauss(K-2,NTN,R,Q_);

	// �R���g���[���|�C���g
	Coord *Q = NewCoord1(K);
	Q[0] = SetCoord(P[0]);
	Q[K-1] = SetCoord(P[PNum-1]);
	for(int i=1;i<K-1;i++){
		Q[i] = SetCoord(Q_[i-1]);
	}
	for(int i=0;i<K;i++){	// �d�݂�1�ŌŒ�
		W[i] = 1;
	}

	GenNurbsC(Nurbs,K,M,Nnum,T,W,Q,V,prop,0);	// NURBS�Ȑ�����

	FreeCoord1(Q);
	FreeCoord1(Q_);
	FreeMatrix(N_,K-2);
	FreeMatrix(NTN,K-2);
	FreeMatrix(NT,K-2);
	FreeCoord1(R);
	FreeMatrix(N,PNum-2);
	FreeVector(T);
	FreeVector(T_);
	FreeVector(W);

	return KOD_TRUE;
}

// �܂���𐶐�����
// ����   *Nurbs:���������NURBS�Ȑ��̃A�h���X   *P:�R���g���[���|�C���g   PNum:�R���g���[���|�C���g�̐�
int NURBS_Func::GenPolygonalLine(NURBSC *Nurbs,Coord *P,int PNum)
{
	if(PNum <= 1){			// �^����ꂽ�_��1�����̏ꍇ�́ANURBS�Ȑ��𐶐��ł��Ȃ�
		SetMessage("NURBS KOD_ERROR:Few Point. You should set over 2 points at least");
		return KOD_ERR;
	}

	int M=2;					// �K��2
	int K=PNum;					// �R���g���[���|�C���g�̐�
	int N=PNum+2;				// �m�b�g�x�N�g���̐�
	int prop[4] = {0,0,1,0};	// �p�����[�^
	double V[2] = {0,1};		// �m�b�g�x�N�g���̊J�n�l,�I���l
	Vector T = NewVector(N);	// �m�b�g�x�N�g��
	Vector W = NewVector(K);	// �d��

	// �m�b�g�x�N�g�������߂�
	T[0] = T[1] = 0.0;
	T[K] = T[K+1] = 1.0;
	double d_sum=0;
	for(int i=1;i<K;i++)
		d_sum += CalcDistance(P[i],P[i-1]);
	for(int i=2;i<K;i++){
		double d = CalcDistance(P[i-1],P[i-2]);
		T[i] = T[i-1] + d/d_sum;
	}

	// �E�F�C�g
	for(int i=0;i<K;i++){
		W[i] = 1.0;
	}

	// NURBS�Ȑ��𐶐�����
	GenNurbsC(Nurbs,K,M,N,T,W,P,V,prop,0);

	FreeVector(T);
	FreeVector(W);

	return KOD_TRUE;
}

// �^����ꂽ�_����Ԃ���ō�4�K��NURBS�Ȗʂ𐶐�����
// �[��������^���Ȃ��o�[�W����
// ����   *Nurbs:���������NURBS�Ȗʂ̃A�h���X   **P:�^����ꂽ�_��   PNum_u,PNum_v:�_�̐��@ Mu,Mv:�K��
int NURBS_Func::GenInterpolatedNurbsS1(NURBSS *Nurbs,Coord **P,int PNum_u,int PNum_v,int Mu,int Mv)
{
	if(PNum_u <= 1 || PNum_v <= 1){			// �^����ꂽ�_���e������1�����̏ꍇ�́ANURBS�Ȗʂ𐶐��ł��Ȃ�
		SetMessage("NURBS ERROR:Few Point. You should set over 2 points at least");
		return KOD_ERR;
	}
	if(PNum_u == 2 || PNum_u == 3)	Mu = PNum_u;	// u�����ɗ^����ꂽ�_��2��3�̏ꍇ�́Au�����̊K���������I��2��3�ɂ���
	if(PNum_v == 2 || PNum_v == 3)	Mv = PNum_v;	// v�����ɗ^����ꂽ�_��2��3�̏ꍇ�́Av�����̊K���������I��2��3�ɂ���

	int K[2] = {PNum_u,PNum_v};		// �R���g���[���|�C���g�̐�
	int N[2] = {Mu+PNum_u,Mv+PNum_v};	// �m�b�g�x�N�g���̐�
	int prop[5] = {0,0,1,0,0};		// �p�����[�^
	double U[2] = {0,1};			// u�����m�b�g�x�N�g���̊J�n�l�A�I���l
	double V[2] = {0,1};			// v�����m�b�g�x�N�g���̊J�n�l�A�I���l

	Vector S_ = NewVector(K[0]);		// u�����̒ʉߓ_��̋Ȑ��p�����[�^
	Vector S = NewVector(N[0]);			// u�����̃m�b�g�x�N�g��
	Vector T_ = NewVector(K[1]);		// v�����̒ʉߓ_��̋Ȑ��p�����[�^
	Vector T = NewVector(N[1]);			// v�����̃m�b�g�x�N�g��
	Matrix Bu = NewMatrix(K[0],K[0]);	// u������B�X�v���C�����֐��s��
	Matrix Bu_ = NewMatrix(K[0],K[0]);	// u������B�X�v���C�����֐��s��̋t�s��i�[�p
	Matrix Bv = NewMatrix(K[1],K[1]);	// v������B�X�v���C�����֐��s��
	Matrix Bv_ = NewMatrix(K[1],K[1]);	// v������B�X�v���C�����֐��s��̋t�s��i�[�p
	Matrix W = NewMatrix(K[0],K[1]);	// �d��
	Coord **PT = NewCoord2(K[1],K[0]);	// �]�u�����_��P
	Coord **R = NewCoord2(K[0],K[1]);	// �A�C�\�p���Ȑ��̃R���g���[���|�C���g
	Coord **RT = NewCoord2(K[1],K[0]);	// �]�u�����R���g���[���|�C���gR
	Coord **Q = NewCoord2(K[0],K[1]);	// NURBS�Ȗʂ̃R���g���[���|�C���g

	// u�����̒ʉߓ_��̋Ȑ��p�����[�^�𓾂�
	Vector du_sum = NewVector(K[1]);
	InitVector(du_sum,K[1]);
	for(int i=0;i<K[1];i++){
		for(int j=1;j<K[0];j++){
			du_sum[i] += CalcEuclid(SubCoord(P[j][i],P[j-1][i]));
		}
	}
	S_[0] = 0;
	for(int i=1;i<K[0]-1;i++){
		S_[i] = 0;
		for(int j=0;j<K[1];j++){
			double du = CalcEuclid(SubCoord(P[i][j],P[i-1][j]));
			S_[i] += S_[i-1] + du/du_sum[j];
		}
		S_[i] /= K[1];
	}
	S_[K[0]-1] = 1;
	
	// v�����̒ʉߓ_��̋Ȑ��p�����[�^�𓾂�
	Vector dv_sum = NewVector(K[0]);
	InitVector(dv_sum,K[0]);
	for(int i=0;i<K[0];i++){
		for(int j=1;j<K[1];j++){
			dv_sum[i] += CalcEuclid(SubCoord(P[i][j],P[i][j-1]));
		}
	}
	T_[0] = 0;
	for(int i=1;i<K[1]-1;i++){
		T_[i] = 0;
		for(int j=0;j<K[0];j++){
			double dv = CalcEuclid(SubCoord(P[j][i],P[j][i-1]));
			T_[i] += T_[i-1] + dv/dv_sum[j];
		}
		T_[i] /= K[0];
	}
	T_[K[1]-1] = 1;

	// �m�b�g�x�N�g��S�𓾂�
	for(int i=0;i<N[0];i++){
		if(i<Mu)		S[i] = 0;
		else if(i>=K[0])	S[i] = 1;
		else{
			double pu=0;
			for(int k=i-Mu+1;k<=i-1;k++){
				pu += S_[k];
			}
			S[i] = pu/(Mu-1);
		}
	}

	// �m�b�g�x�N�g��T�𓾂�
	for(int i=0;i<N[1];i++){
		if(i<Mv)		T[i] = 0;
		else if(i>=K[1])	T[i] = 1;
		else{
			double pv=0;
			for(int k=i-Mv+1;k<=i-1;k++){
				pv += T_[k];
			}
			T[i] = pv/(Mv-1);
		}
	}

	// u������B�X�v���C�����֐��s��𐶐�
	for(int i=0;i<K[0];i++){
		for(int j=0;j<K[0];j++){
			Bu[i][j] = CalcBSbasis(S_[i],S,N[0],j,Mu);
		}
	}

	// v������B�X�v���C�����֐��s��𐶐�
	for(int i=0;i<K[1];i++){
		for(int j=0;j<K[1];j++){
			Bv[i][j] = CalcBSbasis(T_[i],T,N[1],j,Mv);
		}
	}

	// u������B�X�v���C�����֐��s��̋t�s������߂�
	MatInv(K[0],Bu,Bu_);

	// v������B�X�v���C�����֐��s��̋t�s������߂�
	MatInv(K[1],Bv,Bv_);

	// �A�C�\�p���Ȑ��̃R���g���[���|�C���g�𓾂�
	TranMx(P,K[0],K[1],PT);
	for(int i=0;i<K[1];i++){
		MulMxVec(Bu_,K[0],K[0],PT[i],RT[i]);
	}

	// NURBS�Ȗʂ̃R���g���[���|�C���g�𓾂�
	TranMx(RT,K[1],K[0],R);
	for(int i=0;i<K[0];i++){
 		MulMxVec(Bv_,K[1],K[1],R[i],Q[i]);
 	}

	// �d�݂𓾂�
	for(int i=0;i<K[0];i++){
		for(int j=0;j<K[1];j++){
			W[i][j] = 1;
		}
	}

	// NURBS�Ȗʂ𐶐�����
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
	FreeVector(du_sum);
	FreeVector(dv_sum);

	return KOD_TRUE;
}

// �܂�ʂ𐶐�����
// ����   *Nurbs:���������NURBS�Ȗʂ̃A�h���X   **P:�R���g���[���|�C���g   PNum_u,PNum_v:�R���g���[���|�C���g�̐�
int NURBS_Func::GenPolygonalSurface(NURBSS *Nurbs,Coord **P,int PNum_u,int PNum_v)
{
	int Mu=2;						// �K��2
	int Mv=2;
	int K[2] = {PNum_u,PNum_v};		// �R���g���[���|�C���g�̐�
	int N[2] = {PNum_u+2,PNum_v+2};	// �m�b�g�x�N�g���̐�
	int prop[4] = {0,0,1,0};		// �p�����[�^
	double U[2] = {0,1};			// u�����m�b�g�x�N�g���̊J�n�l�A�I���l
	double V[2] = {0,1};			// v�����m�b�g�x�N�g���̊J�n�l�A�I���l
	Vector S = NewVector(N[0]);			// u�����̃m�b�g�x�N�g��
	Vector T = NewVector(N[1]);			// v�����̃m�b�g�x�N�g��
	Matrix W = NewMatrix(K[0],K[1]);	// �d��

	// u�����m�b�g�x�N�g�������߂�
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
	
	// v�����m�b�g�x�N�g�������߂�
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

	// �E�F�C�g
	for(int i=0;i<K[0];i++){
		for(int j=0;j<K[1];j++){
			W[i][j] = 1.0;
		}
	}

	// NURBS�Ȗʂ𐶐�����
	GenNurbsS(Nurbs,Mu,Mv,K[0],K[1],S,T,W,P,U[0],U[1],V[0],V[1]);

	FreeVector(S);
	FreeVector(T);
	FreeMatrix(W,K[1]);
	FreeVector(du_sum);
	FreeVector(dv_sum);

	return KOD_TRUE;
}

// NURBS�Ȗʂƒ����̌�_���Z�o
// ����    *Nurb:NURBS�Ȗ�S(u,v)�ւ̃|�C���^,  r:����N(t)���1�_,  p:����N(t)�̕���,  *ans:��_���W,  *param:��_��u,v,t�p�����[�^
// �Ԓl    KOD_ERR:�������Ȃ�����    KOD_TRUE:��������
int NURBS_Func::CalcuIntersecPtNurbsLine(NURBSS *Nurb,Coord r,Coord p,Coord *ans,Coord *param)
{
	Coord d = SetCoord(100,100,100);		// NURBS�Ȑ�S(u,v)�̔����ω���(du,dv)�A����N(t)�̔����ω���dt���i�[
	Coord F,Fu,Fv,Ft;						// F(u,v,t) = S(u,v) - N(t)    Fu = dF/du     Fv = dF/dv     Ft = dF/dt
	double u_next = Nurb->U[0];				// NURBS�Ȑ�S(u,v)��u�p�����[�^�̏����l
	double v_next = Nurb->V[0];				// NURBS�Ȑ�S(u,v)��v�p�����[�^�̏����l
	double u=0,v=0;								// NURBS�Ȑ�S(u,v)��u,v�p�����[�^�̌��ݒl
	double t = 0;							// ����N(t)��t�p�����[�^
	Matrix A = NewMatrix(3,3);				// Fu,Fv,Ft���\������3x3�s��
	Matrix A_ = NewMatrix(3,3);				// A�̋t�s����i�[
	bool flag = false;						// �����t���O
	double deltav = (Nurb->V[1] - Nurb->V[0])/CONVDIVNUM;	// �������Z�p��v�p�����[�^�̃C���^�[�o���l
	double deltau = (Nurb->U[1] - Nurb->U[0])/CONVDIVNUM;	// �������Z�p��u�p�����[�^�̃C���^�[�o���l
	int loopcount = 0;						// �����v�Z��
	int allloopcount = 0;

	for(int i=0;i<CONVDIVNUM;i++){
		v_next = Nurb->V[0];
		for(int j=0;j<CONVDIVNUM;j++){
			// �����̔����ω���dt(=d.z)��APPROX_ZERO�������܂Ńj���[�g���@�ɂ������v�Z���s��
			while(loopcount < LOOPCOUNTMAX){
				allloopcount++;
				F = SubCoord(CalcNurbsSCoord(Nurb,u,v),AddCoord(r,MulCoord(p,t)));	// F(u,v,t) = S(u,v) - N(t) = S(u,v) - (r+tp)
				Fu = CalcDiffuNurbsS(Nurb,u,v);			// Fu = dF/du = dS/du
				Fv = CalcDiffvNurbsS(Nurb,u,v);			// Fv = dF/dv = dS/dv
				Ft = MulCoord(p,-1);							// Ft = dF/dt = -dN/dt = -p

				// Fu,Fv,Ft��3x3�s��A�ɑ��
				A[0][0] = Fu.x;
				A[0][1] = Fv.x;				//     |Fu.x Fv.x Ft.x|       |du|       |F.x|
				A[0][2] = Ft.x;				// A = |Fu.y Fv.y Ft.y| , d = |dv| , F = |F.y|
				A[1][0] = Fu.y;				//     |Fu.z Fv.z Ft.z|       |dt|       |F.z|
				A[1][1] = Fv.y;
				A[1][2] = Ft.y;				// A�Ed = F   --->   d = A_�EF
				A[2][0] = Fu.z;
				A[2][1] = Fv.z;
				A[2][2] = Ft.z;

				// �t�s������߂�	
				if(MatInv3(A,A_) == KOD_FALSE)	break;

				d = MulCoord(MulMxCoord(A_,F),-1);			// d���Z�o

				// �^�l�Ɏ���������loop����C�ɔ�����
				if(fabs(d.x) <= APPROX_ZERO_H && fabs(d.y) <= APPROX_ZERO_H && fabs(d.z) <= APPROX_ZERO_H){
					flag = true;		// �����t���OON
					break;
				}

				// �^�l�ɒB���Ă��Ȃ�������u,v,t���X�V
				u += d.x;
				v += d.y;
				t += d.z;

				// u,v�̂ǂ��炩�����U������while(1)�𔲂���
				if(u < Nurb->U[0] || u > Nurb->U[1] || v < Nurb->V[0] || v > Nurb->V[1])	break;
				loopcount++;
			}// while�����܂�

			// �������Ă��Ȃ�������l���X�V
			if(flag == false){
				v_next += deltav;
				v = v_next;
				u = u_next;
				t = 0;
				loopcount = 0;
			}
			else
				break;
		}

		// �������Ă��Ȃ�������l���X�V
		if(flag == false){
			u_next += deltau;
			u = u_next;
		}
		else
			break;
	}

	FreeMatrix(A,3);
	FreeMatrix(A_,3);

	if(flag == false){
		return KOD_ERR;
	}
	else{
		*ans = AddCoord(r,MulCoord(p,t));
		*param = SetCoord(u,v,t);
		return allloopcount;
	}
}

// ���ڒ���NURBS�Ȗʏ��1�_(u,v)���g���~���O�̈���ɂ���̂��𔻒肷��
// �Ԓl  KOD_TRUE:�ʏ�  KOD_ONEDGE:�G�b�W��  KOD_FALSE:�ʊO   KOD_ERR:�G���[
int NURBS_Func::DetermPtOnTRMSurf(TRMS *Trim,double u,double v)
{
	int flag;

	// �O���g����
	if(Trim->n1){
		flag = DetermPtOnTRMSurf_sub(Trim->pTO,u,v);
		if(flag == KOD_ERR)
			return KOD_ERR;
		else if(flag == KOD_FALSE)		// �O
			return KOD_FALSE;
		else if(flag == KOD_ONEDGE)		// �G�b�W��
			return KOD_ONEDGE;
	}

	// �����g����
	if(Trim->n2){
		for(int i=0;i<Trim->n2;i++){		// �����̃g���~���O�̈�S�Ăɑ΂���
			flag = DetermPtOnTRMSurf_sub(Trim->pTI[i],u,v);
			if(flag == KOD_ERR)
				return KOD_ERR;
			else if(flag == KOD_TRUE)	// ��
				return KOD_FALSE;
		}
	}

	return KOD_TRUE;
}

// �ʏ���̃^�C�v�������Ȑ��̏ꍇ�̃g���~���O�̈���O����
int NURBS_Func::DetermPtOnTRMSurf_sub(CONPS *Conps,double u,double v)
{
	// �ʏ���������Ȑ��ɂȂ��Ă��邱��
	if(Conps->BType != COMPOSITE_CURVE){
		SetMessage("NURBS_Func ERROR:TRIM������!");
		return KOD_ERR;
	}

	COMPC *CompC=(COMPC *)Conps->pB;	// NURBS�Ȗʂ̃p�����[�^��ԏ�ɍ\������Ă��镡���Ȑ��ւ̃|�C���^�����o��

	double ent_dev=0;
	NURBSC *NurbsC;			// �g�������E��(NURBS�Ȑ�)�̃|�C���^����Ɨp�Ɋi�[
	Coord *P;				// �g�������E����ɐ��������_(���p�`�ߎ��p�̓_)���i�[
	Coord TargetPoint = SetCoord(u,v,0);		// �^�[�Q�b�g�ƂȂ�ʏ�̓_(u,v)��Coord�Ɋi�[
	int trm_flag = KOD_FALSE;	// �g���~���O�̈���O����p�t���O
	int divnum = TRM_BORDERDIVNUM;
	int ptnum=0;

	// �������m��
	if((P = (Coord *)malloc(sizeof(Coord)*(CompC->N*TRM_BORDERDIVNUM))) == NULL){
		return KOD_ERR;
	}
	
	// �g�������E����ɓ_�𐶐��i�g�������E���𑽊p�`�ߎ��j
	for(int i=0;i<CompC->N;i++){
		// �g�������E����NURBS�Ȑ��ō\������Ă���
		if(CompC->DEType[i] == NURBS_CURVE){
			NurbsC = (NURBSC *)CompC->pDE[i];	// ���ڒ���Nurbs�Ȑ��̃|�C���^���擾
			if(NurbsC->K == 2 && CompC->DegeFlag == KOD_TRUE)	divnum = 2;		// �R���g���[���|�C���g��2�̏ꍇ�͒����Ȃ̂ŁA�����_�𐶐����Ȃ��Ă��悭����
			else divnum = TRM_BORDERDIVNUM;
			for(int j=0;j<divnum-1;j++){
				ent_dev = NurbsC->T[NurbsC->M-1]+(NurbsC->T[NurbsC->K]-NurbsC->T[NurbsC->M-1])*(double)j/((double)divnum-1);	// �����_t�����߂�
				P[ptnum] = CalcNurbsCCoord(NurbsC,ent_dev);	// NURBS�Ȗʂ̃p�����[�^��ԓ���NURBS�Ȑ��̕����_t�̍��W�l(u,v)�𓾂�
				ptnum++;
			}
		}
		// ����ȊO
		else{
			SetMessage("NURBS_Func ERROR:�g�������E����NURBS�Ȑ��ȊO�ō\������Ă��܂�.������!");
			goto EXIT;
		}
	}
	
	trm_flag = IsPointInPolygon(TargetPoint,P,ptnum);		// ���O����

EXIT:
	// ���������
	free(P);
	
	return trm_flag;
}

// NURBS�Ȑ�C(t)�̎w��p�����[�^���[a,b]�̐�����L�����߂�
// L = S|C'(t)|dt	(S�͐ϕ��L��)
// �ϕ��͐��l�ϕ�(�K�E�X-���W�����h����10���_)��p����
double NURBS_Func::CalcNurbsCLength(NURBSC *Nurb,double a,double b)
{
	// ���_
	double g[10] = {	0.076526521133497333, 0.227785851141645078,
							0.373706088715419561, 0.510867001950827098,
							0.636053680726515025, 0.746331906460150793,
							0.839116971822218823, 0.912234428251325906,
							0.963971927277913791, 0.993128599185094925};
	// �d��
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
// NURBS�Ȑ�C(t)�̑S��Ԃ̐�����L�����߂�
// L = S|C'(t)|dt	(S�͐ϕ��L��)
// �ϕ��͐��l�ϕ�(�K�E�X-���W�����h����10���_)��p����
double NURBS_Func::CalcNurbsCLength(NURBSC *Nurb)
{
	// ���_
	double g[10] = {	0.076526521133497333, 0.227785851141645078,
							0.373706088715419561, 0.510867001950827098,
							0.636053680726515025, 0.746331906460150793,
							0.839116971822218823, 0.912234428251325906,
							0.963971927277913791, 0.993128599185094925};
	// �d��
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


// �w�肵���Ԋu��NURBS�Ȑ���̍��W�l���o�͂���
// ����
// *Nurb:NURBS�ւ̃|�C���^  D:�Ԋu  *Pts:�o�͂������W�l���i�[
// �Ԓl   �_��
int NURBS_Func::CalcDeltaPtsOnNurbsC(NURBSC *Nurb,double D,Coord *Pts)
{
	double L = CalcNurbsCLength(Nurb);		// NURBS�Ȑ��̐������𓾂�
	double T = Nurb->V[1] - Nurb->V[0];	// �p�����g���b�N��ԓ��ł̐������𓾂�
	double t = T/L*D;
	double now=0;	// �p�����g���b�N��ԓ��ł̌��݂̒��ړ_

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

// ����_����������
// ����   *P:�_�Q(�ύX��̓_�Q�������Ɋi�[�����)   N:�_��
// �Ԓl   �ύX��̓_��
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

// �ɒn�T�����𓾂�
// �ɒn�T������2������
int NURBS_Func::CalcExtSearchCurve(NURBSS *S,Coord n,Coord pt,double ds,NURBSC *C1,NURBSC *C2)
{
	// �H����

}

// �ɒn�X�ΐ��𓾂�
// �ɒn�X�ΐ���2������
int NURBS_Func::CalcExtGradCurve(NURBSS *S,Coord n,Coord pt,double ds,NURBSC *C1,NURBSC *C2)
{
	// �H����
}
