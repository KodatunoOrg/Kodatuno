#include <stdlib.h>
#include "StdAfx.h"

// ���W�l�̏�����
void InitCoord(Coord *a)
{
	a->x = 0;
	a->y = 0;
	a->z = 0;
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

// ���W�l���m�̊���Z
Coord DivCoord(Coord a,Coord b)
{
	Coord ans;

	ans.x = a.x / b.x;
	ans.y = a.y / b.y;
	ans.z = a.z / b.z;

	return ans;
}
Coord DivCoord(Coord a,double b)
{
	Coord ans;

	ans.x = a.x / b;
	ans.y = a.y / b;
	ans.z = a.z / b;

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

// ���W�l�������Ȃ�TRUE�A�قȂ��Ă���Ȃ�FALSE��Ԃ�
int DiffCoord(Coord a,Coord b)
{
	if(a.x == b.x && a.y == b.y && a.z == b.z){
		return TRUE;
	}
	else{
		return FALSE;
	}
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

// double�^2�����z��̃������[�m��
Matrix NewMatrix(int row,int col)
{
	int i;
	Matrix a;

	if((a = (double **)malloc((row+1)*sizeof(double *))) == NULL){
		return NULL;
	}
	for(i=0;i<row;i++){
		if((a[i] = (double *)malloc(col*sizeof(double))) == NULL){
			while(--i>=0) free(a[i]);
			free(a);
			return NULL;
		}
	}
	a[row] = NULL;
	
	return a;
}

// double�^2�����z��̃������[���
void FreeMatrix(Matrix a)
{
	Matrix b;

	b=a;
	while(*b != NULL) free(*b++);
	free(a);
}

// 2����Coord�^�z��̃������[�m��
Coord **NewCoord(int row,int col)
{
	int i;
	Coord **a;

	if((a = (Coord **)malloc((row+1)*sizeof(Coord *))) == NULL){
		return NULL;
	}
	for(i=0;i<row;i++){
		if((a[i] = (Coord *)malloc(col*sizeof(Coord))) == NULL){
			while(--i>=0) free(a[i]);
			free(a);
			return NULL;
		}
	}
	a[row] = NULL;

	return a;
}

// 2����Coord�^�z��̃������[���
void FreeCoord(Coord **a)
{
	Coord **b;

	b=a;
	while(*b != NULL) free(*b++);
	free(a);
}

// �x�N�g���𐳋K������
Coord NormalizeVec(Coord a)
{
	double len=0;

	len = CalcEuclid(a);

	return DivCoord(a,len);
}

// ���[�N���b�h�������Z�o
double CalcEuclid(Coord a)
{
	return sqrt(a.x*a.x + a.y*a.y + a.z*a.z);
}

// 2�������W��ł�2�_�Ԃ̃��[�N���b�h�������Z�o
double CalcEuclidPnts2D(Coord a,Coord b)
{
	return sqrt((a.x-b.x)*(a.x-b.x) + (a.y-b.y)*(a.y-b.y));
}

// 3�������W��ł�2�_�Ԃ̃��[�N���b�h�������Z�o
double CalcEuclidPnts3D(Coord a,Coord b)
{
	return sqrt((a.x-b.x)*(a.x-b.x) + (a.y-b.y)*(a.y-b.y) + (a.z-b.z)*(a.z-b.z));
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

// �C�ӂ̃x�N�g������]�������Ƃ��̉�]�p(radian)�����߂�(2D����)
double Rot_Vec_Ang(Coord a, Coord b)
{
	double angle,sin,cos;
	
	sin = (a.x*b.y - b.x*a.y)/(a.x*a.x + a.y*a.y);
	cos = (a.x*b.x + a.y*b.y)/(a.x*a.x + a.y*a.y);

	angle = atan2(sin,cos);
	if(angle < 0) angle = angle + 2.0*PI;
	
	return angle;
}

// �C�ӂ̃x�N�g������]�������x�N�g�������߂�(2D����)
Coord Rot_Vec(Coord a, double angle)
{
	Coord ans;
	
	ans.x = a.x*cos(angle) - a.y*sin(angle);
	ans.y = a.x*sin(angle) + a.y*cos(angle);

	return ans;
}

// �Ȗʂ�u,v�����̐ڐ��x�N�g������@���x�N�g�������߂�
Coord NormalVector(Coord a,Coord b)
{
	Coord ans;
	
	ans.x = a.y*b.z - a.z*b.y;
	ans.y = a.z*b.x - a.x*b.z;
	ans.z = a.x*b.y - a.y*b.x;
	
	return ans;
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

// 3��������������
// ����  *a:4�̌W��(a[0]x^3 + a[1]x^2 + a[2]x + a[3])   *ans:3�̉�
// �Ԓl�@����3�Ƃ������̏ꍇ��3�A1���������̏ꍇ��1  a[0]==0�̏ꍇ��ERR
int CalcCubicEquation(double *a,double *ans)
{
	double Q,R,r;

	if(fabs(a[0]) > APPROX_ZERO){
		a[1] /= a[0];
		a[2] /= a[0];
		a[3] /= a[0];
	}
	else{
		a[0] = a[1];
		a[1] = a[2];
		a[2] = a[3];
		return(CalcQuadraticEquation(a,ans));
	}

	Q = (a[1]*a[1]-3*a[2])/9;
	R = (2*pow(a[1],3)-9*a[1]*a[2]+27*a[3])/54;
	

	if(Q*Q*Q-R*R >= 0){
		r = acos(R/sqrt(Q*Q*Q));
		ans[0] = -2*sqrt(Q)*cos(r/3)-a[1]/3;
		ans[1] = -2*sqrt(Q)*cos((r+2*PI)/3)-a[1]/3;
		ans[2] = -2*sqrt(Q)*cos((r+4*PI)/3)-a[1]/3;
		return 3;
	}
	else{
		double p = pow(sqrt(R*R-Q*Q*Q)+fabs(R),1/3);
		ans[0] = -sgn(R)*(p + Q/p)-a[1]/3;
		ans[1] = -1;
		ans[2] = -1;
		return 1;
	}
}

// 2��������������
// ����  *a:3�̌W��(a[0]x^2 + a[1]x + a[2])  *ans:2�̉�
// �Ԓl  ���������̏ꍇ��2�A�����̏ꍇ��ERR  a[0]==0�̏ꍇ��ERR
int CalcQuadraticEquation(double *a,double *ans)
{
	double Q,R;

	if(fabs(a[0]) < APPROX_ZERO){
		a[0] = a[1];
		a[1] = a[2];
		return(CalcLinearEquation(a,ans));
	}

	Q = a[1]*a[1] - 4*a[0]*a[2];

	if(Q<0){
		ans[0] = ans[1] = 0;
		return ERR;
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
// �Ԓl  a[0]==0�̏ꍇ��ERR
int CalcLinearEquation(double *a,double *ans)
{
	if(fabs(a[0]) < APPROX_ZERO){
		return ERR;
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

// ���ړ_�̑��p�`���O����
// ����	  TargetPoint:���ړ_  BorderPoint:���p�`�̒��_�Q   CountPoint:���_�̐�
// �Ԓl	  TRUE:��  FALSE:�O
bool IsPointInPolygon(Coord TargetPoint,Coord *BorderPoint,int CountPoint)
{
	int i;
	int iCountCrossing = 0;				// ���O����J�E���^
	Coord p0;					// ���p�`�̈��(�x�N�g��)�̎n�_
	Coord p1;					// ���p�`�̈��(�x�N�g��)�̏I�_
	
	p0 = SetCoord(BorderPoint[0]);			// ���E�����[�v(���p�`)�̎n�_��p��
	bool bFlag0x = (TargetPoint.x <= p0.x);		// �Ώۓ_��x���W�Ƌ��E���̎n�_(���p�`�̈�ڂ̕ӂ̎n�_)��x���W�̑召��r
	bool bFlag0y = (TargetPoint.y <= p0.y);		// �Ώۓ_��y���W�Ƌ��E���̎n�_(���p�`�̈�ڂ̕ӂ̎n�_)��y���W�̑召��r
	
	// ���O���肷��_�ɑ΂��Ă��̓_����L�т锼�����ɂ����O������s��(�������̕����́A�w�v���X����)
	for(i=1;i<CountPoint+1;i++)
	{
		p1 = SetCoord(BorderPoint[i%CountPoint]);	// �Ō�͎n�_������i���p�`�f�[�^�̎n�_�ƏI�_����v���Ă��Ȃ��f�[�^�Ή��j
		
		bool bFlag1x = (TargetPoint.x <= p1.x);		// �Ώۓ_��x���W�Ƌ��E���̏I�_(���p�`�̂����ӂ̏I�_)��x���W�̑召��r
		bool bFlag1y = (TargetPoint.y <= p1.y);		// �Ώۓ_��y���W�Ƌ��E���̏I�_(���p�`�̂����ӂ̏I�_)��y���W�̑召��r

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

	// �N���X�J�E���g���[���̂Ƃ��O(false)�A�[���ȊO�̂Ƃ���(true)�B
	return (0 != iCountCrossing);
}


// �s�񓯎m�̊|���Z
void MulMxMx(Matrix A,int A_row,int A_col,Matrix B,int B_row,int B_col,Matrix C)
{
	for(int i=0;i<A_row;i++){
		for(int j=0;j<B_col;j++){
			C[i][j] = 0;
			for(int k=0;k<A_col;k++){
				C[i][j] += A[i][k] * B[k][j];
			}
		}
	}
}

// �s��ƃx�N�g���̊|���Z
void MulMxVec(Matrix A,int A_row,int A_col,Vector B,int B_row,Vector C)
{
	for(int i=0;i<A_row;i++){
		C[i] = 0;
		for(int j=0;j<A_col;j++){
			C[i] += A[i][j] * B[j];
		}
	}
}

// �����ϊ�(��]+���i�ړ�)
Coord HomoTransform(double R[][3],double T[],Coord I)
{
	Coord O;
	
	O.x = R[0][0]*I.x + R[0][1]*I.y + R[0][2]*I.z + T[0];
	O.y = R[1][0]*I.x + R[1][1]*I.y + R[1][2]*I.z + T[1];
	O.z = R[2][0]*I.x + R[2][1]*I.y + R[2][2]*I.z + T[2];

	return O;
}
