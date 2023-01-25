#include "stdafx.h"
#include "BODY.h"
#include "NURBS_Func.h"
#include "Quaternion.h"

#if defined(_DEBUG) && defined(_MSC_VER)
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////////////////////
// Function: CalcBSbasis
// B�X�v���C�����֐����v�Z���A�v�Z���ʂ�Ԃ�
static	double CalcBSbasis(double,double [],int,int,int);			

// Function: CalcDiffBSbasis
// B�X�v���C�����֐���1�K�����W�������߂�
static	double CalcDiffBSbasis(double,double [],int,int,int);		

// Function: CalcDiffBSbasisN
// B�X�v���C�����֐���N�K�����W�������߂�
static	double CalcDiffBSbasisN(double,double [],int,int,int,int);	

// Function: GetBSplCoef3
// 3����B�X�v���C���Ȑ��̊e�W�������߂�@(at^3 + bt^2 + ct + d�̌W��a,b,c,d��Ԃ�)
static	int GetBSplCoef3(int,int,int,double *,double **);			

// Function: GetBSplCoef2
// 2����B�X�v���C���Ȑ��̊e�W�������߂�@(at^2 + bt + c�̌W��a,b,c��Ԃ�)
static	int GetBSplCoef2(int,int,int,double *,double **);			

// Function: GetBSplCoef1
// 1����B�X�v���C���Ȑ��̊e�W�������߂�@(at + b�̌W��a,b��Ԃ�)
static	int GetBSplCoef1(int,int,int,double *,double **);	

// Function: TrimNurbsSPlaneSub1
// (private)TrimNurbsSPlane�̃T�u�֐�(2�����̌�_�����Ƃ߂�)
static	Coord TrimNurbsSPlaneSub1(double,double,double,double,double,double); 

// Function: GetIntersecEquation
// (private)NURBS�Ȑ��ƕ��ʂ̌�����o�p3���������𓾂�
static	void GetIntersecEquation(int,Coord *,double *,Coord,Coord,double *);		

// Function: CalcEquation
// (private)3���������܂ł𔻕ʂ��ĉ���
static	int CalcEquation(double *,double *,int);					

// Function: GetNurbsSCoef
// (private)NURBS�Ȗʂɂ�����u�܂���v���Œ肵���ꍇ�ɓ�����NURBS�Ȑ�C(u) or C(v)�̕��ꕪ�q�̌W�������߂�
static	void GetNurbsSCoef(int,double **,double *,Coord *,int,Coord *,double *);	

// Function: GetMinDistance
// (private)�ŏ������������W�l��Ԃ�
static	Coord GetMinDistance(Coord,Coord *,int);						

// Function: CheckClossedPoints
// (private)�w�肵���_������2�_��Ίp�Ƃ��闧���̂̒��ɑ��݂��邩�𒲂ׂ�
static	int CheckClossedPoints(Coord,Coord,Coord);				

// Function: ChangeKnotVecRange
// �m�b�g�x�N�g���̃p�����[�^��`���ύX����
static	void ChangeKnotVecRange(Vector,int,int,int,double,double);	

// Function: CalcApproximationCP_LSM
// (private)�ŏ�2��@�ŋߎ��R���g���[���|�C���g�����߂�
static	void CalcApproximationCP_LSM(Coord *,Vector,Vector,int,int,int,int,Coord *);	

// Function: GetEqIntervalKont
// �Ȑ�/�Ȗʃp�����[�^���瓙�Ԋu�ȃm�b�g�x�N�g�����Z�o
static	void GetEqIntervalKont(int,int,Vector);						

// Function: SetApproximationCPnum
// (private)�_�񐔂��琶������R���g���[���|�C���g�����Z�肷��
static	int SetApproximationCPnum(int);									

// Function: GetApproximatedKnot
// (private)�Ȑ�/�Ȗʃp�����[�^����ߎ��p�m�b�g�x�N�g�����Z�o
static	void GetApproximatedKnot(Vector,int,int,int,Vector);			

// Function: GetInterpolatedKnot
// (private)�Ȑ�/�Ȗʃp�����[�^�����ԗp�m�b�g�x�N�g�����Z�o
static	void GetInterpolatedKnot(Vector,int,int,int,Vector);			

// Function: GetSurfaceKnotParam
// (private)�e�ʉߓ_�̋Ȗʃp�����[�^���Z�o
static	void GetSurfaceKnotParam(Vector,Vector,Coord **,int,int);		

// Function: GetCurveKnotParam1
// (private)�e�ʉߓ_�̋Ȑ��p�����[�^���Z�o(�R�[�h���̔䂩��Z�o)
static	void GetCurveKnotParam1(Coord *,int,Vector);					

// Function: GetCurveKnotParam2
// (private)�e�ʉߓ_�̋Ȑ��p�����[�^���Z�o(�R�[�h���̕������̔䂩��Z�o)
static	void GetCurveKnotParam2(Coord *,int,Vector);					

//////////////////////////////////////////////////////////////////////////////////////
// Function: New_NurbsC
// Nurbs�Ȑ��̃������[�m��
//
// Parameters: 
// *nurb - �������[�m�ۂ���Nurbs�Ȑ��ւ̃|�C���^
// K - �R���g���[���|�C���g�̐�
// N - �m�b�g�x�N�g���̐�
//
// Return:
// �����FKOD_TRUE, ���s�FKOD_ERR
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
// NURBS�Ȑ��z��̃������[���
//
// Parameters: 
// *a - �������[���������Nurbs�Ȑ��ւ̃|�C���^
// num - �������[���������NURBS�Ȑ��̐�
void NURBS_Func::Free_NurbsC_1DArray(NURBSC *a,int num)
{
	for(int i=0;i<num;i++)
		Free_NurbsC(&a[i]);
}

// Function: Free_NurbsC
// 1�{��NURBS�Ȑ��̃������[���
//
// Parameters: 
// *a - �������[���������Nurbs�Ȑ��ւ̃|�C���^
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
// NURBS�Ȗʂ̃������[�m��
//
// Parameters: 
// *nurb - �������[�m�ۂ���Nurbs�Ȗʂւ̃|�C���^
// K[2] - u,v�R���g���[���|�C���g�̐�
// N[2] - u,v�m�b�g�x�N�g���̐�
//
// Return:
// �����FKOD_TRUE, ���s�FKOD_ERR
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
// NURBS�Ȗʔz��̃������[���
// 
// Parameters:
// *a - �������[���������Nurbs�Ȗʂւ̃|�C���^
// num - �������[���������NURBS�Ȗʂ̐�
void NURBS_Func::Free_NurbsS_1DArray(NURBSS *a,int num)
{
	for(int i=0;i<num;i++)
		Free_NurbsS(&a[i]);
}


// Function: Free_NurbsS
// 1��NURBS�Ȗʂ̃������[���
//
// Parameters: 
// *a - �������[���������Nurbs�Ȗʂւ̃|�C���^
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
// �g�����ʂ̃������[�m��
//
// Parameters: 
// *trms - �������[�m�ۂ���g�����ʂւ̃|�C���^
// num - �������[�m�ۂ���g�����ʂ̐�
//
// return:
// �����FKOD_TRUE, ���s�FKOD_ERR
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
// �g�����ʔz��̃������[���
//
// Parameters: 
// *a - �������[���������g�����ʔz��ւ̃|�C���^
// num - �������[���������g�����ʂ̐�
void NURBS_Func::Free_TrmS_1DArray(TRMS *a,int num)
{
	for(int i=0;i<num;i++)
		Free_TrmS(&a[i]);
}

// Function: Free_TrmS
// �g�����ʂ̃������[���
//
// Parameters: 
// *a - �������[���������g�����ʂւ̃|�C���^
void NURBS_Func::Free_TrmS(TRMS *a)
{
//	free(a->pTI);
	if ( a->pTI ) {
		delete[]	a->pTI;
		a->pTI = NULL;
	}
}

// Function: New_CompC
// �����Ȑ��̃������[�m��
//
// Parameters: 
// *compc - �������[�m�ۂ��镡���Ȑ��ւ̃|�C���^
// num - �������[�m�ۂ��镡���Ȑ��̐�
//
// return:
// �����FKOD_TRUE, ���s�FKOD_ERR
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
// �����Ȑ��z��̃������[���
//
// Parameters:
// *a - �������[��������镡���Ȑ��z��ւ̃|�C���^
// num - �������[��������镡���Ȑ��̐�
void NURBS_Func::Free_CompC_1DArray(COMPC *a,int num)
{
	for(int i=0;i<num;i++)
		Free_CompC(&a[i]);
}

// Function: Free_CompC
// �����Ȑ��̃������[���
//
// Parameters:
// *a - �������[��������镡���Ȑ��ւ̃|�C���^
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
// 1��Nurbs�Ȑ��𐶐�����
//
// Parameters:
// *Nurbs - ��������NURBS�Ȑ��ւ̃|�C���^
// K - �R���g���[���|�C���g�̐�
// M - �K��
// N - �m�b�g�x�N�g���̐�
// T[] - �m�b�g�x�N�g����
// W[] - �E�F�C�g��
// cp[] - �R���g���[���|�C���g��
// V[2] - �m�b�g�x�N�g���͈̔�
// prop[4] - �v���p�e�B(BODY.h�Q��)
// euflag - �f�B���N�g���� Entity Use Flag �̒l(0:�􉽗v�f 5:2D�p�����g���b�N�v�f)
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
// 1��Nurbs�Ȑ��𐶐�����(NURBS�Ȑ��̃R�s�[)(�I�[�o�[���[�h)
//
// Parameters:
// *Nurbs - �V���ɐ�������NURBS�Ȑ�
// nurb - �����
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
// GenNurbsC()�ɂ���Đ������ꂽNURBS�Ȑ����폜����
// 
// Parameters:
// *Nurbs - �V���ɐ�������NURBS�Ȑ��ւ̃|�C���^
void NURBS_Func::DelNurbsC(NURBSC *Nurbs)
{
	NURBS_Func hbody;
	hbody.Free_NurbsC(Nurbs);
}

// Function: GenNurbsS
// 1��NURBS�Ȗʂ𐶐�����
//
// Parameters:
// Mu,Mv - �K��
// Ku,Kv - �R���g���[���|�C���g�̐�  
// *S,*T - u,v�����m�b�g�x�N�g����
// **W - �E�G�C�g
// **Cp - �R���g���[���|�C���g  
// U_s,U_e,V_s,V_e - u�����m�b�g�x�N�g���̊J�n�l,�I���l
// 
// return:
// �����FKOD_TRUE, ���s�FKOD_ERR
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
// 1��Nurbs�Ȗʂ𐶐�����(NURBS�Ȗʂ̃R�s�[)
//
// Parameters:
// *Nurbs - �V���ɐ�������NURBS�Ȗʂւ̃|�C���^
// nurb - �����
// 
// return:
// �����FKOD_TRUE, ���s�FKOD_ERR
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

	// �������[�m��
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
// NurbsC�����_��ʂ�Axis����deg������]��������]�T�[�t�F�XNurbsS�𐶐�����
//
// Parameter:
// *NurbsS - ����������]�T�[�t�F�X(NURBS�Ȗ�)�ւ̃|�C���^
// NurbsC - ����ƂȂ�NURBS�Ȑ�
// Axis - ��]���x�N�g��
// deg - ��]�p�x�ideg)
//
// Return:
// �����FKOD_TRUE, ���s�FKOD_ERR
//int NURBS_Func::GenRotNurbsS(NURBSS *NurbsS,NURBSC NurbsC,Coord Axis,double deg)
int NURBS_Func::GenRotNurbsS(NURBSS *NurbsS,const NURBSC& NurbsC,Coord Axis,double deg)
{
    Axis = NormalizeVec(Axis);		// ���K��

    // ��]�p�x�ɂ���āC�����̃Z�O�����g�ŉ~�ʂ𐶐����邩���f����
    // ��]�p�x��180�x�����̏ꍇ�C1�Z�O�����g�ŉ~�ʂ�\������
    if(fabs(deg) < 180 ){
        double S[6] = {0,0,0,1,1,1};	// u�����m�b�g�x�N�g��
        double **W;	// �E�G�C�g
        Coord **Cp;	// �R���g���[���|�C���g
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
                Coord Q_ = CalcRotVec(NurbsC.cp[j],Axis,(double)i*rad/2);	// ���X��NURBS�Ȑ���̃R���g���[���|�C���g��Axis�����0,deg/2,deg�x��]
                Coord P = CalcNormalLine(NurbsC.cp[j],SetCoord(0,0,0),Axis);	// Axis��̉�]���S�̍��W
                Coord PQ_ = SubCoord(Q_,P);	// PQ_�x�N�g���𐶐�
                if(i%2 == 0){		// i=0,2�̂Ƃ�
                    W[i][j] = NurbsC.W[j];
                    Cp[i][j] = SetCoord(Q_);
                }
                else{
                    W[i][j] = NurbsC.W[j]*cos(rad/2);
                    Cp[i][j] = AddCoord(MulCoord(PQ_,1/cos(rad/2)),P);
                }
            }
        }
		GenNurbsS(NurbsS,3,NurbsC.M,3,NurbsC.K,S,NurbsC.T,W,Cp,0,1,0,1);		// NURBS�Ȗʐ���
//		GenNurbsS(NurbsS,3,NurbsC.M,3,NurbsC.K,S,NurbsC.T.get(),W,Cp,0,1,0,1);		// NURBS�Ȗʐ���

        FreeMatrix(W,3);
        FreeCoord2(Cp,3);
    }

    // ��]�p�x��270�����̏ꍇ�C2�Z�O�����g�ŉ~�ʂ�\������
    else if(fabs(deg) < 270){
        double S[8] = {0,0,0,0.5,0.5,1,1,1};
        double **W;	// �E�G�C�g
        Coord **Cp;	// �R���g���[���|�C���g
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
                Coord Q_ = CalcRotVec(NurbsC.cp[j],Axis,(double)i*rad/4);	// ���X��NURBS�Ȑ���̃R���g���[���|�C���g��Axis�����0,deg/2,deg�x��]
                Coord P = CalcNormalLine(NurbsC.cp[j],SetCoord(0,0,0),Axis);	// Axis��̉�]���S�̍��W
                Coord PQ_ = SubCoord(Q_,P);	// PQ_�x�N�g���𐶐�
                if(i%2 ==  1){	// i=1,3�̂Ƃ�
                    W[i][j] = NurbsC.W[j]*cos(rad/4);
                    Cp[i][j] = AddCoord(MulCoord(PQ_,1/cos(rad/4)),P);
                }
                else{		// i=0,2,4�̂Ƃ�
                    W[i][j] = NurbsC.W[j];
                    Cp[i][j] = SetCoord(Q_);
                }
            }
        }
		GenNurbsS(NurbsS,3,NurbsC.M,5,NurbsC.K,S,NurbsC.T,W,Cp,0,1,0,1);		// NURBS�Ȗʐ���
//		GenNurbsS(NurbsS,3,NurbsC.M,5,NurbsC.K,S,NurbsC.T.get(),W,Cp,0,1,0,1);		// NURBS�Ȗʐ���

        FreeMatrix(W,5);
        FreeCoord2(Cp,5);
    }

    // ��]�p�x��360�x�����̏ꍇ�C3�Z�O�����g�ŉ~�ʂ�\������
    else if(fabs(deg) < 360){
        double S[10] = {0,0,0,0.33,0.33,0.66,0.66,1,1,1};
        double **W;	// �E�G�C�g
        Coord **Cp;	// �R���g���[���|�C���g
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
                Coord Q_ = CalcRotVec(NurbsC.cp[j],Axis,(double)i*rad/6);	// ���X��NURBS�Ȑ���̃R���g���[���|�C���g��Axis�����0,deg/2,deg�x��]
                Coord P = CalcNormalLine(NurbsC.cp[j],SetCoord(0,0,0),Axis);	// Axis��̉�]���S�̍��W
                Coord PQ_ = SubCoord(Q_,P);	// PQ_�x�N�g���𐶐�
                if(i%2 ==  0){	// i=0,2,4,6�̂Ƃ�
                    W[i][j] = NurbsC.W[j];
                    Cp[i][j] = SetCoord(Q_);
                }
                else{		// i=1,3,5�̂Ƃ�
                    W[i][j] = NurbsC.W[j]*cos(rad/6);
                    Cp[i][j] = AddCoord(MulCoord(PQ_,1/cos(rad/6)),P);
                }
            }
        }
		GenNurbsS(NurbsS,3,NurbsC.M,7,NurbsC.K,S,NurbsC.T,W,Cp,0,1,0,1);		// NURBS�Ȗʐ���
//		GenNurbsS(NurbsS,3,NurbsC.M,7,NurbsC.K,S,NurbsC.T.get(),W,Cp,0,1,0,1);		// NURBS�Ȗʐ���
#ifdef _DEBUG
		NurbsS->DebugForNurbsS();
#endif
        FreeMatrix(W,7);
        FreeCoord2(Cp,7);
    }
    // 360�x�ȏ�
    else{
        // NurbsS�𐶐�
        double S[12] = {0,0,0,0.25,0.25,0.5,0.5,0.75,0.75,1,1,1};		// u�����m�b�g�x�N�g��
        double **W;			// �E�G�C�g
        Coord  **Cp;		// �R���g���[���|�C���g
        if((W = NewMatrix(9,NurbsC.K)) == NULL){
            GuiIF.SetMessage("NURBS_Func KOD_ERROR:fail to allocate memory");
            return KOD_ERR;
        }
        if((Cp = NewCoord2(9,NurbsC.K)) == NULL){
            GuiIF.SetMessage("NURBS_Func KOD_ERROR:fail to allocate memory");
            FreeMatrix(W,9);
            return KOD_ERR;
        }
        for(int i=0;i<9;i++){		// u����
            for(int j=0;j<NurbsC.K;j++){		// v����
                Coord Q_ = CalcRotVec(NurbsC.cp[j],Axis,(double)i*PAI/4);		// ���X��NURBS�Ȑ���̃R���g���[���|�C���g��Axis�����45�x��]
                Coord P = CalcNormalLine(NurbsC.cp[j],SetCoord(0,0,0),Axis);	// Axis��̉�]���S�̍��W
                Coord PQ_ = SubCoord(Q_,P);										// PQ_�x�N�g���𐶐�
                if(i%2 == 0){													// i=0,2,4,6�̂Ƃ�
                    W[i][j] = NurbsC.W[j];										// �E�G�C�g
                    Cp[i][j] = SetCoord(Q_);									// Q_�����̂܂܃R���g���[���|�C���g�ɂȂ�
                }
                else{															// i=1,3,5,7�̂Ƃ�
                    W[i][j] = NurbsC.W[j]*cos(PAI/4);							// �E�G�C�g�v�Z
                    Cp[i][j] = AddCoord(MulCoord(PQ_,1/cos(PAI/4)),P);			// �R���g���[���|�C���g�v�Z
                }
            }
        }

		GenNurbsS(NurbsS,3,NurbsC.M,9,NurbsC.K,S,NurbsC.T,W,Cp,0,1,0,1);		// NURBS�Ȗʐ���
//		GenNurbsS(NurbsS,3,NurbsC.M,9,NurbsC.K,S,NurbsC.T.get(),W,Cp,0,1,0,1);		// NURBS�Ȗʐ���

        FreeMatrix(W,9);
        FreeCoord2(Cp,9);
    }
    return KOD_TRUE;
}

// Function: GenSweepNurbsS
// 1��NURBS�Ȑ����炠�鎲�����ɂ��鋗�������X�C�[�v�������X�C�[�v�T�[�t�F�X�𐶐�����
//
// Parameters:
// *NurbsS - ���������X�C�[�v�T�[�t�F�X(NURBS�Ȗ�)�ւ̃|�C���^
// NurbsC - ����ƂȂ�NURBS�Ȑ�
// Axis - �X�C�[�v�����x�N�g��
// Len - �X�C�[�v����
//
// Return:
// �����FKOD_TRUE, ���s�FKOD_ERR
//int NURBS_Func::GenSweepNurbsS(NURBSS *NurbsS,NURBSC NurbsC,Coord Axis,double Len)
int NURBS_Func::GenSweepNurbsS(NURBSS *NurbsS,const NURBSC& NurbsC,Coord Axis,double Len)
{
	Axis = NormalizeVec(Axis);		// ���K��

	// NurbsS�𐶐�
	double T[4] = {0,0,1,1};		// v�����m�b�g�x�N�g��
	double **W;			// �E�G�C�g
	Coord  **Cp;		// �R���g���[���|�C���g
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
			W[i][j] = NurbsC.W[i];	// �E�G�C�g�v�Z
			if(j==0)
				Cp[i][j] = SetCoord(NurbsC.cp[i]);		// �R���g���[���|�C���g�v�Z
			else
				Cp[i][j] = SetCoord(AddCoord(NurbsC.cp[i],MulCoord(Axis,Len)));		// �R���g���[���|�C���g�v�Z
		}
	}

	GenNurbsS(NurbsS,NurbsC.M,2,NurbsC.K,2,NurbsC.T,T,W,Cp,0,1,NurbsC.V[0],NurbsC.V[1]);	// NURBS�Ȗʐ���
//	GenNurbsS(NurbsS,NurbsC.M,2,NurbsC.K,2,NurbsC.T.get(),T,W,Cp,0,1,NurbsC.V[0],NurbsC.V[1]);	// NURBS�Ȗʐ���

	FreeMatrix(W,2);
	FreeCoord2(Cp,2);

	return KOD_TRUE;
}

// Function: GenIsoparamCurveU
// NURBS�Ȗʏ��u�����p�����[�^�l���Œ肵���Ƃ��̃A�C�\�p�����g���b�NNURBS�Ȑ��𐶐�
//
// Parameters:
// *P - �A�C�\�p�����g���b�N�Ȑ���������NURBS�Ȗ�   
// u - u�����̌Œ�p�����[�^   
// *C - �������ꂽ�A�C�\�p�����g���b�N�Ȑ�
//
// Return:
// �����FKOD_TRUE, ���s�FKOD_ERR�i����u��*P��u�p�����[�^�͈͊O�j
int NURBS_Func::GenIsoparamCurveU(NURBSS *P,double u,NURBSC *C)
{
	if(u < P->U[0] || u > P->U[1])	return KOD_ERR;

	double V[2] = {P->V[0],P->V[1]};	// �m�b�g�x�N�g���͈̔�
	int prop[4] = {0,0,1,0};		// �p�����[�^

	Coord *Q = NewCoord1(P->K[1]);	// �R���g���[���|�C���g
	double *W = NewVector(P->K[1]);	// �E�F�C�g

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
// NURBS�Ȗʏ��v�����p�����[�^�l���Œ肵���Ƃ��̃A�C�\�p�����g���b�NNURBS�Ȑ��𐶐�
//
// Parameters:
// *S - �A�C�\�p�����g���b�N�Ȑ���������NURBS�Ȗ�   
// v - v�����̌Œ�p�����[�^   
// *C - �������ꂽ�A�C�\�p�����g���b�N�Ȑ�
//
// Return:
// �����FKOD_TRUE, ���s�FKOD_ERR�i����v��*P��u�p�����[�^�͈͊O�j
int NURBS_Func::GenIsoparamCurveV(NURBSS *P,double v,NURBSC *C)
{
	if(v < P->V[0] || v > P->V[1])	return KOD_ERR;

	double V[2] = {P->U[0],P->U[1]};	// �m�b�g�x�N�g���͈̔�
	int prop[4] = {0,0,1,0};		// �p�����[�^

	Coord *Q = NewCoord1(P->K[0]);	// �R���g���[���|�C���g
	double *W = NewVector(P->K[0]);	// �E�F�C�g

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
// GenNurbsS()�ɂ���Đ������ꂽNURBS�Ȗʂ��폜����
// 
// Parameters:
// *Nurbs - �폜����NURBS�Ȗʂւ̃|�C���^
void NURBS_Func::DelNurbsS(NURBSS *Nurbs)
{
	NURBS_Func hbody;
	hbody.Free_NurbsS(Nurbs);
}

// Function: GenTrimdNurbsS
// �g�����ʂ�L����NURBS�Ȗʂ��R�s�[����
//
// Parameters:
// *TNurbs - ���������g�����ʂւ̃|�C���^
// tnurb - �R�s�[���̃g������
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

//	nurbsS = (NURBSS *)malloc(sizeof(NURBSS));		// NURBS�Ȗʂ̃������[�m��
//	conps_o = (CONPS *)malloc(sizeof(CONPS));		// �O���g�������\������ʏ���̃������[�m��
//	compc_o = (COMPC *)malloc(sizeof(COMPC));		// �O���g�������\�����镡���Ȑ��̃������[�m��
	nurbsS = new NURBSS[1];		// NURBS�Ȗʂ̃������[�m��
	conps_o = new CONPS[1];		// �O���g�������\������ʏ���̃������[�m��
	compc_o = new COMPC[1];		// �O���g�������\�����镡���Ȑ��̃������[�m��

	// �g�����ʂ��\������NURBS�Ȑ��̑������J�E���g
	for(int i=0;i<tnurb.n2;i++){
//		for(int j=0;j<tnurb.pTI[i]->pB->CompC.N;j++){
		for(int j=0;j<tnurb.pTI[i]->pB.CompC->N;j++){
			curve_num++;
		}
	}
//	curve_num += tnurb.pTO->pB->CompC.N;
	curve_num += tnurb.pTO->pB.CompC->N;

//	nurbsC = (NURBSC *)malloc(sizeof(NURBSC)*curve_num);	// �g�����ʂ��\������NURBS�Ȑ��̐�����NURBS�Ȑ��̃������[���m��
	nurbsC = new NURBSC[curve_num];	// �g�����ʂ��\������NURBS�Ȑ��̐�����NURBS�Ȑ��̃������[���m��

	GenNurbsS(nurbsS,*tnurb.pts);							// �V����NURBS�Ȗʂ�1����
	TNurbs->pts = nurbsS;									// NURBS�Ȗʂ��g�����ʂɊ֘A�t����

	New_TrmS(TNurbs,tnurb.n2);						// �g�����ʂ̃������[�m��

//	conps_i = (CONPS *)malloc(sizeof(CONPS)*tnurb.n2);		// �������\������ʏ���̃������[�m��
//	compc_i = (COMPC *)malloc(sizeof(COMPC)*tnurb.n2);		// �������\�����镡���Ȑ��̃������[�m��
	conps_i = new CONPS[tnurb.n2];		// �������\������ʏ���̃������[�m��
	compc_i = new COMPC[tnurb.n2];		// �������\�����镡���Ȑ��̃������[�m��

	// NURBS�Ȑ����g�����������\������NURBS�Ȑ��Ɋ֘A�t����
	// �O���g����
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

	// �����g����
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
// GenTrimdNurbsS()�ɂ���Đ������ꂽ�g�����ʂ��폜����
//
// Parameters:
// *TNurbs - �폜����g�����ʂւ̃|�C���^
//
// Return:
// KOD_TRUE
int NURBS_Func::DelTrimdNurbsS(TRIMD_NURBSS *TNurbs)
{
	NURBS_Func hbody;
	int curve_num = 0;

	// �g�����ʂ��\������S�Ă�NURBS�Ȑ��̖{���𒲂ׂ�
	for(int i=0;i<TNurbs->n2;i++){
//		for(int j=0;j<TNurbs->pTI[i]->pB->CompC.N;j++){
		for(int j=0;j<TNurbs->pTI[i]->pB.CompC->N;j++){
			curve_num++;
		}
	}
//	curve_num += TNurbs->pTO->pB->CompC.N;
	curve_num += TNurbs->pTO->pB.CompC->N;

//	hbody.Free_NurbsC_1DArray((NURBSC *)TNurbs->pTO->pB->CompC.pDE[0],curve_num);		// �g�����ʂ��\������S�Ă�NURBS�Ȑ��p�����[�^�̃������[���
	hbody.Free_NurbsC_1DArray(TNurbs->pTO->pB.CompC->pDE[0].NurbsC,curve_num);		// �g�����ʂ��\������S�Ă�NURBS�Ȑ��p�����[�^�̃������[���

	hbody.Free_NurbsS(TNurbs->pts);						// �g�����ʂ��\������NURBS�Ȗʃp�����[�^�̃������[���
//	free(TNurbs->pts);								// �g�����ʂ��\������NURBS�Ȗʂ̃������[���
	delete[]	TNurbs->pts;								// �g�����ʂ��\������NURBS�Ȗʂ̃������[���

//	hbody.Free_NurbsC(&TNurbs->pTO->pB->CompC.DegeNurbs);	// �g�����ʊO�����\�����镡���Ȑ����\������k�ޗpNURBS�Ȑ��̃������[���
//	hbody.Free_CompC((COMPC *)TNurbs->pTO->pB);			// �g�����ʊO�����\�����镡���Ȑ����\������NURBS�Ȑ��̃������[���
//	free(TNurbs->pTO->pB);							// �g�����ʊO�����\�����镡���Ȑ��̃������[���
//	free(TNurbs->pTO);								// �g�����ʊO�����\������ʏ���̃������[���
	hbody.Free_NurbsC(&TNurbs->pTO->pB.CompC->DegeNurbs);	// �g�����ʊO�����\�����镡���Ȑ����\������k�ޗpNURBS�Ȑ��̃������[���
	hbody.Free_CompC(TNurbs->pTO->pB.CompC);			// �g�����ʊO�����\�����镡���Ȑ����\������NURBS�Ȑ��̃������[���
	delete[]	TNurbs->pTO->pB.CompC;							// �g�����ʊO�����\�����镡���Ȑ��̃������[���
	delete[]	TNurbs->pTO;								// �g�����ʊO�����\������ʏ���̃������[���

	for(int i=0;i<TNurbs->n2;i++){
//		hbody.Free_NurbsC(&TNurbs->pTI[i]->pB->CompC.DegeNurbs);	// �g�����ʓ������\�����镡���Ȑ����\������k�ޗpNURBS�Ȑ��̃������[���
//		hbody.Free_CompC((COMPC *)TNurbs->pTI[i]->pB);	// �g�����ʓ������\�����镡���Ȑ����\������NURBS�Ȑ��̃������[���
//		free(TNurbs->pTI[i]->pB);					// �g�����ʓ������\�����镡���Ȑ��̃������[���
		hbody.Free_NurbsC(&TNurbs->pTI[i]->pB.CompC->DegeNurbs);	// �g�����ʓ������\�����镡���Ȑ����\������k�ޗpNURBS�Ȑ��̃������[���
		hbody.Free_CompC(TNurbs->pTI[i]->pB.CompC);	// �g�����ʓ������\�����镡���Ȑ����\������NURBS�Ȑ��̃������[���
		delete[]	TNurbs->pTI[i]->pB.CompC;					// �g�����ʓ������\�����镡���Ȑ��̃������[���
	}
	hbody.Free_TrmS(TNurbs);								// �g�����ʃp�����[�^�̃������[���

	return KOD_TRUE;
}

// Function: CalcNurbsCCoord
// �w�肵���m�b�gt�ł�NURBS�Ȑ��̍��W�l�����߂�
//
// Parameters:
// *NurbsC - �ΏۂƂ���NURBS�Ȑ��ւ̃|�C���^
// t - �m�b�g�l
//
// Return:
// ���W�l
//Coord NURBS_Func::CalcNurbsCCoord(NURBSC *NurbsC,double t)
Coord NURBSC::CalcNurbsCCoord(double t)
{
	Coord p;
	Coord bscpw;
	double bsw=0;
	double bs=0;
	int i;

	InitCoord(&bscpw);	// ������

	for(i=0;i<K;i++){
		bs = CalcBSbasis(t,T,N,i,M);		// B�X�v���C�����֐������߂�
//		bs = CalcBSbasis(t,T.get(),N,i,M);		// B�X�v���C�����֐������߂�
		bsw += bs*W[i];							// ����
		bscpw = AddCoord(bscpw,MulCoord(cp[i],bs*W[i]));	// ���q
	}
	
	p = DivCoord(bscpw,bsw);	// ���W�l�����߂�

	return p;
}

// Function: CalcNurbsCCoords
// �w�肵���m�b�gt�Q�ł�NURBS�Ȑ��̍��W�l�����߂�
//
// Parameters:
// *NurbsS - NURBS�Ȗʂւ̃|�C���^   
// Ptnum - ���߂�_�Q�̐�   
// *T - t�p�����[�^�Q���i�[�����z��
// *Pt - �����W�l���i�[
//void NURBS_Func::CalcNurbsCCoords(NURBSC *NurbsC,int Ptnum,double *T,Coord *Pt)
void NURBSC::CalcNurbsCCoords(int Ptnum,double *T,Coord *Pt)
{
	for(int i=0;i<Ptnum;i++){
		Pt[i] = CalcNurbsCCoord(T[i]);
	}
}

// Function: CalcNurbsSCoord
// �w�肵���m�b�gu,v�ł�NURBS�Ȗʂ̍��W�l�����߂�
//
// Parameters:
// *NurbsS - �ΏۂƂ���NURBS�Ȗʂւ̃|�C���^
// div_u - u�����m�b�g�l
// div_v - v�����m�b�g�l
//
// Return:
// ���W�l
//Coord NURBS_Func::CalcNurbsSCoord(NURBSS *NurbsS,double div_u,double div_v)
Coord NURBSS::CalcNurbsSCoord(double div_u,double div_v)
{
	int i,j;
	double bs_u,bs_v;		// u,v����B�X�v���C�����֐�
	double bsw=0;			// ����
	Coord bscpw;			// ���q

	InitCoord(&bscpw);		// ������

	for(i=0;i<K[0];i++){
		bs_u = CalcBSbasis(div_u,S,N[0],i,M[0]);			// u����B�X�v���C�����֐������߂�
		for(j=0;j<K[1];j++){
			bs_v = CalcBSbasis(div_v,T,N[1],j,M[1]);		// v����B�X�v���C�����֐������߂�
			bsw += bs_u*bs_v*W[i][j];
			bscpw = AddCoord(bscpw,MulCoord(cp[i][j],bs_u*bs_v*W[i][j]));
		}
	}
	//if(fabs(bsw) < APPROX_ZERO)
		//return(SetCoord(0,0,0));

	return(DivCoord(bscpw,bsw));
}

// Function: CalcNurbsSCoords
// �w�肵���m�b�gu,v�Q�ł�NURBS�Ȗʂ̍��W�l�Q�����߂�
//
// Parameters:
// *NurbsS - NURBS�Ȗʂւ̃|�C���^   
// Ptnum - ���߂�_�Q�̐�   
// *UV - u,v�p�����[�^�Q���i�[����Coord�^�z��(UV[].x��u�����AUV[].����V�����̃p�����[�^���i�[���Ă�������)
// *Pt - �����W�l���i�[
//void NURBS_Func::CalcNurbsSCoords(NURBSS *NurbsS,int Ptnum,Coord *UV,Coord *Pt)
void NURBSS::CalcNurbsSCoords(int Ptnum,Coord *UV,Coord *Pt)
{
	for(int i=0;i<Ptnum;i++){
		Pt[i] = CalcNurbsSCoord(UV[i].x,UV[i].y);
	}
}

// Function: CalcBSbasis
// B�X�v���C�����֐����v�Z���A�v�Z���ʂ�Ԃ�
//
// Parameters:
// t - �m�b�g�@
// knot[] - �m�b�g�x�N�g��  
// N - �m�b�g�x�N�g���̐�  
// I - Bspl���֐����Y����1��(0�`)  
// M - �K��(Bspl���֐����Y����2��)  
//
// Return:
// �v�Z����
//double NURBS_Func::CalcBSbasis(double t, double knot[],int N,int I,int M)
double CalcBSbasis(double t, double knot[],int N,int I,int M)
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

// Function: CalcDiffBSbasis
// B�X�v���C�����֐���1�K�����W�������߂�
//
// Parameters:
// t - �m�b�g�@
// knot[] - �m�b�g�x�N�g��  
// N - �m�b�g�x�N�g���̐�  
// I - ���ڒ��̃R���g���[���|�C���g  
// M - �K��
//
// Return:
// �v�Z����
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
// B�X�v���C�����֐���N�K�����W�������߂�
//
// Parameters:
// t - �m�b�g�@
// knot[] - �m�b�g�x�N�g��  
// N - �m�b�g�x�N�g���̐�  
// I - ���ڒ��̃R���g���[���|�C���g  
// M - �K��  
// Dn - �����K��
//
// Return:
// �v�Z����
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
// NURBS�Ȑ���1�K�����W�������߂�
// 
// Paramters:
// *NurbsC - NURBS�Ȑ��ւ̃|�C���^
// t - �m�b�g�l
//
// Return:
// �v�Z����
//Coord NURBS_Func::CalcDiffNurbsC(NURBSC *NurbsC,double t)
Coord NURBSC::CalcDiffNurbsC(double t)
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

	// 1�K���������߂�
	p = SubCoord(DivCoord(diff_Ft,Gt),DivCoord(MulCoord(Ft,diff_Gt),Gt*Gt));

	return p;
}

// Function: CalcDiff2NurbsC
// NURBS�Ȑ���2�K�����W�������߂�
// 
// Paramters:
// *NurbsC - NURBS�Ȑ��ւ̃|�C���^
// t - �m�b�g�l
//
// Return:
// �v�Z����
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
// NURBS�Ȑ���r�K�����W�������߂�
// 
// Paramters:
// *NurbsC - NURBS�Ȑ��ւ̃|�C���^
// r - �����K��
// t - �m�b�g�l
//
// Return:
// �v�Z����
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
		Br = AddCoord(Br,MulCoord(CalcDiffNNurbsC(r-i,t),(double)nCr(r,i)*Wi));	// ��A
	}

	return (DivCoord(SubCoord(Ar,Br),W));
}

// Function: CalcDiffuNurbsS
// NURBS�Ȗʂ�u������1�K�����W���𓾂�
//
// Parameters:
// *NurbsS - NURBS�Ȗʂւ̃|�C���^
// div_u - u�����m�b�g�l
// div_v - v�����m�b�g�l
// 
// Return:
// �v�Z����
//Coord NURBS_Func::CalcDiffuNurbsS(NURBSS *NurbsS,double div_u,double div_v)
Coord NURBSS::CalcDiffuNurbsS(double div_u,double div_v)
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

	for(i=0;i<K[0];i++){
		bs_u = CalcBSbasis(div_u,S,N[0],i,M[0]);				// u����B�X�v���C�����֐������߂�
		diff_bs_u = CalcDiffBSbasis(div_u,S,N[0],i,M[0]);	// u����B�X�v���C�����֐���1�K���������߂�
		for(j=0;j<K[1];j++){
			bs_v = CalcBSbasis(div_v,T,N[1],j,M[1]);			// v����B�X�v���C�����֐������߂�
			Ft = AddCoord(Ft,MulCoord(cp[i][j],bs_u*bs_v*W[i][j]));
			diff_Ft = AddCoord(diff_Ft,MulCoord(cp[i][j],diff_bs_u*bs_v*W[i][j]));
			Gt += bs_u*bs_v*W[i][j];
			diff_Gt += diff_bs_u*bs_v*W[i][j];
		}
	}

	if(fabs(Gt) < APPROX_ZERO_H)	return(SetCoord(0,0,0));

	// 1�K���������߂�
	p = SubCoord(DivCoord(diff_Ft,Gt),DivCoord(MulCoord(Ft,diff_Gt),Gt*Gt));

	return p;
}

// Function: CalcDiffvNurbsS
// NURBS�Ȗʂ�v������1�K�����W���𓾂�
//
// Parameters:
// *NurbsS - NURBS�Ȗʂւ̃|�C���^
// div_u - u�����m�b�g�l
// div_v - v�����m�b�g�l
// 
// Return:
// �v�Z����
//Coord NURBS_Func::CalcDiffvNurbsS(NURBSS *NurbsS,double div_u,double div_v)
Coord NURBSS::CalcDiffvNurbsS(double div_u,double div_v)
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

	for(i=0;i<K[0];i++){
		bs_u = CalcBSbasis(div_u,S,N[0],i,M[0]);				// u����B�X�v���C�����֐������߂�
		for(j=0;j<K[1];j++){
			bs_v = CalcBSbasis(div_v,T,N[1],j,M[1]);				// v����B�X�v���C�����֐������߂�
			diff_bs_v = CalcDiffBSbasis(div_v,T,N[1],j,M[1]);	// v����B�X�v���C�����֐���1�K���������߂�
			Ft = AddCoord(Ft,MulCoord(cp[i][j],bs_u*bs_v*W[i][j]));
			diff_Ft = AddCoord(diff_Ft,MulCoord(cp[i][j],bs_u*diff_bs_v*W[i][j]));
			Gt += bs_u*bs_v*W[i][j];
			diff_Gt += bs_u*diff_bs_v*W[i][j];
		}
	}

	if(fabs(Gt) < APPROX_ZERO_H)	return(SetCoord(0,0,0));

	// 1�K���������߂�
	p = SubCoord(DivCoord(diff_Ft,Gt),DivCoord(MulCoord(Ft,diff_Gt),Gt*Gt));

	return p;
}

// Function: CalcDiffNNurbsS
// NURBS�Ȗʂ̊e������C�ӊK���������Ƃ��̔����W�������߂�
//
// Parameters:
// *S - NURBS�Ȗʂւ̃|�C���^   
// k - u�����̔����K��    
// l - v�����̔����K��   
// u,v - u����v�������ꂼ��̃p�����[�^
// 
// Return:
// �v�Z����
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
// (private)NURBS�Ȗʕ���̊e������C�ӊK���������Ƃ��̔����W�������߂�
//
// Parameters:
// *S - NURBS�Ȗʂւ̃|�C���^   
// k - u�����̔����K��    
// l - v�����̔����K��   
// u,v - u����v�������ꂼ��̃p�����[�^
// 
// Return:
// �v�Z����
//double NURBS_Func::CalcDiffNurbsSDenom(NURBSS *S,int k,int l,double u,double v)
double NURBSS::CalcDiffNurbsSDenom(int k,int l,double u,double v)
{
	double w=0;
	for(int i=0;i<K[0];i++){
		double Nk = CalcDiffBSbasisN(u,S,N[0],i,M[0],k);		// u������k�K����
		for(int j=0;j<K[1];j++){
			double Nl = CalcDiffBSbasisN(v,T,N[1],j,M[1],l);	// v������l�K����
			w += Nk*Nl*W[i][j];
		}
	}
	return w;
}

// Function: CalcDiffNurbsSNumer
// (private)NURBS�Ȗʕ��q�̊e������C�ӊK���������Ƃ��̔����W�������߂�
//
// Parameters:
// *S - NURBS�Ȗʂւ̃|�C���^   
// k - u�����̔����K��    
// l - v�����̔����K��   
// u,v - u����v�������ꂼ��̃p�����[�^
// 
// Return:
// �v�Z����
//Coord NURBS_Func::CalcDiffNurbsSNumer(NURBSS *S,int k,int l,double u,double v)
Coord NURBSS::CalcDiffNurbsSNumer(int k,int l,double u,double v)
{
	Coord A=SetCoord(0,0,0);
	for(int i=0;i<K[0];i++){
		double Nk = CalcDiffBSbasisN(u,S,N[0],i,M[0],k);		// u������k�K����
		for(int j=0;j<K[1];j++){
			double Nl = CalcDiffBSbasisN(v,T,N[1],j,M[1],l);	// v������l�K����
			A = AddCoord(A,MulCoord(cp[i][j],Nk*Nl*W[i][j]));
		}
	}
	return A;
}

// Function: CalcNormVecOnNurbsS
// NURBS�Ȗʏ��(u,v)�ɂ�����@���x�N�g�������Ƃ߂�
// 
// Parameters:
// *nurb - NURBS�Ȗʂւ̃|�C���^
// u - u�����m�b�g�l
// v - v�����m�b�g�l
//
// Retrurn:
// �v�Z����
//Coord NURBS_Func::CalcNormVecOnNurbsS(NURBSS *nurb,double u,double v)
Coord NURBSS::CalcNormVecOnNurbsS(double u,double v)
{
	Coord a = CalcDiffuNurbsS(u,v);
	Coord b = CalcDiffvNurbsS(u,v);

	return(NormalizeVec(CalcOuterProduct(a,b)));
}

// Function: CalcTanVecOnNurbsC
// NURBS�Ȑ����t�ɂ�����P�ʐڃx�N�g�������Ƃ߂�
//
// Parameters:
// *C - NURBS�Ȑ��ւ̃|�C���^
// t - �m�b�g�l
//
// Retrurn:
// �v�Z����
//Coord NURBS_Func::CalcTanVecOnNurbsC(NURBSC *C,double t)
Coord NURBSC::CalcTanVecOnNurbsC(double t)
{
    return NormalizeVec(CalcDiffNurbsC(t));
}

// Function: CalcDiffuNormVecOnNurbsS
// NURBS�Ȗʏ��(u,v)�ɂ�����@���x�N�g����u����1�K���������Ƃ߂�
// Nu = Suu�~Sv + Su�~Suv
//
// Parameters:
// *nurb - NURBS�Ȗʂւ̃|�C���^
// u - u�����m�b�g�l
// v - v�����m�b�g�l
//
// Retrurn:
// �v�Z����
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
// NURBS�Ȗʏ��(u,v)�ɂ�����@���x�N�g����v����1�K���������Ƃ߂�
// Nv = Suv�~Sv + Su�~Svv
// 
// Parameters:
// *nurb - NURBS�Ȗʂւ̃|�C���^
// u - u�����m�b�g�l
// v - v�����m�b�g�l
//
// Retrurn:
// �v�Z����
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
// NURBS�Ȗʏ��(u,v)�ɂ����镽�ϋȗ������߂�
// 
// Parameters:
// *nurb - NURBS�Ȗʂւ̃|�C���^
// u - u�����m�b�g�l
// v - v�����m�b�g�l
//
// Retrurn:
// �v�Z����
//double NURBS_Func::CalcMeanCurvature(NURBSS *nurb,double u,double v)
double NURBSS::CalcMeanCurvature(double u,double v)
{
	Coord du = CalcDiffuNurbsS(u,v);			// u����1�K����
	Coord dv = CalcDiffvNurbsS(u,v);			// v����1�K����
	double E = CalcInnerProduct(du,du);				// ��1��{��
	double F = CalcInnerProduct(du,dv);				// ��1��{��
	double G = CalcInnerProduct(dv,dv);				// ��1��{��
	Coord duu = CalcDiffNNurbsS(2,0,u,v);		// u����2�K����
	Coord dvv = CalcDiffNNurbsS(0,2,u,v);		// v����2�K����
	Coord duv = CalcDiffNNurbsS(1,1,u,v);		// u,v�����e1�K����
	Coord n = CalcNormVecOnNurbsS(u,v);		// �@���x�N�g��
	double L = CalcInnerProduct(duu,n);				// ��2��{��
	double M = CalcInnerProduct(duv,n);				// ��2��{��
	double N = CalcInnerProduct(dvv,n);				// ��2��{��
	double H = -(G*L+E*N-2*F*M)/(E*G-F*F)/2;		// ���ϋȗ�

	return H;
}
/*	-- Move to SFQuant.cpp
// Function: CalcMeanCurvature
// NURBS�Ȗʏ��(u,v)�ɂ����镽�ϋȗ������߂�i�I�[�o�[���[�h�j
// 
// Parameters:
// q - �Ȗʂ̊�{�ʂ��Z�b�g�ɂ����\����
//
// Retrurn:
// �v�Z����
double NURBS_Func::CalcMeanCurvature(SFQuant q)
{
	return -(q.G*q.L+q.E*q.N-2*q.F*q.M)/(q.E*q.G-q.F*q.F)/2;		// ���ϋȗ�
}
*/
// Function: CalcMeanCurvatureNormVec
// NURBS�Ȗʏ��(u,v)�ɂ����镽�ϋȗ��@���x�N�g�������Ƃ߂�
//
// Parameters:
// *nurb - NURBS�Ȗʂւ̃|�C���^
// u - u�����m�b�g�l
// v - v�����m�b�g�l
//
// Retrurn:
// �v�Z����
//Coord NURBS_Func::CalcMeanCurvatureNormVec(NURBSS *nurb,double u,double v)
Coord NURBSS::CalcMeanCurvatureNormVec(double u,double v)
{
	Coord n = CalcNormVecOnNurbsS(u,v);				// �@���x�N�g��
	Coord Hn = MulCoord(n,CalcMeanCurvature(u,v));		// ���ϋȗ��@���x�N�g��

	return Hn;
}

// Function: CalcGaussCurvature
// NURBS�Ȗʏ��(u,v)�ɂ�����K�E�X�ȗ������߂�
//
// Parameters:
// *nurb - NURBS�Ȗʂւ̃|�C���^
// u - u�����m�b�g�l
// v - v�����m�b�g�l
//
// Retrurn:
// �v�Z����
//double NURBS_Func::CalcGaussCurvature(NURBSS *nurb,double u,double v)
double NURBSS::CalcGaussCurvature(double u,double v)
{
	Coord du = CalcDiffuNurbsS(u,v);			// u����1�K����
	Coord dv = CalcDiffvNurbsS(u,v);			// v����1�K����
	double E = CalcInnerProduct(du,du);				// ��1��{��
	double F = CalcInnerProduct(du,dv);				// ��1��{��
	double G = CalcInnerProduct(dv,dv);				// ��1��{��
	Coord duu = CalcDiffNNurbsS(2,0,u,v);		// u����2�K����
	Coord dvv = CalcDiffNNurbsS(0,2,u,v);		// v����2�K����
	Coord duv = CalcDiffNNurbsS(1,1,u,v);		// u,v�����e1�K����
	Coord n = CalcNormVecOnNurbsS(u,v);		// �@���x�N�g��
	double L = CalcInnerProduct(duu,n);				// ��2��{��
	double M = CalcInnerProduct(duv,n);				// ��2��{��
	double N = CalcInnerProduct(dvv,n);				// ��2��{��
	double K = (L*N-M*M)/(E*G-F*F);					// �K�E�X�ȗ�

	return K;
}
/*	-- Move to SFQuant.cpp
// Function: CalcGaussCurvature
// NURBS�Ȗʏ��(u,v)�ɂ�����K�E�X�ȗ������߂�i�I�[�o�[���[�h�j
//
// Parameters:
// q - �Ȗʂ̊�{�ʂ��Z�b�g�ɂ����\����
//
// Retrurn:
// �v�Z����
double NURBS_Func::CalcGaussCurvature(SFQuant q)
{
	return (q.L*q.N-q.M*q.M)/(q.E*q.G-q.F*q.F);					// �K�E�X�ȗ�
}
*/
// Function: CalcGaussCurvatureNormVec
// NURBS�Ȗʏ��(u,v)�ɂ�����K�E�X�ȗ��@���x�N�g�������Ƃ߂�
//
// Parameters:
// *nurb - NURBS�Ȗʂւ̃|�C���^
// u - u�����m�b�g�l
// v - v�����m�b�g�l
//
// Retrurn:
// �v�Z����
//Coord NURBS_Func::CalcGaussCurvatureNormVec(NURBSS *nurb,double u,double v)
Coord NURBSS::CalcGaussCurvatureNormVec(double u,double v)
{
	SFQuant q(this,u,v);
//	return MulCoord(q.n,CalcGaussCurvature(q));	// �K�E�X�ȗ��@���x�N�g��
	return MulCoord(q.n,q.CalcGaussCurvature());	// �K�E�X�ȗ��@���x�N�g��
}

// Function: CalcCurvatureNurbsC
// NURBS�Ȑ��̋ȗ������߂�
//
// Parameters:
// *C - NURBS�Ȑ��ւ̃|�C���^
// t - �m�b�g�l
//
// Retrurn:
// �v�Z����
//double NURBS_Func::CalcCurvatureNurbsC(NURBSC *C,double t)
double NURBSC::CalcCurvatureNurbsC(double t)
{
	Coord p_ = CalcDiffNurbsC(t);
	Coord p__ = CalcDiff2NurbsC(t);

	return(CalcEuclid(CalcOuterProduct(p_,p__))/pow(CalcEuclid(p_),3));
}

// Function: DetectInterfereNurbsS
// NURBS�Ȗ�S(u,v)��NURBS�Ȗ�R(w,t)�̊������o����(�g������)
// 
// Parameters:
// *nurbS - NURBS�Ȗ�S(u,v) 
// *nurbR - NURBS�Ȗ�R(w,t) 
// divnum - �p�����[�^������(�����_�̐�)
// 
// Return:
// ���L:KOD_TRUE, ����:KOD_FALSE
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
//						Coord p0 = CalcNurbsSCoord(nurbR,w0,t0);					// R(w0,t0)�ƂȂ�_(�����_)�̍��W
//						Coord q0 = CalcNurbsSCoord(nurbS,u0,v0);					// S(u0,v0)�ƂȂ�_(�����_)�̍��W
//						Coord rw = CalcDiffuNurbsS(nurbR,w0,t0);					// �_R(w0,t0)��u�Δ���(��{�x�N�g��)
//						Coord rt = CalcDiffvNurbsS(nurbR,w0,t0);					// �_R(w0,t0)��v�Δ���(��{�x�N�g��)
						Coord p0 = nurbR->CalcNurbsSCoord(w0,t0);					// R(w0,t0)�ƂȂ�_(�����_)�̍��W
						Coord q0 = nurbS->CalcNurbsSCoord(u0,v0);					// S(u0,v0)�ƂȂ�_(�����_)�̍��W
						Coord rw = nurbR->CalcDiffuNurbsS(w0,t0);					// �_R(w0,t0)��u�Δ���(��{�x�N�g��)
						Coord rt = nurbR->CalcDiffvNurbsS(w0,t0);					// �_R(w0,t0)��v�Δ���(��{�x�N�g��)
						double rwt = CalcEuclid(CalcOuterProduct(rw,rt));
						if(rwt==0.0) break;
						Coord np = DivCoord(CalcOuterProduct(rw,rt),rwt);			// �_R(w0,t0)�̒P�ʖ@���x�N�g��
//						Coord su = CalcDiffuNurbsS(nurbS,u0,v0);					// �_S(u0,v0)��u�Δ���(��{�x�N�g��)
//						Coord sv = CalcDiffvNurbsS(nurbS,u0,v0);					// �_S(u0,v0)��v�Δ���(��{�x�N�g��)
						Coord su = nurbS->CalcDiffuNurbsS(u0,v0);					// �_S(u0,v0)��u�Δ���(��{�x�N�g��)
						Coord sv = nurbS->CalcDiffvNurbsS(u0,v0);					// �_S(u0,v0)��v�Δ���(��{�x�N�g��)
						double suv = CalcEuclid(CalcOuterProduct(su,sv));
						if(suv==0.0) break;
						Coord nq = DivCoord(CalcOuterProduct(su,sv),CalcEuclid(CalcOuterProduct(su,sv)));	// �_S(u0,v0)�̒P�ʖ@���x�N�g��
						double npq = CalcEuclid(CalcOuterProduct(np,nq));
						if(npq==0.0) break;
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
											
						// �\�������������_�����݂��邽�ߊ��L
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
// NURBS�Ȗ�S(u,v)��NURBS�Ȗ�R(w,t)�̊������o����(�g�����L)
// 
// Parameters:
// *tNurbS - NURBS�Ȗ�S(u,v)(�g�����L) 
// *tNurbR - NURBS�Ȗ�R(w,t)(�g�����L) 
// divnum - �p�����[�^������(�����_�̐�)
//
// Return:
// ���L:KOD_TRUE, ����:KOD_FALSE
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
//						Coord p0 = CalcNurbsSCoord(tNurbR->pts,w0,t0);					// R(w0,t0)�ƂȂ�_(�����_)�̍��W
//						Coord q0 = CalcNurbsSCoord(tNurbS->pts,u0,v0);					// S(u0,v0)�ƂȂ�_(�����_)�̍��W
//						Coord rw = CalcDiffuNurbsS(tNurbR->pts,w0,t0);					// �_R(w0,t0)��u�Δ���(��{�x�N�g��)
//						Coord rt = CalcDiffvNurbsS(tNurbR->pts,w0,t0);					// �_R(w0,t0)��v�Δ���(��{�x�N�g��)
						Coord p0 = tNurbR->pts->CalcNurbsSCoord(w0,t0);					// R(w0,t0)�ƂȂ�_(�����_)�̍��W
						Coord q0 = tNurbS->pts->CalcNurbsSCoord(u0,v0);					// S(u0,v0)�ƂȂ�_(�����_)�̍��W
						Coord rw = tNurbR->pts->CalcDiffuNurbsS(w0,t0);					// �_R(w0,t0)��u�Δ���(��{�x�N�g��)
						Coord rt = tNurbR->pts->CalcDiffvNurbsS(w0,t0);					// �_R(w0,t0)��v�Δ���(��{�x�N�g��)
						double rwt = CalcEuclid(CalcOuterProduct(rw,rt));
						if(rwt==0.0) break;
						Coord np = DivCoord(CalcOuterProduct(rw,rt),rwt);				// �_R(w0,t0)�̒P�ʖ@���x�N�g��
//						Coord su = CalcDiffuNurbsS(tNurbS->pts,u0,v0);					// �_S(u0,v0)��u�Δ���(��{�x�N�g��)
//						Coord sv = CalcDiffvNurbsS(tNurbS->pts,u0,v0);					// �_S(u0,v0)��v�Δ���(��{�x�N�g��)
						Coord su = tNurbS->pts->CalcDiffuNurbsS(u0,v0);					// �_S(u0,v0)��u�Δ���(��{�x�N�g��)
						Coord sv = tNurbS->pts->CalcDiffvNurbsS(u0,v0);					// �_S(u0,v0)��v�Δ���(��{�x�N�g��)
						double suv = CalcEuclid(CalcOuterProduct(su,sv));
						if(suv==0.0) break;
						Coord nq = DivCoord(CalcOuterProduct(su,sv),suv);	// �_S(u0,v0)�̒P�ʖ@���x�N�g��
						double npq = CalcEuclid(CalcOuterProduct(np,nq));
						if(npq==0.0) break;
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
										
						// �\�������������_�����݂��邽�ߊ��L
						if(deltapq_dis < CONVERG_GAP){
							if(DetermPtOnTRMSurf(tNurbR,w0,t0) >= KOD_TRUE && DetermPtOnTRMSurf(tNurbS,u0,v0) >= KOD_TRUE){	// �g��������Ȃ����
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
// 3���ȉ���NURBS�Ȗʂƕ��ʂƂ̌�_�Q��㐔��@�ɂċ��߂�(v�p�����[�^����)
// 
// Parameters:
// *nurb - NURBS�Ȗ�  
// pt - ���ʏ�̈�_  
// nvec - ���ʂ̖@���x�N�g��  
// v_divnum - v�p�����[�^������
// *ans - �Z�o���ꂽ��_��u,v�p�����[�^�l�����ꂼ��ans.x,ans.y�Ɋi�[
// ans_size - ans�̔z��
//
// Return:
// ��_�̌�(��_�̐���ans_size�𒴂����ꍇ�FKOD_ERR)
//int NURBS_Func::CalcIntersecPtsPlaneV3(NURBSS *nurb,Coord pt,Coord nvec,int v_divnum,Coord *ans,int ans_size)
int NURBSS::CalcIntersecPtsPlaneV3(Coord pt,Coord nvec,int v_divnum,Coord *ans,int ans_size)
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

//	N = (double *)malloc(sizeof(double)*nurb->K[1]);
//	A = (double *)malloc(sizeof(double)*nurb->K[0]);
//	B = (Coord *)malloc(sizeof(Coord)*nurb->K[0]);
	N = new double[K[1]];
	A = new double[K[0]];
	B = new Coord[K[0]];
	coef = NewMatrix(M[0],M[0]);

	// v�p�����[�^����ԓ��ŕ������A�ev�p�����[�^���NURBS�Ȑ�C(u)�ƕ���(pt,nvec)�Ƃ̌�_�����߂�
	for(int v=0;v<=v_divnum;v++){
		v_const = (V[1] - V[0])*(double)v/(double)v_divnum;		// �K����v�����p�����[�^��ݒ�
		for(int i=0;i<K[1];i++){
			N[i] = CalcBSbasis(v_const,T,N[0],i,M[1]);		// v_const����B�X�v���C�����֐������߂�
		}
		for(int i=0;i<K[0];i++){
			A[i] = 0;
			InitCoord(&B[i]);
			for(int j=0;j<K[1];j++){
				A[i] += N[j]*W[i][j];			// v_const���NURBS�Ȑ�C(u)�̕���̌W��
				B[i] = AddCoord(B[i],MulCoord(cp[i][j],N[j]*W[i][j]));		// v_const���NURBS�Ȑ�C(u)�̕��q�̌W��
			}
		}
		for(int i=0;i<K[0]-M[0]+1;i++){						// i�Ԗڂ̋Ȑ��ɑ΂���
			InitMatrix(coef,M[0],M[0]);
			InitVector(a,4);
			InitCoord(P,4);
			InitVector(Q,4);
			InitVector(t,3);
			if(M[0]-1 == 3){										// 3��
				GetBSplCoef3(M[0],K[0],i,S,coef);		// 3����B�X�v���C�����֐��̌W�������߂�
			}
			else if(M[0]-1 == 2){									// 2��
				GetBSplCoef2(M[0],K[0],i,S,coef);		// 2����B�X�v���C�����֐��̌W�������߂�
			}
			else if(M[0]-1 == 1){									// 1��
				GetBSplCoef1(M[0],K[0],i,S,coef);		// 1����B�X�v���C�����֐��̌W�������߂�
			}
			GetNurbsSCoef(M[0],coef,A,B,i,P,Q);					// �Œ肳�ꂽv�p�����[�^���NURBS�Ȑ�C(u)�̌W�������߂�
			GetIntersecEquation(M[0],P,Q,pt,nvec,a);				// �������𓱏o
			ansnum = CalcEquation(a,t,M[0]-1);					// ������������
			int hitnum = 0;						// �����ɓK��������̐����J�E���g����
			for(int j=0;j<ansnum;j++){			// 3�̉����ꂼ��ɑ΂���
				if(t[j] >= S[i+M[0]-1] && t[j] <= S[i+M[0]]){	// ���ڒ��̃m�b�g�x�N�g���͈͓̔��Ȃ�
					ans[allansnum+hitnum] = SetCoord(t[j],v_const,0);		// ���Ƃ��ēo�^
					hitnum++;
				}
			}
			allansnum += hitnum;				// �����K�����̐��������𐔂��J�E���g�A�b�v
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
// 3���ȉ���NURBS�Ȗʂƕ��ʂƂ̌�_�Q��㐔��@�ɂċ��߂�(u�p�����[�^����)
//
// Parameters:
// *nurb - NURBS�Ȗ�  
// pt - ���ʏ�̈�_  
// nvec - ���ʂ̖@���x�N�g��  
// u_divnum - u�p�����[�^������
// *ans - �Z�o���ꂽ��_��u,v�p�����[�^�l�������ꂼ��ans.x,ans.y�Ɋi�[
// ans_size - ans�̔z��
//
// Return:
// ��_�̌�(��_�̐���ans_size�𒴂����FKOD_ERR)
//int NURBS_Func::CalcIntersecPtsPlaneU3(NURBSS *nurb,Coord pt,Coord nvec,int u_divnum,Coord *ans,int ans_size)
int NURBSS::CalcIntersecPtsPlaneU3(Coord pt,Coord nvec,int u_divnum,Coord *ans,int ans_size)
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

//	N = (double *)malloc(sizeof(double)*nurb->K[0]);
//	A = (double *)malloc(sizeof(double)*nurb->K[1]);
//	B = (Coord *)malloc(sizeof(Coord)*nurb->K[1]);
	N = new double[K[0]];
	A = new double[K[1]];
	B = new Coord[K[1]];
	coef = NewMatrix(M[1],M[1]);

	// u�p�����[�^����ԓ��ŕ������A�eu�p�����[�^���NURBS�Ȑ�C(v)�ƕ���(pt,nvec)�Ƃ̌�_�����߂�
	for(int u=0;u<=u_divnum;u++){
		u_const = (U[1] - U[0])*(double)u/(double)u_divnum;		// �K����u�����p�����[�^��ݒ�
		for(int i=0;i<K[0];i++){
			N[i] = CalcBSbasis(u_const,S,N[0],i,M[0]);		// u_const����B�X�v���C�����֐������߂�
		}
		for(int j=0;j<K[1];j++){
			A[j] = 0;
			InitCoord(&B[j]);
			for(int i=0;i<K[0];i++){
				A[j] += N[i]*W[i][j];			// u_const���NURBS�Ȑ�C(v)�̕���̌W��
				B[j] = AddCoord(B[j],MulCoord(cp[i][j],N[i]*W[i][j]));		// u_const���NURBS�Ȑ�C(v)�̕��q�̌W��
			}
		}
		for(int i=0;i<K[1]-M[1]+1;i++){						// i�Ԗڂ̋Ȑ��ɑ΂���
			if(M[1]-1 == 3){										// 3��
				GetBSplCoef3(M[1],K[1],i,T,coef);		// 3����B�X�v���C�����֐��̌W�������߂�
			}
			else if(M[1]-1 == 2){									// 2��
				GetBSplCoef2(M[1],K[1],i,T,coef);		// 2����B�X�v���C�����֐��̌W�������߂�
			}
			else if(M[1]-1 == 1){									// 1��
				GetBSplCoef1(M[1],K[1],i,T,coef);		// 1����B�X�v���C�����֐��̌W�������߂�
			}
			GetNurbsSCoef(M[1],coef,A,B,i,P,Q);					// �Œ肳�ꂽu�p�����[�^���NURBS�Ȑ�C(v)�̌W�������߂�
			GetIntersecEquation(M[1],P,Q,pt,nvec,a);				// �������𓱏o
			ansnum = CalcEquation(a,t,M[1]-1);					// ������������

			int hitnum = 0;						// �����ɓK��������̐����J�E���g����
			for(int j=0;j<ansnum;j++){			// 3�̉����ꂼ��ɑ΂���
				if(t[j] >= T[i+M[1]-1] && t[j] <= T[i+M[1]]){	// ���ڒ��̃m�b�g�x�N�g���͈͓̔��Ȃ�
					ans[allansnum+hitnum] = SetCoord(u_const,t[j],0);		// ���Ƃ��ēo�^
					hitnum++;
				}
			}
			allansnum += hitnum;				// �����K�����̐��������𐔂��J�E���g�A�b�v
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
// V�����̃A�C�\�p���Ȑ����w�肵���������Ő������C�e�Ȑ���NURBS�ȖʂƂ̌�_���Z�o����
//
// Parameters:
// *nurb - NURBS�Ȗ�  
// pt - ���ʏ�̈�_  
// nvec - ���ʂ̖@���x�N�g��  
// v_divnum - v�p�����[�^������
// *ans - �Z�o���ꂽ��_��u,v�p�����[�^�l�����ꂼ��ans.x,ans.y�Ɋi�[
// ans_size - ans�̔z��
//
// Return:
// ��_�̌�(��_�̐���ans_size�𒴂����FKOD_ERR)
//int NURBS_Func::CalcIntersecPtsPlaneV(NURBSS *nurbss,Coord pt,Coord nvec,int v_divnum,Coord *ans,int ans_size)
int NURBSS::CalcIntersecPtsPlaneV(Coord pt,Coord nvec,int v_divnum,Coord *ans,int ans_size)
{
	double v_const;			// �萔�ƒu�����Ƃ���v�p�����[�^
	int ansnum;				// 1�̃A�C�\�p���Ȑ��ƋȖʂ̌�_�Q�̐�
	int allansnum=0;		// �S��_�����J�E���g
	int ansbufsize = 2*(M[0]-1)*((K[0]>K[1]?K[0]:K[1])-M[0]+1);	// 1�̃A�C�\�p���Ȑ��ƋȖʂ̌�_�Q���i�[����z��̔z��
	double *ansbuf;			// 1�̃A�C�\�p���Ȑ��ƋȖʂ̌�_�Q���i�[����z��
	NURBSC nurbsc;			// 1�̃A�C�\�p���Ȑ�

	ansbuf = NewVector(ansbufsize);

	// v�p�����[�^����ԓ��ŕ������A�ev�p�����[�^���NURBS�Ȑ�C(u)�ƕ���(pt,nvec)�Ƃ̌�_�����߂�
	for(int v=0;v<=v_divnum;v++){
		v_const = (V[1] - V[0])*(double)v/(double)v_divnum;			// �K����v�����p�����[�^��ݒ�
//		ansnum = CalcIntersecIsparaCurveU(nurbss,v_const,pt,nvec,v_divnum,ansbuf,ansbufsize);		// �A�C�\�p���Ȑ��ƋȖʂ̌�_�Q���Z�o
		ansnum = CalcIntersecIsparaCurveU(v_const,pt,nvec,v_divnum,ansbuf,ansbufsize);		// �A�C�\�p���Ȑ��ƋȖʂ̌�_�Q���Z�o
		if(ansnum == KOD_ERR)	goto EXIT;
		for(int i=0;i<ansnum;i++){
//			Coord a = CalcNurbsSCoord(nurbss,ansbuf[i],v_const);
			Coord a = CalcNurbsSCoord(ansbuf[i],v_const);
			ans[allansnum+i] = SetCoord(ansbuf[i],v_const,0);					// ����o�^
		}
		allansnum += ansnum;
	}

EXIT:
	FreeVector(ansbuf);
	return allansnum;
}

// Function: CalcIntersecPtsPlaneU
// U�����̃A�C�\�p���Ȑ����w�肵���������Ő������C�e�Ȑ���NURBS�ȖʂƂ̌�_���Z�o����
//
// Parameters:
// *nurb - NURBS�Ȗ�  
// pt - ���ʏ�̈�_  
// nvec - ���ʂ̖@���x�N�g��  
// u_divnum - u�p�����[�^������
// *ans - �Z�o���ꂽ��_��u,v�p�����[�^�l�����ꂼ��ans.x,ans.y�Ɋi�[
// ans_size - ans�̔z��
//
// Return:
// ��_�̌�(��_�̐���ans_size�𒴂����FKOD_ERR)
//int NURBS_Func::CalcIntersecPtsPlaneU(NURBSS *nurbss,Coord pt,Coord nvec,int u_divnum,Coord *ans,int ans_size)
int NURBSS::CalcIntersecPtsPlaneU(Coord pt,Coord nvec,int u_divnum,Coord *ans,int ans_size)
{
	double u_const;			// �萔�ƒu�����Ƃ���v�p�����[�^
	int ansnum;				// 1�̃A�C�\�p���Ȑ��ƋȖʂ̌�_�Q�̐�
	int allansnum=0;		// �S��_�����J�E���g
	int ansbufsize = 2*(M[0]-1)*((K[0]>K[1]?K[0]:K[1])-M[0]+1);	// 1�̃A�C�\�p���Ȑ��ƋȖʂ̌�_�Q���i�[����z��̔z��
	double *ansbuf;			// 1�̃A�C�\�p���Ȑ��ƋȖʂ̌�_�Q���i�[����z��
	NURBSC nurbsc;			// 1�̃A�C�\�p���Ȑ�

	ansbuf = NewVector(ansbufsize);

	// u�p�����[�^����ԓ��ŕ������A�eu�p�����[�^��̃A�C�\�p���Ȑ�C(v)�ƕ���(pt,nvec)�Ƃ̌�_�����߂�
	for(int u=0;u<=u_divnum;u++){
		u_const = (U[1] - U[0])*(double)u/(double)u_divnum;			// �K����u�����p�����[�^��ݒ�
//		ansnum = CalcIntersecIsparaCurveV(nurbss,u_const,pt,nvec,u_divnum,ansbuf,ansbufsize);		// �A�C�\�p���Ȑ��ƋȖʂ̌�_�Q���Z�o
		ansnum = CalcIntersecIsparaCurveV(u_const,pt,nvec,u_divnum,ansbuf,ansbufsize);		// �A�C�\�p���Ȑ��ƋȖʂ̌�_�Q���Z�o
		if(ansnum == KOD_ERR)	goto EXIT;
		for(int i=0;i<ansnum;i++){
			ans[allansnum+i] = SetCoord(u_const,ansbuf[i],0);					// ����o�^
		}
		allansnum += ansnum;
	}

EXIT:
	FreeVector(ansbuf);
	return allansnum;
}

// Function:
// NURBS�Ȗʂƕ��ʂƂ̌�_�Q���􉽊w�I�ɋ��߂�(�⏕���ʂ�p������@)
//
// Parameters:
// *nurb - NURBS�Ȗ�  
// pt - ���ʏ�̈�_  
// nf - ���ʂ̖@���x�N�g���@
// u_divnum - u�p�����[�^�������@
// v_divnum - v�p�����[�^������
// *ans - �Z�o���ꂽ��_��u,v�p�����[�^�l�����ꂼ��ans.x,ans.y�Ɋi�[
// ans_size - ans�̔z��
//
// Return:
// ��_�̌�(��_�̐���ans_size�𒴂����FERR)
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
//				Coord p0 = CalcNurbsSCoord(nurb,u0,v0);						// S(u0,v0)�ƂȂ�_(�����_)�̍��W
//				Coord su = CalcDiffuNurbsS(nurb,u0,v0);						// �_S(u0,v0)��u�Δ���(��{�x�N�g��)
//				Coord sv = CalcDiffvNurbsS(nurb,u0,v0);						// �_S(u0,v0)��v�Δ���(��{�x�N�g��)
				Coord p0 = CalcNurbsSCoord(u0,v0);						// S(u0,v0)�ƂȂ�_(�����_)�̍��W
				Coord su = CalcDiffuNurbsS(u0,v0);						// �_S(u0,v0)��u�Δ���(��{�x�N�g��)
				Coord sv = CalcDiffvNurbsS(u0,v0);						// �_S(u0,v0)��v�Δ���(��{�x�N�g��)
				if(ZoroCoord(su) == KOD_FALSE || ZoroCoord(sv) == KOD_FALSE) break;
				Coord nt = DivCoord(CalcOuterProduct(su,sv),CalcEuclid(CalcOuterProduct(su,sv)));	// �_S(u0,v0)�̒P�ʖ@���x�N�g��
				Coord nn = DivCoord(CalcOuterProduct(nf,nt),CalcEuclid(CalcOuterProduct(nf,nt)));	// ����F�ƕ���Ft�ɒ������镽��Fn�̒P�ʖ@���x�N�g��
				if(ZoroCoord(nt) == KOD_FALSE || ZoroCoord(nn) == KOD_FALSE) break;
				double df = CalcInnerProduct(pt,nf);						// ���_���畽��F�܂ł̋���
				double dt = CalcInnerProduct(p0,nt);						// ���_���畽��Ft�܂ł̋���
				double dn = CalcInnerProduct(p0,nn);						// ���_���畽��Fn�܂ł̋���
				Coord cross_ntn = CalcOuterProduct(nt,nn);					// �P�ʖ@���x�N�g��nt,nn�̃x�N�g����
				Coord cross_nnf = CalcOuterProduct(nn,nf);					// �P�ʖ@���x�N�g��nn,nf�̃x�N�g����
				Coord cross_nft = CalcOuterProduct(nf,nt);					// �P�ʖ@���x�N�g��nf,nt�̃x�N�g����
				Coord nume_p1_sub =  AddCoord(MulCoord(cross_ntn,df),MulCoord(cross_nnf,dt));	// 3����F,Ft,Fn�̌�_p1�̕��q�̍ŏ���2�����v�Z
				Coord nume_p1 = AddCoord(nume_p1_sub,MulCoord(cross_nft,dn));					// p1�̕��q���Z�o
				double denom_p1 = CalcScalarTriProduct(nf,nt,nn);			// p1�̕�����Z�o
				Coord p1 = DivCoord(nume_p1,denom_p1);						// p1���Z�o
				Coord deltap = SubCoord(p1,p0);								// �_p1�Ɠ_p0�̋������Z�o
				double deltap_dis = CalcEuclid(deltap);						// ��p�̋������Z�o
				double tri_su = CalcScalarTriProduct(su,sv,nf);
				double tri_sv = CalcScalarTriProduct(su,sv,nf);
				if(CheckZero(tri_sv,HIGH_ACCURACY) || CheckZero(tri_su,HIGH_ACCURACY)) break;
				double du = CalcScalarTriProduct(deltap,sv,nf)/CalcScalarTriProduct(su,sv,nf);	// �V�����_s(u0+du,v0+dv)��^���邽�߂�du���Z�o
				double dv = -CalcScalarTriProduct(deltap,su,nf)/CalcScalarTriProduct(su,sv,nf);	// �V�����_s(u0+du,v0+dv)��^���邽�߂�dv���Z�o
				u0 += du;													// �V�����_��u�p�����[�^�𓾂�
				v0 += dv;													// �V�����_��v�p�����[�^�𓾂�
				if(u0 < U[0] || u0 > U[1] || v0 < V[0] || v0 > V[1]){	// �ǐՓ_���p�����[�^�̈�O�ɏo��
				//	fprintf(stderr,"NURBS ERROR:�Ȗ�R�̃p�����[�^���̈�O�ɏo�܂���\n");
					break;
				}
				if(deltap_dis < APPROX_ZERO_H){//CONVERG_GAP){								// ��p������������
					// fprintf(stderr,"   %d:%lf,%lf\n",ansnum,u0,v0);		// debug
					ans[ansnum] = SetCoord(u0,v0,0);						// ���Ƃ��ēo�^
					ansnum++;												// �����J�E���g
					//if(ansnum == ans_size)								// ���̐����������z����
						//return ansnum;
					break;
				}
			}
		}
	}
	
	return ansnum;
}

// Function: CalcIntersecPtsOffsetPlaneSearch
// �I�t�Z�b�gNURBS�Ȗʂƕ��ʂƂ̌�_�Q����_�ǐՖ@�ɂċ��߂�
//
// Parameters:
// *nurb - NURBS�Ȗ�  
// os - �I�t�Z�b�g��  
// pt - ���ʏ��1�_  
// nvec - ���ʂ̖@���x�N�g��  
// ds - ���(��_�Q)�̑e��(��0.1�`2�a)  
// initdivnum - �����_�T���̍r��(��10�`1�a)
// *ans - ���i�[�p�z��  
// ans_size - ���̐�(ans�̔z��)
//
// Return:
// KOD_FALSE:NURBS�Ȗʂƕ��ʂ��������Ă��Ȃ��@KOD_ERR:���ٓ_�܂��͔��U�ɂ�菈���𒆒f
//int NURBS_Func::CalcIntersecPtsOffsetPlaneSearch(NURBSS *nurb,double os,Coord pt,Coord nvec,double ds,int initdivnum,Coord *ans,int ans_size)
int NURBSS::CalcIntersecPtsOffsetPlaneSearch(double os,Coord pt,Coord nvec,double ds,int initdivnum,Coord *ans,int ans_size)
{
	pt.dmy = os;
//	return CalcIntersecPtsPlaneSearch(nurb,pt,nvec,ds,initdivnum,ans,ans_size,CALC_OFFSET);
	return CalcIntersecPtsPlaneSearch(pt,nvec,ds,initdivnum,ans,ans_size,CALC_OFFSET);
}

// ���ʂƃI�t�Z�b�gNURBS�ȖʂƂ̌�_��⏕���ʂ�p���Đ��_���߂�
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
				SFQuant sfq(this,u0,v0);					// S(u0,v0)��̋Ȗʊ�{�ʂ𓾂�
				double H = sfq.E*sfq.G-sfq.F*sfq.F;
				double H2 = H*H;
				if(CheckZero(H,HIGH_ACCURACY) == KOD_TRUE){		// 0����֎~
                    //GuiIF.SetMessage("NURBS KOD_ERROR:The process is stoped by detecting singular point.");
					//return KOD_ERR;		
					break;
				}
				Coord nu = AddCoord(MulCoord(Su,(sfq.M*sfq.F-sfq.L*sfq.G)/H2),MulCoord(Sv,(sfq.L*sfq.F-sfq.M*sfq.E)/H2));	// S�̖@���x�N�g������u��������
				Coord nv = AddCoord(MulCoord(Su,(sfq.N*sfq.F-sfq.M*sfq.G)/H2),MulCoord(Sv,(sfq.M*sfq.F-sfq.N*sfq.E)/H2));	// S�̖@���x�N�g������v��������
				Coord Su_ = AddCoord(Su,MulCoord(nu,d));	// S�̃I�t�Z�b�g�Ȗ�S_��u��������
				Coord Sv_ = AddCoord(Sv,MulCoord(nv,d));	// S�̃I�t�Z�b�g�Ȗ�S_��v��������
//				Coord nt = CalcNormVecOnNurbsS(S,u0,v0);
				Coord nt = CalcNormVecOnNurbsS(u0,v0);
				Coord nn = DivCoord(CalcOuterProduct(nf,nt),CalcEuclid(CalcOuterProduct(nf,nt)));	// ����F�ƕ���Ft�ɒ������镽��Fn�̒P�ʖ@���x�N�g��
//				Coord p0 = AddCoord(CalcNurbsSCoord(S,u0,v0),MulCoord(nt,d));						// S(u0,v0)�̍��W�l
				Coord p0 = AddCoord(CalcNurbsSCoord(u0,v0),MulCoord(nt,d));						// S(u0,v0)�̍��W�l
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
// NURBS�Ȗʂƕ��ʂƂ̌�_�Q����_�ǐՖ@�ɂċ��߂�
//
// Parameters:
// *nurb - NURBS�Ȗ�  
// pt - ���ʏ��1�_  
// nvec - ���ʂ̖@���x�N�g��  
// ds - ���(��_�Q)�̑e��(��0.1�`2�a)  
// initdivnum - �����_�T���̍r��(��10�`1�a)
// *ans - ���i�[�p�z��  
// ans_size - ���̐�(ans�̔z��)
// method:��_�Z�o���̐��l��͖@�̑I��(RUNGE_KUTTA or BULIRSH_STOER)
//
// Return:
// �Ԓl�@KOD_FALSE:NURBS�Ȗʂƕ��ʂ��������Ă��Ȃ��@KOD_ERR:���ٓ_�܂��͔��U�ɂ�菈���𒆒f
//int NURBS_Func::CalcIntersecPtsPlaneSearch(NURBSS *nurb,Coord pt,Coord nvec,double ds,int initdivnum,Coord *ans,int ans_size,int method)
int NURBSS::CalcIntersecPtsPlaneSearch(Coord pt,Coord nvec,double ds,int initdivnum,Coord *ans,int ans_size,int method)
{
	int loop_count=0;		// �����v�Z�̃��[�v��
	int pcount=0;
	int anscount=0;
	Coord oldp;
	Coord newp;
	Coord init_pt[INTERSECPTNUMMAX];		// �����_(u,v�p�����[�^�l)
	Coord init_pt_buf[INTERSECPTNUMMAX];	// �����_���u���o�b�t�@(u,v�p�����[�^�l)
	Coord init_pt_Coord[INTERSECPTNUMMAX];	// �����_(x,y,z���W�l)
	bool  init_pt_flag[INTERSECPTNUMMAX];	// �e�����_��ʂ�I�������𔻕ʂ���t���O
	bool  init_allpt_flag=KOD_FALSE;			// �����_��S�Ēʂ�I�������𔻕ʂ���t���O
	int   init_pt_num=0;						// �����_�̐�
	int   init_pt_flag_count=0;
	double u,v;								// ����ǐՒ���u,v�p�����[�^���Ԓl
//	double dist;							// ���[�v�E�o�p�ɒǐՓ_�ԋ�����臒l�Ƃ���
	int loopbreak_flag = KOD_FALSE;				// �����_��v�t���O
	int  search_flag = KOD_TRUE;				// ����ǐՕ����t���O(KOD_TRUE:������,KOD_FALSE:�t����)
	int  inverse_flag = KOD_FALSE;				// ����ǐՕ����t�]�t���O
	double color[3] = {0,1,1};
//	double *hdid,*hnext;

	//FILE *fp = fopen("debug.csv","w");

	// �����_�ʉߔ��ʃt���O��������
	for(int i=0;i<INTERSECPTNUMMAX;i++){
		init_pt_flag[i] = KOD_FALSE;
	}
	init_pt_flag[0] = KOD_TRUE;

	// �܂�����ǐՖ@�̏����_�Ƃ��Č�_�����������߂�
	if(method == CALC_OFFSET) {
//		init_pt_num = CalcIntersecPtsOffsetPlaneGeom(nurb,pt.dmy,pt,nvec,initdivnum,init_pt,INTERSECPTNUMMAX);
		init_pt_num = CalcIntersecPtsOffsetPlaneGeom(pt.dmy,pt,nvec,initdivnum,init_pt,INTERSECPTNUMMAX);
	}
	else{
		// �����_��2�����ŃT�[�`
//		init_pt_num = CalcIntersecPtsPlaneU(nurb,pt,nvec,initdivnum,init_pt,INTERSECPTNUMMAX);
//		int num = CalcIntersecPtsPlaneV(nurb,pt,nvec,initdivnum,init_pt_buf,INTERSECPTNUMMAX);
		init_pt_num = CalcIntersecPtsPlaneU(pt,nvec,initdivnum,init_pt,INTERSECPTNUMMAX);
		int num = CalcIntersecPtsPlaneV(pt,nvec,initdivnum,init_pt_buf,INTERSECPTNUMMAX);
		init_pt_num = CatCoord(init_pt,init_pt_buf,INTERSECPTNUMMAX,init_pt_num,num);
		if(!init_pt_num) {
//			init_pt_num = CalcIntersecPtsPlaneGeom(nurb,pt,nvec,initdivnum,initdivnum,init_pt,INTERSECPTNUMMAX);	// ���������Ȃ�������C�T�[�`�@��ς��ăg���C
			init_pt_num = CalcIntersecPtsPlaneGeom(pt,nvec,initdivnum,initdivnum,init_pt,INTERSECPTNUMMAX);	// ���������Ȃ�������C�T�[�`�@��ς��ăg���C
		}
	}
	init_pt_num = CheckTheSamePoints(init_pt,init_pt_num);		// ����_�͏�������
	if(!init_pt_num){		// ������Ȃ��ꍇ�́A�������Ă��Ȃ��Ƃ݂Ȃ�
        GuiIF.SetMessage("NURBS KOD_ERROR:Init intersection point is noexistence");
		return KOD_FALSE;					
	}
	else if(init_pt_num == KOD_ERR) return KOD_ERR;			// init_pt_num��init_pt�̔z�񒷂𒴂���

	for(int i=0;i<init_pt_num;i++){
//		init_pt_Coord[i] = CalcNurbsSCoord(nurb,init_pt[i].x,init_pt[i].y);		// ��_��uv�p�����[�^��xyz���W�l�ɕϊ��������̂�ێ����Ă���
		init_pt_Coord[i] = CalcNurbsSCoord(init_pt[i].x,init_pt[i].y);		// ��_��uv�p�����[�^��xyz���W�l�ɕϊ��������̂�ێ����Ă���
		//fprintf(stderr,"%d,%lf,%lf,%lf,%lf,%lf\n",i,init_pt[i].x,init_pt[i].y,init_pt_Coord[i].x,init_pt_Coord[i].y,init_pt_Coord[i].z);	// debug
		//DrawPoint(init_pt_Coord[i],1,3,color);	// debug
	}

	// �����_��S�Ēʉ߂���܂Ō���ǐՖ@���J��Ԃ�
	while(init_allpt_flag == KOD_FALSE){
		//fprintf(stderr,"Init%d,%lf,%lf,%lf,%lf,%lf\n",pcount+1,init_pt[pcount].x,init_pt[pcount].y,init_pt_Coord[pcount].x,init_pt_Coord[pcount].y,init_pt_Coord[pcount].z);		// debug
		// ����ǐՂ̂��߂̎n�_(u,v)���Z�b�g
		u = oldp.x = init_pt[pcount].x;
		v = oldp.y = init_pt[pcount].y;
		ans[anscount] = SetCoord(init_pt[pcount]);
		anscount++;
		init_pt_flag[pcount] = KOD_TRUE;
		init_pt_flag_count++;
		//if(init_pt_flag_count == init_pt_num && init_pt_num > 1)	break;

		if(inverse_flag == KOD_TRUE){	// �t�����ւ̌���ǐՂ��I�����Ă�����
			inverse_flag = KOD_FALSE;	// ����ǐՕ������������ɖ߂�
		}

		// ����ǐՊJ�n
		loop_count = 0;
		while(loop_count < ans_size){
			// �������Ɍ���ǐ�
			if(inverse_flag == KOD_FALSE){
//				if(method == RUNGE_KUTTA)	search_flag = SearchIntersectPt_RKM(nurb,pt,nvec,ds,&u,&v,FORWARD);	// �������̌�_�Z�o
//				else if(method == BULIRSH_STOER)	search_flag = SearchIntersectPt_BS(nurb,pt,nvec,ds,&u,&v,FORWARD);
//				else search_flag = SearchIntersectPt_OS(nurb,pt,nvec,ds,&u,&v,FORWARD);
				if(method == RUNGE_KUTTA)	search_flag = SearchIntersectPt_RKM(pt,nvec,ds,&u,&v,FORWARD);	// �������̌�_�Z�o
				else if(method == BULIRSH_STOER)	search_flag = SearchIntersectPt_BS(pt,nvec,ds,&u,&v,FORWARD);
				else search_flag = SearchIntersectPt_OS(pt,nvec,ds,&u,&v,FORWARD);
				if(search_flag == KOD_ERR){							// �������ǐՂɎ��s�����ꍇ��
					inverse_flag = KOD_TRUE;						// �t�����ǐՃt���O��ON
					//fprintf(stderr,"a,%d,%d,%lf,%lf\n",search_flag,inverse_flag,u,v);	// for debug	
					u = init_pt[pcount].x;							// ��_�ǐՂ̏����_���Z�b�g���Ȃ���
					v = init_pt[pcount].y;
				}
				//fprintf(stderr,"e,%d,%d,%lf,%lf\n",search_flag,inverse_flag,u,v);	// for debug
			}
			// �t�����ǐՃt���O��ON�Ȃ�
			if(inverse_flag == KOD_TRUE){
//				if(method == RUNGE_KUTTA)	search_flag = SearchIntersectPt_RKM(nurb,pt,nvec,ds,&u,&v,INVERSE);	// �t�����̌�_�Z�o
//				else if(method == BULIRSH_STOER)	search_flag = SearchIntersectPt_BS(nurb,pt,nvec,ds,&u,&v,INVERSE);
//				else search_flag = SearchIntersectPt_OS(nurb,pt,nvec,ds,&u,&v,INVERSE);
				if(method == RUNGE_KUTTA)	search_flag = SearchIntersectPt_RKM(pt,nvec,ds,&u,&v,INVERSE);	// �t�����̌�_�Z�o
				else if(method == BULIRSH_STOER)	search_flag = SearchIntersectPt_BS(pt,nvec,ds,&u,&v,INVERSE);
				else search_flag = SearchIntersectPt_OS(pt,nvec,ds,&u,&v,INVERSE);
				if(search_flag == KOD_ERR){					// ���ٓ_���o�ɂ�菈�����p���ł��Ȃ��ꍇ
					//fprintf(stderr,"b,%d,%d,%lf,%lf\n",search_flag,inverse_flag,u,v);	// for debug
					GuiIF.SetMessage("NURBS_FUNC CAUTION: Singler point was ditected.");
					break;
				}
				//fprintf(stderr,"f,%d,%d,%lf,%lf\n",search_flag,inverse_flag,u,v);	// for debug
			}

			// �p�����[�^�͈͊O�̏ꍇ
			if(search_flag == KOD_FALSE){
//				newp = CalcIntersecPtsPlaneSearch_Sub(nurb,u,v,pt,nvec);		// �ʂ����яo����(u,v)���Q�l�ɖʂ̃G�b�W��(new_u,new_v)�𓾂�
				newp = CalcIntersecPtsPlaneSearch_Sub(u,v,pt,nvec);		// �ʂ����яo����(u,v)���Q�l�ɖʂ̃G�b�W��(new_u,new_v)�𓾂�
				//fprintf(stderr,"c,%d,%d,%.12lf,%.12lf\n",search_flag,inverse_flag,newp.x,newp.y);	// for debug
				ans[anscount] = SetCoord(newp);		// ����ꂽu,v�����(��_�Q)�Ƃ��ēo�^
				anscount++;							// ��_�Q�̐����C���N�������g
				// �����_������ǐՖ@�ɂ���đS�Ēʉ߂��������ׂ�
				for(int i=0;i<init_pt_num;i++){
					if(CheckClossedPoints(oldp,newp,init_pt[i]) == KOD_TRUE){ // �V���ɎZ�o���ꂽ��_��1�O�̌�_��Ίp�Ƃ��闧���̂̒��ɏ����_���܂܂�Ă�����
						if(init_pt_flag[i] == KOD_FALSE){		// �܂��ʉ߂��Ă��Ȃ������_�Ō�_��u,v�͈͓���������
							init_pt_flag[i] = KOD_TRUE;			// �ʉ߂������ƂƂ��ēo�^
							init_pt_flag_count++;				// �ʉߍςݏ����_�����J�E���g�A�b�v
							//fprintf(stderr,"%d OK!\n",i);			// debug
						}
					}
				}
				if(inverse_flag == KOD_FALSE){		// �����������Ȃ�
					inverse_flag = KOD_TRUE;		// ���̃T�[�`�͋t�����ɂ���
					u = init_pt[pcount].x;			// ��_�ǐՂ̏����_���Z�b�g���Ȃ���
					v = init_pt[pcount].y;
					oldp = SetCoord(init_pt[pcount]);
					continue;						// �t�������[�v��
				}
				break;								// �����t�����Ȃ烋�[�v�I���
			}

			// ��O�Ȃ��ŉ�������ꂽ
			else{
//				Coord cd = CalcNurbsSCoord(nurb,u,v);
				Coord cd = CalcNurbsSCoord(u,v);
				//fprintf(stderr,"d,%d,%d,%.12lf,%.12lf,%lf,%lf,%lf,%d\n",search_flag,inverse_flag,u,v,cd.x,cd.y,cd.z,anscount);			// for debug
				newp.x = u;					
				newp.y = v;
			}

			// �����_������ǐՖ@�ɂ���đS�Ēʉ߂��������ׂ�
			for(int i=0;i<init_pt_num;i++){
				// �V���ɎZ�o���ꂽ��_��1�O�̌�_��Ίp�Ƃ��闧���̂̒��ɏ����_���܂܂�Ă�����
				if(int asdf = CheckClossedPoints(oldp,newp,init_pt[i]) == KOD_TRUE){
					if(loop_count && i==pcount && inverse_flag == KOD_FALSE){	// ���[�v�ɑ΂��Ĉ�����Ė߂��Ă����ꍇ�̓��[�v�𔲂���
						loopbreak_flag = KOD_TRUE;	
						//fprintf(fp,"%d loop break OK\n",i);		// debug
						break;					
					}
					if(init_pt_flag[i] == KOD_FALSE && search_flag == KOD_TRUE){		// �܂��ʉ߂��Ă��Ȃ������_�Ō�_��u,v�͈͓���������
						init_pt_flag[i] = KOD_TRUE;					// �ʉ߂������ƂƂ��ēo�^
						init_pt_flag_count++;						// �ʉߍςݏ����_�����J�E���g�A�b�v
						//fprintf(fp,"%d OK\n",i);				// debug
					}
				}
			}

			// ���[�v�ɑ΂��Ĉ�����Ă����ꍇ�̓��[�v�𔲂���
			if(loopbreak_flag == KOD_TRUE){
				loopbreak_flag = KOD_FALSE;
				break;
			}

			ans[anscount] = SetCoord(newp);	// ����ꂽu,v�����(��_�Q)�Ƃ��ēo�^
			anscount++;				// ��_�Q�̐����C���N�������g

			// ��_�̐����w��T�C�Y�𒴂����ꍇ�͂����܂łŋ������^�[��
			if(anscount >= ans_size){
                GuiIF.SetMessage("NURBS KOD_ERROR:Intersection points exceeded the allocated array length");
                GuiIF.SetMessage("There is a possibility that you set large ds.");
				return anscount;
			}

			oldp = SetCoord(newp);	// ���̃��[�v�ŎZ�o���ꂽ��_�͎��̃��[�v�ł�1�O�̌�_�ƂȂ�

			loop_count++;		// ���[�v�񐔂��C���N�������g
		}// ����ǐՂ����܂�

		// �c�����_������΁A�ʂ̌��������̂ŁA���̓_���n�_�Ƃ��čēx����ǐՂ��s��
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
// (private)�w�肵���_������2�_��Ίp�Ƃ��闧���̂̒��ɑ��݂��邩�𒲂ׂ�
// 
// Parameters:
// A - �Ίp���1�_
// B - �Ίp���1�_
// P - �w��_
// 
// Return:
// ���݂���FKOD_TRUE,  ���݂��Ȃ��FKOD_FALSE
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
// (private)���ʂ�NURBS�ȖʂƂ̌�_�Q�����߂�֐�CalcIntersecPtsPlaneSearch()�̃T�u�֐��D
// �ʂ����яo����(u,v)���Q�l�ɖʂ̃G�b�W���ɂ������_(new_u,new_v)�𓾂�
//
// Parameters:
// *nurb - NURBS�Ȗʂւ̃|�C���^
// u,v - �Ȗʑ��ݗ̈�O��(u, v)�l 
// pt - ���ʏ��1�_
// nvec - ���ʂ̖@���x�N�g��
// 
// Return:
// �G�b�W����̌�_��(u, v)���W�l�iCoord.x��u�CCoord.y��v�����ꂼ��i�[�����j
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

	// �ǂ����яo���������ׂ�
	if(u < U[0]){
		uflag = true;
		u = U[0];			// �G�b�W��u�Ƃ���
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
//		n = CalcIntersecIsparaCurveV(nurb,u,pt,nvec,5,a,INTERSECPTNUMMAX);	// u���Œ肵���A�C�\�p���Ȑ��ɑ΂��ĕ��ʂƂ̌�_�𓾂�
		n = CalcIntersecIsparaCurveV(u,pt,nvec,5,a,INTERSECPTNUMMAX);	// u���Œ肵���A�C�\�p���Ȑ��ɑ΂��ĕ��ʂƂ̌�_�𓾂�
		for(int i=0;i<n;i++)
			cod_a[i] = SetCoord(u,a[i],0);
	}
	else if(uflag == false && vflag == true){
//		n = CalcIntersecIsparaCurveU(nurb,v,pt,nvec,5,a,INTERSECPTNUMMAX);	// v���Œ肵���A�C�\�p���Ȑ��ɑ΂��ĕ��ʂƂ̌�_�𓾂�
		n = CalcIntersecIsparaCurveU(v,pt,nvec,5,a,INTERSECPTNUMMAX);	// v���Œ肵���A�C�\�p���Ȑ��ɑ΂��ĕ��ʂƂ̌�_�𓾂�
		for(int i=0;i<n;i++)
			cod_a[i] = SetCoord(a[i],v,0);
	}
	else if(uflag == true && vflag == true){
//		n = CalcIntersecIsparaCurveV(nurb,u,pt,nvec,5,a,INTERSECPTNUMMAX);		// u���Œ肵���A�C�\�p���Ȑ��ɑ΂��ĕ��ʂƂ̌�_�𓾂�
		n = CalcIntersecIsparaCurveV(u,pt,nvec,5,a,INTERSECPTNUMMAX);		// u���Œ肵���A�C�\�p���Ȑ��ɑ΂��ĕ��ʂƂ̌�_�𓾂�
		if(n > 0){
			for(int i=0;i<n;i++)
				cod_a[i] = SetCoord(u,a[i],0);
		}
		if(n <= 0){
//			n = CalcIntersecIsparaCurveU(nurb,v,pt,nvec,5,a,INTERSECPTNUMMAX);	// v���Œ肵���A�C�\�p���Ȑ��ɑ΂��ĕ��ʂƂ̌�_�𓾂�
			n = CalcIntersecIsparaCurveU(v,pt,nvec,5,a,INTERSECPTNUMMAX);	// v���Œ肵���A�C�\�p���Ȑ��ɑ΂��ĕ��ʂƂ̌�_�𓾂�
			for(int i=0;i<n;i++)
				cod_a[i] = SetCoord(a[i],v,0);
		}
	}

	min = GetMinDistance(old,cod_a,n);

	return min;
}

// Function: SearchIntersectPt_BS
// (private)Bulirsch-Stoer�@�ɂ���_������������(NURBS�Ȗʂƕ���)
// 
// Parameters:
// *S - 1�ڂ̑ΏۂƂȂ�NURBS�Ȗ�
// pt - ���ʏ��1�_
// nvec - ���ʂ̖@���x�N�g��
// H - BS�@�̃f�t�H���g�̍��ݕ�
// *u0 - ����ꂽ��_��u�p�����[�^
// *v0 - ����ꂽ��_��v�p�����[�^
// direction - �ǐՕ�����\���t���O�iFORWARD or INVERSE)
// 
// Return:
// ���������FKOD_TRUE, �p�����[�^�͈͊O�FKOD_FALSE�C���s�FKOD_ERR
//int NURBS_Func::SearchIntersectPt_BS(NURBSS *S,Coord pt,Coord nvec,double H,double *u0,double *v0,int direction)
int NURBSS::SearchIntersectPt_BS(Coord pt,Coord nvec,double H,double *u0,double *v0,int direction)
{
	// �����w��~�X
	if(direction != FORWARD && direction != INVERSE){
		GuiIF.SetMessage("NURBS ERROR: selected wrong direction");
		return KOD_ERR;
	}

	int    n[BS_DIV] = {2,4,6,8,12,16,24,32,48,64,96};	// B-S�@�̕������Q���w��
	Coord  z[97];										// �C�����_�@�̒��Ԓl���i�[(z.x = u, z.y = v)
	Coord  f;											// f.x = fu(u,v), f.y = fv(u,v)
	Coord  D[BS_DIV][BS_DIV],C[BS_DIV][BS_DIV];			// B-S�@�̒��ԃp�����[�^
	double h[BS_DIV];									// B-S�@�̍��ݕ�
	Coord wek,wek_;										// h=0�̊O�}�l

	for(int lpnum=0;lpnum<4;lpnum++){

		// �e�������ɂ����鍏�ݕ������߂Ă���
		for(int i=0;i<BS_DIV;i++)
			h[i] = H/n[i];

		// ���ݕ��������������珇�ɕύX���Ȃ���AB-S�@�ɂ��O�}�l���v�Z���Ă���
		for(int i=0;i<BS_DIV;i++){
			bool  divzero_flag = false;							// �[�����Ď��t���O

			// �܂��Au(s+H), v(s+H)�̒l���C�����_�@�ɂ��v�Z����
			z[0] = SetCoord(*u0,*v0,1);										// z0��z1�̎Z�o�͕ʏ���
			if(GetSIPParam1(*u0,*v0,pt,nvec,direction,&f) == KOD_ERR){	// z0�ł̔����������̉E�ӂ��v�Z
				break;
			}
			z[1] = AddCoord2D(z[0],MulCoord2D(f,h[i]));							// z0��z1�̎Z�o�͕ʏ���
			for(int j=1;j<n[i];j++){
				if(GetSIPParam1(z[j].x,z[j].y,pt,nvec,direction,&f) == KOD_ERR){	// zj�ł̔����������̉E�ӂ��v�Z
					wek = SetCoord(z[j]);
					divzero_flag = true;
					break;
				}
				z[j+1] = AddCoord2D(z[j-1],MulCoord2D(f,2*h[i]));				// z2�`zn�܂ł��Z�o
			}
			if(divzero_flag == true)	break;						// �[�����ɂȂ�ꍇ��break���C���̃X�e�b�v����
			if(GetSIPParam1(z[n[i]].x,z[n[i]].y,pt,nvec,direction,&f) == KOD_ERR){	// zn�ł̔����������̉E�ӂ��v�Z
				wek = SetCoord(z[n[i]]);
				break;
			}
			C[i][0] = DivCoord(AddCoord2D(AddCoord2D(z[n[i]],z[n[i]-1]),MulCoord2D(f,h[i])),2);		// u(s+H)
			D[i][0] = SetCoord(C[i][0]);
			if(i==0){
				wek_ = SetCoord(D[i][0]);
				continue;
			}

			// B-S�@�̍����\���������߂Ă���
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
	
		// �����܂ŗ����ꍇ�C���ݕ�H��1/4�Ƃ��ăg���C
		H *= 0.25;
		
		if(lpnum==3){
			*u0 = wek.x;
			*v0 = wek.y;
		}
	}

	// �����܂ŗ����ꍇ�C�Ō�ɎZ�o���ꂽ(*u0,*v0)���͈͊O�Ȃ�KOD_FALSE�����^�[��
	if(*u0 < U[0] || *u0 > U[1] || *v0 < V[0] || *v0 > V[1]){
		return KOD_FALSE;
	}
	// ����ȊO�͓��ٓ_�Ƃ���KOD_ERR�����^�[��
	return KOD_ERR;
}

// Function: GetSIPParam1
// (private)SearchIntersectPt_BS()�̃T�u�֐��D�Ȗʂƕ��ʂ̌�_��\�������������̉E�ӂ̒l�𓾂�
//
// Parameters:
// *S - NURBS�Ȗʂւ̃|�C���^
// u,v - ���ڒ���NURBS�Ȗʃp�����[�^ 
// pt - ���ʏ�̈�_
// nvec - ���ʂ̖@���x�N�g��
// direction - �ǐՕ�����\���t���O�iFORWARD or INVERSE)
// *f - �v�Z���� 
//
// Return:
// �����FKOD_TRUE, ���s�FKOD_ERR
//int NURBS_Func::GetSIPParam1(NURBSS *S,double u,double v,Coord pt,Coord nvec,int direction,Coord *f)
int NURBSS::GetSIPParam1(double u,double v,Coord pt,Coord nvec,int direction,Coord *f)
{
//	NURBS_Func NFunc;

//	Coord Su = CalcDiffuNurbsS(S,u,v);
//	Coord Sv = CalcDiffvNurbsS(S,u,v);
	Coord Su = CalcDiffuNurbsS(u,v);
	Coord Sv = CalcDiffvNurbsS(u,v);
	double fu = CalcInnerProduct(nvec,Su);	// nf�ESu
	double fv = CalcInnerProduct(nvec,Sv);	// nf�ESv
	if(CheckZero(fu,HIGH_ACCURACY) == KOD_TRUE && CheckZero(fv,HIGH_ACCURACY) == KOD_TRUE){			// ���ٓ_
		//GuiIF.SetMessage("NURBS KOD_ERROR:The process is stoped by detected singular point.");
		return KOD_ERR;				
	}
	double E = CalcInnerProduct(Su,Su);		// 1���K�i��
	double F = CalcInnerProduct(Su,Sv);		// 1���K�i��
	double G = CalcInnerProduct(Sv,Sv);		// 1���K�i��
	double f_ = 1/sqrt(E*fv*fv - 2*F*fu*fv + G*fu*fu);
	*f = SetCoord(f_*fv*(double)direction, -f_*fu*(double)direction, 0);

	return KOD_TRUE;
}

// Function: SearchIntersectPt_RKM
// (private)4���̃����Q�N�b�^�@�ɂ���_�𓱏o(NURBS�Ȗʂƕ���)
// >du(s)/ds = g(u,v),   dv(s)/ds = h(u,v)
// >u(s+delta) = u(s) + (p1+2p2+2p3+p4)/6
// >v(s+delta) = v(s) + (q1+2q2+2q3+q4)/6
// >p1 = delta*g(u,v),   q1 = delta*h(u,v)
// >p2 = delta*g(u+p1/2,v+q1/2),   q2 = delta*h(u+p1/2,v+q1/2)
// >p3 = delta*g(u+p2/2,v+q2/2),   q3 = delta*h(u+p2/2,v+q2/2)
// >p4 = delta*g(u+p3,v+q3),   q4 = delta*h(u+p3,v+q3)
// 
// Parameters:
// *S - NURBS�Ȗʂւ̃|�C���^
// pt - ���ʏ�̈�_
// n - ���ʂ̖@���x�N�g��
// delta - ��ǐՂ̍��ݕ�
// *u,*v - ��
// direction - �ǐՕ�����\���t���O�iFORWARD or INVERSE)
// 
// Return:
// �����FKOD_TRUE, ���s�FKOD_ERR
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
		if(*u < U[0] || *u > U[1] || *v < V[0] || *v > V[1])	// �p�����[�^�͈͊O
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
		if(CheckZero(fu,LOW_ACCURACY) == KOD_TRUE && CheckZero(fv,LOW_ACCURACY) == KOD_TRUE){			// ���ٓ_
            //GuiIF.SetMessage("NURBS KOD_ERROR:The process is stoped by detected singular point.");
			return KOD_ERR;				
		}
		double E = CalcInnerProduct(Su,Su);		// 1���K�i��
		double F = CalcInnerProduct(Su,Sv);		// 1���K�i��
		double G = CalcInnerProduct(Sv,Sv);		// 1���K�i��
		double denom = sqrt(E*fvv - 2*F*fuv + G*fuu);
		if(CheckZero(denom,LOW_ACCURACY) == KOD_TRUE)	return KOD_ERR;		// ���ٓ_
		double f_ = 1/denom;
		p[i] = -delta*fv*f_*(double)direction;
		q[i] = delta*fu*f_*(double)direction;
	}
	*u = u0+(p[0]+2*p[1]+2*p[2]+p[3])/6;
	*v = v0+(q[0]+2*q[1]+2*q[2]+q[3])/6;

	if(*u < U[0] || *u > U[1] || *v < V[0] || *v > V[1])	// �p�����[�^�͈͊O
		return KOD_FALSE;

	return KOD_TRUE;
}

// Function: SearchIntersectPt_OS
// (private)4���̃����Q�N�b�^�@�ɂ���_�𓱏o(�I�t�Z�b�gNURBS�Ȗʂƕ���)
// >du(s)/ds = g(u,v),   dv(s)/ds = h(u,v)
// >u(s+delta) = u(s) + (p1+2p2+2p3+p4)/6
// >v(s+delta) = v(s) + (q1+2q2+2q3+q4)/6
// >p1 = delta*g(u,v),   q1 = delta*h(u,v)
// >p2 = delta*g(u+p1/2,v+q1/2),   q2 = delta*h(u+p1/2,v+q1/2)
// >p3 = delta*g(u+p2/2,v+q2/2),   q3 = delta*h(u+p2/2,v+q2/2)
// >p4 = delta*g(u+p3,v+q3),   q4 = delta*h(u+p3,v+q3)
// 
// Parameters:
// *S - NURBS�Ȗʂւ̃|�C���^
// pt - ���ʏ�̈�_
// n - ���ʂ̖@���x�N�g��
// delta - ��ǐՂ̍��ݕ�
// *u,*v - ��
// direction - �ǐՕ�����\���t���O�iFORWARD or INVERSE)
// 
// Return:
// �����FKOD_TRUE, �p�����[�^�͈͊O�FKOD_FALSE, ���s�FKOD_ERR
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
		if(CheckZero(H,HIGH_ACCURACY) == KOD_TRUE){			// ���ٓ_
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
		if(CheckZero(fut,HIGH_ACCURACY) == KOD_TRUE && CheckZero(fvt,HIGH_ACCURACY) == KOD_TRUE){			// ���ٓ_
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
		double Et = sfq.E-2*sfq.L*d+nunu*d*d;		// 1���K�i��
		double Ft = sfq.F-2*sfq.M*d+nunv*d*d;		// 1���K�i��
		double Gt = sfq.G-2*sfq.N*d+nvnv*d*d;		// 1���K�i��
		double denom = Et*fvvt - 2*Ft*fuvt + Gt*fuut;
		if(denom <= 0)
			return KOD_ERR;
		double gt_ = 1/sqrt(denom);
		p[i] = -delta*fvt*gt_*(double)direction;
		q[i] = delta*fut*gt_*(double)direction;
	}
	*u = u0+(p[0]+2*p[1]+2*p[2]+p[3])/6;
	*v = v0+(q[0]+2*q[1]+2*q[2]+q[3])/6;
	
	if(*u < U[0] || *u > U[1] || *v < V[0] || *v > V[1])	// �p�����[�^�͈͊O
		return KOD_FALSE;

	return KOD_TRUE;
}

// Function: SearchIntersectPt
// (private)�j���[�g���@�ɂ���_��^�l�Ɏ���������(NURBS�Ȗʂƕ���)
// 
// Parameters:
// *nurb - NURBS�Ȗʂւ̃|�C���^
// pt - ���ʏ�̈�_
// nvec - ���ʂ̖@���x�N�g��
// ds - ��ǐՂ̍��ݕ�
// *u,*v - ��
// direction - �ǐՕ�����\���t���O�iFORWARD or INVERSE)
//
// Return:
// �����FKOD_TURE, �p�����[�^�͈͊O�FKOD_FALSE, ���s(���ٓ_�ɂ��[����)�FKOD_ERR
//int NURBS_Func::SearchIntersectPt(NURBSS *nurb,Coord pt,Coord nvec,double ds,double *u,double *v,int direction)
int NURBSS::SearchIntersectPt(Coord pt,Coord nvec,double ds,double *u,double *v,int direction)
{
	double d = CalcInnerProduct(pt,nvec);	// ���_���畽�ʂ܂ł̋���

	// �܂������l�Ƃ��Ă�du,dv�����߂�
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
	if(CheckZero(phi_u,MID_ACCURACY) == KOD_TRUE && CheckZero(phi_v,MID_ACCURACY) == KOD_TRUE){			// ���ٓ_
        //GuiIF.SetMessage("NURBS KOD_ERROR:The process is stoped by detected singular point.");
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
	else{									// dv<du�̏ꍇ��du��萔�Ƃ��ČŒ肷��
		while(!CheckZero(dv,MID_ACCURACY)){		// dv����������܂ŌJ��Ԃ��v�Z
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
// NURBS�Ȗʂ�NURBS�Ȑ��Ƃ̌�_�Q������ǐՖ@�ŋ��߂�
//
// Parameters:
// *NurbsS, *NurbsC - NURBS�Ȗʂ�NURBS�Ȑ��ւ̃|�C���^   
// Divnum - �����_�T�[�`���̋Ȑ�������   
// *ans - ��  
// ans_size - ans�z��̔z��
//
// Return:
// ��_�̐��i���̐���ans�̃T�C�Y�𒴂����ꍇ�FKOD_ERR�j
int NURBS_Func::CalcIntersecPtsNurbsSNurbsC(NURBSS *NurbsS,NURBSC *NurbsC,int Divnum,Coord *ans,int ans_size)
{
	Coord d = SetCoord(100,100,100);		// NURBS�Ȑ�S(u,v)�̔����ω���(du,dv)�A����N(t)�̔����ω���dt���i�[
	Coord F,Fu,Fv,Ft;						// F(u,v,t) = S(u,v) - N(t)    Fu = dF/du     Fv = dF/dv     Ft = dF/dt
	double u = NurbsS->U[0];				// NURBS�Ȗ�S(u,v)��u�p�����[�^�̌��ݒl
	double v = NurbsS->V[0];				// NURBS�Ȗ�S(u,v)��v�p�����[�^�̌��ݒl
	double t = NurbsC->V[0];				// NURBS�Ȑ�C(t)��t�p�����[�^
	Matrix A = NewMatrix(3,3);				// Fu,Fv,Ft���\������3x3�s��
	Matrix A_ = NewMatrix(3,3);				// A�̋t�s����i�[
	bool flag = false;						// �����t���O
	double dt = (NurbsC->V[1] - NurbsC->V[0])/(double)Divnum;	// �������Z�p��t�p�����[�^�̃C���^�[�o���l
	int loopcount = 0;						// �����v�Z��
	int anscount = 0;						// �Z�o���ꂽ��_�̐�

	// t loop
	for(int i=0;i<Divnum;i++){
		t = NurbsC->V[0] + (double)i*dt;	// �X�e�b�v�p�����[�^t�̏����l���Z�b�g
		u = NurbsS->U[0];					// �X�e�b�v�p�����[�^u�̏����l���Z�b�g
		v = NurbsS->V[0];					// �X�e�b�v�p�����[�^v�̏����l���Z�b�g
		flag = false;						// �����t���O��OFF
		loopcount = 0;						// ���[�v�J�E���g������
		// �����̔����ω���dt(=d.z)��APPROX_ZERO�������܂Ńj���[�g���@�ɂ������v�Z���s��
		while(loopcount < LOOPCOUNTMAX){
//			F = SubCoord(CalcNurbsSCoord(NurbsS,u,v),CalcNurbsCCoord(NurbsC,t));	// F(u,v,t) = S(u,v) - C(t)
//			Fu = CalcDiffuNurbsS(NurbsS,u,v);			// Fu = dF/du = dS/du
//			Fv = CalcDiffvNurbsS(NurbsS,u,v);			// Fv = dF/dv = dS/dv
//			Ft = CalcDiffNurbsC(NurbsC,t);				// Ft = dF/dt = dC/dt
			F = SubCoord(NurbsS->CalcNurbsSCoord(u,v),NurbsC->CalcNurbsCCoord(t));	// F(u,v,t) = S(u,v) - C(t)
			Fu = NurbsS->CalcDiffuNurbsS(u,v);			// Fu = dF/du = dS/du
			Fv = NurbsS->CalcDiffvNurbsS(u,v);			// Fv = dF/dv = dS/dv
			Ft = NurbsC->CalcDiffNurbsC(t);				// Ft = dF/dt = dC/dt
			A[0][0] = Fu.x;				// Fu,Fv,Ft��3x3�s��A�ɑ��
			A[0][1] = Fv.x;				//     |Fu.x Fv.x Ft.x|       |du|       |F.x|
			A[0][2] = Ft.x;				// A = |Fu.y Fv.y Ft.y| , d = |dv| , F = |F.y|
			A[1][0] = Fu.y;				//     |Fu.z Fv.z Ft.z|       |dt|       |F.z|
			A[1][1] = Fv.y;
			A[1][2] = Ft.y;				// A�Ed = F   --->   d = A_�EF
			A[2][0] = Fu.z;
			A[2][1] = Fv.z;
			A[2][2] = Ft.z;	
			if(MatInv3(A,A_) == KOD_FALSE)	break;		// �t�s������߂�
			d = MulCoord(MulMxCoord(A_,F),-1);			// d���Z�o

			if(fabs(d.x) <= APPROX_ZERO && fabs(d.y) <= APPROX_ZERO && fabs(d.z) <= APPROX_ZERO){	// �^�l�Ɏ���������loop�𔲂���
				flag = true;		// �����t���Otrue
				break;
			}

			// �^�l�ɒB���Ă��Ȃ�������u,v,t���X�V
			u += d.x;
			v += d.y;
			t += d.z;

			if(u < NurbsS->U[0] || u > NurbsS->U[1] || v < NurbsS->V[0] || v > NurbsS->V[1] || t < NurbsC->V[0] || t > NurbsC->V[1]){	// u,v�̂ǂ��炩�����U������loop�𔲂���
				flag = false;		// �����t���Ofalse
				break;
			}

			loopcount++;
		}// end of while

		// �������Ă�������Ƃ��ēo�^
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

	anscount = CheckTheSamePoints(ans,anscount);		// ����_�͏�������

	return anscount;
}

// Function: CalcIntersecPtsNurbsSSearch
// NURBS�Ȗ�S(u,v)��NURBS�Ȗ�R(w,t)�̌��(��_�Q)����_�ǐՖ@�ɂċ��߂�
// 
// Parameters:
// nurbsS - NURBS�Ȗ�S(u,v) 
// nurbsR - NURBS�Ȗ�R(w,t) 
// div - �����_�T�[�`���̋Ȗʕ�����  
// ds - ���(��_�Q)�̑e��(��0.1�`2�a)  
// ans - ��  
// ans_size - ans�z��̔z��
//
// Return:
// ��_�̐��iNURBS�Ȗʓ��m���������Ă��Ȃ��FKOD_FALSE�C���ٓ_�܂��͔��U�ɂ�菈���𒆒f�FKOD_ERR�j
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
//		init_pt_Coord_R[i] = CalcNurbsSCoord(nurbR,init_pt_R[i].x,init_pt_R[i].y);		// ��_��uv�p�����[�^��xyz���W�l�ɕϊ��������̂�ێ����Ă���
//		init_pt_Coord_S[i] = CalcNurbsSCoord(nurbS,init_pt_S[i].x,init_pt_S[i].y);		// ��_��uv�p�����[�^��xyz���W�l�ɕϊ��������̂�ێ����Ă���
		init_pt_Coord_R[i] = nurbR->CalcNurbsSCoord(init_pt_R[i].x,init_pt_R[i].y);		// ��_��uv�p�����[�^��xyz���W�l�ɕϊ��������̂�ێ����Ă���
		init_pt_Coord_S[i] = nurbS->CalcNurbsSCoord(init_pt_S[i].x,init_pt_S[i].y);		// ��_��uv�p�����[�^��xyz���W�l�ɕϊ��������̂�ێ����Ă���
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
				if(search_flag != KOD_TRUE)						// uv�p�����[�^�O�ɏo����
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

//			Coord pr = CalcNurbsSCoord(nurbR,w,t);			// ����ꂽu,v��xyz���W�l�ɕϊ�
//			Coord ps = CalcNurbsSCoord(nurbS,u,v);			// ����ꂽu,v��xyz���W�l�ɕϊ�
			Coord pr = nurbR->CalcNurbsSCoord(w,t);			// ����ꂽu,v��xyz���W�l�ɕϊ�
			Coord ps = nurbS->CalcNurbsSCoord(u,v);			// ����ꂽu,v��xyz���W�l�ɕϊ�
			double distr = CalcDistance(init_pt_Coord_R[pnow],pr);	// ����ꂽxyz���W�l�Ə����_�Ƃ̋������Z�o
			double dists = CalcDistance(init_pt_Coord_S[pnow],ps);	// ����ꂽxyz���W�l�Ə����_�Ƃ̋������Z�o
			
			// ��_�̌������~�b�g���z������
			if(ans_count >= ans_size-1){
                GuiIF.SetMessage("NURBS KOD_ERROR:Intersection points exceeded the allocated array length");
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

// Function: CalcIntersecPtsNurbsSGeom
// NURBS�Ȗ�S(u,v)��NURBS�Ȗ�R(w,t)�̌��(��_�Q)���􉽊w�I�ɋ��߂�(�⏕���ʂ�p������@)
//
// Parameters:
// *nurbS - NURBS�Ȗ�S(u,v) 
// *nurbR - NURBS�Ȗ�R(w,t) 
// u_divnum - u�p�����[�^�������@
// v_divnum - v�p�����[�^������
// *ans - �Z�o���ꂽ��_��u,v�p�����[�^�l�����ꂼ��ans.x,ans.y�Ɋi�[
// ans_size - ans�̔z��
//
// Return:
// ��_�̌�
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
//						Coord p0 = CalcNurbsSCoord(nurbR,w0,t0);					// R(w0,t0)�ƂȂ�_(�����_)�̍��W
//						Coord q0 = CalcNurbsSCoord(nurbS,u0,v0);					// S(u0,v0)�ƂȂ�_(�����_)�̍��W
//						Coord rw = CalcDiffuNurbsS(nurbR,w0,t0);					// �_R(w0,t0)��u�Δ���(��{�x�N�g��)
//						Coord rt = CalcDiffvNurbsS(nurbR,w0,t0);					// �_R(w0,t0)��v�Δ���(��{�x�N�g��)
						Coord p0 = nurbR->CalcNurbsSCoord(w0,t0);					// R(w0,t0)�ƂȂ�_(�����_)�̍��W
						Coord q0 = nurbS->CalcNurbsSCoord(u0,v0);					// S(u0,v0)�ƂȂ�_(�����_)�̍��W
						Coord rw = nurbR->CalcDiffuNurbsS(w0,t0);					// �_R(w0,t0)��u�Δ���(��{�x�N�g��)
						Coord rt = nurbR->CalcDiffvNurbsS(w0,t0);					// �_R(w0,t0)��v�Δ���(��{�x�N�g��)
						double rwt = CalcEuclid(CalcOuterProduct(rw,rt));
						if(rwt==0.0) break;
						Coord np = DivCoord(CalcOuterProduct(rw,rt),rwt);			// �_R(w0,t0)�̒P�ʖ@���x�N�g��
//						Coord su = CalcDiffuNurbsS(nurbS,u0,v0);					// �_S(u0,v0)��u�Δ���(��{�x�N�g��)
//						Coord sv = CalcDiffvNurbsS(nurbS,u0,v0);					// �_S(u0,v0)��v�Δ���(��{�x�N�g��)
						Coord su = nurbS->CalcDiffuNurbsS(u0,v0);					// �_S(u0,v0)��u�Δ���(��{�x�N�g��)
						Coord sv = nurbS->CalcDiffvNurbsS(u0,v0);					// �_S(u0,v0)��v�Δ���(��{�x�N�g��)
						double suv = CalcEuclid(CalcOuterProduct(su,sv));
						if(suv==0.0) break;
						Coord nq = DivCoord(CalcOuterProduct(su,sv),suv);			// �_S(u0,v0)�̒P�ʖ@���x�N�g��
						double npq = CalcEuclid(CalcOuterProduct(np,nq));
						if(npq==0.0) break;
						Coord nn = DivCoord(CalcOuterProduct(np,nq),npq);			// ����Fp�ƕ���Fq�ɒ������镽��Fn�̒P�ʖ@���x�N�g��
						double dp = CalcInnerProduct(p0,np);						// ���_���畽��Fp�܂ł̋���
						double dq = CalcInnerProduct(q0,nq);						// ���_���畽��Fq�܂ł̋���
						double dn = CalcInnerProduct(p0,nn);						// ���_���畽��Fn�܂ł̋���
						Coord cross_nqn = CalcOuterProduct(nq,nn);					// �P�ʖ@���x�N�g��nq,nn�̃x�N�g����
						Coord cross_nnp = CalcOuterProduct(nn,np);					// �P�ʖ@���x�N�g��nn,np�̃x�N�g����
						Coord cross_npq = CalcOuterProduct(np,nq);					// �P�ʖ@���x�N�g��np,nq�̃x�N�g����
						Coord nume_p_sub =  AddCoord(MulCoord(cross_nqn,dp),MulCoord(cross_nnp,dq));	// 3����Fp,Fq,Fn�̌�_p�̕��q�̍ŏ���2�����v�Z
						Coord nume_p = AddCoord(nume_p_sub,MulCoord(cross_npq,dn));			// p�̕��q���Z�o
						double denom_p = CalcScalarTriProduct(np,nq,nn);			// p�̕�����Z�o
						Coord p = DivCoord(nume_p,denom_p);							// p���Z�o
						Coord deltap0 = SubCoord(p,p0);								// �_p�Ɠ_p0�̍��x�N�g�����Z�o
						Coord deltaq0 = SubCoord(p,q0);								// �_p�Ɠ_q0�̍��x�N�g�����Z�o
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

// Function: SearchIntersectPt
// (private)�j���[�g���@�ɂ���_��^�l�Ɏ���������(NURBS�Ȗʓ��m)
// 
// Parameters:
// *nurbR - NURBS�Ȗ�S(u,v)
// *nurbS - NURBS�Ȗ�R(w,t) 
// ds - ��ǐՎ��̍��ݕ�
// *w,*t,*u,*v - ��
// direction - �ǐՕ�����\���t���O�iFORWARD or INVERSE)
//
// Return:
// ���������FKOD_TRUE, �p�����[�^�͈͊O�FKOD_FALSE, ���ٓ_���o�FKOD_ERR
int NURBS_Func::SearchIntersectPt(NURBSS *nurbR,NURBSS *nurbS,double ds,double *w,double *t,double *u,double *v,int direction)
{
	double **J;
	double D[3];
	double ans[3];
	int flag = KOD_TRUE;

	// �������m��
	if((J = NewMatrix(3,3)) == NULL){
        GuiIF.SetMessage("NURBS ERROR: fail to malloc\n");
		return KOD_ERR;
	}

	// �܂������l�Ƃ��Ă�dw,dt,du,dv�����߂�
//	Coord r = CalcNurbsSCoord(nurbR,*w,*t);					// �_R(w,t)��NURBS�Ȗʂ̍��W�l�����߂�
//	Coord s = CalcNurbsSCoord(nurbS,*u,*v);					// �_S(u,v)��NURBS�Ȗʂ̍��W�l�����߂�
//	Coord rw = CalcDiffuNurbsS(nurbR,*w,*t);					// �_R(w,t)��u�Δ���(��{�x�N�g��)
//	Coord rt = CalcDiffvNurbsS(nurbR,*w,*t);					// �_R(w,t)��v�Δ���(��{�x�N�g��)
//	Coord su = CalcDiffuNurbsS(nurbS,*u,*v);					// �_S(u,v)��u�Δ���(��{�x�N�g��)
//	Coord sv = CalcDiffvNurbsS(nurbS,*u,*v);					// �_S(u,v)��v�Δ���(��{�x�N�g��)
	Coord r = nurbR->CalcNurbsSCoord(*w,*t);					// �_R(w,t)��NURBS�Ȗʂ̍��W�l�����߂�
	Coord s = nurbS->CalcNurbsSCoord(*u,*v);					// �_S(u,v)��NURBS�Ȗʂ̍��W�l�����߂�
	Coord rw = nurbR->CalcDiffuNurbsS(*w,*t);					// �_R(w,t)��u�Δ���(��{�x�N�g��)
	Coord rt = nurbR->CalcDiffvNurbsS(*w,*t);					// �_R(w,t)��v�Δ���(��{�x�N�g��)
	Coord su = nurbS->CalcDiffuNurbsS(*u,*v);					// �_S(u,v)��u�Δ���(��{�x�N�g��)
	Coord sv = nurbS->CalcDiffvNurbsS(*u,*v);					// �_S(u,v)��v�Δ���(��{�x�N�g��)
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
	if(!phi1 && !phi2){			// ���ٓ_
        GuiIF.SetMessage("NURBS KOD_ERROR:The process is stoped by detected singular point.");
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
//			r = CalcNurbsSCoord(nurbR,*w,*t);						// �_R(w,t)��NURBS�Ȗʂ̍��W�l�����߂�
//			s = CalcNurbsSCoord(nurbS,*u,*v);						// �_S(u,v)��NURBS�Ȗʂ̍��W�l�����߂�
//			rw = CalcDiffuNurbsS(nurbR,*w,*t);						// �_R(w,t)��u�Δ���(��{�x�N�g��)
//			rt = CalcDiffvNurbsS(nurbR,*w,*t);						// �_R(w,t)��v�Δ���(��{�x�N�g��)
//			su = CalcDiffuNurbsS(nurbS,*u,*v);						// �_S(u,v)��u�Δ���(��{�x�N�g��)
//			sv = CalcDiffvNurbsS(nurbS,*u,*v);						// �_S(u,v)��v�Δ���(��{�x�N�g��)
			r = nurbR->CalcNurbsSCoord(*w,*t);						// �_R(w,t)��NURBS�Ȗʂ̍��W�l�����߂�
			s = nurbS->CalcNurbsSCoord(*u,*v);						// �_S(u,v)��NURBS�Ȗʂ̍��W�l�����߂�
			rw = nurbR->CalcDiffuNurbsS(*w,*t);						// �_R(w,t)��u�Δ���(��{�x�N�g��)
			rt = nurbR->CalcDiffvNurbsS(*w,*t);						// �_R(w,t)��v�Δ���(��{�x�N�g��)
			su = nurbS->CalcDiffuNurbsS(*u,*v);						// �_S(u,v)��u�Δ���(��{�x�N�g��)
			sv = nurbS->CalcDiffvNurbsS(*u,*v);						// �_S(u,v)��v�Δ���(��{�x�N�g��)
			
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
//			r = CalcNurbsSCoord(nurbR,*w,*t);					// �_R(w,t)��NURBS�Ȗʂ̍��W�l�����߂�
//			s = CalcNurbsSCoord(nurbS,*u,*v);					// �_S(u,v)��NURBS�Ȗʂ̍��W�l�����߂�
//			rw = CalcDiffuNurbsS(nurbR,*w,*t);					// �_R(w,t)��u�Δ���(��{�x�N�g��)
//			rt = CalcDiffvNurbsS(nurbR,*w,*t);					// �_R(w,t)��v�Δ���(��{�x�N�g��)
//			su = CalcDiffuNurbsS(nurbS,*u,*v);					// �_S(u,v)��u�Δ���(��{�x�N�g��)
//			sv = CalcDiffvNurbsS(nurbS,*u,*v);					// �_S(u,v)��v�Δ���(��{�x�N�g��)
			r = nurbR->CalcNurbsSCoord(*w,*t);					// �_R(w,t)��NURBS�Ȗʂ̍��W�l�����߂�
			s = nurbS->CalcNurbsSCoord(*u,*v);					// �_S(u,v)��NURBS�Ȗʂ̍��W�l�����߂�
			rw = nurbR->CalcDiffuNurbsS(*w,*t);					// �_R(w,t)��u�Δ���(��{�x�N�g��)
			rt = nurbR->CalcDiffvNurbsS(*w,*t);					// �_R(w,t)��v�Δ���(��{�x�N�g��)
			su = nurbS->CalcDiffuNurbsS(*u,*v);					// �_S(u,v)��u�Δ���(��{�x�N�g��)
			sv = nurbS->CalcDiffvNurbsS(*u,*v);					// �_S(u,v)��v�Δ���(��{�x�N�g��)
			
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
//			r = CalcNurbsSCoord(nurbR,*w,*t);					// �_R(w,t)��NURBS�Ȗʂ̍��W�l�����߂�
//			s = CalcNurbsSCoord(nurbS,*u,*v);					// �_S(u,v)��NURBS�Ȗʂ̍��W�l�����߂�
//			rw = CalcDiffuNurbsS(nurbR,*w,*t);					// �_R(w,t)��u�Δ���(��{�x�N�g��)
//			rt = CalcDiffvNurbsS(nurbR,*w,*t);					// �_R(w,t)��v�Δ���(��{�x�N�g��)
//			su = CalcDiffuNurbsS(nurbS,*u,*v);					// �_S(u,v)��u�Δ���(��{�x�N�g��)
//			sv = CalcDiffvNurbsS(nurbS,*u,*v);					// �_S(u,v)��v�Δ���(��{�x�N�g��)
			r = nurbR->CalcNurbsSCoord(*w,*t);					// �_R(w,t)��NURBS�Ȗʂ̍��W�l�����߂�
			s = nurbS->CalcNurbsSCoord(*u,*v);					// �_S(u,v)��NURBS�Ȗʂ̍��W�l�����߂�
			rw = nurbR->CalcDiffuNurbsS(*w,*t);					// �_R(w,t)��u�Δ���(��{�x�N�g��)
			rt = nurbR->CalcDiffvNurbsS(*w,*t);					// �_R(w,t)��v�Δ���(��{�x�N�g��)
			su = nurbS->CalcDiffuNurbsS(*u,*v);					// �_S(u,v)��u�Δ���(��{�x�N�g��)
			sv = nurbS->CalcDiffvNurbsS(*u,*v);					// �_S(u,v)��v�Δ���(��{�x�N�g��)
			
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
//			r = CalcNurbsSCoord(nurbR,*w,*t);					// �_R(w,t)��NURBS�Ȗʂ̍��W�l�����߂�
//			s = CalcNurbsSCoord(nurbS,*u,*v);					// �_S(u,v)��NURBS�Ȗʂ̍��W�l�����߂�
//			rw = CalcDiffuNurbsS(nurbR,*w,*t);					// �_R(w,t)��u�Δ���(��{�x�N�g��)
//			rt = CalcDiffvNurbsS(nurbR,*w,*t);					// �_R(w,t)��v�Δ���(��{�x�N�g��)
//			su = CalcDiffuNurbsS(nurbS,*u,*v);					// �_S(u,v)��u�Δ���(��{�x�N�g��)
//			sv = CalcDiffvNurbsS(nurbS,*u,*v);					// �_S(u,v)��v�Δ���(��{�x�N�g��)
			r = nurbR->CalcNurbsSCoord(*w,*t);					// �_R(w,t)��NURBS�Ȗʂ̍��W�l�����߂�
			s = nurbS->CalcNurbsSCoord(*u,*v);					// �_S(u,v)��NURBS�Ȗʂ̍��W�l�����߂�
			rw = nurbR->CalcDiffuNurbsS(*w,*t);					// �_R(w,t)��u�Δ���(��{�x�N�g��)
			rt = nurbR->CalcDiffvNurbsS(*w,*t);					// �_R(w,t)��v�Δ���(��{�x�N�g��)
			su = nurbS->CalcDiffuNurbsS(*u,*v);					// �_S(u,v)��u�Δ���(��{�x�N�g��)
			sv = nurbS->CalcDiffvNurbsS(*u,*v);					// �_S(u,v)��v�Δ���(��{�x�N�g��)
			
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



// Function: GetNurbsSCoef
// (private)CalcIntersecPtsPlaneU/V3()�̃T�u�֐��DNURBS�Ȑ�C(u) or C(v)�̌W�������߂�
//
// Parameters:
// M - �K��
// **coef - B�X�v���C�����֐��̌W�� 
// *a,*b - u/v���Œ肵������NURBS�Ȑ�C(v)/C(u)�̕���/���q�̌W�� 
// i - �Ȑ��̔ԍ�
// *P, *Q - �Œ肳�ꂽ�p�����[�^�ɂ�����NURBS�Ȗʂ̌W��(P,Q) 
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
// (x,y)���ʏ��NURBS�Ȑ����m�̌�_�����߂�(�j���[�g���@)
// 
// Parameters:
// *NurbA, *NurbB - NURBS�Ȑ�
// Divnum - �����l�ݒ�p�̕�����     
// *ans - ��_�i�[�p�z��   
// ans_size - ans�̔z��
// 
// Return:
// ���̌��ians_size�𒴂�����CKOD_ERR�j
int NURBS_Func::CalcIntersecPtsNurbsCNurbsCParam(NURBSC *NurbA,NURBSC *NurbB,int Divnum,Coord *ans,int ans_size)
{
	double t = NurbA->V[0];		// ���݂�NurbA�̃p�����[�^�l
	double u = 0;				// ���݂�NurbB�̃p�����[�^�l
	double dt = 0;				// �j���[�g���@�ɂ��t�p�����[�^�̍X�V��
	double du = 0;				// �j���[�g���@�ɂ��u�p�����[�^�̍X�V��
	Coord F;					// �j���[�g���@�̑ΏۂƂ��������(F(t,u) = NurbA(t) - NurbB(u))
	Coord Ft;					// F��t�ɂ������l
	Coord Fu;					// F��u�ɂ������l
	double d = (NurbA->V[1] - NurbA->V[0])/(double)Divnum;	// �����_�̑����l
	int loopcount = 0;			// ���[�v��
	bool flag = false;			// �����t���O
	int anscount = 0;			// ��_�̐����J�E���g
	Matrix A = NewMatrix(2,2);	// Ft,Fu�𐬕����ƂɊi�[�����s��
	Matrix A_ = NewMatrix(2,2);	// A�̋t�s����i�[
	

	for(int i=0;i<Divnum;i++){
		flag = false;
		loopcount = 0;
		t = NurbA->V[0] + (double)i*d;		// �����l�X�V
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

			if(CheckZero(dt,HIGH_ACCURACY) == KOD_TRUE && CheckZero(du,HIGH_ACCURACY) == KOD_TRUE){		// �X�V�l��臒l�ȉ��ɂȂ�����Awhile�𔲂��A���Ƃ��ēo�^
				flag = true;
				break;
			}
			t += dt;		// �p�����[�^�l�X�V
			u += du;
			if(t < NurbA->V[0] || t > NurbA->V[1] || u < NurbB->V[0] || u > NurbB->V[1]){		// �p�����[�^�͈͂𒴂�����Awhile�𔲂��A���̏����l�ֈڍs
				flag = false;
				break;
			}
			loopcount++;
		}// end of wihle
		if(flag == true){
			ans[anscount].x = t;		// ���Ƃ��ēo�^
			ans[anscount].y = u;
			anscount++;
			if(anscount == ans_size){	// ���̌���ans_size�𒴂�����AERR�����^�[��
                GuiIF.SetMessage("NURBS_Func ERROR: Ans_size overflow");
				return KOD_ERR;
			}
		}
	}// end of i loop

	anscount = CheckTheSamePoints2D(ans,anscount);		// ����_�͏�������

	FreeMatrix(A,2);
	FreeMatrix(A_,2);

	return anscount;
}

// Function: ClacIntersecPtsNurbsCLine
// 2����NURBS�Ȑ��ƒ����Ƃ̌�_�����߂�
//
// Parameters:
// *C - NURBS�Ȑ�
// P - �������1�_
// r - �����̕����x�N�g��
// *t1 - ��_�ɂ�����NURBS�Ȑ��p�����[�^
// *t2 - ��_�ɂ����钼���p�����[�^
//
// return:
// ��_�̗L���iKOD_TRUE�F��_����C KOD_FALSE�F��_�Ȃ��j
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
        if(CheckZero(dt1,LOW_ACCURACY) == KOD_TRUE && CheckZero(dt2,LOW_ACCURACY) == KOD_TRUE){		// ��_�Ɏ���������while break
            conv_flag = true;
            break;
        }
        else{
            *t1 += dt1/3;
            *t2 += dt2/3;
        }
        if(*t1 < V[0] || *t1 > V[1])	// ���ݒ��ڒ��̃G�b�W�͈̔͂𒴂�����break
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
// 2����NURBS�Ȑ��Ɛ����Ƃ̌�_�����߂�
//
// Parameters:
// *C - NURBS�Ȑ�
// P - �������1�_
// r - �����̕����x�N�g��
// ts - �����̎n�_�p�����[�^
// te - �����̏I�_�p�����[�^
// *t1 - ��_�ɂ�����NURBS�Ȑ��p�����[�^
// *t2 - ��_�ɂ����钼���p�����[�^
//
// return:
// ��_�̗L���iKOD_TRUE�F��_����C KOD_FALSE�F��_�Ȃ��j
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
// NURBS�Ȑ��ƕ��ʂƂ̌�_�����߂�(�j���[�g���@)
// F(t) = nvec�E(C(t)-pt) = 0���j���[�g���@��p���ċ��߂�D
// ��_�͍ő��(M-1)*(K-M+1)�_������.  (��F4�K�ŃR���g���[���|�C���g�̐�8�̏ꍇ�A(4-1)*(8-4+1)=15�_)�D
// ����Ĉ���*ans��(M-1)*(K-M+1)�̔z���p�ӂ��邱�Ƃ��]�܂���.
//
// Parameters:
// *nurb - NURBS�Ȑ�  
// pt - ���ʏ�̈�_  
// nvec - ���ʂ̖@���x�N�g��   
// Divnum - NURBS�Ȑ��̃p�����[�^������  
// *ans - �Z�o���ꂽ��_��t�p�����[�^�l���i�[
// ans_size - ans�̔z��
// LoD - �ڍדx�i�j���[�g���@�̍X�V�p�����[�^�𑫂����ނƂ��ɁCLoD�Ŋ��邱�ƂŁC�}���ȃp�����[�^�ύX�������D�ʏ��1�ł悢���C���������Ȃ��ꍇ�͒l��傫������D2�Ƃ�3�Ƃ��j
//
// Return:
// ��_�̌��iKOD_ERR�F��_�̐���ans_size�𒴂����j
//int NURBS_Func::CalcIntersecCurve(NURBSC *nurb,Coord pt,Coord nvec,int Divnum,double *ans,int ans_size,int LoD)
int NURBSC::CalcIntersecCurve(Coord pt,Coord nvec,int Divnum,double *ans,int ans_size,int LoD)
{
	double t = V[0];		// ���݂�NURBS�Ȑ��̃p�����[�^�l
	double d = 0;				// �j���[�g���@�ɂ��p�����[�^�̍X�V��
	double F;					// �j���[�g���@�̑ΏۂƂ��������
	double Ft;					// F��t�ɂ������l
	double dt = (V[1] - V[0])/(double)Divnum;	// �����_�̑����l
	int loopcount = 0;			// ���[�v��
	bool flag = false;			// �����t���O
	int anscount = 0;			// ��_�̐����J�E���g

	if(!LoD){
		GuiIF.SetMessage("NURBS_Func ERROR: LoD is changed 0 to 1");
		LoD = 1;
	}

	for(int i=0;i<=Divnum;i++){
		flag = false;
		loopcount = 0;
		t = V[0] + (double)i*dt;		// �����l�X�V
		while(loopcount < LOOPCOUNTMAX){
//			F = CalcInnerProduct(nvec,SubCoord(CalcNurbsCCoord(nurb,t),pt));
//			Ft = CalcInnerProduct(nvec,CalcDiffNurbsC(nurb,t));
			F = CalcInnerProduct(nvec,SubCoord(CalcNurbsCCoord(t),pt));
			Ft = CalcInnerProduct(nvec,CalcDiffNurbsC(t));
			d = -F/Ft;		// �X�V�l
			//fprintf(stderr,"   %d:%.14lf,%lf\n",i,d,t);	// for debug
			if(CheckZero(d,HIGH_ACCURACY) == KOD_TRUE){		// �X�V�l��臒l�ȉ��ɂȂ�����Awhile�𔲂��A���Ƃ��ēo�^
				flag = true;
				break;
			}
			t += d/(double)LoD;		// �p�����[�^�l�X�V
			
			if(t < V[0] || t > V[1]){		// �p�����[�^�͈͂𒴂�����Awhile�𔲂��A���̏����l�ֈڍs
				flag = false;
				break;
			}
			loopcount++;
		}// end of wihle
		if(flag == true){
			if(anscount == ans_size){	// ���̌���ans_size�𒴂�����AERR�����^�[��
				GuiIF.SetMessage("NURBS_Func ERROR: Ans_size overflow");
				return KOD_ERR;
			}
			ans[anscount] = t;		// ���Ƃ��ēo�^
			anscount++;
		}
	}// end of i loop

	anscount = CheckTheSamePoints(ans,anscount);		// ����_�͏�������

	return anscount;
}

// Function: CalcIntersecIsparaCurveU
// NURBS�Ȗʂ�U�����A�C�\�p���Ȑ�(V�p�����[�^���Œ�)�ƕ��ʂƂ̌�_�����߂�(�j���[�g���@)
// F(t) = nvec�E(C(t)-pt) = 0���j���[�g���@��p���ċ��߂�
// ��_�͍ő��(M-1)*(K-M+1)�_������.  (��F4�K�ŃR���g���[���|�C���g�̐�8�̏ꍇ�A(4-1)*(8-4+1)=15�_)
// ����Ĉ���*ans��(M-1)*(K-M+1)�̔z���p�ӂ��邱�Ƃ��]�܂���.
//
// Parameters:
// *nurb - NURBS�Ȗ�  
// V - v�̌Œ�l  
// pt - ���ʏ�̈�_  
// nvec - ���ʂ̖@���x�N�g��  
// Divnum - NURBS�Ȑ��̃p�����[�^������  
// *ans - �Z�o���ꂽ��_��t�p�����[�^�l���i�[  
// ans_size - ans�̔z��
//
// Return:
// ��_�̌��iKOD_ERR:��_�̐���ans_size�𒴂����j
//int NURBS_Func::CalcIntersecIsparaCurveU(NURBSS *nurb,double V,Coord pt,Coord nvec,int Divnum,double *ans,int ans_size)
int NURBSS::CalcIntersecIsparaCurveU(double V,Coord pt,Coord nvec,int Divnum,double *ans,int ans_size)
{
	double d = 0;				// �j���[�g���@�ɂ��p�����[�^�̍X�V��
	double F;					// �j���[�g���@�̑ΏۂƂ��������
	double Fu;					// F��u�ɂ������l
	int loopcount = 0;			// ���[�v��
	bool flag = false;			// �����t���O
	int anscount = 0;			// ��_�̐����J�E���g
	double u = U[0];		// ���݂�NURBS�Ȑ��̃p�����[�^�l
	double du = (U[1] - U[0])/(double)Divnum;	// �����_�̑����l

	for(int i=0;i<=Divnum;i++){
		flag = false;
		loopcount = 0;
		u = U[0] + (double)i*du;		// �����l�X�V
		while(loopcount < LOOPCOUNTMAX){
//			F = CalcInnerProduct(nvec,SubCoord(CalcNurbsSCoord(nurb,u,V),pt));
//			Fu = CalcInnerProduct(nvec,CalcDiffuNurbsS(nurb,u,V));
			F = CalcInnerProduct(nvec,SubCoord(CalcNurbsSCoord(u,V),pt));
			Fu = CalcInnerProduct(nvec,CalcDiffuNurbsS(u,V));
			if(CheckZero(Fu,MID_ACCURACY) == KOD_TRUE)	break;
			d = -F/Fu;		// �X�V�l
			if(CheckZero(d,MID_ACCURACY) == KOD_TRUE){		// �X�V�l��臒l�ȉ��ɂȂ�����Awhile�𔲂��A���Ƃ��ēo�^
				flag = true;
				break;
			}
			u += d;		// �p�����[�^�l�X�V
			if(u < U[0] || u > U[1]){		// �p�����[�^�͈͂𒴂�����Awhile�𔲂��A���̏����l�ֈڍs
				flag = false;
				break;
			}
			loopcount++;
		}// end of wihle
		if(flag == true){
			anscount = CheckTheSamePoints(ans,anscount);		// ����_�͏�������
			if(anscount == ans_size){	// ���̌���ans_size�𒴂�����AERR�����^�[��
				GuiIF.SetMessage("NURBS_Func ERROR: Ans_size overflow");
				return KOD_ERR;
			}
			ans[anscount] = u;		// ���Ƃ��ēo�^
			anscount++;
		}
	}// end of i loop

	anscount = CheckTheSamePoints(ans,anscount);		// ����_�͏�������

	return anscount;
}

// Function: CalcIntersecIsparaCurveV
// NURBS�Ȗʂ�V�����A�C�\�p���Ȑ�(U�p�����[�^���Œ�)�ƕ��ʂƂ̌�_�����߂�(�j���[�g���@)
// F(t) = nvec�E(C(t)-pt) = 0���j���[�g���@��p���ċ��߂�
// ��_�͍ő��(M-1)*(K-M+1)�_������.  (��F4�K�ŃR���g���[���|�C���g�̐�8�̏ꍇ�A(4-1)*(8-4+1)=15�_)
// ����Ĉ���*ans��(M-1)*(K-M+1)�̔z���p�ӂ��邱�Ƃ��]�܂���.
//
// Parameters:
// *nurb - NURBS�Ȗ�  
// U - u�̌Œ�l   
// pt - ���ʏ�̈�_  
// nvec - ���ʂ̖@���x�N�g��
// Divnum - NURBS�Ȑ��̃p�����[�^������  
// *ans - �Z�o���ꂽ��_��t�p�����[�^�l���i�[
// ans_size - ans�̔z��
//
// Return:
// ��_�̌��iKOD_ERR:��_�̐���ans_size�𒴂����j
//int NURBS_Func::CalcIntersecIsparaCurveV(NURBSS *nurb,double U,Coord pt,Coord nvec,int Divnum,double *ans,int ans_size)
int NURBSS::CalcIntersecIsparaCurveV(double U,Coord pt,Coord nvec,int Divnum,double *ans,int ans_size)
{
	double d = 0;				// �j���[�g���@�ɂ��p�����[�^�̍X�V��
	double F;					// �j���[�g���@�̑ΏۂƂ��������
	double Fv;					// F��v�ɂ������l
	int loopcount = 0;			// ���[�v��
	bool flag = false;			// �����t���O
	int anscount = 0;			// ��_�̐����J�E���g
	double v = V[0];		// ���݂�NURBS�Ȑ��̃p�����[�^�l
	double dv = (V[1] - V[0])/(double)Divnum;	// �����_�̑����l

	for(int i=0;i<=Divnum;i++){
		flag = false;
		loopcount = 0;
		v = V[0] + (double)i*dv;		// �����l�X�V
		while(loopcount < LOOPCOUNTMAX){
//			F = CalcInnerProduct(nvec,SubCoord(CalcNurbsSCoord(nurb,U,v),pt));
//			Fv = CalcInnerProduct(nvec,CalcDiffvNurbsS(nurb,U,v));
			F = CalcInnerProduct(nvec,SubCoord(CalcNurbsSCoord(U,v),pt));
			Fv = CalcInnerProduct(nvec,CalcDiffvNurbsS(U,v));
			if(CheckZero(Fv,MID_ACCURACY) == KOD_TRUE)	break;
			d = -F/Fv;		// �X�V�l
			if(CheckZero(d,MID_ACCURACY) == KOD_TRUE){		// �X�V�l��臒l�ȉ��ɂȂ�����Awhile�𔲂��A���Ƃ��ēo�^
				flag = true;
				break;
			}
			//fprintf(stderr,"   %lf,%lf,%lf,%lf\n",v,d,F,Fv); //for debug
			v += d;		// �p�����[�^�l�X�V
			if(v < V[0] || v > V[1]){		// �p�����[�^�͈͂𒴂�����Awhile�𔲂��A���̏����l�ֈڍs
				flag = false;
				break;
			}
			loopcount++;
		}// end of wihle
		if(flag == true){
			anscount = CheckTheSamePoints(ans,anscount);		// ����_�͏�������
			if(anscount == ans_size){	// ���̌���ans_size�𒴂�����AERR�����^�[��
				GuiIF.SetMessage("NURBS_Func ERROR: Ans_size overflow");
				return KOD_ERR;
			}
			ans[anscount] = v;		// ���Ƃ��ēo�^
			anscount++;
		}
	}// end of i loop

	anscount = CheckTheSamePoints(ans,anscount);		// ����_�͏�������

	return anscount;
}

// Function: CalcIntersecCurve3 
// NURBS�Ȑ��ƕ��ʂƂ̌�_�����߂�(3���܂őΉ�)
// ��_�͍ő��(M-1)*(K-M+1)�_������.  (��F4�K�ŃR���g���[���|�C���g�̐�8�̏ꍇ�A(4-1)*(8-4+1)=15�_)
// ����Ĉ���*ans��(M-1)*(K-M+1)�̔z���p�ӂ��邱�Ƃ��]�܂���.
// 
// Parameters:
// *nurb - NURBS�Ȑ�  
// pt - ���ʏ�̈�_  
// nvec - ���ʂ̖@���x�N�g��  
// *ans - �Z�o���ꂽ��_��t�p�����[�^�l���i�[
// ans_size - ans�̔z��
//
// Return:
// ��_�̌��i�Ȑ�������3���ȏ�FKOD_ERR�j
//int NURBS_Func::CalcIntersecCurve3(NURBSC *nurb,Coord pt,Coord nvec,double *ans,int ans_size)
int NURBSC::CalcIntersecCurve3(Coord pt,Coord nvec,double *ans,int ans_size)
{
	double **coef;
	double Q[4];	// NURBS�Ȑ��̕���̌W��
	Coord  P[4];	// NURBS�Ȑ��̕��q�̌W��
	double a[4];
	double t[3];
	int ansnum;
	int k=0;

	if((coef = NewMatrix(M,M)) == NULL){
        GuiIF.SetMessage("NURBS KOD_ERROR: CalcIntersecPlane3()");
		return KOD_ERR;
	}

	// 1�{��NURBS�Ȑ���K-M+1�{�̋Ȑ�����\�������B���ꂼ��̍\���Ȑ��ɑ΂��ĕ������𓱏o���A���𓾂�B
	for(int i=0;i<K-M+1;i++){
		if(M-1 == 3){			// 3��			
			GetBSplCoef3(M,K,i,T,coef);	// �e�R���g���[���|�C���g�ɂ�����3��B�X�v���C�����֐��̌W��(coef)�����߂�
//			GetBSplCoef3(M,K,i,T.get(),coef);	// �e�R���g���[���|�C���g�ɂ�����3��B�X�v���C�����֐��̌W��(coef)�����߂�
		}
		else if(M-1 == 2){	// 2��
			GetBSplCoef2(M,K,i,T,coef);	// �e�R���g���[���|�C���g�ɂ�����2��B�X�v���C�����֐��̌W�������߂�
//			GetBSplCoef2(M,K,i,T.get(),coef);	// �e�R���g���[���|�C���g�ɂ�����2��B�X�v���C�����֐��̌W�������߂�
		}
		else if(M-1 == 1){	// 1��	
			GetBSplCoef1(M,K,i,T,coef);	// �e�R���g���[���|�C���g�ɂ�����1��B�X�v���C�����֐��̌W�������߂�
//			GetBSplCoef1(M,K,i,T.get(),coef);	// �e�R���g���[���|�C���g�ɂ�����1��B�X�v���C�����֐��̌W�������߂�
		}
		else{
			char mes[256];
			sprintf(mes,"NURBS KOD_ERROR:Ther order of equation is unsupported. (order = %d)",M-1);
            GuiIF.SetMessage(mes);
			goto EXIT;
		}
		GetNurbsCCoef(coef,i,P,Q);						// NURBS�Ȑ��̌W��(P,Q)�����߂�
		GetIntersecEquation(M,P,Q,pt,nvec,a);			// NURBS�Ȑ��ƕ��ʂ̌�����o�p�������𓾂�
		ansnum = CalcEquation(a,t,M-1);					// �������������A��_�̃p�����[�^�l�𓾂�

		for(int j=0;j<ansnum;j++){
			if(t[j] >= T[i+M-1] && t[j] <= T[i+M]){	// �m�b�g�x�N�g���̒l�ƓK��������̂̂݉��Ƃ��Ē��o
				if(k == ans_size){
                    GuiIF.SetMessage("NURBS KOD_ERROR:Intersection points exceeded the allocated array length");
					goto EXIT;
				}
				ans[k] = t[j];		// �����擾
				k++;				// ���̐����C���N�������g
			}
		}
	}

	return k;

EXIT:
	FreeMatrix(coef,M);
	return KOD_ERR;
}

// Function: CalcEquation
// (private)CalcIntersecCurve3(), CalcIntersecPtsPlaneU/V3()�̃T�u�֐��D3���������܂ł�����
// 
// Parameters:
// *a - �W���s��
// *t - ��
// M - ����
//
// Return:
// ���̌��i�����Ȃ������ꍇ or ������3,2,1�̂����ꂩ�łȂ��FKOD_ERR�j
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
// (private)CalcIntersecCurve3(), CalcIntersecPtsPlaneU/V3()�̃T�u�֐��DNURBS�Ȑ��ƕ��ʂ̌�����o�p�������𓾂�
// 
// Parameters:
// M - �K�� 
// *P, *Q - NURBS�Ȑ��̌W���iP,Q)
// pt - ���ʏ�̈�_
// nvec - ���ʂ̖@���x�N�g�� 
// *a - ���� 
//void NURBS_Func::GetIntersecEquation(int M,Coord *P,double *Q,Coord pt,Coord nvec,double *a)
void GetIntersecEquation(int M,Coord *P,double *Q,Coord pt,Coord nvec,double *a)
{
	for(int i=0;i<M;i++){
		a[i] = (Q[i]*pt.x-P[i].x)*nvec.x + (Q[i]*pt.y-P[i].y)*nvec.y + (Q[i]*pt.z-P[i].z)*nvec.z;
	}
}

// Function: GetNurbsCCoef
// (private)CalcIntersecCurve3()�̃T�u�֐��DNURBS�Ȑ��̌W�������߂�(�ō�3��)
// 
// Parameters:
// *nurb - �ΏۂƂȂ�NURBS�Ȑ� 
// **coef - B�X�v���C�����֐��̌W�� 
// i - �Ȑ��̔ԍ� 
// *P, *Q - NURBS�Ȑ��̌W��(P,Q) 
//
// Return:
// �����FKOD_TRUE, ���s�FKOD_ERR
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
// 3����B�X�v���C���Ȑ��̊e�W�������߂�.
//
// coef[j][0]t^3 + coef[j][1]t^2 + coef[j][2]t + coef[j][3]   (Nj,4)
// 
// Parameters:
// M - �K��  
// K - �R���g���[���|�C���g�̐�  
// i - ���ڒ��̃R���g���[���|�C���g 
// *t - �m�b�g�x�N�g����  
// *coef - �Z�o�����W�����i�[
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
// 2����B�X�v���C���Ȑ��̊e�W�������߂�
//
// coef[j][0]t^2 + coef[j][1]t + coef[j][2]
//
// Parameters:
// M - �K��  
// K - �R���g���[���|�C���g�̐�  
// i - ���ڒ��̃R���g���[���|�C���g 
// *t - �m�b�g�x�N�g����  
// *coef - �Z�o�����W�����i�[
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
// 1����B�X�v���C���Ȑ��̊e�W�������߂�
//
// coef[j][0]t + coef[j][1]
//
// Parameters:
// M - �K��  
// K - �R���g���[���|�C���g�̐�  
// i - ���ڒ��̃R���g���[���|�C���g 
// *t - �m�b�g�x�N�g����  
// *coef - �Z�o�����W�����i�[
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
// NURBS�Ȗʂ̃V�t�g
//
// Parameters:
// *nurbs - �ύX�����NURBS�Ȗ�  
// shift - �V�t�g��
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
// NURBS�Ȑ��̃V�t�g
// 
// Parameters:
// *nurbs - �ύX�����NURBS�Ȑ�  
// shift - �V�t�g��
//void NURBS_Func::ShiftNurbsC(NURBSC *nurbs,Coord shift)
void NURBSC::ShiftNurbsC(Coord shift)
{
	for(int i=0;i<K;i++){
		cp[i] = AddCoord(cp[i],shift);
	}
}

// Function: RotNurbsS
// NURBS�Ȗʂ�D�x�N�g������deg(��)������]������
//
// Parameters:
// *nurbs - �ύX�����NURBS�Ȗʁ@
// axis - ��]���̒P�ʃx�N�g���@
// deg - �p�x(degree)
//void NURBS_Func::RotNurbsS(NURBSS *nurbs,Coord axis,double deg)
void NURBSS::RotNurbsS(Coord axis,double deg)
{
	double rad;			// ���W�A���i�[�p
	QUATERNION QFunc;	// �N�H�[�^�j�I���֘A�̊֐����W�߂��N���X�̃I�u�W�F�N�g�𐶐�
	Quat StartQ;		// ��]�O�̍��W���i�[����N�H�[�^�j�I��
	Quat RotQ;			// ��]�N�H�[�^�j�I��
	Quat ConjuQ;		// �����N�H�[�^�j�I��
	Quat TargetQ;		// ��]��̍��W���i�[����N�H�[�^�j�I��
	
	for(int i=0;i<K[0];i++){			// u�����̃R���g���[���|�C���g�����[�v
		for(int j=0;j<K[1];j++){		// v�����̃R���g���[���|�C���g�����[�v
			StartQ = QFunc.QInit(1,cp[i][j].x,cp[i][j].y,cp[i][j].z);		// NURBS�Ȗʂ��\������cp�̍��W��o�^
			rad = DegToRad(deg);										// degree����radian�ɕϊ�
			RotQ = QFunc.QGenRot(rad,axis.x,axis.y,axis.z);				// ��]�N�H�[�^�j�I���ɉ�]�ʂ�o�^(�����̐�����������ΔC�ӂɉ�]�ł���)
			ConjuQ = QFunc.QConjugation(RotQ);							// RotQ�̋����N�H�[�^�j�I����o�^
			TargetQ = QFunc.QMult(QFunc.QMult(RotQ,StartQ),ConjuQ);		// ��]������
			cp[i][j] = SetCoord(TargetQ.x,TargetQ.y,TargetQ.z);	// ��]��̍��W��o�^
		}
	}
}

// Function: RotNurbsC
// NURBS�Ȗʂ�D�x�N�g������deg(��)������]������
//
// Parameters:
// *nurbs - �ύX�����NURBS�Ȑ��@
// axis - ��]���̒P�ʃx�N�g���@
// deg - �p�x(degree)
//void NURBS_Func::RotNurbsC(NURBSC *nurbs,Coord axis,double deg)
void NURBSC::RotNurbsC(Coord axis,double deg)
{
	double rad;			// ���W�A���i�[�p
	QUATERNION QFunc;	// �N�H�[�^�j�I���֘A�̊֐����W�߂��N���X�̃I�u�W�F�N�g�𐶐�
	Quat StartQ;		// ��]�O�̍��W���i�[����N�H�[�^�j�I��
	Quat RotQ;			// ��]�N�H�[�^�j�I��
	Quat ConjuQ;		// �����N�H�[�^�j�I��
	Quat TargetQ;		// ��]��̍��W���i�[����N�H�[�^�j�I��
	
	for(int i=0;i<K;i++){		// �R���g���[���|�C���g�����[�v
		StartQ = QFunc.QInit(1,cp[i].x,cp[i].y,cp[i].z);		// NURBS�Ȗʂ��\������cp�̍��W��o�^
		rad = DegToRad(deg);									// degree����radian�ɕϊ�
		RotQ = QFunc.QGenRot(rad,axis.x,axis.y,axis.z);			// ��]�N�H�[�^�j�I���ɉ�]�ʂ�o�^(�����̐�����������ΔC�ӂɉ�]�ł���)
		ConjuQ = QFunc.QConjugation(RotQ);						// RotQ�̋����N�H�[�^�j�I����o�^
		TargetQ = QFunc.QMult(QFunc.QMult(RotQ,StartQ),ConjuQ);	// ��]������
		cp[i] = SetCoord(TargetQ.x,TargetQ.y,TargetQ.z);	// ��]��̍��W��o�^
	}
}

// Function: ChRatioNurbsS
// NURBS�Ȗʂ̔{����ύX����
//
// Parameters:
// *nurbs - �ύX�����NURBS�Ȗ�  
// ratio - �{��
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
// NURBS�Ȑ��̔{����ύX����
//
// Parameters:
// *nurbs - �ύX�����NURBS�Ȑ�  
// ratio - �{��
//void NURBS_Func::ChRatioNurbsC(NURBSC *nurbs,Coord ratio)
void NURBSC::ChRatioNurbsC(Coord ratio)
{
	for(int i=0;i<K;i++){
		cp[i] = MulCoord(cp[i],ratio);
	}
}

// Function: SetCPNurbsS
// NURBS�Ȗ�nurbs�̃R���g���[���|�C���g���CNURBS�Ȗ�Nurbs�̃R���g���[���|�C���g�ɒu��������
//
// Parameters:
// *nurbs - �u�������NURBS�Ȗ�  
// Nurbs - �������NURBS�Ȗ�
//
// Return:
// ����I���FKOD_TRUE, ���Ȗʂ̃R���g���[���|�C���g������v���Ă��Ȃ��FKOD_ERR
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
// �^����ꂽ�_����Ԃ���n�K��NURBS�Ȑ��𐶐�����.
// �[��������^���Ȃ��o�[�W����
//
// Parameters:
// *Nurbs - ���������NURBS�Ȑ��̃A�h���X   
// *P - �_��   
// PNum - �_��̐�   
// M - �K��
//
// Return:
// ����I���FKOD_TRUE, �^����ꂽ�_��1�����FKOD_ERR, �v�Z�ߒ��Ń[�����������FKOD_ERR
int NURBS_Func::GenInterpolatedNurbsC1(NURBSC *Nurbs,Coord *P,int PNum,int M)
{
	int retflag = KOD_TRUE;

	if(PNum <= 1){			// �^����ꂽ�_��1�����̏ꍇ�́ANURBS�Ȑ��𐶐��ł��Ȃ�
        GuiIF.SetMessage("NURBS KOD_ERROR:Few Point. You should set over 2 points at least");
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
	GetCurveKnotParam2(P,PNum,T_);
	for(int i=0;i<PNum;i++)
		P[i].dmy = T_[i];

	// �m�b�g�x�N�g���𓾂�
	GetInterpolatedKnot(T_,N,K,M,T);

	// B�X�v���C�����֐��s��𐶐�
	for(int i=0;i<K;i++){
		for(int j=0;j<K;j++){
			B[i][j] = CalcBSbasis(T_[i],T,N,j,M);
		}
	}

	// B�X�v���C�����֐��s��̋t�s������߂�
	double det = Gauss(K,B,P,Q);
	if(det == 0){
        GuiIF.SetMessage("NURBS ERROR:Determinant is 0");
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

// Function: GenInterpolatedNurbsC2
// �^����ꂽ�_����Ԃ���n�K��NURBS�Ȑ��𐶐�����D
// �[������:�n�_��C2�ň�v
//
// Parameters:
// *Nurbs - ���������NURBS�Ȑ��̃A�h���X   
// *P_ - �ʉߓ_��iP_[0]��P_[PNum-1]�͈�v���Ă��邱�Ɓj
// PNum - �ʉߓ_��̐�   
// M - �K��
//
// Return:
// KOD_TRUE:����I��, KOD_FALSE:�_��̎n�_�ƏI�_����v���Ă��Ȃ�, KOD_ERR:�_��̐���1����
int NURBS_Func::GenInterpolatedNurbsC2(NURBSC *Nurbs,Coord *P_,int PNum,int M)
{
	if(DiffCoord(P_[0],P_[PNum-1]) == KOD_FALSE){
        GuiIF.SetMessage("NURBS KOD_ERROR:Given points P0 and Pn are not unmuched");
		return KOD_FALSE;
	}
	if(PNum <= 1){			// �^����ꂽ�_��1�����̏ꍇ�́ANURBS�Ȑ��𐶐��ł��Ȃ�
        GuiIF.SetMessage("NURBS KOD_ERROR:Few Point. You should set over 2 points at least");
		return KOD_ERR;
	}
	if(PNum == 2 || PNum == 3)	M = PNum;	// �^����ꂽ�_��2��3�̏ꍇ�́A�K���������I��2��3�ɂ���

	int K = PNum+2;				// �R���g���[���|�C���g�̐�
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
	GetCurveKnotParam1(P_,PNum,T_);

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
	
	return KOD_TRUE;
}

// Function: GenApproximationNurbsC
// �^����ꂽ�_����ߎ�����n�K��NURBS�Ȑ��𐶐�����
//
// Parameters:
// *Nurbs - ���������NURBS�Ȑ��̃A�h���X   
// *P - �_��   
// PNum - �_��̐�   
// M - �K��
//
// Return:
// ����I���FKOD_TRUE, �^����ꂽ�_��1�����FKOD_ERR
int NURBS_Func::GenApproximationNurbsC(NURBSC *Nurbs,Coord *P,int PNum,int M)
{
	if(PNum <= 1){			// �^����ꂽ�_��1�����̏ꍇ�́ANURBS�Ȑ��𐶐��ł��Ȃ�
        GuiIF.SetMessage("NURBS KOD_ERROR:Few Point. You should set over 2 points at least");
		return KOD_ERR;
	}

	int K = SetApproximationCPnum(PNum);		// �^����ꂽ�_�񂩂�R���g���[���|�C���g�̐������߂�(�R���g���[���|�C���g�̐��ŋߎ������Ȑ����ς��)
	int Nnum = M+K;					// �m�b�g�x�N�g���̐�
	int prop[4] = {0,0,1,0};		// �p�����[�^
	double V[2] = {0,1};			// �m�b�g�x�N�g���̊J�n�l,�I���l

	Vector T_ = NewVector(PNum);	// �ʉߓ_��̋Ȑ��p�����[�^
	Vector T = NewVector(Nnum);		// �m�b�g�x�N�g��
	Coord *Q = NewCoord1(K);		// �R���g���[���|�C���g
	Vector W = NewVector(K);		// �d��

	GetCurveKnotParam1(P,PNum,T_);		// �ʉߓ_��̋Ȑ��p�����[�^�𓾂�

	GetApproximatedKnot(T_,PNum,M,K,T);	// �m�b�g�x�N�g����ݒ肷��

	CalcApproximationCP_LSM(P,T_,T,PNum,Nnum,M,K,Q);	// �ŏ�2��@�ŋߎ��R���g���[���|�C���g�����߂�

	for(int i=0;i<K;i++){	// �d�݂�1�ŌŒ�
		W[i] = 1;
	}

	GenNurbsC(Nurbs,K,M,Nnum,T,W,Q,V,prop,0);	// NURBS�Ȑ�����

	FreeCoord1(Q);
	FreeVector(T);
	FreeVector(T_);
	FreeVector(W);

	return KOD_TRUE;
}

// Function: GenNurbsCfromCP
// �R���g���[���|�C���g����NURBS�Ȑ��𐶐�����
//
// �m�b�g�x�N�g���͓��Ԋu�ɐݒ肳���
//
// �d�݂͑S��1�Ƃ���
//
// Parameters:
// *Nurbs - ���������NURBS�Ȑ��̃A�h���X   
// *P - �_��   
// PNum - �_��̐�   
// M - �K��
// ����I���FKOD_TRUE, �^����ꂽ�_��1�����FKOD_ERR
int NURBS_Func::GenNurbsCfromCP(NURBSC *Nurbs,Coord *P,int PNum,int M)
{
	if(PNum <= 1){			// �^����ꂽ�_��1�����̏ꍇ�́ANURBS�Ȑ��𐶐��ł��Ȃ�
        GuiIF.SetMessage("NURBS KOD_ERROR:Few Point. You should set over 2 points at least");
		return KOD_ERR;
	}

	int Nnum = M+PNum;				// �m�b�g�x�N�g���̐�
	int prop[4] = {0,0,1,0};		// �p�����[�^
	double V[2] = {0,1};			// �m�b�g�x�N�g���̊J�n�l,�I���l
	Vector T = NewVector(Nnum);		// �m�b�g�x�N�g��
	Vector W = NewVector(PNum);		// �d��

	GetEqIntervalKont(PNum,M,T);	// �m�b�g�x�N�g���𓾂�

	for(int i=0;i<PNum;i++){	// �d�݂�1�ŌŒ�
		W[i] = 1;
	}

	GenNurbsC(Nurbs,PNum,M,Nnum,T,W,P,V,prop,0);	// NURBS�Ȑ�����

	FreeVector(T);
	FreeVector(W);

	return KOD_TRUE;
}

// Function: GenPolygonalLine
// �܂��(NURBS�Ȑ�)�𐶐�����
// 
// Parameters:
// *Nurbs - ���������NURBS�Ȑ��̃A�h���X   
// *P - �R���g���[���|�C���g   
// PNum - �R���g���[���|�C���g�̐�
//
// Return:
// ����I���FKOD_TRUE, �^����ꂽ�_��1�����FKOD_ERR
int NURBS_Func::GenPolygonalLine(NURBSC *Nurbs,Coord *P,int PNum)
{
	if(PNum <= 1){			// �^����ꂽ�_��1�����̏ꍇ�́ANURBS�Ȑ��𐶐��ł��Ȃ�
        GuiIF.SetMessage("NURBS KOD_ERROR:Few Point. You should set over 2 points at least");
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

// Function: GenInterpolatedNurbsS1
// �^����ꂽ�_����Ԃ���n�KNURBS�Ȗʂ𐶐�����D
// �[��������^���Ȃ��o�[�W����
//
// Parameters:
// *Nurbs - ���������NURBS�Ȗʂ̃A�h���X   
// **P - �^����ꂽ�_��   
// PNum_u,PNum_v - �_�̐��@ 
// Mu,Mv - �K��
//
// Return:
// ����I���FKOD_TRUE, �^����ꂽ�_��1�����FKOD_ERR
int NURBS_Func::GenInterpolatedNurbsS1(NURBSS *Nurbs,Coord **P,int PNum_u,int PNum_v,int Mu,int Mv)
{
	if(PNum_u <= 1 || PNum_v <= 1){			// �^����ꂽ�_���e������1�����̏ꍇ�́ANURBS�Ȗʂ𐶐��ł��Ȃ�
        GuiIF.SetMessage("NURBS ERROR:Few Point. You should set over 2 points at least");
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


	GetSurfaceKnotParam(S_,T_,P,PNum_u,PNum_v);		// ��ԋȖʗpu,v�p�����[�^�𓾂�

	GetInterpolatedKnot(S_,N[0],K[0],Mu,S);			// �m�b�g�x�N�g��S�𓾂�

	GetInterpolatedKnot(T_,N[1],K[1],Mv,T);			// �m�b�g�x�N�g��T�𓾂�

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

	return KOD_TRUE;
}

// Function: GenApproximationNurbsS
// �^����ꂽ�_����ߎ�����n�K��NURBS�Ȗʂ𐶐�����
//
// Parameters:
// *Nurbs - ���������NURBS�Ȗʂ̃A�h���X   
// **P - �^����ꂽ�_��   
// PNum_u,PNum_v - �_�̐��@ 
// Mu,Mv - �K��
//
// Return:
// ����I���FKOD_TRUE, �^����ꂽ�_��1�����FKOD_ERR
int NURBS_Func::GenApproximationNurbsS(NURBSS *Nurbs,Coord **P,int PNum_u,int PNum_v,int Mu,int Mv)
{
	if(PNum_u <= 1 || PNum_v <= 1){			// �^����ꂽ�_���e������1�����̏ꍇ�́ANURBS�Ȗʂ𐶐��ł��Ȃ�
        GuiIF.SetMessage("NURBS ERROR:Few Point. You should set over 2 points at least");
		return KOD_ERR;
	}
	if(PNum_u == 2 || PNum_u == 3)	Mu = PNum_u;	// u�����ɗ^����ꂽ�_��2��3�̏ꍇ�́Au�����̊K���������I��2��3�ɂ���
	if(PNum_v == 2 || PNum_v == 3)	Mv = PNum_v;	// v�����ɗ^����ꂽ�_��2��3�̏ꍇ�́Av�����̊K���������I��2��3�ɂ���

	// �^����ꂽ�_�񂩂�R���g���[���|�C���g�̐������߂�
	int K[2];
	K[0] = SetApproximationCPnum(PNum_u);
	K[1] = SetApproximationCPnum(PNum_v);

	int N[2] = {Mu+K[0],Mv+K[1]};	// �m�b�g�x�N�g���̐�
	int prop[5] = {0,0,1,0,0};		// �p�����[�^
	double U[2] = {0,1};			// u�����m�b�g�x�N�g���̊J�n�l�A�I���l
	double V[2] = {0,1};			// v�����m�b�g�x�N�g���̊J�n�l�A�I���l

	Vector S_ = NewVector(PNum_u);		// u�����̒ʉߓ_��̋Ȑ��p�����[�^
	Vector S = NewVector(N[0]);			// u�����̃m�b�g�x�N�g��
	Vector T_ = NewVector(PNum_v);		// v�����̒ʉߓ_��̋Ȑ��p�����[�^
	Vector T = NewVector(N[1]);			// v�����̃m�b�g�x�N�g��
	Coord **Q1 = NewCoord2(PNum_u,K[1]);	// NURBS�Ȗʂ̃R���g���[���|�C���g
	Coord **Q2 = NewCoord2(K[1],PNum_u);	
	Coord **Q3 = NewCoord2(K[1],K[0]);
	Coord **Q4 = NewCoord2(K[0],K[1]);
	Coord **P_ = NewCoord2(K[1],K[0]);
	Matrix W = NewMatrix(K[0],K[1]);	// �d��

	GetSurfaceKnotParam(S_,T_,P,PNum_u,PNum_v);		// ��ԋȖʗpu,v�p�����[�^�𓾂�

	GetApproximatedKnot(S_,PNum_u,Mu,K[0],S);		// �m�b�g�x�N�g��S��ݒ肷��
	GetApproximatedKnot(T_,PNum_v,Mv,K[1],T);		// �m�b�g�x�N�g��T��ݒ肷��

	// v�����̓_�񂩂�ߎ�NURBS�Ȑ���PNum_u�쐬����
	for(int i=0;i<PNum_u;i++){
		CalcApproximationCP_LSM(P[i],T_,T,PNum_v,N[1],Mv,K[1],Q1[i]);	// �ŏ�2��@�ŋߎ��R���g���[���|�C���g�����߂�
	}
	TranMx(Q1,PNum_u,K[1],Q2);					// Q�̓]�u

	for(int i=0;i<K[1];i++){
		CalcApproximationCP_LSM(Q2[i],S_,S,PNum_u,N[0],Mu,K[0],Q3[i]);	// �ŏ�2��@�ŋߎ��R���g���[���|�C���g�����߂�
	}
	TranMx(Q3,K[1],K[0],Q4);					// Q�̓]�u

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
// �^����ꂽ�R���g���[���|�C���g����n�K��NURBS�Ȗʂ𐶐�����
//
// �m�b�g�x�N�g���͓��Ԋu�ɐݒ肳���
//
// �d�݂͑S��1�Ƃ���
//
// Parameters:
// *Nurbs - ���������NURBS�Ȗʂ̃A�h���X   
// **P - �^����ꂽ�R���g���[���|�C���g��   
// PNum_u,PNum_v - �_�̐��@ 
// Mu,Mv - �K��
//
// Return:
// ����I���FKOD_TRUE, �^����ꂽ�_��1�����FKOD_ERR
int NURBS_Func::GenNurbsSfromCP(NURBSS *Nurbs,Coord **P,int PNum_u,int PNum_v,int Mu,int Mv)
{
	if(PNum_u <= 1 || PNum_v <= 1){			// �^����ꂽ�_���e������1�����̏ꍇ�́ANURBS�Ȗʂ𐶐��ł��Ȃ�
        GuiIF.SetMessage("NURBS ERROR:Few Point. You should set over 2 points at least");
		return KOD_ERR;
	}
	if(PNum_u == 2 || PNum_u == 3)	Mu = PNum_u;	// u�����ɗ^����ꂽ�_��2��3�̏ꍇ�́Au�����̊K���������I��2��3�ɂ���
	if(PNum_v == 2 || PNum_v == 3)	Mv = PNum_v;	// v�����ɗ^����ꂽ�_��2��3�̏ꍇ�́Av�����̊K���������I��2��3�ɂ���

	int K[2] = {PNum_u,PNum_v};			// �R���g���[���|�C���g�̐�		
	int N[2] = {Mu+K[0],Mv+K[1]};		// �m�b�g�x�N�g���̐�
	int prop[5] = {0,0,1,0,0};			// �p�����[�^
	double U[2] = {0,1};				// u�����m�b�g�x�N�g���̊J�n�l�A�I���l
	double V[2] = {0,1};				// v�����m�b�g�x�N�g���̊J�n�l�A�I���l
	Vector S = NewVector(N[0]);			// u�����̃m�b�g�x�N�g��
	Vector T = NewVector(N[1]);			// v�����̃m�b�g�x�N�g��
	Matrix W = NewMatrix(K[0],K[1]);	// �d��

	GetEqIntervalKont(K[0],Mu,S);		// u�����m�b�g�x�N�g���𓾂�
	GetEqIntervalKont(K[1],Mv,T);		// v�����m�b�g�x�N�g���𓾂�

	// �d�݂𓾂�
	for(int i=0;i<K[0];i++){
		for(int j=0;j<K[1];j++){
			W[i][j] = 1;
		}
	}

	GenNurbsS(Nurbs,Mu,Mv,K[0],K[1],S,T,W,P,U[0],U[1],V[0],V[1]);		// NURBS�Ȗʂ𐶐�����

	FreeVector(S);
	FreeVector(T);
	FreeMatrix(W,K[0]);

	return KOD_TRUE;
}

// Function: 
// �܂��(NURBS�Ȗ�)�𐶐�����GenPolygonalSurface
//
// Parameters:
// *Nurbs - ���������NURBS�Ȗʂ̃A�h���X   
// **P - �R���g���[���|�C���g   
// PNum_u,PNum_v - �R���g���[���|�C���g�̐�
//
// Return:
// KOD_TRUE
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
	FreeMatrix(W,K[0]);
	FreeVector(du_sum);
	FreeVector(dv_sum);

	return KOD_TRUE;
}

// Function: ConnectNurbsSU
// 2����NURBS�Ȗʂ�A������(U�����ɒ����Ȃ�)(S1_U1��S2_U0��A��)
//
// Parameters:
// *S1 - ��1
// *S2 - ��2
// *S_ - �A����̖ʂ��i�[
//
// Return:
// �����FKOD_TRUE,  ���s�FKOD_FALSE
int NURBS_Func::ConnectNurbsSU(NURBSS *S1,NURBSS *S2,NURBSS *S_)			
{
	// �A�������G�b�W��V�����R���g���[���|�C���g�̐����S�ē���������
	if(S1->K[1] != S2->K[1]){
		fprintf(stderr,"ERROR: Number of control point on V direction is not equal.");
		return KOD_ERR;
	}
	// �A�������G�b�W��V�����R���g���[���|�C���g���S�ē���������
	for(int i=0;i<S1->K[1];i++){
		if(DiffCoord(S1->cp[S1->K[0]-1][i],S2->cp[0][i]) == KOD_FALSE){
			fprintf(stderr,"ERROR: Knot value on V direction is not equal.");
			return KOD_ERR;
		}
	}
	// ���Ȗʂ̊K����U,V���ɓ���������
	if(S1->M[0] != S2->M[0] || S1->M[1] != S2->M[1]){
		fprintf(stderr,"ERROR: Rank is not equal.");
		return KOD_ERR;
	}

	int K[2],N[2];

	K[0] = S1->K[0] + S2->K[0] - 1;				// S_��U�����R���g���[���|�C���g�̐�
	K[1] = S1->K[1];							// S_��V�����R���g���[���|�C���g�̐�
	N[0] = S1->N[0] + S2->N[0] - S2->M[0] - 1;	// S_��U�����m�b�g�x�N�g���̐�
	N[1] = S1->N[1];							// S_��V�����m�b�g�x�N�g���̐�

	New_NurbsS(S_,K,N);							// S_���̃������[�m��

	SetKnotVecSU_ConnectS(S1,S2,S_);			// S_��u�����m�b�g��`����w��

	SetCPSU_ConnectS(S1,S2,S_);					// S_��u�����R���g���[���|�C���g�ƃE�F�C�g���w��

	S_->M[0] = S1->M[0];						// S_�̊K�����w��
	S_->M[1] = S1->M[1];

	return KOD_TRUE;
}

// Function: ConnectNurbsSV
// 2����NURBS�Ȗʂ�A������(V�����ɒ����Ȃ�)(S1_V1��S2_V0��A��)
//
// Parameters:
// *S1 - ��1
// *S2 - ��2
// *S_ - �A����̖ʂ��i�[
//
// Return:
// �����FKOD_TRUE,  ���s�FKOD_FALSE
int NURBS_Func::ConnectNurbsSV(NURBSS *S1,NURBSS *S2,NURBSS *S_)			
{
	// �A�������G�b�W��U�����R���g���[���|�C���g�̐����S�ē���������
	if(S1->K[0] != S2->K[0]){
		fprintf(stderr,"ERROR: Number of control point on U direction is not equal.");
		return KOD_ERR;
	}
	// �A�������G�b�W��U�����R���g���[���|�C���g���S�ē���������
	for(int i=0;i<S1->K[0];i++){
		if(DiffCoord(S1->cp[i][S1->K[0]-1],S2->cp[i][0]) == KOD_FALSE){
			fprintf(stderr,"ERROR: Knot value on U direction is not equal.");
			return KOD_ERR;
		}
	}
	// ���Ȗʂ̊K����U,V���ɓ���������
	if(S1->M[0] != S2->M[0] || S1->M[1] != S2->M[1]){
		fprintf(stderr,"ERROR: Rank is not equal.");
		return KOD_ERR;
	}

	int K[2],N[2];

	K[0] = S1->K[0];							// S_��U�����R���g���[���|�C���g�̐�
	K[1] = S1->K[1] + S2->K[1] - 1;				// S_��V�����R���g���[���|�C���g�̐�
	N[0] = S1->N[0];							// S_��U�����m�b�g�x�N�g���̐�
	N[1] = S1->N[1] + S2->N[1] - S2->M[1] - 1;	// S_��V�����m�b�g�x�N�g���̐�

	New_NurbsS(S_,K,N);							// S_���̃������[�m��

	SetKnotVecSV_ConnectS(S1,S2,S_);			// S_��v�����m�b�g��`����w��

	SetCPSV_ConnectS(S1,S2,S_);					// S_��v�����R���g���[���|�C���g�ƃE�F�C�g���w��

	S_->M[0] = S1->M[0];						// S_�̊K�����w��
	S_->M[1] = S1->M[1];

	return KOD_TRUE;
}

// Function: SetCPSU_ConnectS
// (private)ConnectNurbsSU()�̃T�u�֐��DS_��u�����R���g���[���|�C���g�ƃE�F�C�g���w��
//
// Parameters:
// *S1 - ��1
// *S2 - ��2
// *S_ - �A����̖ʂ��i�[
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
// (private)ConnectNurbsSU()�̃T�u�֐��DS_��u�����m�b�g��`����w��
//
// Parameters:
// *S1 - ��1
// *S2 - ��2
// *S_ - �A����̖ʂ��i�[
void NURBS_Func::SetKnotVecSU_ConnectS(NURBSS *S1,NURBSS *S2,NURBSS *S_)
{
	// V����
	S_->N[1] = S1->N[1];				// S_��V�����m�b�g�x�N�g���̐�
	CopyVector(S1->T,S1->N[1],S_->T);	// S_��V�����m�b�g�x�N�g��(V������S1�̂����̂܂ܗ��p)
	S_->V[0] = S1->V[0];				// S_��V�����m�b�g�x�N�g���͈̔�
	S_->V[1] = S1->V[1];

	// U����
	// �R�[�h���𒲂ׂ�
	double us=0,ue=NORM_KNOT_VAL,uc=0;		// U�����J�n�C�I���C�A�����m�b�g�x�N�g��
	double l1=0,l2=0;						// �e�Ȗʂ�U�����m�b�g�x�N�g���̃R�[�h��
	for(int i=0;i<S1->N[0]-1;i++) {
//		l1 += CalcDistance(CalcNurbsSCoord(S1,S1->S[i+1],S1->T[0]),CalcNurbsSCoord(S1,S1->S[i],S1->T[0]));	// S1�̃R�[�h��
		l1 += CalcDistance(S1->CalcNurbsSCoord(S1->S[i+1],S1->T[0]),S1->CalcNurbsSCoord(S1->S[i],S1->T[0]));	// S1�̃R�[�h��
	}
	for(int i=0;i<S2->N[0]-1;i++) {
//		l2 += CalcDistance(CalcNurbsSCoord(S2,S2->S[i+1],S2->T[0]),CalcNurbsSCoord(S2,S2->S[i],S2->T[0]));	// S2�̃R�[�h��
		l2 += CalcDistance(S2->CalcNurbsSCoord(S2->S[i+1],S2->T[0]),S2->CalcNurbsSCoord(S2->S[i],S2->T[0]));	// S2�̃R�[�h��
	}
	uc = l1/(l1+l2);	// �����_�̃m�b�g�x�N�g���l

	// S_�̃m�b�g�x�N�g���͈͂𓾂�
	Vector U1 = NewVector(S1->N[0]);	
	Vector U2 = NewVector(S2->N[0]);	
	CopyVector(S1->S,S1->N[0],U1);		// S1�̃m�b�g�x�N�g����U1�ɃR�s�[
	CopyVector(S2->S,S2->N[0],U2);		// S2�̃m�b�g�x�N�g����U2�ɃR�s�[
	ChangeKnotVecRange(U1,S1->N[0],S1->M[0],S1->K[0],us,uc);	// S1(U1)�̃m�b�g�x�N�g���͈̔͂�ύX
	ChangeKnotVecRange(U2,S2->N[0],S2->M[0],S2->K[0],uc,ue);	// S2(U2)�̃m�b�g�x�N�g���͈̔͂�ύX
	S_->U[0] = us;						// S_��U�����m�b�g�x�N�g���͈̔�
	S_->U[1] = ue;
	S_->N[0] = S1->N[0] + S2->N[0] - S2->M[0] - 1;	// S_�̃m�b�g�x�N�g����

	// S_�̃m�b�g�x�N�g���𓾂�
	for(int i=0;i<S1->K[0];i++)
		S_->S[i] = U1[i];
	for(int i=1;i<S2->N[0];i++)
		S_->S[S1->K[0]+i-1] = U2[i];

	FreeVector(U1);
	FreeVector(U2);
}

// Function: SetCPSV_ConnectS
// (private)ConnectNurbsSV()�̃T�u�֐��DS_��v�����R���g���[���|�C���g�ƃE�F�C�g���w��
//
// Parameters:
// *S1 - ��1
// *S2 - ��2
// *S_ - �A����̖ʂ��i�[
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
// (private)ConnectNurbsSV()�̃T�u�֐��DS_��v�����m�b�g��`����w��
//
// Parameters:
// *S1 - ��1
// *S2 - ��2
// *S_ - �A����̖ʂ��i�[
void NURBS_Func::SetKnotVecSV_ConnectS(NURBSS *S1,NURBSS *S2,NURBSS *S_)
{
	// U����
	S_->N[0] = S1->N[0];				// S_��U�����m�b�g�x�N�g���̐�
	CopyVector(S1->S,S1->N[0],S_->S);	// S_��U�����m�b�g�x�N�g��(U������S1�̂����̂܂ܗ��p)
	S_->U[0] = S1->U[0];				// S_��U�����m�b�g�x�N�g���͈̔�
	S_->U[1] = S1->U[1];

	// V����
	// �R�[�h���𒲂ׂ�
	double vs=0,ve=NORM_KNOT_VAL,vc=0;		// U�����J�n�C�I���C�A�����m�b�g�x�N�g��
	double l1=0,l2=0;						// �e�Ȗʂ�U�����m�b�g�x�N�g���̃R�[�h��
	for(int i=0;i<S1->N[1]-1;i++) {
//		l1 += CalcDistance(CalcNurbsSCoord(S1,S1->S[0],S1->T[i+1]),CalcNurbsSCoord(S1,S1->S[0],S1->T[i]));	// S1�̃R�[�h��
		l1 += CalcDistance(S1->CalcNurbsSCoord(S1->S[0],S1->T[i+1]),S1->CalcNurbsSCoord(S1->S[0],S1->T[i]));	// S1�̃R�[�h��
	}
	for(int i=0;i<S2->N[1]-1;i++) {
//		l2 += CalcDistance(CalcNurbsSCoord(S2,S2->S[0],S2->T[i+1]),CalcNurbsSCoord(S2,S2->S[0],S2->T[i]));	// S2�̃R�[�h��
		l2 += CalcDistance(S2->CalcNurbsSCoord(S2->S[0],S2->T[i+1]),S2->CalcNurbsSCoord(S2->S[0],S2->T[i]));	// S2�̃R�[�h��
	}
	vc = l1/(l1+l2);	// �����_�̃m�b�g�x�N�g���l

	// S_�̃m�b�g�x�N�g���͈͂𓾂�
	Vector V1 = NewVector(S1->N[1]);	
	Vector V2 = NewVector(S2->N[1]);	
	CopyVector(S1->T,S1->N[1],V1);		// S1�̃m�b�g�x�N�g����V1�ɃR�s�[
	CopyVector(S2->T,S2->N[1],V2);		// S2�̃m�b�g�x�N�g����V2�ɃR�s�[
	ChangeKnotVecRange(V1,S1->N[1],S1->M[1],S1->K[1],vs,vc);	// S1(V1)�̃m�b�g�x�N�g���͈̔͂�ύX
	ChangeKnotVecRange(V2,S2->N[1],S2->M[1],S2->K[1],vc,ve);	// S2(V2)�̃m�b�g�x�N�g���͈̔͂�ύX
	S_->V[0] = vs;						// S_��V�����m�b�g�x�N�g���͈̔�
	S_->V[1] = ve;
	S_->N[1] = S1->N[1] + S2->N[1] - S2->M[1] - 1;	// S_�̃m�b�g�x�N�g����

	// S_�̃m�b�g�x�N�g���𓾂�
	for(int i=0;i<S1->K[1];i++)
		S_->T[i] = V1[i];
	for(int i=1;i<S2->N[1];i++)
		S_->T[S1->K[1]+i-1] = V2[i];

	FreeVector(V1);
	FreeVector(V2);
}

// Function: CalcuIntersecPtNurbsLine
// NURBS�Ȗʂƒ����̌�_���Z�o
//
// Parameters:
// *Nurb - NURBS�Ȗ�S(u,v)�ւ̃|�C���^
// r - ����N(t)���1�_
// p - ����N(t)�̕���
// DivNum - NURBS�Ȗʂ̕�����
// *ans - ��_��u,v,t�p�����[�^�i�[�p�z��
// anssize - ans�̔z��
//
// Divnum���傫���قǁA��_�Z�o�̎�肱�ڂ��͏��Ȃ��Ȃ�D
//
// anssize��DivNum*DivNum�ȏ�ɂ���̂��D�܂���.
//
// LoD - �ڍדx�i�j���[�g���@�̍X�V�p�����[�^�𑫂����ނƂ��ɁCLoD�Ŋ��邱�ƂŁC�}���ȃp�����[�^�ύX�������D�ʏ��1�ł悢���C���������Ȃ��ꍇ�͒l��傫������D2�Ƃ�3�Ƃ��j
//
// Return:
// ��_�̐�,   KOD_ERR:��_�̐����w�肵���z�񒷂𒴂���
//int NURBS_Func::CalcuIntersecPtNurbsLine(NURBSS *Nurb,Coord r,Coord p,int Divnum,Coord *ans,int anssize,int LoD)
int NURBSS::CalcuIntersecPtNurbsLine(Coord r,Coord p,int Divnum,Coord *ans,int anssize,int LoD)
{
	Coord d = SetCoord(100,100,100);		// NURBS�Ȑ�S(u,v)�̔����ω���(du,dv)�A����N(t)�̔����ω���dt���i�[
	Coord F,Fu,Fv,Ft;						// F(u,v,t) = S(u,v) - N(t)    Fu = dF/du     Fv = dF/dv     Ft = dF/dt
	double u = U[0];					// NURBS�Ȗ�S(u,v)��u�p�����[�^�̌��ݒl
	double v = V[0];					// NURBS�Ȗ�S(u,v)��v�p�����[�^�̌��ݒl
	double t = 0;							// ����N(t)��t�p�����[�^
	Matrix A = NewMatrix(3,3);				// Fu,Fv,Ft���\������3x3�s��
	Matrix A_ = NewMatrix(3,3);				// A�̋t�s����i�[
	int flag = KOD_FALSE;						// �����t���O
	double dv = (V[1] - V[0])/(double)Divnum;	// �������Z�p��v�p�����[�^�̃C���^�[�o���l
	double du = (U[1] - U[0])/(double)Divnum;	// �������Z�p��u�p�����[�^�̃C���^�[�o���l
	int loopcount = 0;						// �����v�Z��
	int anscount = 0;						// �Z�o���ꂽ��_�̐�

	// u loop
	for(int i=0;i<Divnum;i++){
		// v loop
		for(int j=0;j<Divnum;j++){
			u = U[0] + (double)i*du;			// �X�e�b�v�p�����[�^u�̏����l���Z�b�g
			v = V[0] + (double)j*dv;		// �X�e�b�v�p�����[�^v�̏����l���Z�b�g
			t = 0;								// �X�e�b�v�p�����[�^t�̏����l���Z�b�g
			flag = KOD_FALSE;						// �����t���O��OFF
			loopcount = 0;						// ���[�v�J�E���g������
			// �����̔����ω���dt(=d.z)��APPROX_ZERO�������܂Ńj���[�g���@�ɂ������v�Z���s��
			while(loopcount < LOOPCOUNTMAX){
//				F = SubCoord(CalcNurbsSCoord(Nurb,u,v),AddCoord(r,MulCoord(p,t)));	// F(u,v,t) = S(u,v) - N(t) = S(u,v) - (r+tp)
//				Fu = CalcDiffuNurbsS(Nurb,u,v);			// Fu = dF/du = dS/du
//				Fv = CalcDiffvNurbsS(Nurb,u,v);			// Fv = dF/dv = dS/dv
				F = SubCoord(CalcNurbsSCoord(u,v),AddCoord(r,MulCoord(p,t)));	// F(u,v,t) = S(u,v) - N(t) = S(u,v) - (r+tp)
				Fu = CalcDiffuNurbsS(u,v);			// Fu = dF/du = dS/du
				Fv = CalcDiffvNurbsS(u,v);			// Fv = dF/dv = dS/dv
				Ft = MulCoord(p,-1);					// Ft = dF/dt = -dN/dt = -p
				A[0][0] = Fu.x;				// Fu,Fv,Ft��3x3�s��A�ɑ��
				A[0][1] = Fv.x;				//     |Fu.x Fv.x Ft.x|       |du|       |F.x|
				A[0][2] = Ft.x;				// A = |Fu.y Fv.y Ft.y| , d = |dv| , F = |F.y|
				A[1][0] = Fu.y;				//     |Fu.z Fv.z Ft.z|       |dt|       |F.z|
				A[1][1] = Fv.y;
				A[1][2] = Ft.y;				// A�Ed = F   --->   d = A_�EF
				A[2][0] = Fu.z;
				A[2][1] = Fv.z;
				A[2][2] = Ft.z;	
				//fprintf(stderr,"   %lf,%lf,%lf,%lf,%lf\n",u,v,Fu.x,Fu.y,Fu.z);
				if(MatInv3(A,A_) == KOD_FALSE){		// �t�s������߂�
					flag = KOD_ERR;
					break;		
				}
				d = MulCoord(MulMxCoord(A_,F),-1);			// d���Z�o
				
				if(fabs(d.x) <= APPROX_ZERO && fabs(d.y) <= APPROX_ZERO && fabs(d.z) <= APPROX_ZERO){	// �^�l�Ɏ���������loop�𔲂���
					flag = KOD_TRUE;		// �����t���Otrue
					break;
				}

				// �^�l�ɒB���Ă��Ȃ�������u,v,t���X�V
				u += d.x/(double)LoD;
				v += d.y/(double)LoD;
				t += d.z/(double)LoD;

				//if(u < Nurb->U[0] || u > Nurb->U[1] || v < Nurb->V[0] || v > Nurb->V[1]){	// u,v�̂ǂ��炩�����U������loop�𔲂���
				//	flag = KOD_FALSE;		// �����t���Ofalse
				//	break;
				//}

				loopcount++;
			}// end of while

			// LOOPCOUNTMAX�񃋁[�v���Ă��������Ă��Ȃ�������x��
			if(loopcount == LOOPCOUNTMAX)
                GuiIF.SetMessage("NURBS_Func ERROR: fail to converge");

			// �������Ă�������Ƃ��ēo�^
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

	anscount = CheckTheSamePoints(ans,anscount);		// ����_�͏�������

	return anscount;
}

// Function: CalcIntersecPtNurbsPt
// ��ԏ��1�_P����NURBS�Ȗ�S��̍ŋߖT�_Q�����߂�(�j���[�g���@)
//
// >�����̕�����L(t) = P + tN
// >N��S��̖@���x�N�g���ƈ�v���邩��N=Su�~Sv
// >�������FS(u,v) = P + tN(u,v)
// >F(u,v,t) = S(u,v) - P - tN(u,v)   �Ƃ��āA�j���[�g���@��p����
// >Fu = Su - tNu	Fv = Sv - tNv	Ft = -N
// >|Fu.x Fv.x Ft.x||du|    |F.x|
// >|Fu.y Fv.y Ft.y||dv| = -|F.y|    =>     dF�Ed = -F     =>     d = -F�EdF^-1  
// >|Fu.z Fv.z Ft.z||dt|    |F.z|
// 
// Parameters:
// *S - NURBS�Ȗ�
// P - ��ԏ��1�_
// Divnum - �j���[�g���@�����l�w��p�̋Ȗʕ�����
// LoD - �j���[�g���p�����[�^�X�V���̃X�e�b�v�T�C�Y(1�`)
// Q - ���iS��̓_��u,v,t�p�����[�^��Coord�\���̂Ɋi�[�j
//
// Return:
// KOD_TRUE�F��������    KOD_FALSE:�������Ȃ�����
//int NURBS_Func::CalcIntersecPtNurbsPt(NURBSS *S,Coord P,int Divnum,int LoD,Coord *Q)
int NURBSS::CalcIntersecPtNurbsPt(Coord P,int Divnum,int LoD,Coord *Q)
{
	Matrix dF = NewMatrix(3,3);		// Fu,Fv,Ft���\������3x3�s��
	Matrix dF_ = NewMatrix(3,3);	// dF�̋t�s����i�[
	Coord F,Fu,Fv,Ft;				// F(u,v,t) = S(u,v) - P - t�EN(u,v)	�j���[�g���@�ɂ�����֐�
	Coord N,Nu,Nv;					// N(u,v):S(u,v)��̖@���x�N�g��
	Coord d = InitCoord();			// �j���[�g���@�ɂ���čX�V�����X�e�b�v�T�C�Y�p�����[�^
	int loopcount=0;				// while()���[�v�̃J�E���g
	double u,v,t;					// u,v,t�̌��ݒl
	double dv = (V[1] - V[0])/(double)Divnum;	// �������Z�p��v�p�����[�^�̃C���^�[�o���l
	double du = (U[1] - U[0])/(double)Divnum;	// �������Z�p��u�p�����[�^�̃C���^�[�o���l
	int flag = KOD_FALSE;			// while()�����p���ʃt���O
	Coord *Q_ = NewCoord1(Divnum*Divnum);			// ���̈ꎞ�i�[�p

	// �e�����l�ɑ΂��ăj���[�g���@�K�p
	for(int i=0;i<Divnum;i++){
		for(int j=0;j<Divnum;j++){
			u = U[0] + (double)i*du;			// �X�e�b�v�p�����[�^u�̏����l���Z�b�g
			v = V[0] + (double)j*dv;			// �X�e�b�v�p�����[�^v�̏����l���Z�b�g
			t = 0;								// �X�e�b�v�p�����[�^t�̏����l���Z�b�g
			loopcount = 0;
			flag = KOD_FALSE;

			// �����v�Z
			while(loopcount < LOOPCOUNTMAX){
//				N = CalcNormVecOnNurbsS(S,u,v);									// S(u,v)��̖@���x�N�g��N(u,v)���Z�o
//				Nu = CalcDiffuNormVecOnNurbsS(S,u,v);								// N(u,v)��u�����Δ���
//				Nv = CalcDiffvNormVecOnNurbsS(S,u,v);								// N(u,v)��v�����Δ���
//				F = SubCoord(SubCoord(CalcNurbsSCoord(S,u,v),P),MulCoord(N,t));	// �j���[�g���@�ɂ�����֐�
//				Fu = SubCoord(CalcDiffuNurbsS(S,u,v),MulCoord(Nu,t));				// F��u�����Δ���
//				Fv = SubCoord(CalcDiffvNurbsS(S,u,v),MulCoord(Nv,t));				// F��v�����Δ���
				N = CalcNormVecOnNurbsS(u,v);									// S(u,v)��̖@���x�N�g��N(u,v)���Z�o
				Nu = CalcDiffuNormVecOnNurbsS(u,v);								// N(u,v)��u�����Δ���
				Nv = CalcDiffvNormVecOnNurbsS(u,v);								// N(u,v)��v�����Δ���
				F = SubCoord(SubCoord(CalcNurbsSCoord(u,v),P),MulCoord(N,t));	// �j���[�g���@�ɂ�����֐�
				Fu = SubCoord(CalcDiffuNurbsS(u,v),MulCoord(Nu,t));				// F��u�����Δ���
				Fv = SubCoord(CalcDiffvNurbsS(u,v),MulCoord(Nv,t));				// F��v�����Δ���
				Ft = MulCoord(N,-1);												// F��t�����Δ���
				dF[0][0] = Fu.x;		// 3x3�}�g���b�N�X��Fu,Fv,Ft����
				dF[0][1] = Fv.x;
				dF[0][2] = Ft.x;
				dF[1][0] = Fu.y;
				dF[1][1] = Fv.y;
				dF[1][2] = Ft.y;
				dF[2][0] = Fu.z;
				dF[2][1] = Fv.z;
				dF[2][2] = Ft.z;

				if((flag = MatInv3(dF,dF_)) == KOD_FALSE){		// �t�s��Z�o det��0�Ȃ玟�̏����l��
					//fprintf(stderr,"%d:det = 0\n",loopcount);	// debug
					break;
				}

				d = MulCoord(MulMxCoord(dF_,F),-1);		// �X�e�b�v�T�C�Y�p�����[�^�̍X�V�l���Z�o

				if(fabs(d.x) <= APPROX_ZERO_L && fabs(d.y) <= APPROX_ZERO_L && fabs(d.z) <= APPROX_ZERO_L){	// �^�l�Ɏ���������loop�𔲂���
					flag = KOD_TRUE;		// �����t���Otrue
					break;
				}

				// �^�l�ɒB���Ă��Ȃ�������u,v,t���X�V
				u += d.x/(double)LoD;
				v += d.y/(double)LoD;
				t += d.z/(double)LoD;
				//fprintf(stderr,"%d:%lf,%lf,%lf,%lf,%lf,%lf\n",loopcount,u,v,t,d.x,d.y,d.z);	// debug

				loopcount++;
			}// end of while

			if(flag == KOD_TRUE)	Q_[i*Divnum+j] = SetCoord(u,v,t);		// �������Ă�����

			else Q_[i*Divnum+j] = SetCoord(KOD_ERR,KOD_ERR,KOD_ERR);		// �������Ă��Ȃ�������

		}// end of loop j
	}// end of loop i

	flag = GetMinDist(P,Q_,Divnum*Divnum,Q);		// �ɏ����ɂȂ�Ȃ��悤�C�S�Ẳ��̂����C�������ŏ��̂��̂�^�̉��Ƃ��đI�яo��

	FreeMatrix(dF,3);
	FreeMatrix(dF_,3);
	FreeCoord1(Q_);

	return flag;
}

// Function: CalcIntersecPtNurbsPt
// ��ԏ��1�_P����NURBS�Ȑ�C��̍ŋߖT�_Q(�Ȑ��p�����[�^)�����߂�(�j���[�g���@)
// 
// >F(t) = (P-C'(t))�C'(t) = 0
// >F'(t)dt = -F(t)
// >F'(t) = -|C'(t)|^2 + (P+C(t))�C"(t)
//
// Parameters:
// *C - NURBS�Ȑ�
// P - ��ԏ��1�_
// Divnum - �j���[�g���@�����l�w��p�̋Ȑ�������
// LoD - �j���[�g���p�����[�^�X�V���̃X�e�b�v�T�C�Y(1�`)
// Q - ���iC��̓_��t�p�����[�^�Ŋi�[�j
// 
// Return:
// KOD_TRUE�F��������    KOD_FALSE:�������Ȃ�����
//int NURBS_Func::CalcIntersecPtNurbsPt(NURBSC *C,Coord P,int Divnum,int LoD,double *Q)
int NURBSC::CalcIntersecPtNurbsPt(Coord P,int Divnum,int LoD,double *Q)
{
	double *t_buf = NewVector(Divnum);					// �������i�[�p�o�b�t�@
	double *dist_buf = NewVector(Divnum);				// �et�ł̋����i�[�p�o�b�t�@
	double delta = (V[1] - V[0])/(double)Divnum;	// �������Z�p��t�p�����[�^�̃C���^�[�o���l

	for(int i=0;i<Divnum;i++){
		double t = V[0] + (double)i*delta;	// t�̏����l���Z�b�g
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
			if(fabs(d-c) <= APPROX_ZERO)	break;			// ���ꂪ�[���Ȃ玟�̏����_��
			double dt = (b-a)/(d-c);
			t += dt/(double)LoD;				// t�X�V
			if(fabs(dt) <= APPROX_ZERO_L){	// �������Ă��������ێ�������t��
				t_buf[i] = t;
//				dist_buf[i] = CalcDistance(CalcNurbsCCoord(C,t),P);	// PQ�ԋ����𓾂�
				dist_buf[i] = CalcDistance(CalcNurbsCCoord(t),P);	// PQ�ԋ����𓾂�
				break;
			}
			loopcount++;
			t_buf[i] = dist_buf[i] = -1;		// �������Ă��Ȃ�������C�G���[�t���O�Ƃ���-1����
		}
	}

	// ����ꂽ������CPQ�Ԃ̋������ł��Z�����̂�I��
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
// (private)CalcIntersecPtNurbsPt()�̃T�u�֐��D�ŏ������𒲂ׂ�
//
// Parameters:
// *S - NURBS�Ȗʂւ̃|�C���^
// P - ��ԏ��1�_
// *Q - �Ȗʏ�̓_�Q(u,v�p�����[�^�Ŏw��)
// N - �_��
// *Ans - �ŏ����������Ȗʏ�̓_
//
// Return:
// �����FKOD_TRUE, ���s�FKOD_ERR
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
// ���ڒ���NURBS�Ȗʏ��1�_(u,v)���g���~���O�̈���ɂ���̂��𔻒肷��
// 
// Parameters:
// *Trim - �g�����Ȗ�
// u,v - �g�����Ȗʏ��1�_(u, v)
//
// Return:
// KOD_TRUE:�ʏ�  KOD_ONEDGE:�G�b�W��  KOD_FALSE:�ʊO   KOD_ERR:�G���[
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

// Function: DetermPtOnTRMSurf_sub
// (private)DetermPtOnTRMSurf()�̃T�u�֐��D�ʏ���̃^�C�v�������Ȑ��̏ꍇ�̃g���~���O�̈���O����
//
// Parameter:
// *Conps - �����Ȑ�
// u,v - �g�����Ȗʏ��1�_(u, v)
// 
// Return:
// KOD_TRUE:�ʏ�  KOD_ONEDGE:�G�b�W��  KOD_FALSE:�ʊO   KOD_ERR:�G���[
int NURBS_Func::DetermPtOnTRMSurf_sub(CONPS *Conps,double u,double v)
{
	// �ʏ���������Ȑ��ɂȂ��Ă��邱��
	if(Conps->BType != COMPOSITE_CURVE){
        GuiIF.SetMessage("NURBS_Func ERROR:TRIM������!");
		return KOD_ERR;
	}

//	COMPC *CompC=(COMPC *)Conps->pB;	// NURBS�Ȗʂ̃p�����[�^��ԏ�ɍ\������Ă��镡���Ȑ��ւ̃|�C���^�����o��
	COMPC *CompC=Conps->pB.CompC;	// NURBS�Ȗʂ̃p�����[�^��ԏ�ɍ\������Ă��镡���Ȑ��ւ̃|�C���^�����o��
	Coord *P;							// �g�������E����ɐ��������_(���p�`�ߎ��p�̓_)���i�[
	int ptnum;							// �g�������E����_�Q�ߎ������Ƃ��̓_��

	// �������m��
//	if((P = (Coord *)malloc(sizeof(Coord)*(CompC->N*TRM_BORDERDIVNUM))) == NULL){
	P = new Coord[CompC->N*TRM_BORDERDIVNUM];
	if ( !P ) {
		return KOD_ERR;
	}

	// �g�������E����_�QP�ŋߎ�
	if((ptnum = ApproxTrimBorder(CompC,P)) == KOD_ERR){
			GuiIF.SetMessage("NURBS_Func ERROR:�g�������E����NURBS�Ȑ��ȊO�ō\������Ă��܂�.������!");
			FreeCoord1(P);
			return KOD_ERR;
	}
	
	int trm_flag = KOD_FALSE;							// �g���~���O�̈���O����p�t���O
	Coord TargetPoint = SetCoord(u,v,0);				// �^�[�Q�b�g�ƂȂ�ʏ�̓_(u,v)��Coord�Ɋi�[
	trm_flag = IsPointInPolygon(TargetPoint,P,ptnum);	// ���O����

	return trm_flag;
}

// Function: GetPtsOnOuterTRMSurf
// �O���g�����ʓ��̓_�̂ݎc��
//
// Parameters:
// *Trm - �g�����ʂւ̃|�C���^    
// *Pt - ���ʑΏۂ�(u,v)�Q      
// N - (u,v)�Q�̐�
//
// Return:
// �c�����_�̐��@(�O���g���������݂��Ȃ��FKOD_FALSE)
int NURBS_Func::GetPtsOnOuterTRMSurf(TRMS *Trm,Coord *Pt,int N)
{
	// �O���g���������݂��Ȃ��ꍇ��0�����^�[��
	if(!Trm->n1)
		return KOD_FALSE;

//	COMPC *CompC = (COMPC *)Trm->pTO->pB;	// NURBS�Ȗʂ̃p�����[�^��ԏ�ɍ\������Ă��镡���Ȑ��ւ̃|�C���^�����o��
	COMPC *CompC = Trm->pTO->pB.CompC;	// NURBS�Ȗʂ̃p�����[�^��ԏ�ɍ\������Ă��镡���Ȑ��ւ̃|�C���^�����o��
	Coord *P;								// �g�������E����ɐ��������_(���p�`�ߎ��p�̓_)���i�[
	int ptnum;								// �g�������E����_�Q�ߎ������Ƃ��̓_��

	// �������m��
//	if((P = (Coord *)malloc(sizeof(Coord)*(CompC->N*TRM_BORDERDIVNUM))) == NULL){
	P = new Coord[CompC->N*TRM_BORDERDIVNUM];
	if ( !P ) {
		return KOD_ERR;
	}

	// �g�������E����_�QP�ŋߎ�
	if((ptnum = ApproxTrimBorder(CompC,P)) == KOD_ERR){
			GuiIF.SetMessage("NURBS_Func ERROR:�g�������E����NURBS�Ȑ��ȊO�ō\������Ă��܂�.������!");
			FreeCoord1(P);
			return KOD_ERR;
	}

	Coord *ans = NewCoord1(N);		// �c���_�̊i�[��
	int trm_flag = KOD_FALSE;		// �g���~���O�̈���O����p�t���O
	int n=0;

	// �O���g�����̓����������c��
	for(int i=0;i<N;i++){
		trm_flag = IsPointInPolygon(Pt[i],P,ptnum);		// ���O����
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
// �����g�����ʊO�̓_�̂ݎc��
//
// Parameters:
// *Trm - �g�����ʂւ̃|�C���^    
// *Pt - ���ʑΏۂ�(u,v)�Q      
// N - (u,v)�Q�̐�
//
// Retrun:
// �c�����_�̐��@(�����g���������݂��Ȃ��FKOD_FALSE)
int NURBS_Func::GetPtsOnInnerTRMSurf(TRMS *Trm,Coord *Pt,int N)
{
	// �����g���������݂��Ȃ��ꍇ��0�����^�[��
	if(!Trm->n2){
		return KOD_FALSE;
	}

	COMPC *CompC;				// NURBS�Ȗʂ̃p�����[�^��ԏ�ɍ\������Ă��镡���Ȑ��ւ̃|�C���^�����o��
	Coord *P;					// �g�������E����ɐ��������_(���p�`�ߎ��p�̓_)���i�[
	int ptnum;					// �g�������E����_�Q�ߎ������Ƃ��̓_��
	Coord *ans = NewCoord1(N);	// �c���_�̊i�[��
	int trm_flag = KOD_FALSE;	// �g���~���O�̈���O����p�t���O
	int N_ = N;

	// �����g�����̐��������[�v
	for(int k=0;k<Trm->n2;k++){

//		CompC = (COMPC *)Trm->pTI[k]->pB;	
		CompC = Trm->pTI[k]->pB.CompC;	

		// �������m��
//		if((P = (Coord *)malloc(sizeof(Coord)*(CompC->N*TRM_BORDERDIVNUM))) == NULL){
		P = new Coord[CompC->N*TRM_BORDERDIVNUM];
		if ( !P ) {
			return KOD_ERR;
		}

		// �g�������E����_�QP�ŋߎ�
		if((ptnum = ApproxTrimBorder(CompC,P)) == KOD_ERR){
			GuiIF.SetMessage("NURBS_Func ERROR:�g�������E����NURBS�Ȑ��ȊO�ō\������Ă��܂�.������!");
			FreeCoord1(P);
			return KOD_ERR;
		}

		// �����g�����̊O���������c��
		int n=0;
		for(int i=0;i<N_;i++){
			trm_flag = IsPointInPolygon(Pt[i],P,ptnum);		// ���O����
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
// ���O���g�����ʓ��̓_�̂ݎc��
//
// Parameters:
// *Trm - �g�����ʂւ̃|�C���^    
// *Pt - ���ʑΏۂ�(u,v)�Q      
// N - (u,v)�Q�̐�
//
// Return:
// �c�����_�̐��@(�����g���������݂��Ȃ��FKOD_FALSE)
int NURBS_Func::GetPtsOnInnerOuterTRMSurf(TRMS *Trm,Coord *Pt,int N)
{
	int n=0;

	n = GetPtsOnInnerTRMSurf(Trm,Pt,N);		// �����g����

	if(n == KOD_FALSE)
		n = N;

	n = GetPtsOnOuterTRMSurf(Trm,Pt,n);		// �O���g����

	return n;
}

// Function: ApproxTrimBorder
// (private)�g�������E����_�Q�ŋߎ�����
//
// Parameters:
// *CompC - �g�������E�����\�����镡���Ȑ��ւ̃|�C���^
// *P - �ߎ����ꂽ�_�Q���i�[���邽�߂�Coord�z��
//
// Return:
// �ߎ������_�Q�̐��i�g�������E����NURBS�Ȑ��ȊO�ō\������Ă����ꍇ�́CKOD_ERR�j
int NURBS_Func::ApproxTrimBorder(COMPC *CompC,Coord *P)
{
	double ent_dev=0;				// �����_�p�����[�^
	NURBSC *NurbsC;					// �g�������E��(NURBS�Ȑ�)�̃|�C���^����Ɨp�Ɋi�[
	int trm_flag = KOD_FALSE;		// �g���~���O�̈���O����p�t���O
	int divnum = TRM_BORDERDIVNUM;	// �e���E���̕�����
	int ptnum=0;					// �S�̂̓_�����J�E���g

	// �g�������E����ɓ_�𐶐��i�g�������E���𑽊p�`�ߎ��j
	for(int i=0;i<CompC->N;i++){
		// �g�������E����NURBS�Ȑ��ō\������Ă���
		if(CompC->DEType[i] == NURBS_CURVE){
//			NurbsC = (NURBSC *)CompC->pDE[i];	// ���ڒ���Nurbs�Ȑ��̃|�C���^���擾
			NurbsC = CompC->pDE[i].NurbsC;	// ���ڒ���Nurbs�Ȑ��̃|�C���^���擾
			if(NurbsC->K == 2 && CompC->DegeFlag == KOD_TRUE)	divnum = 2;		// �R���g���[���|�C���g��2�̏ꍇ�͒����Ȃ̂ŁA�����_�𐶐����Ȃ��Ă��悭����
			else divnum = TRM_BORDERDIVNUM;
			for(int j=0;j<divnum-1;j++){
				ent_dev = NurbsC->T[NurbsC->M-1]+(NurbsC->T[NurbsC->K]-NurbsC->T[NurbsC->M-1])*(double)j/((double)divnum-1);	// �����_t�����߂�
//				P[ptnum] = CalcNurbsCCoord(NurbsC,ent_dev);	// NURBS�Ȗʂ̃p�����[�^��ԓ���NURBS�Ȑ��̕����_t�̍��W�l(u,v)�𓾂�
				P[ptnum] = NurbsC->CalcNurbsCCoord(ent_dev);	// NURBS�Ȗʂ̃p�����[�^��ԓ���NURBS�Ȑ��̕����_t�̍��W�l(u,v)�𓾂�
				ptnum++;
			}
		}
		// ����ȊO
		else{
			GuiIF.SetMessage("NURBS_Func ERROR:�g�������E����NURBS�Ȑ��ȊO�ō\������Ă��܂�.������!");
			return KOD_ERR;
		}
	}

	return ptnum;
}

// Function: CalcDeltaPtsOnNurbsC
// �w�肵����������NURBS�Ȑ���̍��W�l���o�͂���
// 
// Parameters:
// *Nurb - NURBS�ւ̃|�C���^  
// D - ������  
// *Pts - �o�͂������W�l���i�[
//
// Return:
// �_��
//int NURBS_Func::CalcDeltaPtsOnNurbsC(NURBSC *Nurb,int D,Coord *Pts)
int NURBSC::CalcDeltaPtsOnNurbsC(int D,Coord *Pts)
{
	double T = (V[1] - V[0])/D;	// �p�����g���b�N��ԓ��ł̐������𓾂�

	for(int i=0;i<=D;i++){
		Pts[i] = CalcNurbsCCoord(V[0] + T*(double)i);
	}

	return D+2;
}

// Function: CalcDeltaPtsOnNurbsC
// �w�肵���Ԋu��NURBS�Ȑ���̍��W�l���o�͂���
//
// Parameters:
// *Nurb - NURBS�Ȑ��ւ̃|�C���^  
// D - �Ԋu  
// *Pts - �o�͂������W�l���i�[
//
// Return:
// �_���iD��0�C���邢�͎w�肵��NURBS�Ȑ��̑S����蒷�������ꍇ�́CKOD_ERR�j
//int NURBS_Func::CalcDeltaPtsOnNurbsC(NURBSC *Nurb,double D,Coord *Pts)
int NURBSC::CalcDeltaPtsOnNurbsC(double D,Coord *Pts)
{
	if(D == 0){
		GuiIF.SetMessage("NURBS_Func ERROR: Set Correct Interval Value");
		return KOD_ERR;
	}

//	double L = CalcNurbsCLength(Nurb);		// NURBS�Ȑ��̐������𓾂�
	double L = CalcNurbsCLength();		// NURBS�Ȑ��̐������𓾂�
	if(D > L){
		GuiIF.SetMessage("NURBS_Func ERROR: Arc Length > Whole Lenth of the Curve");
	}
	//fprintf(stderr,"L = %lf\n",L);		// debug
	//fprintf(stderr,"D = %lf\n",D);		// debug

	int k=1;			// �����J�E���^
	double t = (V[1] - V[0])/(L/D);	// t�̏����l���Z�b�g

	while(t <= V[1]){
//		t = CalcParamLengthOnNurbsC(Nurb,(double)k*D,t);	// ����T��
//		Pts[k-1] = CalcNurbsCCoord(Nurb,t);		// ����o�^
		t = CalcParamLengthOnNurbsC((double)k*D,t);	// ����T��
		Pts[k-1] = CalcNurbsCCoord(t);		// ����o�^
		k++;
		t = k*(V[1] - V[0])/(L/D);	// ����t�̏����l���Z�b�g
	}

	return k-1;
}

// Function: CalcParamLengthOnNurbsC
// NURBS�Ȑ��ɂ����Ĉ�[����̎w�苗���ɂ�����p�����[�^�l��Ԃ�
//
// Parameters:
// *C - NURBS�Ȑ�
// L - �w�苗��
// Init_t - ��T���̏����p�����[�^
//
// Return:
// �w�苗���ɂ�����p�����[�^�l
//double NURBS_Func::CalcParamLengthOnNurbsC(NURBSC *C,double L,double Init_t)
double NURBSC::CalcParamLengthOnNurbsC(double L,double Init_t)
{
	double dt = 1E+12;			// �X�e�b�v�T�C�Y�p�����[�^�̏����l
	double t = Init_t;
	int count = 0;

	while(fabs(dt) > APPROX_ZERO){
//		dt = (L - CalcNurbsCLength(C,0,t))/CalcEuclid(CalcDiffNurbsC(C,t))/2;		// �j���[�g���@�ɂ������v�Z
		dt = (L - CalcNurbsCLength(0,t))/CalcEuclid(CalcDiffNurbsC(t))/2;		// �j���[�g���@�ɂ������v�Z
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
// �w�肵����������NURBS�Ȗʏ�̍��W�l�����߂�
// 
// Parameters:
// *S - NURBSS�ւ̃|�C���^  
// Du,Dv - u�����Cv�����̕�����  
// **Pts - �o�͂������W�l���i�[
//
// Return:
// �_��
//int NURBS_Func::CalcDeltaPtsOnNurbsS(NURBSS *S,int Du,int Dv,Coord **Pts)
int NURBSS::CalcDeltaPtsOnNurbsS(int Du,int Dv,Coord **Pts)
{
	double u_val = (U[1] - U[0])/Du;		// �p�����g���b�N��ԓ��ł�u�����������𓾂�
	double v_val = (V[1] - V[0])/Dv;		// �p�����g���b�N��ԓ��ł�v�����������𓾂�

	// u�����Cv�����̊e�����_�ɂ�������W�l�����߂�
	int num=0;
	for(int i=0;i<=Du;i++){
		for(int j=0;j<=Dv;j++){
//			Pts[i][j] = CalcNurbsSCoord(S,S->U[0]+u_val*i,S->V[0]+v_val*j);	// �w�肵��(u,v)�̍��W�l�����߂�
			Pts[i][j] = CalcNurbsSCoord(U[0]+u_val*i,V[0]+v_val*j);	// �w�肵��(u,v)�̍��W�l�����߂�
			num++;
		}
	}
	
	return num;
}

// Funciton: RemoveTheSamePoints
// (private)����_����������
//
// Parameters:
// *S - �Ȗ� 
// *Q - �Ȗʏ��(u,v)�p�����[�^�Q(�ύX��̓_�Q�������Ɋi�[�����)   
// N - �_��
//
// Return:
// �ύX��̓_��
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
// NURBS�Ȑ��̎w�肵�������ɂ�����ɒl�̍��W�l�𓾂�
//
// Parameters:
// *C - �ɒl���W�����߂���NURBS�Ȑ��ւ̃|�C���^   
// nf - �����x�N�g��     
// *pt - ����ꂽ�ɒl��Nurbs�Ȑ��p�����[�^�l��    
// ptnum - *pt�̔z��
//
// Return:
// ����ꂽ�ɒl�p�����[�^�̐��iKOD_FALSE:�����Ȃ�����, KOD_ERR:�ɒl�p�����[�^�̐���ptnum�𒴂����j
//int NURBS_Func::CalcExtremumNurbsC(NURBSC *C,Coord nf,double *pt,int ptnum)
int NURBSC::CalcExtremumNurbsC(Coord nf,double *pt,int ptnum)
{
	int anscount=0;			// �ɒl�̐�

	// NURBS�Ȑ��̃p�����[�^��Ԃ�CONVDIVNUM�ŋ�؂�A���ꂼ��ɑ΂��ăj���[�g���@�ɂ������v�Z���s��
	for(int i=0;i<=CONVDIVNUM;i++){
		double t = V[0] + (V[1] - V[0])/CONVDIVNUM*(double)i;	// �T���J�n�p�����[�^�l
		double dt=0;					// �j���[�g���@�p�̑����l
		int lpcount=0;					// �����v�Z��
		bool flag = false;				// ��O�t���O

		// �����v�Z
		while(lpcount < LOOPCOUNTMAX){
//			double f_ = CalcInnerProduct(nf,CalcDiffNurbsC(C,t));
//			double f__ = CalcInnerProduct(nf,CalcDiff2NurbsC(C,t));
			double f_ = CalcInnerProduct(nf,CalcDiffNurbsC(t));
			double f__ = CalcInnerProduct(nf,CalcDiff2NurbsC(t));
			if(f__ == 0.0)	break;
			dt = f_/f__;

			if(CheckZero(dt,MID_ACCURACY)){			// ��������
				flag = true;
				break;
			}
			t -= dt;	// �j���[�g���p�����[�^�X�V
			if(t < V[0] || t > V[1])	break;		// �͈͊O�ɏo��
			lpcount++;
		}// End while

		// �������Ă�����
		if(flag == true){
			pt[anscount] = t;	// ���Ƃ��ēo�^
			anscount++;
			if(anscount == ptnum){
                GuiIF.SetMessage("NURBS_ERROR:range over");
				return KOD_ERR;
			}
		}

	}// End for i

	anscount = CheckTheSamePoints(pt,anscount);		// ����_����������

	return anscount;
}

// Function: CalcExtSearchCurve
// �i�������j�ɒl�T�����𓾂�
// 
// Parameters:
// *S - �ΏۂƂ���NURBS�Ȑ�
// n - �@���x�N�g��
// pt - 
// ds - �ɒl�T������ǐՂ���ۂ̍��ݕ�
// *C1 - ����ꂽ�ɒl�T�����iNURBS�Ȑ��j
// *C2 -  ����ꂽ�ɒl�T�����iNURBS�Ȑ��j�i�ɒn�T������2������j
//
// Return:
// KOD_TRUE
int NURBS_Func::CalcExtSearchCurve(NURBSS *S,Coord n,Coord pt,double ds,NURBSC *C1,NURBSC *C2)
{
	// �H����
	return KOD_TRUE;
}

// Function: CalcExtGradCurve
// �i�������j�ɒl�X�ΐ��𓾂�
//
// Parameters:
// *S - �ΏۂƂ���NURBS�Ȑ�
// n - �@���x�N�g��
// pt - 
// ds - �ɒl�X�ΐ���ǐՂ���ۂ̍��ݕ�
// *C1 - ����ꂽ�ɒl�X�ΐ��iNURBS�Ȑ��j
// *C2 -  ����ꂽ�ɒl�X�ΐ��iNURBS�Ȑ��j�i�ɒl�X�ΐ���2������j
//
// Return:
// KOD_TRUE
int NURBS_Func::CalcExtGradCurve(NURBSS *S,Coord n,Coord pt,double ds,NURBSC *C1,NURBSC *C2)
{
	// �H����
	return KOD_TRUE;
}

// Funciton: TrimNurbsSPlane
// NURBS�Ȗʂ𕽖ʂŃg��������
//
// Parameters:
// *Trm - �g�����ʁi�g�������ꂽ�ʂ������ɓ���j
// pt - ���ʏ��1�_
// nvec - ���ʂ̖@���x�N�g��
//
// Return:
// KOD_TRUE
int NURBS_Func::TrimNurbsSPlane(TRMS *Trm,Coord pt,Coord nvec)
{
	Coord t[2000];					// ��
	int   num;						// ���̐�
	double pcolor[3] = {0,1,0};		// �\���̐F
	double tcolor[3] = {1,0,0};


//	num = CalcIntersecPtsPlaneSearch(Trm->pts,pt,nvec,0.5,5,t,2000,RUNGE_KUTTA);		// NURBS�Ȗʂƕ��ʂƂ̌�_�Q������ǐՖ@�ŋ��߂�
	num = Trm->pts->CalcIntersecPtsPlaneSearch(pt,nvec,0.5,5,t,2000,RUNGE_KUTTA);		// NURBS�Ȗʂƕ��ʂƂ̌�_�Q������ǐՖ@�ŋ��߂�
	
	// �p�����g���b�N�̈���Œ����ߎ�(�ŏ�2��@�ŋߎ������̌W��2�����߂�)
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
	MulMxVec(A_,2,2,B,2,B_);		// �����̌W����B_�Ɋi�[�����By = B_[0]x + B_[1]

	// �[�_���o
	// �p�����g���b�N�̈����U-V�͈̔͂����߂�4�_���瓾����4�{�̒����ƁA���������߂��ߎ������Ƃ̌�_4�����߂�
	Coord P[4];
	P[0] = TrimNurbsSPlaneSub1(B_[0],B_[1],Trm->pts->U[0],Trm->pts->V[0],Trm->pts->U[1],Trm->pts->V[0]);
	P[1] = TrimNurbsSPlaneSub1(B_[0],B_[1],Trm->pts->U[1],Trm->pts->V[0],Trm->pts->U[1],Trm->pts->V[1]);
	P[2] = TrimNurbsSPlaneSub1(B_[0],B_[1],Trm->pts->U[1],Trm->pts->V[1],Trm->pts->U[0],Trm->pts->V[1]);
	P[3] = TrimNurbsSPlaneSub1(B_[0],B_[1],Trm->pts->U[0],Trm->pts->V[1],Trm->pts->U[0],Trm->pts->V[0]);
	// ����ꂽ4�̌�_P����AU-V�͈͓��ɂ���2�_�𒊏o
	Coord Q[2];
	int j=0;
	for(int i=0;i<4;i++){
		if(P[i].x >= Trm->pts->U[0] && P[i].x <= Trm->pts->U[1] && P[i].y >= Trm->pts->V[0] && P[i].y <= Trm->pts->V[1]){
			Q[j] = SetCoord(P[i]);
			j++;
		}
	}
	// ����ꂽ2�̓_Q����NURBS�Ȑ�(����)�𐶐�
	double T[4] = {0,0,1,1};
	double W[2] = {1,1};
	double V[2] = {0,1};
	int prop[4] = {0,0,1,0};
	Coord cp[2];
	InitCoord(&cp[0]);
	InitCoord(&cp[1]);
	//GenNurbsC(&body->CompC[i].DegeNurbs,2,2,4,T,W,cp,V,prop,1);	

	// ���łɓo�^����Ă���O���g�����Ȑ��ƁA�V���ɓ��o�����O���g�����p����Q����A�V���ȕȐ����`��
	

	FILE *fp = fopen("Debug.csv","w");
	for(int i=0;i<num;i++){
//		Coord p = CalcNurbsSCoord(Trm->pts,t[i].x,t[i].y);			// ��_���p�����[�^�l������W�l�֕ϊ�
		Coord p = Trm->pts->CalcNurbsSCoord(t[i].x,t[i].y);			// ��_���p�����[�^�l������W�l�֕ϊ�
		DrawPoint(p,1,3,pcolor);			// ��_��`��
		fprintf(fp,"%lf,%lf\n",t[i].x,t[i].y);
	}
	fclose(fp);

	return KOD_TRUE;
}

// Function: SearchExtremum_BS
// Bulirsch-Stoer�@�ɂ��ɒn�T�����s��(�����������Fdu(s)/ds = fu(u,v) �ƁAdv(s)/ds = fv(u,v)�̉�T��)
// 
// Parameters:
// *S - �ɒl�T�������NURBS�Ȗʂւ̃|�C���^
// nf - ���ʂ̖@���x�N�g��
// u0,v0 - �J�n�_
// H - �T����
// param - u������1�K������0�ƂȂ�ɒl�̒T��(PARAM_U) or v�����T��(PARAM_V)�̑I��
// direction - �������T��(FORWARD) or�t�����T��(INVERSE)
// *ans - �X�V���ꂽu,v�p�����[�^(ans.x = u, ans.y = v)
//
// Return:
// KOD_TRUE:����I��,  KOD_FALSE:���ٓ_�ɂ�菈���𒆒f,  KOD_ERR:�p�����[�^�̎w��~�X�ɂ�菈���𒆒f
//int NURBS_Func::SearchExtremum_BS(NURBSS *S,Coord nf,double u0,double v0,double H,int param,int direction,Coord *ans)
int NURBSS::SearchExtremum_BS(Coord nf,double u0,double v0,double H,int param,int direction,Coord *ans)
{
	// �����w��~�X
	if(direction != FORWARD && direction != INVERSE){
        GuiIF.SetMessage("NURBS ERROR: selected wrong direction");
		return KOD_ERR;
	}

	int    n[11] = {2,4,6,8,12,16,24,32,48,64,96};		// B-S�@�̕������Q���w��
	Coord  z[97];							// �C�����_�@�̒��Ԓl���i�[(z.x = u, z.y = v)
	Coord  f;								// f.x = fu(u,v), f.y = fv(u,v)
	Coord  D[10][10],C[10][10],P[11];		// B-S�@�̒��ԃp�����[�^
	double h[11];							// B-S�@�̍��ݕ�
	Coord  R;								// h=0�̊O�}�l
	int    conv_flag = KOD_FALSE;			// �����t���O

	// �e�������ɂ����鍏�ݕ������߂Ă���
	for(int i=0;i<11;i++)
		h[i] = H/n[i];
	
	// ���ݕ��������������珇�ɕύX���Ȃ���AB-S�@�ɂ��O�}�l���v�Z���Ă���
	for(int i=0;i<11;i++){

		// �܂��Au(s+H)�̒l���C�����_�@�ɂ��v�Z����
		z[0] = SetCoord(u0,v0,0);										// z0��z1�̎Z�o�͕ʏ���
		if(GetSECParam1(u0,v0,nf,param,direction,&f) == KOD_FALSE)	// z0�ł̔����������̉E�ӂ��v�Z
			return KOD_FALSE;
			//fprintf(stderr,"f%d=(%lf,%lf)\n",i,f.x,f.y);
		z[1] = AddCoord(z[0],MulCoord(f,h[i]));							// z0��z1�̎Z�o�͕ʏ���
		for(int j=1;j<n[i];j++){
			if(GetSECParam1(z[j].x,z[j].y,nf,param,direction,&f) == KOD_FALSE)	// zj�ł̔����������̉E�ӂ��v�Z
				return KOD_FALSE;
			z[j+1] = AddCoord(z[j-1],MulCoord(f,2*h[i]));				// z2�`zn�܂ł��Z�o
		}
		if(GetSECParam1(z[n[i]].x,z[n[i]].y,nf,param,direction,&f) == KOD_FALSE)	// zn�ł̔����������̉E�ӂ��v�Z
			return KOD_FALSE;
		P[i] = DivCoord(AddCoord(AddCoord(z[n[i]],z[n[i]-1]),MulCoord(f,h[i])),2);		// u(s+H)
			//fprintf(stderr,"P%d=(%lf,%lf)\n",i,P[i].x,P[i].y);

		// B-S�@�̍����\���������߂Ă���
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
			R = AddCoord(R,D[k][i-1-k]);		// �O�}�l
			//fprintf(stderr,"%d,D%d=(%lf,%lf)\n",i,k,D[k][i-1-k].x,D[k][i-1-k].y);
		}

		// fprintf(stderr,"%d,%lf,%.16lf\n",i,h[i],CalcEuclid2D(D[0][i-1].x,D[0][i-1].y));

		// D[0][i-1]�������臒l�����������Ȃ�����A���̂Ƃ��̊O�}�l�����Ƃ��ĉ��Z�������I������
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
// (private)�ɒl�T����Sub�֐�1
// 
// Parameters:
// *S - NURBS�Ȗ�
// u, v - ���ڒ���(u, v)�p�����[�^
// nf - ���ʂ̖@���x�N�g��
// param - u������1�K������0�ƂȂ�ɒl�̒T��(PARAM_U) or v�����T��(PARAM_V)�̑I��
// direction - �������T��(FORWARD) or�t�����T��(INVERSE)
// *f - f.x = fu(u,v), f.y = fv(u,v)
// 
// Return:
// �����FKOD_TURE, ���ٓ_�ɂ������𒆒f�����FKOD_FALSE
//int NURBS_Func::GetSECParam1(NURBSS *S,double u,double v,Coord nf,int param,int direction,Coord *f)
int NURBSS::GetSECParam1(double u,double v,Coord nf,int param,int direction,Coord *f)
{
//	double fuu = CalcInnerProduct(nf,CalcDiffNNurbsS(S,2,0,u,v));	// nf�ESuu
//	double fuv = CalcInnerProduct(nf,CalcDiffNNurbsS(S,1,1,u,v));	// nf�ESuv
//	double fvv = CalcInnerProduct(nf,CalcDiffNNurbsS(S,0,2,u,v));	// nf�ESvv
//	Coord Su = CalcDiffuNurbsS(S,u,v);		// �Ȗʂ�u����1�K����
//	Coord Sv = CalcDiffvNurbsS(S,u,v);		// �Ȗʂ�v����1�K����
	double fuu = CalcInnerProduct(nf,CalcDiffNNurbsS(2,0,u,v));	// nf�ESuu
	double fuv = CalcInnerProduct(nf,CalcDiffNNurbsS(1,1,u,v));	// nf�ESuv
	double fvv = CalcInnerProduct(nf,CalcDiffNNurbsS(0,2,u,v));	// nf�ESvv
	Coord Su = CalcDiffuNurbsS(u,v);		// �Ȗʂ�u����1�K����
	Coord Sv = CalcDiffvNurbsS(u,v);		// �Ȗʂ�v����1�K����
	double E = CalcInnerProduct(Su,Su);		// 1���K�i��
	double F = CalcInnerProduct(Su,Sv);		// 1���K�i��
	double G = CalcInnerProduct(Sv,Sv);		// 1���K�i��
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
// (private)TrimNurbsSPlane�̃T�u�֐�(2D���2�����̌�_�����Ƃ߂�)
//
// Parameters:
// a,b - 1�ڂ̒����̌W��
// x0, y0, x1, y1 - 2�ڂ̒������ʂ�2�_
//
// Return:
// ��_��2D���W�l
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
// (private)�e�ʉߓ_�̋Ȑ��p�����[�^���Z�o(�R�[�h���̔䂩��Z�o)
//
// Parameters:
// *P - �ʉߓ_��   
// PNum - �ʉߓ_��̐�    
// T_ - �Ȑ��p�����[�^���i�[
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
// (private)�e�ʉߓ_�̋Ȑ��p�����[�^���Z�o(�R�[�h���̕������̔䂩��Z�o)
//
// Parameters:
// *P - �ʉߓ_��   
// PNum - �ʉߓ_��̐�    
// T_ - �Ȑ��p�����[�^���i�[
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
// (private)��ԋȖʗpu,v�p�����[�^�𓾂�
// 
// Parameters:
// S - u�����Ȑ��p�����[�^
// T - v�����Ȑ��p�����[�^ 
// **P - �^����ꂽ�_��
// uNum, vNum - u�����Cv�����̓_��
//void NURBS_Func::GetSurfaceKnotParam(Vector S,Vector T,Coord **P,int uNum,int vNum)
void GetSurfaceKnotParam(Vector S,Vector T,Coord **P,int uNum,int vNum)
{
	double d;
	Matrix p_ = NewMatrix(uNum,vNum);

	// u�����̒ʉߓ_��̋Ȑ��p�����[�^�𓾂�
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

	// v�����̒ʉߓ_��̋Ȑ��p�����[�^�𓾂�
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
// (private)�Ȑ�/�Ȗʃp�����[�^���瓙�Ԋu�ȃm�b�g�x�N�g�����Z�o
// 
// Parameters:
// K - �R���g���[���|�C���g�̐�  
// M - �K��   
// T - �i�[����m�b�g�x�N�g����
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
// (private)�Ȑ�/�Ȗʃp�����[�^�����ԗp�m�b�g�x�N�g�����Z�o
// 
// Parameters:
// T_ - �Ȑ��p�����[�^��  
// N - �m�b�g�x�N�g���̐�  
// K - �R���g���[���|�C���g�̐�  
// M - �K��   
// T - �i�[����m�b�g�x�N�g����
//void NURBS_Func::GetInterpolatedKnot(Vector T_,int N,int K,int M,Vector T)
void GetInterpolatedKnot(Vector T_,int N,int K,int M,Vector T)
{
	for(int i=0;i<M;i++)
		T[i] = 0;

	// T_���Q�l�ɂ���
	for(int j=1;j<K-M+1;j++){
		double d=0;
		for(int i=j;i<j+M-1;i++){
			d += T_[i];
		}
		T[j+M-1] = d/((double)M-1);
	}

	// ���Ԋu�ɐݒ�
	//for(int i=M;i<K;i++)
	//	T[i] = ((double)i-(double)M+1)/((double)K-(double)M+1);

	for(int i=K;i<K+M;i++)
		T[i] = 1;
}

// Function: GetApproximatedKnot
// (private)�Ȑ�/�Ȗʃp�����[�^����ߎ��p�m�b�g�x�N�g�����Z�o
// 
// Parameters:
// T_ - �Ȑ��p�����[�^��  
// N - �Ȑ��p�����[�^�̐�  
// M - �K��  
// K - �R���g���[���|�C���g�̐�  
// T - �i�[����m�b�g�x�N�g����
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
		T[j+M-1] += 0.0001;					// ��!  T��T_�����l�ɂȂ�ƁA�ŏ��Q��@�����܂������Ȃ��̂ŁA�֋X�I�ɓ��l�ɂȂ�Ȃ��悤�ɂ��Ă���B
	}
}

// Function: ChangeKnotVecRange
// (private)�Ȑ�/�Ȗʃp�����[�^�̒�`���ύX����
// 
// Parameters:
// T - �ύX�������m�b�g�x�N�g����
// N - T�̔z��
// M - �K��
// K - �R���g���[���|�C���g�̐�
// Tst - �J�n�m�b�g�x�N�g��
// Te - �I���m�b�g�x�N�g��
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
// (private)�ŏ�2��@�ŋߎ��R���g���[���|�C���g�����߂�
// 
// Parameters:
// *P - �ʉߓ_��  
// T_ - �Ȑ��p�����[�^��  
// T - �m�b�g�x�N�g��  
// Pnum - �Ȑ��p�����[�^�̐�  
// Nnum - �m�b�g�x�N�g���̐�  
// M - �K��  
// K - �R���g���[���|�C���g�̐�   
// *Q - �Z�o���ꂽ�R���g���[���|�C���g��
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
	Matrix NT = NewMatrix(K-2,Pnum-2);		// N�̓]�u�s��NT
	TranMx(N,Pnum-2,K-2,NT);				// calc NT
	MulMxMx(NT,K-2,Pnum-2,N,Pnum-2,K-2,NTN);// calc NTN

	Coord *Q_ = NewCoord1(K-2);
	Gauss(K-2,NTN,R,Q_);

	// �R���g���[���|�C���g
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
// (private)�_�񐔂��琶������R���g���[���|�C���g�����Z�肷��i���ł��B�j
// 
// Parameters:
// PNum - �_��
//
// Return:
// �R���g���[���|�C���g�̐�
//int NURBS_Func::SetApproximationCPnum(int PNum)
int SetApproximationCPnum(int PNum)
{
	if(PNum < 5)		// ��
		return PNum;
	else if(PNum < 10)	// ��
		return PNum-1;
	else 
		return PNum/2;	// ��
}
#ifdef _DEBUG
// Function: DebugForNurbsC
// NURBS�Ȑ������f�o�b�O�v�����g
//
// Parameters:
// *nurbs - �f�o�b�O����NURBS�Ȑ�
//void NURBS_Func::DebugForNurbsC(NURBSC *nurbs)
void NURBSC::DebugForNurbsC(void)
{
	fprintf(stderr,"Cp num: %d\n",K);
	fprintf(stderr,"Rank: %d\n",M);
	fprintf(stderr,"Knot num: %d\n",N);
	fprintf(stderr,"Knot range: %lf - %lf\n",V[0], V[1]);

	// �R���g���[���|�C���g
	fprintf(stderr,"Control Point\n");
	for(int i=0;i<K;i++){
		fprintf(stderr,"#%d: (%lf,%lf,%lf)\t",i+1,cp[i].x,cp[i].y,cp[i].z);
	}
	fprintf(stderr,"\n");

	// �m�b�g�V�[�P���X
	fprintf(stderr,"Knot Vector\t");
	for(int i=0;i<K+M;i++){
		fprintf(stderr,"#%d: %lf\t",i+1,T[i]);
	}
	fprintf(stderr,"\n");

	// �E�F�C�g
	fprintf(stderr,"Weight\n");
	for(int i=0;i<K;i++){
		fprintf(stderr,"#%d: %lf\t",i+1,W[i]);
	}
}

// Function: DebugForNurbsS
// NURBS�Ȗʏ����f�o�b�O�v�����g
//
// Parameters:
// *nurbs - �f�o�b�O����NURBS�Ȗ�
//void NURBS_Func::DebugForNurbsS(NURBSS *nurbs)
void NURBSS::DebugForNurbsS(void)
{
	fprintf(stderr,"Cp num: %d-%d\n",K[0],K[1]);
	fprintf(stderr,"Rank: %d-%d\n",M[0],M[1]);
	fprintf(stderr,"Knot num: %d-%d\n",N[0],N[1]);
	fprintf(stderr,"Knot range: (%lf - %lf),(%lf - %lf)\n",U[0],U[1],V[0],V[1]);

	// �R���g���[���|�C���g
	fprintf(stderr,"Control Point\n");
	for(int i=0;i<K[0];i++){
		for(int j=0;j<K[1];j++){
			fprintf(stderr,"#(%d-%d): (%lf,%lf,%lf)\t",i+1,j+1,cp[i][j].x,cp[i][j].y,cp[i][j].z);
		}
	}
	fprintf(stderr,"\n");

	// U�����m�b�g�V�[�P���X
	fprintf(stderr,"U Knot Vector\t");
	for(int i=0;i<K[0]+M[0];i++){
		fprintf(stderr,"#%d: %lf\t",i+1,S[i]);
	}
	fprintf(stderr,"\n");

	// V�����m�b�g�V�[�P���X
	fprintf(stderr,"V Knot Vector\t");
	for(int i=0;i<K[1]+M[1];i++){
		fprintf(stderr,"#%d: %lf\t",i+1,T[i]);
	}
	fprintf(stderr,"\n");

	// �E�F�C�g
	//fprintf(stderr,"Weight\n");
	//for(int i=0;i<nurbs->K[0];i++){
	//	for(int j=0;j<nurbs->K[1];j++){
	//		fprintf(stderr,"#(%d-%d): %lf\t",i+1,j+1,nurbs->W[i][j]);
	//	}
	//}
}
#endif
// Function: CalcNurbsCLength
// NURBS�Ȑ�C(t)�̎w��p�����[�^���[a,b]�̐�����L�����߂�
//
// L = S|C'(t)|dt	(S�͐ϕ��L��)
//
// �ϕ��͐��l�ϕ�(�K�E�X-���W�����h����80���_)��p����
//
// Parameters:
// *Nurb - �ΏۂƂȂ�NURBS�Ȑ�
// a, b - �w��p�����[�^���[a,b]
//
// Return:
// ������
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
// NURBS�Ȑ�C(t)�̑S��Ԃ̐�����L�����߂�
//
// L = S|C'(t)|dt	(S�͐ϕ��L��)
//
// �ϕ��͐��l�ϕ�(�K�E�X-���W�����h����80���_)��p����
//
// Parameters:
// *Nurb - �ΏۂƂȂ�NURBS�Ȑ�
//
// Return:
// ������
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
// (private)�ŏ������������W�l��Ԃ�
//
// Parameters:
// a - �ΏۂƂ���1�_
// *b - �T������_�Q
// n - �_�Q�̐�
//
// Return:
// �ŏ������ƂȂ�_b_min
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
// NURBS�Ȑ����w�肵���ʒu�i�[����̌ʒ��j�ŕ�������
//
// Parameters:
// *C0 - ��������NURBS�Ȑ��ւ̃|�C���^        
// *C1 - �������ꂽNURBS�Ȑ��ւ̃|�C���^
// *C2 - �������ꂽNURBS�Ȑ��ւ̃|�C���^     
// L - �[����̌ʒ�
//
// Return:
// �����FKOD_TRUE, ���s�FKOD_FALSE 
int NURBS_Func::DivNurbsC(NURBSC *C0, NURBSC *C1, NURBSC *C2, double L)
{
//	double dLEN = CalcNurbsCLength(C0);					// NURBS�Ȑ��̐������𓾂�
	double dLEN = C0->CalcNurbsCLength();					// NURBS�Ȑ��̐������𓾂�
	double t_init = (C0->V[1] - C0->V[0])*L/dLEN;		// t�̏����l���Z�b�g
//	double t = CalcParamLengthOnNurbsC(C0,L,t_init);	// �����_�p�����[�^�l�擾
	double t = C0->CalcParamLengthOnNurbsC(L,t_init);	// �����_�p�����[�^�l�擾

	int iKOD = DivNurbsCParam(C0,C1,C2,t);		// ����

	return iKOD;

}

// Function: DivNurbsCParam
// NURBS�Ȑ����w�肵���p�����[�^�l�ŕ�������
//
// Parameters:
// *C0 - ��������NURBS�Ȑ��ւ̃|�C���^        
// *C1 - �������ꂽNURBS�Ȑ��ւ̃|�C���^
// *C2 - �������ꂽNURBS�Ȑ��ւ̃|�C���^    
// t - �����ʒu��\���Ȑ��p�����[�^
//
// Return:
// �����FKOD_TRUE, ���s�FKOD_FALSE 
int NURBS_Func::DivNurbsCParam(NURBSC *C0, NURBSC *C1, NURBSC *C2, double t)
{
	// t�p�����[�^���K���͈͂�
	if(t <= C0->T[0] || t >= C0->T[C0->N-1]){
		GuiIF.SetMessage("NURBS_Func ERROR: Wrong Curve Parameter is set.");
		return KOD_ERR;
	}

	int deg = C0->M - 1;		// ���d�x

	// �����̉�����
	// �����p�Ȑ�C0_����������
	NURBSC C0_;
	C0_.K = C0->K + deg;
	C0_.N = C0->M + C0_.K;
	New_NurbsC(&C0_,C0_.K,C0_.N);

	// C0�̃m�b�g�x�N�g����t�Ɠ����l������ꍇ�́C���d�x��1���Ƃ�
	for(int i=0;i<C0->N;i++){
		if(t == C0->T[i])	deg--;
	}

	// �����ʒu�p�����[�^t��C0_�ɑ}������
	int k = InsertNewKnotOnNurbsC(C0,&C0_,t,deg);

	// 2�{�̕����Ȑ��𐶐�
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

	// �m�b�g�x�N�g���C�R���g���[���|�C���g�C�E�F�C�g��C1,C2�ɕ��z
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

	// �m�b�g�͈̔͂�0-1�ɕύX
	ChangeKnotVecRange(T1,N1,C0->M,K1,0,1);
	ChangeKnotVecRange(T2,N2,C0->M,K2,0,1);

	// C1,C2����
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
// 2�{��NURBS�Ȑ���A������
//
// Parameter:
// *C1 - �Ȑ�1
// *C2 - �Ȑ�2
// *C_ - �A����̋Ȑ����i�[
//
// Return:
// �����FKOD_TRUE,  ���s�FKOD_FALSE
int NURBS_Func::ConnectNurbsC(NURBSC *C1,NURBSC *C2,NURBSC *C_)
{
	int flag = -1;		// �A���ʒu���ʗp�t���O

	// 2�Ȑ��̘A���ʒu�𒲂ׁC�A���_��C1(1), C2(0)�ƂȂ�悤�ǂ��炩�̋Ȑ��𒲐�����
	if(DiffCoord(C1->cp[0],C2->cp[0]) == KOD_TRUE){
//		ReverseNurbsC(C1);				// C1�̌����𔽓]����
		C1->ReverseNurbsC();				// C1�̌����𔽓]����
	}
	else if(DiffCoord(C1->cp[0],C2->cp[C2->K-1]) == KOD_TRUE){
		NURBSC *C;
		C = C2;
		C2 = C1;
		C1 = C;
	}
	else if(DiffCoord(C1->cp[C1->K-1],C2->cp[0]) == KOD_TRUE){
		// ���̃P�[�X��OK�D���ɒ����K�v�Ȃ�
	}
	else if(DiffCoord(C1->cp[C1->K-1],C2->cp[C2->K-1]) == KOD_TRUE){
//		ReverseNurbsC(C2);				// C2�̌����𔽓]����
		C2->ReverseNurbsC();				// C2�̌����𔽓]����
	}
	else{
		GuiIF.SetMessage("NURBS_Func ERROR: Two Curves don't share the same coordinate value.");
		return KOD_ERR;
	}

	// 2�Ȑ��̊K��������������
	if(C1->M != C2->M){
		GuiIF.SetMessage("NURBS_Func ERROR: Two Curves don't have the same rank.");
		return KOD_ERR;
	}

	int K = C1->K + C2->K - 1;				// C_�̃R���g���[���|�C���g�̐�
	int N = C1->N + C2->N - C2->M - 1;		// C_�̃m�b�g�x�N�g���̐�

	New_NurbsC(C_,K,N);						// C_���̃������[�m��

	SetKnotVecC_ConnectC(C1,C2,C_);			// C_�̃m�b�g��`����w��

	SetCPC_ConnectC(C1,C2,C_);				// C_�̃R���g���[���|�C���g�ƃE�F�C�g���w��

	//for(int i=0;i<C_->N;i++)
	//	fprintf(stderr,"%d,%lf\n",i+1,C_->T[i]);
	//fprintf(stderr,"\n");
	//for(int i=0;i<C_->K;i++)
	//	fprintf(stderr,"%d,%lf,%lf,%lf,%lf\n",i+1,C_->cp[i].x,C_->cp[i].y,C_->cp[i].z,C_->W[i]);

	C_->M = C1->M;							// C_�̊K�����w��

	for(int i=0;i<4;i++)
		C_->prop[i] = C1->prop[i];
	C_->EntUseFlag = C1->EntUseFlag;

	return KOD_TRUE;
}

// Function: ReverseNurbsC
// NURBS�Ȑ��̃m�b�g�x�N�g�������𔽓]����
//
// Parameters:
// *C - NURBS�Ȑ� 
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
// (private)2�{�̋Ȑ����q�����Ƃ��̃m�b�g�x�N�g����ݒ肷��
// 
// Parameters:
// *C1, *Cs - �A������2��NURBS�Ȑ�
// *C_ - �A�����NURBS�Ȑ�
void NURBS_Func::SetKnotVecC_ConnectC(NURBSC *C1,NURBSC *C2,NURBSC *C_)
{
	// �R�[�h���𒲂ׂ�
	double s=0,e=NORM_KNOT_VAL,c=0;			// �J�n�C�I���C�A�����m�b�g�x�N�g��
	double l1=0,l2=0;						// �e�Ȑ��̃m�b�g�x�N�g���̃R�[�h��
	for(int i=0;i<C1->N-1;i++) {
//		l1 += CalcDistance(CalcNurbsCCoord(C1,C1->T[i+1]),CalcNurbsCCoord(C1,C1->T[i]));	// C1�̃R�[�h��
		l1 += CalcDistance(C1->CalcNurbsCCoord(C1->T[i+1]),C1->CalcNurbsCCoord(C1->T[i]));	// C1�̃R�[�h��
	}
	for(int i=0;i<C2->N-1;i++) {
//		l2 += CalcDistance(CalcNurbsCCoord(C2,C2->T[i+1]),CalcNurbsCCoord(C2,C2->T[i]));	// C2�̃R�[�h��
		l2 += CalcDistance(C2->CalcNurbsCCoord(C2->T[i+1]),C2->CalcNurbsCCoord(C2->T[i]));	// C2�̃R�[�h��
	}
	c = l1/(l1+l2);	// �����_�̃m�b�g�x�N�g���l

	// C_�̃m�b�g�x�N�g���͈͂𓾂�
	Vector T1 = NewVector(C1->N);	
	Vector T2 = NewVector(C2->N);	
	CopyVector(C1->T,C1->N,T1);		// C1�̃m�b�g�x�N�g����T1�ɃR�s�[
	CopyVector(C2->T,C2->N,T2);		// C2�̃m�b�g�x�N�g����T2�ɃR�s�[
//	CopyVector(C1->T.get(),C1->N,T1);		// C1�̃m�b�g�x�N�g����T1�ɃR�s�[
//	CopyVector(C2->T.get(),C2->N,T2);		// C2�̃m�b�g�x�N�g����T2�ɃR�s�[
	ChangeKnotVecRange(T1,C1->N,C1->M,C1->K,s,c);	// C1(T1)�̃m�b�g�x�N�g���͈̔͂�ύX
	ChangeKnotVecRange(T2,C2->N,C2->M,C2->K,c,e);	// C2(U2)�̃m�b�g�x�N�g���͈̔͂�ύX
	C_->V[0] = s;						// C_�̃m�b�g�x�N�g���͈̔�
	C_->V[1] = e;
	C_->N = C1->N + C2->N - C2->M - 1;	// C_�̃m�b�g�x�N�g����

	// C_�̃m�b�g�x�N�g���𓾂�
	for(int i=0;i<C1->K;i++)
		C_->T[i] = T1[i];
	for(int i=1;i<C2->N;i++)
		C_->T[C1->K+i-1] = T2[i];

	FreeVector(T1);
	FreeVector(T2);
}

// Function: SetCPC_ConnectC
// (private)2�{�̋Ȑ����q�����Ƃ��̃R���g���[���|�C���g�ƃE�F�C�g��ݒ肷��
// 
// Parameters:
// *C1, *C2 - �A������2��NURBS�Ȑ�
// *C_ - �A�����NURBS�Ȑ�
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
// (private)NURBS�Ȑ��ɐV���ȃm�b�g��}������
//
// Parameters:
// *C - ����NURBS�Ȑ�  
// *C_ - �}���Ώۂ�NURBS�Ȑ�     
// t - �}������m�b�g     
// deg - ���d�x
//
// Return:
// �V���ȃm�b�g�x�N�g����ɂ�����t�̑}���ʒu
int NURBS_Func::InsertNewKnotOnNurbsC(NURBSC *C,NURBSC *C_,double t,int deg)
{
	int k=0;					// t�̑}���ʒu
	int m = C->M;				// �K��
	int n = C->K;				// �R���g���[���|�C���g�̐�

	double *T_buf = NewVector(C->K+C->M+deg);	// �m�b�g�x�N�g���ꎞ�i�[�p�o�b�t�@
	Coord *cp_buf = NewCoord1(C->K+deg);			// �R���g���[���|�C���g�ꎞ�i�[�p�o�b�t�@
	double *W_buf = NewVector(C->K+deg);		// �E�F�C�g�ꎞ�i�[�p�o�b�t�@
	//double T_buf[C->K+C->M+deg];
	//Coord  cp_buf[C->K+deg];
	//double W_buf[C->K+deg];


	// C_�Ɍ���NURBS�Ȑ���T,cp,W�������l�Ƃ��đ��
	for(int i=0;i<m+n;i++)
		C_->T[i] = C->T[i];
	for(int i=0;i<n;i++)
		C_->cp[i] = SetCoord(C->cp[i]);
	for(int i=0;i<n;i++)
		C_->W[i] = C->W[i];

	// ���d�x���Ct�̑}�����J��Ԃ�
	for(int count=0;count<deg;count++){
		// �ebuf��C_��T,cp,W����
		for(int i=0;i<n+m;i++)
			T_buf[i] = C_->T[i];
		for(int i=0;i<n;i++)
			cp_buf[i] = SetCoord(C_->cp[i]);
		for(int i=0;i<n;i++)
			W_buf[i] = C_->W[i];

		// t�̑}���ʒuk�𒲂ׂ�
		k=0;
		for(int i=0;i<n+m-1;i++){
			if(t >= T_buf[i] && t < T_buf[i+1]){
				k = i;
				break;
			}
		}

		// C_�̃m�b�g�x�N�g�����X�V
		for(int i=0;i<=k;i++)
			C_->T[i] = T_buf[i];
		C_->T[k+1] = t;
		for(int i=k+2;i<=n+m;i++)
			C_->T[i] = T_buf[i-1];

		// �R���g���[���|�C���g�ƃE�F�C�g�̍X�V
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
