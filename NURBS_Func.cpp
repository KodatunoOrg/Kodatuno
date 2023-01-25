#include "NURBS_Func.h"


// 1��Nurbs�Ȑ��𐶐�����
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

	return TRUE;
}

// GenNurbsC()�ɂ���Đ������ꂽNURBS�Ȑ����폜����
void NURBS_Func::DelNurbsC(NURBSC *Nurbs)
{
	Free_NurbsC(Nurbs);
}

// 1��Nurbs�Ȗʂ𐶐�����(NURBS�Ȗʂ̃R�s�[)
// �����@*Nurbs:�V���ɐ�������NURBS�Ȗʁ@nurb:�����
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

	// �������[�m��
	if(New_NurbsS(Nurbs,Nurbs->K,Nurbs->N) == ERR){
		fprintf(stderr,"NURBS_Func ERROR:�������[�̊m�ۂɎ��s���܂���\n");
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

// GenNurbsS()�ɂ���Đ������ꂽNURBS�Ȗʂ��폜����
void NURBS_Func::DelNurbsS(NURBSS *Nurbs)
{
	Free_NurbsS(Nurbs);
}

// �g�����ʂ�L����NURBS�Ȗʂ𐶐�����
int NURBS_Func::GenTrimdNurbsS(TRIMD_NURBSS *TNurbs,TRIMD_NURBSS  tnurb)
{
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

	New_TrmS(TNurbs,tnurb.n2);								// �g�����ʂ̃������[�m��

	conps_i = (CONPS *)malloc(sizeof(CONPS)*tnurb.n2);		// �������\������ʏ���̃������[�m��
	compc_i = (COMPC *)malloc(sizeof(COMPC)*tnurb.n2);		// �������\�����镡���Ȑ��̃������[�m��

	// NURBS�Ȑ����g�����������\������NURBS�Ȑ��Ɋ֘A�t����
	// �O���g����
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

	// �����g����
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

// GenTrimdNurbsS()�ɂ���Đ������ꂽ�g�����ʂ��폜����
int NURBS_Func::DelTrimdNurbsS(TRIMD_NURBSS *TNurbs)
{
	int curve_num = 0;


	// �g�����ʂ��\������S�Ă�NURBS�Ȑ��̖{���𒲂ׂ�
	for(int i=0;i<TNurbs->n2;i++){
		for(int j=0;j<TNurbs->pTI[i]->pB->CompC.N;j++){
			curve_num++;
		}
	}
	curve_num += TNurbs->pTO->pB->CompC.N;

	Free_NurbsC_1DArray((NURBSC *)TNurbs->pTO->pB->CompC.pDE[0],curve_num);		// �g�����ʂ��\������S�Ă�NURBS�Ȑ��p�����[�^�̃������[���

	Free_NurbsS(TNurbs->pts);						// �g�����ʂ��\������NURBS�Ȗʃp�����[�^�̃������[���
	free(TNurbs->pts);								// �g�����ʂ��\������NURBS�Ȗʂ̃������[���

	Free_NurbsC(&TNurbs->pTO->pB->CompC.DegeNurbs);	// �g�����ʊO�����\�����镡���Ȑ����\������k�ޗpNURBS�Ȑ��̃������[���
	Free_CompC((COMPC *)TNurbs->pTO->pB);			// �g�����ʊO�����\�����镡���Ȑ����\������NURBS�Ȑ��̃������[���
	free(TNurbs->pTO->pB);							// �g�����ʊO�����\�����镡���Ȑ��̃������[���
	free(TNurbs->pTO);								// �g�����ʊO�����\������ʏ���̃������[���

	for(int i=0;i<TNurbs->n2;i++){
		Free_NurbsC(&TNurbs->pTI[i]->pB->CompC.DegeNurbs);	// �g�����ʓ������\�����镡���Ȑ����\������k�ޗpNURBS�Ȑ��̃������[���
		Free_CompC((COMPC *)TNurbs->pTI[i]->pB);	// �g�����ʓ������\�����镡���Ȑ����\������NURBS�Ȑ��̃������[���
		free(TNurbs->pTI[i]->pB);					// �g�����ʓ������\�����镡���Ȑ��̃������[���
	}
	Free_TrmS(TNurbs);								// �g�����ʃp�����[�^�̃������[���

	return TRUE;
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
		bs = CalcBSbasis(t,NurbsC->T,i,NurbsC->M);		// B�X�v���C�����֐������߂�
		bsw += bs*NurbsC->W[i];							// ����
		bscpw = AddCoord(bscpw,MulCoord(NurbsC->cp[i],bs*NurbsC->W[i]));	// ���q
	}
	
	p = DivCoord(bscpw,bsw);	// ���W�l�����߂�

	return p;
}


// �w�肵���m�b�gt�ł�NURBS�Ȗʂ̍��W�l�����߂�
Coord NURBS_Func::CalcNurbsSCoord(NURBSS *NurbsS,double div_u,double div_v)
{
	int i,j;
	double bs_u,bs_v;		// u,v����B�X�v���C�����֐�
	double bsw=0;			// ����
	Coord bscpw;			// ���q
	Coord p;

	InitCoord(&bscpw);		// ������

	for(i=0;i<NurbsS->K[0];i++){
		bs_u = CalcBSbasis(div_u,NurbsS->S,i,NurbsS->M[0]);			// u����B�X�v���C�����֐������߂�
		for(j=0;j<NurbsS->K[1];j++){
			bs_v = CalcBSbasis(div_v,NurbsS->T,j,NurbsS->M[1]);		// v����B�X�v���C�����֐������߂�
			bsw += bs_u*bs_v*NurbsS->W[i][j];
			bscpw = AddCoord(bscpw,MulCoord(NurbsS->cp[i][j],bs_u*bs_v*NurbsS->W[i][j]));
		}
	}

	p = DivCoord(bscpw,bsw);

	return p;
}

// B�X�v���C�����֐����v�Z���A�v�Z���ʂ�Ԃ�
// t:�m�b�g�@knot[]:�m�b�g�x�N�g��  span:���ڒ��̃R���g���[���|�C���g  order:�K��
double NURBS_Func::CalcBSbasis(double t, double knot[],int span,int order)
{
	double n1=0.0;
	double n2=0.0;

	// �K��(order)��1�̎�
	if( order == 1 ){
		if( knot[span] <= t && t <= knot[span+1] ){
			return 1.0;
		}
		else{
			return 0.0;
		}
	}

	// ����ȊO�̎�
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

// B�X�v���C�����֐���1�K�����W�������߂�
// t:�m�b�g�@knot[]:�m�b�g�x�N�g��  span:���ڒ��̃R���g���[���|�C���g  order:�K��
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
		bs = CalcBSbasis(t,NurbsC->T,i,NurbsC->M);
		diff_bs = CalcDiffBSbasis(t,NurbsC->T,i,NurbsC->M);

		Ft = AddCoord(Ft,MulCoord(NurbsC->cp[i],bs*NurbsC->W[i]));
		diff_Ft = AddCoord(diff_Ft,MulCoord(NurbsC->cp[i],diff_bs*NurbsC->W[i]));

		Gt += bs*NurbsC->W[i];
		diff_Gt += diff_bs*NurbsC->W[i];
	}

	// 1�K���������߂�
	p = SubCoord(DivCoord(diff_Ft,Gt),DivCoord(MulCoord(Ft,diff_Gt),Gt*Gt));

	return p;
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
		bs_u = CalcBSbasis(div_u,NurbsS->S,i,NurbsS->M[0]);				// u����B�X�v���C�����֐������߂�
		diff_bs_u = CalcDiffBSbasis(div_u,NurbsS->S,i,NurbsS->M[0]);	// u����B�X�v���C�����֐���1�K���������߂�
		for(j=0;j<NurbsS->K[1];j++){
			bs_v = CalcBSbasis(div_v,NurbsS->T,j,NurbsS->M[1]);			// v����B�X�v���C�����֐������߂�
			Ft = AddCoord(Ft,MulCoord(NurbsS->cp[i][j],bs_u*bs_v*NurbsS->W[i][j]));
			diff_Ft = AddCoord(diff_Ft,MulCoord(NurbsS->cp[i][j],diff_bs_u*bs_v*NurbsS->W[i][j]));
			Gt += bs_u*bs_v*NurbsS->W[i][j];
			diff_Gt += diff_bs_u*bs_v*NurbsS->W[i][j];
		}
	}

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
		bs_u = CalcBSbasis(div_u,NurbsS->S,i,NurbsS->M[0]);				// u����B�X�v���C�����֐������߂�
		for(j=0;j<NurbsS->K[1];j++){
			bs_v = CalcBSbasis(div_v,NurbsS->T,j,NurbsS->M[1]);				// v����B�X�v���C�����֐������߂�
			diff_bs_v = CalcDiffBSbasis(div_v,NurbsS->T,j,NurbsS->M[1]);	// v����B�X�v���C�����֐���1�K���������߂�
			Ft = AddCoord(Ft,MulCoord(NurbsS->cp[i][j],bs_u*bs_v*NurbsS->W[i][j]));
			diff_Ft = AddCoord(diff_Ft,MulCoord(NurbsS->cp[i][j],bs_u*diff_bs_v*NurbsS->W[i][j]));
			Gt += bs_u*bs_v*NurbsS->W[i][j];
			diff_Gt += bs_u*diff_bs_v*NurbsS->W[i][j];
		}
	}

	// 1�K���������߂�
	p = SubCoord(DivCoord(diff_Ft,Gt),DivCoord(MulCoord(Ft,diff_Gt),Gt*Gt));

	return p;
}

// NURBS�Ȗʏ��(u,v)�ɂ�����@���x�N�g�������Ƃ߂�
Coord NURBS_Func::CalcNormVecOnNurbsS(NURBSS *nurb,double u,double v)
{
	Coord a = CalcDiffuNurbsS(nurb,u,v);
	Coord b = CalcDiffvNurbsS(nurb,u,v);

	return(NormalizeVec(CalcOuterProduct(a,b)));
}

// 3���ȉ���NURBS�Ȗʂƕ��ʂƂ̌�_�Q��㐔��@�ɂċ��߂�(v�p�����[�^����)
// ����  *nurb:NURBS�Ȗ�  pt:���ʏ�̈�_  nvec:���ʂ̖@���x�N�g��  v_divnum:v�p�����[�^������
//       *ans:�Z�o���ꂽ��_��u,v�p�����[�^�l�����ꂼ��ans.x,ans.y�Ɋi�[,  ans_size:ans�̔z��
// �Ԓl  ��_�̌�(��_�̐���ans_size�𒴂����FERR)
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
			N[i] = CalcBSbasis(v_const,nurb->T,i,nurb->M[1]);		// v_const����B�X�v���C�����֐������߂�
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
				fprintf(stderr,"NURBS ERR:��_�̐����m�ۂ��ꂽ�z�񒷂𒴂��܂���\n");
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

// 3���ȉ���NURBS�Ȗʂƕ��ʂƂ̌�_�Q��㐔��@�ɂċ��߂�(u�p�����[�^����)
// ����  *nurb:NURBS�Ȗ�  pt:���ʏ�̈�_  nvec:���ʂ̖@���x�N�g��  u_divnum:u�p�����[�^������
//       *ans:�Z�o���ꂽ��_��u,v�p�����[�^�l�������ꂼ��ans.x,ans.y�Ɋi�[,  ans_size:ans�̔z��
// �Ԓl  ��_�̌�(��_�̐���ans_size�𒴂����FERR)
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
			N[i] = CalcBSbasis(u_const,nurb->S,i,nurb->M[0]);		// u_const����B�X�v���C�����֐������߂�
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
				fprintf(stderr,"NURBS ERR:��_�̐����m�ۂ��ꂽ�z�񒷂𒴂��܂���\n");
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

// 3���ȉ���NURBS�Ȗʂƕ��ʂƂ̌�_�Q����_�ǐՖ@�ɂċ��߂�
// �����@NURBSS:NURBS�Ȗ�  pt:���ʏ��1�_  nvec:���ʂ̖@���x�N�g��  ds:���(��_�Q)�̑e��(��0.1�`2�a)  ans:��  ans_size:���̐�
// �Ԓl�@FALSE:NURBS�Ȗʂƕ��ʂ��������Ă��Ȃ��@ERR:���ٓ_�܂��͔��U�ɂ�菈���𒆒f
int NURBS_Func::CalcIntersecPtsPlaneSearch3(NURBSS *nurb,Coord pt,Coord nvec,double ds,Coord *ans,int ans_size)
{
	int ans_count=0;		// �ǐՓ_�̑���
	int loop_count=0;		// �����v�Z�̃��[�v��
	int pnow=0;
	Coord init_pt[INTERSECPTNUMMAX];		// �����_(u,v�p�����[�^�l)
	Coord init_pt_Coord[INTERSECPTNUMMAX];	// �����_(x,y,z���W�l)
	bool  init_pt_flag[INTERSECPTNUMMAX];	// �e�����_��ʂ�I�������𔻕ʂ���t���O
	bool  init_allpt_flag=false;			// �����_��S�Ēʂ�I�������𔻕ʂ���t���O
	int   init_pt_num;						// �����_�̐�
	double u,v;								// ����ǐՒ���u,v�p�����[�^���Ԓl
	double d = CalcInnerProduct(pt,nvec);	// ���_���畽�ʂ܂ł̋���
	double dist;							// ���[�v�E�o�p�ɒǐՓ_�ԋ�����臒l�Ƃ���
	//FILE *fp=fopen("debug.csv","w");
	//double color[3] = {1,0,0};

	// �����_�ʉߔ��ʃt���O��������
	init_pt_flag[0] = true;
	for(int i=1;i<INTERSECPTNUMMAX;i++){
		init_pt_flag[i] = false;
	}

	// �܂�����ǐՖ@�̏����_�Ƃ��Č�_�����������߂�
	init_pt_num = CalcIntersecPtsPlaneV3(nurb,pt,nvec,10,init_pt,INTERSECPTNUMMAX);
	if(!init_pt_num){
		init_pt_num = CalcIntersecPtsPlaneV3(nurb,pt,nvec,100,init_pt,INTERSECPTNUMMAX);	// ��_��������Ȃ������ꍇ�͕��������ׂ������Ă�����x�T�[�`
	}
	if(!init_pt_num){
		init_pt_num = CalcIntersecPtsPlaneU3(nurb,pt,nvec,10,init_pt,INTERSECPTNUMMAX);		// ����ł���_��������Ȃ������ꍇ�͕�����ς��ăT�[�`
	}
	if(!init_pt_num){
		init_pt_num = CalcIntersecPtsPlaneU3(nurb,pt,nvec,100,init_pt,INTERSECPTNUMMAX);	// ����ł���_��������Ȃ������ꍇ�͕��������ׂ������Ă�����x�T�[�`
	}
	if(!init_pt_num){		// ����ł�������Ȃ��ꍇ�́A�������Ă��Ȃ��Ƃ݂Ȃ�
		fprintf(stderr,"NURBS ERROR:��_�������܂���ł���\n");
		return FALSE;					
	}
	else if(init_pt_num == ERR) return ERR;			// init_pt_num��init_pt�̔z�񒷂𒴂���

	for(int i=0;i<init_pt_num;i++){
		init_pt_Coord[i] = CalcNurbsSCoord(nurb,init_pt[i].x,init_pt[i].y);		// ��_��uv�p�����[�^��xyz���W�l�ɕϊ��������̂�ێ����Ă���
		//fprintf(fp,"%lf,%lf\n",init_pt_Coord[i]);
		//DrawPoint(init_pt_Coord[i],1,3,color);
	}

	ans[ans_count] = SetCoord(init_pt[0]);
	ans_count++;

	// �����_��S�Ēʉ߂���܂Ō���ǐՖ@���J��Ԃ�
	while(init_allpt_flag == false){

		// ����ǐՂ̂��߂̎n�_(u,v)���Z�b�g
		u = ans[ans_count].x = init_pt[pnow].x;
		v = ans[ans_count].y = init_pt[pnow].y;
		init_pt_flag[pnow] = true;

		// ����ǐՊJ�n
		loop_count = 0;
		while(1){
			// �܂������l�Ƃ��Ă�du,dv�����߂�
			Coord pu = CalcDiffuNurbsS(nurb,u,v);
			Coord pv = CalcDiffvNurbsS(nurb,u,v);
			double phi = CalcInnerProduct(nvec,CalcNurbsSCoord(nurb,u,v));
			double phi_u = CalcInnerProduct(nvec,pu);
			double phi_v = CalcInnerProduct(nvec,pv);
			double E = CalcInnerProduct(pu,pu);
			double F = CalcInnerProduct(pu,pv);
			double G = CalcInnerProduct(pv,pv);
			double f = sqrt(E*phi_v*phi_v - 2*F*phi_u*phi_v + G*phi_u*phi_u); 
			if(!f){			// ���ٓ_
				fprintf(stderr,"NURBS ERROR:���ٓ_���o�ɂ�菈���𒆒f���܂�\n");
				return ERR;				
			}
			double f1 = 1/f;
			double f2 = -f1;
			double du = -f2*phi_v*ds;		// �����l
			double dv = f2*phi_u*ds;		// �����l

			// �j���[�g���@��p����u,v��^�l�Ɏ���������
			int k=0;
			if(fabs(dv) > fabs(du)){				// dv>du�̏ꍇ��dv��萔�Ƃ��ČŒ肷��
				while(fabs(du) > CONVERG_GAP){		// du����������܂ŌJ��Ԃ��v�Z
					phi = CalcInnerProduct(nvec,CalcNurbsSCoord(nurb,u,v));
					phi_u = CalcInnerProduct(nvec,CalcDiffuNurbsS(nurb,u,v));
					phi_v = CalcInnerProduct(nvec,CalcDiffvNurbsS(nurb,u,v));
					du = (d-phi-phi_v*dv)/phi_u;
					u += du;
					if(u < nurb->U[0] || u > nurb->U[1] || k > LOOPCOUNTMAX){
						fprintf(stderr,"NURBS ERROR:�����v�Z�Ɏ��s���܂���\n");
						return ERR;
					}
					k++;
				}
				v += dv;
			}
			else{									// dv<du�̏ꍇ��du��萔�Ƃ��ČŒ肷��
				while(fabs(dv) > CONVERG_GAP){		// dv����������܂ŌJ��Ԃ��v�Z
					phi = CalcInnerProduct(nvec,CalcNurbsSCoord(nurb,u,v));
					phi_u = CalcInnerProduct(nvec,CalcDiffuNurbsS(nurb,u,v));
					phi_v = CalcInnerProduct(nvec,CalcDiffvNurbsS(nurb,u,v));
					dv = (d-phi-phi_u*du)/phi_v;
					v += dv;
					if(v < nurb->V[0] || v > nurb->V[1] || k>LOOPCOUNTMAX){
						fprintf(stderr,"NURBS ERROR:�����v�Z�Ɏ��s���܂���\n");
						return ERR;
					}
					k++;
				}
				u += du;
			}

			Coord p = CalcNurbsSCoord(nurb,u,v);			// ����ꂽu,v��xyz���W�l�ɕϊ�
			dist = CalcEuclidPnts3D(init_pt_Coord[pnow],p);	// ����ꂽxyz���W�l�Ə����_�Ƃ̋������Z�o

			//fprintf(fp,"%d,%lf,%lf,%lf,%lf\n",ans_count,p.x,p.y,u,v);

			ans_count++;		// ��_�Q�̐����C���N�������g

			if(ans_count == ans_size){
				fprintf(stderr,"NURBS ERROR:��_�̐����m�ۂ��ꂽ�z�񒷂𒴂��܂���\n");
				return ans_count;
			}

			// �ŏ��ɋ��߂������_������ǐՖ@�ɂ���đS�Ēʉ߂��������ׂ�
			for(int i=0;i<init_pt_num;i++){
				if(init_pt_flag[i] == false){
					if(CalcEuclidPnts3D(init_pt_Coord[i],p) < ds)
						init_pt_flag[i] = true;
					else
						init_pt_flag[i] = false;
				}
			}

			// u,v����蓾��p�����[�^�͈́i0�`1�j�𒴂����ꍇ�܂��́A�P�����Ė߂��Ă����ꍇ�̓��[�v�𔲂���
			if(u < nurb->U[0] || u > nurb->U[1] || v < nurb->V[0] || v > nurb->V[1] || (dist < ds/2 && loop_count > 0))
				break;

			// ����ꂽu,v�����(��_�Q)�Ƃ��ēo�^
			ans[ans_count].x = u;
			ans[ans_count].y = v;

			loop_count++;		// ���[�v�񐔂��C���N�������g

		}// ����ǐՂ����܂�

		// �c�����_������΁A�ʂ̌��������̂ŁA���̓_���n�_�Ƃ��čēx����ǐՂ��s��
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
		fprintf(stderr,"NURBS ERROR: CalcIntersecPlane3() \n");
		return ERR;
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
			fprintf(stderr,"NURBS ERROR:�������Ή�(���� = %d)\n",nurb->M-1);
			goto EXIT;
		}
		GetNurbsCCoef(nurb,coef,i,P,Q);						// NURBS�Ȑ��̌W��(P,Q)�����߂�
		GetIntersecEquation(nurb->M,P,Q,pt,nvec,a);			// NURBS�Ȑ��ƕ��ʂ̌�����o�p�������𓾂�
		ansnum = CalcEquation(a,t,nurb->M-1);					// �������������A��_�̃p�����[�^�l�𓾂�

		for(int j=0;j<ansnum;j++){
			if(t[j] >= nurb->T[i+nurb->M-1] && t[j] <= nurb->T[i+nurb->M]){	// �m�b�g�x�N�g���̒l�ƓK��������̂̂݉��Ƃ��Ē��o
				if(k == ans_size){
					fprintf(stderr,"NURBS ERROR:��_�̐����m�ۂ��ꂽ�z�񒷂𒴂��܂���\n");
					goto EXIT;
				}
				ans[k] = t[j];		// �����擾
				k++;				// ���̐����C���N�������g
			}
		}
	}

	return k;

EXIT:
	FreeMatrix(coef);
	return ERR;
}

// 3���������܂ł�����
int NURBS_Func::CalcEquation(double *a,double *t,int M)
{
	int flag;

	if(M == 3)		flag = CalcCubicEquation(a,t);
	else if(M == 2)	flag = CalcQuadraticEquation(a,t);
	else if(M == 1) flag = CalcLinearEquation(a,t);
	else			return ERR;

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
	
	return TRUE;
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

	return TRUE;
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

	return TRUE;
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

	return TRUE;
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
		fprintf(stderr,"NURBS ERROR:�R���g���[���|�C���g�̐����قȂ�܂�\n");
		return ERR;
	}

	for(int i=0;i<Nurbs.K[0];i++){
		for(int j=0;j<Nurbs.K[1];j++){
			nurbs->cp[i][j] = SetCoord(Nurbs.cp[i][j]);
		}
	}

	return TRUE;
}