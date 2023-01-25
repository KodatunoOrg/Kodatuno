// BODY�̒�`

#ifndef _BODY_H_
#define _BODY_H_

#include "StdAfx.h"
#include "KodListFunc.h"
#include "MESH.h"
#include <string>
//#include "boost/variant.hpp"
//#include "boost/shared_array.hpp"

// Constants: General Defines
// ALL_ENTITY_TYPE_NUM -	�S�G���e�B�e�B�^�C�v�̐�(21)
// CTLPNUMMAX -				NURBS�ŗp������R���g���[���|�C���g�̐��̏��(1024)
// KNOTNUMMAX -				NURBS�ŗp������m�b�g�V�[�P���X�̐��̏��(1024)
// GEOMTRYELEM -			IGES�f�B���N�g����"Entity Use Flag"���A�􉽗v�f������(0)
// PARAMETRICELEM -			IGES�f�B���N�g����"Entity Use Flag"���A2D�p�����g���b�N�v�f������(5)
// NORM_KNOT_VAL -			�m�b�g�x�N�g���𐳋K������Ƃ��͈̔͂̍ő�l(1)
// MIN_KNOT_RANG -			�ׂ荇���m�b�g�x�N�g���̍������̒l�ȏ�ł��邱��(0.0002)
#define CTLPNUMMAX  1024
#define KNOTNUMMAX  1024
#define GEOMTRYELEM 0
#define PARAMETRICELEM 5
#define NORM_KNOT_VAL	1
#define MIN_KNOT_RANGE	0.0002

// Constants: Symbol of Entity Type
//	CIRCLE_ARC -				�~/�~��(100)
//	COMPOSITE_CURVE -			�����Ȑ�(102)
//	CONIC_ARC -					�~���Ȑ�(104)
//	COPIOUS_DATA -				�L�ӓ_��(106)
//	PLANE -						����(108)
//	LINE -						����(110)
//	PARAMETRIC_SPLINE_CURVE -	�p�����g���b�N�X�v���C���Ȑ�(112)
//	PARAMETRIC_SPLINE_SURFACE - �p�����g���b�N�X�v���C���Ȗ�(114)
//	POINT -						�_(116)
//	TRANSFORMATION_MATRIX -		�ϊ��s��(124)
//	NURBS_CURVE -				�L��B�X�v���C���Ȑ�(126)
//	NURBS_SURFACE -				�L��B�X�v���C���Ȗ�(128)
//	CURVE_ON_PARAMETRIC_SURFACE - �ʏ��(142)
//	TRIMMED_SURFACE -			�g������(144)
//	SUBFIGURE_DEFINITION -		�q�}�̒�`(308)
//	ASSOCIATIVITY_INSTANCE -	�O���[�v(402)
//	DRAWING -					�}��(404)
//	PROPERTY -					�}�ʃT�C�Y(406)
//	SINGULAR_SUBFIGURE_INSTANCE - �q�}�̎Q��(408)
//	VIEW - ���ۖ�(410)
#define	CIRCLE_ARC					100
#define	COMPOSITE_CURVE				102
#define	CONIC_ARC					104
#define	COPIOUS_DATA				106
#define	PLANE						108
#define	LINE						110
#define	PARAMETRIC_SPLINE_CURVE		112
#define	PARAMETRIC_SPLINE_SURFACE	114
#define	POINT						116
#define	TRANSFORMATION_MATRIX		124
#define	NURBS_CURVE					126
#define	NURBS_SURFACE				128
#define	CURVE_ON_PARAMETRIC_SURFACE 142
#define	TRIMMED_SURFACE				144
#define	SUBFIGURE_DEFINITION		308
#define	ASSOCIATIVITY_INSTANCE		402
#define	DRAWING						404	
#define	PROPERTY					406
#define	SINGULAR_SUBFIGURE_INSTANCE 408
#define	VIEW						410

// Enum: Enum Symbol of Entity Type
// _CIRCLE_ARC -					0:�~�E�~��
// _COMPOSITE_CURVE -				1:�����Ȑ�
// _CONIC_ARC -						2:�~���Ȑ�
// _COPIOUS_DATA -					3:�L�ӓ_��
// _PLANE -							4:����
// _LINE -							5:����
// _PARAMETRIC_SPLINE_CURVE -		6:�p�����g���b�N�X�v���C���Ȑ�
// _PARAMETRIC_SPLINE_SURFACE -		7:�p�����g���b�N�X�v���C���Ȗ�
// _POINT -							8:�_
// _TRANSFORMATION_MATRIX -			9:�ϊ��s��
// _NURBSC -						10:�L��B�X�v���C���Ȑ�
// _NURBSS -						11:�L��B�X�v���C���Ȗ�
// _CURVE_ON_PARAMETRIC_SURFACE -	12:�ʏ��
// _TRIMMED_SURFACE -				13:�g������
// _SUBFIGURE_DEFINITION -			14:�q�}�̒�`
// _ASSOCIATIVITY_INSTANCE -		15:�O���[�v
// _DRAWING -						16:�}��
// _PROPERTY -						17:�}�ʃT�C�Y
// _SINGULAR_SUBFIGURE_INSTANCE -	18:�q�}�̎Q��
// _VIEW -							19:���ۖ�
// _MESH -							20:���b�V��
enum EntityType{
	_CIRCLE_ARC = 0,
	_COMPOSITE_CURVE,
	_CONIC_ARC,
	_COPIOUS_DATA,
	_PLANE,
	_LINE,
	_PARAMETRIC_SPLINE_CURVE,
	_PARAMETRIC_SPLINE_SURFACE,
	_POINT,
	_TRANSFORMATION_MATRIX,
	_NURBSC,
	_NURBSS,
	_CURVE_ON_PARAMETRIC_SURFACE,
	_TRIMMED_SURFACE,
	_SUBFIGURE_DEFINITION,
	_ASSOCIATIVITY_INSTANCE,
	_DRAWING,
	_PROPERTY,
	_SINGULAR_SUBFIGURE_INSTANCE,
	_VIEW,
	_MESH,
		ALL_ENTITY_TYPE_NUM		// 21
};

/*
 * �G���e�B�e�B�^�C�v���Ƃɍ\���̂��`
 * 9�̃G���e�B�e�B�^�C�v��ǂݍ��ݑΏۂƂ���(����ȊO�͓ǂݎ̂�)
 * �ǉ�����ꍇ�́A�ȉ��ɒǉ�����G���e�B�e�B�^�C�v�̍\���̂��`���Ă�������
 */

// Structure: CIRA
// �~�E�~�ʂ�\�킷�\����
// 
// Variables:
// double	zt -			Z�������̐[��
// Coord	cp[3] -			�~�E�~�ʂ̒��S�_�A�n�_�A�I�_
// double	R -				���a
// double	t[2] -			t[0]:�n�_�̊p�x t[1]:�I�_�̊p�x
// Coord	U,V -			�~�C�~�ʂ��\�����镽�ʂ̔C�ӂ̒����x�N�g��
// int		EntUseFlag -	�f�B���N�g���� Entity Use Flag �̒l(0:�􉽗v�f 5:2D�p�����g���b�N�v�f)
// int		pD -			�f�B���N�g�����ւ̋t�|�C���^
// DispStat	Dstat -			 �\�������i�Fr,g,b�j
struct CIRA
{
	double zt;		
	Coord  cp[3];	
	double R;
	double t[2];
	Coord  U,V;
	int EntUseFlag;
	int pD;
	DispStat Dstat;

	CIRA() {
		zt = 0;
		cp[0] = cp[1] = cp[2] = SetCoord(0,0,0);
		R = 0;
		t[0] = t[1] = 0;
		U = V = SetCoord(0,0,0);
		EntUseFlag = 0;
		pD = 0;
	}
};

// Structure: CONA
// �~���Ȑ���\�킷�\����
//
// Variables:
// double prop[6] - �W��
// double zt -		ZT���ʂ̒�`
// Coord  cp[2] -	�n�_�A�I�_
// int pD -			�f�B���N�g�����ւ̋t�|�C���^
// DispStat Dstat - �\�������i�Fr,g,b�j
struct CONA
{
	double prop[6];
	double zt;
	Coord  cp[2];
	int pD;
	DispStat Dstat;

	CONA() {
		InitVector(prop,6);
		zt = 0;
		cp[0] = cp[1] = SetCoord(0,0,0);
		pD = 0;
	}
};

// Structure: LINE_
// ������\�킷�\����
//
// Variables:
// Coord cp[2] -	�n�_�A�I�_
// int EntUseFlag - �f�B���N�g���� Entity Use Flag �̒l(0:�􉽗v�f 5:2D�p�����g���b�N�v�f)
// int pD -			�f�B���N�g�����ւ̋t�|�C���^
// DispStat Dstat - �\�������i�Fr,g,b�j
struct LINE_
{
	Coord cp[2];
	int EntUseFlag;
	int pD;
	DispStat Dstat;

	LINE_() {
		cp[0] = cp[1] = SetCoord(0,0,0);
		EntUseFlag = 0;
		pD = 0;
	}
};

// Structure: TMAT
// �ϊ��}�g���b�N�X��\�킷�\����
//
// Variables:
// double R[3][3] - ��]�s��
// double T[3] -	���i�x�N�g��
// int pD -			�f�B���N�g�����ւ̋t�|�C���^
struct TMAT
{
	double R[3][3];	// ��]�s��
	double T[3];	// ���i�x�N�g��
	int pD;			// �f�B���N�g�����ւ̋t�|�C���^

	TMAT() {
		for(int j=0;j<3;j++)
			InitVector(R[j],3);
		InitVector(T,3);
		pD = 0;
	}
};

// Structure: NURBSC
// �L��B�X�v���C��(NURBS)�Ȑ���\�킷�\����
//
// Variables:
// int K -			�R���g���[���|�C���g�̐�
// int M -			�K��(=����+1)
// int N -			�m�b�g�x�N�g���̐�
// int prop[4] -	�e�v���p�e�B
//					prop[0]==0:�񕽖ʓ��Ȑ�, 1:���ʓ��Ȑ�
//					prop[1]==0:�Ȑ��łȂ��C1:�Ȑ�
//					prop[2]==0:�L�����C1:������
//					prop[3]==0:������I�Ȑ�, 1:�����I�Ȑ�	
// double *T -		�m�b�g�V�[�P���X�̒l K+M��				
// double *W -		Weight�̒l K��							
// Coord *cp -		�R���g���[���|�C���g K��								
// double V[2] -	�p�����[�^�͈̔�
// Coord norm -		�@���x�N�g��
// int EntUseFlag - �f�B���N�g���� Entity Use Flag �̒l(0:�􉽗v�f 5:2D�p�����g���b�N�v�f)
// int pD -			�f�B���N�g�����ւ̋t�|�C���^
// int OriginEnt -	���̃G���e�B�e�B�^�C�v
// void *pOriginEnt - ���̃G���e�B�e�B�ւ̃|�C���^
// DispStat Dstat - �\�������i�Fr,g,b�j
struct NURBSC
{
	int K;
	int M;
	int N;
	int prop[4];
	double *T;
	double *W;
	Coord *cp;
//	boost::shared_array<double>	T;
//	boost::shared_array<double>	W;
//	boost::shared_array<Coord>	cp;
	double V[2];
	Coord norm;
	int EntUseFlag;
	int pD;
	int OriginEnt;
	void *pOriginEnt;
	DispStat Dstat;

	NURBSC() {
		K = 0;
		M = 0;
		N = 0;
		prop[0] = prop[1] = prop[2] = prop[3] = 0;
		T = NULL;
		W = NULL;
		cp = NULL;
		V[0] = V[1] = 0;
		norm = SetCoord(0,0,0);
		EntUseFlag = 0;
		pD = 0;
		OriginEnt = 0;
		pOriginEnt = NULL;
	}

	~NURBSC() {		// ���������[�N�h�~�B�B�B�ł��Ȃ��H�H
		if ( T )
			delete[]	T;
		if ( W )
			delete[]	W;
		if ( cp )
			delete[]	cp;
	}

	// Function: CalcNurbsCCoord
	// �w�肵��t�ł�NURBS�Ȑ��̍��W�l�����߂�
	Coord CalcNurbsCCoord(double);

	// Function: CalcNurbsCCoords
	// �w�肵��t�Q�ł�NURBS�Ȑ��̍��W�l�Q�����߂�
	void CalcNurbsCCoords(int,double *,Coord *);

	// Function: CalcDiffNurbsC
	// NURBS�Ȑ���1�K�����W�������߂�
	Coord CalcDiffNurbsC(double);						

	// Function: CalcDiff2NurbsC
	// NURBS�Ȑ���2�K�����W�������߂�
	Coord CalcDiff2NurbsC(double);						

	// Function: CalcDiffNNurbsC
	// NURBS�Ȑ���r�K�����W�������߂�
	Coord CalcDiffNNurbsC(int,double);					

	// Function: CalcNurbsCLength
	// NURBS�Ȑ��̐����������߂�
	double CalcNurbsCLength(void);							

	// Function: CalcNurbsCLength
	// NURBS�Ȑ��̎w���Ԃ̐����������߂�
	double CalcNurbsCLength(double,double);			

	// Function: CalcDeltaPtsOnNurbsC
	// �w�肵����������NURBS�Ȑ���̍��W�l�����߂�
	int CalcDeltaPtsOnNurbsC(int,Coord *);				

	// Function: CalcTanVecOnNurbsC
	// NURBS�Ȑ����t�ɂ�����P�ʐڃx�N�g�������Ƃ߂�
	Coord CalcTanVecOnNurbsC(double);					

	// Function: CalcCurvatureNurbsC
	// NURBS�Ȑ��̋ȗ������߂�
	double CalcCurvatureNurbsC(double);				

	// Function: ShiftNurbsC
	// NURBS�Ȑ��̃V�t�g
	void ShiftNurbsC(Coord);							

	// Function: ChRatioNurbsC
	// NURBS�Ȑ��̔{����ύX����
	void ChRatioNurbsC(Coord);							

	// Function: RotNurbsC
	// NURBS�Ȑ�����]
	void RotNurbsC(Coord,double);						

	// Function: CalcIntersecPtNurbsPt
	// ��ԏ��1�_����NURBS�Ȑ���̍ŋߖT�_�����߂�(�j���[�g���@)(�I�[�o�[���[�h)
	int CalcIntersecPtNurbsPt(Coord,int,int,double *);			

	// Function: CalcIntersecCurve
	// NURBS�Ȑ��ƕ��ʂƂ̌�_�����߂�(�j���[�g���@)
	int CalcIntersecCurve(Coord,Coord,int,double *,int,int);	

	// Function: CalcIntersecCurve3
	// 3���ȉ���NURBS�Ȑ��ƕ��ʂƂ̌�_�����߂�
	int CalcIntersecCurve3(Coord,Coord,double *,int);	

    // Function: CalcIntersecPtsNurbsCLine
    // 2����NURBS�Ȑ��ƒ����Ƃ̌�_�����߂�
    int ClacIntersecPtsNurbsCLine(Coord, Coord, double *, double *);

    // Function: CalcIntersecPtsNurbsCLineSeg
    // 2����NURBS�Ȑ��Ɛ����Ƃ̌�_�����߂�
    int ClacIntersecPtsNurbsCLineSeg(Coord, Coord, double, double, double *, double *);

	// Function: CalcParamLengthOnNurbsC
	// NURBS�Ȑ��ɂ����Ĉ�[����̎w�苗���ɂ�����p�����[�^�l��Ԃ�
	double CalcParamLengthOnNurbsC(double,double);		

	// Function: CalcDeltaPtsOnNurbsC
	// �w�肵���Ԋu��NURBS�Ȑ���̍��W�l�����߂�
	int CalcDeltaPtsOnNurbsC(double,Coord *);			

	// Function: ReverseNurbsC
	// NURBS�Ȑ��̃m�b�g�x�N�g�������𔽓]����
	void ReverseNurbsC(void);								

	// Function: CalcExtremumNurbsC
	// NURBS�Ȑ��̎w�肵�������ɂ�����ɒl�̍��W�l�𓾂�
	int CalcExtremumNurbsC(Coord,double *,int);		

#ifdef _DEBUG
	// Function: DebugForNurbsC
	// NURBS�Ȑ������f�o�b�O�v�����g
	void DebugForNurbsC(void);				
#endif

private:
	// Function: GetNurbsCCoef
	// (private)NURBS�Ȑ��̌W�������߂�(�ō�3��)
	int GetNurbsCCoef(double **,int,Coord *,double *);	
};

// Structure: NURBSS
// �L��B�X�v���C��(NURBS)�Ȗʂ�\�킷�\����
//
// Variables:
// int K[2] -		�R���g���[���|�C���g�̐�(u����,v����)
// int M[2] -		�K��(=����+1)
// int N[2] -		�m�b�g�x�N�g���̐�(K+M)
// int prop[5] -	�p�����[�^
//					prop[0]==0:u�����ŕ��Ă���, 1:���Ă��Ȃ�
//					prop[1]==0:v�����ŕ��Ă���C1:���Ă��Ȃ�
//					prop[2]==0:�L�����C1:������
//					prop[3]==0:u�����Ŕ�����I, 1:�����I
//					prop[4]==0:v�����Ŕ�����I, 1:�����I
// double *S -		u�����m�b�g�x�N�g���̒l A+1��			
// double *T -		v�����m�b�g�x�N�g���̒l B+1��			
// double **W -		Weight�̒l								
// Coord  **cp -	�R���g���[���|�C���g C��					
// double U[2] -	u�����p�����[�^�͈̔�
// double V[2] -	v�����p�����[�^�͈̔�
// int pD -			�f�B���N�g�����ւ̋t�|�C���^
// int TrmdSurfFlag - ����NURBS�Ȗʂ��g�����ʂƂ��ČĂ΂�Ă���̂��A�Ɨ����đ��݂���̂��������t���O(�g������:KOD_TRUE  �Ɨ���:KOD_FALSE)
// DispStat Dstat - �\�������i�Fr,g,b,�j
struct NURBSS
{
	int K[2];
	int M[2];
	int N[2];
	int prop[5];
	double *S;
	double *T;
	double **W;
	Coord  **cp;
	double U[2];
	double V[2];
	int pD;
	int TrmdSurfFlag;
	DispStat Dstat;

	NURBSS() {
		K[0] = K[1] = 0;
		M[0] = M[1] = 0;
		N[0] = N[0] = 0;
		prop[0] = prop[1] = prop[2] = prop[3] = prop[4] = 0;
		S = NULL;
		T = NULL;
		W = NULL;
		cp = NULL;
		U[0] = U[1] = 0;
		V[0] = V[1] = 0;
		pD = 0;
		TrmdSurfFlag = 0;
	}

	// Function: CalcNurbsSCoord
	// �w�肵��u,v�ł�NURBS�Ȗʂ̍��W�_�����߂�
	Coord CalcNurbsSCoord(double,double);				

	// Function: CalcNurbsSCoords
	// �w�肵��u,v�Q�ł�NURBS�Ȗʂ̍��W�l�Q�����߂�
	void CalcNurbsSCoords(int,Coord *,Coord *);		

	// Function: CalcDiffuNurbsS
	// NURBS�Ȗʂ�u����1�K�����W�������߂�
	Coord CalcDiffuNurbsS(double,double);				

	// Function: CalcDiffvNurbsS
	// NURBS�Ȗʂ�v����1�K�����W�������߂�
	Coord CalcDiffvNurbsS(double,double);				

	// Function: CalcDiffNNurbsS
	// NURBS�Ȗʂ̊e������C�ӊK���������Ƃ��̔����W�������߂�
	Coord CalcDiffNNurbsS(int,int,double,double);		

	// Function: CalcNormVecOnNurbsS
	// NURBS�Ȗʏ��(u,v)�ɂ�����@���x�N�g�������Ƃ߂�
	Coord CalcNormVecOnNurbsS(double,double);			

	// Function: CalcDiffuNormVecOnNurbsS
	// NURBS�Ȗʏ��(u,v)�ɂ�����@���x�N�g����u����1�K���������Ƃ߂�
	Coord CalcDiffuNormVecOnNurbsS(double,double);		

	// Function: CalcDiffvNormVecOnNurbsS
	// NURBS�Ȗʏ��(u,v)�ɂ�����@���x�N�g����v����1�K���������Ƃ߂�
	Coord CalcDiffvNormVecOnNurbsS(double,double);		

	// Function: CalcMeanCurvature
	// NURBS�Ȗʏ��(u,v)�ɂ����镽�ϋȗ������߂�
	double CalcMeanCurvature(double,double);			

	// Function: CalcMeanCurvatureNormVec
	// NURBS�Ȗʏ��(u,v)�ɂ����镽�ϋȗ��@���x�N�g�������߂�
	Coord CalcMeanCurvatureNormVec(double,double);		

	// Function: CalcGaussCurvature
	// NURBS�Ȗʏ��(u,v)�ɂ�����K�E�X�ȗ������߂�
	double CalcGaussCurvature(double,double);			

	// Function: CalcGaussCurvatureNormVec
	// NURBS�Ȗʏ��(u,v)�ɂ�����K�E�X�ȗ��@���x�N�g�������߂�
	Coord CalcGaussCurvatureNormVec(double,double);	

	// Function: ShiftNurbsS
	// NURBS�Ȗʂ̃V�t�g
	void ShiftNurbsS(Coord);							

	// Function: ChRatioNurbsS
	// NURBS�Ȗʂ̔{����ύX����
	void ChRatioNurbsS(Coord);							

	// Function: RotNurbsS
	// NURBS�Ȗʂ���]
	void RotNurbsS(Coord,double);						

	// Function: CalcuIntersecPtNurbsLine
	// NURBS�Ȗʂƒ����̌�_���Z�o
	int CalcuIntersecPtNurbsLine(Coord,Coord,int,Coord *,int,int);	

	// Function: CalcIntersecPtNurbsPt
	// ��ԏ��1�_����NURBS�Ȗʏ�̍ŋߖT�_�����߂�(�j���[�g���@)
	int CalcIntersecPtNurbsPt(Coord,int,int,Coord *);		

	// Function: CalcIntersecIsparaCurveU
	// u�����A�C�\�p���Ȑ��ƕ��ʂƂ̌�_�����߂�(�j���[�g���@)
	int CalcIntersecIsparaCurveU(double,Coord,Coord,int,double *,int);	

	// Function: CalcIntersecIsparaCurveV
	// v�����A�C�\�p���Ȑ��ƕ��ʂƂ̌�_�����߂�(�j���[�g���@)
	int CalcIntersecIsparaCurveV(double,Coord,Coord,int,double *,int); 

	// Function: CalcIntersecPtsPlaneV3
	// V�����̃A�C�\�p���Ȑ����w�肵���������Ő������C�e3���ȉ��̋Ȑ���NURBS�ȖʂƂ̌�_��㐔�v�Z�ŎZ�o����
	int CalcIntersecPtsPlaneV3(Coord,Coord,int,Coord *,int);	

	// Function: CalcIntersecPtsPlaneU3
	// V�����̃A�C�\�p���Ȑ����w�肵���������Ő������C�e3���ȉ��̋Ȑ���NURBS�ȖʂƂ̌�_��㐔�v�Z�ŎZ�o����
	int CalcIntersecPtsPlaneU3(Coord,Coord,int,Coord *,int);	

	// Function: CalcIntersecPtsPlaneV
	// V�����̃A�C�\�p���Ȑ����w�肵���������Ő������C�e�Ȑ���NURBS�ȖʂƂ̌�_���Z�o����
	int CalcIntersecPtsPlaneV(Coord,Coord,int,Coord *,int);	

	// Function: CalcIntersecPtsPlaneU
	// U�����̃A�C�\�p���Ȑ����w�肵���������Ő������C�e�Ȑ���NURBS�ȖʂƂ̌�_���Z�o����
	int CalcIntersecPtsPlaneU(Coord,Coord,int,Coord *,int);	

	// Function: CalcIntersecPtsPlaneGeom
	// NURBS�Ȗʂƕ��ʂƌ�_�ǐ՗p�����_�𓾂�(�⏕���ʂ�p�������@)
	int CalcIntersecPtsPlaneGeom(Coord,Coord,int,int,Coord *,int);			

	// Function: CalcIntersecPtsPlaneSearch
	// NURBS�Ȗʂƕ��ʂƂ̌�_�Q������ǐՖ@�ŋ��߂�
	int CalcIntersecPtsPlaneSearch(Coord,Coord,double,int,Coord *,int,int);	

	// Function: CalcIntersecPtsOffsetPlaneGeom
	// �I�t�Z�b�gNURBS�Ȗʂƕ��ʂƌ�_�ǐ՗p�����_�𓾂�(�⏕���ʂ�p�������@)(������)
	int CalcIntersecPtsOffsetPlaneGeom(double,Coord,Coord,int,Coord *,int);	

	// Function: CalcIntersecPtsOffsetPlaneSearch
	// �I�t�Z�b�gNURBS�Ȗʂƕ��ʂƂ̌�_�Q������ǐՖ@�ŋ��߂�(������)
	int CalcIntersecPtsOffsetPlaneSearch(double,Coord,Coord,double,int,Coord *,int);

	// Function: SearchExtremum_BS
	// Bulirsch-Stoer�@�ɂ��ɒn�T�����s��
	int SearchExtremum_BS(Coord,double,double,double,int,int,Coord *);	

	// Function: CalcDeltaPtsOnNurbsS
	// �w�肵����������NURBS�Ȗʏ�̍��W�l�����߂�
	int CalcDeltaPtsOnNurbsS(int,int,Coord **);		

#ifdef _DEBUG
	// Function: DebugForNurbsS
	// NURBS�Ȗʏ����f�o�b�O�v�����g
	void DebugForNurbsS(void);
#endif

private:
	// Function: CalcDiffNurbsSDenom
	// (private)NURBS�Ȗʕ���̊e������C�ӊK���������Ƃ��̔����W�������߂�
	double CalcDiffNurbsSDenom(int,int,double,double);		

	// Function: CalcDiffNurbsSNumer
	// (private)NURBS�Ȗʕ��q�̊e������C�ӊK���������Ƃ��̔����W�������߂�
	Coord CalcDiffNurbsSNumer(int,int,double,double);		

	// Function: GetMinDist
	// (private)�ŏ������𒲂ׂ�
	int GetMinDist(Coord ,Coord *,int ,Coord *);			

	// Function: SearchIntersectPt
	// (private)�j���[�g���@�ɂ���_������������(NURBS�Ȗʂƕ���)
	int SearchIntersectPt(Coord,Coord,double,double *,double *,int);	

	// Function: SearchIntersectPt_RKM
	// (private)4���̃����Q�N�b�^�@�ɂ���_������������(NURBS�Ȗʂƕ���)
	int SearchIntersectPt_RKM(Coord,Coord,double,double *,double *,int);	

	// Function: SearchIntersectPt_BS
	// (private)Bulirsch-Stoer�@�ɂ���_������������(NURBS�Ȗʂƕ���)
	int SearchIntersectPt_BS(Coord,Coord,double,double *,double *,int);	

	// Function: SearchIntersectPt_OS
	// (private)4���̃����Q�N�b�^�@�ɂ���_������������(�I�t�Z�b�gNURBS�Ȗʂƕ���)
	int SearchIntersectPt_OS(Coord,Coord,double,double *,double *,int);		

	// Function: GetSIPParam1
	// (private)NURBS�Ȗʂƕ��ʂ̌�_��\�������������̉E�ӂ̒l�𓾂�
	int GetSIPParam1(double ,double ,Coord ,Coord ,int ,Coord *);		

	// Function: CalcIntersecPtsPlaneSearch_Sub
	// (private)�ʂ����яo����(u,v)���Q�l�ɖʂ̃G�b�W��(new_u,new_v)�𓾂�
	Coord CalcIntersecPtsPlaneSearch_Sub(double,double,Coord,Coord);	

	// Function: RemoveTheSamePoints
	// (private)NURBS�Ȗʏ�̓���_����������
	int RemoveTheSamePoints(Coord *,int);					

	// Function: GetSECParam1
	// (private)�ɒl�T����Sub�֐�1
	int GetSECParam1(double,double,Coord,int,int,Coord *);	
};

// Structure: COMPELEM
// �����Ȑ����\���ł���Ȑ��Q�����p�̂Ő錾
//
// Variables:
// CONA ConA -		�~���Ȑ�
// LINE_ Line -		����
// NURBSC NurbsC -	NURBS�Ȑ�
union COMPELEM
{
//	CIRA CirA;
//	CONA ConA;
//	LINE_ Line;
//	NURBSC NurbsC;
	void*	substitution;	// �����ɑ��
	CIRA*	CirA;
	CONA*	ConA;
	LINE_*	Line;
	NURBSC*	NurbsC;
};

// Structure: COMPC
// �����Ȑ�
//
// Variables:
// int N -				�\���v�f��
// int *DEType -		�e�\���v�f�̃G���e�B�e�B�^�C�v
// COMPELEM **pDE -		�e�\���v�f�̍\���̂ւ̃|�C���^
// int DegeFlag -		�����Ȑ����k�ނ���2D�p�����g���b�N�Ȑ���\���t���O
// NURBSC DegeNurbs -	�����Ȑ����k�ނ���2D�p�����g���b�N�Ȑ��������ꍇ�ɏk�ނ��������邽�߂�NURBS�Ȑ�
// int pD -				�f�B���N�g�����ւ̋t�|�C���^
struct COMPC
{
	int N;
	int *DEType;
//	COMPELEM **pDE;
	COMPELEM*	pDE;
	int DegeFlag;
	NURBSC DegeNurbs;
	int pD;

	COMPC() {
		N = 0;
		DEType = NULL;
		pDE = NULL;
		DegeFlag = 0;
		pD = 0;
	}
};

// Structure: CURVE
// �ʏ�����\���ł���Ȑ��Q�����p�̂Ő錾
//
// Variables:
// CIRA  CirA -		�~�E�~��
// COMPC CompC -	�����Ȑ�
// CONA  ConA -		�~���Ȑ�
// NURBSC NurbsC -	NURBS�Ȑ�
union CURVE
{
//	CIRA  CirA;
//	COMPC CompC;
//	CONA  ConA;
//	NURBSC NurbsC;
	void*	substitution;	// �����ɑ��
	CIRA*	CirA;
	COMPC*	CompC;
	CONA*	ConA;
	NURBSC*	NurbsC;
};
//typedef CURVE	boost::variant<CIRA*, COMPC*, CONA*, NURBSC*>

// Structure: CONPS
// �ʏ��
//
// Variables:
// int crtn -	�ʏ�����ǂ̂悤�ɍ��ꂽ��������
// int SType -	Surface S�̃G���e�B�e�B�^�C�v
// int BType -	Curve B�̃G���e�B�e�B�^�C�v
// int CType -	Curve C�̃G���e�B�e�B�^�C�v
// NURBSS *pS - Curve�����Surface�\���̂ւ̃|�C���^
// CURVE *pB -	Surface S�̃p�����[�^��Ԃɂ�����Curve B�\���̂ւ̃|�C���^
// CURVE *pC -	Curve C�\���̂ւ̃|�C���^
// int pref -	���葤�V�X�e���ō̂��Ă����\���������t���O
// int pD -		�f�B���N�g�����ւ̋t�|�C���^
struct CONPS
{
	int crtn;
	int SType;
	int BType;
	int CType;
	NURBSS *pS;
//	CURVE *pB;
//	CURVE *pC;
	CURVE pB;
	CURVE pC;
	int pref;
	int pD;

	CONPS() {
		crtn = 0;
		SType = 0;
		BType = 0;
		CType = 0;
		pS = NULL;
		pB.substitution = NULL;
		pC.substitution = NULL;
		pref = 0;
		pD = 0;
	}
};

// Class TRMS
// �g�����ʒ�`�N���X
class TRMS
{
public:
	TRMS() {
		pts = NULL;
		n1 = 0;
		n2 = 0;
		pTO = NULL;
		pTI = NULL;
		pD = 0;
	}

    // Function: GetOuterEdgeNum
    // �g�����ʂ��\������O���G�b�W�̐����擾����
    int GetOuterEdgeNum();

    // Function: GetInnerTrmNum
    // �g�����ʂ��\����������g�����̐����擾����
    int GetInnerTrmNum();

    // Function: GetInnerEdgeNum
    // �g�����ʂ��\����������g�������\������G�b�W�̐����擾����
    int GetInnerEdgeNum(int);

    // Function: GetOuterCompC
    // �g�����ʂ��\������O���g�����Ȑ�(�����Ȑ�)�ւ̃|�C���^���擾����
    COMPC *GetOuterCompC();

    // Function: GetInnerCompC
    // �g�����ʂ��\����������g�����Ȑ�(�����Ȑ�)�ւ̃|�C���^���擾����
    COMPC *GetInnerCompC(int);

    // Funciton: GetNurbsS
    // �g�����ʂ��\������NURBS�Ȗʂւ̃|�C���^�𓾂�
    NURBSS *GetNurbsS();

public:
    // Variable: *pts
    // �g���������Surface Entity��DE���ւ̃|�C���^
    NURBSS *pts;

    // Variable: n1
    // 0:�O����D�̋��E�ƈ�v�A1:����ȊO
    int n1;

    // Variable: n2
    // Trimmed Surface�̓����ɂ�����P���Ȑ��̐�
    int n2;

    // Variable: *pTO
    // Trimmed Surface�̊O���ɂ�����P���Ȑ��\���̂ւ̃|�C���^
    CONPS *pTO;

    // Variable: **pTI
    // Trimmed Surface�̓����ɂ�����P���Ȑ��\���̂ւ̃|�C���^
    CONPS **pTI;

    // Variable: pD
    // �f�B���N�g�����ւ̋t�|�C���^
    int pD;
};

// Typedef: TRMS
// TRIMD_NURBSS - �g�����ʂɑ΂���Nurbs�Ȗʂ�z�N�����閼�̂�^���Ă���
typedef TRMS TRIMD_NURBSS;	// �g�����ʂɑ΂���Nurbs�Ȗʂ�z�N�����閼�̂�^���Ă���

// Structure: OBJECT
// �s�b�N���ꂽ�I�u�W�F�N�g�������\����
//
// Variables:
// int Body -	BODY�I�u�W�F�N�g�̔ԍ�
// int Type -	�G���e�B�e�B�^�C�v�̃V���{��(NURBS�Ȑ�:126 , NURBS�Ȗ�:128 , �g������:144)
// int Num -	Type�ɂ�����v�f�ԍ�(NURBS�Ȑ���4�{��������A����4�{�Ɋ��蓖�Ă�ꂽ0�`3�̔ԍ�)
// int CCount - ���ԖڂɃs�b�N���ꂽ�Ȑ�����\��
// int SCount - ���ԖڂɃs�b�N���ꂽ�Ȗʂ���\��
struct OBJECT
{
	int Body;		// BODY�I�u�W�F�N�g�̔ԍ�
	int Type;		// �G���e�B�e�B�^�C�v�̃V���{��(NURBS�Ȑ�:126 , NURBS�Ȗ�:128 , �g������:144)
	int Num;		// Type�ɂ�����v�f�ԍ�(NURBS�Ȑ���4�{��������A����4�{�Ɋ��蓖�Ă�ꂽ0�`3�̔ԍ�)
	int CCount;		// ���ԖڂɃs�b�N���ꂽ�Ȑ�����\��
	int SCount;		// ���ԖڂɃs�b�N���ꂽ�Ȗʂ���\��
};

#include "NURBS_Func.h"

// Class: BODY
// �S�ẴG���e�B�e�B�𓝊�����BODY�N���X
class BODY
{
public:
	// Constructor: BODY
	// BODY�N���X�̃R���X�g���N�^�D�e�평����
	BODY();
	~BODY();

	// Function: NewBodyElem
	// BODY���\������S�v�f�̃������[�m��
	void NewBodyElem();				

	// Function: DelBodyElem
	// BODY���\������S�v�f�̃������[���
	void DelBodyElem();				

	// Function: DelBodyElem
	// �w�肵��TypeNum[]���̃������[�����
	void DelBodyElem(int []);		

	// Function: NewCirA
	// CIRA���w�肵���������������[�m�ۂ��C����������
	CIRA *NewCirA(int);	

	// Function: NewCompC
	// COMPC���w�肵���������������[�m�ۂ��C����������
	COMPC *NewCompC(int);

	// Function: NewConA
	// CONA���w�肵���������������[�m�ۂ��C����������
	CONA *NewConA(int);	

	// Function: NewLine
	// LINE_���w�肵���������������[�m�ۂ��C����������
	LINE_ *NewLine(int);

	// Function: NewTMat
	// TMAT���w�肵���������������[�m�ۂ��C����������
	TMAT *NewTMat(int);		

	// Function: NewNurbsC
	// NURBSC���w�肵���������������[�m�ۂ��C����������
	NURBSC *NewNurbsC(int);			

	// Function: NewNurbsS
	// NURBSS���w�肵���������������[�m�ۂ��C����������
	NURBSS *NewNurbsS(int);			
	
	// Function: NewConpS
	// CONPS���w�肵���������������[�m�ۂ��C����������
	CONPS *NewConpS(int);			

	// Function: NewTrmS
	// TRMS���w�肵���������������[�m�ۂ��C����������
	TRMS *NewTrmS(int);				
	
	// Function: RotBody
	// BODY�̉�]
	void RotBody(Coord,double);	

	// Function: ShiftBody
	// BODY�̃V�t�g
	void ShiftBody(Coord);		

	// Function: ExpandBody
	// BODY�̊g��k��
	void ExpandBody(Coord);		

	// Function: RegistBody
	// ������V����BODY�Ƃ��ēo�^����
	void RegistBody(BODYList *,const char []);		

	// Function: DeleteBody
	// �������g����������
	void DeleteBody(BODYList *);	

	// Function: RegistNurbsCtoBody
	// 1��NURBS�Ȑ���V����BODY�Ƃ��ēo�^����
//	void RegistNurbsCtoBody(BODYList *,NURBSC,const char []);	
	void RegistNurbsCtoBody(BODYList *,const NURBSC&,const char []);	

	// Function: RegistNurbsCtoBodyN
	// N��NURBS�Ȑ���V����BODY�Ƃ��ēo�^����
	void RegistNurbsCtoBodyN(BODYList *,NURBSC [],const char [],int);	

	// Function: RegistNurbsStoBody
	// 1��NURBS�Ȗʂ�V����BODY�Ƃ��ēo�^����
	void RegistNurbsStoBody(BODYList *,NURBSS,const char []);	

	// Function: RegistNurbsStoBodyN
	// N��NURBS�Ȗʂ�V����BODY�Ƃ��ēo�^����
	void RegistNurbsStoBodyN(BODYList *,NURBSS [],const char [],int);	

	// Function: ChangeStatColor
	// �G���e�B�e�B�̃X�e�[�^�X�Œ�`����Ă���F��ύX
	void ChangeStatColor(float *,float,float,float,float);	

	// Function: InitCurveColor
	// ���̐F�̏����l��^����
	void InitCurveColor(float *);	

	// Function: InitSurfaceColor
	// �ʂ̐F�̏����l��^����
	void InitSurfaceColor(float *);							

	// Function: GetNurbsCFromLine
	// �����G���e�B�e�B��NURBS�Ȑ��G���e�B�e�B�ւƕϊ�����
	int GetNurbsCFromLine(int,int);					

	// Function: GetNurbsCFromCirA
	// �~�E�~�ʃG���e�B�e�B��NURBS�Ȑ��G���e�B�e�B�ւƕϊ�����
	int GetNurbsCFromCirA(int,int);							

private:
	// Function: CirAToNurbsC_seg1
	// (private)�~�E�~�ʃG���e�B�e�B��1�Z�O�����g�̏ꍇ
	int CirAToNurbsC_seg1(int,int,Coord [],double);

	// Function: CirAToNurbsC_seg2
	// (private)�~�E�~�ʃG���e�B�e�B��2�Z�O�����g�̏ꍇ
	int CirAToNurbsC_seg2(int,int,Coord [],double);	

	// Function: CirAToNurbsC_seg3
	// (private)�~�E�~�ʃG���e�B�e�B��3�Z�O�����g�̏ꍇ
	int CirAToNurbsC_seg3(int,int,Coord [],double);	

	// Function: CirAToNurbsC_seg4
	// (private)�~�E�~�ʃG���e�B�e�B��4�Z�O�����g�̏ꍇ
	int CirAToNurbsC_seg4(int,int,Coord [],double);			

public:
	// Variable: *CirA
	// �~�E�~��
	CIRA  *CirA;		

	// Variable: *CompC
	// �����Ȑ�
	COMPC *CompC;

	// Variable: *ConA
	// �~���Ȑ�
	CONA  *ConA;

	// Variable: *Line
	// ����
	LINE_ *Line;	

	// Variable: *TMat
	// �ϊ��s��
	TMAT  *TMat;	

	// Variable: *NurbsC
	// NURBS�Ȑ�
	NURBSC *NurbsC;
//	boost::shared_array<NURBSC>	NurbsC;	// ���ꎩ�̂��X�}�[�g�|�C���^�ɂ��Ă�...

	// Variable: *NurbsS
	// NURBS�Ȗ�
	NURBSS *NurbsS;

	// Variable: *ConpS
	// �ʏ��
	CONPS *ConpS;		

	// Variable: *TrmS
	// �g������
	TRMS  *TrmS;	

	// Variable: TypeNum[ALL_ENTITY_TYPE_NUM]
	// BODY���\������e�G���e�B�e�B�̐����i�[�����z��
	int  TypeNum[ALL_ENTITY_TYPE_NUM];	

	// Variable: *Mesh
	// Half-Edge�\�����b�V��(���X�g�\���A���X�g�̐擪�A�h���X������)
	MESH  *Mesh;

	// Variable: MaxCoord
	// ���̂̐��@�̍ő�l(���̒l�ŏ����\���{��������)
	double MaxCoord;
	// Add by K.Magara
	Coord	minmaxCoord[2];		// NCVC�p [0]:min [1]:max
	void	ClearMinMaxCoord(void);
	// --

	// Variable: Name[FNAMEMAX]
	// BODY��
//	char Name[FNAMEMAX];	
	std::string		Name;

	// Variable: *Mom
	// ������������e(BodyList)�̃A�h���X
	Data *Mom;				
};

#endif
