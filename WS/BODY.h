// BODYの定義

#ifndef _BODY_H_
#define _BODY_H_

#include "StdAfx.h"

#define ALL_ENTITY_TYPE_NUM	20		// 全エンティティタイプの数
#define CTLPNUMMAX  256		// NURBSで用いられるコントロールポイントの数の上限
#define KNOTNUMMAX  256		// NURBSで用いられるノットシーケンスの数の上限
#define GEOMTRYELEM 0		// IGESディレクトリ部"Entity Use Flag"より、幾何要素を示す
#define PARAMETRICELEM 5	// IGESディレクトリ部"Entity Use Flag"より、2Dパラメトリック要素を示す

typedef WSClistData BODYList;	// WideStudioが用意するデータリストの型をBODYListとして再登録
typedef WSClistData OBJECTList;	// WideStudioが用意するデータリストの型をOBJECTListとして再登録

// エンティティタイプのシンボルを定義
#define	CIRCLE_ARC					100		// 円/円弧
#define	COMPOSITE_CURVE				102		// 複合曲線
#define	CONIC_ARC					104		// 円錐曲線
#define	COPIOUS_DATA				106		// 有意点列
#define	PLANE						108		// 平面
#define	LINE						110		// 線分
#define	PARAMETRIC_SPLINE_CURVE		112		// パラメトリックスプライン曲線
#define	PARAMETRIC_SPLINE_SURFACE	114		// パラメトリックスプライン曲面
#define	POINT						116		// 点
#define	TRANSFORMATION_MATRIX		124		// 変換行列
#define	NURBS_CURVE					126		// 有理Bスプライン曲線
#define	NURBS_SURFACE				128		// 有理Bスプライン曲面
#define	CURVE_ON_PARAMETRIC_SURFACE 142		// 面上線
#define	TRIMMED_SURFACE				144		// トリム面
#define	SUBFIGURE_DEFINITION		308		// 子図の定義
#define	ASSOCIATIVITY_INSTANCE		402		// グループ
#define	DRAWING						404		// 図面
#define	PROPERTY					406		// 図面サイズ
#define	SINGULAR_SUBFIGURE_INSTANCE 408		// 子図の参照
#define	VIEW						410		// 投象面

// エンティティタイプのシンボルをenumで定義
enum EntityType{
	_CIRCLE_ARC,					// 円・円弧
	_COMPOSITE_CURVE,				// 複合曲線
	_CONIC_ARC,						// 円錐曲線
	_COPIOUS_DATA,					// 有意点列
	_PLANE,							// 平面
	_LINE,							// 線分
	_PARAMETRIC_SPLINE_CURVE,		// パラメトリックスプライン曲線
	_PARAMETRIC_SPLINE_SURFACE,		// パラメトリックスプライン曲面
	_POINT,							// 点
	_TRANSFORMATION_MATRIX,			// 変換行列
	_NURBSC,						// 有理Bスプライン曲線
	_NURBSS,						// 有理Bスプライン曲面
	_CURVE_ON_PARAMETRIC_SURFACE,	// 面上線
	_TRIMMED_SURFACE,				// トリム面
	_SUBFIGURE_DEFINITION,			// 子図の定義
	_ASSOCIATIVITY_INSTANCE,		// グループ
	_DRAWING,						// 図面
	_PROPERTY,						// 図面サイズ
	_SINGULAR_SUBFIGURE_INSTANCE,	// 子図の参照
	_VIEW							// 投象面
};

/*
 * エンティティタイプごとに構造体を定義
 * 9つのエンティティタイプを読み込み対象とする(それ以外は読み捨て)
 * 追加する場合は、以下に追加するエンティティタイプの構造体を定義してください
 */

// 円・円弧
typedef struct{
	double zt;		// Z軸方向の深さ
	Coord  cp[3];	// 円・円弧の中心点、始点、終点
	double R;		// 半径
	double t[2];	// t[0]:始点の角度 t[1]:終点の角度
	int EntUseFlag; // ディレクトリ部 Entity Use Flag の値(0:幾何要素 5:2Dパラメトリック要素)
	int pD;			// ディレクトリ部への逆ポインタ
	DispStat Dstat;	// 表示属性（色r,g,b）
}CIRA;

// 円錐曲線
typedef struct{
	double prop[6];	// 係数
	double zt;		// ZT平面の定義
	Coord  cp[2];	// 始点、終点
	int pD;			// ディレクトリ部への逆ポインタ
	DispStat Dstat;	// 表示属性（色r,g,b）
}CONA;

// 線分
typedef struct{
	Coord cp[2];	// 始点、終点
	int EntUseFlag; // ディレクトリ部 Entity Use Flag の値(0:幾何要素 5:2Dパラメトリック要素)
	int pD;			// ディレクトリ部への逆ポインタ
	DispStat Dstat;	// 表示属性（色r,g,b）
}LINE_;

// 変換マトリックス
typedef struct{
	double R[3][3];	// 回転行列
	double T[3];	// 並進ベクトル
	int pD;			// ディレクトリ部への逆ポインタ
}TMAT;

// 有理Bスプライン(NURBS)曲線
typedef struct{
	int K;			// コントロールポイントの数
	int M;			// 階数(=次数+1)
	int N;			// ノットベクトルの数
	int prop[4];	// 各プロパティ
	double *T;		// ノットシーケンスの値 A+1個
	double *W;		// Weightの値 K+1個
	Coord *cp;		// コントロールポイント K+1個
	double V[2];	// パラメータの範囲
	Coord norm;		// 法線ベクトル
	int EntUseFlag; // ディレクトリ部 Entity Use Flag の値(0:幾何要素 5:2Dパラメトリック要素)
	int pD;			// ディレクトリ部への逆ポインタ
	DispStat Dstat;	// 表示属性（色r,g,b）
}NURBSC;

// 有理Bスプライン(NURBS)曲面
typedef struct{
	int K[2];		// コントロールポイントの数(u方向,v方向)
	int M[2];		// 階数(=次数+1)
	int N[2];		// ノットベクトルの数
	int prop[5];	// パラメータ
	double *S;		// u方向ノットベクトルの値 A+1個
	double *T;		// v方向ノットベクトルの値 B+1個
	double **W;		// Weightの値
	Coord  **cp;	// コントロールポイント C個
	double U[2];	// u方向パラメータ
	double V[2];	// v方向パラメータ
	int pD;			// ディレクトリ部への逆ポインタ
	int TrmdSurfFlag;// このNURBS曲面がトリム面として呼ばれているのか、独立して存在するのかを示すフラグ(トリム面:KOD_TRUE  独立面:KOD_FALSE)
	DispStat Dstat;	// 表示属性（色r,g,b,）
}NURBSS;

// 複合曲線を構成できる曲線群を共用体で宣言
typedef union{
	CIRA CirA;		// 円・円弧
	CONA ConA;		// 円錐曲線
	LINE_ Line;		// 直線
	NURBSC NurbsC;	// NURBS曲線
}COMPELEM;

// 複合曲線
typedef struct{
	int N;			// 構成要素数
	int *DEType;	// 各構成要素のエンティティタイプ
	COMPELEM **pDE;	// 各構成要素の構造体へのポインタ
	int DegeFlag;	// 複合曲線が縮退した2Dパラメトリック曲線を表すフラグ
	NURBSC DegeNurbs;// 複合曲線が縮退した2Dパラメトリック曲線だった場合に縮退を解消するためのNURBS曲線
	int pD;			// ディレクトリ部への逆ポインタ
}COMPC;

// 面上線を構成できる曲線群を共用体で宣言
typedef union{
	CIRA  CirA;		// 円・円弧
	COMPC CompC;	// 複合曲線
	CONA  ConA;		// 円錐曲線
	NURBSC NurbsC;	// NURBS曲線
}CURVE;

// 面上線
typedef struct{
	int crtn;		// 面上線がどのように作られたかを示す
	int SType;		// Surface Sのエンティティタイプ
	int BType;		// Curve Bのエンティティタイプ
	int CType;		// Curve Cのエンティティタイプ
	NURBSS *pS;		// Curveが乗るSurface構造体へのポインタ
	CURVE *pB;		// Surface Sのパラメータ空間におけるCurve B構造体へのポインタ
	CURVE *pC;		// Curve C構造体へのポインタ
	int pref;		// 送り側システムで採られていた表現を示すフラグ
	int pD;			// ディレクトリ部への逆ポインタ
}CONPS;

// トリム面
typedef struct{
	NURBSS *pts;	// トリムされるSurface EntityのDE部へのポインタ
	int n1;			// 0:外周がDの境界と一致、1:それ以外
	int n2;			// Trimmed Surfaceの内周にあたる単純閉曲線の数
	CONPS *pTO;		// Trimmed Surfaceの外周にあたる単純閉曲線構造体へのポインタ
	CONPS **pTI;	// Trimmed Surfaceの内周にあたる単純閉曲線構造体へのポインタ
	int pD;			// ディレクトリ部への逆ポインタ
}TRMS;

typedef TRMS TRIMD_NURBSS;	// トリム面に対してNurbs曲面を想起させる名称を与えておく

// パス生成対象エンティティを示す構造体
typedef struct{
	int Body;		// BODYオブジェクトの番号
	int Type;		// エンティティタイプのシンボル(NURBS曲線:126 , NURBS曲面:128 , トリム面:144)
	int Num;		// Typeにおける要素番号(NURBS曲線が4本あったら、その4本に割り当てられた0～3の番号)
	int CCount;		// 何番目にピックされた曲線かを表す
	int SCount;		// 何番目にピックされた曲面かを表す
}OBJECT;

// 全てのエンティティを統括するBODY構造体
typedef struct{
	CIRA  *CirA;		// 円・円弧
	COMPC *CompC;		// 複合曲線
	CONA  *ConA;		// 円錐曲線
	LINE_ *Line;		// 線分
	TMAT  *TMat;		// 変換行列
	NURBSC *NurbsC;		// NRBS曲線
	NURBSS *NurbsS;		// NRBS曲面
	CONPS *ConpS;		// 面上線
	TRMS  *TrmS;		// トリム面
	double MaxCoord;	// 立体の寸法の最大値(この値で初期表示倍率を決定)
	int  TypeNum[ALL_ENTITY_TYPE_NUM];	// BODYを構成する各エンティティの数を格納した配列
	char Name[FNAMEMAX];	// BODY名
}BODY;

class BODY_Handler
{
public:
	void InitCurveColor(float *);							// 線の色の初期値を与える
	void InitSurfaceColor(float *);							// 面の色の初期値を与える
	void ChangeStatColor(float *,float,float,float,float);	// エンティティのステータスで定義されている色を変更
	BODY *SearchBodyList(BODYList *,int);					// セレクションされているBODY番号を得る
	void Init_body(BODY *);									// BODY初期化
	void RegistNurbsCtoBody(BODYList *,BODY *,NURBSC,char []);	// 1つのNURBC曲線を新たなBODYとして登録する
	void RegistNurbsCtoBodyN(BODYList *,BODY *,NURBSC [],char [],int);	// N個のNURBC曲線を新たなBODYとして登録する
	void RegistNurbsStoBody(BODYList *,BODY *,NURBSS,char []);	// 1つのNURBS曲面を新たなBODYとして登録する
	void RegistNurbsStoBodyN(BODYList *,BODY *,NURBSS [],char [],int);	// N個のNURBS曲面を新たなBODYとして登録する


	// 各エンティティのメモリー操作関数
	void New_body(BODY *,int []);				// 全エンティティのメモリー確保
	void Free_body(BODY *,int []);				// 全エンティティのメモリー解放
	int New_CompC(COMPC *,int);					// 複合曲線のメモリー確保
	void Free_CompC_1DArray(COMPC *,int);		// 複合曲線配列のメモリー解放
	void Free_CompC(COMPC *);					// 複合曲線のメモリー解放
	int New_NurbsC(NURBSC *,int,int);			// NURBS曲線のメモリー確保
	void Free_NurbsC_1DArray(NURBSC *,int);		// NURBS曲線配列のメモリー解放
	void Free_NurbsC(NURBSC *);					// NURBS曲線のメモリー解放
	int New_NurbsS(NURBSS *,int [],int []);		// NURBS曲面のメモリー確保
	void Free_NurbsS_1DArray(NURBSS *,int);		// NURBS曲面配列のメモリー解放
	void Free_NurbsS(NURBSS *);					// NURBS曲面のメモリー解放
	int New_TrmS(TRMS *,int);					// トリム面のメモリー確保
	void Free_TrmS_1DArray(TRMS *,int);			// トリム面配列のメモリー解放
	void Free_TrmS(TRMS *);						// トリム面のメモリー解放

};

#endif
