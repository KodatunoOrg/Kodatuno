#include <stdlib.h>
#include "StdAfx.h"


// Operator: +
// Coord同士の足し算(AddCoord())
Coord Coord::operator +(Coord a)
{
	return(AddCoord(*this,a));
}

// Operator: -
// Coord同士の引き算(SubCoord())
Coord Coord::operator -(Coord a)
{
	return(SubCoord(*this,a));
}

// Oeprator: *
// Coord同士の掛け算(MulCoord())
Coord Coord::operator *(Coord a)
{
	return(MulCoord(*this,a));
}

// Oeprator: *
// Coordとdoubleとの掛け算（オーバーロード）
Coord Coord::operator *(double a)
{
	return(MulCoord(*this,a));
}

// Operator: /
// Coord同士の割り算(DivCoord())
Coord Coord::operator /(Coord a)
{
	return(DivCoord(*this,a));
}

// Operator: /
// Coordとdoubleとの割り算（オーバーロード）
Coord Coord::operator /(double a)
{
	return(DivCoord(*this,a));
}

// Operator: &
// Coord同士の内積(CalcInnerProduct())
double Coord::operator &(Coord a)
{
	return(CalcInnerProduct(*this,a));
}

// Operator: &&
// Coord同士の外積(CalcOuterProduct())
Coord Coord::operator &&(Coord a)
{
	return(CalcOuterProduct(*this,a));
}

// Function: InitCoord
// 座標値の初期化
// 
// Parameters:
// *a - 初期化したい座標値のポインタ
void InitCoord(Coord *a)
{
	a->x = 0;
	a->y = 0;
	a->z = 0;
	a->dmy = 0;
}
// Function: InitCoord
// 座標値配列の初期化(オーバーロード)
// 
// Parameters:
// *a - 初期化したい座標値配列
// n - 配列要素数
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
// 座標値の初期化(オーバーロード)
// 
// Return:
// 初期化された座標値
Coord InitCoord()
{
	Coord a;
	InitCoord(&a);

	return a;
}

// Function: AddCoord
// 座標値同士の足し算
//
// Parameter: 
// a, b - 加算する2つの座標値
//
// Return:
// 演算結果(a.x+b.x, a.y+b.y, a.z+b.z)
Coord AddCoord(Coord a,Coord b)
{
	Coord ans;

	ans.x = a.x + b.x;
	ans.y = a.y + b.y;
	ans.z = a.z + b.z;

	return ans;
}
// Function: AddCoord
// 座標値同士の足し算(オーバーロード)
//
// Parameter: 
// a - 加算される座標値
// b - 加算するdouble値(a.x, a.y, a.zそれぞれにbが加算される)
//
// Return:
// 演算結果(a.x+b, a.y+b, a.z+b)
Coord AddCoord(Coord a,double b)
{
	Coord ans;

	ans.x = a.x + b;
	ans.y = a.y + b;
	ans.z = a.z + b;

	return ans;
}

// Function: AddCoord
// 座標値同士の足し算(オーバーロード)
//
// Parameter: 
// a - 加算される座標値
// x,y,z - 加算するdouble値(a.x, a.y, a.zそれぞれにx,y,zが加算される)
//
// Return:
// 演算結果(a.x+x, a.y+y, a.z+z)
Coord AddCoord(Coord a,double x,double y,double z)
{
	Coord ans;

	ans.x = a.x + x;
	ans.y = a.y + y;
	ans.z = a.z + z;

	return ans;
}

// Function: AddCoord2D
// 座標値同士の足し算 (2D Ver.)
//
// Parameter: 
// a, b - 加算する2つの座標値
//
// Return:
// 演算結果(a.x+b.x, a.y+b.y)
Coord AddCoord2D(Coord a,Coord b)
{
	Coord ans;

	ans.x = a.x + b.x;
	ans.y = a.y + b.y;

	return ans;
}

// Function: AddCoord2D
// 座標値同士の足し算(オーバーロード) (2D Ver.)
//
// Parameter: 
// a - 加算される座標値
// b - 加算するdouble値(a.x, a.yそれぞれにbが加算される)
//
// Return:
// 演算結果(a.x+b, a.y+b)
Coord AddCoord2D(Coord a,double b)
{
	Coord ans;

	ans.x = a.x + b;
	ans.y = a.y + b;

	return ans;
}

// Function: AddCoord
// 座標値同士の足し算(オーバーロード) (2D Ver.)
//
// Parameter: 
// a - 加算される座標値
// x,y,z - 加算するdouble値(a.x, a.yそれぞれにx,yが加算される)
//
// Return:
// 演算結果(a.x+x, a.y+y)
Coord AddCoord2D(Coord a,double x,double y)
{
	Coord ans;

	ans.x = a.x + x;
	ans.y = a.y + y;

	return ans;
}

// Function: DivCoord
// 座標値同士の割り算
//
// Parameter: 
// a,b - 除算する2つの座標値
//
// Return:
// 演算結果(a.x/b.x, a.y/b.y, a.z/b.z)
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
// 座標値同士の割り算(オーバーロード)
//
// Parameter: 
// a - 除算される座標値
// b - 除算するdouble値
// 
// Return:
// 演算結果(a.x/b, a.y/b, a.z/b)
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
// 座標値同士の割り算(オーバーロード)
//
// Parameter: 
// a - 除算される座標値
// b - 除算するdouble値
// 
// Return:
// 演算結果(a.x/x, a.y/y, a.z/z)
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
// 座標値同士の割り算 (2D Ver.)
//
// Parameter: 
// a,b - 除算する2つの座標値
//
// Return:
// 演算結果(a.x/b.x, a.y/b.y)
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
// 座標値同士の割り算(オーバーロード) (2D Ver.)
//
// Parameter: 
// a - 除算される座標値
// b - 除算するdouble値
// 
// Return:
// 演算結果(a.x/b, a.y/b)
Coord DivCoord2D(Coord a,double b)
{
	if(b == 0.0)	return SetCoord2D(0.0,0.0);

	Coord ans;

	ans.x = a.x / b;
	ans.y = a.y / b;

	return ans;
}

// Function: DivCoord2D
// 座標値同士の割り算(オーバーロード) (2D Ver.)
//
// Parameter: 
// a - 除算される座標値
// x,y - 除算するdouble値
// 
// Return:
// 演算結果(a.x/x, a.y/y)
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
// 座標値同士の引き算
//
// Parameter: 
// a,b - 引き算する2つの座標値
//
// Return:
// 演算結果(a.x-b.x, a.y-b.y, a.z-b.z)
Coord SubCoord(Coord a,Coord b)
{
	Coord ans;

	ans.x = a.x - b.x;
	ans.y = a.y - b.y;
	ans.z = a.z - b.z;

	return ans;
}

// Function: SubCoord
// 座標値同士の引き算(オーバーロード)
//
// Parameter: 
// a - 引き算される座標値
// b - 引き算するdouble値
//
// Return:
// 演算結果(a.x-b, a.y-b, a.z-b)
Coord SubCoord(Coord a,double b)
{
	Coord ans;

	ans.x = a.x - b;
	ans.y = a.y - b;
	ans.z = a.z - b;

	return ans;
}

// Function: SubCoord
// 座標値同士の引き算(オーバーロード)
//
// Parameter: 
// a - 引き算される座標値
// x,x,z - 引き算するdouble値
//
// Return:
// 演算結果(a.x-x, a.y-y, a.z-z)
Coord SubCoord(Coord a,double x,double y,double z)
{
	Coord ans;

	ans.x = a.x - x;
	ans.y = a.y - y;
	ans.z = a.z - z;

	return ans;
}

// Function: SubCoord2D
// 座標値同士の引き算 (2D Ver.)
//
// Parameter: 
// a,b - 引き算する2つの座標値
//
// Return:
// 演算結果(a.x-b.x, a.y-b.y)
Coord SubCoord2D(Coord a,Coord b)
{
	Coord ans;

	ans.x = a.x - b.x;
	ans.y = a.y - b.y;

	return ans;
}

// Function: SubCoord2D
// 座標値同士の引き算(オーバーロード) (2D Ver.)
//
// Parameter: 
// a - 引き算される座標値
// b - 引き算するdouble値
//
// Return:
// 演算結果(a.x-b, a.y-b)
Coord SubCoord2D(Coord a,double b)
{
	Coord ans;

	ans.x = a.x - b;
	ans.y = a.y - b;

	return ans;
}

// Function: SubCoord2D
// 座標値同士の引き算(オーバーロード) (2D Ver.)
//
// Parameter: 
// a - 引き算される座標値
// x,y - 引き算するdouble値
//
// Return:
// 演算結果(a.x-x, a.y-y)
Coord SubCoord2D(Coord a,double x,double y)
{
	Coord ans;

	ans.x = a.x - x;
	ans.y = a.y - y;

	return ans;
}

// Function: MulCoord
// 座標値同士の掛け算
//
// Parameter: 
// a,b - 掛け算する2つの座標値
//
// Return:
// 演算結果(a.x*b.x, a.y*b.y, a.z*b.z)
Coord MulCoord(Coord a,Coord b)
{
	Coord ans;

	ans.x = a.x * b.x;
	ans.y = a.y * b.y;
	ans.z = a.z * b.z;

	return ans;
}

// Function: MulCoord
// 座標値同士の掛け算(オーバーロード)
//
// Parameter: 
// a - 掛け算される座標値
// b - 掛け算するdouble値
// 
// Return:
// 演算結果(a.x*b, a.y*b, a.z*b)
Coord MulCoord(Coord a,double b)
{
	Coord ans;

	ans.x = a.x * b;
	ans.y = a.y * b;
	ans.z = a.z * b;

	return ans;
}

// Function: MulCoord
// 座標値同士の掛け算(オーバーロード)
//
// Parameter: 
// a - 掛け算される座標値
// x,y,z - 掛け算するdouble値
// 
// Return:
// 演算結果(a.x*x, a.y*y, a.z*z)
Coord MulCoord(Coord a,double x,double y,double z)
{
	Coord ans;

	ans.x = a.x * x;
	ans.y = a.y * y;
	ans.z = a.z * z;

	return ans;
}

// Function: MulCoord2D
// 座標値同士の掛け算 (2D Ver.)
//
// Parameter: 
// a,b - 掛け算する2つの座標値
//
// Return:
// 演算結果(a.x*b.x, a.y*b.y)
Coord MulCoord2D(Coord a,Coord b)
{
	Coord ans;

	ans.x = a.x * b.x;
	ans.y = a.y * b.y;

	return ans;
}

// Function: MulCoord2D
// 座標値同士の掛け算(オーバーロード) (2D Ver.)
//
// Parameter: 
// a - 掛け算される座標値
// b - 掛け算するdouble値
// 
// Return:
// 演算結果(a.x*b, a.y*b)
Coord MulCoord2D(Coord a,double b)
{
	Coord ans;

	ans.x = a.x * b;
	ans.y = a.y * b;

	return ans;
}

// Function: MulCoord2D
// 座標値同士の掛け算(オーバーロード) (2D Ver.)
//
// Parameter: 
// a - 掛け算される座標値
// x,y - 掛け算するdouble値
// 
// Return:
// 演算結果(a.x*x, a.y*y)
Coord MulCoord2D(Coord a,double x,double y)
{
	Coord ans;

	ans.x = a.x * x;
	ans.y = a.y * y;

	return ans;
}

// Function: DiffCoord
// 座標値がAPPROX_ZEROの精度で同じならKOD_TRUE、異なっているならKOD_FALSEを返す
//
// Parameter: 
// a,b - 比較する2つの座標値
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
// 座標値が指定の精度で同じならKOD_TRUE、異なっているならKOD_FALSEを返す(オーバーロード) 
//
// Parameter: 
// a,b - 比較する2つの座標値
// App - 精度（APPROX_ZERO_L_L, APPROX_ZERO_L, APPROX_ZERO, APPROX_ZERO_Hから選択）
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
// 2D平面での座標値がAPPROX_ZEROの精度で同じならKOD_TRUE、異なっているならKOD_FALSEを返す
//
// Parameter: 
// a,b - 比較する2つの座標値
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
// 2D平面での座標値が指定の精度で同じならKOD_TRUE、異なっているならKOD_FALSEを返す(オーバーロード)
//
// Parameter: 
// a,b - 比較する2つの座標値
// App - 精度（APPROX_ZERO_L_L, APPROX_ZERO_L, APPROX_ZERO, APPROX_ZERO_Hから選択）
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
// 座標値の絶対値を返す
//
// Parameter: 
// a - 座標値
//
// Return:
// x,y,z各座標の絶対値を返す
Coord AbsCoord(Coord a)
{
	Coord ans;

	ans.x = fabs(a.x);
	ans.y = fabs(a.y);
	ans.z = fabs(a.z);

	return ans;
}

// Function: AbsCoord2D
// 座標値の絶対値を返す(2D Ver.)
//
// Parameter: 
// a - 座標値
//
// Return:
// x,y,z各座標の絶対値を返す
Coord AbsCoord2D(Coord a)
{
	Coord ans;

	ans.x = fabs(a.x);
	ans.y = fabs(a.y);

	return ans;
}

// Function: SetCoord
// 座標値を代入する
// 
// Parameter:
// a - 代入する座標値
// 
// Return:
// 引数aの値がそのまま返る
Coord SetCoord(Coord a)
{
	return a;
}

// Function: SetCoord
// 座標値を代入する(オーバーロード)
// 
// Parameter:
// x,y,z - 代入する座標値を要素ごとに指定
// 
// Return:
// (x,y,z)の値がCoordとして返る
Coord SetCoord(double x,double y,double z)
{
	Coord ans;

	ans.x = x;
	ans.y = y;
	ans.z = z;

	return ans;
}

// Function: SetCoord2D
// 座標値を代入する(2D Ver.)
// 
// Parameter:
// a - 代入する座標値
// 
// Return:
// 引数aの値がそのまま返る
Coord SetCoord2D(Coord a)
{
	return a;
}

// Function: SetCoord2D
// 座標値を代入する(オーバーロード)(2D Ver.)
// 
// Parameter:
// x,y - 代入する座標値を要素ごとに指定
// 
// Return:
// (x,y)の値がCoordとして返る
Coord SetCoord2D(double x,double y)
{
	Coord ans;

	ans.x = x;
	ans.y = y;

	return ans;
}

// Function: CopyCoord
// 座標値群のコピー(b<--a)
// 
// Parameter:
// *a - 代入する座標値配列
// n - 配列の要素数
// *b - 代入される方の座標値配列
void CopyCoord(Coord *a,int n,Coord *b)
{
	for(int i=0;i<n;i++)
		b[i] = SetCoord(a[i]);
}

// Function: CopyCoord2D
// 座標値群のコピー(b<--a)(2D Ver.)
// 
// Parameter:
// *a - 代入する座標値配列
// n - 配列の要素数
// *b - 代入される方の座標値配列
void CopyCoord2D(Coord *a,int n,Coord *b)
{
	for(int i=0;i<n;i++)
		b[i] = SetCoord2D(a[i]);
}

// Function: ZoroCoord
// 座標値aが(0,0,0)のときKOD_FALSEを返す
//
// Parameters:
// a - 検証する座標値
// KOD_TRUE: (0,0,0)でない．  KOD_FALSE: (0,0,0)
int ZoroCoord(Coord a)
{
	if(a.x == 0.0 && a.y == 0.0 && a.z == 0.0)
		return KOD_FALSE;

	return KOD_TRUE;
}

// Function: ZoroCoord2D
// 座標値aが(0,0)のときKOD_FALSEを返す
//
// Parameters:
// a - 検証する座標値
// KOD_TRUE: (0,0)でない．  KOD_FALSE: (0,0)
int ZoroCoord2D(Coord a)
{
	if(a.x == 0.0 && a.y == 0.0)
		return KOD_FALSE;

	return KOD_TRUE;
}

// Function: NewVector
// double型1次元配列のメモリー確保
//
// Parameters:
// len - メモリー確保するdouble型配列要素数
//
// Return:
// 確保されたdouble型1次元配列へのポインタ（メモリー確保に失敗：NULL）
Vector NewVector(int len)
{
	Vector a;
	if((a = (double *)malloc(len*sizeof(double))) == NULL){
        GuiIF.SetMessage("fail to allocate memoly");
		return NULL;
	}

	return a;
}

// Function: NewVector
// double型2次元配列のメモリー確保
//
// Parameters:
// row, col - メモリー確保するdouble型2次元配列の行，列の要素数
//
// Return:
// 確保されたdouble型2次元配列へのポインタ（メモリー確保に失敗：NULL）
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

// Function: FreeVector
// double型1次元配列のメモリー解放
//
// Parameter:
// a - メモリー解放するdouble型1次元配列へのポインタ
void FreeVector(Vector a)
{
	free(a);
}

// Function: FreeMatrix
// double型2次元配列のメモリー解放
//
// Parameter:
// a - メモリー解放するdouble型2次元配列へのポインタ
// col - aの行要素数
void FreeMatrix(Matrix a,int col)
{
	for(int i=0;i<col;i++)
		free(a[i]);

	free(a);
}

// Function: NewCoord1
// 1次元Coord型配列のメモリー確保
//
// Parameters:
// len - メモリー確保するCoord型配列要素数
//
// Return:
// 確保されたdouble型1次元配列へのポインタ（メモリー確保に失敗：NULL）
Coord *NewCoord1(int len)
{
	Coord *a;

	if((a = (Coord *)malloc(len*sizeof(Coord))) == NULL){
        GuiIF.SetMessage("fail to allocate memoly");
		return NULL;
	}

	return a;
}

// Function: NewCoord2
// 2次元Coord型配列のメモリー確保
//
// Parameters:
// row, col - メモリー確保するCoord型2次元配列の行，列の要素数
//
// Return:
// 確保されたCoord型2次元配列へのポインタ（メモリー確保に失敗：NULL）
Coord **NewCoord2(int row,int col)
{
	int i;
	Coord **a;

	if((a = (Coord **)malloc((row)*sizeof(Coord *))) == NULL){
        GuiIF.SetMessage("fail to allocate memoly");
		return NULL;
	}
	for(i=0;i<row;i++){
		if((a[i] = (Coord *)malloc(col*sizeof(Coord))) == NULL){
            GuiIF.SetMessage("fail to allocate memoly");
			while(--i>=0) free(a[i]);
			free(a);
			return NULL;
		}
	}

	return a;
}

// Function: NewCoord3
// 3次元Coord型配列のメモリー確保
//
// Parameters:
// x, y, z - メモリー確保するCoord型3次元配列の各配列要素数
//
// Return:
// 確保されたCoord型3次元配列へのポインタ（メモリー確保に失敗：NULL）
Coord ***NewCoord3(int x,int y,int z)
{
	int i,j;
	Coord ***a;

	if((a = (Coord ***)malloc(x*sizeof(Coord **))) == NULL){
        GuiIF.SetMessage("fail to allocate memoly x");
		return NULL;
	}
	for(i=0;i<x;i++){
		if((a[i] = (Coord **)malloc(y*sizeof(Coord *))) == NULL){
            GuiIF.SetMessage("fail to allocate memoly y");
			while(--i>=0) free(a[i]);
			free(a);
			return NULL;
		}
		for(j=0;j<y;j++){
			if((a[i][j] = (Coord *)malloc(z*sizeof(Coord))) == NULL){
				GuiIF.SetMessage("fail to allocate memoly z");
				while(--j>=0) free(a[i][j]);
				while(--i>=0) free(a[i]);
				free(a);
				return NULL;
			}
		}
	}

	return a;
}

// Function: FreeCoord1
// 1次元Coord型配列のメモリー解放 
//
// Parameters:
// *a - 解放する1次元Coord型配列へのポインタ
void FreeCoord1(Coord *a)
{
	free(a);
}

// Function: FreeCoord2
// 2次元Coord型配列のメモリー解放 
//
// Parameters:
// *a - 解放する2次元Coord型配列へのポインタ
// col - aの行要素数
void FreeCoord2(Coord **a,int col)
{
	Coord **b;

	b=a;
	for(int i=0;i<col;i++){
		free(b[i]);
	}
	free(a);
}

// Function: FreeCoord3
// 3次元Coord型配列のメモリー解放 
//
// Parameters:
// *a - 解放する3次元Coord型配列へのポインタ
// x,y - aの行,列要素数
void FreeCoord3(Coord ***a,int x,int y)
{
	int i,j;

	for(i=0;i<x;i++){
		for(j=0;j<y;j++){
			free(a[i][j]);
		}
		free(a[i]);
	}
	free(a);
}

// Function: NormalizeVec
// ベクトルを正規化する
//
// Parameters:
// a - 正規化する三次元ベクトル
//
// Return:
// 正規化された三次元ベクトル
Coord NormalizeVec(Coord a)
{
	double len=0;

	len = CalcEuclid(a);

	return DivCoord(a,len);
}

// Function: NormalizeVec
// ベクトルを正規化する(オーバーロード)
//
// Parameters:
// x,y,z - 正規化する3次元ベクトルを，(x,y,z)座標値で指定
//
// Return:
// 正規化された3次元ベクトル
Coord NormalizeVec(double x,double y,double z)
{
	double len=0;
	Coord a = SetCoord(x,y,z);

	len = CalcEuclid(a);

	return DivCoord(a,len);
}

// Function: CalcEuclid
// ユークリッド距離を算出
//
// Parameters:
// a - 3次元ベクトル
//
// Return:
// ユークリッド距離
double CalcEuclid(Coord a)
{
	return sqrt(a.x*a.x + a.y*a.y + a.z*a.z);
}

// Function: CalcEuclid2D
// 2次元ユークリッド距離を算出
//
// Parameters:
// a,b - 2次元ベクトル(a,b)
//
// Return:
// ユークリッド距離
double CalcEuclid2D(double a,double b)
{
	return sqrt(a*a+b*b);
}

// Function: CalcDistance
// 2点間のユークリッド距離を求める
//
// Parameters:
// a,b - 2点
//
// Return:
// 2点間のユークリッド距離
double CalcDistance(Coord a,Coord b)
{
	return(CalcEuclid(SubCoord(a,b)));
}

// Function: CalcDistance2D
// 2次元座標上での2点間のユークリッド距離を算出
//
// Parameters:
// a,b - 2点
//
// Return:
// 2点間のユークリッド距離
double CalcDistance2D(Coord a,Coord b)
{
	return sqrt((a.x-b.x)*(a.x-b.x) + (a.y-b.y)*(a.y-b.y));
}

// Function: CalcInnerProduct
// 内積を求める
// 
// Parameters:
// a,b - 2つの3次元ベクトル
//
// Return:
// 内積
double CalcInnerProduct(Coord a,Coord b)
{
	return(a.x*b.x+a.y*b.y+a.z*b.z);
}

// Function: CalcInnerProduct
// 内積を求める(オーバーロード)
// 
// Parameters:
// a - 3次元ベクトル(Coordで指定)
// x,y,z - 3次元ベクトル((x,y,z)座標値で指定)
//
// Return:
// 内積
double CalcInnerProduct(Coord a,double x,double y,double z)
{
	return(a.x*x+a.y*y+a.z*z);
}

// Function: CalcOuterProduct
// 外積を求める
// 
// Parameters:
// a,b - 2つの3次元ベクトル
//
// Return:
// 外積
Coord CalcOuterProduct(Coord a,Coord b)
{
	Coord c;

	c.x = a.y*b.z - a.z*b.y;
	c.y = a.z*b.x - a.x*b.z;
	c.z = a.x*b.y - a.y*b.x;

	return c;
}

// Function: CalcOuterProduct2D
// 外積を求める (2D Ver.)
//
// Parameters:
// a,b - 2つの3次元ベクトル
//
// Return:
// 外積
double CalcOuterProduct2D(Coord a,Coord b)
{
	return(a.x*b.y - a.y*b.x);
}

// Function: CalcVecAngle
// 2つのベクトルのなす角を求める(返値はrad)
//
// Parameters:
// a,b - 2つの3次元ベクトル
//
// Return:
// 2つのベクトルのなす角(rad)
double CalcVecAngle(Coord a,Coord b)
{
	double inn = CalcInnerProduct(a,b);
	double abs_a = CalcEuclid(a);
	double abs_b = CalcEuclid(b);

	return(acos(inn/abs_a/abs_b));
}

// Function: CalcInterDivPt
// 2点p(t=0),q(t=1)をt(0～1)で内分したときの点の座標をもとめる
//
// Parameters:
// p,q - 2つの3次元座標
// t - 内分比を0-1の間で指定．
// 
// Return:
// 内分点座標
Coord CalcInterDivPt(Coord p,Coord q,double t)
{
	return(AddCoord(p,MulCoord(SubCoord(q,p),t)));
}

// Function: CalcOrthoProjection
// 任意の点を任意の平面へ正射影する
//
// Parameters:
// p - 任意の平面上の点
// n - 任意の平面の単位法線ベクトル
// q - 正射影したい点
//
// Return:
// 正射影された点の座標値
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
// 任意の点から任意の平面までの距離を求める
//
// Parameters:
// Pt - 任意の点  
// P0 - 平面上の1点  
// N - 平面の法線ベクトル
//
// Return:
// 計算結果
double CalcDistPtToPlane(Coord Pt,Coord P0,Coord N)
{
	return((fabs(N.x*Pt.x + N.y*Pt.y + N.z*Pt.z - (N.x*P0.x + N.y*P0.y + N.z*P0.z)))/CalcEuclid(N));
}

// Function: CalcScalarTriProduct
// スカラー三重積を求める
//
// Parameters:
// a,b,c - 3つの3次元ベクトル
//
// Return:
// スカラー三重積
double CalcScalarTriProduct(Coord a,Coord b,Coord c)
{
	return(CalcInnerProduct(a,CalcOuterProduct(b,c)));
}

// Function: CalcAnglePlaneVec
// 平面と直線とのなす角を求める
//
// Parameters:
// a - 直線の方向ベクトル  
// n - 平面の法線ベクトル
//
// Return:
// 計算結果(radian)
double CalcAnglePlaneVec(Coord a,Coord n)
{
	return(PI/2 - CalcVecAngle(a,n));
}

// Function: DrawPoint
// 点を描画（OpenGL）
//
// Parameters:
// p - 点の座標値  
// Scale - pをScale倍する  
// Width - 点のサイズ  
// Color[3] - 点の色をRGBで指定　(0<= r,g,b <=1) 
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
// 点群を描画（OpenGL）
//
// Parameters:
// *p - 点群配列へのポインタ
// n - 点数
// scale -  pをScale倍する
// width - 点のサイズ
// color[3] - 点の色をRGBで指定　(0<= r,g,b <=1) 
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
// ベクトルを描画（OpenGL）
//
// Parameters:
// s,e - 3次元ベクトルの始点と終点座標
// vec_len - 表示するベクトルの長さの倍率
// width - 描画する線分の太さ
// color[3] - 点の色をRGBで指定　(0<= r,g,b <=1) 
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
// 2点間に線分を描画（OpenGL）
//
// Parameters:
// s,e - 描画する線分の始点と終点座標
// width - 描画する線分の太さ
// color[3] - 点の色をRGBで指定　(0<= r,g,b <=1) 
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
// 四角錐を描画する
//
// Parameters:
// r - 錐の底辺の半径
// h - 錐の高さ
void DrawSolidCone(double r, double h)
{
	double x[4],y[4];

	double drad = 2*PI/4;

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
// 角度をdegreeからradianへ変換
//
// Parameters:
// degree - degree
//
// Return:
// radian
double DegToRad(double degree)
{
	double radian;

	radian = degree * PI / 180.0;

	return radian;	
}

// Function: RadToDeg
// 角度をradianからdegreeへ変換
//
// Parameters:
// radian - radian
//
// Return:
// degree
double RadToDeg(double radian)
{
	double degree = 0.0;

	degree = radian * 180.0 / PI;

	return degree;
}

// Function: Arc_CP
// 円の中心点(vec[0])から円上に接する任意の2本の接線が交わる点へのベクトル(中心角0<θ<π)
//
// Parameters:
// a - 円弧をなすベクトル1  
// b - 円弧をなすベクトル2  
// cos - 中心角の余弦
//
// Return:
// 計算結果
Coord Arc_CP(Coord a, Coord b, double cos)
{
	Coord ans;

	ans.x = (a.x + b.x)/(1 + cos);
	ans.y = (a.y + b.y)/(1 + cos);
	ans.z = (a.z + b.z)/(1 + cos);

	return ans;
}

// Function: CalcVecAngle2D
// 2つのベクトルのなす角を求める(2D平面)
// 
// Parameters:
// a,b - 2つの2次元ベクトル
//
// Return: 
// 計算結果
double CalcVecAngle2D(Coord a, Coord b)
{
	double angle,sin,cos;

	sin = (a.x*b.y - b.x*a.y)/(a.x*a.x + a.y*a.y);
	cos = (a.x*b.x + a.y*b.y)/(a.x*a.x + a.y*a.y);

	angle = atan2(sin,cos);
	if(angle < 0) angle = angle + 2.0*PI;

	return angle;
}

// Function: CalcRotVec2D
// 任意のベクトルを回転させたベクトルを求める(2D平面)
// 
// Parameters:
// a - 任意の2次元ベクトル
// angle - 回転角度(rad)
//
// Return:
// 回転後の2次元ベクトル
Coord CalcRotVec2D(Coord a, double angle)
{
	Coord ans;

	ans.x = a.x*cos(angle) - a.y*sin(angle);
	ans.y = a.x*sin(angle) + a.y*cos(angle);
	ans.z = a.z;

	return ans;
}

// Function: CalcRotVec
// 任意のベクトルを原点を通る任意軸周りに回転させたベクトルを求める(ロドリゲスの回転公式)
//
// Parameters:
// a - 回転させたいベクトル  
// e - 原点を通る任意軸(単位ベクトルであること)  
// ang - 回転角(rad)
//
// Return: 
// 回転後のベクトル
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
// 任意の点Pから任意の直線(点Aを通り単位ベクトルuの方向を持つ)へ下ろした点を求める
//
// Parameters:
// P - 任意の点
// A - 任意の直線上の点
// u - 任意の直線の単位方向ベクトル
//
// Return:
// 計算結果
Coord CalcNormalLine(Coord P,Coord A,Coord u)
{
	double k = CalcInnerProduct(SubCoord(P,A),u);

	return(AddCoord(A,MulCoord(u,k)));
}

// Function: BubbleSort
// int型配列のバブルソート(基本交換法)
// 
// Parameters:
// array[] - ソートされる数値配列
// array_size - 配列要素数
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
// double型配列のバブルソート(基本交換法)(オーバーロード)
// 
// Parameters:
// array[] - ソートされる数値配列
// array_size - 配列要素数
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
// C言語標準関数qsort(double型、降順)で用いる比較関数
//
// Parameters:
// *a,*b - 比較するdouble型の数値
//
// Return:
// a<b：1, a>b：-1, a=b：0
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
// Coord配列の反転
// 
// Parameters:
// p[] - Coord配列
// n - pの要素数
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
// double型配列の反転操作(オーバーロード)
//
// Parameters:
// p[] - double配列
// n - pの要素数
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
// 1次元配列の初期化
// 
// Parameters:
// vec - 1次元配列へのポインタ
// size - 配列要素数
void InitVector(Vector vec,int size)
{
	for(int i=0;i<size;i++){
		vec[i] = 0.0;
	}
}

// Function: InitMatrix
// 2次元配列の初期化
// 
// Parameters:
// mat - 2次元配列へのポインタ
// size_x,size_y - 行列要素数
void InitMatrix(Matrix mat,int size_x,int size_y)
{
	for(int i=0;i<size_x;i++){
		for(int j=0;j<size_y;j++){
			mat[i][j] = 0.0;
		}
	}
}

// Function: CopyVector
// ベクトルのコピー(aベクトルをbベクトルへ代入)
//
// Parameters:
// a - コピー元1次元配列へのポインタ
// n - aの要素数
// b - コピー先1次元配列へのポインタ
void CopyVector(Vector a,int n,Vector b)
{
	for(int i=0;i<n;i++)
		b[i] = a[i];
}

// Function: CalcCubicEquation
// 3次方程式を解く
//
// Parameters:
// *p - 4つの係数(a[0]x^3 + a[1]x^2 + a[2]x + a[3])   
// *ans - 3つの解
//
// Return:
// 解が3つとも実根の場合は3、1つだけ実根の場合は1  a[0]==0の場合はKOD_ERR
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

// Function: CalcQuadraticEquation
// 2次方程式を解く
//
// Parameters:
// *a - 3つの係数(a[0]x^2 + a[1]x + a[2])
// *ans - 2つの解
//
// Return:
// 解が実根の場合は2、虚根の場合はKOD_ERR  a[0]==0の場合はKOD_ERR
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
// 1次方程式を解く
//
// Parameters:
// *a - 2つの係数(a[0]x + a[1])  
// *ans - 解
// 
// Return:
// a[0]==0の場合はKOD_ERR
int CalcLinearEquation(double *a,double *ans)
{
	if(fabs(a[0]) < APPROX_ZERO_H){
		return KOD_FALSE;
	}

	ans[0] = -a[1]/a[0];

	return 1;
}

// Function: sgn
// 符号判定
// 
// Parameters:
// x - 対象とするdouble値
//
// Return:
// x<0：-1，x==0：0, x>0：1
double sgn(double x)
{
	if(x<0)	return -1;
	else if(x == 0) return 0;
	else return 1;
}

// Function: CheckZero
// 値がAPPROX_ZEROの範囲で0であるかチェック
//
// Parameters:
// val - 入力値  
// flag - 精度(HIGH_ACCURACY or MID_ACCURACY or LOW_ACCURACY)
//
// -APPROX_ZERO < val < APPROX_ZERO
//
// Return:
// KOD_TRUE:範囲内でゼロとみなせる     KOD_FALSE:範囲外     KOD_ERR:引数のflag指定が間違っている
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
// 指定した値が指定した範囲内であるかをチェック
//
// Parameters:
// low - 下限  
// up - 上限   
// val - 調べたい値
// flag - チェックタイプを以下より選択
// >flag = 0:(low <= val <= up) --> (low-ap < val < up+ap), 
// >       1:(low < val < up) --> (low+ap < val < up-ap),
// >       2:(val <= up) --> (val < up+ap),
// >       3:(val < up) --> (val < up-ap),
// >       4:(low <= val) --> (low-ap < val),
// >       5:(low < val) --> (low+ap < val)
// >注意　valがAPPROX_ZERO(ap)内でlowまたはupと一致する場合は、範囲内にあるものとする
//
// Return:
// KOD_TRUE:範囲内　　KOD_FALSE:範囲外　　　KOD_ERR:flagに指定した値が0,1以外
int CheckRange(double low,double up,double val,int flag)
{
	if(flag < 0 || flag > 5){
		char mes[256];
		sprintf(mes,"CheckRange ERROR:wrong specified value. 0 or 1");
        GuiIF.SetMessage(mes);
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
// 2つの値の大小比較
//
// Parameters:
// val1,val2 - 入力値   
// flag - 精度(HIGH_ACCURACY or or MID_ACCURACY or LOW_ACCURACY or LOW_LOW_ACCURACY)
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
// 注目点の多角形内外判別(x-y平面内)
//
// Parameters:
// TargetPoint - 注目点  
// *BorderPoint - 多角形の頂点群配列   
// CountPoint - 頂点の数
// 
// Returns:
// KOD_TRUE:内  KOD_FALSE:外  KOD_ONEDGE:エッジ上
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

		// TargetPointがエッジ上(p0とp1の線上)にあるかチェック
		double a = (p1.x-p0.x)*(TargetPoint.x-p0.x) + (p1.y-p0.y)*(TargetPoint.y-p0.y);
		double L1 = CalcDistance2D(p1,p0);
		double L2 = CalcDistance2D(TargetPoint,p0);
		if(CheckZero(a-L1*L2,MID_ACCURACY) == KOD_TRUE && L1 >= L2){	// エッジ上だった
			return KOD_ONEDGE;		// 問答無用でreturn
		}
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

// Function: CalcNormVecFrom3Pts
// 空間上の3点からなる平面の法線ベクトルを求める
//
// Parameters:
// p1,p2,p3 - 空間上の3点
//
// Return:
// 計算結果
Coord CalcNormVecFrom3Pts(Coord p1,Coord p2,Coord p3)
{
	Coord denom = (p2-p1)&&(p3-p1);
	double numer = CalcEuclid(denom);

	return DivCoord(denom,numer);
}

// Function: CalcPolygonArea
// 空間上の多角形の面積を得る
//
// Parameters:
// p[] - 頂点列
// Vnum - 頂点の数
//
// Return:
// 計算結果
double CalcPolygonArea(Coord p[],int Vnum)
{
	double area=0;

	for(int i=0;i<Vnum;i++){
		area += CalcEuclid(CalcOuterProduct(p[i],p[(i+1)%Vnum]));
	}

	return(area/2);
}

// Function: ClacPolygonArea2D
// 2D平面上の多角形の符号付き面積を得る(CCW：正，CW：負)
//
// Parameters:
// p[] - 頂点列
// Vnum - 頂点の数
//
// Return:
// 計算結果
double ClacPolygonArea2D(Coord p[],int Vnum)
{
	double area=0;

	for(int i=0;i<Vnum;i++){
		area += CalcOuterProduct2D(p[i],p[(i+1)%Vnum]);
	}

	return(area/2);
}

// Function: DiscriminateCW2D
// 2D平面上の多角形が時計回りか反時計回りかを判別する
//
// Parameters:
// p[] - 頂点列
// Vnum - 頂点の数
//
// Return:
// CCW：KOD_TRUE     CW：KOD_FALSE
int DiscriminateCW2D(Coord p[],int Vnum)
{
	// 指定点数が1点以下の場合
	if(Vnum <= 2)
		return KOD_ERR;

	// 指定点数が3点以上の場合
	else{
		if(ClacPolygonArea2D(p,Vnum) > 0)	// CCW
			return CCW;

		else	// CW
			return CW;
	}

	return KOD_ERR;
}

// Function: AddMxMx
// 行列同士の足し算
//
// Parameters:
// A,B,C - [C] = [A] +[B]
// row,col - 行，列要素数
void AddMxMx(Matrix A,Matrix B,Matrix C,int row,int col)
{
	for(int i=0;i<row;i++){
		for(int j=0;j<col;j++){
			C[i][j] = A[i][j] + B[i][j];
		}
	}
}

// Function: MulMxMx
// 行列同士の掛け算
// 
// Parameters:
// A,B,C - [C] = [A][B]
// A_row, A_col - 行列Aの行，列要素数
// B_row, B_col - 行列Bの行，列要素数
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
// 行列とベクトルの掛け算
// 
// Parameters:
// A,B,C - {C} = [A]{B}
// A_row - 行数  
// A_col - 列数  
// B_row - ベクトルの次元数
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
// 行列と座標値ベクトルの掛け算(オーバーロード)
// >|A[0][0]     A[0][1] . .   A[0][col-1]  ||  B[0]  |
// >|A[1][0]     A[1][1] . .   A[1][col-1]  ||  B[1]  |
// >|   .           .    . .       .        ||    .   |
// >|   .           .    . .       .        ||    .   |
// >|A[row-1][0]    .    . . A[row-1][col-1]||B[row-1]|
//
// Parameters:
// A,B,C - {C} = [A]{B}
// A_row - 行数  
// A_col - 列数  
// B_row - ベクトルの次元数
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
// Coordで表現される3x3行列とCoordベクトルとの掛け算
// >    |A[0].x A[1].x A[2].x|       |d.x|
// >A = |A[0].y A[1].y A[2].y| , d = |d.y|
// >    |A[0].z A[1].z A[2].z|       |d.z|
//
// Parameters:
// A[3] - Coord表現の3x3行列
// d - Coord表現の3次元ベクトル
//
// Return:
// 計算結果
Coord MulMxCoord(Coord A[],Coord d)
{
	Coord ans;

	ans.x = A[0].x*d.x + A[1].x*d.y + A[2].x*d.z;
	ans.y = A[0].y*d.x + A[1].y*d.y + A[2].y*d.z;
	ans.z = A[0].z*d.x + A[1].z*d.y + A[2].z*d.z;

	return ans;
}

// Function: MulMxCoord
// Matrixで表現される3x3行列とCoordベクトルとの掛け算(オーバーロード)
// 
// Parameters:
// A - double型2次元配列へのポインタ(3x3行列)
// d - Coord表現の3次元ベクトル
//
// Return:
// 計算結果
Coord MulMxCoord(Matrix A,Coord d)
{
	Coord ans;

	ans.x = A[0][0]*d.x + A[0][1]*d.y + A[0][2]*d.z;
	ans.y = A[1][0]*d.x + A[1][1]*d.y + A[1][2]*d.z;
	ans.z = A[2][0]*d.x + A[2][1]*d.y + A[2][2]*d.z;

	return ans;
}

// Function: TranMx
// 転置行列を得る
// 
// Parameters:
// **A - 元の行列  
// m - Aの行数  
// n - Aの列数  
// **B - 転置行列を格納
//
// 転置されるとmとnが逆になるので、Bのメモリー確保に注意!
void TranMx(Matrix A,int m,int n,Matrix B)
{
	for(int i=0;i<m;i++){
		for(int j=0;j<n;j++){
			B[j][i] = A[i][j];
		}
	}
}

// Function: TranMx
// 2次元Coord配列によって構成される行列の転置行列を得る(オーバーロード)
//
// Parameters:
// **A - 元の行列  
// m - Aの行数  
// n - Aの列数  
// **B - 転置行列を格納
//
// 転置されるとmとnが逆になるので、Bのメモリー確保に注意!
void TranMx(Coord **A,int m,int n,Coord **B)
{
	for(int i=0;i<m;i++){
		for(int j=0;j<n;j++){
			B[j][i] = SetCoord(A[i][j]);
		}
	}
}

// Function: TranMx
// 1次元Coord配列によって構成される行列の転置行列を得る(オーバーロード)
// >             |A[0].x A[1].x A[2].x|                |B[0].x B[1].x B[2].x|   |A[0].x A[0].y A[0].z|
// >Coord A[3] = |A[0].y A[1].y A[2].y| , Coord B[3] = |B[0].y B[1].y B[2].y| = |A[1].x A[1].y A[1].z| = A^T
// >             |A[0].z A[1].z A[2].z|                |B[0].z B[1].z B[2].z|   |A[2].x A[2].y A[2].z|
//
// Parameters:
// A[3] - 元の行列
// B[3] - 転置行列を格納
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
// 同次変換行列(R,T)と座標値(I(3Dベクトル))との掛け算
// 
// Parameters:
// R[][3] - 同次変換行列の回転行列成分
// T[3] - 同次変換行列の並進ベクトル成分
// I - 座標値
// 
// Return: 
// 計算結果
Coord MulFrameCoord(double R[][3],double T[],Coord I)
{
	Coord O;

	O.x = R[0][0]*I.x + R[0][1]*I.y + R[0][2]*I.z + T[0];
	O.y = R[1][0]*I.x + R[1][1]*I.y + R[1][2]*I.z + T[1];
	O.z = R[2][0]*I.x + R[2][1]*I.y + R[2][2]*I.z + T[2];

	return O;
}

// Function: MulFrameCoord
// 同次変換行列(f)と座標値(I(3Dベクトル))との掛け算(オーバーロード)
// 
// Parameters:
// f - 同次変換行列
// I - 座標値
// 
// Return: 
// 計算結果
Coord MulFrameCoord(FRAME f,Coord I)
{
	Coord O;

	O.x = f.Rot[0].x*I.x + f.Rot[1].x*I.y + f.Rot[2].x*I.z + f.Trl.x;
	O.y = f.Rot[0].y*I.x + f.Rot[1].y*I.y + f.Rot[2].y*I.z + f.Trl.y;
	O.z = f.Rot[0].z*I.x + f.Rot[1].z*I.y + f.Rot[2].z*I.z + f.Trl.z;

	return O;
}

// Function: InvFrame
// 同次変換行列Fの逆行列を得る
// >F = |R T|    F^-1 = |R^-1 -R^-1*T|
// >    |0 1|           |  0     1   |
//
// Parameters:
// F - 同次変換行列
//
// Return:
// 計算結果
FRAME InvFrame(FRAME F)
{
	FRAME f;

	TranMx(F.Rot,f.Rot);				// F.Rotの転置行列F.Rot^Tを得る
	f.Trl = MulMxCoord(f.Rot,F.Trl);	// F.Rot^T * F.Trl
	f.Trl = MulCoord(f.Trl,-1);			// -(F.Rot^T * F.Trl)

	return f;
}

// Function: MulFrame
// 同次変換行列の掛け算
//
// Parameters:
// a,b - 同次変換行列
//
// Return:
// 計算結果
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
// 回転行列からZYZオイラー角を算出（ tmp.x がα角(O), tmp.y がβ角(A), tmp.z がγ角(T) にそれぞれ対応 ）
//
// Prameters:
// rot[3] - Coord表現の回転行列(rad)
//
// Return:
// 計算結果(deg)
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

// Function: InitFrame
// FRAMEの初期化
//
// Parameters:
// *f - 初期化するFRAMEへのポインタ
void InitFrame(FRAME *f)
{
	f->Rot[0] = SetCoord(0,0,0);
	f->Rot[1] = SetCoord(0,0,0);
	f->Rot[2] = SetCoord(0,0,0);
	f->Trl = SetCoord(0,0,0);
}

// Function: Gauss
// 連立1次方程式の解を求める
// 
// Parameters:
// n:行数
// a,b,x - [a]{x}={b}で、解はxに入る(b,xはdouble型配列)
//
// Return:
// 行列式(メモリーエラー：KOD_ERR)
double Gauss(int n,Matrix a,Vector b,Vector x)
{
	long double det;	// 行列式
	int *ip;			// 行交換の情報

	ip = (int *)malloc(sizeof(int)*n);
	if(ip == NULL){
        GuiIF.SetMessage("fail to allocate");
		return KOD_ERR;
	}

	det = LU(n,a,ip);					// LU分解
	if(det == 0) return KOD_FALSE;		// 行列式が0
	else LU_Solver(n,a,b,ip,x);	// LU分解の結果を使って連立方程式を解く

	free(ip);                   

	return det;					// 戻り値は行列式
}

// Function: Gauss
// 連立1次方程式の解を求める(オーバーロード)
// 
// Parameters:
// n:行数
// a,b,x - [a]{x}={b}で、解はxに入る(b,xはCoord型配列)
//
// Return:
// 行列式(メモリーエラー：KOD_ERR)
double Gauss(int n,Matrix a,Coord *b,Coord *x)
{
	long double det;	// 行列式
	int *ip;			// 行交換の情報

	ip = (int *)malloc(sizeof(int)*n);
	if(ip == NULL){
        GuiIF.SetMessage("fail to allocate");
		return KOD_ERR;
	}

	det = LU(n,a,ip);					// LU分解
	if(det == 0) return KOD_FALSE;		// 行列式が0
	else LU_Solver(n,a,b,ip,x);	// LU分解の結果を使って連立方程式を解く

	free(ip);                   

	return det;					// 戻り値は行列式
}

// Function: LU_Solver
// LU分解の結果から連立1次方程式を解く
//
// Parameters:
// n - 行/列数  
// a - n*nの係数行列 (注意:出力としてLU分解された結果が格納される)
// b - n次元の右辺ベクトル  
// ip - 行交換の情報
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

// Function: LU_Solver
// LU分解の結果から連立1次方程式を解く(オーバーロード)
//
// Parameters:
// n - 行/列数  
// a - n*nの係数行列 (注意:出力としてLU分解された結果が格納される)
// b - n次元の右辺Coord配列  
// ip - 行交換の情報
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

// Function: MatInv
// 逆行列を求める
//
// Parameters:
// n - 行(列)数	
// a - 元の行列	
// a_inv - 行列aの逆行列
//
// Return:
// 行列式(メモリーエラー：KOD_ERR)
double MatInv(int n,Matrix a,Matrix a_inv)
{
	int i, j, k, ii;
	long double t, det;
	int *ip;		// 行交換の情報

	ip = (int *)malloc(sizeof(int)*n);
	if (ip==NULL){
        GuiIF.SetMessage("fail to allocate");
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

// Function: LU
// LU分解ルーチン
// 
// Parameters:
// n - 行/列数
// a - n*n行列 (注意:出力としてLU分解された結果が格納される)
// *ip - 行交換の情報が格納される(n個のint配列を用意すること) 
//
// Return:
// 行列式
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

// Function: MatInv3
// 3x3の逆行列を求める
//
// Parameters:
// A - 元の行列
// A_inv - Aの逆行列を格納
//
// Return:
// 行列式
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

// Function: MatInv2
// 2x2の逆行列を求める
//
// Parameters:
// A - 元の行列
// A_inv - Aの逆行列を格納
//
// Return:
// 行列式
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

// Function: nCr
// 2項係数(nCrの組合せ総数)を求める
// 
// Parameters:
// n,r - nCrのnとr
//
// Return:
// 計算結果
int nCr(int n,int r)
{
	int p=1;
	for(int i=1;i<=r;i++)
		p *= (n-i+1)/i;

	return p;
}

// Function: Factorial
// 自然数nの階乗を求める(桁数に注意．int型の場合，10!でオーバーフロー)
//
// Parameters:
// n - n!のn
//
// Return:
// 計算結果
int Factorial(int n)
{
	int ans = 1;
	for(int i=1;i<=n;i++)
		ans *= i;
	return ans;
}

// Function: Round
// 四捨五入する
// 
// Parameters:
// x - 四捨五入するdouble型値
//
// Return:
// 計算結果
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
// カラーステータス構造体DispStatに値を代入する
//
// Parameters:
// *ds - 代入先のDispStat構造体へのポインタ
// r,g,b,a - カラーステータス
void SetColorStat(DispStat *ds,float r, float g, float b, float a)
{
	ds->Color[0] = r;
	ds->Color[1] = g;
	ds->Color[2] = b;
	ds->Color[3] = a;
}

// Function: CatCoord
// ある座標値配列の後ろに新たな座標値配列を繋げる
//
// Parameters:
// a[] - 代入される座標値配列		
// b[] - 代入する座標値配列		
// alim - a[]の配列要素数
// anum - 現在のa[]にデータが格納されている最後尾番号
// bnum - b[]の代入したい配列要素数
//
// Return: 
// 合算された配列要素数
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
// 同一点を除去する
//
// Prameters:
// *P - 点群(変更後の点群もここに格納される)   
// N - 点数
//
// Return:
// 変更後の点数
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
// 同一値を除去する(オーバーロード)
//
// Prameters:
// *P - 数値配列(変更後もここに格納される)   
// N - 点数
//
// Return:
// 変更後の点数
int CheckTheSamePoints(double *P,int N)
{
	if(!N) return 0;

	bool *flag = (bool *)malloc(sizeof(bool)*N);

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
	free(flag);

	return k;
}

// Function: CheckTheSamePoints2D
// 2次元平面内の同一点を除去する (座標値はCoordのxとyで与える)
// 
// Parameters:
// *P - 点群(変更後の点群もここに格納される)   
// N - 点数
//
// Return:
// 変更後の点数
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
// Coordをdouble配列に代入
//
// Parameters:
// a - Coord値
// b[3] - double配列
void CoordToArray(Coord a,double b[3])
{
	b[0] = a.x;
	b[1] = a.y;
	b[2] = a.z;
}

// Function: CoordToArray2D
// Coordをdouble配列に代入(2D Ver.)
//
// Parameters:
// a - Coord値
// b[2] - double配列
void CoordToArray2D(Coord a,double b[2])
{
	b[0] = a.x;
	b[1] = a.y;
}
