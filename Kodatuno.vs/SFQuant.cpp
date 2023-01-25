#include "stdafx.h"
#include "BODY.h"
#include "NURBS_Func.h"
#include "SFQuant.h"

#if defined(_DEBUG) && defined(_MSC_VER)
#define new DEBUG_NEW
#endif

// Function: SFQuant
// �R���X�g���N�^(������)
SFQuant::SFQuant()
{
	U = V = 0;
	n = InitCoord();
	E = F = G = 0;
	L = M = N = 0;
}

// Function: SFQuant
// �R���X�g���N�^(��{�ʂ𓾂�)
//
// Parameters:
// *S - NURBS�Ȗʂւ̃|�C���^
// u,v - (u, v)�p�����[�^
SFQuant::SFQuant(NURBSS *S,double u,double v)
{
//	NURBS_Func nf;
	U = u;
	V = v;
//	Coord du = nf.CalcDiffuNurbsS(S,u,v);			// u����1�K����
//	Coord dv = nf.CalcDiffvNurbsS(S,u,v);			// v����1�K����
//	Coord duu = nf.CalcDiffNNurbsS(S,2,0,u,v);		// u����2�K����
//	Coord dvv = nf.CalcDiffNNurbsS(S,0,2,u,v);		// v����2�K����
//	Coord duv = nf.CalcDiffNNurbsS(S,1,1,u,v);		// u,v�����e1�K����
//	n = nf.CalcNormVecOnNurbsS(S,u,v);				// �@���x�N�g��
	Coord du = S->CalcDiffuNurbsS(u,v);			// u����1�K����
	Coord dv = S->CalcDiffvNurbsS(u,v);			// v����1�K����
	Coord duu = S->CalcDiffNNurbsS(2,0,u,v);		// u����2�K����
	Coord dvv = S->CalcDiffNNurbsS(0,2,u,v);		// v����2�K����
	Coord duv = S->CalcDiffNNurbsS(1,1,u,v);		// u,v�����e1�K����
	n = S->CalcNormVecOnNurbsS(u,v);				// �@���x�N�g��
	E = CalcInnerProduct(du,du);				// ��1��{��
	F = CalcInnerProduct(du,dv);				// ��1��{��
	G = CalcInnerProduct(dv,dv);				// ��1��{��
	L = CalcInnerProduct(duu,n);				// ��2��{��
	M = CalcInnerProduct(duv,n);				// ��2��{��
	N = CalcInnerProduct(dvv,n);				// ��2��{��

}

// Function: SetSFQ
// ��{�ʂ𓾂�
//
// Parameters:
// *S - NURBS�Ȗʂւ̃|�C���^
// u,v - (u, v)�p�����[�^
int SFQuant::SetSFQ(NURBSS *S,double u,double v)
{	
//	NURBS_Func nf;
	U = u;
	V = v;
//	Coord du = nf.CalcDiffuNurbsS(S,u,v);			// u����1�K����
//	Coord dv = nf.CalcDiffvNurbsS(S,u,v);			// v����1�K����
//	Coord duu = nf.CalcDiffNNurbsS(S,2,0,u,v);		// u����2�K����
//	Coord dvv = nf.CalcDiffNNurbsS(S,0,2,u,v);		// v����2�K����
//	Coord duv = nf.CalcDiffNNurbsS(S,1,1,u,v);		// u,v�����e1�K����
//	n = nf.CalcNormVecOnNurbsS(S,u,v);				// �@���x�N�g��
	Coord du = S->CalcDiffuNurbsS(u,v);			// u����1�K����
	Coord dv = S->CalcDiffvNurbsS(u,v);			// v����1�K����
	Coord duu = S->CalcDiffNNurbsS(2,0,u,v);		// u����2�K����
	Coord dvv = S->CalcDiffNNurbsS(0,2,u,v);		// v����2�K����
	Coord duv = S->CalcDiffNNurbsS(1,1,u,v);		// u,v�����e1�K����
	n = S->CalcNormVecOnNurbsS(u,v);				// �@���x�N�g��
	E = CalcInnerProduct(du,du);				// ��1��{��
	F = CalcInnerProduct(du,dv);				// ��1��{��
	G = CalcInnerProduct(dv,dv);				// ��1��{��
	L = CalcInnerProduct(duu,n);				// ��2��{��
	M = CalcInnerProduct(duv,n);				// ��2��{��
	N = CalcInnerProduct(dvv,n);				// ��2��{��

	return KOD_TRUE;
}

// Function: CalcMeanCurvature
// NURBS�Ȗʏ��(u,v)�ɂ����镽�ϋȗ������߂�i�I�[�o�[���[�h�j
// 
// Parameters:
// q - �Ȗʂ̊�{�ʂ��Z�b�g�ɂ����\����
//
// Retrurn:
// �v�Z����
double SFQuant::CalcMeanCurvature(void)
{
	return -(G*L+E*N-2*F*M)/(E*G-F*F)/2;		// ���ϋȗ�
}

// Function: CalcGaussCurvature
// NURBS�Ȗʏ��(u,v)�ɂ�����K�E�X�ȗ������߂�i�I�[�o�[���[�h�j
//
// Parameters:
// q - �Ȗʂ̊�{�ʂ��Z�b�g�ɂ����\����
//
// Retrurn:
// �v�Z����
double SFQuant::CalcGaussCurvature(void)
{
	return (L*N-M*M)/(E*G-F*F);					// �K�E�X�ȗ�
}
