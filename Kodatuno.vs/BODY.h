// BODYの定義

#ifndef _BODY_H_
#define _BODY_H_

#include "StdAfx.h"
#include "KodListFunc.h"
#include "MESH.h"
#include <string>
//#include "boost/variant.hpp"
//#include "boost/shared_array.hpp"

// Constants: General Defines
// ALL_ENTITY_TYPE_NUM -	全エンティティタイプの数(21)
// CTLPNUMMAX -				NURBSで用いられるコントロールポイントの数の上限(1024)
// KNOTNUMMAX -				NURBSで用いられるノットシーケンスの数の上限(1024)
// GEOMTRYELEM -			IGESディレクトリ部"Entity Use Flag"より、幾何要素を示す(0)
// PARAMETRICELEM -			IGESディレクトリ部"Entity Use Flag"より、2Dパラメトリック要素を示す(5)
// NORM_KNOT_VAL -			ノットベクトルを正規化するときの範囲の最大値(1)
// MIN_KNOT_RANG -			隣り合うノットベクトルの差がこの値以上であること(0.0002)
#define CTLPNUMMAX  1024
#define KNOTNUMMAX  1024
#define GEOMTRYELEM 0
#define PARAMETRICELEM 5
#define NORM_KNOT_VAL	1
#define MIN_KNOT_RANGE	0.0002

// Constants: Symbol of Entity Type
//	CIRCLE_ARC -				円/円弧(100)
//	COMPOSITE_CURVE -			複合曲線(102)
//	CONIC_ARC -					円錐曲線(104)
//	COPIOUS_DATA -				有意点列(106)
//	PLANE -						平面(108)
//	LINE -						線分(110)
//	PARAMETRIC_SPLINE_CURVE -	パラメトリックスプライン曲線(112)
//	PARAMETRIC_SPLINE_SURFACE - パラメトリックスプライン曲面(114)
//	POINT -						点(116)
//	TRANSFORMATION_MATRIX -		変換行列(124)
//	NURBS_CURVE -				有理Bスプライン曲線(126)
//	NURBS_SURFACE -				有理Bスプライン曲面(128)
//	CURVE_ON_PARAMETRIC_SURFACE - 面上線(142)
//	TRIMMED_SURFACE -			トリム面(144)
//	SUBFIGURE_DEFINITION -		子図の定義(308)
//	ASSOCIATIVITY_INSTANCE -	グループ(402)
//	DRAWING -					図面(404)
//	PROPERTY -					図面サイズ(406)
//	SINGULAR_SUBFIGURE_INSTANCE - 子図の参照(408)
//	VIEW - 投象面(410)
#define	CIRCLE_ARC					100
#define	COMPOSITE_CURVE				102
#define	CONIC_ARC					104
#define	COPIOUS_DATA				106
#define	PLANE						108
#define	LINE						110
#define	PARAMETRIC_SPLINE_CURVE		112
#define	PARAMETRIC_SPLINE_SURFACE	114
#define	POINT						116
#define	TRANSFORMATION_MATRIX		124
#define	NURBS_CURVE					126
#define	NURBS_SURFACE				128
#define	CURVE_ON_PARAMETRIC_SURFACE 142
#define	TRIMMED_SURFACE				144
#define	SUBFIGURE_DEFINITION		308
#define	ASSOCIATIVITY_INSTANCE		402
#define	DRAWING						404	
#define	PROPERTY					406
#define	SINGULAR_SUBFIGURE_INSTANCE 408
#define	VIEW						410

// Enum: Enum Symbol of Entity Type
// _CIRCLE_ARC -					0:円・円弧
// _COMPOSITE_CURVE -				1:複合曲線
// _CONIC_ARC -						2:円錐曲線
// _COPIOUS_DATA -					3:有意点列
// _PLANE -							4:平面
// _LINE -							5:線分
// _PARAMETRIC_SPLINE_CURVE -		6:パラメトリックスプライン曲線
// _PARAMETRIC_SPLINE_SURFACE -		7:パラメトリックスプライン曲面
// _POINT -							8:点
// _TRANSFORMATION_MATRIX -			9:変換行列
// _NURBSC -						10:有理Bスプライン曲線
// _NURBSS -						11:有理Bスプライン曲面
// _CURVE_ON_PARAMETRIC_SURFACE -	12:面上線
// _TRIMMED_SURFACE -				13:トリム面
// _SUBFIGURE_DEFINITION -			14:子図の定義
// _ASSOCIATIVITY_INSTANCE -		15:グループ
// _DRAWING -						16:図面
// _PROPERTY -						17:図面サイズ
// _SINGULAR_SUBFIGURE_INSTANCE -	18:子図の参照
// _VIEW -							19:投象面
// _MESH -							20:メッシュ
enum EntityType{
	_CIRCLE_ARC = 0,
	_COMPOSITE_CURVE,
	_CONIC_ARC,
	_COPIOUS_DATA,
	_PLANE,
	_LINE,
	_PARAMETRIC_SPLINE_CURVE,
	_PARAMETRIC_SPLINE_SURFACE,
	_POINT,
	_TRANSFORMATION_MATRIX,
	_NURBSC,
	_NURBSS,
	_CURVE_ON_PARAMETRIC_SURFACE,
	_TRIMMED_SURFACE,
	_SUBFIGURE_DEFINITION,
	_ASSOCIATIVITY_INSTANCE,
	_DRAWING,
	_PROPERTY,
	_SINGULAR_SUBFIGURE_INSTANCE,
	_VIEW,
	_MESH,
		ALL_ENTITY_TYPE_NUM		// 21
};

/*
 * エンティティタイプごとに構造体を定義
 * 9つのエンティティタイプを読み込み対象とする(それ以外は読み捨て)
 * 追加する場合は、以下に追加するエンティティタイプの構造体を定義してください
 */

// Structure: CIRA
// 円・円弧を表わす構造体
// 
// Variables:
// double	zt -			Z軸方向の深さ
// Coord	cp[3] -			円・円弧の中心点、始点、終点
// double	R -				半径
// double	t[2] -			t[0]:始点の角度 t[1]:終点の角度
// Coord	U,V -			円，円弧を構成する平面の任意の直交ベクトル
// int		EntUseFlag -	ディレクトリ部 Entity Use Flag の値(0:幾何要素 5:2Dパラメトリック要素)
// int		pD -			ディレクトリ部への逆ポインタ
// DispStat	Dstat -			 表示属性（色r,g,b）
struct CIRA
{
	double zt;		
	Coord  cp[3];	
	double R;
	double t[2];
	Coord  U,V;
	int EntUseFlag;
	int pD;
	DispStat Dstat;

	CIRA() {
		zt = 0;
		cp[0] = cp[1] = cp[2] = SetCoord(0,0,0);
		R = 0;
		t[0] = t[1] = 0;
		U = V = SetCoord(0,0,0);
		EntUseFlag = 0;
		pD = 0;
	}
};

// Structure: CONA
// 円錐曲線を表わす構造体
//
// Variables:
// double prop[6] - 係数
// double zt -		ZT平面の定義
// Coord  cp[2] -	始点、終点
// int pD -			ディレクトリ部への逆ポインタ
// DispStat Dstat - 表示属性（色r,g,b）
struct CONA
{
	double prop[6];
	double zt;
	Coord  cp[2];
	int pD;
	DispStat Dstat;

	CONA() {
		InitVector(prop,6);
		zt = 0;
		cp[0] = cp[1] = SetCoord(0,0,0);
		pD = 0;
	}
};

// Structure: LINE_
// 線分を表わす構造体
//
// Variables:
// Coord cp[2] -	始点、終点
// int EntUseFlag - ディレクトリ部 Entity Use Flag の値(0:幾何要素 5:2Dパラメトリック要素)
// int pD -			ディレクトリ部への逆ポインタ
// DispStat Dstat - 表示属性（色r,g,b）
struct LINE_
{
	Coord cp[2];
	int EntUseFlag;
	int pD;
	DispStat Dstat;

	LINE_() {
		cp[0] = cp[1] = SetCoord(0,0,0);
		EntUseFlag = 0;
		pD = 0;
	}
};

// Structure: TMAT
// 変換マトリックスを表わす構造体
//
// Variables:
// double R[3][3] - 回転行列
// double T[3] -	並進ベクトル
// int pD -			ディレクトリ部への逆ポインタ
struct TMAT
{
	double R[3][3];	// 回転行列
	double T[3];	// 並進ベクトル
	int pD;			// ディレクトリ部への逆ポインタ

	TMAT() {
		for(int j=0;j<3;j++)
			InitVector(R[j],3);
		InitVector(T,3);
		pD = 0;
	}
};

// Structure: NURBSC
// 有理Bスプライン(NURBS)曲線を表わす構造体
//
// Variables:
// int K -			コントロールポイントの数
// int M -			階数(=次数+1)
// int N -			ノットベクトルの数
// int prop[4] -	各プロパティ
//					prop[0]==0:非平面内曲線, 1:平面内曲線
//					prop[1]==0:閉曲線でない，1:閉曲線
//					prop[2]==0:有理式，1:多項式
//					prop[3]==0:非周期的曲線, 1:周期的曲線	
// double *T -		ノットシーケンスの値 K+M個				
// double *W -		Weightの値 K個							
// Coord *cp -		コントロールポイント K個								
// double V[2] -	パラメータの範囲
// Coord norm -		法線ベクトル
// int EntUseFlag - ディレクトリ部 Entity Use Flag の値(0:幾何要素 5:2Dパラメトリック要素)
// int pD -			ディレクトリ部への逆ポインタ
// int OriginEnt -	元のエンティティタイプ
// void *pOriginEnt - 元のエンティティへのポインタ
// DispStat Dstat - 表示属性（色r,g,b）
struct NURBSC
{
	int K;
	int M;
	int N;
	int prop[4];
	double *T;
	double *W;
	Coord *cp;
//	boost::shared_array<double>	T;
//	boost::shared_array<double>	W;
//	boost::shared_array<Coord>	cp;
	double V[2];
	Coord norm;
	int EntUseFlag;
	int pD;
	int OriginEnt;
	void *pOriginEnt;
	DispStat Dstat;

	NURBSC() {
		K = 0;
		M = 0;
		N = 0;
		prop[0] = prop[1] = prop[2] = prop[3] = 0;
		T = NULL;
		W = NULL;
		cp = NULL;
		V[0] = V[1] = 0;
		norm = SetCoord(0,0,0);
		EntUseFlag = 0;
		pD = 0;
		OriginEnt = 0;
		pOriginEnt = NULL;
	}

	~NURBSC() {		// メモリリーク防止。。。できない？？
		if ( T )
			delete[]	T;
		if ( W )
			delete[]	W;
		if ( cp )
			delete[]	cp;
	}

	// Function: CalcNurbsCCoord
	// 指定したtでのNURBS曲線の座標値を求める
	Coord CalcNurbsCCoord(double);

	// Function: CalcNurbsCCoords
	// 指定したt群でのNURBS曲線の座標値群を求める
	void CalcNurbsCCoords(int,double *,Coord *);

	// Function: CalcDiffNurbsC
	// NURBS曲線の1階微分係数を求める
	Coord CalcDiffNurbsC(double);						

	// Function: CalcDiff2NurbsC
	// NURBS曲線の2階微分係数を求める
	Coord CalcDiff2NurbsC(double);						

	// Function: CalcDiffNNurbsC
	// NURBS曲線のr階微分係数を求める
	Coord CalcDiffNNurbsC(int,double);					

	// Function: CalcNurbsCLength
	// NURBS曲線の線分長を求める
	double CalcNurbsCLength(void);							

	// Function: CalcNurbsCLength
	// NURBS曲線の指定区間の線分長を求める
	double CalcNurbsCLength(double,double);			

	// Function: CalcDeltaPtsOnNurbsC
	// 指定した分割数でNURBS曲線上の座標値を求める
	int CalcDeltaPtsOnNurbsC(int,Coord *);				

	// Function: CalcTanVecOnNurbsC
	// NURBS曲線上のtにおける単位接ベクトルをもとめる
	Coord CalcTanVecOnNurbsC(double);					

	// Function: CalcCurvatureNurbsC
	// NURBS曲線の曲率を求める
	double CalcCurvatureNurbsC(double);				

	// Function: ShiftNurbsC
	// NURBS曲線のシフト
	void ShiftNurbsC(Coord);							

	// Function: ChRatioNurbsC
	// NURBS曲線の倍率を変更する
	void ChRatioNurbsC(Coord);							

	// Function: RotNurbsC
	// NURBS曲線を回転
	void RotNurbsC(Coord,double);						

	// Function: CalcIntersecPtNurbsPt
	// 空間上の1点からNURBS曲線上の最近傍点を求める(ニュートン法)(オーバーロード)
	int CalcIntersecPtNurbsPt(Coord,int,int,double *);			

	// Function: CalcIntersecCurve
	// NURBS曲線と平面との交点を求める(ニュートン法)
	int CalcIntersecCurve(Coord,Coord,int,double *,int,int);	

	// Function: CalcIntersecCurve3
	// 3次以下のNURBS曲線と平面との交点を求める
	int CalcIntersecCurve3(Coord,Coord,double *,int);	

    // Function: CalcIntersecPtsNurbsCLine
    // 2次元NURBS曲線と直線との交点を求める
    int ClacIntersecPtsNurbsCLine(Coord, Coord, double *, double *);

    // Function: CalcIntersecPtsNurbsCLineSeg
    // 2次元NURBS曲線と線分との交点を求める
    int ClacIntersecPtsNurbsCLineSeg(Coord, Coord, double, double, double *, double *);

	// Function: CalcParamLengthOnNurbsC
	// NURBS曲線において一端からの指定距離におけるパラメータ値を返す
	double CalcParamLengthOnNurbsC(double,double);		

	// Function: CalcDeltaPtsOnNurbsC
	// 指定した間隔でNURBS曲線上の座標値を求める
	int CalcDeltaPtsOnNurbsC(double,Coord *);			

	// Function: ReverseNurbsC
	// NURBS曲線のノットベクトル向きを反転する
	void ReverseNurbsC(void);								

	// Function: CalcExtremumNurbsC
	// NURBS曲線の指定した方向における極値の座標値を得る
	int CalcExtremumNurbsC(Coord,double *,int);		

#ifdef _DEBUG
	// Function: DebugForNurbsC
	// NURBS曲線情報をデバッグプリント
	void DebugForNurbsC(void);				
#endif

private:
	// Function: GetNurbsCCoef
	// (private)NURBS曲線の係数を求める(最高3次)
	int GetNurbsCCoef(double **,int,Coord *,double *);	
};

// Structure: NURBSS
// 有理Bスプライン(NURBS)曲面を表わす構造体
//
// Variables:
// int K[2] -		コントロールポイントの数(u方向,v方向)
// int M[2] -		階数(=次数+1)
// int N[2] -		ノットベクトルの数(K+M)
// int prop[5] -	パラメータ
//					prop[0]==0:u方向で閉じている, 1:閉じていない
//					prop[1]==0:v方向で閉じている，1:閉じていない
//					prop[2]==0:有理式，1:多項式
//					prop[3]==0:u方向で非周期的, 1:周期的
//					prop[4]==0:v方向で非周期的, 1:周期的
// double *S -		u方向ノットベクトルの値 A+1個			
// double *T -		v方向ノットベクトルの値 B+1個			
// double **W -		Weightの値								
// Coord  **cp -	コントロールポイント C個					
// double U[2] -	u方向パラメータの範囲
// double V[2] -	v方向パラメータの範囲
// int pD -			ディレクトリ部への逆ポインタ
// int TrmdSurfFlag - このNURBS曲面がトリム面として呼ばれているのか、独立して存在するのかを示すフラグ(トリム面:KOD_TRUE  独立面:KOD_FALSE)
// DispStat Dstat - 表示属性（色r,g,b,）
struct NURBSS
{
	int K[2];
	int M[2];
	int N[2];
	int prop[5];
	double *S;
	double *T;
	double **W;
	Coord  **cp;
	double U[2];
	double V[2];
	int pD;
	int TrmdSurfFlag;
	DispStat Dstat;

	NURBSS() {
		K[0] = K[1] = 0;
		M[0] = M[1] = 0;
		N[0] = N[0] = 0;
		prop[0] = prop[1] = prop[2] = prop[3] = prop[4] = 0;
		S = NULL;
		T = NULL;
		W = NULL;
		cp = NULL;
		U[0] = U[1] = 0;
		V[0] = V[1] = 0;
		pD = 0;
		TrmdSurfFlag = 0;
	}

	// Function: CalcNurbsSCoord
	// 指定したu,vでのNURBS曲面の座標点を求める
	Coord CalcNurbsSCoord(double,double);				

	// Function: CalcNurbsSCoords
	// 指定したu,v群でのNURBS曲面の座標値群を求める
	void CalcNurbsSCoords(int,Coord *,Coord *);		

	// Function: CalcDiffuNurbsS
	// NURBS曲面のu方向1階微分係数を求める
	Coord CalcDiffuNurbsS(double,double);				

	// Function: CalcDiffvNurbsS
	// NURBS曲面のv方向1階微分係数を求める
	Coord CalcDiffvNurbsS(double,double);				

	// Function: CalcDiffNNurbsS
	// NURBS曲面の各方向を任意階微分したときの微分係数を求める
	Coord CalcDiffNNurbsS(int,int,double,double);		

	// Function: CalcNormVecOnNurbsS
	// NURBS曲面上の(u,v)における法線ベクトルをもとめる
	Coord CalcNormVecOnNurbsS(double,double);			

	// Function: CalcDiffuNormVecOnNurbsS
	// NURBS曲面上の(u,v)における法線ベクトルのu方向1階微分をもとめる
	Coord CalcDiffuNormVecOnNurbsS(double,double);		

	// Function: CalcDiffvNormVecOnNurbsS
	// NURBS曲面上の(u,v)における法線ベクトルのv方向1階微分をもとめる
	Coord CalcDiffvNormVecOnNurbsS(double,double);		

	// Function: CalcMeanCurvature
	// NURBS曲面上の(u,v)における平均曲率を求める
	double CalcMeanCurvature(double,double);			

	// Function: CalcMeanCurvatureNormVec
	// NURBS曲面上の(u,v)における平均曲率法線ベクトルを求める
	Coord CalcMeanCurvatureNormVec(double,double);		

	// Function: CalcGaussCurvature
	// NURBS曲面上の(u,v)におけるガウス曲率を求める
	double CalcGaussCurvature(double,double);			

	// Function: CalcGaussCurvatureNormVec
	// NURBS曲面上の(u,v)におけるガウス曲率法線ベクトルを求める
	Coord CalcGaussCurvatureNormVec(double,double);	

	// Function: ShiftNurbsS
	// NURBS曲面のシフト
	void ShiftNurbsS(Coord);							

	// Function: ChRatioNurbsS
	// NURBS曲面の倍率を変更する
	void ChRatioNurbsS(Coord);							

	// Function: RotNurbsS
	// NURBS曲面を回転
	void RotNurbsS(Coord,double);						

	// Function: CalcuIntersecPtNurbsLine
	// NURBS曲面と直線の交点を算出
	int CalcuIntersecPtNurbsLine(Coord,Coord,int,Coord *,int,int);	

	// Function: CalcIntersecPtNurbsPt
	// 空間上の1点からNURBS曲面上の最近傍点を求める(ニュートン法)
	int CalcIntersecPtNurbsPt(Coord,int,int,Coord *);		

	// Function: CalcIntersecIsparaCurveU
	// u方向アイソパラ曲線と平面との交点を求める(ニュートン法)
	int CalcIntersecIsparaCurveU(double,Coord,Coord,int,double *,int);	

	// Function: CalcIntersecIsparaCurveV
	// v方向アイソパラ曲線と平面との交点を求める(ニュートン法)
	int CalcIntersecIsparaCurveV(double,Coord,Coord,int,double *,int); 

	// Function: CalcIntersecPtsPlaneV3
	// V方向のアイソパラ曲線を指定した分割数で生成し，各3次以下の曲線とNURBS曲面との交点を代数計算で算出する
	int CalcIntersecPtsPlaneV3(Coord,Coord,int,Coord *,int);	

	// Function: CalcIntersecPtsPlaneU3
	// V方向のアイソパラ曲線を指定した分割数で生成し，各3次以下の曲線とNURBS曲面との交点を代数計算で算出する
	int CalcIntersecPtsPlaneU3(Coord,Coord,int,Coord *,int);	

	// Function: CalcIntersecPtsPlaneV
	// V方向のアイソパラ曲線を指定した分割数で生成し，各曲線とNURBS曲面との交点を算出する
	int CalcIntersecPtsPlaneV(Coord,Coord,int,Coord *,int);	

	// Function: CalcIntersecPtsPlaneU
	// U方向のアイソパラ曲線を指定した分割数で生成し，各曲線とNURBS曲面との交点を算出する
	int CalcIntersecPtsPlaneU(Coord,Coord,int,Coord *,int);	

	// Function: CalcIntersecPtsPlaneGeom
	// NURBS曲面と平面と交点追跡用初期点を得る(補助平面を用いた方法)
	int CalcIntersecPtsPlaneGeom(Coord,Coord,int,int,Coord *,int);			

	// Function: CalcIntersecPtsPlaneSearch
	// NURBS曲面と平面との交点群を交線追跡法で求める
	int CalcIntersecPtsPlaneSearch(Coord,Coord,double,int,Coord *,int,int);	

	// Function: CalcIntersecPtsOffsetPlaneGeom
	// オフセットNURBS曲面と平面と交点追跡用初期点を得る(補助平面を用いた方法)(準備中)
	int CalcIntersecPtsOffsetPlaneGeom(double,Coord,Coord,int,Coord *,int);	

	// Function: CalcIntersecPtsOffsetPlaneSearch
	// オフセットNURBS曲面と平面との交点群を交線追跡法で求める(準備中)
	int CalcIntersecPtsOffsetPlaneSearch(double,Coord,Coord,double,int,Coord *,int);

	// Function: SearchExtremum_BS
	// Bulirsch-Stoer法により極地探索を行う
	int SearchExtremum_BS(Coord,double,double,double,int,int,Coord *);	

	// Function: CalcDeltaPtsOnNurbsS
	// 指定した分割数でNURBS曲面上の座標値を求める
	int CalcDeltaPtsOnNurbsS(int,int,Coord **);		

#ifdef _DEBUG
	// Function: DebugForNurbsS
	// NURBS曲面情報をデバッグプリント
	void DebugForNurbsS(void);
#endif

private:
	// Function: CalcDiffNurbsSDenom
	// (private)NURBS曲面分母の各方向を任意階微分したときの微分係数を求める
	double CalcDiffNurbsSDenom(int,int,double,double);		

	// Function: CalcDiffNurbsSNumer
	// (private)NURBS曲面分子の各方向を任意階微分したときの微分係数を求める
	Coord CalcDiffNurbsSNumer(int,int,double,double);		

	// Function: GetMinDist
	// (private)最小距離を調べる
	int GetMinDist(Coord ,Coord *,int ,Coord *);			

	// Function: SearchIntersectPt
	// (private)ニュートン法により交点を収束させる(NURBS曲面と平面)
	int SearchIntersectPt(Coord,Coord,double,double *,double *,int);	

	// Function: SearchIntersectPt_RKM
	// (private)4次のルンゲクッタ法により交点を収束させる(NURBS曲面と平面)
	int SearchIntersectPt_RKM(Coord,Coord,double,double *,double *,int);	

	// Function: SearchIntersectPt_BS
	// (private)Bulirsch-Stoer法により交点を収束させる(NURBS曲面と平面)
	int SearchIntersectPt_BS(Coord,Coord,double,double *,double *,int);	

	// Function: SearchIntersectPt_OS
	// (private)4次のルンゲクッタ法により交点を収束させる(オフセットNURBS曲面と平面)
	int SearchIntersectPt_OS(Coord,Coord,double,double *,double *,int);		

	// Function: GetSIPParam1
	// (private)NURBS曲面と平面の交点を表す微分方程式の右辺の値を得る
	int GetSIPParam1(double ,double ,Coord ,Coord ,int ,Coord *);		

	// Function: CalcIntersecPtsPlaneSearch_Sub
	// (private)面から飛び出した(u,v)を参考に面のエッジ部(new_u,new_v)を得る
	Coord CalcIntersecPtsPlaneSearch_Sub(double,double,Coord,Coord);	

	// Function: RemoveTheSamePoints
	// (private)NURBS曲面上の同一点を除去する
	int RemoveTheSamePoints(Coord *,int);					

	// Function: GetSECParam1
	// (private)極値探索線Sub関数1
	int GetSECParam1(double,double,Coord,int,int,Coord *);	
};

// Structure: COMPELEM
// 複合曲線を構成できる曲線群を共用体で宣言
//
// Variables:
// CONA ConA -		円錐曲線
// LINE_ Line -		直線
// NURBSC NurbsC -	NURBS曲線
union COMPELEM
{
//	CIRA CirA;
//	CONA ConA;
//	LINE_ Line;
//	NURBSC NurbsC;
	void*	substitution;	// ここに代入
	CIRA*	CirA;
	CONA*	ConA;
	LINE_*	Line;
	NURBSC*	NurbsC;
};

// Structure: COMPC
// 複合曲線
//
// Variables:
// int N -				構成要素数
// int *DEType -		各構成要素のエンティティタイプ
// COMPELEM **pDE -		各構成要素の構造体へのポインタ
// int DegeFlag -		複合曲線が縮退した2Dパラメトリック曲線を表すフラグ
// NURBSC DegeNurbs -	複合曲線が縮退した2Dパラメトリック曲線だった場合に縮退を解消するためのNURBS曲線
// int pD -				ディレクトリ部への逆ポインタ
struct COMPC
{
	int N;
	int *DEType;
//	COMPELEM **pDE;
	COMPELEM*	pDE;
	int DegeFlag;
	NURBSC DegeNurbs;
	int pD;

	COMPC() {
		N = 0;
		DEType = NULL;
		pDE = NULL;
		DegeFlag = 0;
		pD = 0;
	}
};

// Structure: CURVE
// 面上線を構成できる曲線群を共用体で宣言
//
// Variables:
// CIRA  CirA -		円・円弧
// COMPC CompC -	複合曲線
// CONA  ConA -		円錐曲線
// NURBSC NurbsC -	NURBS曲線
union CURVE
{
//	CIRA  CirA;
//	COMPC CompC;
//	CONA  ConA;
//	NURBSC NurbsC;
	void*	substitution;	// ここに代入
	CIRA*	CirA;
	COMPC*	CompC;
	CONA*	ConA;
	NURBSC*	NurbsC;
};
//typedef CURVE	boost::variant<CIRA*, COMPC*, CONA*, NURBSC*>

// Structure: CONPS
// 面上線
//
// Variables:
// int crtn -	面上線がどのように作られたかを示す
// int SType -	Surface Sのエンティティタイプ
// int BType -	Curve Bのエンティティタイプ
// int CType -	Curve Cのエンティティタイプ
// NURBSS *pS - Curveが乗るSurface構造体へのポインタ
// CURVE *pB -	Surface Sのパラメータ空間におけるCurve B構造体へのポインタ
// CURVE *pC -	Curve C構造体へのポインタ
// int pref -	送り側システムで採られていた表現を示すフラグ
// int pD -		ディレクトリ部への逆ポインタ
struct CONPS
{
	int crtn;
	int SType;
	int BType;
	int CType;
	NURBSS *pS;
//	CURVE *pB;
//	CURVE *pC;
	CURVE pB;
	CURVE pC;
	int pref;
	int pD;

	CONPS() {
		crtn = 0;
		SType = 0;
		BType = 0;
		CType = 0;
		pS = NULL;
		pB.substitution = NULL;
		pC.substitution = NULL;
		pref = 0;
		pD = 0;
	}
};

// Class TRMS
// トリム面定義クラス
class TRMS
{
public:
	TRMS() {
		pts = NULL;
		n1 = 0;
		n2 = 0;
		pTO = NULL;
		pTI = NULL;
		pD = 0;
	}

    // Function: GetOuterEdgeNum
    // トリム面を構成する外側エッジの数を取得する
    int GetOuterEdgeNum();

    // Function: GetInnerTrmNum
    // トリム面を構成する内側トリムの数を取得する
    int GetInnerTrmNum();

    // Function: GetInnerEdgeNum
    // トリム面を構成する内側トリムを構成するエッジの数を取得する
    int GetInnerEdgeNum(int);

    // Function: GetOuterCompC
    // トリム面を構成する外側トリム曲線(複合曲線)へのポインタを取得する
    COMPC *GetOuterCompC();

    // Function: GetInnerCompC
    // トリム面を構成する内側トリム曲線(複合曲線)へのポインタを取得する
    COMPC *GetInnerCompC(int);

    // Funciton: GetNurbsS
    // トリム面を構成するNURBS曲面へのポインタを得る
    NURBSS *GetNurbsS();

public:
    // Variable: *pts
    // トリムされるSurface EntityのDE部へのポインタ
    NURBSS *pts;

    // Variable: n1
    // 0:外周がDの境界と一致、1:それ以外
    int n1;

    // Variable: n2
    // Trimmed Surfaceの内周にあたる単純閉曲線の数
    int n2;

    // Variable: *pTO
    // Trimmed Surfaceの外周にあたる単純閉曲線構造体へのポインタ
    CONPS *pTO;

    // Variable: **pTI
    // Trimmed Surfaceの内周にあたる単純閉曲線構造体へのポインタ
    CONPS **pTI;

    // Variable: pD
    // ディレクトリ部への逆ポインタ
    int pD;
};

// Typedef: TRMS
// TRIMD_NURBSS - トリム面に対してNurbs曲面を想起させる名称を与えておく
typedef TRMS TRIMD_NURBSS;	// トリム面に対してNurbs曲面を想起させる名称を与えておく

// Structure: OBJECT
// ピックされたオブジェクトを示す構造体
//
// Variables:
// int Body -	BODYオブジェクトの番号
// int Type -	エンティティタイプのシンボル(NURBS曲線:126 , NURBS曲面:128 , トリム面:144)
// int Num -	Typeにおける要素番号(NURBS曲線が4本あったら、その4本に割り当てられた0～3の番号)
// int CCount - 何番目にピックされた曲線かを表す
// int SCount - 何番目にピックされた曲面かを表す
struct OBJECT
{
	int Body;		// BODYオブジェクトの番号
	int Type;		// エンティティタイプのシンボル(NURBS曲線:126 , NURBS曲面:128 , トリム面:144)
	int Num;		// Typeにおける要素番号(NURBS曲線が4本あったら、その4本に割り当てられた0～3の番号)
	int CCount;		// 何番目にピックされた曲線かを表す
	int SCount;		// 何番目にピックされた曲面かを表す
};

#include "NURBS_Func.h"

// Class: BODY
// 全てのエンティティを統括するBODYクラス
class BODY
{
public:
	// Constructor: BODY
	// BODYクラスのコンストラクタ．各種初期化
	BODY();
	~BODY();

	// Function: NewBodyElem
	// BODYを構成する全要素のメモリー確保
	void NewBodyElem();				

	// Function: DelBodyElem
	// BODYを構成する全要素のメモリー解放
	void DelBodyElem();				

	// Function: DelBodyElem
	// 指定したTypeNum[]分のメモリーを解放
	void DelBodyElem(int []);		

	// Function: NewCirA
	// CIRAを指定した数だけメモリー確保し，初期化する
	CIRA *NewCirA(int);	

	// Function: NewCompC
	// COMPCを指定した数だけメモリー確保し，初期化する
	COMPC *NewCompC(int);

	// Function: NewConA
	// CONAを指定した数だけメモリー確保し，初期化する
	CONA *NewConA(int);	

	// Function: NewLine
	// LINE_を指定した数だけメモリー確保し，初期化する
	LINE_ *NewLine(int);

	// Function: NewTMat
	// TMATを指定した数だけメモリー確保し，初期化する
	TMAT *NewTMat(int);		

	// Function: NewNurbsC
	// NURBSCを指定した数だけメモリー確保し，初期化する
	NURBSC *NewNurbsC(int);			

	// Function: NewNurbsS
	// NURBSSを指定した数だけメモリー確保し，初期化する
	NURBSS *NewNurbsS(int);			
	
	// Function: NewConpS
	// CONPSを指定した数だけメモリー確保し，初期化する
	CONPS *NewConpS(int);			

	// Function: NewTrmS
	// TRMSを指定した数だけメモリー確保し，初期化する
	TRMS *NewTrmS(int);				
	
	// Function: RotBody
	// BODYの回転
	void RotBody(Coord,double);	

	// Function: ShiftBody
	// BODYのシフト
	void ShiftBody(Coord);		

	// Function: ExpandBody
	// BODYの拡大縮小
	void ExpandBody(Coord);		

	// Function: RegistBody
	// 自分を新たなBODYとして登録する
	void RegistBody(BODYList *,const char []);		

	// Function: DeleteBody
	// 自分自身を消去する
	void DeleteBody(BODYList *);	

	// Function: RegistNurbsCtoBody
	// 1つのNURBS曲線を新たなBODYとして登録する
//	void RegistNurbsCtoBody(BODYList *,NURBSC,const char []);	
	void RegistNurbsCtoBody(BODYList *,const NURBSC&,const char []);	

	// Function: RegistNurbsCtoBodyN
	// N個のNURBS曲線を新たなBODYとして登録する
	void RegistNurbsCtoBodyN(BODYList *,NURBSC [],const char [],int);	

	// Function: RegistNurbsStoBody
	// 1つのNURBS曲面を新たなBODYとして登録する
	void RegistNurbsStoBody(BODYList *,NURBSS,const char []);	

	// Function: RegistNurbsStoBodyN
	// N個のNURBS曲面を新たなBODYとして登録する
	void RegistNurbsStoBodyN(BODYList *,NURBSS [],const char [],int);	

	// Function: ChangeStatColor
	// エンティティのステータスで定義されている色を変更
	void ChangeStatColor(float *,float,float,float,float);	

	// Function: InitCurveColor
	// 線の色の初期値を与える
	void InitCurveColor(float *);	

	// Function: InitSurfaceColor
	// 面の色の初期値を与える
	void InitSurfaceColor(float *);							

	// Function: GetNurbsCFromLine
	// 直線エンティティをNURBS曲線エンティティへと変換する
	int GetNurbsCFromLine(int,int);					

	// Function: GetNurbsCFromCirA
	// 円・円弧エンティティをNURBS曲線エンティティへと変換する
	int GetNurbsCFromCirA(int,int);							

private:
	// Function: CirAToNurbsC_seg1
	// (private)円・円弧エンティティが1セグメントの場合
	int CirAToNurbsC_seg1(int,int,Coord [],double);

	// Function: CirAToNurbsC_seg2
	// (private)円・円弧エンティティが2セグメントの場合
	int CirAToNurbsC_seg2(int,int,Coord [],double);	

	// Function: CirAToNurbsC_seg3
	// (private)円・円弧エンティティが3セグメントの場合
	int CirAToNurbsC_seg3(int,int,Coord [],double);	

	// Function: CirAToNurbsC_seg4
	// (private)円・円弧エンティティが4セグメントの場合
	int CirAToNurbsC_seg4(int,int,Coord [],double);			

public:
	// Variable: *CirA
	// 円・円弧
	CIRA  *CirA;		

	// Variable: *CompC
	// 複合曲線
	COMPC *CompC;

	// Variable: *ConA
	// 円錐曲線
	CONA  *ConA;

	// Variable: *Line
	// 線分
	LINE_ *Line;	

	// Variable: *TMat
	// 変換行列
	TMAT  *TMat;	

	// Variable: *NurbsC
	// NURBS曲線
	NURBSC *NurbsC;
//	boost::shared_array<NURBSC>	NurbsC;	// これ自体をスマートポインタにしても...

	// Variable: *NurbsS
	// NURBS曲面
	NURBSS *NurbsS;

	// Variable: *ConpS
	// 面上線
	CONPS *ConpS;		

	// Variable: *TrmS
	// トリム面
	TRMS  *TrmS;	

	// Variable: TypeNum[ALL_ENTITY_TYPE_NUM]
	// BODYを構成する各エンティティの数を格納した配列
	int  TypeNum[ALL_ENTITY_TYPE_NUM];	

	// Variable: *Mesh
	// Half-Edge構造メッシュ(リスト構造、リストの先頭アドレスを示す)
	MESH  *Mesh;

	// Variable: MaxCoord
	// 立体の寸法の最大値(この値で初期表示倍率を決定)
	double MaxCoord;
	// Add by K.Magara
	Coord	minmaxCoord[2];		// NCVC用 [0]:min [1]:max
	void	ClearMinMaxCoord(void);
	// --

	// Variable: Name[FNAMEMAX]
	// BODY名
//	char Name[FNAMEMAX];	
	std::string		Name;

	// Variable: *Mom
	// 自分が属する親(BodyList)のアドレス
	Data *Mom;				
};

#endif
