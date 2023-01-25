#include "StdAfx.h"

#if defined(_DEBUG) && defined(_MSC_VER)
#define new DEBUG_NEW
#endif

GUI_Interface GuiIF;		// �N���XGUI_Interface�̃C���X�^���X���O���[�o���Ő錾

// �R�}���h�v�����v�g�Ƀ��b�Z�[�W���o��
int GUI_Interface::SetMessage(char *mes)
{
#if defined(_DEBUG) && defined(_MSC_VER)
	AfxMessageBox((LPCTSTR)mes);
#else
	fprintf(stderr,"%s\n",mes);
#endif
	return 0;
}

// �V���ɓo�^����BODY�̖��O���E�B���h�E���X�g�ɕ\������
void GUI_Interface::AddBodyNameToWin(const char *name)
{
	// void
}



// Operator: +
// Coord���m�̑����Z(AddCoord())
Coord Coord::operator +(Coord a)
{
	return(AddCoord(*this,a));
}

// Operator: -
// Coord���m�̈����Z(SubCoord())
Coord Coord::operator -(Coord a)
{
	return(SubCoord(*this,a));
}

// Oeprator: *
// Coord���m�̊|���Z(MulCoord())
Coord Coord::operator *(Coord a)
{
	return(MulCoord(*this,a));
}

// Oeprator: *
// Coord��double�Ƃ̊|���Z�i�I�[�o�[���[�h�j
Coord Coord::operator *(double a)
{
	return(MulCoord(*this,a));
}

// Operator: /
// Coord���m�̊���Z(DivCoord())
Coord Coord::operator /(Coord a)
{
	return(DivCoord(*this,a));
}

// Operator: /
// Coord��double�Ƃ̊���Z�i�I�[�o�[���[�h�j
Coord Coord::operator /(double a)
{
	return(DivCoord(*this,a));
}

// Operator: &
// Coord���m�̓���(CalcInnerProduct())
double Coord::operator &(Coord a)
{
	return(CalcInnerProduct(*this,a));
}

// Operator: &&
// Coord���m�̊O��(CalcOuterProduct())
Coord Coord::operator &&(Coord a)
{
	return(CalcOuterProduct(*this,a));
}

// Add by K.Magara
void Coord::SetMinCoord(const Coord& a)
{
	if ( x > a.x )	x = a.x;
	if ( y > a.y )	y = a.y;
	if ( z > a.z )	z = a.z;
}
void Coord::SetMaxCoord(const Coord& a)
{
	if ( x < a.x )	x = a.x;
	if ( y < a.y )	y = a.y;
	if ( z < a.z )	z = a.z;
}
// --

// Function: InitCoord
// ���W�l�̏�����
// 
// Parameters:
// *a - ���������������W�l�̃|�C���^
void InitCoord(Coord *a)
{
	a->x = 0;
	a->y = 0;
	a->z = 0;
	a->dmy = 0;
}
// Function: InitCoord
// ���W�l�z��̏�����(�I�[�o�[���[�h)
// 
// Parameters:
// *a - ���������������W�l�z��
// n - �z��v�f��
void InitCoord(Coord *a,int n)
{
	for(int i=0;i<n;i++){
		a[i].x = 0;
		a[i].y = 0;
		a[i].z = 0;
		a[i].dmy = 0;
	}
}

// Function: InitCoord
// ���W�l�̏�����(�I�[�o�[���[�h)
// 
// Return:
// ���������ꂽ���W�l
Coord InitCoord()
{
	Coord a;
	InitCoord(&a);

	return a;
}

// Function: AddCoord
// ���W�l���m�̑����Z
//
// Parameter: 
// a, b - ���Z����2�̍��W�l
//
// Return:
// ���Z����(a.x+b.x, a.y+b.y, a.z+b.z)
Coord AddCoord(Coord a,Coord b)
{
	Coord ans;

	ans.x = a.x + b.x;
	ans.y = a.y + b.y;
	ans.z = a.z + b.z;

	return ans;
}
// Function: AddCoord
// ���W�l���m�̑����Z(�I�[�o�[���[�h)
//
// Parameter: 
// a - ���Z�������W�l
// b - ���Z����double�l(a.x, a.y, a.z���ꂼ���b�����Z�����)
//
// Return:
// ���Z����(a.x+b, a.y+b, a.z+b)
Coord AddCoord(Coord a,double b)
{
	Coord ans;

	ans.x = a.x + b;
	ans.y = a.y + b;
	ans.z = a.z + b;

	return ans;
}

// Function: AddCoord
// ���W�l���m�̑����Z(�I�[�o�[���[�h)
//
// Parameter: 
// a - ���Z�������W�l
// x,y,z - ���Z����double�l(a.x, a.y, a.z���ꂼ���x,y,z�����Z�����)
//
// Return:
// ���Z����(a.x+x, a.y+y, a.z+z)
Coord AddCoord(Coord a,double x,double y,double z)
{
	Coord ans;

	ans.x = a.x + x;
	ans.y = a.y + y;
	ans.z = a.z + z;

	return ans;
}

// Function: AddCoord2D
// ���W�l���m�̑����Z (2D Ver.)
//
// Parameter: 
// a, b - ���Z����2�̍��W�l
//
// Return:
// ���Z����(a.x+b.x, a.y+b.y)
Coord AddCoord2D(Coord a,Coord b)
{
	Coord ans;

	ans.x = a.x + b.x;
	ans.y = a.y + b.y;

	return ans;
}

// Function: AddCoord2D
// ���W�l���m�̑����Z(�I�[�o�[���[�h) (2D Ver.)
//
// Parameter: 
// a - ���Z�������W�l
// b - ���Z����double�l(a.x, a.y���ꂼ���b�����Z�����)
//
// Return:
// ���Z����(a.x+b, a.y+b)
Coord AddCoord2D(Coord a,double b)
{
	Coord ans;

	ans.x = a.x + b;
	ans.y = a.y + b;

	return ans;
}

// Function: AddCoord
// ���W�l���m�̑����Z(�I�[�o�[���[�h) (2D Ver.)
//
// Parameter: 
// a - ���Z�������W�l
// x,y,z - ���Z����double�l(a.x, a.y���ꂼ���x,y�����Z�����)
//
// Return:
// ���Z����(a.x+x, a.y+y)
Coord AddCoord2D(Coord a,double x,double y)
{
	Coord ans;

	ans.x = a.x + x;
	ans.y = a.y + y;

	return ans;
}

// Function: DivCoord
// ���W�l���m�̊���Z
//
// Parameter: 
// a,b - ���Z����2�̍��W�l
//
// Return:
// ���Z����(a.x/b.x, a.y/b.y, a.z/b.z)
Coord DivCoord(Coord a,Coord b)
{
	if(b.x == 0.0 || b.y == 0.0 || b.z == 0.0)
		return SetCoord(0.0,0.0,0.0);

	Coord ans;

	ans.x = a.x / b.x;
	ans.y = a.y / b.y;
	ans.z = a.z / b.z;

	return ans;
}

// Function: DivCoord
// ���W�l���m�̊���Z(�I�[�o�[���[�h)
//
// Parameter: 
// a - ���Z�������W�l
// b - ���Z����double�l
// 
// Return:
// ���Z����(a.x/b, a.y/b, a.z/b)
Coord DivCoord(Coord a,double b)
{
	if(b == 0.0)	return SetCoord(0.0,0.0,0.0);

	Coord ans;

	ans.x = a.x / b;
	ans.y = a.y / b;
	ans.z = a.z / b;

	return ans;
}

// Function: DivCoord
// ���W�l���m�̊���Z(�I�[�o�[���[�h)
//
// Parameter: 
// a - ���Z�������W�l
// b - ���Z����double�l
// 
// Return:
// ���Z����(a.x/x, a.y/y, a.z/z)
Coord DivCoord(Coord a,double x,double y,double z)
{
	if(x == 0.0 || y == 0.0 || z == 0.0)
		return SetCoord(0.0,0.0,0.0);

	Coord ans;

	ans.x = a.x/x;
	ans.y = a.y/y;
	ans.z = a.z/z;

	return ans;
}

// Function: DivCoord2D
// ���W�l���m�̊���Z (2D Ver.)
//
// Parameter: 
// a,b - ���Z����2�̍��W�l
//
// Return:
// ���Z����(a.x/b.x, a.y/b.y)
Coord DivCoord2D(Coord a,Coord b)
{
	if(b.x == 0.0 || b.y == 0.0)
		return SetCoord2D(0.0,0.0);

	Coord ans;

	ans.x = a.x / b.x;
	ans.y = a.y / b.y;

	return ans;
}

// Function: DivCoord2D
// ���W�l���m�̊���Z(�I�[�o�[���[�h) (2D Ver.)
//
// Parameter: 
// a - ���Z�������W�l
// b - ���Z����double�l
// 
// Return:
// ���Z����(a.x/b, a.y/b)
Coord DivCoord2D(Coord a,double b)
{
	if(b == 0.0)	return SetCoord2D(0.0,0.0);

	Coord ans;

	ans.x = a.x / b;
	ans.y = a.y / b;

	return ans;
}

// Function: DivCoord2D
// ���W�l���m�̊���Z(�I�[�o�[���[�h) (2D Ver.)
//
// Parameter: 
// a - ���Z�������W�l
// x,y - ���Z����double�l
// 
// Return:
// ���Z����(a.x/x, a.y/y)
Coord DivCoord2D(Coord a,double x,double y)
{
	if(x == 0.0 || y == 0.0)
		return SetCoord2D(0.0,0.0);

	Coord ans;

	ans.x = a.x/x;
	ans.y = a.y/y;

	return ans;
}

// Function: SubCoord
// ���W�l���m�̈����Z
//
// Parameter: 
// a,b - �����Z����2�̍��W�l
//
// Return:
// ���Z����(a.x-b.x, a.y-b.y, a.z-b.z)
Coord SubCoord(Coord a,Coord b)
{
	Coord ans;

	ans.x = a.x - b.x;
	ans.y = a.y - b.y;
	ans.z = a.z - b.z;

	return ans;
}

// Function: SubCoord
// ���W�l���m�̈����Z(�I�[�o�[���[�h)
//
// Parameter: 
// a - �����Z�������W�l
// b - �����Z����double�l
//
// Return:
// ���Z����(a.x-b, a.y-b, a.z-b)
Coord SubCoord(Coord a,double b)
{
	Coord ans;

	ans.x = a.x - b;
	ans.y = a.y - b;
	ans.z = a.z - b;

	return ans;
}

// Function: SubCoord
// ���W�l���m�̈����Z(�I�[�o�[���[�h)
//
// Parameter: 
// a - �����Z�������W�l
// x,x,z - �����Z����double�l
//
// Return:
// ���Z����(a.x-x, a.y-y, a.z-z)
Coord SubCoord(Coord a,double x,double y,double z)
{
	Coord ans;

	ans.x = a.x - x;
	ans.y = a.y - y;
	ans.z = a.z - z;

	return ans;
}

// Function: SubCoord2D
// ���W�l���m�̈����Z (2D Ver.)
//
// Parameter: 
// a,b - �����Z����2�̍��W�l
//
// Return:
// ���Z����(a.x-b.x, a.y-b.y)
Coord SubCoord2D(Coord a,Coord b)
{
	Coord ans;

	ans.x = a.x - b.x;
	ans.y = a.y - b.y;

	return ans;
}

// Function: SubCoord2D
// ���W�l���m�̈����Z(�I�[�o�[���[�h) (2D Ver.)
//
// Parameter: 
// a - �����Z�������W�l
// b - �����Z����double�l
//
// Return:
// ���Z����(a.x-b, a.y-b)
Coord SubCoord2D(Coord a,double b)
{
	Coord ans;

	ans.x = a.x - b;
	ans.y = a.y - b;

	return ans;
}

// Function: SubCoord2D
// ���W�l���m�̈����Z(�I�[�o�[���[�h) (2D Ver.)
//
// Parameter: 
// a - �����Z�������W�l
// x,y - �����Z����double�l
//
// Return:
// ���Z����(a.x-x, a.y-y)
Coord SubCoord2D(Coord a,double x,double y)
{
	Coord ans;

	ans.x = a.x - x;
	ans.y = a.y - y;

	return ans;
}

// Function: MulCoord
// ���W�l���m�̊|���Z
//
// Parameter: 
// a,b - �|���Z����2�̍��W�l
//
// Return:
// ���Z����(a.x*b.x, a.y*b.y, a.z*b.z)
Coord MulCoord(Coord a,Coord b)
{
	Coord ans;

	ans.x = a.x * b.x;
	ans.y = a.y * b.y;
	ans.z = a.z * b.z;

	return ans;
}

// Function: MulCoord
// ���W�l���m�̊|���Z(�I�[�o�[���[�h)
//
// Parameter: 
// a - �|���Z�������W�l
// b - �|���Z����double�l
// 
// Return:
// ���Z����(a.x*b, a.y*b, a.z*b)
Coord MulCoord(Coord a,double b)
{
	Coord ans;

	ans.x = a.x * b;
	ans.y = a.y * b;
	ans.z = a.z * b;

	return ans;
}

// Function: MulCoord
// ���W�l���m�̊|���Z(�I�[�o�[���[�h)
//
// Parameter: 
// a - �|���Z�������W�l
// x,y,z - �|���Z����double�l
// 
// Return:
// ���Z����(a.x*x, a.y*y, a.z*z)
Coord MulCoord(Coord a,double x,double y,double z)
{
	Coord ans;

	ans.x = a.x * x;
	ans.y = a.y * y;
	ans.z = a.z * z;

	return ans;
}

// Function: MulCoord2D
// ���W�l���m�̊|���Z (2D Ver.)
//
// Parameter: 
// a,b - �|���Z����2�̍��W�l
//
// Return:
// ���Z����(a.x*b.x, a.y*b.y)
Coord MulCoord2D(Coord a,Coord b)
{
	Coord ans;

	ans.x = a.x * b.x;
	ans.y = a.y * b.y;

	return ans;
}

// Function: MulCoord2D
// ���W�l���m�̊|���Z(�I�[�o�[���[�h) (2D Ver.)
//
// Parameter: 
// a - �|���Z�������W�l
// b - �|���Z����double�l
// 
// Return:
// ���Z����(a.x*b, a.y*b)
Coord MulCoord2D(Coord a,double b)
{
	Coord ans;

	ans.x = a.x * b;
	ans.y = a.y * b;

	return ans;
}

// Function: MulCoord2D
// ���W�l���m�̊|���Z(�I�[�o�[���[�h) (2D Ver.)
//
// Parameter: 
// a - �|���Z�������W�l
// x,y - �|���Z����double�l
// 
// Return:
// ���Z����(a.x*x, a.y*y)
Coord MulCoord2D(Coord a,double x,double y)
{
	Coord ans;

	ans.x = a.x * x;
	ans.y = a.y * y;

	return ans;
}

// Function: DiffCoord
// ���W�l��APPROX_ZERO�̐��x�œ����Ȃ�KOD_TRUE�A�قȂ��Ă���Ȃ�KOD_FALSE��Ԃ�
//
// Parameter: 
// a,b - ��r����2�̍��W�l
//
// Return:
// A==B: KOD_TRUE, A!=B: KOD_FALSE
int DiffCoord(Coord a,Coord b)
{
	if(fabs(a.x-b.x) <= APPROX_ZERO && fabs(a.y-b.y) <= APPROX_ZERO && fabs(a.z-b.z) <= APPROX_ZERO)
	//if(fabs(a.x-b.x) == 0 && fabs(a.y-b.y) == 0 && fabs(a.z-b.z) == 0)
		return KOD_TRUE;
	else
		return KOD_FALSE;
}

// Function: DiffCoord
// ���W�l���w��̐��x�œ����Ȃ�KOD_TRUE�A�قȂ��Ă���Ȃ�KOD_FALSE��Ԃ�(�I�[�o�[���[�h) 
//
// Parameter: 
// a,b - ��r����2�̍��W�l
// App - ���x�iAPPROX_ZERO_L_L, APPROX_ZERO_L, APPROX_ZERO, APPROX_ZERO_H����I���j
//
// Return:
// A==B: KOD_TRUE, A!=B: KOD_FALSE
int DiffCoord(Coord a,Coord b,double App)
{
	if(fabs(a.x-b.x) <= App && fabs(a.y-b.y) <= App && fabs(a.z-b.z) <= App)
		return KOD_TRUE;
	else
		return KOD_FALSE;
}

// Function: DiffCoord2D
// 2D���ʂł̍��W�l��APPROX_ZERO�̐��x�œ����Ȃ�KOD_TRUE�A�قȂ��Ă���Ȃ�KOD_FALSE��Ԃ�
//
// Parameter: 
// a,b - ��r����2�̍��W�l
//
// Return:
// A==B: KOD_TRUE, A!=B: KOD_FALSE
int DiffCoord2D(Coord a,Coord b)
{
	if(fabs(a.x-b.x) <= APPROX_ZERO && fabs(a.y-b.y) <= APPROX_ZERO)
	//if(fabs(a.x-b.x) == 0 && fabs(a.y-b.y) == 0 && fabs(a.z-b.z) == 0)
		return KOD_TRUE;
	else
		return KOD_FALSE;
}

// Function: DiffCoord2D
// 2D���ʂł̍��W�l���w��̐��x�œ����Ȃ�KOD_TRUE�A�قȂ��Ă���Ȃ�KOD_FALSE��Ԃ�(�I�[�o�[���[�h)
//
// Parameter: 
// a,b - ��r����2�̍��W�l
// App - ���x�iAPPROX_ZERO_L_L, APPROX_ZERO_L, APPROX_ZERO, APPROX_ZERO_H����I���j
//
// Return:
// A==B: KOD_TRUE, A!=B: KOD_FALSE
int DiffCoord2D(Coord a,Coord b,double App)
{
	if(fabs(a.x-b.x) <= App && fabs(a.y-b.y) <= App)
	//if(fabs(a.x-b.x) == 0 && fabs(a.y-b.y) == 0 && fabs(a.z-b.z) == 0)
		return KOD_TRUE;
	else
		return KOD_FALSE;
}

// Function: AbsCoord
// ���W�l�̐�Βl��Ԃ�
//
// Parameter: 
// a - ���W�l
//
// Return:
// x,y,z�e���W�̐�Βl��Ԃ�
Coord AbsCoord(Coord a)
{
	Coord ans;

	ans.x = fabs(a.x);
	ans.y = fabs(a.y);
	ans.z = fabs(a.z);

	return ans;
}

// Function: AbsCoord2D
// ���W�l�̐�Βl��Ԃ�(2D Ver.)
//
// Parameter: 
// a - ���W�l
//
// Return:
// x,y,z�e���W�̐�Βl��Ԃ�
Coord AbsCoord2D(Coord a)
{
	Coord ans;

	ans.x = fabs(a.x);
	ans.y = fabs(a.y);

	return ans;
}

// Function: SetCoord
// ���W�l��������
// 
// Parameter:
// a - ���������W�l
// 
// Return:
// ����a�̒l�����̂܂ܕԂ�
Coord SetCoord(Coord a)
{
	return a;
}

// Function: SetCoord
// ���W�l��������(�I�[�o�[���[�h)
// 
// Parameter:
// x,y,z - ���������W�l��v�f���ƂɎw��
// 
// Return:
// (x,y,z)�̒l��Coord�Ƃ��ĕԂ�
Coord SetCoord(double x,double y,double z)
{
	Coord ans;

	ans.x = x;
	ans.y = y;
	ans.z = z;

	return ans;
}

// Function: SetCoord2D
// ���W�l��������(2D Ver.)
// 
// Parameter:
// a - ���������W�l
// 
// Return:
// ����a�̒l�����̂܂ܕԂ�
Coord SetCoord2D(Coord a)
{
	return a;
}

// Function: SetCoord2D
// ���W�l��������(�I�[�o�[���[�h)(2D Ver.)
// 
// Parameter:
// x,y - ���������W�l��v�f���ƂɎw��
// 
// Return:
// (x,y)�̒l��Coord�Ƃ��ĕԂ�
Coord SetCoord2D(double x,double y)
{
	Coord ans;

	ans.x = x;
	ans.y = y;

	return ans;
}

// Function: CopyCoord
// ���W�l�Q�̃R�s�[(b<--a)
// 
// Parameter:
// *a - ���������W�l�z��
// n - �z��̗v�f��
// *b - ����������̍��W�l�z��
void CopyCoord(Coord *a,int n,Coord *b)
{
	for(int i=0;i<n;i++)
		b[i] = SetCoord(a[i]);
}

// Function: CopyCoord2D
// ���W�l�Q�̃R�s�[(b<--a)(2D Ver.)
// 
// Parameter:
// *a - ���������W�l�z��
// n - �z��̗v�f��
// *b - ����������̍��W�l�z��
void CopyCoord2D(Coord *a,int n,Coord *b)
{
	for(int i=0;i<n;i++)
		b[i] = SetCoord2D(a[i]);
}

// Function: ZoroCoord
// ���W�la��(0,0,0)�̂Ƃ�KOD_FALSE��Ԃ�
//
// Parameters:
// a - ���؂�����W�l
// KOD_TRUE: (0,0,0)�łȂ��D  KOD_FALSE: (0,0,0)
int ZoroCoord(Coord a)
{
	if(a.x == 0.0 && a.y == 0.0 && a.z == 0.0)
		return KOD_FALSE;

	return KOD_TRUE;
}

// Function: ZoroCoord2D
// ���W�la��(0,0)�̂Ƃ�KOD_FALSE��Ԃ�
//
// Parameters:
// a - ���؂�����W�l
// KOD_TRUE: (0,0)�łȂ��D  KOD_FALSE: (0,0)
int ZoroCoord2D(Coord a)
{
	if(a.x == 0.0 && a.y == 0.0)
		return KOD_FALSE;

	return KOD_TRUE;
}

// Function: NewVector
// double�^1�����z��̃������[�m��
//
// Parameters:
// len - �������[�m�ۂ���double�^�z��v�f��
//
// Return:
// �m�ۂ��ꂽdouble�^1�����z��ւ̃|�C���^�i�������[�m�ۂɎ��s�FNULL�j
Vector NewVector(int len)
{
	Vector a;
//	if((a = (double *)malloc(len*sizeof(double))) == NULL){
	a = new double[len];
	if ( !a ) {
        GuiIF.SetMessage("fail to allocate memoly");
		return NULL;
	}

	return a;
}

// Function: NewVector
// double�^2�����z��̃������[�m��
//
// Parameters:
// row, col - �������[�m�ۂ���double�^2�����z��̍s�C��̗v�f��
//
// Return:
// �m�ۂ��ꂽdouble�^2�����z��ւ̃|�C���^�i�������[�m�ۂɎ��s�FNULL�j
Matrix NewMatrix(int row,int col)
{
	int i;
	Matrix a;

//	if((a = (double **)malloc(row*sizeof(double *))) == NULL){
	a = new double*[row];;
	if ( !a ) {
		return NULL;
	}
	for(i=0;i<row;i++){
//		if((a[i] = (double *)malloc(col*sizeof(double))) == NULL){
		a[i] = new double[col];
		if( !a[i] ) {
//			while(--i>=0) free(a[i]);
//			free(a);
			while(--i>=0) delete[]	a[i];
			delete[]	a;
			return NULL;
		}
	}

	return a;
}

// Function: FreeVector
// double�^1�����z��̃������[���
//
// Parameter:
// a - �������[�������double�^1�����z��ւ̃|�C���^
void FreeVector(Vector a)
{
//	free(a);
	delete[]	a;
}

// Function: FreeMatrix
// double�^2�����z��̃������[���
//
// Parameter:
// a - �������[�������double�^2�����z��ւ̃|�C���^
// col - a�̍s�v�f��
void FreeMatrix(Matrix a,int col)
{
	for(int i=0;i<col;i++) {
//		free(a[i]);
		delete[]	a[i];
	}

//	free(a);
	delete[]	a;
}

// Function: NewCoord1
// 1����Coord�^�z��̃������[�m��
//
// Parameters:
// len - �������[�m�ۂ���Coord�^�z��v�f��
//
// Return:
// �m�ۂ��ꂽdouble�^1�����z��ւ̃|�C���^�i�������[�m�ۂɎ��s�FNULL�j
Coord *NewCoord1(int len)
{
	Coord *a;

//	if((a = (Coord *)malloc(len*sizeof(Coord))) == NULL){
	a = new Coord[len];
	if ( !a ) {
        GuiIF.SetMessage("fail to allocate memoly");
		return NULL;
	}

	return a;
}

// Function: NewCoord2
// 2����Coord�^�z��̃������[�m��
//
// Parameters:
// row, col - �������[�m�ۂ���Coord�^2�����z��̍s�C��̗v�f��
//
// Return:
// �m�ۂ��ꂽCoord�^2�����z��ւ̃|�C���^�i�������[�m�ۂɎ��s�FNULL�j
Coord **NewCoord2(int row,int col)
{
	int i;
	Coord **a;

//	if((a = (Coord **)malloc((row)*sizeof(Coord *))) == NULL){
	a = new Coord*[row];
	if ( !a ) {
        GuiIF.SetMessage("fail to allocate memoly");
		return NULL;
	}
	for(i=0;i<row;i++){
//		if((a[i] = (Coord *)malloc(col*sizeof(Coord))) == NULL){
		a[i] = new Coord[col];
		if ( !a[i] ) {
            GuiIF.SetMessage("fail to allocate memoly");
//			while(--i>=0) free(a[i]);
//			free(a);
			while(--i>=0) delete[]	a[i];
			delete[]	a;
			return NULL;
		}
	}

	return a;
}

// Function: NewCoord3
// 3����Coord�^�z��̃������[�m��
//
// Parameters:
// x, y, z - �������[�m�ۂ���Coord�^3�����z��̊e�z��v�f��
//
// Return:
// �m�ۂ��ꂽCoord�^3�����z��ւ̃|�C���^�i�������[�m�ۂɎ��s�FNULL�j
Coord ***NewCoord3(int x,int y,int z)
{
	int i,j;
	Coord ***a;

//	if((a = (Coord ***)malloc(x*sizeof(Coord **))) == NULL){
	a = new Coord**[x];
	if ( !a ) {
        GuiIF.SetMessage("fail to allocate memoly x");
		return NULL;
	}
	for(i=0;i<x;i++){
//		if((a[i] = (Coord **)malloc(y*sizeof(Coord *))) == NULL){
		a[i] = new Coord*[y];
		if( !a[i] ) {
            GuiIF.SetMessage("fail to allocate memoly y");
//			while(--i>=0) free(a[i]);
//			free(a);
			while(--i>=0) delete[]	a[i];
			delete[]	a;
			return NULL;
		}
		for(j=0;j<y;j++){
//			if((a[i][j] = (Coord *)malloc(z*sizeof(Coord))) == NULL){
			a[i][j] = new Coord[z];
			if ( !a[i][j] ) {
				GuiIF.SetMessage("fail to allocate memoly z");
//				while(--j>=0) free(a[i][j]);
//				while(--i>=0) free(a[i]);
//				free(a);
				while(--j>=0) delete[]	a[i][j];
				while(--i>=0) delete[]	a[i];
				delete[]	a;
				return NULL;
			}
		}
	}

	return a;
}

// Function: FreeCoord1
// 1����Coord�^�z��̃������[��� 
//
// Parameters:
// *a - �������1����Coord�^�z��ւ̃|�C���^
void FreeCoord1(Coord *a)
{
//	free(a);
	delete[]	a;
}

// Function: FreeCoord2
// 2����Coord�^�z��̃������[��� 
//
// Parameters:
// *a - �������2����Coord�^�z��ւ̃|�C���^
// col - a�̍s�v�f��
void FreeCoord2(Coord **a,int col)
{
	Coord **b;

	b=a;
	for(int i=0;i<col;i++){
//		free(b[i]);
		delete[]	b[i];
	}
//	free(a);
	delete[]	a;
}

// Function: FreeCoord3
// 3����Coord�^�z��̃������[��� 
//
// Parameters:
// *a - �������3����Coord�^�z��ւ̃|�C���^
// x,y - a�̍s,��v�f��
void FreeCoord3(Coord ***a,int x,int y)
{
	int i,j;

	for(i=0;i<x;i++){
		for(j=0;j<y;j++){
//			free(a[i][j]);
			delete[]	a[i][j];
		}
//		free(a[i]);
		delete[]	a[i];
	}
//	free(a);
	delete[]	a;
}

// Function: NormalizeVec
// �x�N�g���𐳋K������
//
// Parameters:
// a - ���K������O�����x�N�g��
//
// Return:
// ���K�����ꂽ�O�����x�N�g��
Coord NormalizeVec(Coord a)
{
	double len=0;

	len = CalcEuclid(a);

	return DivCoord(a,len);
}

// Function: NormalizeVec
// �x�N�g���𐳋K������(�I�[�o�[���[�h)
//
// Parameters:
// x,y,z - ���K������3�����x�N�g�����C(x,y,z)���W�l�Ŏw��
//
// Return:
// ���K�����ꂽ3�����x�N�g��
Coord NormalizeVec(double x,double y,double z)
{
	double len=0;
	Coord a = SetCoord(x,y,z);

	len = CalcEuclid(a);

	return DivCoord(a,len);
}

// Function: CalcEuclid
// ���[�N���b�h�������Z�o
//
// Parameters:
// a - 3�����x�N�g��
//
// Return:
// ���[�N���b�h����
double CalcEuclid(Coord a)
{
	return sqrt(a.x*a.x + a.y*a.y + a.z*a.z);
}

// Function: CalcEuclid2D
// 2�������[�N���b�h�������Z�o
//
// Parameters:
// a,b - 2�����x�N�g��(a,b)
//
// Return:
// ���[�N���b�h����
double CalcEuclid2D(double a,double b)
{
	return sqrt(a*a+b*b);
}

// Function: CalcDistance
// 2�_�Ԃ̃��[�N���b�h���������߂�
//
// Parameters:
// a,b - 2�_
//
// Return:
// 2�_�Ԃ̃��[�N���b�h����
double CalcDistance(Coord a,Coord b)
{
	return(CalcEuclid(SubCoord(a,b)));
}

// Function: CalcDistance2D
// 2�������W��ł�2�_�Ԃ̃��[�N���b�h�������Z�o
//
// Parameters:
// a,b - 2�_
//
// Return:
// 2�_�Ԃ̃��[�N���b�h����
double CalcDistance2D(Coord a,Coord b)
{
	return sqrt((a.x-b.x)*(a.x-b.x) + (a.y-b.y)*(a.y-b.y));
}

// Function: CalcInnerProduct
// ���ς����߂�
// 
// Parameters:
// a,b - 2��3�����x�N�g��
//
// Return:
// ����
double CalcInnerProduct(Coord a,Coord b)
{
	return(a.x*b.x+a.y*b.y+a.z*b.z);
}

// Function: CalcInnerProduct
// ���ς����߂�(�I�[�o�[���[�h)
// 
// Parameters:
// a - 3�����x�N�g��(Coord�Ŏw��)
// x,y,z - 3�����x�N�g��((x,y,z)���W�l�Ŏw��)
//
// Return:
// ����
double CalcInnerProduct(Coord a,double x,double y,double z)
{
	return(a.x*x+a.y*y+a.z*z);
}

// Function: CalcOuterProduct
// �O�ς����߂�
// 
// Parameters:
// a,b - 2��3�����x�N�g��
//
// Return:
// �O��
Coord CalcOuterProduct(Coord a,Coord b)
{
	Coord c;

	c.x = a.y*b.z - a.z*b.y;
	c.y = a.z*b.x - a.x*b.z;
	c.z = a.x*b.y - a.y*b.x;

	return c;
}

// Function: CalcOuterProduct2D
// �O�ς����߂� (2D Ver.)
//
// Parameters:
// a,b - 2��3�����x�N�g��
//
// Return:
// �O��
double CalcOuterProduct2D(Coord a,Coord b)
{
	return(a.x*b.y - a.y*b.x);
}

// Function: CalcVecAngle
// 2�̃x�N�g���̂Ȃ��p�����߂�(�Ԓl��rad)
//
// Parameters:
// a,b - 2��3�����x�N�g��
//
// Return:
// 2�̃x�N�g���̂Ȃ��p(rad)
double CalcVecAngle(Coord a,Coord b)
{
	double inn = CalcInnerProduct(a,b);
	double abs_a = CalcEuclid(a);
	double abs_b = CalcEuclid(b);

	return(acos(inn/abs_a/abs_b));
}

// Function: CalcInterDivPt
// 2�_p(t=0),q(t=1)��t(0�`1)�œ��������Ƃ��̓_�̍��W�����Ƃ߂�
//
// Parameters:
// p,q - 2��3�������W
// t - �������0-1�̊ԂŎw��D
// 
// Return:
// �����_���W
Coord CalcInterDivPt(Coord p,Coord q,double t)
{
	return(AddCoord(p,MulCoord(SubCoord(q,p),t)));
}

// Function: CalcOrthoProjection
// �C�ӂ̓_��C�ӂ̕��ʂ֐��ˉe����
//
// Parameters:
// p - �C�ӂ̕��ʏ�̓_
// n - �C�ӂ̕��ʂ̒P�ʖ@���x�N�g��
// q - ���ˉe�������_
//
// Return:
// ���ˉe���ꂽ�_�̍��W�l
Coord CalcOrthoProjection(Coord p,Coord n,Coord q)
{
	if(fabs(1-CalcEuclid(n)) > APPROX_ZERO){
        GuiIF.SetMessage("ERROR:Norm vector is not unit vector.");
        GuiIF.SetMessage("Norm vetor is resized to unit vector.");
		NormalizeVec(n);
	}
	double inn = CalcInnerProduct(SubCoord(q,p),n);
	return (SubCoord(q,MulCoord(n,inn)));
}

// Function: CalcDistPtToPlane
// �C�ӂ̓_����C�ӂ̕��ʂ܂ł̋��������߂�
//
// Parameters:
// Pt - �C�ӂ̓_  
// P0 - ���ʏ��1�_  
// N - ���ʂ̖@���x�N�g��
//
// Return:
// �v�Z����
double CalcDistPtToPlane(Coord Pt,Coord P0,Coord N)
{
	return((fabs(N.x*Pt.x + N.y*Pt.y + N.z*Pt.z - (N.x*P0.x + N.y*P0.y + N.z*P0.z)))/CalcEuclid(N));
}

// Function: CalcScalarTriProduct
// �X�J���[�O�d�ς����߂�
//
// Parameters:
// a,b,c - 3��3�����x�N�g��
//
// Return:
// �X�J���[�O�d��
double CalcScalarTriProduct(Coord a,Coord b,Coord c)
{
	return(CalcInnerProduct(a,CalcOuterProduct(b,c)));
}

// Function: CalcAnglePlaneVec
// ���ʂƒ����Ƃ̂Ȃ��p�����߂�
//
// Parameters:
// a - �����̕����x�N�g��  
// n - ���ʂ̖@���x�N�g��
//
// Return:
// �v�Z����(radian)
double CalcAnglePlaneVec(Coord a,Coord n)
{
	return(PAI/2 - CalcVecAngle(a,n));
}

// Function: DrawPoint
// �_��`��iOpenGL�j
//
// Parameters:
// p - �_�̍��W�l  
// Scale - p��Scale�{����  
// Width - �_�̃T�C�Y  
// Color[3] - �_�̐F��RGB�Ŏw��@(0<= r,g,b <=1) 
void DrawPoint(Coord p,double scale,double width,double color[3])
{
	glDisable(GL_LIGHTING);
	glPointSize(width);
	glColor3f(color[0],color[1],color[2]);
	glBegin(GL_POINTS);
	glVertex3d(p.x*scale,p.y*scale,p.z*scale);
	glEnd();
	glEnable(GL_LIGHTING);
}

// Function: DrawPoints
// �_�Q��`��iOpenGL�j
//
// Parameters:
// *p - �_�Q�z��ւ̃|�C���^
// n - �_��
// scale -  p��Scale�{����
// width - �_�̃T�C�Y
// color[3] - �_�̐F��RGB�Ŏw��@(0<= r,g,b <=1) 
void DrawPoints(Coord *p,int n,double scale,double width,double color[3])
{
	glDisable(GL_LIGHTING);
	glPointSize(width);
	glColor3f(color[0],color[1],color[2]);
	glBegin(GL_POINTS);
	for(int i=0;i<n;i++){
		glVertex3d(p[i].x*scale,p[i].y*scale,p[i].z*scale);
	}
	glEnd();
	glEnable(GL_LIGHTING);
}

// Function: DrawVector
// �x�N�g����`��iOpenGL�j
//
// Parameters:
// s,e - 3�����x�N�g���̎n�_�ƏI�_���W
// vec_len - �\������x�N�g���̒����̔{��
// width - �`�悷������̑���
// color[3] - �_�̐F��RGB�Ŏw��@(0<= r,g,b <=1) 
void DrawVector(Coord s,Coord e,double vec_len,double width,double color[3])
{
	glDisable(GL_LIGHTING);
	glLineWidth(width);
	glColor3f(color[0],color[1],color[2]);
	e = MulCoord(e,vec_len);
	glBegin(GL_LINES);
	glVertex3d(s.x,s.y,s.z);
	glVertex3d(s.x+e.x,s.y+e.y,s.z+e.z);
	glEnd();
	glEnable(GL_LIGHTING);
}

// Function: DrawLine
// 2�_�Ԃɐ�����`��iOpenGL�j
//
// Parameters:
// s,e - �`�悷������̎n�_�ƏI�_���W
// width - �`�悷������̑���
// color[3] - �_�̐F��RGB�Ŏw��@(0<= r,g,b <=1) 
void DrawLine(Coord s,Coord e,double width,double color[3])
{
	glDisable(GL_LIGHTING);
	glLineWidth(width);
	glColor3f(color[0],color[1],color[2]);
	glBegin(GL_LINES);
	glVertex3d(s.x,s.y,s.z);
	glVertex3d(e.x,e.y,e.z);
	glEnd();
	glEnable(GL_LIGHTING);
}

// Function: DrawSolidCone
// �l�p����`�悷��
//
// Parameters:
// r - ���̒�ӂ̔��a
// h - ���̍���
void DrawSolidCone(double r, double h)
{
	double x[4],y[4];

	double drad = 2*PAI/4;

	for(int i=0;i<4;i++){
		double rad = (double)i*drad;
		x[i] = r*cos(rad);
		y[i] = r*sin(rad);
	}
	glBegin(GL_POLYGON);
	for(int i=0;i<4;i++){
		glVertex3d(x[i],y[i],0);
	}
	glEnd();

	glBegin(GL_TRIANGLE_FAN);
	glVertex3d(0,0,h);
	for(int i=0;i<4;i++){
		glVertex3d(x[i],y[i],0);

	}
	glEnd();

}

// Function: DegToRad
// �p�x��degree����radian�֕ϊ�
//
// Parameters:
// degree - degree
//
// Return:
// radian
double DegToRad(double degree)
{
	double radian;

	radian = degree * PAI / 180.0;

	return radian;	
}

// Function: RadToDeg
// �p�x��radian����degree�֕ϊ�
//
// Parameters:
// radian - radian
//
// Return:
// degree
double RadToDeg(double radian)
{
	double degree = 0.0;

	degree = radian * 180.0 / PAI;

	return degree;
}

// Function: Arc_CP
// �~�̒��S�_(vec[0])����~��ɐڂ���C�ӂ�2�{�̐ڐ��������_�ւ̃x�N�g��(���S�p0<��<��)
//
// Parameters:
// a - �~�ʂ��Ȃ��x�N�g��1  
// b - �~�ʂ��Ȃ��x�N�g��2  
// cos - ���S�p�̗]��
//
// Return:
// �v�Z����
Coord Arc_CP(Coord a, Coord b, double cos)
{
	Coord ans;

	ans.x = (a.x + b.x)/(1 + cos);
	ans.y = (a.y + b.y)/(1 + cos);
	ans.z = (a.z + b.z)/(1 + cos);

	return ans;
}

// Function: CalcVecAngle2D
// 2�̃x�N�g���̂Ȃ��p�����߂�(2D����)
// 
// Parameters:
// a,b - 2��2�����x�N�g��
//
// Return: 
// �v�Z����
double CalcVecAngle2D(Coord a, Coord b)
{
	double angle,sin,cos;

	sin = (a.x*b.y - b.x*a.y)/(a.x*a.x + a.y*a.y);
	cos = (a.x*b.x + a.y*b.y)/(a.x*a.x + a.y*a.y);

	angle = atan2(sin,cos);
	if(angle < 0) angle = angle + 2.0*PAI;

	return angle;
}

// Function: CalcRotVec2D
// �C�ӂ̃x�N�g������]�������x�N�g�������߂�(2D����)
// 
// Parameters:
// a - �C�ӂ�2�����x�N�g��
// angle - ��]�p�x(rad)
//
// Return:
// ��]���2�����x�N�g��
Coord CalcRotVec2D(Coord a, double angle)
{
	Coord ans;

	ans.x = a.x*cos(angle) - a.y*sin(angle);
	ans.y = a.x*sin(angle) + a.y*cos(angle);
	ans.z = a.z;

	return ans;
}

// Function: CalcRotVec
// �C�ӂ̃x�N�g�������_��ʂ�C�ӎ�����ɉ�]�������x�N�g�������߂�
//
// Parameters:
// a - ��]���������x�N�g��  
// e - ���_��ʂ�C�ӎ�(�P�ʃx�N�g���ł��邱��)  
// ang - ��]�p(rad)
//
// Return: 
// ��]��̃x�N�g��
Coord CalcRotVec(Coord a,Coord e,double ang)
{
	if(ang == 0.0)	return a;

	Coord ans;
	double ca = 1-cos(ang);

	ans.x = (e.x*e.x*ca+cos(ang))*a.x + (e.x*e.y*ca-e.z*sin(ang))*a.y + (e.x*e.z*ca+e.y*sin(ang))*a.z;
	ans.y = (e.x*e.y*ca+e.z*sin(ang))*a.x + (e.y*e.y*ca+cos(ang))*a.y + (e.y*e.z*ca-e.x*sin(ang))*a.z;
	ans.z = (e.x*e.z*ca-e.y*sin(ang))*a.x + (e.y*e.z*ca+e.x*sin(ang))*a.y + (e.z*e.z*ca+cos(ang))*a.z;

	return ans;
}

// Function: CalcNormalLine
// �C�ӂ̓_P����C�ӂ̒���(�_A��ʂ�P�ʃx�N�g��u�̕���������)�։��낵���_�����߂�
//
// Parameters:
// P - �C�ӂ̓_
// A - �C�ӂ̒�����̓_
// u - �C�ӂ̒����̒P�ʕ����x�N�g��
//
// Return:
// �v�Z����
Coord CalcNormalLine(Coord P,Coord A,Coord u)
{
	double k = CalcInnerProduct(SubCoord(P,A),u);

	return(AddCoord(A,MulCoord(u,k)));
}

// Function: BubbleSort
// int�^�z��̃o�u���\�[�g(��{�����@)
// 
// Parameters:
// array[] - �\�[�g����鐔�l�z��
// array_size - �z��v�f��
void BubbleSort(int array[],int array_size)
{
	int i,j,temp;

	for(i=0;i<array_size-1;i++){
		for(j=array_size-1;j>i;j--){
			if(array[j-1]>=array[j]) {
				temp = array[j];
				array[j] = array[j-1];
				array[j-1] = temp;
			}
		}
	}
}

// Function: BubbleSort
// double�^�z��̃o�u���\�[�g(��{�����@)(�I�[�o�[���[�h)
// 
// Parameters:
// array[] - �\�[�g����鐔�l�z��
// array_size - �z��v�f��
void BubbleSort(double array[],int array_size)
{
	int i,j;
	double temp;

	for(i=0;i<array_size-1;i++){
		for(j=array_size-1;j>i;j--){
			if(array[j-1]>=array[j]) {
				temp = array[j];
				array[j] = array[j-1];
				array[j-1] = temp;
			}
		}
	}
}

// Function: QCmp
// C����W���֐�qsort(double�^�A�~��)�ŗp�����r�֐�
//
// Parameters:
// *a,*b - ��r����double�^�̐��l
//
// Return:
// a<b�F1, a>b�F-1, a=b�F0
int QCmp(const void*a,const void*b)
{
	double *x,*y;

	x = (double*)a;
	y = (double*)b;

	if(*x < *y)
		return 1;
	else if(*x > *y)
		return -1;
	else
		return 0;
}

// Function: Reverse
// Coord�z��̔��]
// 
// Parameters:
// p[] - Coord�z��
// n - p�̗v�f��
void Reverse(Coord p[],int n)
{
	int i,j;
	Coord temp;

	for(i=0,j=n-1;i<j;i++,j--){
		temp = SetCoord(p[i]);
		p[i] = SetCoord(p[j]);
		p[j] = SetCoord(temp);
	}
}

// Function: Reverse
// double�^�z��̔��]����(�I�[�o�[���[�h)
//
// Parameters:
// p[] - double�z��
// n - p�̗v�f��
void Reverse(double p[],int n)
{
	int i,j;
	double temp;

	for(i=0,j=n-1;i<j;i++,j--){
		temp = p[i];
		p[i] = p[j];
		p[j] = temp;
	}
}

// Function: InitVector
// 1�����z��̏�����
// 
// Parameters:
// vec - 1�����z��ւ̃|�C���^
// size - �z��v�f��
void InitVector(Vector vec,int size)
{
	for(int i=0;i<size;i++){
		vec[i] = 0.0;
	}
}

// Function: InitMatrix
// 2�����z��̏�����
// 
// Parameters:
// mat - 2�����z��ւ̃|�C���^
// size_x,size_y - �s��v�f��
void InitMatrix(Matrix mat,int size_x,int size_y)
{
	for(int i=0;i<size_x;i++){
		for(int j=0;j<size_y;j++){
			mat[i][j] = 0.0;
		}
	}
}

// Function: CopyVector
// �x�N�g���̃R�s�[(a�x�N�g����b�x�N�g���֑��)
//
// Parameters:
// a - �R�s�[��1�����z��ւ̃|�C���^
// n - a�̗v�f��
// b - �R�s�[��1�����z��ւ̃|�C���^
void CopyVector(Vector a,int n,Vector b)
{
	for(int i=0;i<n;i++)
		b[i] = a[i];
}

// Function: CalcCubicEquation
// 3��������������
//
// Parameters:
// *p - 4�̌W��(a[0]x^3 + a[1]x^2 + a[2]x + a[3])   
// *ans - 3�̉�
//
// Return:
// ����3�Ƃ������̏ꍇ��3�A1���������̏ꍇ��1  a[0]==0�̏ꍇ��KOD_ERR
int CalcCubicEquation(double *p,double *ans)
{
	// x^3�̌W����0�̏ꍇ
	if(fabs(p[0]) < APPROX_ZERO_H){
		p[0] = p[1];
		p[1] = p[2];
		p[2] = p[3];
		return(CalcQuadraticEquation(p,ans));	// 2��������������
	}

	double a = p[0];
	double b = p[1];
	double c = p[2];
	double d = p[3];
	double x0[3];
	int k;
	int ansnum=0;

	double D = b*b-3*a*c;		// 1�K�������ꂽ2���������̔��ʎ�
	if(D<0){					// ���ʎ�����-->�ɒl����
		ansnum = 1;				// ����1��
		x0[0] = 1;				// �j���[�g���@�ł̏����l��1�Ɍ��ߑł�
	}
	else if(D==0.0){			// ���ʎ����[��-->�ϋɓ_��1����
		ansnum = 1;				// ����1��
		x0[0] = -b/3/a;			// �j���[�g���@�ł̏����l�͕ϋɓ_�Ƃ���
	}
	else {									// ���ʎ�����-->�ɑ�ɏ�����
		double x1 = (-b-sqrt(D))/(3*a);		// �ɓ_�����߂�
		double x2 = (-b+sqrt(D))/(3*a);
		if(x1 > x2){						// x1 < x2�Ƃ���
			double dmy = x1;
			x1 = x2;
			x2 = dmy;
		}
		double y1 = ((a*x1+b)*x1+c)*x1+d;	// x1�̂Ƃ���y1�����߂�
		double y2 = ((a*x2+b)*x2+c)*x2+d;	// x2�̂Ƃ���y2�����߂�
		if(y1*y2 < 0.0){					// y1��y2�̕������قȂ�ꍇ
			ansnum = 3;						// ����3����Ƃ�����
			x0[0] = x1 - 1;					// �j���[�g���@�̏����l���e��t�߂ƂȂ�悤�ݒ�
			x0[1] = (x1+x2)/2;
			x0[2] = x2 + 1;
		}
		else if(y1 == 0.0 || y2 == 0.0){	// y1,y2�ǂ��炩���[���̏ꍇ
			ansnum = 2;						// ����2��
			x0[0] = x1 - 1;					// �j���[�g���@�̏����l���e��t�߂ƂȂ�悤�ݒ�
			x0[1] = x2 + 1;
		}
		else{								// y1,y2���������̏ꍇ
			ansnum = 1;						// ����1��
			if(y1 < 0.0)					// ���������̏ꍇ
				x0[0] = x2 + 1;				// �j���[�g���@�̏����l��x2�ߖT�̂͂�
			else							// ���������̏ꍇ
				x0[0] = x1 - 1;				// �j���[�g���@�̏����l��x1�ߖT�̂͂�
		}
	}
	//fprintf(stderr,"ans num = %d\n",ansnum);

	// �j���[�g���@�ɂ�����T��
	double x,xold;
	double F,Df;
	for(int i=0;i<ansnum;i++){
		xold = x0[i];
		for(k=0;k<LOOPCOUNTMAX;k++){
			F = ((a*xold+b)*xold+c)*xold+d;
			Df = (3*a*xold+2*b)*xold+c;
			if(fabs(Df)<APPROX_ZERO_H){
				x=xold;
				break;
			}
			else{
				x = xold-F/Df;
				if(fabs(x-xold)<APPROX_ZERO_H)
					break;
				else
					xold = x;
			}
		}
		//F = ((a*x+b)*x+c)*x+d;
		//fprintf(stderr,"%d:(%lf,%lf)   x0=%lf    converge:%d\n",i,x,F,x0[i],k);
		ans[i] = x;
	}

	return ansnum;
}

// Function: CalcQuadraticEquation
// 2��������������
//
// Parameters:
// *a - 3�̌W��(a[0]x^2 + a[1]x + a[2])
// *ans - 2�̉�
//
// Return:
// ���������̏ꍇ��2�A�����̏ꍇ��KOD_ERR  a[0]==0�̏ꍇ��KOD_ERR
int CalcQuadraticEquation(double *a,double *ans)
{
	double Q,R;

	if(fabs(a[0]) < APPROX_ZERO_H){
		a[0] = a[1];
		a[1] = a[2];
		return(CalcLinearEquation(a,ans));
	}

	Q = a[1]*a[1] - 4*a[0]*a[2];

	if(Q<0){
		ans[0] = ans[1] = 0;
		return KOD_ERR;
	}

	else{
		R = -(a[1]+sgn(a[1])*sqrt(Q))/2;
		ans[0] = R/a[0];
		ans[1] = a[2]/R;
		return 2;
	}
}

// Function: CalcLinearEquation
// 1��������������
//
// Parameters:
// *a - 2�̌W��(a[0]x + a[1])  
// *ans - ��
// 
// Return:
// a[0]==0�̏ꍇ��KOD_ERR
int CalcLinearEquation(double *a,double *ans)
{
	if(fabs(a[0]) < APPROX_ZERO_H){
		return KOD_FALSE;
	}

	ans[0] = -a[1]/a[0];

	return 1;
}

// Function: sgn
// ��������
// 
// Parameters:
// x - �ΏۂƂ���double�l
//
// Return:
// x<0�F-1�Cx==0�F0, x>0�F1
double sgn(double x)
{
	if(x<0)	return -1;
	else if(x == 0) return 0;
	else return 1;
}

// Function: CheckZero
// �l��APPROX_ZERO�͈̔͂�0�ł��邩�`�F�b�N
//
// Parameters:
// val - ���͒l  
// flag - ���x(HIGH_ACCURACY or MID_ACCURACY or LOW_ACCURACY)
//
// -APPROX_ZERO < val < APPROX_ZERO
//
// Return:
// KOD_TRUE:�͈͓��Ń[���Ƃ݂Ȃ���     KOD_FALSE:�͈͊O     KOD_ERR:������flag�w�肪�Ԉ���Ă���
int CheckZero(double val,int flag)
{
	double ap;
	if(flag == LOW_ACCURACY)
		ap = APPROX_ZERO_L;
	else if(flag == HIGH_ACCURACY)
		ap = APPROX_ZERO_H;
	else if(flag == MID_ACCURACY)
		ap = APPROX_ZERO;
	else
		return KOD_ERR;

	if(fabs(val) < ap)
		return KOD_TRUE;

	return KOD_FALSE;
}

// Function: CheckRange
// �w�肵���l���w�肵���͈͓��ł��邩���`�F�b�N
//
// Parameters:
// low - ����  
// up - ���   
// val - ���ׂ����l
// flag - �`�F�b�N�^�C�v���ȉ����I��
// >flag = 0:(low <= val <= up) --> (low-ap < val < up+ap), 
// >       1:(low < val < up) --> (low+ap < val < up-ap),
// >       2:(val <= up) --> (val < up+ap),
// >       3:(val < up) --> (val < up-ap),
// >       4:(low <= val) --> (low-ap < val),
// >       5:(low < val) --> (low+ap < val)
// >���Ӂ@val��APPROX_ZERO(ap)����low�܂���up�ƈ�v����ꍇ�́A�͈͓��ɂ�����̂Ƃ���
//
// Return:
// KOD_TRUE:�͈͓��@�@KOD_FALSE:�͈͊O�@�@�@KOD_ERR:flag�Ɏw�肵���l��0,1�ȊO
int CheckRange(double low,double up,double val,int flag)
{
	if(flag < 0 || flag > 5){
        GuiIF.SetMessage("CheckRange ERROR:wrong specified value. 0 or 1");
		return KOD_ERR;
	}
	else if(flag == 0){
		if(val > low-APPROX_ZERO && val < up+APPROX_ZERO){
			return KOD_TRUE;
		}
	}
	else if(flag == 1){
		if(val > low+APPROX_ZERO && val < up-APPROX_ZERO){
			return KOD_TRUE;
		}
	}
	else if(flag == 2){
		if(val < up+APPROX_ZERO){
			return KOD_TRUE;
		}
	}
	else if(flag == 3){
		if(val < up-APPROX_ZERO){
			return KOD_TRUE;
		}
	}
	else if(flag == 4){
		if(val > low-APPROX_ZERO){
			return KOD_TRUE;
		}
	}
	else if(flag == 5){
		if(val > low+APPROX_ZERO){
			return KOD_TRUE;
		}
	}

	return KOD_FALSE;
}

// Function: CheckMag
// 2�̒l�̑召��r
//
// Parameters:
// val1,val2 - ���͒l   
// flag - ���x(HIGH_ACCURACY or or MID_ACCURACY or LOW_ACCURACY or LOW_LOW_ACCURACY)
// 
// Returns:
// KOD_EQUAL -  val1 = val2 (|va1-val2| < APPROX_ZERO)
// KOD_TRUE -   val1 > val2 ()
// KOD_FALSE -  val1 < val2 ()
int CheckMag(double val1,double val2,int flag)
{
	double ap;
	if(flag == LOW_ACCURACY)
		ap = APPROX_ZERO_L;
	else if(flag == HIGH_ACCURACY)
		ap = APPROX_ZERO_H;
	else if(flag == MID_ACCURACY)
		ap = APPROX_ZERO;
	else if(flag == LOW_LOW_ACCURACY)
		ap = APPROX_ZERO_L_L;
	else
		return KOD_ERR;

	double d = val1-val2;
	if(fabs(d) < ap)
		return KOD_EQUAL;
	else if(d < 0)
		return KOD_FALSE;
	else
		return KOD_TRUE;
}

// Function: IsPointInPolygon
// ���ړ_�̑��p�`���O����(x-y���ʓ�)
//
// Parameters:
// TargetPoint - ���ړ_  
// *BorderPoint - ���p�`�̒��_�Q�z��   
// CountPoint - ���_�̐�
// 
// Returns:
// KOD_TRUE:��  KOD_FALSE:�O  KOD_ONEDGE:�G�b�W��
int IsPointInPolygon(Coord TargetPoint,Coord *BorderPoint,int CountPoint)
{
	int i;
	int iCountCrossing = 0;				// ���O����J�E���^
	Coord p0;					// ���p�`�̈��(�x�N�g��)�̎n�_
	Coord p1;					// ���p�`�̈��(�x�N�g��)�̏I�_

	p0 = SetCoord(BorderPoint[0]);			// ���E�����[�v(���p�`)�̎n�_��p��
	bool bFlag0x = (TargetPoint.x <= p0.x);	// �Ώۓ_��x���W�Ƌ��E���̎n�_(���p�`�̈�ڂ̕ӂ̎n�_)��x���W�̑召��r
	bool bFlag0y = (TargetPoint.y <= p0.y);		// �Ώۓ_��y���W�Ƌ��E���̎n�_(���p�`�̈�ڂ̕ӂ̎n�_)��y���W�̑召��r

	// ���O���肷��_�ɑ΂��Ă��̓_����L�т锼�����ɂ����O������s��(�������̕����́A�w�v���X����)
	for(i=1;i<CountPoint+1;i++)
	{
		p1 = SetCoord(BorderPoint[i%CountPoint]);	// �Ō�͎n�_������i���p�`�f�[�^�̎n�_�ƏI�_����v���Ă��Ȃ��f�[�^�Ή��j

		// TargetPoint���G�b�W��(p0��p1�̐���)�ɂ��邩�`�F�b�N
		double a = (p1.x-p0.x)*(TargetPoint.x-p0.x) + (p1.y-p0.y)*(TargetPoint.y-p0.y);
		double L1 = CalcDistance2D(p1,p0);
		double L2 = CalcDistance2D(TargetPoint,p0);
		if(CheckZero(a-L1*L2,MID_ACCURACY) == KOD_TRUE && L1 >= L2){	// �G�b�W�ゾ����
			return KOD_ONEDGE;		// �ⓚ���p��return
		}
		bool bFlag1x = (TargetPoint.x <= p1.x);		
		bool bFlag1y = (TargetPoint.y <= p1.y);	

		if(bFlag0y != bFlag1y){			// �����͔����������؂�\������
			if(bFlag0x == bFlag1x){		// �����̂Q�[�_�͑Ώۓ_�ɑ΂��ė����E���������ɂ���
				if(bFlag0x){			// ���S�ɉE�A�����͔����������؂�
					iCountCrossing += (bFlag0y ? -1 : 1);	// �ォ�牺�ɔ����������؂�Ƃ��ɂ́A�����񐔂��P�����A�������͂P�����B
				}
			}
			else{					// �������ƌ������邩�ǂ����A�Ώۓ_�Ɠ��������ŁA�Ώۓ_�̉E�Ō������邩�A���Ō������邩�����߂�B
				if(TargetPoint.x <= (p0.x + (p1.x - p0.x)*(TargetPoint.y - p0.y )/(p1.y - p0.y))){	// �����́A�Ώۓ_�Ɠ��������ŁA�Ώۓ_�̉E�Ō�������B�����͔����������؂�
					iCountCrossing += (bFlag0y ? -1 : 1);	// �ォ�牺�ɔ����������؂�Ƃ��ɂ́A�����񐔂��P�����A�������͂P�����B
				}
			}
		}

		// ���̔���̂��߂̏���(�I�_���������̂����̕ӂ̎n�_��)
		p0 = p1;
		bFlag0x = bFlag1x;
		bFlag0y = bFlag1y;
	}

	// �N���X�J�E���g���[���̂Ƃ��O(KOD_FALSE)�A�[���ȊO�̂Ƃ���(KOD_TRUE)�B
	if(iCountCrossing)
		return KOD_TRUE;
	else
		return KOD_FALSE;
}

// Function: CalcNormVecFrom3Pts
// ��ԏ��3�_����Ȃ镽�ʂ̖@���x�N�g�������߂�
//
// Parameters:
// p1,p2,p3 - ��ԏ��3�_
//
// Return:
// �v�Z����
Coord CalcNormVecFrom3Pts(Coord p1,Coord p2,Coord p3)
{
	Coord denom = (p2-p1)&&(p3-p1);
	double numer = CalcEuclid(denom);

	return DivCoord(denom,numer);
}

// Function: CalcPolygonArea
// ��ԏ�̑��p�`�̖ʐς𓾂�
//
// Parameters:
// p[] - ���_��
// Vnum - ���_�̐�
//
// Return:
// �v�Z����
double CalcPolygonArea(Coord p[],int Vnum)
{
	double area=0;

	for(int i=0;i<Vnum;i++){
		area += CalcEuclid(CalcOuterProduct(p[i],p[(i+1)%Vnum]));
	}

	return(area/2);
}

// Function: ClacPolygonArea2D
// 2D���ʏ�̑��p�`�̕����t���ʐς𓾂�(CCW�F���CCW�F��)
//
// Parameters:
// p[] - ���_��
// Vnum - ���_�̐�
//
// Return:
// �v�Z����
double ClacPolygonArea2D(Coord p[],int Vnum)
{
	double area=0;

	for(int i=0;i<Vnum;i++){
		area += CalcOuterProduct2D(p[i],p[(i+1)%Vnum]);
	}

	return(area/2);
}

// Function: DiscriminateCW2D
// 2D���ʏ�̑��p�`�����v��肩�����v��肩�𔻕ʂ���
//
// Parameters:
// p[] - ���_��
// Vnum - ���_�̐�
//
// Return:
// CCW�FKOD_TRUE     CW�FKOD_FALSE
int DiscriminateCW2D(Coord p[],int Vnum)
{
	// �w��_����1�_�ȉ��̏ꍇ
	if(Vnum <= 2)
		return KOD_ERR;

	// �w��_����3�_�ȏ�̏ꍇ
	else{
		if(ClacPolygonArea2D(p,Vnum) > 0)	// CCW
			return CCW;

		else	// CW
			return CW;
	}

	return KOD_ERR;
}

// Function: AddMxMx
// �s�񓯎m�̑����Z
//
// Parameters:
// A,B,C - [C] = [A] +[B]
// row,col - �s�C��v�f��
void AddMxMx(Matrix A,Matrix B,Matrix C,int row,int col)
{
	for(int i=0;i<row;i++){
		for(int j=0;j<col;j++){
			C[i][j] = A[i][j] + B[i][j];
		}
	}
}

// Function: MulMxMx
// �s�񓯎m�̊|���Z
// 
// Parameters:
// A,B,C - [C] = [A][B]
// A_row, A_col - �s��A�̍s�C��v�f��
// B_row, B_col - �s��B�̍s�C��v�f��
void MulMxMx(Matrix A,int A_row,int A_col,Matrix B,int B_row,int B_col,Matrix C)
{
	for(int i=0;i<A_row;i++){
		for(int k=0;k<B_col;k++){
			C[i][k] = 0;
			if(A_col >= B_row){
				for(int j=0;j<B_row;j++){
					C[i][k] += A[i][j] * B[j][k];
				}
			}
			else{
				for(int j=0;j<A_col;j++){
					C[i][k] += A[i][j] * B[j][k];
				}
			}
		}
	}
}	

// Function: MulMxVec
// �s��ƃx�N�g���̊|���Z
// 
// Parameters:
// A,B,C - {C} = [A]{B}
// A_row - �s��  
// A_col - ��  
// B_row - �x�N�g���̎�����
void MulMxVec(Matrix A,int A_row,int A_col,Vector B,int B_row,Vector C)
{
	for(int i=0;i<A_row;i++){
		C[i] = 0;
		for(int j=0;j<A_col;j++){
			C[i] += A[i][j] * B[j];
		}
	}
}

// Function: MulMxVec
// �s��ƍ��W�l�x�N�g���̊|���Z(�I�[�o�[���[�h)
// >|A[0][0]     A[0][1] . .   A[0][col-1]  ||  B[0]  |
// >|A[1][0]     A[1][1] . .   A[1][col-1]  ||  B[1]  |
// >|   .           .    . .       .        ||    .   |
// >|   .           .    . .       .        ||    .   |
// >|A[row-1][0]    .    . . A[row-1][col-1]||B[row-1]|
//
// Parameters:
// A,B,C - {C} = [A]{B}
// A_row - �s��  
// A_col - ��  
// B_row - �x�N�g���̎�����
void MulMxVec(Matrix A,int A_row,int A_col,Coord *B,Coord *C)
{
	for(int i=0;i<A_row;i++){
		C[i] = SetCoord(0,0,0);
		for(int j=0;j<A_col;j++){
			C[i] = AddCoord(C[i],MulCoord(B[j],A[i][j]));
		}
	}
}

// Function: MulMxCoord
// Coord�ŕ\�������3x3�s���Coord�x�N�g���Ƃ̊|���Z
// >    |A[0].x A[1].x A[2].x|       |d.x|
// >A = |A[0].y A[1].y A[2].y| , d = |d.y|
// >    |A[0].z A[1].z A[2].z|       |d.z|
//
// Parameters:
// A[3] - Coord�\����3x3�s��
// d - Coord�\����3�����x�N�g��
//
// Return:
// �v�Z����
Coord MulMxCoord(Coord A[],Coord d)
{
	Coord ans;

	ans.x = A[0].x*d.x + A[1].x*d.y + A[2].x*d.z;
	ans.y = A[0].y*d.x + A[1].y*d.y + A[2].y*d.z;
	ans.z = A[0].z*d.x + A[1].z*d.y + A[2].z*d.z;

	return ans;
}

// Function: MulMxCoord
// Matrix�ŕ\�������3x3�s���Coord�x�N�g���Ƃ̊|���Z(�I�[�o�[���[�h)
// 
// Parameters:
// A - double�^2�����z��ւ̃|�C���^(3x3�s��)
// d - Coord�\����3�����x�N�g��
//
// Return:
// �v�Z����
Coord MulMxCoord(Matrix A,Coord d)
{
	Coord ans;

	ans.x = A[0][0]*d.x + A[0][1]*d.y + A[0][2]*d.z;
	ans.y = A[1][0]*d.x + A[1][1]*d.y + A[1][2]*d.z;
	ans.z = A[2][0]*d.x + A[2][1]*d.y + A[2][2]*d.z;

	return ans;
}

// Function: TranMx
// �]�u�s��𓾂�
// 
// Parameters:
// **A - ���̍s��  
// m - A�̍s��  
// n - A�̗�  
// **B - �]�u�s����i�[
//
// �]�u������m��n���t�ɂȂ�̂ŁAB�̃������[�m�ۂɒ���!
void TranMx(Matrix A,int m,int n,Matrix B)
{
	for(int i=0;i<m;i++){
		for(int j=0;j<n;j++){
			B[j][i] = A[i][j];
		}
	}
}

// Function: TranMx
// 2����Coord�z��ɂ���č\�������s��̓]�u�s��𓾂�(�I�[�o�[���[�h)
//
// Parameters:
// **A - ���̍s��  
// m - A�̍s��  
// n - A�̗�  
// **B - �]�u�s����i�[
//
// �]�u������m��n���t�ɂȂ�̂ŁAB�̃������[�m�ۂɒ���!
void TranMx(Coord **A,int m,int n,Coord **B)
{
	for(int i=0;i<m;i++){
		for(int j=0;j<n;j++){
			B[j][i] = SetCoord(A[i][j]);
		}
	}
}

// Function: TranMx
// 1����Coord�z��ɂ���č\�������s��̓]�u�s��𓾂�(�I�[�o�[���[�h)
// >             |A[0].x A[1].x A[2].x|                |B[0].x B[1].x B[2].x|   |A[0].x A[0].y A[0].z|
// >Coord A[3] = |A[0].y A[1].y A[2].y| , Coord B[3] = |B[0].y B[1].y B[2].y| = |A[1].x A[1].y A[1].z| = A^T
// >             |A[0].z A[1].z A[2].z|                |B[0].z B[1].z B[2].z|   |A[2].x A[2].y A[2].z|
//
// Parameters:
// A[3] - ���̍s��
// B[3] - �]�u�s����i�[
void TranMx(Coord A[],Coord B[])
{
	B[0].x = A[0].x;
	B[0].y = A[1].x;
	B[0].z = A[2].x;
	B[1].x = A[0].y;
	B[1].y = A[1].y;
	B[1].z = A[2].y;
	B[2].x = A[0].z;
	B[2].y = A[1].z;
	B[2].z = A[2].z;
}

// Function: MulFrameCoord
// �����ϊ��s��(R,T)�ƍ��W�l(I(3D�x�N�g��))�Ƃ̊|���Z
// 
// Parameters:
// R[][3] - �����ϊ��s��̉�]�s�񐬕�
// T[3] - �����ϊ��s��̕��i�x�N�g������
// I - ���W�l
// 
// Return: 
// �v�Z����
Coord MulFrameCoord(double R[][3],double T[],Coord I)
{
	Coord O;

	O.x = R[0][0]*I.x + R[0][1]*I.y + R[0][2]*I.z + T[0];
	O.y = R[1][0]*I.x + R[1][1]*I.y + R[1][2]*I.z + T[1];
	O.z = R[2][0]*I.x + R[2][1]*I.y + R[2][2]*I.z + T[2];

	return O;
}

// Function: MulFrameCoord
// �����ϊ��s��(f)�ƍ��W�l(I(3D�x�N�g��))�Ƃ̊|���Z(�I�[�o�[���[�h)
// 
// Parameters:
// f - �����ϊ��s��
// I - ���W�l
// 
// Return: 
// �v�Z����
Coord MulFrameCoord(FRAME f,Coord I)
{
	Coord O;

	O.x = f.Rot[0].x*I.x + f.Rot[1].x*I.y + f.Rot[2].x*I.z + f.Trl.x;
	O.y = f.Rot[0].y*I.x + f.Rot[1].y*I.y + f.Rot[2].y*I.z + f.Trl.y;
	O.z = f.Rot[0].z*I.x + f.Rot[1].z*I.y + f.Rot[2].z*I.z + f.Trl.z;

	return O;
}

// Function: InvFrame
// �����ϊ��s��F�̋t�s��𓾂�
// >F = |R T|    F^-1 = |R^-1 -R^-1*T|
// >    |0 1|           |  0     1   |
//
// Parameters:
// F - �����ϊ��s��
//
// Return:
// �v�Z����
FRAME InvFrame(FRAME F)
{
	FRAME f;

	TranMx(F.Rot,f.Rot);				// F.Rot�̓]�u�s��F.Rot^T�𓾂�
	f.Trl = MulMxCoord(f.Rot,F.Trl);	// F.Rot^T * F.Trl
	f.Trl = MulCoord(f.Trl,-1);			// -(F.Rot^T * F.Trl)

	return f;
}

// Function: MulFrame
// �����ϊ��s��̊|���Z
//
// Parameters:
// a,b - �����ϊ��s��
//
// Return:
// �v�Z����
FRAME MulFrame(FRAME a,FRAME b)
{
	FRAME f;

	f.Rot[0].x = a.Rot[2].x*b.Rot[0].z + a.Rot[1].x*b.Rot[0].y + a.Rot[0].x*b.Rot[0].x;
	f.Rot[1].x = a.Rot[2].x*b.Rot[1].z + a.Rot[1].x*b.Rot[1].y + a.Rot[0].x*b.Rot[1].x;
	f.Rot[2].x = a.Rot[2].x*b.Rot[2].z + a.Rot[1].x*b.Rot[2].y + a.Rot[0].x*b.Rot[2].x;
	f.Rot[0].y = a.Rot[2].y*b.Rot[0].z + a.Rot[1].y*b.Rot[0].y + a.Rot[0].y*b.Rot[0].x;
	f.Rot[1].y = a.Rot[2].y*b.Rot[1].z + a.Rot[1].y*b.Rot[1].y + a.Rot[0].y*b.Rot[1].x;
	f.Rot[2].y = a.Rot[2].y*b.Rot[2].z + a.Rot[1].y*b.Rot[2].y + a.Rot[0].y*b.Rot[2].x;
	f.Rot[0].z = a.Rot[2].z*b.Rot[0].z + a.Rot[1].z*b.Rot[0].y + a.Rot[0].z*b.Rot[0].x;
	f.Rot[1].z = a.Rot[2].z*b.Rot[1].z + a.Rot[1].z*b.Rot[1].y + a.Rot[0].z*b.Rot[1].x;
	f.Rot[2].z = a.Rot[2].z*b.Rot[2].z + a.Rot[1].z*b.Rot[2].y + a.Rot[0].z*b.Rot[2].x;
	f.Trl.x = a.Rot[2].x*b.Trl.z + a.Rot[1].x*b.Trl.y + a.Rot[0].x*b.Trl.x + a.Trl.x;
	f.Trl.y = a.Rot[2].y*b.Trl.z + a.Rot[1].y*b.Trl.y + a.Rot[0].y*b.Trl.x + a.Trl.y;
	f.Trl.z = a.Rot[2].z*b.Trl.z + a.Rot[1].z*b.Trl.y + a.Rot[0].z*b.Trl.x + a.Trl.z;

	return f;
}

// Function: RotToZYZEuler
// ��]�s�񂩂�ZYZ�I�C���[�p���Z�o�i tmp.x �����p(O), tmp.y �����p(A), tmp.z �����p(T) �ɂ��ꂼ��Ή� �j
//
// Prameters:
// rot[3] - Coord�\���̉�]�s��(rad)
//
// Return:
// �v�Z����(deg)
Coord RotToZYZEuler( Coord rot[])
{
	Coord tmp = SetCoord(0,0,0);

	tmp.y = atan2( sqrt( rot[0].z * rot[0].z + rot[1].z * rot[1].z ), rot[2].z );

	if( fabs( tmp.y ) <= APPROX_ZERO ){
		tmp.x = 0.0;
		tmp.z = atan2( -rot[1].x, rot[0].x );
	}
	else if( fabs( tmp.y - PAI ) <= APPROX_ZERO ){
		tmp.x = 0.0;
		//tmp.z = atan2( rot[1].x, -rot[0].x );		// ���X���������������A
		tmp.z = atan2( rot[1].x, rot[0].x );		// �}�C�i�X���Ȃ������B
	}
	else{
		tmp.x = atan2( rot[2].y / sin( tmp.y ), rot[2].x / sin( tmp.y ) );
		tmp.z = atan2( rot[1].z / sin( tmp.y ), -rot[0].z / sin( tmp.y ) );
	}	
	tmp = MulCoord(tmp,180/PAI);

	return tmp;
}

// Function: InitFrame
// FRAME�̏�����
//
// Parameters:
// *f - ����������FRAME�ւ̃|�C���^
void InitFrame(FRAME *f)
{
	f->Rot[0] = SetCoord(0,0,0);
	f->Rot[1] = SetCoord(0,0,0);
	f->Rot[2] = SetCoord(0,0,0);
	f->Trl = SetCoord(0,0,0);
}

// Function: Gauss
// �A��1���������̉������߂�
// 
// Parameters:
// n:�s��
// a,b,x - [a]{x}={b}�ŁA����x�ɓ���(b,x��double�^�z��)
//
// Return:
// �s��(�������[�G���[�FKOD_ERR)
double Gauss(int n,Matrix a,Vector b,Vector x)
{
	long double det;	// �s��
	int *ip;			// �s�����̏��

//	ip = (int *)malloc(sizeof(int)*n);
	ip = new int[n];
	if(ip == NULL){
        GuiIF.SetMessage("fail to allocate");
		return KOD_ERR;
	}

	det = LU(n,a,ip);					// LU����
	if(det == 0) return KOD_FALSE;		// �s�񎮂�0
	else LU_Solver(n,a,b,ip,x);	// LU�����̌��ʂ��g���ĘA��������������

//	free(ip);                   
	delete[]	ip;                   

	return det;					// �߂�l�͍s��
}

// Function: Gauss
// �A��1���������̉������߂�(�I�[�o�[���[�h)
// 
// Parameters:
// n:�s��
// a,b,x - [a]{x}={b}�ŁA����x�ɓ���(b,x��Coord�^�z��)
//
// Return:
// �s��(�������[�G���[�FKOD_ERR)
double Gauss(int n,Matrix a,Coord *b,Coord *x)
{
	long double det;	// �s��
	int *ip;			// �s�����̏��

//	ip = (int *)malloc(sizeof(int)*n);
	ip = new int[n];
	if(ip == NULL){
        GuiIF.SetMessage("fail to allocate");
		return KOD_ERR;
	}

	det = LU(n,a,ip);					// LU����
	if(det == 0) return KOD_FALSE;		// �s�񎮂�0
	else LU_Solver(n,a,b,ip,x);	// LU�����̌��ʂ��g���ĘA��������������

//	free(ip);                   
	delete[]	ip;                   

	return det;					// �߂�l�͍s��
}

// Function: LU_Solver
// LU�����̌��ʂ���A��1��������������
//
// Parameters:
// n - �s/��  
// a - n*n�̌W���s�� (����:�o�͂Ƃ���LU�������ꂽ���ʂ��i�[�����)
// b - n�����̉E�Ӄx�N�g��  
// ip - �s�����̏��
void LU_Solver(int n,Matrix a,Vector b,int *ip,Vector x)
{
	int ii;
	double t;

	for(int i=0;i<n;i++) {       // Gauss�����@�̎c��
		ii = ip[i];
		t = b[ii];
		for(int j=0;j<i;j++)
			t -= a[ii][j]*x[j];
		x[i] = t;
	}
	for(int i=n-1;i>=0;i--){  // ��ޑ��
		t = x[i];  
		ii = ip[i];
		for(int j=i+1;j<n;j++) 
			t -= a[ii][j]*x[j];
		x[i] = t/a[ii][i];
	}
}

// Function: LU_Solver
// LU�����̌��ʂ���A��1��������������(�I�[�o�[���[�h)
//
// Parameters:
// n - �s/��  
// a - n*n�̌W���s�� (����:�o�͂Ƃ���LU�������ꂽ���ʂ��i�[�����)
// b - n�����̉E��Coord�z��  
// ip - �s�����̏��
void LU_Solver(int n,Matrix a,Coord *b,int *ip,Coord *x)
{
	int ii;
	Coord t;

	for(int i=0;i<n;i++) {       // Gauss�����@�̎c��
		ii = ip[i];
		t = SetCoord(b[ii]);
		for(int j=0;j<i;j++)
			t = SubCoord(t,MulCoord(x[j],a[ii][j]));
		x[i] = SetCoord(t);
	}
	for(int i=n-1;i>=0;i--){  // ��ޑ��
		t = SetCoord(x[i]);  
		ii = ip[i];
		for(int j=i+1;j<n;j++) 
			t = SubCoord(t,MulCoord(x[j],a[ii][j]));
		x[i] = DivCoord(t,a[ii][i]);
	}
}

// Function: MatInv
// �t�s������߂�
//
// Parameters:
// n - �s(��)��	
// a - ���̍s��	
// a_inv - �s��a�̋t�s��
//
// Return:
// �s��(�������[�G���[�FKOD_ERR)
double MatInv(int n,Matrix a,Matrix a_inv)
{
	int i, j, k, ii;
	long double t, det;
	int *ip;		// �s�����̏��

//	ip = (int *)malloc(sizeof(int)*n);
	ip = new int[n];
	if (ip==NULL){
        GuiIF.SetMessage("fail to allocate");
		return KOD_ERR;
	}

	det = LU(n,a,ip);		// LU����
	if(det != 0){
		for(k=0;k<n;k++){
			for(i=0;i<n;i++){
				ii = ip[i];
				t = (ii==k);
				for(j=0;j<i;j++)
					t -= a[ii][j]*a_inv[j][k];
				a_inv[i][k] = t;
			}
			for(i=n-1;i>=0;i--){
				t = a_inv[i][k];
				ii = ip[i];
				for(j=i+1;j<n;j++)
					t -= a[ii][j]*a_inv[j][k];
				a_inv[i][k] = t/a[ii][i];
			}
		}
	}

//	free(ip);
	delete[]	ip;

	return det;
}

// Function: LU
// LU�������[�`��
// 
// Parameters:
// n - �s/��
// a - n*n�s�� (����:�o�͂Ƃ���LU�������ꂽ���ʂ��i�[�����)
// *ip - �s�����̏�񂪊i�[�����(n��int�z���p�ӂ��邱��) 
//
// Return:
// �s��
double LU(int n,Matrix a,int *ip)
{
	int i, j, k, ii, ik;
	long double t, u, det;
	Vector weight;

	weight = NewVector(n);    /* weight[0..n-1] �̋L���̈�m�� */
	det = 0;                   /* �s�� */
	for (k = 0; k < n; k++) {  /* �e�s�ɂ��� */
		ip[k] = k;             /* �s�������̏����l */
		u = 0;                 /* ���̍s�̐�Βl�ő�̗v�f�����߂� */
		for (j = 0; j < n; j++) {
			t = fabs(a[k][j]);  if (t > u) u = t;
		}
		if (u == 0){
			goto EXIT; /* 0 �Ȃ�s���LU�����ł��Ȃ� */
		}
		weight[k] = 1 / u;     /* �ő��Βl�̋t�� */
	}
	det = 1;                   /* �s�񎮂̏����l */
	for (k = 0; k < n; k++) {  /* �e�s�ɂ��� */
		u = -1;
		for (i = k; i < n; i++) {  /* ��艺�̊e�s�ɂ��� */
			ii = ip[i];            /* �d�݁~��Βl ���ő�̍s�������� */
			t = fabs(a[ii][k]) * weight[ii];
			if (t > u) {  u = t;  j = i;  }
		}
		ik = ip[j];
		if (j != k) {
			ip[j] = ip[k];  ip[k] = ik;  /* �s�ԍ������� */
			det = -det;  /* �s����������΍s�񎮂̕������ς�� */
		}
		u = a[ik][k];  det *= u;  /* �Ίp���� */
		if (u == 0){
			goto EXIT;    /* 0 �Ȃ�s���LU�����ł��Ȃ� */
		}
		for (i = k + 1; i < n; i++) {  /* Gauss�����@ */
			ii = ip[i];
			t = (a[ii][k] /= u);
			for (j = k + 1; j < n; j++)
				a[ii][j] -= t * a[ik][j];
		}
	}

EXIT:
	FreeVector(weight);  /* �L���̈����� */
	return det;           /* �߂�l�͍s�� */
}

// Function: MatInv3
// 3x3�̋t�s������߂�
//
// Parameters:
// A - ���̍s��
// A_inv - A�̋t�s����i�[
//
// Return:
// �s��
double MatInv3(Matrix A,Matrix A_inv)
{
	double det;
	det = A[0][0]*A[1][1]*A[2][2] + A[1][0]*A[2][1]*A[0][2] + A[2][0]*A[0][1]*A[1][2]
	      - A[0][0]*A[2][1]*A[1][2] - A[2][0]*A[1][1]*A[0][2] - A[1][0]*A[0][1]*A[2][2];

	if(det == 0) return KOD_FALSE;		// �s�񎮂�0

	A_inv[0][0] = (A[1][1]*A[2][2]-A[1][2]*A[2][1])/det;
	A_inv[0][1] = (A[0][2]*A[2][1]-A[0][1]*A[2][2])/det;
	A_inv[0][2] = (A[0][1]*A[1][2]-A[0][2]*A[1][1])/det;
	A_inv[1][0] = (A[1][2]*A[2][0]-A[1][0]*A[2][2])/det;
	A_inv[1][1] = (A[0][0]*A[2][2]-A[0][2]*A[2][0])/det;
	A_inv[1][2] = (A[0][2]*A[1][0]-A[0][0]*A[1][2])/det;
	A_inv[2][0] = (A[1][0]*A[2][1]-A[1][1]*A[2][0])/det;
	A_inv[2][1] = (A[0][1]*A[2][0]-A[0][0]*A[2][1])/det;
	A_inv[2][2] = (A[0][0]*A[1][1]-A[0][1]*A[1][0])/det;

	return det;
}

// Function: MatInv2
// 2x2�̋t�s������߂�
//
// Parameters:
// A - ���̍s��
// A_inv - A�̋t�s����i�[
//
// Return:
// �s��
double MatInv2(Matrix A,Matrix A_inv)
{
	double det;

	det = A[0][0]*A[1][1] - A[0][1]*A[1][0];

	if(det == 0) return KOD_FALSE;		// �s�񎮂�0

	A_inv[0][0] = A[1][1]/det;
	A_inv[0][1] = -A[0][1]/det;
	A_inv[1][0] = -A[1][0]/det;
	A_inv[1][1] = A[0][0]/det;

	return det;
}

// Function: nCr
// 2���W��(nCr�̑g��������)�����߂�
// 
// Parameters:
// n,r - nCr��n��r
//
// Return:
// �v�Z����
int nCr(int n,int r)
{
	int p=1;
	for(int i=1;i<=r;i++)
		p *= (n-i+1)/i;

	return p;
}

// Function: Factorial
// ���R��n�̊K������߂�(�����ɒ��ӁDint�^�̏ꍇ�C10!�ŃI�[�o�[�t���[)
//
// Parameters:
// n - n!��n
//
// Return:
// �v�Z����
int Factorial(int n)
{
	int ans = 1;
	for(int i=1;i<=n;i++)
		ans *= i;
	return ans;
}

// Function: Round
// �l�̌ܓ�����
// 
// Parameters:
// x - �l�̌ܓ�����double�^�l
//
// Return:
// �v�Z����
double Round(double x)
{
	if(x > 0.0){
		return floor(x + 0.5);
	}
	else{
		return -1.0 * floor(fabs(x) + 0.5);
	}
}

// Function: SetColorStat
// �J���[�X�e�[�^�X�\����DispStat�ɒl��������
//
// Parameters:
// *ds - ������DispStat�\���̂ւ̃|�C���^
// r,g,b,a - �J���[�X�e�[�^�X
void SetColorStat(DispStat *ds,float r, float g, float b, float a)
{
	ds->Color[0] = r;
	ds->Color[1] = g;
	ds->Color[2] = b;
	ds->Color[3] = a;
}

// Function: CatCoord
// ������W�l�z��̌��ɐV���ȍ��W�l�z����q����
//
// Parameters:
// a[] - ����������W�l�z��		
// b[] - ���������W�l�z��		
// alim - a[]�̔z��v�f��
// anum - ���݂�a[]�Ƀf�[�^���i�[����Ă���Ō���ԍ�
// bnum - b[]�̑���������z��v�f��
//
// Return: 
// ���Z���ꂽ�z��v�f��
int CatCoord(Coord a[],Coord b[],int alim,int anum,int bnum)
{
	if(alim < anum+bnum){
		GuiIF.SetMessage("stack over flow");
		return KOD_ERR;
	}

	for(int i=0;i<bnum;i++){
		a[anum+i] = SetCoord(b[i]);
	}

	return anum+bnum;
}

// Function: CheckTheSamePoints
// ����_����������
//
// Prameters:
// *P - �_�Q(�ύX��̓_�Q�������Ɋi�[�����)   
// N - �_��
//
// Return:
// �ύX��̓_��
int CheckTheSamePoints(Coord *P,int N)
{
	for(int i=0;i<N;i++)
		P[i].dmy = KOD_FALSE;

	for(int i=0;i<N;i++){
		if(P[i].dmy == KOD_FALSE){
			for(int j=i+1;j<N;j++){
				if(DiffCoord(P[i],P[j],APPROX_ZERO_L) == KOD_TRUE){
					P[j].dmy = KOD_TRUE;
				}
			}
		}
	}
	int k=0;
	for(int i=0;i<N;i++){
		if(P[i].dmy != KOD_TRUE){
			P[k] = SetCoord(P[i]);
			k++;
		}
	}
	return k;
}
// Function: CheckTheSamePoints
// ����l����������(�I�[�o�[���[�h)
//
// Prameters:
// *P - ���l�z��(�ύX��������Ɋi�[�����)   
// N - �_��
//
// Return:
// �ύX��̓_��
int CheckTheSamePoints(double *P,int N)
{
	if(!N) return 0;

//	bool *flag = (bool *)malloc(sizeof(bool)*N);
	bool *flag = new bool[N];

	for(int i=0;i<N;i++)
		flag[i] = false;

	for(int i=0;i<N;i++){
		if(flag[i] == false){
			for(int j=i+1;j<N;j++){
				if(CheckZero(P[i]-P[j],MID_ACCURACY) == KOD_TRUE){
					flag[j] = true;
				}
			}
		}
	}
	int k=0;
	for(int i=0;i<N;i++){
		if(flag[i] != KOD_TRUE){
			P[k] = P[i];
			k++;
		}
	}
//	free(flag);
	delete[]	flag;

	return k;
}

// Function: CheckTheSamePoints2D
// 2�������ʓ��̓���_���������� (���W�l��Coord��x��y�ŗ^����)
// 
// Parameters:
// *P - �_�Q(�ύX��̓_�Q�������Ɋi�[�����)   
// N - �_��
//
// Return:
// �ύX��̓_��
int CheckTheSamePoints2D(Coord *P,int N)
{
	Coord *Q = NewCoord1(N);

	for(int i=0;i<N;i++)
		P[i].dmy = KOD_FALSE;

	int k=0;
	for(int i=0;i<N;i++){
		if(P[i].dmy == KOD_FALSE){
			Q[k] = SetCoord(P[i]);
			for(int j=0;j<N;j++){
				if(DiffCoord(Q[k],P[j]) == KOD_TRUE){
					P[j].dmy = KOD_TRUE;
				}
			}
			k++;
		}
	}

	for(int i=0;i<k;i++)
		P[i] = SetCoord(Q[i]);

	FreeCoord1(Q);

	return k;
}

// Function: CoordToArray
// Coord��double�z��ɑ��
//
// Parameters:
// a - Coord�l
// b[3] - double�z��
void CoordToArray(Coord a,double b[3])
{
	b[0] = a.x;
	b[1] = a.y;
	b[2] = a.z;
}

// Function: CoordToArray2D
// Coord��double�z��ɑ��(2D Ver.)
//
// Parameters:
// a - Coord�l
// b[2] - double�z��
void CoordToArray2D(Coord a,double b[2])
{
	b[0] = a.x;
	b[1] = a.y;
}