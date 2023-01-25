#include "NURBS_Func.h"
#include "SFQuant.h"

// �R���X�g���N�^
SFQuant::SFQuant()
{
	U = V = 0;
	n = InitCoord();
	E = F = G = 0;
	L = M = N = 0;
}

// �R���X�g���N�^(��{�ʂ𓾂�)
SFQuant::SFQuant(NURBSS *S,double u,double v)
{
	NURBS_Func nf;
	U = u;
	V = v;
	Coord du = nf.CalcDiffuNurbsS(S,u,v);			// u����1�K����
	Coord dv = nf.CalcDiffvNurbsS(S,u,v);			// v����1�K����
	Coord duu = nf.CalcDiffNNurbsS(S,2,0,u,v);		// u����2�K����
	Coord dvv = nf.CalcDiffNNurbsS(S,0,2,u,v);		// v����2�K����
	Coord duv = nf.CalcDiffNNurbsS(S,1,1,u,v);		// u,v�����e1�K����
	n = nf.CalcNormVecOnNurbsS(S,u,v);				// �@���x�N�g��
	E = CalcInnerProduct(du,du);				// ��1��{��
	F = CalcInnerProduct(du,dv);				// ��1��{��
	G = CalcInnerProduct(dv,dv);				// ��1��{��
	L = CalcInnerProduct(duu,n);				// ��2��{��
	M = CalcInnerProduct(duv,n);				// ��2��{��
	N = CalcInnerProduct(dvv,n);				// ��2��{��

}

// ��{�ʂ𓾂�
int SFQuant::SetSFQ(NURBSS *S,double u,double v)
{	
	NURBS_Func nf;
	U = u;
	V = v;
	Coord du = nf.CalcDiffuNurbsS(S,u,v);			// u����1�K����
	Coord dv = nf.CalcDiffvNurbsS(S,u,v);			// v����1�K����
	Coord duu = nf.CalcDiffNNurbsS(S,2,0,u,v);		// u����2�K����
	Coord dvv = nf.CalcDiffNNurbsS(S,0,2,u,v);		// v����2�K����
	Coord duv = nf.CalcDiffNNurbsS(S,1,1,u,v);		// u,v�����e1�K����
	n = nf.CalcNormVecOnNurbsS(S,u,v);				// �@���x�N�g��
	E = CalcInnerProduct(du,du);				// ��1��{��
	F = CalcInnerProduct(du,dv);				// ��1��{��
	G = CalcInnerProduct(dv,dv);				// ��1��{��
	L = CalcInnerProduct(duu,n);				// ��2��{��
	M = CalcInnerProduct(duv,n);				// ��2��{��
	N = CalcInnerProduct(dvv,n);				// ��2��{��

	return KOD_TRUE;
}