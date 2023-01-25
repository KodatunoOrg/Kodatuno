#ifndef _NURBS_FUNC_H_
#define _NURBS_FUNC_H_

#include "BODY.h"
#include "Describe_BODY.h"
#include "Quaternion.h"

#define PTNUMMAX			10000			// NURBS�̓_��̍ő吔
#define RANKMAX				9				// NURBS�̊K���̍ő�l
#define INTERSECPTNUMMAX	1000			// ��_�i�[�z��
#define NEAREST_GAP			0.01			// 2�_������_�Ƃ݂Ȃ�����
#define CONVERG_GAP			0.00001			// �j���[�g���@�̎����𔻕ʂ���臒l
#define CONVDIVNUM			10				// �����v�Z�p�̃p�����[�^������
#define TRM_BORDERDIVNUM	100				// �g�������E����ɐ�������_�̐�
#define FORWARD				1				// ����ǐՂ̕���(��)
#define INVERSE				-1				// ����ǐՂ̕���(�t)
#define OUTTER_TRIM			0				// �O���g���~���O�̈�
#define INNER_TRIM			1				// �����g���~���O�̈�


// NURBS�̑�����W�߂��N���X���`
class NURBS_Func : public BODY_Handler
{
public:
	Coord CalcNurbsCCoord(NURBSC *,double);						// �w�肵���m�b�gt�ł�NURBS�Ȑ��̍��W�l�����߂�
	void CalcNurbsCCoords(NURBSC *,int,double *,Coord *);		// �w�肵���m�b�gt�Q�ł�NURBS�Ȑ��̍��W�l�Q�����߂�
	Coord CalcNurbsSCoord(NURBSS *,double,double);				// �w�肵���m�b�gu,v�ł�NURBS�Ȗʂ̍��W�_�����߂�
	void CalcNurbsSCoords(NURBSS *,int,Coord *,Coord *);		// �w�肵���m�b�gu,v�Q�ł�NURBS�Ȗʂ̍��W�l�Q�����߂�
	int GenNurbsC(NURBSC *,int,int,int,double [],double [],Coord [],double [],int[],int);	// 1��NURBS�Ȑ��𐶐�����
	int GenNurbsC(NURBSC *,NURBSC);								// 1��NURBS�Ȑ��𐶐�����(NURBS�Ȑ��̃R�s�[)(�I�[�o�[���[�h)
	int GenNurbsS(NURBSS *,int,int,int,int,double *,double *,double **,Coord **,double,double,double,double);	// 1��NURBS�Ȗʂ𐶐�����
	int GenNurbsS(NURBSS *,NURBSS);								// 1��NURBS�Ȗʂ𐶐�����(NURBS�Ȗʂ̃R�s�[)(�I�[�o�[���[�h)
	int GenRotNurbsS(NURBSS *,NURBSC,Coord,double);				// 1��NURBS�Ȑ������鎲���ɂ���p�x������]��������]�T�[�t�F�X�𐶐�����
	int GenSweepNurbsS(NURBSS *,NURBSC,Coord,double);			// 1��NURBS�Ȑ����炠�鎲�����ɂ��鋗�������X�C�[�v�������X�C�[�v�T�[�t�F�X�𐶐�����
	int GenTrimdNurbsS(TRIMD_NURBSS *,TRIMD_NURBSS);			// �g�����ʂ𐶐�����
	int DelTrimdNurbsS(TRIMD_NURBSS *);							// �g�����ʂ��폜(�������[���)����
	void DelNurbsC(NURBSC *);									// NURBS�Ȑ����폜����
	void DelNurbsS(NURBSS *);									// NURBS�Ȗʂ��폜����
	double CalcBSbasis(double,double [],int,int,int);			// B�X�v���C�����֐����v�Z���A�v�Z���ʂ�Ԃ�
	double CalcDiffBSbasis(double,double [],int,int,int);		// B�X�v���C�����֐���1�K�����W�������߂�
	double CalcDiffBSbasisN(double,double [],int,int,int,int);	// B�X�v���C�����֐���N�K�����W�������߂�
	Coord CalcDiffNurbsC(NURBSC *,double);						// NURBS�Ȑ���1�K�����W�������߂�
	Coord CalcDiff2NurbsC(NURBSC *NurbsC,double t);				// NURBS�Ȑ���2�K�����W�������߂�
	Coord CalcDiffuNurbsS(NURBSS *,double,double);				// NURBS�Ȗʂ�u����1�K�����W�������߂�
	Coord CalcDiffvNurbsS(NURBSS *,double,double);				// NURBS�Ȗʂ�v����1�K�����W�������߂�
	Coord CalcDiffNurbsS(NURBSS *,int,int,double,double);		// NURBS�Ȗʂ̊e������C�ӊK���������Ƃ��̔����W�������߂�
	Coord CalcNormVecOnNurbsS(NURBSS *,double,double);			// NURBS�Ȗʏ��(u,v)�ɂ�����@���x�N�g�������Ƃ߂�
	int CalcuIntersecPtNurbsLine(NURBSS *,Coord ,Coord ,Coord *,Coord *);	// NURBS�Ȗʂƒ����̌�_���Z�o
	int CalcIntersecCurve3(NURBSC *,Coord,Coord,double *,int);	// 3���ȉ���NURBS�Ȑ��ƕ��ʂƂ̌�_�����߂�
	int CalcIntersecPtsPlaneV3(NURBSS *,Coord,Coord,int,Coord *,int);	// 3���ȉ���NURBS�Ȗʂƕ��ʂƂ̌�_�Q��㐔�v�Z�ŋ��߂�(v�p�����[�^����)
	int CalcIntersecPtsPlaneU3(NURBSS *,Coord,Coord,int,Coord *,int);	// 3���ȉ���NURBS�Ȗʂƕ��ʂƂ̌�_�Q��㐔�v�Z�ŋ��߂�(u�p�����[�^����)
	int CalcIntersecPtsPlaneSearch3(NURBSS *,Coord,Coord,double,int,Coord *,int);	// 3���ȉ���NURBS�Ȗʂƕ��ʂƂ̌�_�Q������ǐՖ@�ŋ��߂�
	int CalcIntersecPtsNurbsSGeom(NURBSS *,NURBSS *,int,int,Coord *,Coord *,int);		// NURBS�Ȗʓ��m�̌����̓_���􉽊w�I�ɂ��������߂�
	int CalcIntersecPtsNurbsSSearch(NURBSS *,NURBSS *,int,double,Coord *,Coord *,int);		// NURBS�Ȗʓ��m�̌��(��_�Q)������ǐՖ@�ŋ��߂�
	int GetBSplCoef3(int,int,int,double *,double **);			// 3����B�X�v���C���Ȑ��̊e�W�������߂�@(at^3 + bt^2 + ct + d�̌W��a,b,c,d��Ԃ�)
	int GetBSplCoef2(int,int,int,double *,double **);			// 2����B�X�v���C���Ȑ��̊e�W�������߂�@(at^2 + bt + c�̌W��a,b,c��Ԃ�)
	int GetBSplCoef1(int,int,int,double *,double **);			// 1����B�X�v���C���Ȑ��̊e�W�������߂�@(at + b�̌W��a,b��Ԃ�)
	void ShiftNurbsS(NURBSS *,Coord);							// NURBS�Ȗʂ̃V�t�g
	void ShiftNurbsC(NURBSC *,Coord);							// NURBS�Ȑ��̃V�t�g
	void ChRatioNurbsS(NURBSS *,Coord);							// NURBS�Ȗʂ̔{����ύX����
	void ChRatioNurbsC(NURBSC *,Coord);							// NURBS�Ȑ��̔{����ύX����
	int SetCPNurbsS(NURBSS *,NURBSS);							// �R���g���[���|�C���g��������
	int GenInterpolatedNurbsC1(NURBSC *,Coord *,int,int);		// �^����ꂽ�_����Ԃ���n�K��NURBS�Ȑ��𐶐�����
	int GenInterpolatedNurbsC2(NURBSC *,Coord *,int,int);		// �^����ꂽ�_����Ԃ���n�K��NURBS�Ȑ��𐶐�����(�����Ȑ�)
	int GenApproximationNurbsC(NURBSC *,Coord *,int,int);		// �^����ꂽ�_����ߎ�����n�K��NURBS�Ȑ��𐶐�����
	int GenPolygonalLine(NURBSC *,Coord *,int);					// �܂���𐶐�����
	int GenInterpolatedNurbsS1(NURBSS *,Coord **,int,int,int,int);	// �^����ꂽ�_����Ԃ���4�K��NURBS�Ȗʂ𐶐�����
	int GenPolygonalSurface(NURBSS *,Coord **,int,int);				// �܂�ʂ𐶐�����
	int DetermPtOnTRMSurf(TRMS *,double,double);					// ���ڒ���NURBS�Ȗʏ��1�_(u,v)���g���~���O�̈���ɂ���̂��𔻒肷��
	void RotNurbsS(NURBSS *,Coord,double);							// NURBS�Ȗʂ���]
	void RotNurbsC(NURBSC *,Coord,double);							// NURBS�Ȑ�����]
	int DetectInterfereNurbsS(NURBSS *,NURBSS *,int);				// NURBS�Ȗ�(�g������)���m�̊����o
	int DetectInterfereTrmS(TRIMD_NURBSS *,TRIMD_NURBSS *,int);		// NURBS�Ȗ�(�g�����L)���m�̊����o
	int CalcIntersecPtsPlaneGeom(NURBSS *,Coord,Coord,int,int,Coord *,int);			// NURBS�Ȗʂƕ��ʂƂ̌�_�Q���􉽊w�I�ɋ��߂�
	double CalcNurbsCLength(NURBSC *);							// NURBS�Ȑ��̐����������߂�
	double CalcNurbsCLength(NURBSC *,double,double);			// NURBS�Ȑ��̎w���Ԃ̐����������߂�
	int CalcDeltaPtsOnNurbsC(NURBSC *,double,Coord *);			// �w�肵���Ԋu��NURBS�Ȑ���̍��W�l���o�͂���
	int CalcExtSearchCurve(NURBSS *,Coord,Coord,double,NURBSC *,NURBSC *);			// �ɒn�T�����𓾂�
	int CalcExtGradCurve(NURBSS *,Coord,Coord,double,NURBSC *,NURBSC *);			// �ɒn�X�ΐ��𓾂�


private:
	int GetNurbsCCoef(NURBSC *,double **,int,Coord *,double *);	// NURBS�Ȑ��̌W�������߂�(�ō�3��)
	int CalcEquation(double *,double *,int);					// 3���������܂ł𔻕ʂ��ĉ���
	void GetNurbsSCoef(int,double **,double *,Coord *,int,Coord *,double *);	// NURBS�Ȗʂɂ�����u�܂���v���Œ肵���ꍇ�ɓ�����NURBS�Ȑ�C(u) or C(v)�̕��ꕪ�q�̌W�������߂�
	void GetIntersecEquation(int,Coord *,double *,Coord,Coord,double *);		// NURBS�Ȑ��ƕ��ʂ̌�����o�p3���������𓾂�
	int SearchIntersectPt(NURBSS *,Coord,Coord,double,double *,double *,int);	// �j���[�g���@�ɂ���_������������(NURBS�Ȗʂƕ���)
	int SearchIntersectPt_RKM(NURBSS *,Coord,Coord,double,double *,double *,int);	// 4���̃����Q�N�b�^�@�ɂ���_������������(NURBS�Ȗʂƕ���)
	int SearchIntersectPt(NURBSS *,NURBSS *,double,double *,double *,double *,double *,int);		// �j���[�g���@�ɂ���_������������(NURBS�Ȗʓ��m)
	int DetermPtOnTRMSurf_sub(CONPS *,double,double);						// �g�������E���������Ȑ��̏ꍇ�̃g���~���O�̈���O����
	void GetKnot(Vector,int,int,int,Vector);						// �Ȑ��p�����[�^����m�b�g�x�N�g���𓾂�
	void GetKnotParam1(Coord *,int,Vector);							// �e�ʉߓ_�̋Ȑ��p�����[�^���Z�o(�R�[�h���̔䂩��Z�o)
	void GetKnotParam2(Coord *,int,Vector);							// �e�ʉߓ_�̋Ȑ��p�����[�^���Z�o(�R�[�h���̕������̔䂩��Z�o)
	int CheckTheSamePoints(Coord *,int);							// ����_����������
	double CalcDiffNurbsSDenom(NURBSS *,int,int,double,double);		// NURBS�Ȗʕ���̊e������C�ӊK���������Ƃ��̔����W�������߂�
	Coord CalcDiffNurbsSNumer(NURBSS *,int,int,double,double);		// NURBS�Ȗʕ��q�̊e������C�ӊK���������Ƃ��̔����W�������߂�

};

#endif
