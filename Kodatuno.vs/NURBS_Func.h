#ifndef _NURBS_FUNC_H_
#define _NURBS_FUNC_H_

#include "BODY.h"
//#include "Quaternion.h"
#include "SFQuant.h"

// Constants: General Defines
// PTNUMMAX -			NURBS�̓_��̍ő吔(10000)
// RANKMAX -			NURBS�̊K���̍ő�l(9)
// INTERSECPTNUMMAX -	��_�i�[�z��(1000)
// NEAREST_GAP -		2�_������_�Ƃ݂Ȃ�����(0.01)
// CONVERG_GAP -		�j���[�g���@�̎����𔻕ʂ���臒l(0.00001)
// CONVDIVNUM -			�����v�Z�p�̃p�����[�^������(100)
// TRM_BORDERDIVNUM -	�g�������E����ɐ�������_�̐�(100)
// FORWARD -			����ǐՂ̕���(��)(1)
// INVERSE -			����ǐՂ̕���(�t)(-1)
// PARAM_U -			u������\���V���{��(0)
// PARAM_V -			v������\���V���{��(1)
// OUTTER_TRIM -		�O���g���~���O�̈�(0)
// INNER_TRIM -			�����g���~���O�̈�(1)
// PARAMDIVNUM -		�����l�T���p�̃p�����[�^������(10)
// RUNGE_KUTTA -		Runge-Kutta�@�̃V���{��(0)
// BULIRSH_STOER -		Bulirsch-Stoer�@�̃V���{��(1)
// CALC_OFFSET -		�I�t�Z�b�g�Ȗʌv�Z�̃V���{��(2)
// BS_DIV -				Bulirsch-Stoer�@�̍��ݐ�(11)
#define PTNUMMAX			10000
#define RANKMAX				9
#define INTERSECPTNUMMAX	1000
#define NEAREST_GAP			0.01
#define CONVERG_GAP			0.00001
#define CONVDIVNUM			100
#define TRM_BORDERDIVNUM	100
#define FORWARD				1
#define INVERSE				-1
#define PARAM_U				0
#define PARAM_V				1
#define OUTTER_TRIM			0
#define INNER_TRIM			1
#define PARAMDIVNUM			10
#define RUNGE_KUTTA			0
#define BULIRSH_STOER		1
#define CALC_OFFSET			2
#define BS_DIV				11


//////////////////////////////////////////////////////////////////////////////////////
// Class: NURBS_Func
// NURBS�Ȑ�/�Ȗʂ̑�����W�߂��N���X
class NURBS_Func
{
public:
	// Function: CalcNurbsCCoord
	// �w�肵��t�ł�NURBS�Ȑ��̍��W�l�����߂�
//	Coord CalcNurbsCCoord(NURBSC *,double);

	// Function: CalcNurbsCCoords
	// �w�肵��t�Q�ł�NURBS�Ȑ��̍��W�l�Q�����߂�
//	void CalcNurbsCCoords(NURBSC *,int,double *,Coord *);		

	// Function: CalcNurbsSCoord
	// �w�肵��u,v�ł�NURBS�Ȗʂ̍��W�_�����߂�
//	Coord CalcNurbsSCoord(NURBSS *,double,double);				

	// Function: CalcNurbsSCoords
	// �w�肵��u,v�Q�ł�NURBS�Ȗʂ̍��W�l�Q�����߂�
//	void CalcNurbsSCoords(NURBSS *,int,Coord *,Coord *);		

	// Function: GenNurbsC
	// 1��NURBS�Ȑ��𐶐�����
	int GenNurbsC(NURBSC *,int,int,int,double [],double [],Coord [],double [],int[],int);	

	// Function: GenNurbsC
	// 1��NURBS�Ȑ��𐶐�����(NURBS�Ȑ��̃R�s�[)(�I�[�o�[���[�h)
//	int GenNurbsC(NURBSC *,NURBSC);								
	int GenNurbsC(NURBSC *,NURBSC*);	// union�����o�ύX�̉e��

	// Function: GenNurbsS
	// 1��NURBS�Ȗʂ𐶐�����
	int GenNurbsS(NURBSS *,int,int,int,int,double *,double *,double **,Coord **,double,double,double,double);	

	// Function: GenNurbsS
	// 1��NURBS�Ȗʂ𐶐�����(NURBS�Ȗʂ̃R�s�[)(�I�[�o�[���[�h)
	int GenNurbsS(NURBSS *,NURBSS);								

	// Function: GenRotNurbsS
	// 1��NURBS�Ȑ������鎲���ɂ���p�x������]��������]�T�[�t�F�X�𐶐�����
//	int GenRotNurbsS(NURBSS *,NURBSC,Coord,double);				
	int GenRotNurbsS(NURBSS *,const NURBSC&,Coord,double);				

	// Function: GenSweepNurbsS
	// 1��NURBS�Ȑ����炠�鎲�����ɂ��鋗�������X�C�[�v�������X�C�[�v�T�[�t�F�X�𐶐�����
//	int GenSweepNurbsS(NURBSS *,NURBSC,Coord,double);			
	int GenSweepNurbsS(NURBSS *,const NURBSC&,Coord,double);			

	// Function: GenIsoparamCurveU
	// NURBS�Ȗʏ��u�����p�����[�^�l���Œ肵���Ƃ��̃A�C�\�p�����g���b�NNURBS�Ȑ��𐶐�
	int GenIsoparamCurveU(NURBSS *,double,NURBSC *);			

	// Function: GenIsoparamCurveV
	// NURBS�Ȗʏ��v�����p�����[�^�l���Œ肵���Ƃ��̃A�C�\�p�����g���b�NNURBS�Ȑ��𐶐�
	int GenIsoparamCurveV(NURBSS *,double,NURBSC *);			

	// Function: GenTrimdNurbsS
	// �g�����ʂ𐶐�����
	int GenTrimdNurbsS(TRIMD_NURBSS *,TRIMD_NURBSS);			

	// Function: DelTrimdNurbsS
	// �g�����ʂ��폜(�������[���)����
	int DelTrimdNurbsS(TRIMD_NURBSS *);							

	// Function: DelNurbsC
	// NURBS�Ȑ����폜����
	void DelNurbsC(NURBSC *);									

	// Function: DelNurbsS
	// NURBS�Ȗʂ��폜����
	void DelNurbsS(NURBSS *);									
/*	-- member function -> static function
	// Function: CalcBSbasis
	// B�X�v���C�����֐����v�Z���A�v�Z���ʂ�Ԃ�
	double CalcBSbasis(double,double [],int,int,int);			

	// Function: CalcDiffBSbasis
	// B�X�v���C�����֐���1�K�����W�������߂�
	double CalcDiffBSbasis(double,double [],int,int,int);		

	// Function: CalcDiffBSbasisN
	// B�X�v���C�����֐���N�K�����W�������߂�
	double CalcDiffBSbasisN(double,double [],int,int,int,int);	

	// Function: GetBSplCoef3
	// 3����B�X�v���C���Ȑ��̊e�W�������߂�@(at^3 + bt^2 + ct + d�̌W��a,b,c,d��Ԃ�)
	int GetBSplCoef3(int,int,int,double *,double **);			

	// Function: GetBSplCoef2
	// 2����B�X�v���C���Ȑ��̊e�W�������߂�@(at^2 + bt + c�̌W��a,b,c��Ԃ�)
	int GetBSplCoef2(int,int,int,double *,double **);			

	// Function: GetBSplCoef1
	// 1����B�X�v���C���Ȑ��̊e�W�������߂�@(at + b�̌W��a,b��Ԃ�)
	int GetBSplCoef1(int,int,int,double *,double **);			

	// Function: ChangeKnotVecRange
	// �m�b�g�x�N�g���̃p�����[�^��`���ύX����
	void ChangeKnotVecRange(Vector,int,int,int,double,double);	

	// Function: GetEqIntervalKont
	// �Ȑ�/�Ȗʃp�����[�^���瓙�Ԋu�ȃm�b�g�x�N�g�����Z�o
	void GetEqIntervalKont(int,int,Vector);						
*/
	// Function: CalcDiffNurbsC
	// NURBS�Ȑ���1�K�����W�������߂�
//	Coord CalcDiffNurbsC(NURBSC *,double);						

	// Function: CalcDiff2NurbsC
	// NURBS�Ȑ���2�K�����W�������߂�
//	Coord CalcDiff2NurbsC(NURBSC *,double);						

	// Function: CalcDiffNNurbsC
	// NURBS�Ȑ���r�K�����W�������߂�
//	Coord CalcDiffNNurbsC(NURBSC *,int,double);					

	// Function: CalcDiffuNurbsS
	// NURBS�Ȗʂ�u����1�K�����W�������߂�
//	Coord CalcDiffuNurbsS(NURBSS *,double,double);				

	// Function: CalcDiffvNurbsS
	// NURBS�Ȗʂ�v����1�K�����W�������߂�
//	Coord CalcDiffvNurbsS(NURBSS *,double,double);				

	// Function: CalcDiffNNurbsS
	// NURBS�Ȗʂ̊e������C�ӊK���������Ƃ��̔����W�������߂�
//	Coord CalcDiffNNurbsS(NURBSS *,int,int,double,double);		

	// Function: CalcNormVecOnNurbsS
	// NURBS�Ȗʏ��(u,v)�ɂ�����@���x�N�g�������Ƃ߂�
//	Coord CalcNormVecOnNurbsS(NURBSS *,double,double);			

	// Function: CalcDiffuNormVecOnNurbsS
	// NURBS�Ȗʏ��(u,v)�ɂ�����@���x�N�g����u����1�K���������Ƃ߂�
//	Coord CalcDiffuNormVecOnNurbsS(NURBSS *,double,double);		

	// Function: CalcDiffvNormVecOnNurbsS
	// NURBS�Ȗʏ��(u,v)�ɂ�����@���x�N�g����v����1�K���������Ƃ߂�
//	Coord CalcDiffvNormVecOnNurbsS(NURBSS *,double,double);		

	// Function: CalcMeanCurvature
	// NURBS�Ȗʏ��(u,v)�ɂ����镽�ϋȗ������߂�
//	double CalcMeanCurvature(NURBSS *,double,double);			

	// Function: CalcMeanCurvature
	// �I�[�o�[���[�h
//	double CalcMeanCurvature(SFQuant);							

	// Function: CalcMeanCurvatureNormVec
	// NURBS�Ȗʏ��(u,v)�ɂ����镽�ϋȗ��@���x�N�g�������߂�
//	Coord CalcMeanCurvatureNormVec(NURBSS *,double,double);		

	// Function: CalcGaussCurvature
	// NURBS�Ȗʏ��(u,v)�ɂ�����K�E�X�ȗ������߂�
//	double CalcGaussCurvature(NURBSS *,double,double);			

	// Function: CalcGaussCurvature
	// �I�[�o�[���[�h
//	double CalcGaussCurvature(SFQuant);							

	// Function: CalcGaussCurvatureNormVec
	// NURBS�Ȗʏ��(u,v)�ɂ�����K�E�X�ȗ��@���x�N�g�������߂�
//	Coord CalcGaussCurvatureNormVec(NURBSS *,double,double);	

	// Function: CalcuIntersecPtNurbsLine
	// NURBS�Ȗʂƒ����̌�_���Z�o
//	int CalcuIntersecPtNurbsLine(NURBSS *,Coord,Coord,int,Coord *,int,int);	

	// Function: CalcIntersecPtNurbsPt
	// ��ԏ��1�_����NURBS�Ȗʏ�̍ŋߖT�_�����߂�(�j���[�g���@)
//	int CalcIntersecPtNurbsPt(NURBSS *,Coord,int,int,Coord *);		

	// Function: CalcIntersecPtNurbsPt
	// ��ԏ��1�_����NURBS�Ȑ���̍ŋߖT�_�����߂�(�j���[�g���@)(�I�[�o�[���[�h)
//	int CalcIntersecPtNurbsPt(NURBSC *,Coord,int,int,double *);			

	// Function: CalcIntersecIsparaCurveU
	// u�����A�C�\�p���Ȑ��ƕ��ʂƂ̌�_�����߂�(�j���[�g���@)
//	int CalcIntersecIsparaCurveU(NURBSS *,double,Coord,Coord,int,double *,int);	

	// Function: CalcIntersecIsparaCurveV
	// v�����A�C�\�p���Ȑ��ƕ��ʂƂ̌�_�����߂�(�j���[�g���@)
//	int CalcIntersecIsparaCurveV(NURBSS *,double,Coord,Coord,int,double *,int); 

	// Function: CalcIntersecCurve
	// NURBS�Ȑ��ƕ��ʂƂ̌�_�����߂�(�j���[�g���@)
//	int CalcIntersecCurve(NURBSC *,Coord,Coord,int,double *,int,int);	

	// Function: CalcIntersecCurve3
	// 3���ȉ���NURBS�Ȑ��ƕ��ʂƂ̌�_�����߂�
//	int CalcIntersecCurve3(NURBSC *,Coord,Coord,double *,int);	

	// Function: CalcIntersecPtsPlaneV3
	// V�����̃A�C�\�p���Ȑ����w�肵���������Ő������C�e3���ȉ��̋Ȑ���NURBS�ȖʂƂ̌�_��㐔�v�Z�ŎZ�o����
//	int CalcIntersecPtsPlaneV3(NURBSS *,Coord,Coord,int,Coord *,int);	

	// Function: CalcIntersecPtsPlaneU3
	// V�����̃A�C�\�p���Ȑ����w�肵���������Ő������C�e3���ȉ��̋Ȑ���NURBS�ȖʂƂ̌�_��㐔�v�Z�ŎZ�o����
//	int CalcIntersecPtsPlaneU3(NURBSS *,Coord,Coord,int,Coord *,int);	

	// Function: CalcIntersecPtsPlaneV
	// V�����̃A�C�\�p���Ȑ����w�肵���������Ő������C�e�Ȑ���NURBS�ȖʂƂ̌�_���Z�o����
//	int CalcIntersecPtsPlaneV(NURBSS *,Coord,Coord,int,Coord *,int);	

	// Function: CalcIntersecPtsPlaneU
	// U�����̃A�C�\�p���Ȑ����w�肵���������Ő������C�e�Ȑ���NURBS�ȖʂƂ̌�_���Z�o����
//	int CalcIntersecPtsPlaneU(NURBSS *,Coord,Coord,int,Coord *,int);	

	// Function: CalcIntersecPtsPlaneSearch
	// NURBS�Ȗʂƕ��ʂƂ̌�_�Q������ǐՖ@�ŋ��߂�
//	int CalcIntersecPtsPlaneSearch(NURBSS *,Coord,Coord,double,int,Coord *,int,int);	

	// Function: CalcIntersecPtsOffsetPlaneSearch
	// �I�t�Z�b�gNURBS�Ȗʂƕ��ʂƂ̌�_�Q������ǐՖ@�ŋ��߂�(������)
//	int CalcIntersecPtsOffsetPlaneSearch(NURBSS *,double,Coord,Coord,double,int,Coord *,int);

	// Function: CalcIntersecPtsNurbsSNurbsC
	// NURBS�Ȗʂ�NURBS�Ȑ��Ƃ̌�_�����߂�(�j���[�g���@)
	int CalcIntersecPtsNurbsSNurbsC(NURBSS *,NURBSC *,int,Coord *,int);	

	// Function: CalcIntersecPtsNurbsSGeom
	// NURBS�Ȗʓ��m�̌����̓_���􉽊w�I�ɂ��������߂�
	int CalcIntersecPtsNurbsSGeom(NURBSS *,NURBSS *,int,int,Coord *,Coord *,int);		

	// Function: CalcIntersecPtsNurbsSSearch
	// NURBS�Ȗʓ��m�̌��(��_�Q)������ǐՖ@�ŋ��߂�
	int CalcIntersecPtsNurbsSSearch(NURBSS *,NURBSS *,int,double,Coord *,Coord *,int);	

	// Function: CalcIntersecPtsNurbsCNurbsCParam
    // 2����NURBS�Ȑ����m�̌�_�����߂�
	int CalcIntersecPtsNurbsCNurbsCParam(NURBSC *,NURBSC *,int,Coord *,int);	

    // Function: CalcIntersecPtsNurbsCLine
    // 2����NURBS�Ȑ��ƒ����Ƃ̌�_�����߂�
//	int ClacIntersecPtsNurbsCLine(NURBSC *, Coord, Coord, double *, double *);

    // Function: CalcIntersecPtsNurbsCLineSeg
    // 2����NURBS�Ȑ��Ɛ����Ƃ̌�_�����߂�
//	int ClacIntersecPtsNurbsCLineSeg(NURBSC *, Coord, Coord, double, double, double *, double *);

	// Function: SearchExtremum_BS
	// Bulirsch-Stoer�@�ɂ��ɒn�T�����s��
//	int SearchExtremum_BS(NURBSS *,Coord,double,double,double,int,int,Coord *);	

	// Function: ShiftNurbsS
	// NURBS�Ȗʂ̃V�t�g
//	void ShiftNurbsS(NURBSS *,Coord);							

	// Function: ShiftNurbsC
	// NURBS�Ȑ��̃V�t�g
//	void ShiftNurbsC(NURBSC *,Coord);							

	// Function: ChRatioNurbsS
	// NURBS�Ȗʂ̔{����ύX����
//	void ChRatioNurbsS(NURBSS *,Coord);							

	// Function: ChRatioNurbsC
	// NURBS�Ȑ��̔{����ύX����
//	void ChRatioNurbsC(NURBSC *,Coord);							

	// Function: RotNurbsS
	// NURBS�Ȗʂ���]
//	void RotNurbsS(NURBSS *,Coord,double);						

	// Function: RotNurbsC
	// NURBS�Ȑ�����]
//	void RotNurbsC(NURBSC *,Coord,double);						

	// Function: SetCPNurbsS
	// �R���g���[���|�C���g��������
	int SetCPNurbsS(NURBSS *,NURBSS);							

	// Function: GenInterpolatedNurbsC1
	// �^����ꂽ�_����Ԃ���n�K��NURBS�Ȑ��𐶐�����
	int GenInterpolatedNurbsC1(NURBSC *,Coord *,int,int);		

	// Function: GenInterpolatedNurbsC2
	// �^����ꂽ�_����Ԃ���n�K��NURBS�Ȑ��𐶐�����(�����Ȑ�)
	int GenInterpolatedNurbsC2(NURBSC *,Coord *,int,int);		

	// Function: GenApproximationNurbsC
	// �^����ꂽ�_����ߎ�����n�K��NURBS�Ȑ��𐶐�����
	int GenApproximationNurbsC(NURBSC *,Coord *,int,int);		

	// Function: GenNurbsCfromCP
	// �R���g���[���|�C���g����NURBS�Ȑ��𐶐�����
	int GenNurbsCfromCP(NURBSC *,Coord *,int,int);				

	// Function: GenPolygonalLine
	// �܂���𐶐�����
	int GenPolygonalLine(NURBSC *,Coord *,int);					

	// Function: GenInterpolatedNurbsS1
	// �^����ꂽ�_����Ԃ���n�KNURBS�Ȗʂ𐶐�����
	int GenInterpolatedNurbsS1(NURBSS *,Coord **,int,int,int,int);	

	// Function: GenPolygonalSurface
	// �܂�ʂ𐶐�����
	int GenPolygonalSurface(NURBSS *,Coord **,int,int);				

	// Function: GenApproximationNurbsS
	// �^����ꂽ�_����ߎ�����n�K��NURBS�Ȗʂ𐶐�����
	int GenApproximationNurbsS(NURBSS *,Coord **,int,int,int,int);	

	// Function: GenNurbsSfromCP
	// �^����ꂽ�R���g���[���|�C���g����n�K��NURBS�Ȗʂ𐶐�����
	int GenNurbsSfromCP(NURBSS *,Coord **,int,int,int,int);			

	// Function: DetermPtOnTRMSurf
	// ���ڒ���NURBS�Ȗʏ��1�_(u,v)���g���~���O�̈���ɂ���̂��𔻒肷��
	int DetermPtOnTRMSurf(TRMS *,double,double);					

	// Function: GetPtsOnOuterTRMSurf
	// �O���g�����ʓ��̓_�̂ݎc��
	int GetPtsOnOuterTRMSurf(TRMS *,Coord *,int);					

	// Function: GetPtsOnInnerTRMSurf
	// �����g�����ʊO�̓_�̂ݎc��
	int GetPtsOnInnerTRMSurf(TRMS *,Coord *,int);					 

	// Function: GetPtsOnInnerOuterTRMSurf
	// ���O���g�����ʓ��̓_�̂ݎc��
	int GetPtsOnInnerOuterTRMSurf(TRMS *,Coord *,int);				

	// Function: DetectInterfereNurbsS
	// NURBS�Ȗ�(�g������)���m�̊����o
	int DetectInterfereNurbsS(NURBSS *,NURBSS *,int);				

	// Function: DetectInterfereTrmS
	// NURBS�Ȗ�(�g�����L)���m�̊����o
	int DetectInterfereTrmS(TRIMD_NURBSS *,TRIMD_NURBSS *,int);		

	// Function: CalcIntersecPtsPlaneGeom
	// NURBS�Ȗʂƕ��ʂƌ�_�ǐ՗p�����_�𓾂�(�⏕���ʂ�p�������@)
//	int CalcIntersecPtsPlaneGeom(NURBSS *,Coord,Coord,int,int,Coord *,int);			

	// Function: CalcNurbsCLength
	// NURBS�Ȑ��̐����������߂�
//	double CalcNurbsCLength(NURBSC *);							

	// Function: CalcNurbsCLength
	// NURBS�Ȑ��̎w���Ԃ̐����������߂�
//	double CalcNurbsCLength(NURBSC *,double,double);			

	// Function: CalcDeltaPtsOnNurbsC
	// �w�肵����������NURBS�Ȑ���̍��W�l�����߂�
//	int CalcDeltaPtsOnNurbsC(NURBSC *,int,Coord *);				

	// Function: CalcDeltaPtsOnNurbsS
	// �w�肵����������NURBS�Ȗʏ�̍��W�l�����߂�
//	int CalcDeltaPtsOnNurbsS(NURBSS *,int,int,Coord **);		

	// Function: CalcExtremumNurbsC
	// NURBS�Ȑ��̎w�肵�������ɂ�����ɒl�̍��W�l�𓾂�
//	int CalcExtremumNurbsC(NURBSC *,Coord,double *,int);		


	int CalcExtSearchCurve(NURBSS *,Coord,Coord,double,NURBSC *,NURBSC *);			// �ɒn�T�����𓾂�(������)
	int CalcExtGradCurve(NURBSS *,Coord,Coord,double,NURBSC *,NURBSC *);			// �ɒn�X�ΐ��𓾂�(������)
	int TrimNurbsSPlane(TRMS *,Coord,Coord);										// NURBS�Ȗʂ𕽖ʂŃg��������(������)

	// Function: New_NurbsC
	// NURBS�Ȑ��̃������[�m��
	int New_NurbsC(NURBSC *,int,int);			

	// Function: Free_NurbsC_1DArray
	// NURBS�Ȑ��z��̃������[���
	void Free_NurbsC_1DArray(NURBSC *,int);		

	// Function: Free_NurbsC
	// NURBS�Ȑ��̃������[���
	void Free_NurbsC(NURBSC *);					

	// Function: New_NurbsS
	// NURBS�Ȗʂ̃������[�m��
	int New_NurbsS(NURBSS *,int [],int []);		

	// Function: Free_NurbsS_1DArray
	// NURBS�Ȗʔz��̃������[���
	void Free_NurbsS_1DArray(NURBSS *,int);		

	// Function: Free_NurbsS
	// NURBS�Ȗʂ̃������[���
	void Free_NurbsS(NURBSS *);					

	// Function: New_TrmS
	// �g�����ʂ̃������[�m��
	int New_TrmS(TRMS *,int);					

	// Function: Free_TrmS_1DArray
	// �g�����ʔz��̃������[���
	void Free_TrmS_1DArray(TRMS *,int);			

	// Function: Free_TrmS
	// �g�����ʂ̃������[���
	void Free_TrmS(TRMS *);						

	// Function: New_CompC
	// �����Ȑ��̃������[�m��
	int New_CompC(COMPC *,int);					

	// Function: Free_CompC_1DArray
	// �����Ȑ��z��̃������[���
	void Free_CompC_1DArray(COMPC *,int);		

	// Function: Free_CompC
	// �����Ȑ��̃������[���
	void Free_CompC(COMPC *);					

	// Function: DebugForNurbsC
	// NURBS�Ȑ������f�o�b�O�v�����g
//	void DebugForNurbsC(NURBSC *);				

	// Function: DebugForNurbsS
	// NURBS�Ȗʏ����f�o�b�O�v�����g
//	void DebugForNurbsS(NURBSS *);				

	// Function: CalcIntersecPtsOffsetPlaneGeom
	// �I�t�Z�b�gNURBS�Ȗʂƕ��ʂƌ�_�ǐ՗p�����_�𓾂�(�⏕���ʂ�p�������@)(������)
//	int CalcIntersecPtsOffsetPlaneGeom(NURBSS *,double,Coord,Coord,int,Coord *,int);	

	// Function: CalcTanVecOnNurbsC
	// NURBS�Ȑ����t�ɂ�����P�ʐڃx�N�g�������Ƃ߂�
//	Coord CalcTanVecOnNurbsC(NURBSC *,double);					

	// Function: ConnectNurbsSU
	// 2����NURBS�Ȗʂ�A������(U�����ɒ����Ȃ�)(S1_U1��S2_U0��A��)
	int ConnectNurbsSU(NURBSS *,NURBSS *,NURBSS *);				

	// Function: ConnectNurbsSV
	// 2����NURBS�Ȗʂ�A������(V�����ɒ����Ȃ�)(S1_V1��S2_V0��A��)
	int ConnectNurbsSV(NURBSS *,NURBSS *,NURBSS *);				

	// Function: CalcCurvatureNurbsC
	// NURBS�Ȑ��̋ȗ������߂�
//	double CalcCurvatureNurbsC(NURBSC *,double);				


//	double CalcTorsionNurbsC(NURBSC *,double);					// NURBS�Ȑ��̝��������߂�

	// Function: DivNurbsCParam
	// NURBS�Ȑ����w�肵���p�����[�^�l�ŕ�������
	int DivNurbsCParam(NURBSC *, NURBSC *, NURBSC *, double);	

	// Function: DivNurbsC
	// NURBS�Ȑ����w�肵���ʒu�i�[����̋����j�ŕ�������
	int DivNurbsC(NURBSC *, NURBSC *, NURBSC *, double);		

	// Function: ConnectNurbsC
	// NURBS�Ȑ��̘A��
	int ConnectNurbsC(NURBSC *, NURBSC *, NURBSC *);			

	// Function: ReverseNurbsC
	// NURBS�Ȑ��̃m�b�g�x�N�g�������𔽓]����
//	void ReverseNurbsC(NURBSC *);								

	// Function: CalcParamLengthOnNurbsC
	// NURBS�Ȑ��ɂ����Ĉ�[����̎w�苗���ɂ�����p�����[�^�l��Ԃ�
//	double CalcParamLengthOnNurbsC(NURBSC *,double,double);		


	//int CalcDeltaParamsOnNurbsC(NURBSC *,double,Coord *);		// �w�肵���p�����[�^�̊Ԋu��NURBS�Ȑ���̍��W�l���o�͂���

	// Function: CalcDeltaPtsOnNurbsC
	// �w�肵���Ԋu��NURBS�Ȑ���̍��W�l�����߂�
//	int CalcDeltaPtsOnNurbsC(NURBSC *,double,Coord *);			

private:


	// Function: GetNurbsCCoef
	// (private)NURBS�Ȑ��̌W�������߂�(�ō�3��)
//	int GetNurbsCCoef(NURBSC *,double **,int,Coord *,double *);	

/*	-- member function -> static function
	// Function: GetNurbsSCoef
	// (private)NURBS�Ȗʂɂ�����u�܂���v���Œ肵���ꍇ�ɓ�����NURBS�Ȑ�C(u) or C(v)�̕��ꕪ�q�̌W�������߂�
	void GetNurbsSCoef(int,double **,double *,Coord *,int,Coord *,double *);	

	// Function: GetIntersecEquation
	// (private)NURBS�Ȑ��ƕ��ʂ̌�����o�p3���������𓾂�
	void GetIntersecEquation(int,Coord *,double *,Coord,Coord,double *);		

	// Function: CalcEquation
	// (private)3���������܂ł𔻕ʂ��ĉ���
	int CalcEquation(double *,double *,int);					

	// Function: TrimNurbsSPlaneSub1
	// (private)TrimNurbsSPlane�̃T�u�֐�(2�����̌�_�����Ƃ߂�)
	Coord TrimNurbsSPlaneSub1(double,double,double,double,double,double); 

	// Function: GetMinDistance
	// (private)�ŏ������������W�l��Ԃ�
	Coord GetMinDistance(Coord,Coord *,int);						

	// Function: CheckClossedPoints
	// (private)�w�肵���_������2�_��Ίp�Ƃ��闧���̂̒��ɑ��݂��邩�𒲂ׂ�
	int CheckClossedPoints(Coord,Coord,Coord);				

	// Function: CalcApproximationCP_LSM
	// (private)�ŏ�2��@�ŋߎ��R���g���[���|�C���g�����߂�
	void CalcApproximationCP_LSM(Coord *,Vector,Vector,int,int,int,int,Coord *);	

	// Function: SetApproximationCPnum
	// (private)�_�񐔂��琶������R���g���[���|�C���g�����Z�肷��
	int SetApproximationCPnum(int);									

	// Function: GetApproximatedKnot
	// (private)�Ȑ�/�Ȗʃp�����[�^����ߎ��p�m�b�g�x�N�g�����Z�o
	void GetApproximatedKnot(Vector,int,int,int,Vector);			

	// Function: GetInterpolatedKnot
	// (private)�Ȑ�/�Ȗʃp�����[�^�����ԗp�m�b�g�x�N�g�����Z�o
	void GetInterpolatedKnot(Vector,int,int,int,Vector);			

	// Function: GetSurfaceKnotParam
	// (private)�e�ʉߓ_�̋Ȗʃp�����[�^���Z�o
	void GetSurfaceKnotParam(Vector,Vector,Coord **,int,int);		

	// Function: GetCurveKnotParam1
	// (private)�e�ʉߓ_�̋Ȑ��p�����[�^���Z�o(�R�[�h���̔䂩��Z�o)
	void GetCurveKnotParam1(Coord *,int,Vector);					

	// Function: GetCurveKnotParam2
	// (private)�e�ʉߓ_�̋Ȑ��p�����[�^���Z�o(�R�[�h���̕������̔䂩��Z�o)
	void GetCurveKnotParam2(Coord *,int,Vector);					
*/
	// Function: SearchIntersectPt
	// (private)�j���[�g���@�ɂ���_������������(NURBS�Ȗʂƕ���)
//	int SearchIntersectPt(NURBSS *,Coord,Coord,double,double *,double *,int);	

	// Function: SearchIntersectPt_RKM
	// (private)4���̃����Q�N�b�^�@�ɂ���_������������(NURBS�Ȗʂƕ���)
//	int SearchIntersectPt_RKM(NURBSS *,Coord,Coord,double,double *,double *,int);	

	// Function: SearchIntersectPt_BS
	// (private)Bulirsch-Stoer�@�ɂ���_������������(NURBS�Ȗʂƕ���)
//	int SearchIntersectPt_BS(NURBSS *,Coord,Coord,double,double *,double *,int);	

	// Function: SearchIntersectPt_OS
	// (private)4���̃����Q�N�b�^�@�ɂ���_������������(�I�t�Z�b�gNURBS�Ȗʂƕ���)
//	int SearchIntersectPt_OS(NURBSS *,Coord,Coord,double,double *,double *,int);		

	// Function: GetSIPParam1
	// (private)NURBS�Ȗʂƕ��ʂ̌�_��\�������������̉E�ӂ̒l�𓾂�
//	int GetSIPParam1(NURBSS *,double ,double ,Coord ,Coord ,int ,Coord *);		

	// Function: SearchIntersectPt
	// (private)�j���[�g���@�ɂ���_������������(NURBS�Ȗʓ��m)
	int SearchIntersectPt(NURBSS *,NURBSS *,double,double *,double *,double *,double *,int);		

	// Function: DetermPtOnTRMSurf_sub
	// (private)�g�������E���������Ȑ��̏ꍇ�̃g���~���O�̈���O����
	int DetermPtOnTRMSurf_sub(CONPS *,double,double);				

	// Function: ApproxTrimBorder
	// (private)�g�������E����_�Q�ŋߎ�����
	int ApproxTrimBorder(COMPC *,Coord *);							

	// Function: RemoveTheSamePoints
	// (private)NURBS�Ȗʏ�̓���_����������
//	int RemoveTheSamePoints(NURBSS *,Coord *,int);					

	// Function: CalcDiffNurbsSDenom
	// (private)NURBS�Ȗʕ���̊e������C�ӊK���������Ƃ��̔����W�������߂�
//	double CalcDiffNurbsSDenom(NURBSS *,int,int,double,double);		

	// Function: CalcDiffNurbsSNumer
	// (private)NURBS�Ȗʕ��q�̊e������C�ӊK���������Ƃ��̔����W�������߂�
//	Coord CalcDiffNurbsSNumer(NURBSS *,int,int,double,double);		

	// Function: CalcIntersecPtsPlaneSearch_Sub
	// (private)�ʂ����яo����(u,v)���Q�l�ɖʂ̃G�b�W��(new_u,new_v)�𓾂�
//	Coord CalcIntersecPtsPlaneSearch_Sub(NURBSS *,double,double,Coord,Coord);	

	// Function: GetSECParam1
	// (private)�ɒl�T����Sub�֐�1
//	int GetSECParam1(NURBSS *,double,double,Coord,int,int,Coord *);	

	// Function: GetMinDist
	// (private)�ŏ������𒲂ׂ�
//	int GetMinDist(NURBSS *,Coord ,Coord *,int ,Coord *);			

	// Function: SetKnotVecSU_ConnectS
	// (private)NURBS�ȖʘA���pSUB�֐�(�A����̋Ȗʂ�U�����m�b�g��`���ݒ肷��)
	void SetKnotVecSU_ConnectS(NURBSS *,NURBSS *,NURBSS *);			

	// Function: SetKnotVecSV_ConnectS
	// (private)NURBS�ȖʘA���pSUB�֐�(�A����̋Ȗʂ�V�����m�b�g��`���ݒ肷��)
	void SetKnotVecSV_ConnectS(NURBSS *,NURBSS *,NURBSS *);			

	// Function: SetCPSU_ConnectS
	// (private)NURBS�ȖʘA���pSUB�֐�(�A����̋Ȗʂ�U�����R���g���[���|�C���g�ƃE�F�C�g��ݒ肷��)
	void SetCPSU_ConnectS(NURBSS *,NURBSS *,NURBSS *);				

	// Function: SetCPSV_ConnectS
	// (private)NURBS�ȖʘA���pSUB�֐�(�A����̋Ȗʂ�V�����R���g���[���|�C���g�ƃE�F�C�g��ݒ肷��)
	void SetCPSV_ConnectS(NURBSS *,NURBSS *,NURBSS *);				

	// Function: InsertNewKnotOnNurbsC
	// (private)NURBS�Ȑ��ɐV���ȃm�b�g��}������
	int InsertNewKnotOnNurbsC(NURBSC *,NURBSC *,double,int);		

	// Function: SetKnotVecC_ConnectC
	// (private)NURBS�Ȑ��A���pSUB�֐�(�A����̋Ȑ��̃m�b�g��`���ݒ肷��)
	void SetKnotVecC_ConnectC(NURBSC *,NURBSC *,NURBSC *);			

	// Function: SetCPC_ConnectC
	// (private)NURBS�Ȑ��A���pSUB�֐�(�A����̋Ȑ��̃R���g���[���|�C���g�ƃE�F�C�g��ݒ肷��)
	void SetCPC_ConnectC(NURBSC *,NURBSC *,NURBSC *);				

};

#endif
