#include <stdlib.h>
#include "StdAfx.h"


// Coordクラスのオペレータのオーバーロード
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

// 座標値の初期化
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

// 座標値同士の足し算
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

// 座標値同士の足し算 (2D Ver.)
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

// 座標値同士の割り算
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

// 座標値同士の割り算 (2D Ver.)
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

// 座標値同士の引き算
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

// 座標値同士の引き算 (2D Ver.)
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

// 座標値同士の掛け算
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

// 座標値同士の掛け算 (2D Ver.)
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

// 座標値が同じならKOD_TRUE、異なっているならKOD_FALSEを返す
int DiffCoord(Coord a,Coord b)
{
	if(fabs(a.x-b.x) <= APPROX_ZERO && fabs(a.y-b.y) <= APPROX_ZERO && fabs(a.z-b.z) <= APPROX_ZERO)
	//if(fabs(a.x-b.x) == 0 && fabs(a.y-b.y) == 0 && fabs(a.z-b.z) == 0)
		return KOD_TRUE;
	else
		return KOD_FALSE;
}
// オーバーロード(精度指定)
int DiffCoord(Coord a,Coord b,double App)
{
	if(fabs(a.x-b.x) <= App && fabs(a.y-b.y) <= App && fabs(a.z-b.z) <= App)
		return KOD_TRUE;
	else
		return KOD_FALSE;
}

// 2D平面での座標値が同じならKOD_TRUE、異なっているならKOD_FALSEを返す
int DiffCoord2D(Coord a,Coord b)
{
	if(fabs(a.x-b.x) <= APPROX_ZERO && fabs(a.y-b.y) <= APPROX_ZERO)
	//if(fabs(a.x-b.x) == 0 && fabs(a.y-b.y) == 0 && fabs(a.z-b.z) == 0)
		return KOD_TRUE;
	else
		return KOD_FALSE;
}
// オーバーロード(精度指定)
int DiffCoord2D(Coord a,Coord b,double App)
{
	if(fabs(a.x-b.x) <= App && fabs(a.y-b.y) <= App)
	//if(fabs(a.x-b.x) == 0 && fabs(a.y-b.y) == 0 && fabs(a.z-b.z) == 0)
		return KOD_TRUE;
	else
		return KOD_FALSE;
}

// 座標値の絶対値を返す
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

// 座標値を代入する
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

// 座標値群のコピー(b<--a)
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

// (0,0,0)のときKOD_FALSEを返す
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

// double型1次元配列のメモリー確保
Vector NewVector(int len)
{
	Vector a;
	if((a = (double *)malloc(len*sizeof(double))) == NULL){
		SetMessage("fail to allocate memoly");
		return NULL;
	}

	return a;
}

// double型2次元配列のメモリー確保
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

// double型1次元配列のメモリー解放
void FreeVector(Vector a)
{
	free(a);
}

// double型2次元配列のメモリー解放
void FreeMatrix(Matrix a,int col)
{
	for(int i=0;i<col;i++)
		free(a[i]);

	free(a);
}

// 1次元Coord型配列のメモリー確保
Coord *NewCoord1(int len)
{
	Coord *a;

	if((a = (Coord *)malloc(len*sizeof(Coord))) == NULL){
		SetMessage("fail to allocate memoly");
		return NULL;
	}

	return a;
}

// 2次元Coord型配列のメモリー確保
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

// 1次元Coord型配列のメモリー解放 
void FreeCoord1(Coord *a)
{
	free(a);
}

// 2次元Coord型配列のメモリー解放
void FreeCoord2(Coord **a,int col)
{
	Coord **b;

	b=a;
	for(int i=0;i<col;i++){
		free(b[i]);
	}
	free(a);
}

// ベクトルを正規化する
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

// ユークリッド距離を算出
double CalcEuclid(Coord a)
{
	return sqrt(a.x*a.x + a.y*a.y + a.z*a.z);
}

// 2次元ユークリッド距離を算出
double CalcEuclid2D(double a,double b)
{
	return sqrt(a*a+b*b);
}

// 2点間のユークリッド距離を求める
double CalcDistance(Coord a,Coord b)
{
	return(CalcEuclid(SubCoord(a,b)));
}

// 2次元座標上での2点間のユークリッド距離を算出
double CalcDistance2D(Coord a,Coord b)
{
	return sqrt((a.x-b.x)*(a.x-b.x) + (a.y-b.y)*(a.y-b.y));
}

// 内積を求める
double CalcInnerProduct(Coord a,Coord b)
{
	return(a.x*b.x+a.y*b.y+a.z*b.z);
}


// 内積を求める(オーバーロード)
double CalcInnerProduct(Coord a,double x,double y,double z)
{
	return(a.x*x+a.y*y+a.z*z);
}

// 外積を求める
Coord CalcOuterProduct(Coord a,Coord b)
{
	Coord c;

	c.x = a.y*b.z - a.z*b.y;
	c.y = a.z*b.x - a.x*b.z;
	c.z = a.x*b.y - a.y*b.x;

	return c;
}

// 外積を求める (2D Ver.)
double CalcOuterProduct2D(Coord a,Coord b)
{
	return(a.x*b.y - a.y*b.x);
}

// 2つのベクトルのなす角を求める(返値はrad)
double CalcVecAngle(Coord a,Coord b)
{
	double inn = CalcInnerProduct(a,b);
	double abs_a = CalcEuclid(a);
	double abs_b = CalcEuclid(b);

	return(acos(inn/abs_a/abs_b));
}

// 2点p(t=0),q(t=1)をt(0～1)で内分したときの点の座標をもとめる
Coord CalcInterDivPt(Coord p,Coord q,double t)
{
	return(AddCoord(p,MulCoord(SubCoord(q,p),t)));
}

// 任意の点を任意の平面へ正射影する
// 引数   p:任意の平面上の点,  n:任意の平面の単位法線ベクトル,  q:正射影したい点
// 返値   正射影された点の座標値
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

// 任意の点から任意の平面までの距離を求める
// 引数 Pt:任意の点  P0:平面上の1点  N:平面の法線ベクトル
double CalcDistPtToPlane(Coord Pt,Coord P0,Coord N)
{
	return((fabs(N.x*Pt.x + N.y*Pt.y + N.z*Pt.z - (N.x*P0.x + N.y*P0.y + N.z*P0.z)))/CalcEuclid(N));
}

// スカラー三重積を求める
double CalcScalarTriProduct(Coord a,Coord b,Coord c)
{
	return(CalcInnerProduct(a,CalcOuterProduct(b,c)));
}

// 点を描画
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

// 点群を描画
// 引数   *p:点群, n:点数, scale:スケール, width:点のサイズ, color,点の色
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

// ベクトルを描画
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

// 2点間に線分を描画
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

// 角度をdegreeからradianへ変換
double DegToRad(double degree)
{
	double radian;

	radian = degree * PI / 180.0;

	return radian;	
}

// 角度をradianからdegreeへ変換
double RadToDeg(double radian)
{
	double degree = 0.0;

	degree = radian * 180.0 / PI;

	return degree;
}

// 円の中心点(vec[0])から円上に接する任意の2本の接線が交わる点へのベクトル(中心角0<θ<π)
Coord Arc_CP(Coord a, Coord b, double cos)
{
	Coord ans;

	ans.x = (a.x + b.x)/(1 + cos);
	ans.y = (a.y + b.y)/(1 + cos);
	ans.z = (a.z + b.z)/(1 + cos);

	return ans;
}

// 2つのベクトルのなす角を求める(2D平面)
double CalcVecAngle2D(Coord a, Coord b)
{
	double angle,sin,cos;

	sin = (a.x*b.y - b.x*a.y)/(a.x*a.x + a.y*a.y);
	cos = (a.x*b.x + a.y*b.y)/(a.x*a.x + a.y*a.y);

	angle = atan2(sin,cos);
	if(angle < 0) angle = angle + 2.0*PI;

	return angle;
}

// 任意のベクトルを回転させたベクトルを求める(2D平面)
Coord CalcRotVec2D(Coord a, double angle)
{
	Coord ans;

	ans.x = a.x*cos(angle) - a.y*sin(angle);
	ans.y = a.x*sin(angle) + a.y*cos(angle);

	return ans;
}

// 任意のベクトルを原点を通る任意軸周りに回転させたベクトルを求める(3D平面)
// 引数  a:回転させたいベクトル  e:原点を通る任意軸(単位ベクトルであること)  ang:回転角(rad)
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

// 任意の点Pから任意の直線(点Aを通り単位ベクトルuの方向を持つ)へ下ろした点を求める
Coord CalcNormalLine(Coord P,Coord A,Coord u)
{
	double k = CalcInnerProduct(SubCoord(P,A),u);

	return(AddCoord(A,MulCoord(u,k)));
}

// バブルソート(基本交換法)
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

// クイックソート(double型、降順)で用いる比較関数
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

// Coord配列の反転操作
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

// double型配列の反転操作
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


// 1次元配列の初期化
void InitVector(Vector vec,int size)
{
	for(int i=0;i<size;i++){
		vec[i] = 0.0;
	}
}

// 2次元配列の初期化
void InitMatrix(Matrix mat,int size_x,int size_y)
{
	for(int i=0;i<size_x;i++){
		for(int j=0;j<size_y;j++){
			mat[i][j] = 0.0;
		}
	}
}

// ベクトルのコピー
// aベクトルをbベクトルへ代入
void CopyVector(Vector a,int n,Vector b)
{
	for(int i=0;i<n;i++)
		b[i] = a[i];
}

// 3次方程式を解く
// 引数  *a:4つの係数(a[0]x^3 + a[1]x^2 + a[2]x + a[3])   *ans:3つの解
// 返値　解が3つとも実根の場合は3、1つだけ実根の場合は1  a[0]==0の場合はKOD_ERR
int CalcCubicEquation(double *p,double *ans)
{
	// x^3の係数が0の場合
	if(fabs(p[0]) < APPROX_ZERO_H){
		p[0] = p[1];
		p[1] = p[2];
		p[2] = p[3];
		return(CalcQuadraticEquation(p,ans));	// 2次方程式を解く
	}

	double a = p[0];
	double b = p[1];
	double c = p[2];
	double d = p[3];
	double x0[3];
	int k;
	int ansnum=0;

	double D = b*b-3*a*c;		// 1階微分された2次方程式の判別式
	if(D<0){					// 判別式が負-->極値無し
		ansnum = 1;				// 解は1つ
		x0[0] = 1;				// ニュートン法での初期値を1に決め打ち
	}
	else if(D==0.0){			// 判別式がゼロ-->変極点が1つある
		ansnum = 1;				// 解は1つ
		x0[0] = -b/3/a;			// ニュートン法での初期値は変極点とする
	}
	else {									// 判別式が正-->極大極小あり
		double x1 = (-b-sqrt(D))/(3*a);		// 極点を求める
		double x2 = (-b+sqrt(D))/(3*a);
		if(x1 > x2){						// x1 < x2とする
			double dmy = x1;
			x1 = x2;
			x2 = dmy;
		}
		double y1 = ((a*x1+b)*x1+c)*x1+d;	// x1のときのy1を求める
		double y2 = ((a*x2+b)*x2+c)*x2+d;	// x2のときのy2を求める
		if(y1*y2 < 0.0){					// y1とy2の符号が異なる場合
			ansnum = 3;						// 解は3つあるといえる
			x0[0] = x1 - 1;					// ニュートン法の初期値を各解付近となるよう設定
			x0[1] = (x1+x2)/2;
			x0[2] = x2 + 1;
		}
		else if(y1 == 0.0 || y2 == 0.0){	// y1,y2どちらかがゼロの場合
			ansnum = 2;						// 解は2つ
			x0[0] = x1 - 1;					// ニュートン法の初期値を各解付近となるよう設定
			x0[1] = x2 + 1;
		}
		else{								// y1,y2が同符号の場合
			ansnum = 1;						// 解は1つ
			if(y1 < 0.0)					// 符号が負の場合
				x0[0] = x2 + 1;				// ニュートン法の初期値はx2近傍のはず
			else							// 符号が正の場合
				x0[0] = x1 - 1;				// ニュートン法の初期値はx1近傍のはず
		}
	}
	//fprintf(stderr,"ans num = %d\n",ansnum);

	// ニュートン法により解を探索
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

// 2次方程式を解く
// 引数  *a:3つの係数(a[0]x^2 + a[1]x + a[2])  *ans:2つの解
// 返値  解が実根の場合は2、虚根の場合はKOD_ERR  a[0]==0の場合はKOD_ERR
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

// 1次方程式を解く
// 引数  *a:2つの係数(a[0]x + a[1])  *ans:解
// 返値  a[0]==0の場合はKOD_ERR
int CalcLinearEquation(double *a,double *ans)
{
	if(fabs(a[0]) < APPROX_ZERO_H){
		return KOD_FALSE;
	}

	ans[0] = -a[1]/a[0];

	return 1;
}

// 符号判定
double sgn(double x)
{
	if(x<0)	return -1;
	else if(x == 0) return 0;
	else return 1;
}

// 値がAPPROX_ZEROの範囲で0であるかチェック
// -APPROX_ZERO < val < APPROX_ZERO
// 引数 val:入力値  flag:精度(HIGH_ACCURACY or MID_ACCURACY or LOW_ACCURACY)
// 帰値   KOD_TRUE:範囲内でゼロとみなせる     KOD_FALSE:範囲外     KOD_ERR:引数のflag指定が間違っている
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

// 指定した値が指定した範囲内であるかをチェック
// 引数  low:下限  up:上限   val:調べたい値
//       flag = 0:(low <= val <= up) --> (low-ap < val < up+ap), 
//              1:(low < val < up) --> (low+ap < val < up-ap),
//              2:(val <= up) --> (val < up+ap),
//              3:(val < up) --> (val < up-ap),
//              4:(low <= val) --> (low-ap < val),
//              5:(low < val) --> (low+ap < val)
// 注意　valがAPPROX_ZERO(ap)内でlowまたはupと一致する場合は、範囲内にあるものとする
// 返値  KOD_TRUE:範囲内　　KOD_FALSE:範囲外　　　KOD_ERR:flagに指定した値が0,1以外
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

// 2つの値の大小比較
// 引数  val1,val2:入力値   flag:精度(HIGH_ACCURACY or or MID_ACCURACY or LOW_ACCURACY)
// 返値　KOD_EQUAL: val1 = val2 (|va1-val2| < APPROX_ZERO)
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

// 注目点の多角形内外判別(x-y平面内)
// 引数	  TargetPoint:注目点  BorderPoint:多角形の頂点群   CountPoint:頂点の数
// 返値	  KOD_TRUE:内  KOD_FALSE:外  KOD_ONEDGE:エッジ上
int IsPointInPolygon(Coord TargetPoint,Coord *BorderPoint,int CountPoint)
{
	int i;
	int iCountCrossing = 0;				// 内外判定カウンタ
	Coord p0;					// 多角形の一辺(ベクトル)の始点
	Coord p1;					// 多角形の一辺(ベクトル)の終点

	p0 = SetCoord(BorderPoint[0]);			// 境界線ループ(多角形)の始点を用意
	bool bFlag0x = (TargetPoint.x <= p0.x);	// 対象点のx座標と境界線の始点(多角形の一つ目の辺の始点)のx座標の大小比較
	bool bFlag0y = (TargetPoint.y <= p0.y);		// 対象点のy座標と境界線の始点(多角形の一つ目の辺の始点)のy座標の大小比較

	// 内外判定する点に対してその点から伸びる半直線により内外判定を行う(半直線の方向は、Ｘプラス方向)
	for(i=1;i<CountPoint+1;i++)
	{
		p1 = SetCoord(BorderPoint[i%CountPoint]);	// 最後は始点が入る（多角形データの始点と終点が一致していないデータ対応）

		double a = (p1.x - p0.x)*(p1.y - TargetPoint.y) - (p1.y - p0.y)*(p1.x - TargetPoint.x);		// TargetPointがエッジ上(p0とp1の線上)にあるかチェック
		if(CheckZero(a,MID_ACCURACY) == KOD_TRUE)			
		//if(a == 0)					// エッジ上なら
			return KOD_ONEDGE;		// 問答無用でreturn

		bool bFlag1x = (TargetPoint.x <= p1.x);		
		bool bFlag1y = (TargetPoint.y <= p1.y);	

		if(bFlag0y != bFlag1y){			// 線分は半直線を横切る可能性あり
			if(bFlag0x == bFlag1x){		// 線分の２端点は対象点に対して両方右か両方左にある
				if(bFlag0x){			// 完全に右、線分は半直線を横切る
					iCountCrossing += (bFlag0y ? -1 : 1);	// 上から下に半直線を横切るときには、交差回数を１引く、下から上は１足す。
				}
			}
			else{					// 半直線と交差するかどうか、対象点と同じ高さで、対象点の右で交差するか、左で交差するかを求める。
				if(TargetPoint.x <= (p0.x + (p1.x - p0.x)*(TargetPoint.y - p0.y )/(p1.y - p0.y))){	// 線分は、対象点と同じ高さで、対象点の右で交差する。線分は半直線を横切る
					iCountCrossing += (bFlag0y ? -1 : 1);	// 上から下に半直線を横切るときには、交差回数を１引く、下から上は１足す。
				}
			}
		}

		// 次の判定のための準備(終点だったものを次の辺の始点へ)
		p0 = p1;
		bFlag0x = bFlag1x;
		bFlag0y = bFlag1y;
	}

	// クロスカウントがゼロのとき外(KOD_FALSE)、ゼロ以外のとき内(KOD_TRUE)。
	if(iCountCrossing)
		return KOD_TRUE;
	else
		return KOD_FALSE;
}

// 空間上の3点からなる平面の法線ベクトルを求める
Coord CalcNormVecFrom3Pts(Coord p1,Coord p2,Coord p3)
{
	Coord denom = (p2-p1)&&(p3-p1);
	double numer = CalcEuclid(denom);

	return DivCoord(denom,numer);
}

// 空間上の多角形の面積を得る(頂点の数:Vnum, 頂点列：p[Vnum])
double CalcPolygonArea(Coord p[],int Vnum)
{
	double area=0;

	for(int i=0;i<Vnum;i++){
		area += CalcEuclid(CalcOuterProduct(p[i],p[(i+1)%Vnum]));
	}

	return(area/2);
}

// 2D平面上の多角形の符号付き面積を得る(頂点の数:Vnum, 頂点列：p[Vnum])
// CCW：正    CW：負
double ClacPolygonArea2D(Coord p[],int Vnum)
{
	double area=0;

	for(int i=0;i<Vnum;i++){
		area += CalcOuterProduct2D(p[i],p[(i+1)%Vnum]);
	}

	return(area/2);
}

// 2D平面上の多角形が時計回りか反時計回りかを判別する
// 返値   CCW：KOD_TRUE     CW：KOD_FALSE
int DiscriminateCW2D(Coord p[],int Vnum)
{
	if(ClacPolygonArea2D(p,Vnum) > 0)	// CCW
		return KOD_TRUE;

	else	// CW
		return KOD_FALSE;
}

// 行列同士の足し算
void AddMxMx(Matrix A,Matrix B,Matrix C,int row,int col)
{
	for(int i=0;i<row;i++){
		for(int j=0;j<col;j++){
			C[i][j] = A[i][j] + B[i][j];
		}
	}
}


// 行列同士の掛け算
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

// 行列とベクトルの掛け算
// 引数  C = AB   A_row:行数  A_col:列数  B_row:ベクトルの次元数
void MulMxVec(Matrix A,int A_row,int A_col,Vector B,int B_row,Vector C)
{
	for(int i=0;i<A_row;i++){
		C[i] = 0;
		for(int j=0;j<A_col;j++){
			C[i] += A[i][j] * B[j];
		}
	}
}

// 行列と座標値ベクトルの掛け算
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

// Coordで表現される3x3行列とCoordベクトルとの掛け算
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

// 転置行列を得る
// 引数  **A:元の行列  m:Aの行数  n:Aの列数  **B:転置行列を格納
// 転置されるとmとnが逆になるので、Bのメモリー確保に注意!
void TranMx(Matrix A,int m,int n,Matrix B)
{
	for(int i=0;i<m;i++){
		for(int j=0;j<n;j++){
			B[j][i] = A[i][j];
		}
	}
}

// 転置行列を得る(オーバーロード)
// 引数  **A:元の行列  m:Aの行数  n:Aの列数  **B:転置行列を格納
void TranMx(Coord **A,int m,int n,Coord **B)
{
	for(int i=0;i<m;i++){
		for(int j=0;j<n;j++){
			B[j][i] = SetCoord(A[i][j]);
		}
	}
}

// 転置行列を得る(オーバーロード)
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

// 同次変換行列と座標値(3Dベクトル)との掛け算
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

// 同次変換行列の逆行列を得る
// F = |R T|    F^-1 = |R^-1 -R^-1*T|
//     |0 1|           |  0     1   |
FRAME InvFrame(FRAME F)
{
	FRAME f;

	TranMx(F.Rot,f.Rot);				// F.Rotの転置行列F.Rot^Tを得る
	f.Trl = MulMxCoord(f.Rot,F.Trl);	// F.Rot^T * F.Trl
	f.Trl = MulCoord(f.Trl,-1);			// -(F.Rot^T * F.Trl)

	return f;
}

// 同次変換行列の掛け算
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


// 回転行列からZYZオイラー角を算出（ tmp.x がα角(O), tmp.y がβ角(A), tmp.z がγ角(T) にそれぞれ対応 ）
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
		//tmp.z = atan2( rot[1].x, -rot[0].x );		// 元々こっちだったが、
		tmp.z = atan2( rot[1].x, rot[0].x );		// マイナスをなくした。
	}
	else{
		tmp.x = atan2( rot[2].y / sin( tmp.y ), rot[2].x / sin( tmp.y ) );
		tmp.z = atan2( rot[1].z / sin( tmp.y ), -rot[0].z / sin( tmp.y ) );
	}	
	tmp = MulCoord(tmp,180/PI);

	return tmp;
}

// FRAMEの初期化
void InitFrame(FRAME *f)
{
	f->Rot[0] = SetCoord(0,0,0);
	f->Rot[1] = SetCoord(0,0,0);
	f->Rot[2] = SetCoord(0,0,0);
	f->Trl = SetCoord(0,0,0);
}


// 連立1次方程式の解を求める
// 引数 n:行数  ax=bで、解はxに入る
// 返値 行列式
double Gauss(int n,Matrix a,Vector b,Vector x)
{
	long double det;	// 行列式
	int *ip;			// 行交換の情報

	ip = (int *)malloc(sizeof(int)*n);
	if(ip == NULL){
		SetMessage("fail to allocate");
		return KOD_ERR;
	}

	det = LU(n,a,ip);					// LU分解
	if(det == 0) return KOD_FALSE;		// 行列式が0
	else LU_Solver(n,a,b,ip,x);	// LU分解の結果を使って連立方程式を解く

	free(ip);                   

	return det;					// 戻り値は行列式
}

// 連立1次方程式の解を求める
// 引数 n:行数  ax=bで、解はxに入る
// 返値 行列式
double Gauss(int n,Matrix a,Coord *b,Coord *x)
{
	long double det;	// 行列式
	int *ip;			// 行交換の情報

	ip = (int *)malloc(sizeof(int)*n);
	if(ip == NULL){
		SetMessage("fail to allocate");
		return KOD_ERR;
	}

	det = LU(n,a,ip);					// LU分解
	if(det == 0) return KOD_FALSE;		// 行列式が0
	else LU_Solver(n,a,b,ip,x);	// LU分解の結果を使って連立方程式を解く

	free(ip);                   

	return det;					// 戻り値は行列式
}

// LU分解の結果から連立1次方程式を解く
void LU_Solver(int n,Matrix a,Vector b,int *ip,Vector x)
{
	int ii;
	double t;

	for(int i=0;i<n;i++) {       // Gauss消去法の残り
		ii = ip[i];
		t = b[ii];
		for(int j=0;j<i;j++)
			t -= a[ii][j]*x[j];
		x[i] = t;
	}
	for(int i=n-1;i>=0;i--){  // 後退代入
		t = x[i];  
		ii = ip[i];
		for(int j=i+1;j<n;j++) 
			t -= a[ii][j]*x[j];
		x[i] = t/a[ii][i];
	}
}

// LU分解の結果から連立1次方程式を解く
void LU_Solver(int n,Matrix a,Coord *b,int *ip,Coord *x)
{
	int ii;
	Coord t;

	for(int i=0;i<n;i++) {       // Gauss消去法の残り
		ii = ip[i];
		t = SetCoord(b[ii]);
		for(int j=0;j<i;j++)
			t = SubCoord(t,MulCoord(x[j],a[ii][j]));
		x[i] = SetCoord(t);
	}
	for(int i=n-1;i>=0;i--){  // 後退代入
		t = SetCoord(x[i]);  
		ii = ip[i];
		for(int j=i+1;j<n;j++) 
			t = SubCoord(t,MulCoord(x[j],a[ii][j]));
		x[i] = DivCoord(t,a[ii][i]);
	}
}

// 逆行列を求める
// 引数	n:行(列)数	a:元の行列	a_inv:行列aの逆行列
// 返値	行列式
double MatInv(int n,Matrix a,Matrix a_inv)
{
	int i, j, k, ii;
	long double t, det;
	int *ip;		// 行交換の情報

	ip = (int *)malloc(sizeof(int)*n);
	if (ip==NULL){
		SetMessage("fail to allocate");
		return KOD_ERR;
	}

	det = LU(n,a,ip);		// LU分解
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

// LU分解
double LU(int n,Matrix a,int *ip)
{
	int i, j, k, ii, ik;
	long double t, u, det;
	Vector weight;

	weight = NewVector(n);    /* weight[0..n-1] の記憶領域確保 */
	det = 0;                   /* 行列式 */
	for (k = 0; k < n; k++) {  /* 各行について */
		ip[k] = k;             /* 行交換情報の初期値 */
		u = 0;                 /* その行の絶対値最大の要素を求める */
		for (j = 0; j < n; j++) {
			t = fabs(a[k][j]);  if (t > u) u = t;
		}
		if (u == 0){
			goto EXIT; /* 0 なら行列はLU分解できない */
		}
		weight[k] = 1 / u;     /* 最大絶対値の逆数 */
	}
	det = 1;                   /* 行列式の初期値 */
	for (k = 0; k < n; k++) {  /* 各行について */
		u = -1;
		for (i = k; i < n; i++) {  /* より下の各行について */
			ii = ip[i];            /* 重み×絶対値 が最大の行を見つける */
			t = fabs(a[ii][k]) * weight[ii];
			if (t > u) {  u = t;  j = i;  }
		}
		ik = ip[j];
		if (j != k) {
			ip[j] = ip[k];  ip[k] = ik;  /* 行番号を交換 */
			det = -det;  /* 行を交換すれば行列式の符号が変わる */
		}
		u = a[ik][k];  det *= u;  /* 対角成分 */
		if (u == 0){
			goto EXIT;    /* 0 なら行列はLU分解できない */
		}
		for (i = k + 1; i < n; i++) {  /* Gauss消去法 */
			ii = ip[i];
			t = (a[ii][k] /= u);
			for (j = k + 1; j < n; j++)
				a[ii][j] -= t * a[ik][j];
		}
	}

EXIT:
	FreeVector(weight);  /* 記憶領域を解放 */
	return det;           /* 戻り値は行列式 */
}

// 3x3の逆行列
double MatInv3(Matrix A,Matrix A_inv)
{
	double det;
	det = A[0][0]*A[1][1]*A[2][2] + A[1][0]*A[2][1]*A[0][2] + A[2][0]*A[0][1]*A[1][2]
	      - A[0][0]*A[2][1]*A[1][2] - A[2][0]*A[1][1]*A[0][2] - A[1][0]*A[0][1]*A[2][2];

	if(det == 0) return KOD_FALSE;		// 行列式が0

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

// 2×2の逆行列
double MatInv2(Matrix A,Matrix A_inv)
{
	double det;

	det = A[0][0]*A[1][1] - A[0][1]*A[1][0];

	if(det == 0) return KOD_FALSE;		// 行列式が0

	A_inv[0][0] = A[1][1]/det;
	A_inv[0][1] = -A[0][1]/det;
	A_inv[1][0] = -A[1][0]/det;
	A_inv[1][1] = A[0][0]/det;

	return det;
}

// 2項係数(nCrの組合せ総数)を求める
int nCr(int n,int r)
{
	int p=1;
	for(int i=1;i<=r;i++)
		p *= (n-i+1)/i;

	return p;
}

// 自然数nの階乗を求める
// 桁数に注意すること
// int型の場合、10!でオーバーフローしてしまう
int Factorial(int n)
{
	int ans = 1;
	for(int i=1;i<=n;i++)
		ans *= i;
	return ans;
}

// カーラーステータス
void SetColorStat(DispStat *ds,float r, float g, float b, float a)
{
	ds->Color[0] = r;
	ds->Color[1] = g;
	ds->Color[2] = b;
	ds->Color[3] = a;
}