#ifndef _STDAFX_H_
#define _STDAFX_H_

// コールバックに関するWindowsとUNIXの互換性を保つマクロ(たぶん)
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

#define ERR		-1					// 偽のシンボル
#define FALSE	1					// 偽のシンボル
#define TRUE	0					// 真のシンボル
#define PI 3.141592653589793		// 円周率
#define APPROX_ZERO	1.0e-8			// ゼロと見なせる値

typedef double **Matrix;	// double形の2次元配列をMatrixとして定義
typedef double *Vector;		// double形の1次元配列をVectorとして定義

// 座標用構造体を定義
typedef struct{
	double x,y,z;
}Coord;

// 表示属性用構造体
typedef struct{
	float Color[4];	// 色(r,g,b,?)
	// 表示属性の追加はここに記述
}DispStat;


// グローバルな関数の定義 ///////////////////////////////////////////////////////////////////////////

// 幾何,3次元ベクトル,行列演算
void InitCoord(Coord *);			// 座標値の初期化
Coord AddCoord(Coord,Coord);		// 座標値の足し算
Coord AddCoord(Coord,double);		// オーバーロード
Coord DivCoord(Coord,Coord);		// 座標値の割り算
Coord DivCoord(Coord,double);		// オーバーロード
Coord MulCoord(Coord,Coord);		// 座標値の掛け算(ベクトルの内積,外積ではないので注意)
Coord MulCoord(Coord,double);		// オーバーロード
Coord SubCoord(Coord,Coord);		// 座標値の引き算
Coord SubCoord(Coord,double);		// オーバーロード
Coord SetCoord(Coord);					// 座標値を代入
Coord SetCoord(double,double,double);	// オーバーロード
int DiffCoord(Coord,Coord);			// 座標値が同じならTRUE、異なっているならFALSEを返す
void InitVector(Vector,int);		// 1次元配列の初期化(引数:ベクトル,配列長)
void InitMatrix(Matrix,int,int);	// 2次元配列の初期化
Coord NormalizeVec(Coord);			// 3次元ベクトルを正規化
double CalcEuclid(Coord);			// ユークリッド距離をもとめる
double CalcEuclidPnts2D(Coord,Coord);	// 2次元座標上での2点間のユークリッド距離を算出
double CalcEuclidPnts3D(Coord,Coord);	// 3次元座標上での2点間のユークリッド距離を算出
double CalcInnerProduct(Coord,Coord);	// 内積を求める
Coord CalcOuterProduct(Coord,Coord);	// 外積を求める
double CalcScalarTriProduct(Coord,Coord,Coord);	// スカラー三重積を求める
double CalcinnerProduct(Coord,double,double,double);	// オーバーロード
double DegToRad(double degree);			// 角度単位をdegreeからradianへ
double RadToDeg(double radian);			// 角度単位をradianからdegreeへ
Coord Arc_CP(Coord,Coord,double);		// 円の中心点(vec[0])から円上に接する任意の2本の接線が交わる点へのベクトル(中心角0<θ<π)
double Rot_Vec_Ang(Coord,Coord);		// 任意のベクトルを回転させたときの回転角を求める(2D平面)
Coord Rot_Vec(Coord,double);			// 任意のベクトルを回転させたベクトルを求める(2D平面)
Coord NormalVector(Coord,Coord);		// 曲面のu,v方向の接線ベクトルから法線ベクトルを求める
bool IsPointInPolygon(Coord,Coord *,int);	// 注目点の多角形内外判別
void MulMxMx(Matrix,int,int,Matrix,int,int,Matrix);	// 行列同士の掛け算
void MulMxVec(Matrix,int,int,Vector,int,Vector);	// 行列とベクトルの掛け算
Coord HomoTransform(double[][3],double[],Coord);	// 同時変換(回転+並進移動)

// 数値計算
double sgn(double);						// 符号判定
void BubbleSort(int[],int);				// バブルソート(基本交換法)
void BubbleSort(double[],int);			// オーバーロード
int QCmp(const void *,const void *);	// クイックソート 
int CalcCubicEquation(double *,double *);		// 3次方程式の解を求める
int CalcQuadraticEquation(double *,double *);	// 2次方程式の解を求める
int CalcLinearEquation(double *,double *);		// 1次方程式の解を求める

// 描画関連
void DrawPoint(Coord,double,double,double []);			// 点を描画
void DrawVector(Coord,Coord,double,double,double []);	// ベクトルを描画

// メモリー関連
Matrix NewMatrix(int,int);			// 2次元double型配列のメモリー確保
void FreeMatrix(Matrix);			// 2次元double型配列のメモリー解放
Coord **NewCoord(int,int);			// 2次元Coord型配列のメモリー確保
void FreeCoord(Coord **);			// 2次元Coord型配列のメモリー解放



#endif
