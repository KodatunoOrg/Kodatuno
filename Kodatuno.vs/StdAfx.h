// stdafx.h : 標準のシステム インクルード ファイルのインクルード ファイル、または
// 参照回数が多く、かつあまり変更されない、プロジェクト専用のインクルード ファイル
// を記述します。
//

#pragma once

#include "targetver.h"

#ifdef _MSC_VER
#define WIN32_LEAN_AND_MEAN             // Windows ヘッダーから使用されていない部分を除外します。
#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // 一部の CString コンストラクターは明示的です。
//#define _AFX_NO_MFC_CONTROLS_IN_DIALOGS         // ダイアログ内の MFC コントロールのサポートを削除します。
#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // Windows ヘッダーから使用されていない部分を除外します。
#endif
#define	NO_WARN_MBCS_MFC_DEPRECATION	// VS2015～
#define _CRT_SECURE_NO_WARNINGS			// とりあえずコンパイルを通す
#include <afx.h>
#include <afxwin.h>         // MFC のコアおよび標準コンポーネント
#endif

#define	GLEW_STATIC			// GLEW static link
#include <gl/glew.h>		// OpenGL Extention
#include <gl/wglew.h>
#include <gl/gl.h>			// OpenGL


// TODO: プログラムに必要な追加ヘッダーをここで参照してください


// GUIインターフェースに依存したラップ関数集
// Wide Studioバージョン
class GUI_Interface
{
public:
	int SetMessage(char *);					// コンソールにメッセージを出力
	int SetCommand(char *);					// コンソールにコマンドを出力
	void AddBodyNameToWin(const char *);		// 新たに登録したBODYの名前をウィンドウリストに表示する
};
extern GUI_Interface GuiIF;		// クラスGUI_Interfaceのインスタンスをグローバルで宣言


// Constants: General Defines
// KOD_ERR -					ERRORのシンボル(-1)
// KOD_FALSE -					偽のシンボル(0)
// KOD_TRUE -					真のシンボル(1)
// KOD_DONE -					実行済みを示すシンボル(2)
// KOD_ONEDGE -					点がエッジ上にあることを示すシンボル(2)
// KOD_LARGE -					a > b　のシンボル(0)
// KOD_SMALL -					a < b　のシンボル(1)
// KOD_EQUAL -					a = b　のシンボル(2)
// KOD_LARGE_EQ -				a >= b　のシンボル(3)
// KOD_SMALL_EQ -				a <= b　のシンボル(4)
// LOW_LOW_ACCURACY -			低低精度のシンボル(-1)
// LOW_ACCURACY -				低精度のシンボル(0)
// MID_ACCURACY -				普通精度のシンボル(1)
// HIGH_ACCURACY -				高精度のシンボル(2)
// FNAMEMAX -					ファイル名の最大文字数(256)
// PI -							円周率(3.141592653589793)
// APPROX_ZERO_L_L -			ゼロと見なせる値(低低精度)(1.0e-3)
// APPROX_ZERO_L -				ゼロと見なせる値(低精度)(1.0e-6)
// APPROX_ZERO -				ゼロと見なせる値(普通精度)(1.0e-9)
// APPROX_ZERO_H -				ゼロと見なせる値(高精度)(1.0e-12)
// LOOPCOUNTMAX -				収束計算回数の条件(10000)
// COORDINDEX -					3次元座標を示すインデックス数(3)
// QUADINDEX -					3次元同次座標を示すインデックス数(4)
// CW -							正転のシンボル(0)
// CCW -						逆転のシンボル(1)
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
// Matrix - double形の2次元配列をMatrixとして定義
typedef double **Matrix;

// Typedef: double *
// Vector - double形の1次元配列をVectorとして定義
typedef double *Vector;

// Class: Coord 
// 座標値用クラスを定義
class Coord
{
public:

	// Variables: x,y,z,dmy
	// 三次元座標値(x, y, z)及び，汎用としてdmyを用意
	double x,y,z,dmy;

	// Operator: +
	// Coord同士の足し算(AddCoord())
	Coord operator +(Coord);

	// Oeprator: -
	// Coord同士の引き算(SubCoord())
	Coord operator -(Coord);	

	// Oeprator: *
	// Coord同士の掛け算(MulCoord())
	Coord operator *(Coord);

	// Oeprator: *
	// Coordとdoubleとの掛け算（オーバーロード）
	Coord operator *(double);

	// Operator: /
	// Coord同士の割り算(DivCoord())
	Coord operator /(Coord);

	// Operator: /
	// Coordとdoubleとの割り算（オーバーロード）
	Coord operator /(double);	// オーバーロード

	// Operator: &
	// Coord同士の内積(CalcInnerProduct())
	double operator &(Coord);

	// Operator: &&
	// Coord同士の外積(CalcOuterProduct())
	Coord operator &&(Coord);

	// Add by K.Magara
	void	SetMinCoord(const Coord&);
	void	SetMaxCoord(const Coord&);
};


// Structure: FRAME
// 同次変換行列用構造体
//
// Variables:
// Coord Rot[COORINDEX] -	// 回転行列
// Coord Trl -				// 並進成分
typedef struct{
	Coord Rot[COORDINDEX];		
	Coord Trl;
}FRAME;


// Structure: DispStat
// 表示属性用構造体
//
// Variables:
// float Color[4] -	// 色(r,g,b,?)
struct DispStat
{
	float Color[4];	
	DispStat() {
		Color[0] = Color[1] = Color[2] = Color[3] = 0.0f;
	}
	// 表示属性の追加はここに記述
};


// Package: グローバルな関数の定義

// Group: Functions(3次元ベクトル幾何演算)

// Function: InitCoord
// 座標値の初期化
void InitCoord(Coord *);

// Function: InitCoord
// 座標値の初期化(オーバーロード)
void InitCoord(Coord *,int);				

// Function: InitCoord
// 座標値の初期化(オーバーロード)
Coord InitCoord();

// Function: AddCoord
// 座標値の足し算
Coord AddCoord(Coord,Coord);					

// Function: AddCoord
// 座標値の足し算(オーバーロード)
Coord AddCoord(Coord,double);					

// Function: AddCoord
//  座標値の足し算(オーバーロード)
Coord AddCoord(Coord,double,double,double);		

// Function: DivCoord
// 座標値の割り算
Coord DivCoord(Coord,Coord);					

// Function: DivCoord
// 座標値の割り算(オーバーロード)
Coord DivCoord(Coord,double);					

// Function: DivCoord
// 座標値の割り算(オーバーロード)
Coord DivCoord(Coord,double,double,double);		

// Function: MulCoord
// 座標値の掛け算(ベクトルの内積,外積ではないので注意)
Coord MulCoord(Coord,Coord);					

// Function: MulCoord
// 座標値の掛け算(オーバーロード)
Coord MulCoord(Coord,double);					

// Function: MulCoord
// 座標値の掛け算(オーバーロード)
Coord MulCoord(Coord,double,double,double);		

// Function: SubCoord
// 座標値の引き算
Coord SubCoord(Coord,Coord);					

// Function: SubCoord
// 座標値の引き算(オーバーロード)
Coord SubCoord(Coord,double);					

// Function: SubCoord
// 座標値の引き算(オーバーロード)
Coord SubCoord(Coord,double,double,double);

// Function: SetCoord
// 座標値を代入
Coord SetCoord(Coord);							

// Function: SetCoord
// 座標値を代入(オーバーロード)
Coord SetCoord(double,double,double);			

// Function: CopyCoord
// 座標値群をコピー
void CopyCoord(Coord *,int,Coord *);			

// Function: DiffCoord
// 座標値が同じならKOD_TRUE、異なっているならKOD_FALSEを返す
int DiffCoord(Coord,Coord);						

// Function: DiffCoord
// 座標値が同じならKOD_TRUE、異なっているならKOD_FALSEを返す(オーバーロード(精度指定))
int DiffCoord(Coord,Coord,double);				

// Function: AbsCoord
// 座標値の絶対値を返す
Coord AbsCoord(Coord);							

// Function: ZoroCoord
// (0,0,0)の場合にKOD_TRUEを返す
int ZoroCoord(Coord);							

// Function: CalcEuclid
// ユークリッド距離をもとめる
double CalcEuclid(Coord);						

// Function: CalcDistance
// 2点間のユークリッド距離を求める
double CalcDistance(Coord,Coord);				

// Function: CalcRotVec
// 任意のベクトルを原点を通る任意軸周りに回転させたベクトルを求める(3D平面)
Coord CalcRotVec(Coord,Coord,double);			

// Function: CalcVecAngle
// 2つのベクトルのなす角を求める
double CalcVecAngle(Coord,Coord);				

// Function: CalcAnglePlaneVec
// 平面と直線とのなす角を求める
double CalcAnglePlaneVec(Coord,Coord);			

// Function: NormalizeVec
// 3次元ベクトルを正規化(単位ベクトル化)
Coord NormalizeVec(Coord);						

// Function: NormalizeVec
// 3次元ベクトルを正規化(単位ベクトル化)(オーバーロード)
Coord NormalizeVec(double,double,double);		

// Function: CalcInnerProduct
// 内積を求める
double CalcInnerProduct(Coord,Coord);			

// Function: CalcInnerProduct
// 内積を求める(オーバーロード)
double CalcInnerProduct(Coord,double,double,double);	

// Function: CalcOuterProduct
// 外積を求める
Coord CalcOuterProduct(Coord,Coord);			

// Function: CalcInterDivPt
// 2点間の内分点を求める
Coord CalcInterDivPt(Coord,Coord,double);		

// Function: CalcOrthoProjection
// 任意の点を任意の平面へ正射影する
Coord CalcOrthoProjection(Coord,Coord,Coord);	

// Function: CalcDistPtToPlane
// 任意の点から任意の平面までの距離を求める
double CalcDistPtToPlane(Coord,Coord,Coord);	

// Function: CalcScalarTriProduct
// スカラー三重積を求める
double CalcScalarTriProduct(Coord,Coord,Coord);	

// Function: CalcNormalLine
// 任意の点から任意の直線へ下ろした点を求める
Coord CalcNormalLine(Coord,Coord,Coord);		

// Function: Arc_CP
// 円の中心点(vec[0])から円上に接する任意の2本の接線が交わる点へのベクトル(中心角0<θ<π)
Coord Arc_CP(Coord,Coord,double);				

// Function: IsPointInPolygon
// 注目点の多角形内外判別
int IsPointInPolygon(Coord,Coord *,int);		

// Function: CalcNormVecFrom3Pts
// 空間上の3点からなる平面の法線ベクトルを求める
Coord CalcNormVecFrom3Pts(Coord,Coord,Coord);	


// Group: Functions(2次元ベクトル幾何演算)

// Function: AddCoord2D
// 座標値の足し算 (2D Ver.)
Coord AddCoord2D(Coord,Coord);

// Function: AddCoord2D
// 座標値の足し算(オーバーロード) (2D Ver.)
Coord AddCoord2D(Coord,double);					

// Function: AddCoord2D
// 座標値の足し算(オーバーロード) (2D Ver.)
Coord AddCoord2D(Coord,double,double);			

// Function: DivCoord2D
// 座標値の割り算 (2D Ver.)
Coord DivCoord2D(Coord,Coord);					

// Function: DivCoord2D
// 座標値の割り算(オーバーロード) (2D Ver.)
Coord DivCoord2D(Coord,double);					

// Function: DivCoord2D
// 座標値の割り算(オーバーロード) (2D Ver.)
Coord DivCoord2D(Coord,double,double);			

// Function: MulCoord2D
// 座標値の掛け算(ベクトルの内積,外積ではないので注意) (2D Ver.)
Coord MulCoord2D(Coord,Coord);					

// Function: MulCoord2D
// 座標値の掛け算(ベクトルの内積,外積ではないので注意)(オーバーロード) (2D Ver.)
Coord MulCoord2D(Coord,double);					

// Function: MulCoord2D
// 座標値の掛け算(ベクトルの内積,外積ではないので注意)(オーバーロード) (2D Ver.)
Coord MulCoord2D(Coord,double,double);			

// Function: SubCoord2D
// 座標値の引き算 (2D Ver.)
Coord SubCoord2D(Coord,Coord);					

// Function: SubCoord2D
// 座標値の引き算(オーバーロード) (2D Ver.)
Coord SubCoord2D(Coord,double);					

// Function: SubCoord2D
// 座標値の引き算(オーバーロード) (2D Ver.)
Coord SubCoord2D(Coord,double,double);			

// Function: SetCoord2D
// 座標値を代入 (2D Ver.)
Coord SetCoord2D(Coord);						

// Function: SetCoord2D
// オーバーロード (2D Ver.)
Coord SetCoord2D(double,double);				

// Function: CopyCoord2D
// 座標値群をコピー (2D Ver.)
void CopyCoord2D(Coord *,int,Coord *);			

// Function: DiffCoord2D
// 座標値が同じならKOD_TRUE、異なっているならKOD_FALSEを返す (2D Ver.)
int DiffCoord2D(Coord,Coord);					

// Function: DiffCoord2D
// 座標値が同じならKOD_TRUE、異なっているならKOD_FALSEを返す(オーバーロード(精度指定)) (2D Ver.)
int DiffCoord2D(Coord,Coord,double);			

// Function: AbsCoord2D
// 座標値の絶対値を返す (2D Ver.)
Coord AbsCoord2D(Coord);						

// Function: ZoroCoord2D
// (0,0,0)の場合にKOD_TRUEを返す (2D Ver.)
int ZoroCoord2D(Coord);							

// Function: CalcEuclid2D
// ユークリッド距離をもとめる (2D Ver.)
double CalcEuclid2D(double,double);				

// Function: CalcDistance2D
// 2次元座標上での2点間のユークリッド距離を算出 (2D Ver.)
double CalcDistance2D(Coord,Coord);				

// Function: CalcVecAngle2D
// 2つのベクトルのなす角を求める (2D Ver.)
double CalcVecAngle2D(Coord,Coord);				

// Function: CalcRotVec2D
// 任意のベクトルを回転させたベクトルを求める(2D平面)
Coord CalcRotVec2D(Coord,double);				

// Function: CalcOuterProduct2D
// 外積を求める (2D Ver.)
double CalcOuterProduct2D(Coord,Coord);			

// Function: ClacPolygonArea2D
// 2D平面上の多角形の符号付き面積を得る
double ClacPolygonArea2D(Coord [],int);			

// Function: DiscriminateCW2D
// 2D平面上の多角形が時計回りか反時計回りかを判別する
int DiscriminateCW2D(Coord [],int);				


// Group: Functions(同次変換行列、回転行列の演算)

// Function: MulFrameCoord
// 同次変換行列と座標値(3Dベクトル)との掛け算
Coord MulFrameCoord(double[][3],double[],Coord);	

// Function: MulFrameCoord
// 同次変換行列と座標値(3Dベクトル)との掛け算(オーバーロード)
Coord MulFrameCoord(FRAME,Coord);				 

// Function: MulFrame
// 同次変換行列の掛け算
FRAME MulFrame(FRAME ,FRAME);					

// Function: InvFrame
// 同次変換行列の逆行列を得る
FRAME InvFrame(FRAME);							

// Function: RotToZYZEuler
// 回転行列をZYZオイラー角へ変換
Coord RotToZYZEuler(Coord []);					

// Function: InitFrame
// FRAMEの初期化
void InitFrame(FRAME *);						


// Group: Functions(多次元ベクトル、多次元行列の演算)

// Function: InitVector
// 1次元配列の初期化(引数:ベクトル,配列長)
void InitVector(Vector,int);					

// Function: InitMatrix
// 2次元配列の初期化
void InitMatrix(Matrix,int,int);				

// Function: CopyVector
// ベクトルのコピー
void CopyVector(Vector,int,Vector);				

// Function: AddMxMx
// 行列同士の足し算
void AddMxMx(Matrix,Matrix,Matrix,int,int);		

// Function: MulMxMx
// 行列同士の掛け算
void MulMxMx(Matrix,int,int,Matrix,int,int,Matrix);	

// Function: MulMxVec
// 行列とベクトルの掛け算
void MulMxVec(Matrix,int,int,Vector,int,Vector);	

// Function: MulMxVec
// 行列と座標値ベクトルの掛け算
void MulMxVec(Matrix,int,int,Coord *,Coord *);	

// Function: MulMxCoord
// Coordで表現される3x3行列とCoordベクトルとの掛け算
Coord MulMxCoord(Coord [],Coord);				

// Function: MulMxCoord
// 3x3行列とCoordベクトルとの掛け算
Coord MulMxCoord(Matrix,Coord);					

// Function: TranMx
// 転置行列を得る
void TranMx(Matrix,int,int,Matrix);				

// Function: TranMx
// 転置行列を得る(オーバーロード)
void TranMx(Coord **,int,int,Coord **);			

// Function: TranMx
// 転置行列を得る(オーバーロード)
void TranMx(Coord [],Coord []);					

// Function: Gauss
// 連立1次方程式の解を求める
double Gauss(int,Matrix,Vector,Vector);			

// Function: Gauss
// 連立1次方程式の解を求める(オーバーロード)
double Gauss(int,Matrix,Coord *,Coord *);		

// Function: LU_Solver
// LU分解の結果から連立1次方程式を解く
void LU_Solver(int,Matrix,Vector,int *,Vector);	

// Function: LU_Solver
// LU分解の結果から連立1次方程式を解く(オーバーロード)
void LU_Solver(int,Matrix,Coord *,int *,Coord *);

// Function: LU
// LU分解
double LU(int,Matrix,int *);					

// Function: MatInv
// 逆行列を求める
double MatInv(int,Matrix,Matrix);				

// Function: MatInv3
// 3x3の逆行列
double MatInv3(Matrix,Matrix);					

// Function: MatInv2
// 2x2の逆行列
double MatInv2(Matrix,Matrix);					


// Group: Functions(数値計算)

// Function: DegToRad
// 角度単位をdegreeからradianへ
double DegToRad(double degree);					

// Function: RadToDeg
// 角度単位をradianからdegreeへ
double RadToDeg(double radian);					

// Function: CalcCubicEquation
// 3次方程式の解を求める
int CalcCubicEquation(double *,double *);		

// Function: CalcQuadraticEquation
// 2次方程式の解を求める
int CalcQuadraticEquation(double *,double *);	

// Function: CalcLinearEquation
// 1次方程式の解を求める
int CalcLinearEquation(double *,double *);		

// Function: nCr
// 2項係数(nCrの組合せ総数)を求める
int nCr(int n,int r);							

// Function: Factorial
// 自然数nの階乗を求める
int Factorial(int n);							

// Function: Round
// 四捨五入
double Round(double);							


// Group: Functions(描画関連)

// Function: DrawPoint
// 点を描画
void DrawPoint(Coord,double,double,double []);			

// Function: DrawPoints
// 点群を描画
void DrawPoints(Coord *,int,double,double,double []);	

// Function: DrawVector
// ベクトルを描画
void DrawVector(Coord,Coord,double,double,double []);	

// Function: DrawLine
// 2点間に線分を描画
void DrawLine(Coord,Coord,double,double []);			

// Function: SetColorStat
// カラーステータスを変更
void SetColorStat(DispStat *ds,float r, float g, float b, float a=0.5);	

// Function: DrawSolidCone
// 四角錐を描画する
void DrawSolidCone(double,double);		


// Group: Functions(メモリー関連)

// Function: NewMatrix
// 2次元double型配列のメモリー確保
Matrix NewMatrix(int,int);			

// Function: NewVector
// 1次元double型配列のメモリー確保
Vector NewVector(int);				

// Function: FreeMatrix
// 2次元double型配列のメモリー解放
void FreeMatrix(Matrix,int);		

// Function: FreeVector
// 1次元double型配列のメモリー解放
void FreeVector(Vector);			

// Function: NewCoord1
// 1次元Coord型配列のメモリー確保
Coord *NewCoord1(int);				

// Function: FreeCoord1
// 1次元Coord型配列のメモリー解放
void FreeCoord1(Coord *);			

// Function: NewCoord2
// 2次元Coord型配列のメモリー確保
Coord **NewCoord2(int,int);			

// Function: FreeCoord2
// 2次元Coord型配列のメモリー解放
void FreeCoord2(Coord **,int);		

// Function: NewCoord3
// 3次元Coord型配列のメモリー確保
Coord ***NewCoord3(int,int,int);	

// Function: FreeCoord3
// 3次元Coord型配列のメモリー解放
void FreeCoord3(Coord ***,int,int);	


// Group: Functions(その他)

// Function: sgn
// 符号判定
double sgn(double);						

// Function: CheckZero
// 値がAPPROX_ZEROの範囲で0であるかチェック
int CheckZero(double,int);				

// Function: CheckRange
// 指定した値が指定した範囲内であるかをチェック
int CheckRange(double,double,double,int);	

// Function: CheckMag
// 2つの値の大小比較 
int CheckMag(double,double,int);		

// Function: BubbleSort
// バブルソート(基本交換法)
void BubbleSort(int[],int);				

// Function: BubbleSort
// オーバーロード
void BubbleSort(double[],int);			

// Function: QCmp
// クイックソート 
int QCmp(const void *,const void *);	

// Function: Reverse
// Coord配列の反転操作
void Reverse(Coord [],int);				

// Function: Reverse
// double配列の反転操作
void Reverse(double [],int);			

// Function: CatCoord
// ある配列の後ろに新たな配列を繋げる
int CatCoord(Coord [],Coord [],int,int,int);	

// Function: CheckTheSamePoints
// 同一点を除去する
int CheckTheSamePoints(Coord *,int);	

// Function: CheckTheSamePoints
// 同一点を除去する
int CheckTheSamePoints(double *,int);	

// Function: CheckTheSamePoints2D
// 2D平面内の同一点を除去する
int CheckTheSamePoints2D(Coord *,int);	

// Function: CoordToArray
// Coordをdouble配列に代入
void CoordToArray(Coord,double []);		

// Function: CoordToArray2D
// Coordをdouble配列に代入
void CoordToArray2D(Coord,double []);	
