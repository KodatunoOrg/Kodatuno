#ifndef _NURBS_FUNC_H_
#define _NURBS_FUNC_H_

#include "BODY.h"
#include "Quaternion.h"

#define PTNUMMAX			10000			// NURBS�̓_��̍ő吔
#define RANKMAX				9				// NURBS�̊K���̍ő�l
#define INTERSECPTNUMMAX	1000			// ��_�i�[�z��
#define NEAREST_GAP			0.01			// 2�_������_�Ƃ݂Ȃ�����
#define CONVERG_GAP			0.00001			// �j���[�g���@�̎����𔻕ʂ���臒l
#define CONVDIVNUM			100				// �����v�Z�p�̃p�����[�^������
#define TRM_BORDERDIVNUM	100				// �g�������E����ɐ�������_�̐�
#define FORWARD				1				// ����ǐՂ̕���(��)
#define INVERSE				-1				// ����ǐՂ̕���(�t)
#define PARAM_U				0				// u������\���V���{��
#define PARAM_V				1				// v������\���V���{��
#define OUTTER_TRIM			0				// �O���g���~���O�̈�
#define INNER_TRIM			1				// �����g���~���O�̈�
#define PARAMDIVNUM			10				// �����l�T���p�̃p�����[�^������


// NURBS�̑�����W�߂��N���X���`
class NURBS_Func
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
	int GenIsoparamCurveU(NURBSS *,double,NURBSC *);			// NURBS�Ȗʏ��u�����p�����[�^�l���Œ肵���Ƃ��̃A�C�\�p�����g���b�NNURBS�Ȑ��𐶐�
	int GenIsoparamCurveV(NURBSS *,double,NURBSC *);			// NURBS�Ȗʏ��v�����p�����[�^�l���Œ肵���Ƃ��̃A�C�\�p�����g���b�NNURBS�Ȑ��𐶐�
	int GenTrimdNurbsS(TRIMD_NURBSS *,TRIMD_NURBSS);			// �g�����ʂ𐶐�����
	int DelTrimdNurbsS(TRIMD_NURBSS *);							// �g�����ʂ��폜(�������[���)����
	void DelNurbsC(NURBSC *);									// NURBS�Ȑ����폜����
	void DelNurbsS(NURBSS *);									// NURBS�Ȗʂ��폜����
	double CalcBSbasis(double,double [],int,int,int);			// B�X�v���C�����֐����v�Z���A�v�Z���ʂ�Ԃ�
	double CalcDiffBSbasis(double,double [],int,int,int);		// B�X�v���C�����֐���1�K�����W�������߂�
	double CalcDiffBSbasisN(double,double [],int,int,int,int);	// B�X�v���C�����֐���N�K�����W�������߂�
	Coord CalcDiffNurbsC(NURBSC *,double);						// NURBS�Ȑ���1�K�����W�������߂�
	Coord CalcDiff2NurbsC(NURBSC *,double);						// NURBS�Ȑ���2�K�����W�������߂�
	Coord CalcDiffNNurbsC(NURBSC *,int,double);					// NURBS�Ȑ���r�K�����W�������߂�
	Coord CalcDiffuNurbsS(NURBSS *,double,double);				// NURBS�Ȗʂ�u����1�K�����W�������߂�
	Coord CalcDiffvNurbsS(NURBSS *,double,double);				// NURBS�Ȗʂ�v����1�K�����W�������߂�
	Coord CalcDiffNNurbsS(NURBSS *,int,int,double,double);		// NURBS�Ȗʂ̊e������C�ӊK���������Ƃ��̔����W�������߂�
	Coord CalcNormVecOnNurbsS(NURBSS *,double,double);			// NURBS�Ȗʏ��(u,v)�ɂ�����@���x�N�g�������Ƃ߂�
	Coord CalcDiffuNormVecOnNurbsS(NURBSS *,double,double);		// NURBS�Ȗʏ��(u,v)�ɂ�����@���x�N�g����u����1�K���������Ƃ߂�
	Coord CalcDiffvNormVecOnNurbsS(NURBSS *,double,double);		// NURBS�Ȗʏ��(u,v)�ɂ�����@���x�N�g����v����1�K���������Ƃ߂�
	double CalcMeanCurvature(NURBSS *,double,double);			// NURBS�Ȗʏ��(u,v)�ɂ����镽�ϋȗ������߂�
	Coord CalcMeanCurvatureNormVec(NURBSS *,double,double);		// NURBS�Ȗʏ��(u,v)�ɂ����镽�ϋȗ��@���x�N�g�������߂�
	double CalcGaussCurvature(NURBSS *,double,double);			// NURBS�Ȗʏ��(u,v)�ɂ�����K�E�X�ȗ������߂�
	Coord CalcGaussCurvatureNormVec(NURBSS *,double,double);	// NURBS�Ȗʏ��(u,v)�ɂ�����K�E�X�ȗ��@���x�N�g�������߂�
	int CalcuIntersecPtNurbsLine(NURBSS *,Coord,Coord,int,Coord *,int);	// NURBS�Ȗʂƒ����̌�_���Z�o
	int CalcIntersecPtNurbsPt(NURBSS *,Coord,int,Coord *);		// NURBS�Ȗʏ�ɂ������ԏ��1�_����̍ŋߖT�_�����߂�(�j���[�g���@)
	int CalcIntersecCurve(NURBSC *,Coord,Coord,int,double *,int);	// NURBS�Ȑ��ƕ��ʂƂ̌�_�����߂�(�j���[�g���@)
	int CalcIntersecCurve3(NURBSC *,Coord,Coord,double *,int);	// 3���ȉ���NURBS�Ȑ��ƕ��ʂƂ̌�_�����߂�
	int CalcIntersecPtsPlaneV3(NURBSS *,Coord,Coord,int,Coord *,int);	// V�����̃A�C�\�p���Ȑ����w�肵���������Ő������C�e3���ȉ��̋Ȑ���NURBS�ȖʂƂ̌�_��㐔�v�Z�ŎZ�o����
	int CalcIntersecPtsPlaneU3(NURBSS *,Coord,Coord,int,Coord *,int);	// V�����̃A�C�\�p���Ȑ����w�肵���������Ő������C�e3���ȉ��̋Ȑ���NURBS�ȖʂƂ̌�_��㐔�v�Z�ŎZ�o����
	int CalcIntersecPtsPlaneV(NURBSS *,Coord,Coord,int,Coord *,int);	// V�����̃A�C�\�p���Ȑ����w�肵���������Ő������C�e�Ȑ���NURBS�ȖʂƂ̌�_���Z�o����
	int CalcIntersecPtsPlaneU(NURBSS *,Coord,Coord,int,Coord *,int);	// U�����̃A�C�\�p���Ȑ����w�肵���������Ő������C�e�Ȑ���NURBS�ȖʂƂ̌�_���Z�o����
	int CalcIntersecPtsPlaneSearch(NURBSS *,Coord,Coord,double,int,Coord *,int);	// NURBS�Ȗʂƕ��ʂƂ̌�_�Q������ǐՖ@�ŋ��߂�
	int CalcIntersecPtsNurbsSNurbsC(NURBSS *,NURBSC *,int,Coord *,int);	// NURBS�Ȗʂ�NURBS�Ȑ��Ƃ̌�_�����߂�(�j���[�g���@)
	int CalcIntersecPtsNurbsSGeom(NURBSS *,NURBSS *,int,int,Coord *,Coord *,int);		// NURBS�Ȗʓ��m�̌����̓_���􉽊w�I�ɂ��������߂�
	int CalcIntersecPtsNurbsSSearch(NURBSS *,NURBSS *,int,double,Coord *,Coord *,int);	// NURBS�Ȗʓ��m�̌��(��_�Q)������ǐՖ@�ŋ��߂�
	int CalcIntersecPtsNurbsCNurbsCParam(NURBSC *,NURBSC *,int,Coord *,int);	// 2D�p�����g���b�NNURBS�Ȑ����m�̌�_�����߂�
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
	int GenNurbsCfromCP(NURBSC *,Coord *,int,int);				// �R���g���[���|�C���g����NURBS�Ȑ��𐶐�����
	int GenPolygonalLine(NURBSC *,Coord *,int);					// �܂���𐶐�����
	int GenInterpolatedNurbsS1(NURBSS *,Coord **,int,int,int,int);	// �^����ꂽ�_����Ԃ���n�KNURBS�Ȗʂ𐶐�����
	int GenPolygonalSurface(NURBSS *,Coord **,int,int);				// �܂�ʂ𐶐�����
	int GenApproximationNurbsS(NURBSS *,Coord **,int,int,int,int);	// �^����ꂽ�_����ߎ�����n�K��NURBS�Ȗʂ𐶐�����
	int GenNurbsSfromCP(NURBSS *,Coord **,int,int,int,int);			// �^����ꂽ�R���g���[���|�C���g����n�K��NURBS�Ȗʂ𐶐�����
	int DetermPtOnTRMSurf(TRMS *,double,double);					// ���ڒ���NURBS�Ȗʏ��1�_(u,v)���g���~���O�̈���ɂ���̂��𔻒肷��
	void RotNurbsS(NURBSS *,Coord,double);							// NURBS�Ȗʂ���]
	void RotNurbsC(NURBSC *,Coord,double);							// NURBS�Ȑ�����]
	int DetectInterfereNurbsS(NURBSS *,NURBSS *,int);				// NURBS�Ȗ�(�g������)���m�̊����o
	int DetectInterfereTrmS(TRIMD_NURBSS *,TRIMD_NURBSS *,int);		// NURBS�Ȗ�(�g�����L)���m�̊����o
	int CalcIntersecPtsPlaneGeom(NURBSS *,Coord,Coord,int,int,Coord *,int);			// NURBS�Ȗʂƕ��ʂƂ̌�_�Q���􉽊w�I�ɋ��߂�
	double CalcNurbsCLength(NURBSC *);							// NURBS�Ȑ��̐����������߂�
	double CalcNurbsCLength(NURBSC *,double,double);			// NURBS�Ȑ��̎w���Ԃ̐����������߂�
	int CalcDeltaPtsOnNurbsC(NURBSC *,double,Coord *);			// �w�肵���Ԋu��NURBS�Ȑ���̍��W�l���o�͂���
	int CalcExtremumNurbsC(NURBSC *,Coord,double *,int);		// NURBS�Ȑ��̎w�肵�������ɂ�����ɒl�̍��W�l�𓾂�
	void GetEqIntervalKont(int,int,Vector);						// �Ȑ�/�Ȗʃp�����[�^���瓙�Ԋu�ȃm�b�g�x�N�g�����Z�o

	int CalcExtSearchCurve(NURBSS *,Coord,Coord,double,NURBSC *,NURBSC *);			// �ɒn�T�����𓾂�(������)
	int CalcExtGradCurve(NURBSS *,Coord,Coord,double,NURBSC *,NURBSC *);			// �ɒn�X�ΐ��𓾂�(������)
	int TrimNurbsSPlane(TRMS *,Coord,Coord);					// NURBS�Ȗʂ𕽖ʂŃg��������(������)

	int New_NurbsC(NURBSC *,int,int);			// NURBS�Ȑ��̃������[�m��
	void Free_NurbsC_1DArray(NURBSC *,int);		// NURBS�Ȑ��z��̃������[���
	void Free_NurbsC(NURBSC *);					// NURBS�Ȑ��̃������[���
	int New_NurbsS(NURBSS *,int [],int []);		// NURBS�Ȗʂ̃������[�m��
	void Free_NurbsS_1DArray(NURBSS *,int);		// NURBS�Ȗʔz��̃������[���
	void Free_NurbsS(NURBSS *);					// NURBS�Ȗʂ̃������[���
	int New_TrmS(TRMS *,int);					// �g�����ʂ̃������[�m��
	void Free_TrmS_1DArray(TRMS *,int);			// �g�����ʔz��̃������[���
	void Free_TrmS(TRMS *);						// �g�����ʂ̃������[���
	int New_CompC(COMPC *,int);					// �����Ȑ��̃������[�m��
	void Free_CompC_1DArray(COMPC *,int);		// �����Ȑ��z��̃������[���
	void Free_CompC(COMPC *);					// �����Ȑ��̃������[���


private:
	int GetNurbsCCoef(NURBSC *,double **,int,Coord *,double *);	// NURBS�Ȑ��̌W�������߂�(�ō�3��)
	int CalcEquation(double *,double *,int);					// 3���������܂ł𔻕ʂ��ĉ���
	void GetNurbsSCoef(int,double **,double *,Coord *,int,Coord *,double *);	// NURBS�Ȗʂɂ�����u�܂���v���Œ肵���ꍇ�ɓ�����NURBS�Ȑ�C(u) or C(v)�̕��ꕪ�q�̌W�������߂�
	void GetIntersecEquation(int,Coord *,double *,Coord,Coord,double *);		// NURBS�Ȑ��ƕ��ʂ̌�����o�p3���������𓾂�
	int SearchIntersectPt(NURBSS *,Coord,Coord,double,double *,double *,int);	// �j���[�g���@�ɂ���_������������(NURBS�Ȗʂƕ���)
	int SearchIntersectPt_RKM(NURBSS *,Coord,Coord,double,double *,double *,int);	// 4���̃����Q�N�b�^�@�ɂ���_������������(NURBS�Ȗʂƕ���)
	int SearchIntersectPt_BS(NURBSS *,Coord,Coord,double,double *,double *,int);	// Bulirsch-Stoer�@�ɂ���_������������(NURBS�Ȗʂƕ���)
	int GetSIPParam1(NURBSS *,double ,double ,Coord ,Coord ,int ,Coord *);		// NURBS�Ȗʂƕ��ʂ̌�_��\�������������̉E�ӂ̒l�𓾂�
	int SearchIntersectPt(NURBSS *,NURBSS *,double,double *,double *,double *,double *,int);		// �j���[�g���@�ɂ���_������������(NURBS�Ȗʓ��m)
	int DetermPtOnTRMSurf_sub(CONPS *,double,double);				// �g�������E���������Ȑ��̏ꍇ�̃g���~���O�̈���O����
	void GetCurveKnotParam1(Coord *,int,Vector);					// �e�ʉߓ_�̋Ȑ��p�����[�^���Z�o(�R�[�h���̔䂩��Z�o)
	void GetCurveKnotParam2(Coord *,int,Vector);					// �e�ʉߓ_�̋Ȑ��p�����[�^���Z�o(�R�[�h���̕������̔䂩��Z�o)
	void GetSurfaceKnotParam(Vector,Vector,Coord **,int,int);		// �e�ʉߓ_�̋Ȗʃp�����[�^���Z�o
	void GetInterpolatedKnot(Vector,int,int,int,Vector);			// �Ȑ�/�Ȗʃp�����[�^�����ԗp�m�b�g�x�N�g�����Z�o
	void GetApproximatedKnot(Vector,int,int,int,Vector);			// �Ȑ�/�Ȗʃp�����[�^����ߎ��p�m�b�g�x�N�g�����Z�o
	int SetApproximationCPnum(int);									// �_�񐔂��琶������R���g���[���|�C���g�����Z�肷��
	void CalcApproximationCP_LSM(Coord *,Vector,Vector,int,int,int,int,Coord *);	// �ŏ�2��@�ŋߎ��R���g���[���|�C���g�����߂�
	int CheckTheSamePoints(Coord *,int);							// ����_����������
	int CheckTheSamePoints(double *,int);							// ����_����������
	int CheckTheSamePoints2D(Coord *,int);							// 2D���ʓ��̓���_����������
	double CalcDiffNurbsSDenom(NURBSS *,int,int,double,double);		// NURBS�Ȗʕ���̊e������C�ӊK���������Ƃ��̔����W�������߂�
	Coord CalcDiffNurbsSNumer(NURBSS *,int,int,double,double);		// NURBS�Ȗʕ��q�̊e������C�ӊK���������Ƃ��̔����W�������߂�
	Coord TrimNurbsSPlaneSub1(double,double,double,double,double,double); // TrimNurbsSPlane�̃T�u�֐�(2�����̌�_�����Ƃ߂�)

};

#endif
