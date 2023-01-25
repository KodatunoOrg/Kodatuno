#include <stdlib.h>
#include "StdAfx.h"


// Coord�N���X�̃I�y���[�^�̃I�[�o�[���[�h
Coord Coord::operator +(Coord a)
{
	return(AddCoord(*this,a));
}
Coord Coord::operator -(Coord a)
{
	return(SubCoord(*this,a));
}
Coord Coord::operator *(Coord a)
{
	return(MulCoord(*this,a));
}
Coord Coord::operator *(double a)
{
	return(MulCoord(*this,a));
}
Coord Coord::operator /(Coord a)
{
	return(DivCoord(*this,a));
}
double Coord::operator &(Coord a)
{
	return(CalcInnerProduct(*this,a));
}
Coord Coord::operator &&(Coord a)
{
	return(CalcOuterProduct(*this,a));
}

// ���W�l�̏�����
void InitCoord(Coord *a)
{
	a->x = 0;
	a->y = 0;
	a->z = 0;
	a->dmy = 0;
}
void InitCoord(Coord *a,int n)
{
	for(int i=0;i<n;i++){
		a[i].x = 0;
		a[i].y = 0;
		a[i].z = 0;
		a[i].dmy = 0;
	}
}
Coord InitCoord()
{
	Coord a;
	InitCoord(&a);

	return a;
}

// ���W�l���m�̑����Z
Coord AddCoord(Coord a,Coord b)
{
	Coord ans;

	ans.x = a.x + b.x;
	ans.y = a.y + b.y;
	ans.z = a.z + b.z;

	return ans;
}
Coord AddCoord(Coord a,double b)
{
	Coord ans;

	ans.x = a.x + b;
	ans.y = a.y + b;
	ans.z = a.z + b;

	return ans;
}
Coord AddCoord(Coord a,double x,double y,double z)
{
	Coord ans;

	ans.x = a.x + x;
	ans.y = a.y + y;
	ans.z = a.z + z;

	return ans;
}

// ���W�l���m�̑����Z (2D Ver.)
Coord AddCoord2D(Coord a,Coord b)
{
	Coord ans;

	ans.x = a.x + b.x;
	ans.y = a.y + b.y;

	return ans;
}
Coord AddCoord2D(Coord a,double b)
{
	Coord ans;

	ans.x = a.x + b;
	ans.y = a.y + b;

	return ans;
}
Coord AddCoord2D(Coord a,double x,double y)
{
	Coord ans;

	ans.x = a.x + x;
	ans.y = a.y + y;

	return ans;
}

// ���W�l���m�̊���Z
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
Coord DivCoord(Coord a,double b)
{
	if(b == 0.0)	return SetCoord(0.0,0.0,0.0);

	Coord ans;

	ans.x = a.x / b;
	ans.y = a.y / b;
	ans.z = a.z / b;

	return ans;
}
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

// ���W�l���m�̊���Z (2D Ver.)
Coord DivCoord2D(Coord a,Coord b)
{
	if(b.x == 0.0 || b.y == 0.0)
		return SetCoord2D(0.0,0.0);

	Coord ans;

	ans.x = a.x / b.x;
	ans.y = a.y / b.y;

	return ans;
}
Coord DivCoord2D(Coord a,double b)
{
	if(b == 0.0)	return SetCoord2D(0.0,0.0);

	Coord ans;

	ans.x = a.x / b;
	ans.y = a.y / b;

	return ans;
}
Coord DivCoord2D(Coord a,double x,double y)
{
	if(x == 0.0 || y == 0.0)
		return SetCoord2D(0.0,0.0);

	Coord ans;

	ans.x = a.x/x;
	ans.y = a.y/y;

	return ans;
}

// ���W�l���m�̈����Z
Coord SubCoord(Coord a,Coord b)
{
	Coord ans;

	ans.x = a.x - b.x;
	ans.y = a.y - b.y;
	ans.z = a.z - b.z;

	return ans;
}
Coord SubCoord(Coord a,double b)
{
	Coord ans;

	ans.x = a.x - b;
	ans.y = a.y - b;
	ans.z = a.z - b;

	return ans;
}
Coord SubCoord(Coord a,double x,double y,double z)
{
	Coord ans;

	ans.x = a.x - x;
	ans.y = a.y - y;
	ans.z = a.z - z;

	return ans;
}

// ���W�l���m�̈����Z (2D Ver.)
Coord SubCoord2D(Coord a,Coord b)
{
	Coord ans;

	ans.x = a.x - b.x;
	ans.y = a.y - b.y;

	return ans;
}
Coord SubCoord2D(Coord a,double b)
{
	Coord ans;

	ans.x = a.x - b;
	ans.y = a.y - b;

	return ans;
}
Coord SubCoord2D(Coord a,double x,double y)
{
	Coord ans;

	ans.x = a.x - x;
	ans.y = a.y - y;

	return ans;
}

// ���W�l���m�̊|���Z
Coord MulCoord(Coord a,Coord b)
{
	Coord ans;

	ans.x = a.x * b.x;
	ans.y = a.y * b.y;
	ans.z = a.z * b.z;

	return ans;
}
Coord MulCoord(Coord a,double b)
{
	Coord ans;

	ans.x = a.x * b;
	ans.y = a.y * b;
	ans.z = a.z * b;

	return ans;
}
Coord MulCoord(Coord a,double x,double y,double z)
{
	Coord ans;

	ans.x = a.x * x;
	ans.y = a.y * y;
	ans.z = a.z * z;

	return ans;
}

// ���W�l���m�̊|���Z (2D Ver.)
Coord MulCoord2D(Coord a,Coord b)
{
	Coord ans;

	ans.x = a.x * b.x;
	ans.y = a.y * b.y;

	return ans;
}
Coord MulCoord2D(Coord a,double b)
{
	Coord ans;

	ans.x = a.x * b;
	ans.y = a.y * b;

	return ans;
}
Coord MulCoord2D(Coord a,double x,double y)
{
	Coord ans;

	ans.x = a.x * x;
	ans.y = a.y * y;

	return ans;
}

// ���W�l�������Ȃ�KOD_TRUE�A�قȂ��Ă���Ȃ�KOD_FALSE��Ԃ�
int DiffCoord(Coord a,Coord b)
{
	if(fabs(a.x-b.x) <= APPROX_ZERO && fabs(a.y-b.y) <= APPROX_ZERO && fabs(a.z-b.z) <= APPROX_ZERO)
	//if(fabs(a.x-b.x) == 0 && fabs(a.y-b.y) == 0 && fabs(a.z-b.z) == 0)
		return KOD_TRUE;
	else
		return KOD_FALSE;
}
// �I�[�o�[���[�h(���x�w��)
int DiffCoord(Coord a,Coord b,double App)
{
	if(fabs(a.x-b.x) <= App && fabs(a.y-b.y) <= App && fabs(a.z-b.z) <= App)
		return KOD_TRUE;
	else
		return KOD_FALSE;
}

// 2D���ʂł̍��W�l�������Ȃ�KOD_TRUE�A�قȂ��Ă���Ȃ�KOD_FALSE��Ԃ�
int DiffCoord2D(Coord a,Coord b)
{
	if(fabs(a.x-b.x) <= APPROX_ZERO && fabs(a.y-b.y) <= APPROX_ZERO)
	//if(fabs(a.x-b.x) == 0 && fabs(a.y-b.y) == 0 && fabs(a.z-b.z) == 0)
		return KOD_TRUE;
	else
		return KOD_FALSE;
}
// �I�[�o�[���[�h(���x�w��)
int DiffCoord2D(Coord a,Coord b,double App)
{
	if(fabs(a.x-b.x) <= App && fabs(a.y-b.y) <= App)
	//if(fabs(a.x-b.x) == 0 && fabs(a.y-b.y) == 0 && fabs(a.z-b.z) == 0)
		return KOD_TRUE;
	else
		return KOD_FALSE;
}

// ���W�l�̐�Βl��Ԃ�
Coord AbsCoord(Coord a)
{
	Coord ans;

	ans.x = fabs(a.x);
	ans.y = fabs(a.y);
	ans.z = fabs(a.z);

	return ans;
}
Coord AbsCoord2D(Coord a)
{
	Coord ans;

	ans.x = fabs(a.x);
	ans.y = fabs(a.y);

	return ans;
}

// ���W�l��������
Coord SetCoord(Coord a)
{
	return a;
}
Coord SetCoord(double x,double y,double z)
{
	Coord ans;

	ans.x = x;
	ans.y = y;
	ans.z = z;

	return ans;
}
Coord SetCoord2D(Coord a)
{
	return a;
}
Coord SetCoord2D(double x,double y)
{
	Coord ans;

	ans.x = x;
	ans.y = y;

	return ans;
}

// ���W�l�Q�̃R�s�[(b<--a)
void CopyCoord(Coord *a,int n,Coord *b)
{
	for(int i=0;i<n;i++)
		b[i] = SetCoord(a[i]);
}
void CopyCoord2D(Coord *a,int n,Coord *b)
{
	for(int i=0;i<n;i++)
		b[i] = SetCoord2D(a[i]);
}

// (0,0,0)�̂Ƃ�KOD_FALSE��Ԃ�
int ZoroCoord(Coord a)
{
	if(a.x == 0.0 && a.y == 0.0 && a.z == 0.0)
		return KOD_FALSE;

	return KOD_TRUE;
}
int ZoroCoord2D(Coord a)
{
	if(a.x == 0.0 && a.y == 0.0)
		return KOD_FALSE;

	return KOD_TRUE;
}

// double�^1�����z��̃������[�m��
Vector NewVector(int len)
{
	Vector a;
	if((a = (double *)malloc(len*sizeof(double))) == NULL){
		SetMessage("fail to allocate memoly");
		return NULL;
	}

	return a;
}

// double�^2�����z��̃������[�m��
Matrix NewMatrix(int row,int col)
{
	int i;
	Matrix a;

	if((a = (double **)malloc(row*sizeof(double *))) == NULL){
		return NULL;
	}
	for(i=0;i<row;i++){
		if((a[i] = (double *)malloc(col*sizeof(double))) == NULL){
			while(--i>=0) free(a[i]);
			free(a);
			return NULL;
		}
	}

	return a;
}

// double�^1�����z��̃������[���
void FreeVector(Vector a)
{
	free(a);
}

// double�^2�����z��̃������[���
void FreeMatrix(Matrix a,int col)
{
	for(int i=0;i<col;i++)
		free(a[i]);

	free(a);
}

// 1����Coord�^�z��̃������[�m��
Coord *NewCoord1(int len)
{
	Coord *a;

	if((a = (Coord *)malloc(len*sizeof(Coord))) == NULL){
		SetMessage("fail to allocate memoly");
		return NULL;
	}

	return a;
}

// 2����Coord�^�z��̃������[�m��
Coord **NewCoord2(int row,int col)
{
	int i;
	Coord **a;

	if((a = (Coord **)malloc((row)*sizeof(Coord *))) == NULL){
		SetMessage("fail to allocate memoly");
		return NULL;
	}
	for(i=0;i<row;i++){
		if((a[i] = (Coord *)malloc(col*sizeof(Coord))) == NULL){
			SetMessage("fail to allocate memoly");
			while(--i>=0) free(a[i]);
			free(a);
			return NULL;
		}
	}

	return a;
}

// 1����Coord�^�z��̃������[��� 
void FreeCoord1(Coord *a)
{
	free(a);
}

// 2����Coord�^�z��̃������[���
void FreeCoord2(Coord **a,int col)
{
	Coord **b;

	b=a;
	for(int i=0;i<col;i++){
		free(b[i]);
	}
	free(a);
}

// �x�N�g���𐳋K������
Coord NormalizeVec(Coord a)
{
	double len=0;

	len = CalcEuclid(a);

	return DivCoord(a,len);
}
Coord NormalizeVec(double x,double y,double z)
{
	double len=0;
	Coord a = SetCoord(x,y,z);

	len = CalcEuclid(a);

	return DivCoord(a,len);
}

// ���[�N���b�h�������Z�o
double CalcEuclid(Coord a)
{
	return sqrt(a.x*a.x + a.y*a.y + a.z*a.z);
}

// 2�������[�N���b�h�������Z�o
double CalcEuclid2D(double a,double b)
{
	return sqrt(a*a+b*b);
}

// 2�_�Ԃ̃��[�N���b�h���������߂�
double CalcDistance(Coord a,Coord b)
{
	return(CalcEuclid(SubCoord(a,b)));
}

// 2�������W��ł�2�_�Ԃ̃��[�N���b�h�������Z�o
double CalcDistance2D(Coord a,Coord b)
{
	return sqrt((a.x-b.x)*(a.x-b.x) + (a.y-b.y)*(a.y-b.y));
}

// ���ς����߂�
double CalcInnerProduct(Coord a,Coord b)
{
	return(a.x*b.x+a.y*b.y+a.z*b.z);
}


// ���ς����߂�(�I�[�o�[���[�h)
double CalcInnerProduct(Coord a,double x,double y,double z)
{
	return(a.x*x+a.y*y+a.z*z);
}

// �O�ς����߂�
Coord CalcOuterProduct(Coord a,Coord b)
{
	Coord c;

	c.x = a.y*b.z - a.z*b.y;
	c.y = a.z*b.x - a.x*b.z;
	c.z = a.x*b.y - a.y*b.x;

	return c;
}

// �O�ς����߂� (2D Ver.)
double CalcOuterProduct2D(Coord a,Coord b)
{
	return(a.x*b.y - a.y*b.x);
}

// 2�̃x�N�g���̂Ȃ��p�����߂�(�Ԓl��rad)
double CalcVecAngle(Coord a,Coord b)
{
	double inn = CalcInnerProduct(a,b);
	double abs_a = CalcEuclid(a);
	double abs_b = CalcEuclid(b);

	return(acos(inn/abs_a/abs_b));
}

// 2�_p(t=0),q(t=1)��t(0�`1)�œ��������Ƃ��̓_�̍��W�����Ƃ߂�
Coord CalcInterDivPt(Coord p,Coord q,double t)
{
	return(AddCoord(p,MulCoord(SubCoord(q,p),t)));
}

// �C�ӂ̓_��C�ӂ̕��ʂ֐��ˉe����
// ����   p:�C�ӂ̕��ʏ�̓_,  n:�C�ӂ̕��ʂ̒P�ʖ@���x�N�g��,  q:���ˉe�������_
// �Ԓl   ���ˉe���ꂽ�_�̍��W�l
Coord CalcOrthoProjection(Coord p,Coord n,Coord q)
{
	if(fabs(1-CalcEuclid(n)) > APPROX_ZERO){
		SetMessage("ERROR:Norm vector is not unit vector.");
		SetMessage("Norm vetor is resized to unit vector.");
		NormalizeVec(n);
	}
	double inn = CalcInnerProduct(SubCoord(q,p),n);
	return (SubCoord(q,MulCoord(n,inn)));
}

// �C�ӂ̓_����C�ӂ̕��ʂ܂ł̋��������߂�
// ���� Pt:�C�ӂ̓_  P0:���ʏ��1�_  N:���ʂ̖@���x�N�g��
double CalcDistPtToPlane(Coord Pt,Coord P0,Coord N)
{
	return((fabs(N.x*Pt.x + N.y*Pt.y + N.z*Pt.z - (N.x*P0.x + N.y*P0.y + N.z*P0.z)))/CalcEuclid(N));
}

// �X�J���[�O�d�ς����߂�
double CalcScalarTriProduct(Coord a,Coord b,Coord c)
{
	return(CalcInnerProduct(a,CalcOuterProduct(b,c)));
}

// �_��`��
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

// �_�Q��`��
// ����   *p:�_�Q, n:�_��, scale:�X�P�[��, width:�_�̃T�C�Y, color,�_�̐F
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

// �x�N�g����`��
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

// 2�_�Ԃɐ�����`��
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

// �p�x��degree����radian�֕ϊ�
double DegToRad(double degree)
{
	double radian;

	radian = degree * PI / 180.0;

	return radian;	
}

// �p�x��radian����degree�֕ϊ�
double RadToDeg(double radian)
{
	double degree = 0.0;

	degree = radian * 180.0 / PI;

	return degree;
}

// �~�̒��S�_(vec[0])����~��ɐڂ���C�ӂ�2�{�̐ڐ��������_�ւ̃x�N�g��(���S�p0<��<��)
Coord Arc_CP(Coord a, Coord b, double cos)
{
	Coord ans;

	ans.x = (a.x + b.x)/(1 + cos);
	ans.y = (a.y + b.y)/(1 + cos);
	ans.z = (a.z + b.z)/(1 + cos);

	return ans;
}

// 2�̃x�N�g���̂Ȃ��p�����߂�(2D����)
double CalcVecAngle2D(Coord a, Coord b)
{
	double angle,sin,cos;

	sin = (a.x*b.y - b.x*a.y)/(a.x*a.x + a.y*a.y);
	cos = (a.x*b.x + a.y*b.y)/(a.x*a.x + a.y*a.y);

	angle = atan2(sin,cos);
	if(angle < 0) angle = angle + 2.0*PI;

	return angle;
}

// �C�ӂ̃x�N�g������]�������x�N�g�������߂�(2D����)
Coord CalcRotVec2D(Coord a, double angle)
{
	Coord ans;

	ans.x = a.x*cos(angle) - a.y*sin(angle);
	ans.y = a.x*sin(angle) + a.y*cos(angle);

	return ans;
}

// �C�ӂ̃x�N�g�������_��ʂ�C�ӎ�����ɉ�]�������x�N�g�������߂�(3D����)
// ����  a:��]���������x�N�g��  e:���_��ʂ�C�ӎ�(�P�ʃx�N�g���ł��邱��)  ang:��]�p(rad)
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

// �C�ӂ̓_P����C�ӂ̒���(�_A��ʂ�P�ʃx�N�g��u�̕���������)�։��낵���_�����߂�
Coord CalcNormalLine(Coord P,Coord A,Coord u)
{
	double k = CalcInnerProduct(SubCoord(P,A),u);

	return(AddCoord(A,MulCoord(u,k)));
}

// �o�u���\�[�g(��{�����@)
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

// �N�C�b�N�\�[�g(double�^�A�~��)�ŗp�����r�֐�
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

// Coord�z��̔��]����
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

// double�^�z��̔��]����
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


// 1�����z��̏�����
void InitVector(Vector vec,int size)
{
	for(int i=0;i<size;i++){
		vec[i] = 0.0;
	}
}

// 2�����z��̏�����
void InitMatrix(Matrix mat,int size_x,int size_y)
{
	for(int i=0;i<size_x;i++){
		for(int j=0;j<size_y;j++){
			mat[i][j] = 0.0;
		}
	}
}

// �x�N�g���̃R�s�[
// a�x�N�g����b�x�N�g���֑��
void CopyVector(Vector a,int n,Vector b)
{
	for(int i=0;i<n;i++)
		b[i] = a[i];
}

// 3��������������
// ����  *a:4�̌W��(a[0]x^3 + a[1]x^2 + a[2]x + a[3])   *ans:3�̉�
// �Ԓl�@����3�Ƃ������̏ꍇ��3�A1���������̏ꍇ��1  a[0]==0�̏ꍇ��KOD_ERR
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

// 2��������������
// ����  *a:3�̌W��(a[0]x^2 + a[1]x + a[2])  *ans:2�̉�
// �Ԓl  ���������̏ꍇ��2�A�����̏ꍇ��KOD_ERR  a[0]==0�̏ꍇ��KOD_ERR
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

// 1��������������
// ����  *a:2�̌W��(a[0]x + a[1])  *ans:��
// �Ԓl  a[0]==0�̏ꍇ��KOD_ERR
int CalcLinearEquation(double *a,double *ans)
{
	if(fabs(a[0]) < APPROX_ZERO_H){
		return KOD_FALSE;
	}

	ans[0] = -a[1]/a[0];

	return 1;
}

// ��������
double sgn(double x)
{
	if(x<0)	return -1;
	else if(x == 0) return 0;
	else return 1;
}

// �l��APPROX_ZERO�͈̔͂�0�ł��邩�`�F�b�N
// -APPROX_ZERO < val < APPROX_ZERO
// ���� val:���͒l  flag:���x(HIGH_ACCURACY or MID_ACCURACY or LOW_ACCURACY)
// �A�l   KOD_TRUE:�͈͓��Ń[���Ƃ݂Ȃ���     KOD_FALSE:�͈͊O     KOD_ERR:������flag�w�肪�Ԉ���Ă���
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
int CheckZero(Coord cod,int flag)
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

	if(fabs(cod.x) < ap && fabs(cod.y) < ap && fabs(cod.z) < ap)
		return KOD_TRUE;

	return KOD_FALSE;
}

// �w�肵���l���w�肵���͈͓��ł��邩���`�F�b�N
// ����  low:����  up:���   val:���ׂ����l
//       flag = 0:(low <= val <= up) --> (low-ap < val < up+ap), 
//              1:(low < val < up) --> (low+ap < val < up-ap),
//              2:(val <= up) --> (val < up+ap),
//              3:(val < up) --> (val < up-ap),
//              4:(low <= val) --> (low-ap < val),
//              5:(low < val) --> (low+ap < val)
// ���Ӂ@val��APPROX_ZERO(ap)����low�܂���up�ƈ�v����ꍇ�́A�͈͓��ɂ�����̂Ƃ���
// �Ԓl  KOD_TRUE:�͈͓��@�@KOD_FALSE:�͈͊O�@�@�@KOD_ERR:flag�Ɏw�肵���l��0,1�ȊO
int CheckRange(double low,double up,double val,int flag)
{
	if(flag < 0 || flag > 5){
		char mes[256];
		sprintf(mes,"CheckRange ERROR:wrong specified value. 0 or 1");
		SetMessage(mes);
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

// 2�̒l�̑召��r
// ����  val1,val2:���͒l   flag:���x(HIGH_ACCURACY or or MID_ACCURACY or LOW_ACCURACY)
// �Ԓl�@KOD_EQUAL: val1 = val2 (|va1-val2| < APPROX_ZERO)
//       KOD_LARGE:  val1 > val2 ()
//       KOD_SMALL: val1 < val2 ()
int CheckMag(double val1,double val2,int flag)
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

	double d = val1-val2;
	if(fabs(d) < ap)
		return KOD_EQUAL;
	else if(d < 0)
		return KOD_SMALL;
	else
		return KOD_LARGE;
}

// ���ړ_�̑��p�`���O����(x-y���ʓ�)
// ����	  TargetPoint:���ړ_  BorderPoint:���p�`�̒��_�Q   CountPoint:���_�̐�
// �Ԓl	  KOD_TRUE:��  KOD_FALSE:�O  KOD_ONEDGE:�G�b�W��
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

		double a = (p1.x - p0.x)*(p1.y - TargetPoint.y) - (p1.y - p0.y)*(p1.x - TargetPoint.x);		// TargetPoint���G�b�W��(p0��p1�̐���)�ɂ��邩�`�F�b�N
		if(CheckZero(a,MID_ACCURACY) == KOD_TRUE)			
		//if(a == 0)					// �G�b�W��Ȃ�
			return KOD_ONEDGE;		// �ⓚ���p��return

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

// ��ԏ��3�_����Ȃ镽�ʂ̖@���x�N�g�������߂�
Coord CalcNormVecFrom3Pts(Coord p1,Coord p2,Coord p3)
{
	Coord denom = (p2-p1)&&(p3-p1);
	double numer = CalcEuclid(denom);

	return DivCoord(denom,numer);
}

// ��ԏ�̑��p�`�̖ʐς𓾂�(���_�̐�:Vnum, ���_��Fp[Vnum])
double CalcPolygonArea(Coord p[],int Vnum)
{
	double area=0;

	for(int i=0;i<Vnum;i++){
		area += CalcEuclid(CalcOuterProduct(p[i],p[(i+1)%Vnum]));
	}

	return(area/2);
}

// 2D���ʏ�̑��p�`�̕����t���ʐς𓾂�(���_�̐�:Vnum, ���_��Fp[Vnum])
// CCW�F��    CW�F��
double ClacPolygonArea2D(Coord p[],int Vnum)
{
	double area=0;

	for(int i=0;i<Vnum;i++){
		area += CalcOuterProduct2D(p[i],p[(i+1)%Vnum]);
	}

	return(area/2);
}

// 2D���ʏ�̑��p�`�����v��肩�����v��肩�𔻕ʂ���
// �Ԓl   CCW�FKOD_TRUE     CW�FKOD_FALSE
int DiscriminateCW2D(Coord p[],int Vnum)
{
	if(ClacPolygonArea2D(p,Vnum) > 0)	// CCW
		return KOD_TRUE;

	else	// CW
		return KOD_FALSE;
}

// �s�񓯎m�̑����Z
void AddMxMx(Matrix A,Matrix B,Matrix C,int row,int col)
{
	for(int i=0;i<row;i++){
		for(int j=0;j<col;j++){
			C[i][j] = A[i][j] + B[i][j];
		}
	}
}


// �s�񓯎m�̊|���Z
void MulMxMx(Matrix A,int A_row,int A_col,Matrix B,int B_row,int B_col,Matrix C)
{
	//for(int i=0;i<A_row;i++){
	//	for(int j=0;j<B_col;j++){
	//		C[i][j] = 0;
	//		for(int k=0;k<A_col;k++){
	//			C[i][j] += A[i][k] * B[k][j];
	//		}
	//	}
	//}

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

// �s��ƃx�N�g���̊|���Z
// ����  C = AB   A_row:�s��  A_col:��  B_row:�x�N�g���̎�����
void MulMxVec(Matrix A,int A_row,int A_col,Vector B,int B_row,Vector C)
{
	for(int i=0;i<A_row;i++){
		C[i] = 0;
		for(int j=0;j<A_col;j++){
			C[i] += A[i][j] * B[j];
		}
	}
}

// �s��ƍ��W�l�x�N�g���̊|���Z
// |A[0][0]     A[0][1] . .   A[0][col-1]  ||  B[0]  |
// |A[1][0]     A[1][1] . .   A[1][col-1]  ||  B[1]  |
// |   .           .    . .       .        ||    .   |
// |   .           .    . .       .        ||    .   |
// |A[row-1][0]    .    . . A[row-1][col-1]||B[row-1]|
//
void MulMxVec(Matrix A,int A_row,int A_col,Coord *B,Coord *C)
{
	for(int i=0;i<A_row;i++){
		C[i] = SetCoord(0,0,0);
		for(int j=0;j<A_col;j++){
			C[i] = AddCoord(C[i],MulCoord(B[j],A[i][j]));
		}
	}
}

// Coord�ŕ\�������3x3�s���Coord�x�N�g���Ƃ̊|���Z
//     |A[0].x A[1].x A[2].x|       |d.x|
// A = |A[0].y A[1].y A[2].y| , d = |d.y|
//     |A[0].z A[1].z A[2].z|       |d.z|
Coord MulMxCoord(Coord A[],Coord d)
{
	Coord ans;

	ans.x = A[0].x*d.x + A[1].x*d.y + A[2].x*d.z;
	ans.y = A[0].y*d.x + A[1].y*d.y + A[2].y*d.z;
	ans.z = A[0].z*d.x + A[1].z*d.y + A[2].z*d.z;

	return ans;
}

Coord MulMxCoord(Matrix A,Coord d)
{
	Coord ans;

	ans.x = A[0][0]*d.x + A[0][1]*d.y + A[0][2]*d.z;
	ans.y = A[1][0]*d.x + A[1][1]*d.y + A[1][2]*d.z;
	ans.z = A[2][0]*d.x + A[2][1]*d.y + A[2][2]*d.z;

	return ans;
}

// �]�u�s��𓾂�
// ����  **A:���̍s��  m:A�̍s��  n:A�̗�  **B:�]�u�s����i�[
// �]�u������m��n���t�ɂȂ�̂ŁAB�̃������[�m�ۂɒ���!
void TranMx(Matrix A,int m,int n,Matrix B)
{
	for(int i=0;i<m;i++){
		for(int j=0;j<n;j++){
			B[j][i] = A[i][j];
		}
	}
}

// �]�u�s��𓾂�(�I�[�o�[���[�h)
// ����  **A:���̍s��  m:A�̍s��  n:A�̗�  **B:�]�u�s����i�[
void TranMx(Coord **A,int m,int n,Coord **B)
{
	for(int i=0;i<m;i++){
		for(int j=0;j<n;j++){
			B[j][i] = SetCoord(A[i][j]);
		}
	}
}

// �]�u�s��𓾂�(�I�[�o�[���[�h)
//              |A[0].x A[1].x A[2].x|                |B[0].x B[1].x B[2].x|   |A[0].x A[0].y A[0].z|
// Coord A[3] = |A[0].y A[1].y A[2].y| , Coord B[3] = |B[0].y B[1].y B[2].y| = |A[1].x A[1].y A[1].z| = A^T
//              |A[0].z A[1].z A[2].z|                |B[0].z B[1].z B[2].z|   |A[2].x A[2].y A[2].z|
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

// �����ϊ��s��ƍ��W�l(3D�x�N�g��)�Ƃ̊|���Z
Coord MulFrameCoord(double R[][3],double T[],Coord I)
{
	Coord O;

	O.x = R[0][0]*I.x + R[0][1]*I.y + R[0][2]*I.z + T[0];
	O.y = R[1][0]*I.x + R[1][1]*I.y + R[1][2]*I.z + T[1];
	O.z = R[2][0]*I.x + R[2][1]*I.y + R[2][2]*I.z + T[2];

	return O;
}
Coord MulFrameCoord(FRAME f,Coord I)
{
	Coord O;

	O.x = f.Rot[0].x*I.x + f.Rot[1].x*I.y + f.Rot[2].x*I.z + f.Trl.x;
	O.y = f.Rot[0].y*I.x + f.Rot[1].y*I.y + f.Rot[2].y*I.z + f.Trl.y;
	O.z = f.Rot[0].z*I.x + f.Rot[1].z*I.y + f.Rot[2].z*I.z + f.Trl.z;

	return O;
}

// �����ϊ��s��̋t�s��𓾂�
// F = |R T|    F^-1 = |R^-1 -R^-1*T|
//     |0 1|           |  0     1   |
FRAME InvFrame(FRAME F)
{
	FRAME f;

	TranMx(F.Rot,f.Rot);				// F.Rot�̓]�u�s��F.Rot^T�𓾂�
	f.Trl = MulMxCoord(f.Rot,F.Trl);	// F.Rot^T * F.Trl
	f.Trl = MulCoord(f.Trl,-1);			// -(F.Rot^T * F.Trl)

	return f;
}

// �����ϊ��s��̊|���Z
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


// ��]�s�񂩂�ZYZ�I�C���[�p���Z�o�i tmp.x �����p(O), tmp.y �����p(A), tmp.z �����p(T) �ɂ��ꂼ��Ή� �j
Coord RotToZYZEuler( Coord rot[])
{
	Coord tmp = SetCoord(0,0,0);

	tmp.y = atan2( sqrt( rot[0].z * rot[0].z + rot[1].z * rot[1].z ), rot[2].z );

	if( fabs( tmp.y ) <= APPROX_ZERO ){
		tmp.x = 0.0;
		tmp.z = atan2( -rot[1].x, rot[0].x );
	}
	else if( fabs( tmp.y - PI ) <= APPROX_ZERO ){
		tmp.x = 0.0;
		//tmp.z = atan2( rot[1].x, -rot[0].x );		// ���X���������������A
		tmp.z = atan2( rot[1].x, rot[0].x );		// �}�C�i�X���Ȃ������B
	}
	else{
		tmp.x = atan2( rot[2].y / sin( tmp.y ), rot[2].x / sin( tmp.y ) );
		tmp.z = atan2( rot[1].z / sin( tmp.y ), -rot[0].z / sin( tmp.y ) );
	}	
	tmp = MulCoord(tmp,180/PI);

	return tmp;
}

// FRAME�̏�����
void InitFrame(FRAME *f)
{
	f->Rot[0] = SetCoord(0,0,0);
	f->Rot[1] = SetCoord(0,0,0);
	f->Rot[2] = SetCoord(0,0,0);
	f->Trl = SetCoord(0,0,0);
}


// �A��1���������̉������߂�
// ���� n:�s��  ax=b�ŁA����x�ɓ���
// �Ԓl �s��
double Gauss(int n,Matrix a,Vector b,Vector x)
{
	long double det;	// �s��
	int *ip;			// �s�����̏��

	ip = (int *)malloc(sizeof(int)*n);
	if(ip == NULL){
		SetMessage("fail to allocate");
		return KOD_ERR;
	}

	det = LU(n,a,ip);					// LU����
	if(det == 0) return KOD_FALSE;		// �s�񎮂�0
	else LU_Solver(n,a,b,ip,x);	// LU�����̌��ʂ��g���ĘA��������������

	free(ip);                   

	return det;					// �߂�l�͍s��
}

// �A��1���������̉������߂�
// ���� n:�s��  ax=b�ŁA����x�ɓ���
// �Ԓl �s��
double Gauss(int n,Matrix a,Coord *b,Coord *x)
{
	long double det;	// �s��
	int *ip;			// �s�����̏��

	ip = (int *)malloc(sizeof(int)*n);
	if(ip == NULL){
		SetMessage("fail to allocate");
		return KOD_ERR;
	}

	det = LU(n,a,ip);					// LU����
	if(det == 0) return KOD_FALSE;		// �s�񎮂�0
	else LU_Solver(n,a,b,ip,x);	// LU�����̌��ʂ��g���ĘA��������������

	free(ip);                   

	return det;					// �߂�l�͍s��
}

// LU�����̌��ʂ���A��1��������������
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

// LU�����̌��ʂ���A��1��������������
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

// �t�s������߂�
// ����	n:�s(��)��	a:���̍s��	a_inv:�s��a�̋t�s��
// �Ԓl	�s��
double MatInv(int n,Matrix a,Matrix a_inv)
{
	int i, j, k, ii;
	long double t, det;
	int *ip;		// �s�����̏��

	ip = (int *)malloc(sizeof(int)*n);
	if (ip==NULL){
		SetMessage("fail to allocate");
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

	free(ip);

	return det;
}

// LU����
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

// 3x3�̋t�s��
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

// 2�~2�̋t�s��
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

// 2���W��(nCr�̑g��������)�����߂�
int nCr(int n,int r)
{
	int p=1;
	for(int i=1;i<=r;i++)
		p *= (n-i+1)/i;

	return p;
}

// ���R��n�̊K������߂�
// �����ɒ��ӂ��邱��
// int�^�̏ꍇ�A10!�ŃI�[�o�[�t���[���Ă��܂�
int Factorial(int n)
{
	int ans = 1;
	for(int i=1;i<=n;i++)
		ans *= i;
	return ans;
}

// �J�[���[�X�e�[�^�X
void SetColorStat(DispStat *ds,float r, float g, float b, float a)
{
	ds->Color[0] = r;
	ds->Color[1] = g;
	ds->Color[2] = b;
	ds->Color[3] = a;
}