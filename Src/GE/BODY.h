// BODYの定義

#ifndef _BODY_H_
#define _BODY_H_

#include "StdAfx.h"
#include "KodListFunc.h"
#include "MESH.h"

#define ALL_ENTITY_TYPE_NUM	21	// 全エンティティタイプの数
#define CTLPNUMMAX  1024		// NURBSで用いられるコントロールポイントの数の上限
#define KNOTNUMMAX  1024		// NURBSで用いられるノットシーケンスの数の上限
#define GEOMTRYELEM 0			// IGESディレクトリ部"Entity Use Flag"より、幾何要素を示す
#define PARAMETRICELEM 5		// IGESディレクトリ部"Entity Use Flag"より、2Dパラメトリック要素を示す
#define NORM_KNOT_VAL	1		// ノットベクトルを正規化するときの範囲の最大値
#define MIN_KNOT_RANGE	0.0002	// 隣り合うノットベクトルの差がこの値以上であること

typedef KODlistData BODYList;	// 汎用データリストの型をBODYListとして再登録
typedef KODlistData OBJECTList;	// 汎用データリストの型をOBJECTListとして再登録

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
	_CIRCLE_ARC,					// 0:円・円弧
	_COMPOSITE_CURVE,				// 1:複合曲線
	_CONIC_ARC,						// 2:円錐曲線
	_COPIOUS_DATA,					// 3:有意点列
	_PLANE,							// 4:平面
	_LINE,							// 5:線分
	_PARAMETRIC_SPLINE_CURVE,		// 6:パラメトリックスプライン曲線
	_PARAMETRIC_SPLINE_SURFACE,		// 7:パラメトリックスプライン曲面
	_POINT,							// 8:点
	_TRANSFORMATION_MATRIX,			// 9:変換行列
	_NURBSC,						// 10:有理Bスプライン曲線
	_NURBSS,						// 11:有理Bスプライン曲面
	_CURVE_ON_PARAMETRIC_SURFACE,	// 12:面上線
	_TRIMMED_SURFACE,				// 13:トリム面
	_SUBFIGURE_DEFINITION,			// 14:子図の定義
	_ASSOCIATIVITY_INSTANCE,		// 15:グループ
	_DRAWING,						// 16:図面
	_PROPERTY,						// 17:図面サイズ
	_SINGULAR_SUBFIGURE_INSTANCE,	// 18:子図の参照
	_VIEW,							// 19:投象面
	_MESH							// 20:メッシュ
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
	Coord  U,V;		// 円，円弧を構成する平面の任意の直交ベクトル
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
	int prop[4];	// 各プロパティ								// prop[0]==0:非平面内曲線, 1:平面内曲線
	double *T;		// ノットシーケンスの値 K+M個				// prop[1]==0:閉曲線でない，1:閉曲線
	double *W;		// Weightの値 K個							// prop[2]==0:有理式，1:多項式
	Coord *cp;		// コントロールポイント K個					// prop[3]==0:非周期的曲線, 1:周期的曲線				
	double V[2];	// パラメータの範囲
	Coord norm;		// 法線ベクトル
	int EntUseFlag; // ディレクトリ部 Entity Use Flag の値(0:幾何要素 5:2Dパラメトリック要素)
	int pD;			// ディレクトリ部への逆ポインタ
	int OriginEnt;	// 元のエンティティタイプ
	void *pOriginEnt;	// 元のエンティティへのポインタ
	DispStat Dstat;	// 表示属性（色r,g,b）
}NURBSC;

// 有理Bスプライン(NURBS)曲面
typedef struct{
	int K[2];		// コントロールポイントの数(u方向,v方向)
	int M[2];		// 階数(=次数+1)
	int N[2];		// ノットベクトルの数(K+M)
	int prop[5];	// パラメータ								// prop[0]==0:u方向で閉じている, 1:閉じていない
	double *S;		// u方向ノットベクトルの値 A+1個			// prop[1]==0:v方向で閉じている，1:閉じていない
	double *T;		// v方向ノットベクトルの値 B+1個			// prop[2]==0:有理式，1:多項式
	double **W;		// Weightの値								// prop[3]==0:u方向で非周期的, 1:周期的
	Coord  **cp;	// コントロールポイント C個					// prop[4]==0:v方向で非周期的, 1:周期的
	double U[2];	// u方向パラメータの範囲
	double V[2];	// v方向パラメータの範囲
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

// ピックされたオブジェクトを示す構造体
typedef struct{
	int Body;		// BODYオブジェクトの番号
	int Type;		// エンティティタイプのシンボル(NURBS曲線:126 , NURBS曲面:128 , トリム面:144)
	int Num;		// Typeにおける要素番号(NURBS曲線が4本あったら、その4本に割り当てられた0～3の番号)
	int CCount;		// 何番目にピックされた曲線かを表す
	int SCount;		// 何番目にピックされた曲面かを表す
}OBJECT;

#include "NURBS_Func.h"


// 全てのエンティティを統括するBODYクラス
class BODY
{
public:
	BODY();							// コンストラクタ
	void NewBodyElem();				// BODYを構成する全要素のメモリー確保
	void DelBodyElem();				// BODYを構成する全要素のメモリー解放
	void DelBodyElem(int []);		// 指定したTypeNum[]分のメモリーを解放
	CIRA *NewCirA(int);				// CIRAを指定した数だけメモリー確保し，初期化する
	COMPC *NewCompC(int);			// COMPCを指定した数だけメモリー確保し，初期化する
	CONA *NewConA(int);				// CONAを指定した数だけメモリー確保し，初期化する
	LINE_ *NewLine(int);			// LINE_を指定した数だけメモリー確保し，初期化する
	TMAT *NewTMat(int);				// TMATを指定した数だけメモリー確保し，初期化する
	NURBSC *NewNurbsC(int);			// NURBSCを指定した数だけメモリー確保し，初期化する
	NURBSS *NewNurbsS(int);			// NURBSSを指定した数だけメモリー確保し，初期化する
	CONPS *NewConpS(int);			// CONPSを指定した数だけメモリー確保し，初期化する
	TRMS *NewTrmS(int);				// TRMSを指定した数だけメモリー確保し，初期化する
	
	void RotBody(Coord,double);		// BODYの回転
	void ShiftBody(Coord);			// BODYのシフト
	void ExpandBody(Coord);			// BODYの拡大縮小
	void RegistBody(BODYList *,char []);		// 自分を新たなBODYとして登録する
	void DeleteBody(BODYList *);				// 自分自身を消去する
	void RegistNurbsCtoBody(BODYList *,NURBSC,char []);			// 1つのNURBS曲線を新たなBODYとして登録する
	void RegistNurbsCtoBodyN(BODYList *,NURBSC [],char [],int);	// N個のNURBS曲線を新たなBODYとして登録する
	void RegistNurbsStoBody(BODYList *,NURBSS,char []);			// 1つのNURBS曲面を新たなBODYとして登録する
	void RegistNurbsStoBodyN(BODYList *,NURBSS [],char [],int);	// N個のNURBS曲面を新たなBODYとして登録する
	void ChangeStatColor(float *,float,float,float,float);	// エンティティのステータスで定義されている色を変更
	void InitCurveColor(float *);							// 線の色の初期値を与える
	void InitSurfaceColor(float *);							// 面の色の初期値を与える

	int GetNurbsCFromLine(int,int);							// 直線エンティティをNURBS曲線エンティティへと変換する
	int GetNurbsCFromCirA(int,int);							// 円・円弧エンティティをNURBS曲線エンティティへと変換する

private:
	int CirAToNurbsC_seg1(int,int,Coord [],double);			// 円・円弧エンティティが1セグメントの場合
	int CirAToNurbsC_seg2(int,int,Coord [],double);			// 円・円弧エンティティが2セグメントの場合
	int CirAToNurbsC_seg3(int,int,Coord [],double);			// 円・円弧エンティティが3セグメントの場合
	int CirAToNurbsC_seg4(int,int,Coord [],double);			// 円・円弧エンティティが4セグメントの場合

public:
	// IGES準拠
	CIRA  *CirA;		// 円・円弧
	COMPC *CompC;		// 複合曲線
	CONA  *ConA;		// 円錐曲線
	LINE_ *Line;		// 線分
	TMAT  *TMat;		// 変換行列
	NURBSC *NurbsC;		// NURBS曲線
	NURBSS *NurbsS;		// NURBS曲面
	CONPS *ConpS;		// 面上線
	TRMS  *TrmS;		// トリム面
	int  TypeNum[ALL_ENTITY_TYPE_NUM];	// BODYを構成する各エンティティの数を格納した配列

	// メッシュ(Half-Edge)
	MESH  *Mesh;			// メッシュ(リスト構造、リストの先頭アドレスを示す)

	double MaxCoord;		// 立体の寸法の最大値(この値で初期表示倍率を決定)
	char Name[FNAMEMAX];	// BODY名

	Data *Mom;				// 自分が属する親(BodyList)のアドレス
};


#endif
