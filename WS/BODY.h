// BODY�̒�`

#ifndef _BODY_H_
#define _BODY_H_

#include "StdAfx.h"

#define ALL_ENTITY_TYPE_NUM	20		// �S�G���e�B�e�B�^�C�v�̐�
#define CTLPNUMMAX  256		// NURBS�ŗp������R���g���[���|�C���g�̐��̏��
#define KNOTNUMMAX  256		// NURBS�ŗp������m�b�g�V�[�P���X�̐��̏��
#define GEOMTRYELEM 0		// IGES�f�B���N�g����"Entity Use Flag"���A�􉽗v�f������
#define PARAMETRICELEM 5	// IGES�f�B���N�g����"Entity Use Flag"���A2D�p�����g���b�N�v�f������

typedef WSClistData BODYList;	// WideStudio���p�ӂ���f�[�^���X�g�̌^��BODYList�Ƃ��čēo�^
typedef WSClistData OBJECTList;	// WideStudio���p�ӂ���f�[�^���X�g�̌^��OBJECTList�Ƃ��čēo�^

// �G���e�B�e�B�^�C�v�̃V���{�����`
#define	CIRCLE_ARC					100		// �~/�~��
#define	COMPOSITE_CURVE				102		// �����Ȑ�
#define	CONIC_ARC					104		// �~���Ȑ�
#define	COPIOUS_DATA				106		// �L�ӓ_��
#define	PLANE						108		// ����
#define	LINE						110		// ����
#define	PARAMETRIC_SPLINE_CURVE		112		// �p�����g���b�N�X�v���C���Ȑ�
#define	PARAMETRIC_SPLINE_SURFACE	114		// �p�����g���b�N�X�v���C���Ȗ�
#define	POINT						116		// �_
#define	TRANSFORMATION_MATRIX		124		// �ϊ��s��
#define	NURBS_CURVE					126		// �L��B�X�v���C���Ȑ�
#define	NURBS_SURFACE				128		// �L��B�X�v���C���Ȗ�
#define	CURVE_ON_PARAMETRIC_SURFACE 142		// �ʏ��
#define	TRIMMED_SURFACE				144		// �g������
#define	SUBFIGURE_DEFINITION		308		// �q�}�̒�`
#define	ASSOCIATIVITY_INSTANCE		402		// �O���[�v
#define	DRAWING						404		// �}��
#define	PROPERTY					406		// �}�ʃT�C�Y
#define	SINGULAR_SUBFIGURE_INSTANCE 408		// �q�}�̎Q��
#define	VIEW						410		// ���ۖ�

// �G���e�B�e�B�^�C�v�̃V���{����enum�Œ�`
enum EntityType{
	_CIRCLE_ARC,					// �~�E�~��
	_COMPOSITE_CURVE,				// �����Ȑ�
	_CONIC_ARC,						// �~���Ȑ�
	_COPIOUS_DATA,					// �L�ӓ_��
	_PLANE,							// ����
	_LINE,							// ����
	_PARAMETRIC_SPLINE_CURVE,		// �p�����g���b�N�X�v���C���Ȑ�
	_PARAMETRIC_SPLINE_SURFACE,		// �p�����g���b�N�X�v���C���Ȗ�
	_POINT,							// �_
	_TRANSFORMATION_MATRIX,			// �ϊ��s��
	_NURBSC,						// �L��B�X�v���C���Ȑ�
	_NURBSS,						// �L��B�X�v���C���Ȗ�
	_CURVE_ON_PARAMETRIC_SURFACE,	// �ʏ��
	_TRIMMED_SURFACE,				// �g������
	_SUBFIGURE_DEFINITION,			// �q�}�̒�`
	_ASSOCIATIVITY_INSTANCE,		// �O���[�v
	_DRAWING,						// �}��
	_PROPERTY,						// �}�ʃT�C�Y
	_SINGULAR_SUBFIGURE_INSTANCE,	// �q�}�̎Q��
	_VIEW							// ���ۖ�
};

/*
 * �G���e�B�e�B�^�C�v���Ƃɍ\���̂��`
 * 9�̃G���e�B�e�B�^�C�v��ǂݍ��ݑΏۂƂ���(����ȊO�͓ǂݎ̂�)
 * �ǉ�����ꍇ�́A�ȉ��ɒǉ�����G���e�B�e�B�^�C�v�̍\���̂��`���Ă�������
 */

// �~�E�~��
typedef struct{
	double zt;		// Z�������̐[��
	Coord  cp[3];	// �~�E�~�ʂ̒��S�_�A�n�_�A�I�_
	double R;		// ���a
	double t[2];	// t[0]:�n�_�̊p�x t[1]:�I�_�̊p�x
	int EntUseFlag; // �f�B���N�g���� Entity Use Flag �̒l(0:�􉽗v�f 5:2D�p�����g���b�N�v�f)
	int pD;			// �f�B���N�g�����ւ̋t�|�C���^
	DispStat Dstat;	// �\�������i�Fr,g,b�j
}CIRA;

// �~���Ȑ�
typedef struct{
	double prop[6];	// �W��
	double zt;		// ZT���ʂ̒�`
	Coord  cp[2];	// �n�_�A�I�_
	int pD;			// �f�B���N�g�����ւ̋t�|�C���^
	DispStat Dstat;	// �\�������i�Fr,g,b�j
}CONA;

// ����
typedef struct{
	Coord cp[2];	// �n�_�A�I�_
	int EntUseFlag; // �f�B���N�g���� Entity Use Flag �̒l(0:�􉽗v�f 5:2D�p�����g���b�N�v�f)
	int pD;			// �f�B���N�g�����ւ̋t�|�C���^
	DispStat Dstat;	// �\�������i�Fr,g,b�j
}LINE_;

// �ϊ��}�g���b�N�X
typedef struct{
	double R[3][3];	// ��]�s��
	double T[3];	// ���i�x�N�g��
	int pD;			// �f�B���N�g�����ւ̋t�|�C���^
}TMAT;

// �L��B�X�v���C��(NURBS)�Ȑ�
typedef struct{
	int K;			// �R���g���[���|�C���g�̐�
	int M;			// �K��(=����+1)
	int N;			// �m�b�g�x�N�g���̐�
	int prop[4];	// �e�v���p�e�B
	double *T;		// �m�b�g�V�[�P���X�̒l A+1��
	double *W;		// Weight�̒l K+1��
	Coord *cp;		// �R���g���[���|�C���g K+1��
	double V[2];	// �p�����[�^�͈̔�
	Coord norm;		// �@���x�N�g��
	int EntUseFlag; // �f�B���N�g���� Entity Use Flag �̒l(0:�􉽗v�f 5:2D�p�����g���b�N�v�f)
	int pD;			// �f�B���N�g�����ւ̋t�|�C���^
	DispStat Dstat;	// �\�������i�Fr,g,b�j
}NURBSC;

// �L��B�X�v���C��(NURBS)�Ȗ�
typedef struct{
	int K[2];		// �R���g���[���|�C���g�̐�(u����,v����)
	int M[2];		// �K��(=����+1)
	int N[2];		// �m�b�g�x�N�g���̐�
	int prop[5];	// �p�����[�^
	double *S;		// u�����m�b�g�x�N�g���̒l A+1��
	double *T;		// v�����m�b�g�x�N�g���̒l B+1��
	double **W;		// Weight�̒l
	Coord  **cp;	// �R���g���[���|�C���g C��
	double U[2];	// u�����p�����[�^
	double V[2];	// v�����p�����[�^
	int pD;			// �f�B���N�g�����ւ̋t�|�C���^
	int TrmdSurfFlag;// ����NURBS�Ȗʂ��g�����ʂƂ��ČĂ΂�Ă���̂��A�Ɨ����đ��݂���̂��������t���O(�g������:KOD_TRUE  �Ɨ���:KOD_FALSE)
	DispStat Dstat;	// �\�������i�Fr,g,b,�j
}NURBSS;

// �����Ȑ����\���ł���Ȑ��Q�����p�̂Ő錾
typedef union{
	CIRA CirA;		// �~�E�~��
	CONA ConA;		// �~���Ȑ�
	LINE_ Line;		// ����
	NURBSC NurbsC;	// NURBS�Ȑ�
}COMPELEM;

// �����Ȑ�
typedef struct{
	int N;			// �\���v�f��
	int *DEType;	// �e�\���v�f�̃G���e�B�e�B�^�C�v
	COMPELEM **pDE;	// �e�\���v�f�̍\���̂ւ̃|�C���^
	int DegeFlag;	// �����Ȑ����k�ނ���2D�p�����g���b�N�Ȑ���\���t���O
	NURBSC DegeNurbs;// �����Ȑ����k�ނ���2D�p�����g���b�N�Ȑ��������ꍇ�ɏk�ނ��������邽�߂�NURBS�Ȑ�
	int pD;			// �f�B���N�g�����ւ̋t�|�C���^
}COMPC;

// �ʏ�����\���ł���Ȑ��Q�����p�̂Ő錾
typedef union{
	CIRA  CirA;		// �~�E�~��
	COMPC CompC;	// �����Ȑ�
	CONA  ConA;		// �~���Ȑ�
	NURBSC NurbsC;	// NURBS�Ȑ�
}CURVE;

// �ʏ��
typedef struct{
	int crtn;		// �ʏ�����ǂ̂悤�ɍ��ꂽ��������
	int SType;		// Surface S�̃G���e�B�e�B�^�C�v
	int BType;		// Curve B�̃G���e�B�e�B�^�C�v
	int CType;		// Curve C�̃G���e�B�e�B�^�C�v
	NURBSS *pS;		// Curve�����Surface�\���̂ւ̃|�C���^
	CURVE *pB;		// Surface S�̃p�����[�^��Ԃɂ�����Curve B�\���̂ւ̃|�C���^
	CURVE *pC;		// Curve C�\���̂ւ̃|�C���^
	int pref;		// ���葤�V�X�e���ō̂��Ă����\���������t���O
	int pD;			// �f�B���N�g�����ւ̋t�|�C���^
}CONPS;

// �g������
typedef struct{
	NURBSS *pts;	// �g���������Surface Entity��DE���ւ̃|�C���^
	int n1;			// 0:�O����D�̋��E�ƈ�v�A1:����ȊO
	int n2;			// Trimmed Surface�̓����ɂ�����P���Ȑ��̐�
	CONPS *pTO;		// Trimmed Surface�̊O���ɂ�����P���Ȑ��\���̂ւ̃|�C���^
	CONPS **pTI;	// Trimmed Surface�̓����ɂ�����P���Ȑ��\���̂ւ̃|�C���^
	int pD;			// �f�B���N�g�����ւ̋t�|�C���^
}TRMS;

typedef TRMS TRIMD_NURBSS;	// �g�����ʂɑ΂���Nurbs�Ȗʂ�z�N�����閼�̂�^���Ă���

// �p�X�����ΏۃG���e�B�e�B�������\����
typedef struct{
	int Body;		// BODY�I�u�W�F�N�g�̔ԍ�
	int Type;		// �G���e�B�e�B�^�C�v�̃V���{��(NURBS�Ȑ�:126 , NURBS�Ȗ�:128 , �g������:144)
	int Num;		// Type�ɂ�����v�f�ԍ�(NURBS�Ȑ���4�{��������A����4�{�Ɋ��蓖�Ă�ꂽ0�`3�̔ԍ�)
	int CCount;		// ���ԖڂɃs�b�N���ꂽ�Ȑ�����\��
	int SCount;		// ���ԖڂɃs�b�N���ꂽ�Ȗʂ���\��
}OBJECT;

// �S�ẴG���e�B�e�B�𓝊�����BODY�\����
typedef struct{
	CIRA  *CirA;		// �~�E�~��
	COMPC *CompC;		// �����Ȑ�
	CONA  *ConA;		// �~���Ȑ�
	LINE_ *Line;		// ����
	TMAT  *TMat;		// �ϊ��s��
	NURBSC *NurbsC;		// NRBS�Ȑ�
	NURBSS *NurbsS;		// NRBS�Ȗ�
	CONPS *ConpS;		// �ʏ��
	TRMS  *TrmS;		// �g������
	double MaxCoord;	// ���̂̐��@�̍ő�l(���̒l�ŏ����\���{��������)
	int  TypeNum[ALL_ENTITY_TYPE_NUM];	// BODY���\������e�G���e�B�e�B�̐����i�[�����z��
	char Name[FNAMEMAX];	// BODY��
}BODY;

class BODY_Handler
{
public:
	void InitCurveColor(float *);							// ���̐F�̏����l��^����
	void InitSurfaceColor(float *);							// �ʂ̐F�̏����l��^����
	void ChangeStatColor(float *,float,float,float,float);	// �G���e�B�e�B�̃X�e�[�^�X�Œ�`����Ă���F��ύX
	BODY *SearchBodyList(BODYList *,int);					// �Z���N�V��������Ă���BODY�ԍ��𓾂�
	void Init_body(BODY *);									// BODY������
	void RegistNurbsCtoBody(BODYList *,BODY *,NURBSC,char []);	// 1��NURBC�Ȑ���V����BODY�Ƃ��ēo�^����
	void RegistNurbsCtoBodyN(BODYList *,BODY *,NURBSC [],char [],int);	// N��NURBC�Ȑ���V����BODY�Ƃ��ēo�^����
	void RegistNurbsStoBody(BODYList *,BODY *,NURBSS,char []);	// 1��NURBS�Ȗʂ�V����BODY�Ƃ��ēo�^����
	void RegistNurbsStoBodyN(BODYList *,BODY *,NURBSS [],char [],int);	// N��NURBS�Ȗʂ�V����BODY�Ƃ��ēo�^����


	// �e�G���e�B�e�B�̃������[����֐�
	void New_body(BODY *,int []);				// �S�G���e�B�e�B�̃������[�m��
	void Free_body(BODY *,int []);				// �S�G���e�B�e�B�̃������[���
	int New_CompC(COMPC *,int);					// �����Ȑ��̃������[�m��
	void Free_CompC_1DArray(COMPC *,int);		// �����Ȑ��z��̃������[���
	void Free_CompC(COMPC *);					// �����Ȑ��̃������[���
	int New_NurbsC(NURBSC *,int,int);			// NURBS�Ȑ��̃������[�m��
	void Free_NurbsC_1DArray(NURBSC *,int);		// NURBS�Ȑ��z��̃������[���
	void Free_NurbsC(NURBSC *);					// NURBS�Ȑ��̃������[���
	int New_NurbsS(NURBSS *,int [],int []);		// NURBS�Ȗʂ̃������[�m��
	void Free_NurbsS_1DArray(NURBSS *,int);		// NURBS�Ȗʔz��̃������[���
	void Free_NurbsS(NURBSS *);					// NURBS�Ȗʂ̃������[���
	int New_TrmS(TRMS *,int);					// �g�����ʂ̃������[�m��
	void Free_TrmS_1DArray(TRMS *,int);			// �g�����ʔz��̃������[���
	void Free_TrmS(TRMS *);						// �g�����ʂ̃������[���

};

#endif
