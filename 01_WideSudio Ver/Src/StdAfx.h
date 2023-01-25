#ifndef _STD_AFX_H_
#define _STD_AFX_H_

#define USE_WIDE_STUDIO

// コールバックに関するWindowsとUNIXの互換性を保つマクロ
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

// GLUTヘッダファイルのインクルード
// 各OSに合わせて適切にGLUT.hのファイル位置を指定してください
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

#define KOD_ERR	-1					// ERRORのシンボル
#define KOD_FALSE	0				// 偽のシンボル
#define KOD_TRUE	1				// 真のシンボル
#define KOD_DONE	2				// 実行済みを示すシンボル
#define KOD_ONEDGE	2				// 点がエッジ上にあることを示すシンボル
#define KOD_EQUAL	2				// 同値のシンボル
#define LOW_ACCURACY  0				// 低精度のシンボル
#define MID_ACCURACY  1				// 普通精度のシンボル
#define HIGH_ACCURACY 2				// 高精度のシンボル
#define FNAMEMAX	256				// ファイル名の最大文字数
#define PI 3.141592653589793		// 円周率
#define APPROX_ZERO_L 1.0e-6		// ゼロと見なせる値(低精度)
#define APPROX_ZERO	1.0e-12			// ゼロと見なせる値(普通精度)
#define APPROX_ZERO_H 1.0e-16		// ゼロと見なせる値(高精度)
#define LOOPCOUNTMAX	1000		// 収束計算回数の条件


typedef double **Matrix;	// double形の2次元配列をMatrixとして定義
typedef double *Vector;		// double形の1次元配列をVectorとして定義

// 座標用構造体を定義
typedef struct{
	double x,y,z;	// x,y,z座標値
	double dmy;		// 汎用
}Coord;

// 同次変換行列用構造体
typedef struct{
	Coord Rot[3];	// 回転行列
	Coord Trl;		// 並進成分
}FRAME;

// 表示属性用構造体
typedef struct{
	float Color[4];	// 色(r,g,b,?)
	// 表示属性の追加はここに記述
}DispStat;

// グローバルな関数の定義 ///////////////////////////////////////////////////////////////////////////

// 幾何,3次元ベクトル演算
void InitCoord(Coord *);						// 座標値の初期化
void InitCoord(Coord *,int);					// オーバーロード
Coord InitCoord();								// オーバーロード
Coord AddCoord(Coord,Coord);					// 座標値の足し算
Coord AddCoord(Coord,double);					// オーバーロード
Coord AddCoord(Coord,double,double,double);		// オーバーロード
Coord DivCoord(Coord,Coord);					// 座標値の割り算
Coord DivCoord(Coord,double);					// オーバーロード
Coord DivCoord(Coord,double,double,double);		// オーバーロード
Coord MulCoord(Coord,Coord);					// 座標値の掛け算(ベクトルの内積,外積ではないので注意)
Coord MulCoord(Coord,double);					// オーバーロード
Coord MulCoord(Coord,double,double,double);		// オーバーロード
Coord SubCoord(Coord,Coord);					// 座標値の引き算
Coord SubCoord(Coord,double);					// オーバーロード
Coord SubCoord(Coord,double,double,double);		// オーバーロード
Coord SetCoord(Coord);							// 座標値を代入
Coord SetCoord(double,double,double);			// オーバーロード
void CopyCoord(Coord *,int,Coord *);			// 座標値群をコピー
int DiffCoord(Coord,Coord);						// 座標値が同じならKOD_TRUE、異なっているならKOD_FALSEを返す
int DiffCoord(Coord,Coord,double);				// オーバーロード(精度指定)
int DiffCoord2D(Coord,Coord);					// 座標値が同じならKOD_TRUE、異なっているならKOD_FALSEを返す(2D)
int DiffCoord2D(Coord,Coord,double);			// オーバーロード(精度指定)
Coord AbsCoord(Coord);							// 座標値の絶対値を返す
int ZoroCoord(Coord);							// (0,0,0)の場合にKOD_TRUEを返す
Coord NormalizeVec(Coord);						// 3次元ベクトルを正規化(単位ベクトル化)
Coord NormalizeVec(double,double,double);		// オーバーロード
double CalcEuclid(Coord);						// ユークリッド距離をもとめる
double CalcEuclid2D(double,double);				// 2次元ユークリッド距離をもとめる
double CalcDistance(Coord,Coord);				// 2点間のユークリッド距離を求める
double CalcDistance2D(Coord,Coord);				// 2次元座標上での2点間のユークリッド距離を算出
double CalcEuclidPnts2D(Coord,Coord);			// 2次元座標上での2点間のユークリッド距離を算出
double CalcInnerProduct(Coord,Coord);			// 内積を求める
double CalcinnerProduct(Coord,double,double,double);	// オーバーロード
Coord CalcOuterProduct(Coord,Coord);			// 外積を求める
double CalcVecAngle(Coord,Coord);				// 2つのベクトルのなす角を求める
double CalcVecAngle2D(Coord,Coord);				// 2つのベクトルのなす角を求める(2D平面)
Coord CalcInterDivPt(Coord,Coord,double);		// 2点間の内分点を求める
Coord CalcOrthoProjection(Coord,Coord,Coord);	// 任意の点を任意の平面へ正射影する
double CalcDistPtToPlane(Coord,Coord,Coord);	// 任意の点から任意の平面までの距離を求める
double CalcScalarTriProduct(Coord,Coord,Coord);	// スカラー三重積を求める
Coord CalcRotVec2D(Coord,double);				// 任意のベクトルを回転させたベクトルを求める(2D平面)
Coord CalcRotVec(Coord,Coord,double);			// 任意のベクトルを原点を通る任意軸周りに回転させたベクトルを求める(3D平面)
Coord CalcNormalLine(Coord,Coord,Coord);		// 任意の点から任意の直線へ下ろした点を求める
Coord Arc_CP(Coord,Coord,double);				// 円の中心点(vec[0])から円上に接する任意の2本の接線が交わる点へのベクトル(中心角0<θ<π)
int IsPointInPolygon(Coord,Coord *,int);		// 注目点の多角形内外判別

// 同次変換行列、回転行列の演算
Coord MulFrameCoord(double[][3],double[],Coord);	// 同次変換行列と座標値(3Dベクトル)との掛け算
Coord MulFrameCoord(FRAME,Coord);				// オーバーロード 
FRAME MulFrame(FRAME ,FRAME);					// 同次変換行列の掛け算
FRAME InvFrame(FRAME);							// 同次変換行列の逆行列を得る
Coord RotToZYZEuler(Coord []);					// 回転行列をZYZオイラー角へ変換
void InitFrame(FRAME *);						// FRAMEの初期化

// 多次元ベクトル、多次元行列の演算
void InitVector(Vector,int);					// 1次元配列の初期化(引数:ベクトル,配列長)
void InitMatrix(Matrix,int,int);				// 2次元配列の初期化
void CopyVector(Vector,int,Vector);				// ベクトルのコピー
void MulMxMx(Matrix,int,int,Matrix,int,int,Matrix);	// 行列同士の掛け算
void MulMxVec(Matrix,int,int,Vector,int,Vector);	// 行列とベクトルの掛け算
void MulMxVec(Matrix,int,int,Coord *,Coord *);	// 行列と座標値ベクトルの掛け算
Coord MulMxCoord(Coord [],Coord);				// Coordで表現される3x3行列とCoordベクトルとの掛け算
Coord MulMxCoord(Matrix,Coord);					// 3x3行列とCoordベクトルとの掛け算
void TranMx(Matrix,int,int,Matrix);				// 転置行列を得る
void TranMx(Coord **,int,int,Coord **);			// 転置行列を得る(オーバーロード)
void TranMx(Coord [],Coord []);					// 転置行列を得る(オーバーロード)
double Gauss(int,Matrix,Vector,Vector);			// 連立1次方程式の解を求める
double Gauss(int,Matrix,Coord *,Coord *);		// 連立1次方程式の解を求める(オーバーロード)
void LU_Solver(int,Matrix,Vector,int *,Vector);	// LU分解の結果から連立1次方程式を解く
void LU_Solver(int,Matrix,Coord *,int *,Coord *);// LU分解の結果から連立1次方程式を解く(オーバーロード)
double LU(int,Matrix,int *);					// LU分解
double MatInv(int,Matrix,Matrix);				// 逆行列を求める
double MatInv3(Matrix,Matrix);					// 3x3の逆行列
double MatInv2(Matrix,Matrix);					// 2x2の逆行列

// 数値計算
double DegToRad(double degree);					// 角度単位をdegreeからradianへ
double RadToDeg(double radian);					// 角度単位をradianからdegreeへ
int CalcCubicEquation(double *,double *);		// 3次方程式の解を求める
int CalcQuadraticEquation(double *,double *);	// 2次方程式の解を求める
int CalcLinearEquation(double *,double *);		// 1次方程式の解を求める
int nCr(int n,int r);							// 2項係数(nCrの組合せ総数)を求める
int Factorial(int n);							// 自然数nの階乗を求める

// 描画関連
void DrawPoint(Coord,double,double,double []);			// 点を描画
void DrawPoints(Coord *,int,double,double,double []);	// 点群を描画
void DrawVector(Coord,Coord,double,double,double []);	// ベクトルを描画
void DrawLine(Coord,Coord,double,double []);			// 2点間に線分を描画

// メモリー関連
Matrix NewMatrix(int,int);			// 2次元double型配列のメモリー確保
Vector NewVector(int);				// 1次元double型配列のメモリー確保
void FreeMatrix(Matrix,int);		// 2次元double型配列のメモリー解放
void FreeVector(Vector);			// 1次元double型配列のメモリー解放
Coord *NewCoord1(int);				// 1次元Coord型配列のメモリー確保
void FreeCoord1(Coord *);			// 1次元Coord型配列のメモリー解放
Coord **NewCoord2(int,int);			// 2次元Coord型配列のメモリー確保
void FreeCoord2(Coord **,int);		// 2次元Coord型配列のメモリー解放

// その他
double sgn(double);						// 符号判定
int CheckZero(double,int);				// 値がAPPROX_ZEROの範囲で0であるかチェック
int CheckRange(double,double,double,int);	// 指定した値が指定した範囲内であるかをチェック
int CheckMag(double,double,int);		// 2つの値の大小比較 
void BubbleSort(int[],int);				// バブルソート(基本交換法)
void BubbleSort(double[],int);			// オーバーロード
int QCmp(const void *,const void *);	// クイックソート 

#endif
