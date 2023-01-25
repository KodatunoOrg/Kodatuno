#ifndef _STD_AFX_H_
#define _STD_AFX_H_

#define USE_WIDE_STUDIO

// �R�[���o�b�N�Ɋւ���Windows��UNIX�̌݊�����ۂ}�N��
#ifdef _MSC_VER
#ifndef CALLBACK
#define CALLBACK __stdcall
#endif
#define EXPORT extern "C" __declspec(dllexport)
#else
#ifndef CALLBACK
#define	CALLBACK
#endif
#define EXPORT extern "C"
#endif

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

// GLUT�w�b�_�t�@�C���̃C���N���[�h
// �eOS�ɍ��킹�ēK�؂�GLUT.h�̃t�@�C���ʒu���w�肵�Ă�������
#ifndef MacOS
#include <GL/glut.h>
#else
#include <GLUT/glut.h>
#endif

#ifdef USE_WIDE_STUDIO
#include "StdAfxWS.h"
#else
#include "StdAfx__.h"
#endif

#define KOD_ERR	-1					// ERROR�̃V���{��
#define KOD_FALSE	0				// �U�̃V���{��
#define KOD_TRUE	1				// �^�̃V���{��
#define KOD_DONE	2				// ���s�ς݂������V���{��
#define KOD_ONEDGE	2				// �_���G�b�W��ɂ��邱�Ƃ������V���{��
#define KOD_EQUAL	2				// ���l�̃V���{��
#define LOW_ACCURACY  0				// �ᐸ�x�̃V���{��
#define MID_ACCURACY  1				// ���ʐ��x�̃V���{��
#define HIGH_ACCURACY 2				// �����x�̃V���{��
#define FNAMEMAX	256				// �t�@�C�����̍ő啶����
#define PI 3.141592653589793		// �~����
#define APPROX_ZERO_L 1.0e-6		// �[���ƌ��Ȃ���l(�ᐸ�x)
#define APPROX_ZERO	1.0e-12			// �[���ƌ��Ȃ���l(���ʐ��x)
#define APPROX_ZERO_H 1.0e-16		// �[���ƌ��Ȃ���l(�����x)
#define LOOPCOUNTMAX	1000		// �����v�Z�񐔂̏���


typedef double **Matrix;	// double�`��2�����z���Matrix�Ƃ��Ē�`
typedef double *Vector;		// double�`��1�����z���Vector�Ƃ��Ē�`

// ���W�p�\���̂��`
typedef struct{
	double x,y,z;	// x,y,z���W�l
	double dmy;		// �ėp
}Coord;

// �����ϊ��s��p�\����
typedef struct{
	Coord Rot[3];	// ��]�s��
	Coord Trl;		// ���i����
}FRAME;

// �\�������p�\����
typedef struct{
	float Color[4];	// �F(r,g,b,?)
	// �\�������̒ǉ��͂����ɋL�q
}DispStat;

// �O���[�o���Ȋ֐��̒�` ///////////////////////////////////////////////////////////////////////////

// ��,3�����x�N�g�����Z
void InitCoord(Coord *);						// ���W�l�̏�����
void InitCoord(Coord *,int);					// �I�[�o�[���[�h
Coord InitCoord();								// �I�[�o�[���[�h
Coord AddCoord(Coord,Coord);					// ���W�l�̑����Z
Coord AddCoord(Coord,double);					// �I�[�o�[���[�h
Coord AddCoord(Coord,double,double,double);		// �I�[�o�[���[�h
Coord DivCoord(Coord,Coord);					// ���W�l�̊���Z
Coord DivCoord(Coord,double);					// �I�[�o�[���[�h
Coord DivCoord(Coord,double,double,double);		// �I�[�o�[���[�h
Coord MulCoord(Coord,Coord);					// ���W�l�̊|���Z(�x�N�g���̓���,�O�ςł͂Ȃ��̂Œ���)
Coord MulCoord(Coord,double);					// �I�[�o�[���[�h
Coord MulCoord(Coord,double,double,double);		// �I�[�o�[���[�h
Coord SubCoord(Coord,Coord);					// ���W�l�̈����Z
Coord SubCoord(Coord,double);					// �I�[�o�[���[�h
Coord SubCoord(Coord,double,double,double);		// �I�[�o�[���[�h
Coord SetCoord(Coord);							// ���W�l����
Coord SetCoord(double,double,double);			// �I�[�o�[���[�h
void CopyCoord(Coord *,int,Coord *);			// ���W�l�Q���R�s�[
int DiffCoord(Coord,Coord);						// ���W�l�������Ȃ�KOD_TRUE�A�قȂ��Ă���Ȃ�KOD_FALSE��Ԃ�
int DiffCoord(Coord,Coord,double);				// �I�[�o�[���[�h(���x�w��)
int DiffCoord2D(Coord,Coord);					// ���W�l�������Ȃ�KOD_TRUE�A�قȂ��Ă���Ȃ�KOD_FALSE��Ԃ�(2D)
int DiffCoord2D(Coord,Coord,double);			// �I�[�o�[���[�h(���x�w��)
Coord AbsCoord(Coord);							// ���W�l�̐�Βl��Ԃ�
int ZoroCoord(Coord);							// (0,0,0)�̏ꍇ��KOD_TRUE��Ԃ�
Coord NormalizeVec(Coord);						// 3�����x�N�g���𐳋K��(�P�ʃx�N�g����)
Coord NormalizeVec(double,double,double);		// �I�[�o�[���[�h
double CalcEuclid(Coord);						// ���[�N���b�h���������Ƃ߂�
double CalcEuclid2D(double,double);				// 2�������[�N���b�h���������Ƃ߂�
double CalcDistance(Coord,Coord);				// 2�_�Ԃ̃��[�N���b�h���������߂�
double CalcDistance2D(Coord,Coord);				// 2�������W��ł�2�_�Ԃ̃��[�N���b�h�������Z�o
double CalcEuclidPnts2D(Coord,Coord);			// 2�������W��ł�2�_�Ԃ̃��[�N���b�h�������Z�o
double CalcInnerProduct(Coord,Coord);			// ���ς����߂�
double CalcinnerProduct(Coord,double,double,double);	// �I�[�o�[���[�h
Coord CalcOuterProduct(Coord,Coord);			// �O�ς����߂�
double CalcVecAngle(Coord,Coord);				// 2�̃x�N�g���̂Ȃ��p�����߂�
double CalcVecAngle2D(Coord,Coord);				// 2�̃x�N�g���̂Ȃ��p�����߂�(2D����)
Coord CalcInterDivPt(Coord,Coord,double);		// 2�_�Ԃ̓����_�����߂�
Coord CalcOrthoProjection(Coord,Coord,Coord);	// �C�ӂ̓_��C�ӂ̕��ʂ֐��ˉe����
double CalcDistPtToPlane(Coord,Coord,Coord);	// �C�ӂ̓_����C�ӂ̕��ʂ܂ł̋��������߂�
double CalcScalarTriProduct(Coord,Coord,Coord);	// �X�J���[�O�d�ς����߂�
Coord CalcRotVec2D(Coord,double);				// �C�ӂ̃x�N�g������]�������x�N�g�������߂�(2D����)
Coord CalcRotVec(Coord,Coord,double);			// �C�ӂ̃x�N�g�������_��ʂ�C�ӎ�����ɉ�]�������x�N�g�������߂�(3D����)
Coord CalcNormalLine(Coord,Coord,Coord);		// �C�ӂ̓_����C�ӂ̒����։��낵���_�����߂�
Coord Arc_CP(Coord,Coord,double);				// �~�̒��S�_(vec[0])����~��ɐڂ���C�ӂ�2�{�̐ڐ��������_�ւ̃x�N�g��(���S�p0<��<��)
int IsPointInPolygon(Coord,Coord *,int);		// ���ړ_�̑��p�`���O����

// �����ϊ��s��A��]�s��̉��Z
Coord MulFrameCoord(double[][3],double[],Coord);	// �����ϊ��s��ƍ��W�l(3D�x�N�g��)�Ƃ̊|���Z
Coord MulFrameCoord(FRAME,Coord);				// �I�[�o�[���[�h 
FRAME MulFrame(FRAME ,FRAME);					// �����ϊ��s��̊|���Z
FRAME InvFrame(FRAME);							// �����ϊ��s��̋t�s��𓾂�
Coord RotToZYZEuler(Coord []);					// ��]�s���ZYZ�I�C���[�p�֕ϊ�
void InitFrame(FRAME *);						// FRAME�̏�����

// �������x�N�g���A�������s��̉��Z
void InitVector(Vector,int);					// 1�����z��̏�����(����:�x�N�g��,�z��)
void InitMatrix(Matrix,int,int);				// 2�����z��̏�����
void CopyVector(Vector,int,Vector);				// �x�N�g���̃R�s�[
void MulMxMx(Matrix,int,int,Matrix,int,int,Matrix);	// �s�񓯎m�̊|���Z
void MulMxVec(Matrix,int,int,Vector,int,Vector);	// �s��ƃx�N�g���̊|���Z
void MulMxVec(Matrix,int,int,Coord *,Coord *);	// �s��ƍ��W�l�x�N�g���̊|���Z
Coord MulMxCoord(Coord [],Coord);				// Coord�ŕ\�������3x3�s���Coord�x�N�g���Ƃ̊|���Z
Coord MulMxCoord(Matrix,Coord);					// 3x3�s���Coord�x�N�g���Ƃ̊|���Z
void TranMx(Matrix,int,int,Matrix);				// �]�u�s��𓾂�
void TranMx(Coord **,int,int,Coord **);			// �]�u�s��𓾂�(�I�[�o�[���[�h)
void TranMx(Coord [],Coord []);					// �]�u�s��𓾂�(�I�[�o�[���[�h)
double Gauss(int,Matrix,Vector,Vector);			// �A��1���������̉������߂�
double Gauss(int,Matrix,Coord *,Coord *);		// �A��1���������̉������߂�(�I�[�o�[���[�h)
void LU_Solver(int,Matrix,Vector,int *,Vector);	// LU�����̌��ʂ���A��1��������������
void LU_Solver(int,Matrix,Coord *,int *,Coord *);// LU�����̌��ʂ���A��1��������������(�I�[�o�[���[�h)
double LU(int,Matrix,int *);					// LU����
double MatInv(int,Matrix,Matrix);				// �t�s������߂�
double MatInv3(Matrix,Matrix);					// 3x3�̋t�s��
double MatInv2(Matrix,Matrix);					// 2x2�̋t�s��

// ���l�v�Z
double DegToRad(double degree);					// �p�x�P�ʂ�degree����radian��
double RadToDeg(double radian);					// �p�x�P�ʂ�radian����degree��
int CalcCubicEquation(double *,double *);		// 3���������̉������߂�
int CalcQuadraticEquation(double *,double *);	// 2���������̉������߂�
int CalcLinearEquation(double *,double *);		// 1���������̉������߂�
int nCr(int n,int r);							// 2���W��(nCr�̑g��������)�����߂�
int Factorial(int n);							// ���R��n�̊K������߂�

// �`��֘A
void DrawPoint(Coord,double,double,double []);			// �_��`��
void DrawPoints(Coord *,int,double,double,double []);	// �_�Q��`��
void DrawVector(Coord,Coord,double,double,double []);	// �x�N�g����`��
void DrawLine(Coord,Coord,double,double []);			// 2�_�Ԃɐ�����`��

// �������[�֘A
Matrix NewMatrix(int,int);			// 2����double�^�z��̃������[�m��
Vector NewVector(int);				// 1����double�^�z��̃������[�m��
void FreeMatrix(Matrix,int);		// 2����double�^�z��̃������[���
void FreeVector(Vector);			// 1����double�^�z��̃������[���
Coord *NewCoord1(int);				// 1����Coord�^�z��̃������[�m��
void FreeCoord1(Coord *);			// 1����Coord�^�z��̃������[���
Coord **NewCoord2(int,int);			// 2����Coord�^�z��̃������[�m��
void FreeCoord2(Coord **,int);		// 2����Coord�^�z��̃������[���

// ���̑�
double sgn(double);						// ��������
int CheckZero(double,int);				// �l��APPROX_ZERO�͈̔͂�0�ł��邩�`�F�b�N
int CheckRange(double,double,double,int);	// �w�肵���l���w�肵���͈͓��ł��邩���`�F�b�N
int CheckMag(double,double,int);		// 2�̒l�̑召��r 
void BubbleSort(int[],int);				// �o�u���\�[�g(��{�����@)
void BubbleSort(double[],int);			// �I�[�o�[���[�h
int QCmp(const void *,const void *);	// �N�C�b�N�\�[�g 

#endif
