#ifndef _STDAFX_H_
#define _STDAFX_H_

// �R�[���o�b�N�Ɋւ���Windows��UNIX�̌݊�����ۂ}�N��(���Ԃ�)
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
#include <GL/glut.h>

#define ERR		-1					// �U�̃V���{��
#define FALSE	1					// �U�̃V���{��
#define TRUE	0					// �^�̃V���{��
#define PI 3.141592653589793		// �~����
#define APPROX_ZERO	1.0e-8			// �[���ƌ��Ȃ���l

typedef double **Matrix;	// double�`��2�����z���Matrix�Ƃ��Ē�`
typedef double *Vector;		// double�`��1�����z���Vector�Ƃ��Ē�`

// ���W�p�\���̂��`
typedef struct{
	double x,y,z;
}Coord;

// �\�������p�\����
typedef struct{
	float Color[4];	// �F(r,g,b,?)
	// �\�������̒ǉ��͂����ɋL�q
}DispStat;


// �O���[�o���Ȋ֐��̒�` ///////////////////////////////////////////////////////////////////////////

// ��,3�����x�N�g��,�s�񉉎Z
void InitCoord(Coord *);			// ���W�l�̏�����
Coord AddCoord(Coord,Coord);		// ���W�l�̑����Z
Coord AddCoord(Coord,double);		// �I�[�o�[���[�h
Coord DivCoord(Coord,Coord);		// ���W�l�̊���Z
Coord DivCoord(Coord,double);		// �I�[�o�[���[�h
Coord MulCoord(Coord,Coord);		// ���W�l�̊|���Z(�x�N�g���̓���,�O�ςł͂Ȃ��̂Œ���)
Coord MulCoord(Coord,double);		// �I�[�o�[���[�h
Coord SubCoord(Coord,Coord);		// ���W�l�̈����Z
Coord SubCoord(Coord,double);		// �I�[�o�[���[�h
Coord SetCoord(Coord);					// ���W�l����
Coord SetCoord(double,double,double);	// �I�[�o�[���[�h
int DiffCoord(Coord,Coord);			// ���W�l�������Ȃ�TRUE�A�قȂ��Ă���Ȃ�FALSE��Ԃ�
void InitVector(Vector,int);		// 1�����z��̏�����(����:�x�N�g��,�z��)
void InitMatrix(Matrix,int,int);	// 2�����z��̏�����
Coord NormalizeVec(Coord);			// 3�����x�N�g���𐳋K��
double CalcEuclid(Coord);			// ���[�N���b�h���������Ƃ߂�
double CalcEuclidPnts2D(Coord,Coord);	// 2�������W��ł�2�_�Ԃ̃��[�N���b�h�������Z�o
double CalcEuclidPnts3D(Coord,Coord);	// 3�������W��ł�2�_�Ԃ̃��[�N���b�h�������Z�o
double CalcInnerProduct(Coord,Coord);	// ���ς����߂�
Coord CalcOuterProduct(Coord,Coord);	// �O�ς����߂�
double CalcScalarTriProduct(Coord,Coord,Coord);	// �X�J���[�O�d�ς����߂�
double CalcinnerProduct(Coord,double,double,double);	// �I�[�o�[���[�h
double DegToRad(double degree);			// �p�x�P�ʂ�degree����radian��
double RadToDeg(double radian);			// �p�x�P�ʂ�radian����degree��
Coord Arc_CP(Coord,Coord,double);		// �~�̒��S�_(vec[0])����~��ɐڂ���C�ӂ�2�{�̐ڐ��������_�ւ̃x�N�g��(���S�p0<��<��)
double Rot_Vec_Ang(Coord,Coord);		// �C�ӂ̃x�N�g������]�������Ƃ��̉�]�p�����߂�(2D����)
Coord Rot_Vec(Coord,double);			// �C�ӂ̃x�N�g������]�������x�N�g�������߂�(2D����)
Coord NormalVector(Coord,Coord);		// �Ȗʂ�u,v�����̐ڐ��x�N�g������@���x�N�g�������߂�
bool IsPointInPolygon(Coord,Coord *,int);	// ���ړ_�̑��p�`���O����
void MulMxMx(Matrix,int,int,Matrix,int,int,Matrix);	// �s�񓯎m�̊|���Z
void MulMxVec(Matrix,int,int,Vector,int,Vector);	// �s��ƃx�N�g���̊|���Z
Coord HomoTransform(double[][3],double[],Coord);	// �����ϊ�(��]+���i�ړ�)

// ���l�v�Z
double sgn(double);						// ��������
void BubbleSort(int[],int);				// �o�u���\�[�g(��{�����@)
void BubbleSort(double[],int);			// �I�[�o�[���[�h
int QCmp(const void *,const void *);	// �N�C�b�N�\�[�g 
int CalcCubicEquation(double *,double *);		// 3���������̉������߂�
int CalcQuadraticEquation(double *,double *);	// 2���������̉������߂�
int CalcLinearEquation(double *,double *);		// 1���������̉������߂�

// �`��֘A
void DrawPoint(Coord,double,double,double []);			// �_��`��
void DrawVector(Coord,Coord,double,double,double []);	// �x�N�g����`��

// �������[�֘A
Matrix NewMatrix(int,int);			// 2����double�^�z��̃������[�m��
void FreeMatrix(Matrix);			// 2����double�^�z��̃������[���
Coord **NewCoord(int,int);			// 2����Coord�^�z��̃������[�m��
void FreeCoord(Coord **);			// 2����Coord�^�z��̃������[���



#endif
