#ifndef _NURBS_FUNC_H_
#define _NURBS_FUNC_H_

#include "BODY.h"
#include "Describe_BODY.h"

#define INTERSECPTNUMMAX	1000			// ��_�i�[�z��
#define NEAREST_GAP			0.01			// 2�_������_�Ƃ݂Ȃ�����
#define CONVERG_GAP			0.000001		// �j���[�g���@�̎����𔻕ʂ���臒l
#define LOOPCOUNTMAX		100				// �����v�Z�񐔂̏���

// NURBS�̑�����W�߂��N���X���`
class NURBS_Func
{
public:
	Coord CalcNurbsCCoord(NURBSC *,double);						// �w�肵���m�b�gt�ł�NURBS�Ȑ��̍��W�l�����߂�
	Coord CalcNurbsSCoord(NURBSS *,double,double);				// �w�肵���m�b�gu,v�ł�NURBS�Ȗʂ̍��W�_�����߂�
	Coord CalcDiffuNurbsS(NURBSS *,double,double);				// �w�肵���m�b�gu,v�ł�NURBS�Ȑ���u����1�K�����W�������߂�
	Coord CalcDiffvNurbsS(NURBSS *,double,double);				// NURBS�Ȑ���v����1�K�����W�������߂�
	int GenNurbsC(NURBSC *,int,int,int,double [],double [],Coord []);	// 1��NURBS�Ȑ��𐶐�����
	int GenNurbsC(NURBSC *,NURBSC);								// 1��NURBS�Ȑ��𐶐�����(NURBS�Ȑ��̃R�s�[)(�I�[�o�[���[�h)
	void DelNurbsC(NURBSC *);									// NURBS�Ȑ����폜����
	int GenNurbsS(NURBSS *,NURBSS);								// 1��NURBS�Ȗʂ𐶐�����(NURBS�Ȗʂ̃R�s�[)
	void DelNurbsS(NURBSS *);									// NURBS�Ȗʂ��폜����
	int GenTrimdNurbsS(TRIMD_NURBSS *,TRIMD_NURBSS);			// �g�����ʂ𐶐�����
	int DelTrimdNurbsS(TRIMD_NURBSS *);							// �g�����ʂ��폜(�������[���)����
	double CalcBSbasis(double,double [],int,int);				// B�X�v���C�����֐����v�Z���A�v�Z���ʂ�Ԃ�
	double CalcDiffBSbasis(double,double [],int,int);			// B�X�v���C�����֐���1�K�����W�������߂�
	Coord CalcDiffNurbsC(NURBSC *,double);						// NURBS�Ȑ���1�K�����W�������߂�
	Coord CalcNormVecOnNurbsS(NURBSS *,double,double);			// NURBS�Ȗʏ��(u,v)�ɂ�����@���x�N�g�������Ƃ߂�
	int CalcIntersecCurve3(NURBSC *,Coord,Coord,double *,int);	// 3���ȉ���NURBS�Ȑ��ƕ��ʂƂ̌�_�����߂�
	int CalcIntersecPtsPlaneV3(NURBSS *,Coord,Coord,int,Coord *,int);	// 3���ȉ���NURBS�Ȗʂƕ��ʂƂ̌�_�Q��㐔�v�Z�ŋ��߂�(v�p�����[�^����)
	int CalcIntersecPtsPlaneU3(NURBSS *,Coord,Coord,int,Coord *,int);	// 3���ȉ���NURBS�Ȗʂƕ��ʂƂ̌�_�Q��㐔�v�Z�ŋ��߂�(u�p�����[�^����)
	int CalcIntersecPtsPlaneSearch3(NURBSS *,Coord,Coord,double,Coord *,int);	// 3���ȉ���NURBS�Ȗʂƕ��ʂƂ̌�_�Q������ǐՖ@�ŋ��߂�
	int GetBSplCoef3(int,int,int,double *,double **);			// 3����B�X�v���C���Ȑ��̊e�W�������߂�@(at^3 + bt^2 + ct + d�̌W��a,b,c,d��Ԃ�)
	int GetBSplCoef2(int,int,int,double *,double **);			// 2����B�X�v���C���Ȑ��̊e�W�������߂�@(at^2 + bt + c�̌W��a,b,c��Ԃ�)
	int GetBSplCoef1(int,int,int,double *,double **);			// 1����B�X�v���C���Ȑ��̊e�W�������߂�@(at + b�̌W��a,b��Ԃ�)
	void ShiftNurbsS(NURBSS *,Coord);							// NURBS�Ȗʂ̃V�t�g
	void ShiftNurbsC(NURBSC *,Coord);							// NURBS�Ȑ��̃V�t�g
	void ChRatioNurbsS(NURBSS *,Coord);							// NURBS�Ȗʂ̔{����ύX����
	void ChRatioNurbsC(NURBSC *,Coord);							// NURBS�Ȑ��̔{����ύX����
	int SetCPNurbsS(NURBSS *,NURBSS);							// �R���g���[���|�C���g��������

private:
	int GetNurbsCCoef(NURBSC *,double **,int,Coord *,double *);	// NURBS�Ȑ��̌W�������߂�(�ō�3��)
	int CalcEquation(double *,double *,int);					// 3���������܂ł𔻕ʂ��ĉ���
	void GetNurbsSCoef(int,double **,double *,Coord *,int,Coord *,double *);	// NURBS�Ȗʂɂ�����u�܂���v���Œ肵���ꍇ�ɓ�����NURBS�Ȑ�C(u) or C(v)�̕��ꕪ�q�̌W�������߂�
	void GetIntersecEquation(int,Coord *,double *,Coord,Coord,double *);		// NURBS�Ȑ��ƕ��ʂ̌�����o�p3���������𓾂�
};

#endif
