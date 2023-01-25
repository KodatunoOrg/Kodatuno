// stdafx.h : �W���̃V�X�e�� �C���N���[�h �t�@�C���̃C���N���[�h �t�@�C���A�܂���
// �Q�Ɖ񐔂������A�����܂�ύX����Ȃ��A�v���W�F�N�g��p�̃C���N���[�h �t�@�C��
// ���L�q���܂��B
//

#pragma once

#include "targetver.h"

#ifdef _MSC_VER
#define WIN32_LEAN_AND_MEAN             // Windows �w�b�_�[����g�p����Ă��Ȃ����������O���܂��B
#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // �ꕔ�� CString �R���X�g���N�^�[�͖����I�ł��B
//#define _AFX_NO_MFC_CONTROLS_IN_DIALOGS         // �_�C�A���O���� MFC �R���g���[���̃T�|�[�g���폜���܂��B
#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // Windows �w�b�_�[����g�p����Ă��Ȃ����������O���܂��B
#endif
#define	NO_WARN_MBCS_MFC_DEPRECATION	// VS2015�`
#define _CRT_SECURE_NO_WARNINGS			// �Ƃ肠�����R���p�C����ʂ�
#include <afx.h>
#include <afxwin.h>         // MFC �̃R�A����ѕW���R���|�[�l���g
#endif

#define	GLEW_STATIC			// GLEW static link
#include <gl/glew.h>		// OpenGL Extention
#include <gl/wglew.h>
#include <gl/gl.h>			// OpenGL


// TODO: �v���O�����ɕK�v�Ȓǉ��w�b�_�[�������ŎQ�Ƃ��Ă�������


// GUI�C���^�[�t�F�[�X�Ɉˑ��������b�v�֐��W
// Wide Studio�o�[�W����
class GUI_Interface
{
public:
	int SetMessage(char *);					// �R���\�[���Ƀ��b�Z�[�W���o��
	int SetCommand(char *);					// �R���\�[���ɃR�}���h���o��
	void AddBodyNameToWin(const char *);		// �V���ɓo�^����BODY�̖��O���E�B���h�E���X�g�ɕ\������
};
extern GUI_Interface GuiIF;		// �N���XGUI_Interface�̃C���X�^���X���O���[�o���Ő錾


// Constants: General Defines
// KOD_ERR -					ERROR�̃V���{��(-1)
// KOD_FALSE -					�U�̃V���{��(0)
// KOD_TRUE -					�^�̃V���{��(1)
// KOD_DONE -					���s�ς݂������V���{��(2)
// KOD_ONEDGE -					�_���G�b�W��ɂ��邱�Ƃ������V���{��(2)
// KOD_LARGE -					a > b�@�̃V���{��(0)
// KOD_SMALL -					a < b�@�̃V���{��(1)
// KOD_EQUAL -					a = b�@�̃V���{��(2)
// KOD_LARGE_EQ -				a >= b�@�̃V���{��(3)
// KOD_SMALL_EQ -				a <= b�@�̃V���{��(4)
// LOW_LOW_ACCURACY -			��ᐸ�x�̃V���{��(-1)
// LOW_ACCURACY -				�ᐸ�x�̃V���{��(0)
// MID_ACCURACY -				���ʐ��x�̃V���{��(1)
// HIGH_ACCURACY -				�����x�̃V���{��(2)
// FNAMEMAX -					�t�@�C�����̍ő啶����(256)
// PI -							�~����(3.141592653589793)
// APPROX_ZERO_L_L -			�[���ƌ��Ȃ���l(��ᐸ�x)(1.0e-3)
// APPROX_ZERO_L -				�[���ƌ��Ȃ���l(�ᐸ�x)(1.0e-6)
// APPROX_ZERO -				�[���ƌ��Ȃ���l(���ʐ��x)(1.0e-9)
// APPROX_ZERO_H -				�[���ƌ��Ȃ���l(�����x)(1.0e-12)
// LOOPCOUNTMAX -				�����v�Z�񐔂̏���(10000)
// COORDINDEX -					3�������W�������C���f�b�N�X��(3)
// QUADINDEX -					3�����������W�������C���f�b�N�X��(4)
// CW -							���]�̃V���{��(0)
// CCW -						�t�]�̃V���{��(1)
#define KOD_ERR	-1
#define KOD_FALSE	0
#define KOD_TRUE	1
#define KOD_DONE	2
#define KOD_ONEDGE	2
#define KOD_LARGE	0
#define KOD_SMALL	1
#define KOD_EQUAL	2
#define KOD_LARGE_EQ 3
#define KOD_SMALL_EQ 4
#define LOW_LOW_ACCURACY -1
#define LOW_ACCURACY  0
#define MID_ACCURACY  1
#define HIGH_ACCURACY 2
#define FNAMEMAX	256
#define PAI 3.141592653589793
#define APPROX_ZERO_L_L 1.0e-3
#define APPROX_ZERO_L 1.0e-6
#define APPROX_ZERO	1.0e-9
#define APPROX_ZERO_H 1.0e-12
#define LOOPCOUNTMAX	10000
#define COORDINDEX 3
#define QUADINDEX 4
#define CW  0
#define CCW 1

// Typedef: double **
// Matrix - double�`��2�����z���Matrix�Ƃ��Ē�`
typedef double **Matrix;

// Typedef: double *
// Vector - double�`��1�����z���Vector�Ƃ��Ē�`
typedef double *Vector;

// Class: Coord 
// ���W�l�p�N���X���`
class Coord
{
public:

	// Variables: x,y,z,dmy
	// �O�������W�l(x, y, z)�y�сC�ėp�Ƃ���dmy��p��
	double x,y,z,dmy;

	// Operator: +
	// Coord���m�̑����Z(AddCoord())
	Coord operator +(Coord);

	// Oeprator: -
	// Coord���m�̈����Z(SubCoord())
	Coord operator -(Coord);	

	// Oeprator: *
	// Coord���m�̊|���Z(MulCoord())
	Coord operator *(Coord);

	// Oeprator: *
	// Coord��double�Ƃ̊|���Z�i�I�[�o�[���[�h�j
	Coord operator *(double);

	// Operator: /
	// Coord���m�̊���Z(DivCoord())
	Coord operator /(Coord);

	// Operator: /
	// Coord��double�Ƃ̊���Z�i�I�[�o�[���[�h�j
	Coord operator /(double);	// �I�[�o�[���[�h

	// Operator: &
	// Coord���m�̓���(CalcInnerProduct())
	double operator &(Coord);

	// Operator: &&
	// Coord���m�̊O��(CalcOuterProduct())
	Coord operator &&(Coord);

	// Add by K.Magara
	void	SetMinCoord(const Coord&);
	void	SetMaxCoord(const Coord&);
};


// Structure: FRAME
// �����ϊ��s��p�\����
//
// Variables:
// Coord Rot[COORINDEX] -	// ��]�s��
// Coord Trl -				// ���i����
typedef struct{
	Coord Rot[COORDINDEX];		
	Coord Trl;
}FRAME;


// Structure: DispStat
// �\�������p�\����
//
// Variables:
// float Color[4] -	// �F(r,g,b,?)
struct DispStat
{
	float Color[4];	
	DispStat() {
		Color[0] = Color[1] = Color[2] = Color[3] = 0.0f;
	}
	// �\�������̒ǉ��͂����ɋL�q
};


// Package: �O���[�o���Ȋ֐��̒�`

// Group: Functions(3�����x�N�g���􉽉��Z)

// Function: InitCoord
// ���W�l�̏�����
void InitCoord(Coord *);

// Function: InitCoord
// ���W�l�̏�����(�I�[�o�[���[�h)
void InitCoord(Coord *,int);				

// Function: InitCoord
// ���W�l�̏�����(�I�[�o�[���[�h)
Coord InitCoord();

// Function: AddCoord
// ���W�l�̑����Z
Coord AddCoord(Coord,Coord);					

// Function: AddCoord
// ���W�l�̑����Z(�I�[�o�[���[�h)
Coord AddCoord(Coord,double);					

// Function: AddCoord
//  ���W�l�̑����Z(�I�[�o�[���[�h)
Coord AddCoord(Coord,double,double,double);		

// Function: DivCoord
// ���W�l�̊���Z
Coord DivCoord(Coord,Coord);					

// Function: DivCoord
// ���W�l�̊���Z(�I�[�o�[���[�h)
Coord DivCoord(Coord,double);					

// Function: DivCoord
// ���W�l�̊���Z(�I�[�o�[���[�h)
Coord DivCoord(Coord,double,double,double);		

// Function: MulCoord
// ���W�l�̊|���Z(�x�N�g���̓���,�O�ςł͂Ȃ��̂Œ���)
Coord MulCoord(Coord,Coord);					

// Function: MulCoord
// ���W�l�̊|���Z(�I�[�o�[���[�h)
Coord MulCoord(Coord,double);					

// Function: MulCoord
// ���W�l�̊|���Z(�I�[�o�[���[�h)
Coord MulCoord(Coord,double,double,double);		

// Function: SubCoord
// ���W�l�̈����Z
Coord SubCoord(Coord,Coord);					

// Function: SubCoord
// ���W�l�̈����Z(�I�[�o�[���[�h)
Coord SubCoord(Coord,double);					

// Function: SubCoord
// ���W�l�̈����Z(�I�[�o�[���[�h)
Coord SubCoord(Coord,double,double,double);

// Function: SetCoord
// ���W�l����
Coord SetCoord(Coord);							

// Function: SetCoord
// ���W�l����(�I�[�o�[���[�h)
Coord SetCoord(double,double,double);			

// Function: CopyCoord
// ���W�l�Q���R�s�[
void CopyCoord(Coord *,int,Coord *);			

// Function: DiffCoord
// ���W�l�������Ȃ�KOD_TRUE�A�قȂ��Ă���Ȃ�KOD_FALSE��Ԃ�
int DiffCoord(Coord,Coord);						

// Function: DiffCoord
// ���W�l�������Ȃ�KOD_TRUE�A�قȂ��Ă���Ȃ�KOD_FALSE��Ԃ�(�I�[�o�[���[�h(���x�w��))
int DiffCoord(Coord,Coord,double);				

// Function: AbsCoord
// ���W�l�̐�Βl��Ԃ�
Coord AbsCoord(Coord);							

// Function: ZoroCoord
// (0,0,0)�̏ꍇ��KOD_TRUE��Ԃ�
int ZoroCoord(Coord);							

// Function: CalcEuclid
// ���[�N���b�h���������Ƃ߂�
double CalcEuclid(Coord);						

// Function: CalcDistance
// 2�_�Ԃ̃��[�N���b�h���������߂�
double CalcDistance(Coord,Coord);				

// Function: CalcRotVec
// �C�ӂ̃x�N�g�������_��ʂ�C�ӎ�����ɉ�]�������x�N�g�������߂�(3D����)
Coord CalcRotVec(Coord,Coord,double);			

// Function: CalcVecAngle
// 2�̃x�N�g���̂Ȃ��p�����߂�
double CalcVecAngle(Coord,Coord);				

// Function: CalcAnglePlaneVec
// ���ʂƒ����Ƃ̂Ȃ��p�����߂�
double CalcAnglePlaneVec(Coord,Coord);			

// Function: NormalizeVec
// 3�����x�N�g���𐳋K��(�P�ʃx�N�g����)
Coord NormalizeVec(Coord);						

// Function: NormalizeVec
// 3�����x�N�g���𐳋K��(�P�ʃx�N�g����)(�I�[�o�[���[�h)
Coord NormalizeVec(double,double,double);		

// Function: CalcInnerProduct
// ���ς����߂�
double CalcInnerProduct(Coord,Coord);			

// Function: CalcInnerProduct
// ���ς����߂�(�I�[�o�[���[�h)
double CalcInnerProduct(Coord,double,double,double);	

// Function: CalcOuterProduct
// �O�ς����߂�
Coord CalcOuterProduct(Coord,Coord);			

// Function: CalcInterDivPt
// 2�_�Ԃ̓����_�����߂�
Coord CalcInterDivPt(Coord,Coord,double);		

// Function: CalcOrthoProjection
// �C�ӂ̓_��C�ӂ̕��ʂ֐��ˉe����
Coord CalcOrthoProjection(Coord,Coord,Coord);	

// Function: CalcDistPtToPlane
// �C�ӂ̓_����C�ӂ̕��ʂ܂ł̋��������߂�
double CalcDistPtToPlane(Coord,Coord,Coord);	

// Function: CalcScalarTriProduct
// �X�J���[�O�d�ς����߂�
double CalcScalarTriProduct(Coord,Coord,Coord);	

// Function: CalcNormalLine
// �C�ӂ̓_����C�ӂ̒����։��낵���_�����߂�
Coord CalcNormalLine(Coord,Coord,Coord);		

// Function: Arc_CP
// �~�̒��S�_(vec[0])����~��ɐڂ���C�ӂ�2�{�̐ڐ��������_�ւ̃x�N�g��(���S�p0<��<��)
Coord Arc_CP(Coord,Coord,double);				

// Function: IsPointInPolygon
// ���ړ_�̑��p�`���O����
int IsPointInPolygon(Coord,Coord *,int);		

// Function: CalcNormVecFrom3Pts
// ��ԏ��3�_����Ȃ镽�ʂ̖@���x�N�g�������߂�
Coord CalcNormVecFrom3Pts(Coord,Coord,Coord);	


// Group: Functions(2�����x�N�g���􉽉��Z)

// Function: AddCoord2D
// ���W�l�̑����Z (2D Ver.)
Coord AddCoord2D(Coord,Coord);

// Function: AddCoord2D
// ���W�l�̑����Z(�I�[�o�[���[�h) (2D Ver.)
Coord AddCoord2D(Coord,double);					

// Function: AddCoord2D
// ���W�l�̑����Z(�I�[�o�[���[�h) (2D Ver.)
Coord AddCoord2D(Coord,double,double);			

// Function: DivCoord2D
// ���W�l�̊���Z (2D Ver.)
Coord DivCoord2D(Coord,Coord);					

// Function: DivCoord2D
// ���W�l�̊���Z(�I�[�o�[���[�h) (2D Ver.)
Coord DivCoord2D(Coord,double);					

// Function: DivCoord2D
// ���W�l�̊���Z(�I�[�o�[���[�h) (2D Ver.)
Coord DivCoord2D(Coord,double,double);			

// Function: MulCoord2D
// ���W�l�̊|���Z(�x�N�g���̓���,�O�ςł͂Ȃ��̂Œ���) (2D Ver.)
Coord MulCoord2D(Coord,Coord);					

// Function: MulCoord2D
// ���W�l�̊|���Z(�x�N�g���̓���,�O�ςł͂Ȃ��̂Œ���)(�I�[�o�[���[�h) (2D Ver.)
Coord MulCoord2D(Coord,double);					

// Function: MulCoord2D
// ���W�l�̊|���Z(�x�N�g���̓���,�O�ςł͂Ȃ��̂Œ���)(�I�[�o�[���[�h) (2D Ver.)
Coord MulCoord2D(Coord,double,double);			

// Function: SubCoord2D
// ���W�l�̈����Z (2D Ver.)
Coord SubCoord2D(Coord,Coord);					

// Function: SubCoord2D
// ���W�l�̈����Z(�I�[�o�[���[�h) (2D Ver.)
Coord SubCoord2D(Coord,double);					

// Function: SubCoord2D
// ���W�l�̈����Z(�I�[�o�[���[�h) (2D Ver.)
Coord SubCoord2D(Coord,double,double);			

// Function: SetCoord2D
// ���W�l���� (2D Ver.)
Coord SetCoord2D(Coord);						

// Function: SetCoord2D
// �I�[�o�[���[�h (2D Ver.)
Coord SetCoord2D(double,double);				

// Function: CopyCoord2D
// ���W�l�Q���R�s�[ (2D Ver.)
void CopyCoord2D(Coord *,int,Coord *);			

// Function: DiffCoord2D
// ���W�l�������Ȃ�KOD_TRUE�A�قȂ��Ă���Ȃ�KOD_FALSE��Ԃ� (2D Ver.)
int DiffCoord2D(Coord,Coord);					

// Function: DiffCoord2D
// ���W�l�������Ȃ�KOD_TRUE�A�قȂ��Ă���Ȃ�KOD_FALSE��Ԃ�(�I�[�o�[���[�h(���x�w��)) (2D Ver.)
int DiffCoord2D(Coord,Coord,double);			

// Function: AbsCoord2D
// ���W�l�̐�Βl��Ԃ� (2D Ver.)
Coord AbsCoord2D(Coord);						

// Function: ZoroCoord2D
// (0,0,0)�̏ꍇ��KOD_TRUE��Ԃ� (2D Ver.)
int ZoroCoord2D(Coord);							

// Function: CalcEuclid2D
// ���[�N���b�h���������Ƃ߂� (2D Ver.)
double CalcEuclid2D(double,double);				

// Function: CalcDistance2D
// 2�������W��ł�2�_�Ԃ̃��[�N���b�h�������Z�o (2D Ver.)
double CalcDistance2D(Coord,Coord);				

// Function: CalcVecAngle2D
// 2�̃x�N�g���̂Ȃ��p�����߂� (2D Ver.)
double CalcVecAngle2D(Coord,Coord);				

// Function: CalcRotVec2D
// �C�ӂ̃x�N�g������]�������x�N�g�������߂�(2D����)
Coord CalcRotVec2D(Coord,double);				

// Function: CalcOuterProduct2D
// �O�ς����߂� (2D Ver.)
double CalcOuterProduct2D(Coord,Coord);			

// Function: ClacPolygonArea2D
// 2D���ʏ�̑��p�`�̕����t���ʐς𓾂�
double ClacPolygonArea2D(Coord [],int);			

// Function: DiscriminateCW2D
// 2D���ʏ�̑��p�`�����v��肩�����v��肩�𔻕ʂ���
int DiscriminateCW2D(Coord [],int);				


// Group: Functions(�����ϊ��s��A��]�s��̉��Z)

// Function: MulFrameCoord
// �����ϊ��s��ƍ��W�l(3D�x�N�g��)�Ƃ̊|���Z
Coord MulFrameCoord(double[][3],double[],Coord);	

// Function: MulFrameCoord
// �����ϊ��s��ƍ��W�l(3D�x�N�g��)�Ƃ̊|���Z(�I�[�o�[���[�h)
Coord MulFrameCoord(FRAME,Coord);				 

// Function: MulFrame
// �����ϊ��s��̊|���Z
FRAME MulFrame(FRAME ,FRAME);					

// Function: InvFrame
// �����ϊ��s��̋t�s��𓾂�
FRAME InvFrame(FRAME);							

// Function: RotToZYZEuler
// ��]�s���ZYZ�I�C���[�p�֕ϊ�
Coord RotToZYZEuler(Coord []);					

// Function: InitFrame
// FRAME�̏�����
void InitFrame(FRAME *);						


// Group: Functions(�������x�N�g���A�������s��̉��Z)

// Function: InitVector
// 1�����z��̏�����(����:�x�N�g��,�z��)
void InitVector(Vector,int);					

// Function: InitMatrix
// 2�����z��̏�����
void InitMatrix(Matrix,int,int);				

// Function: CopyVector
// �x�N�g���̃R�s�[
void CopyVector(Vector,int,Vector);				

// Function: AddMxMx
// �s�񓯎m�̑����Z
void AddMxMx(Matrix,Matrix,Matrix,int,int);		

// Function: MulMxMx
// �s�񓯎m�̊|���Z
void MulMxMx(Matrix,int,int,Matrix,int,int,Matrix);	

// Function: MulMxVec
// �s��ƃx�N�g���̊|���Z
void MulMxVec(Matrix,int,int,Vector,int,Vector);	

// Function: MulMxVec
// �s��ƍ��W�l�x�N�g���̊|���Z
void MulMxVec(Matrix,int,int,Coord *,Coord *);	

// Function: MulMxCoord
// Coord�ŕ\�������3x3�s���Coord�x�N�g���Ƃ̊|���Z
Coord MulMxCoord(Coord [],Coord);				

// Function: MulMxCoord
// 3x3�s���Coord�x�N�g���Ƃ̊|���Z
Coord MulMxCoord(Matrix,Coord);					

// Function: TranMx
// �]�u�s��𓾂�
void TranMx(Matrix,int,int,Matrix);				

// Function: TranMx
// �]�u�s��𓾂�(�I�[�o�[���[�h)
void TranMx(Coord **,int,int,Coord **);			

// Function: TranMx
// �]�u�s��𓾂�(�I�[�o�[���[�h)
void TranMx(Coord [],Coord []);					

// Function: Gauss
// �A��1���������̉������߂�
double Gauss(int,Matrix,Vector,Vector);			

// Function: Gauss
// �A��1���������̉������߂�(�I�[�o�[���[�h)
double Gauss(int,Matrix,Coord *,Coord *);		

// Function: LU_Solver
// LU�����̌��ʂ���A��1��������������
void LU_Solver(int,Matrix,Vector,int *,Vector);	

// Function: LU_Solver
// LU�����̌��ʂ���A��1��������������(�I�[�o�[���[�h)
void LU_Solver(int,Matrix,Coord *,int *,Coord *);

// Function: LU
// LU����
double LU(int,Matrix,int *);					

// Function: MatInv
// �t�s������߂�
double MatInv(int,Matrix,Matrix);				

// Function: MatInv3
// 3x3�̋t�s��
double MatInv3(Matrix,Matrix);					

// Function: MatInv2
// 2x2�̋t�s��
double MatInv2(Matrix,Matrix);					


// Group: Functions(���l�v�Z)

// Function: DegToRad
// �p�x�P�ʂ�degree����radian��
double DegToRad(double degree);					

// Function: RadToDeg
// �p�x�P�ʂ�radian����degree��
double RadToDeg(double radian);					

// Function: CalcCubicEquation
// 3���������̉������߂�
int CalcCubicEquation(double *,double *);		

// Function: CalcQuadraticEquation
// 2���������̉������߂�
int CalcQuadraticEquation(double *,double *);	

// Function: CalcLinearEquation
// 1���������̉������߂�
int CalcLinearEquation(double *,double *);		

// Function: nCr
// 2���W��(nCr�̑g��������)�����߂�
int nCr(int n,int r);							

// Function: Factorial
// ���R��n�̊K������߂�
int Factorial(int n);							

// Function: Round
// �l�̌ܓ�
double Round(double);							


// Group: Functions(�`��֘A)

// Function: DrawPoint
// �_��`��
void DrawPoint(Coord,double,double,double []);			

// Function: DrawPoints
// �_�Q��`��
void DrawPoints(Coord *,int,double,double,double []);	

// Function: DrawVector
// �x�N�g����`��
void DrawVector(Coord,Coord,double,double,double []);	

// Function: DrawLine
// 2�_�Ԃɐ�����`��
void DrawLine(Coord,Coord,double,double []);			

// Function: SetColorStat
// �J���[�X�e�[�^�X��ύX
void SetColorStat(DispStat *ds,float r, float g, float b, float a=0.5);	

// Function: DrawSolidCone
// �l�p����`�悷��
void DrawSolidCone(double,double);		


// Group: Functions(�������[�֘A)

// Function: NewMatrix
// 2����double�^�z��̃������[�m��
Matrix NewMatrix(int,int);			

// Function: NewVector
// 1����double�^�z��̃������[�m��
Vector NewVector(int);				

// Function: FreeMatrix
// 2����double�^�z��̃������[���
void FreeMatrix(Matrix,int);		

// Function: FreeVector
// 1����double�^�z��̃������[���
void FreeVector(Vector);			

// Function: NewCoord1
// 1����Coord�^�z��̃������[�m��
Coord *NewCoord1(int);				

// Function: FreeCoord1
// 1����Coord�^�z��̃������[���
void FreeCoord1(Coord *);			

// Function: NewCoord2
// 2����Coord�^�z��̃������[�m��
Coord **NewCoord2(int,int);			

// Function: FreeCoord2
// 2����Coord�^�z��̃������[���
void FreeCoord2(Coord **,int);		

// Function: NewCoord3
// 3����Coord�^�z��̃������[�m��
Coord ***NewCoord3(int,int,int);	

// Function: FreeCoord3
// 3����Coord�^�z��̃������[���
void FreeCoord3(Coord ***,int,int);	


// Group: Functions(���̑�)

// Function: sgn
// ��������
double sgn(double);						

// Function: CheckZero
// �l��APPROX_ZERO�͈̔͂�0�ł��邩�`�F�b�N
int CheckZero(double,int);				

// Function: CheckRange
// �w�肵���l���w�肵���͈͓��ł��邩���`�F�b�N
int CheckRange(double,double,double,int);	

// Function: CheckMag
// 2�̒l�̑召��r 
int CheckMag(double,double,int);		

// Function: BubbleSort
// �o�u���\�[�g(��{�����@)
void BubbleSort(int[],int);				

// Function: BubbleSort
// �I�[�o�[���[�h
void BubbleSort(double[],int);			

// Function: QCmp
// �N�C�b�N�\�[�g 
int QCmp(const void *,const void *);	

// Function: Reverse
// Coord�z��̔��]����
void Reverse(Coord [],int);				

// Function: Reverse
// double�z��̔��]����
void Reverse(double [],int);			

// Function: CatCoord
// ����z��̌��ɐV���Ȕz����q����
int CatCoord(Coord [],Coord [],int,int,int);	

// Function: CheckTheSamePoints
// ����_����������
int CheckTheSamePoints(Coord *,int);	

// Function: CheckTheSamePoints
// ����_����������
int CheckTheSamePoints(double *,int);	

// Function: CheckTheSamePoints2D
// 2D���ʓ��̓���_����������
int CheckTheSamePoints2D(Coord *,int);	

// Function: CoordToArray
// Coord��double�z��ɑ��
void CoordToArray(Coord,double []);		

// Function: CoordToArray2D
// Coord��double�z��ɑ��
void CoordToArray2D(Coord,double []);	
