#include <stdlib.h>
#include "StdAfx.h"

// 座標値の初期化
void InitCoord(Coord *a)
{
	a->x = 0;
	a->y = 0;
	a->z = 0;
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

// 座標値同士の割り算
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

// 座標値が同じならTRUE、異なっているならFALSEを返す
int DiffCoord(Coord a,Coord b)
{
	if(a.x == b.x && a.y == b.y && a.z == b.z){
		return TRUE;
	}
	else{
		return FALSE;
	}
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

// double型2次元配列のメモリー確保
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

// double型2次元配列のメモリー解放
void FreeMatrix(Matrix a)
{
	Matrix b;

	b=a;
	while(*b != NULL) free(*b++);
	free(a);
}

// 2次元Coord型配列のメモリー確保
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

// 2次元Coord型配列のメモリー解放
void FreeCoord(Coord **a)
{
	Coord **b;

	b=a;
	while(*b != NULL) free(*b++);
	free(a);
}

// ベクトルを正規化する
Coord NormalizeVec(Coord a)
{
	double len=0;

	len = CalcEuclid(a);

	return DivCoord(a,len);
}

// ユークリッド距離を算出
double CalcEuclid(Coord a)
{
	return sqrt(a.x*a.x + a.y*a.y + a.z*a.z);
}

// 2次元座標上での2点間のユークリッド距離を算出
double CalcEuclidPnts2D(Coord a,Coord b)
{
	return sqrt((a.x-b.x)*(a.x-b.x) + (a.y-b.y)*(a.y-b.y));
}

// 3次元座標上での2点間のユークリッド距離を算出
double CalcEuclidPnts3D(Coord a,Coord b)
{
	return sqrt((a.x-b.x)*(a.x-b.x) + (a.y-b.y)*(a.y-b.y) + (a.z-b.z)*(a.z-b.z));
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

// 任意のベクトルを回転させたときの回転角(radian)を求める(2D平面)
double Rot_Vec_Ang(Coord a, Coord b)
{
	double angle,sin,cos;
	
	sin = (a.x*b.y - b.x*a.y)/(a.x*a.x + a.y*a.y);
	cos = (a.x*b.x + a.y*b.y)/(a.x*a.x + a.y*a.y);

	angle = atan2(sin,cos);
	if(angle < 0) angle = angle + 2.0*PI;
	
	return angle;
}

// 任意のベクトルを回転させたベクトルを求める(2D平面)
Coord Rot_Vec(Coord a, double angle)
{
	Coord ans;
	
	ans.x = a.x*cos(angle) - a.y*sin(angle);
	ans.y = a.x*sin(angle) + a.y*cos(angle);

	return ans;
}

// 曲面のu,v方向の接線ベクトルから法線ベクトルを求める
Coord NormalVector(Coord a,Coord b)
{
	Coord ans;
	
	ans.x = a.y*b.z - a.z*b.y;
	ans.y = a.z*b.x - a.x*b.z;
	ans.z = a.x*b.y - a.y*b.x;
	
	return ans;
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

// 3次方程式を解く
// 引数  *a:4つの係数(a[0]x^3 + a[1]x^2 + a[2]x + a[3])   *ans:3つの解
// 返値　解が3つとも実根の場合は3、1つだけ実根の場合は1  a[0]==0の場合はERR
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

// 2次方程式を解く
// 引数  *a:3つの係数(a[0]x^2 + a[1]x + a[2])  *ans:2つの解
// 返値  解が実根の場合は2、虚根の場合はERR  a[0]==0の場合はERR
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

// 1次方程式を解く
// 引数  *a:2つの係数(a[0]x + a[1])  *ans:解
// 返値  a[0]==0の場合はERR
int CalcLinearEquation(double *a,double *ans)
{
	if(fabs(a[0]) < APPROX_ZERO){
		return ERR;
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

// 注目点の多角形内外判別
// 引数	  TargetPoint:注目点  BorderPoint:多角形の頂点群   CountPoint:頂点の数
// 返値	  TRUE:内  FALSE:外
bool IsPointInPolygon(Coord TargetPoint,Coord *BorderPoint,int CountPoint)
{
	int i;
	int iCountCrossing = 0;				// 内外判定カウンタ
	Coord p0;					// 多角形の一辺(ベクトル)の始点
	Coord p1;					// 多角形の一辺(ベクトル)の終点
	
	p0 = SetCoord(BorderPoint[0]);			// 境界線ループ(多角形)の始点を用意
	bool bFlag0x = (TargetPoint.x <= p0.x);		// 対象点のx座標と境界線の始点(多角形の一つ目の辺の始点)のx座標の大小比較
	bool bFlag0y = (TargetPoint.y <= p0.y);		// 対象点のy座標と境界線の始点(多角形の一つ目の辺の始点)のy座標の大小比較
	
	// 内外判定する点に対してその点から伸びる半直線により内外判定を行う(半直線の方向は、Ｘプラス方向)
	for(i=1;i<CountPoint+1;i++)
	{
		p1 = SetCoord(BorderPoint[i%CountPoint]);	// 最後は始点が入る（多角形データの始点と終点が一致していないデータ対応）
		
		bool bFlag1x = (TargetPoint.x <= p1.x);		// 対象点のx座標と境界線の終点(多角形のある一辺の終点)のx座標の大小比較
		bool bFlag1y = (TargetPoint.y <= p1.y);		// 対象点のy座標と境界線の終点(多角形のある一辺の終点)のy座標の大小比較

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

	// クロスカウントがゼロのとき外(false)、ゼロ以外のとき内(true)。
	return (0 != iCountCrossing);
}


// 行列同士の掛け算
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

// 行列とベクトルの掛け算
void MulMxVec(Matrix A,int A_row,int A_col,Vector B,int B_row,Vector C)
{
	for(int i=0;i<A_row;i++){
		C[i] = 0;
		for(int j=0;j<A_col;j++){
			C[i] += A[i][j] * B[j];
		}
	}
}

// 同次変換(回転+並進移動)
Coord HomoTransform(double R[][3],double T[],Coord I)
{
	Coord O;
	
	O.x = R[0][0]*I.x + R[0][1]*I.y + R[0][2]*I.z + T[0];
	O.y = R[1][0]*I.x + R[1][1]*I.y + R[1][2]*I.z + T[1];
	O.z = R[2][0]*I.x + R[2][1]*I.y + R[2][2]*I.z + T[2];

	return O;
}
