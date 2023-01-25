// BODYの定義

#ifndef _BODY_H_
#define _BODY_H_

#include "StdAfx.h"
#include "KodListFunc.h"
#include "MESH.h"

// Constants: General Defines
// ALL_ENTITY_TYPE_NUM -	全エンティティタイプの数(21)
// CTLPNUMMAX -				NURBSで用いられるコントロールポイントの数の上限(1024)
// KNOTNUMMAX -				NURBSで用いられるノットシーケンスの数の上限(1024)
// GEOMTRYELEM -			IGESディレクトリ部"Entity Use Flag"より、幾何要素を示す(0)
// PARAMETRICELEM -			IGESディレクトリ部"Entity Use Flag"より、2Dパラメトリック要素を示す(5)
// NORM_KNOT_VAL -			ノットベクトルを正規化するときの範囲の最大値(1)
// MIN_KNOT_RANG -			隣り合うノットベクトルの差がこの値以上であること(0.0002)
#define ALL_ENTITY_TYPE_NUM	21
#define CTLPNUMMAX  1024
#define KNOTNUMMAX  1024
#define GEOMTRYELEM 0
#define PARAMETRICELEM 5
#define NORM_KNOT_VAL	1
#define MIN_KNOT_RANGE	0.0002

// Typedefs: KODlistData
// BODYList - 汎用データリストの型をBODYListとして再登録
// OBJECTList - 汎用データリストの型をOBJECTListとして再登録
typedef KODlistData BODYList;
typedef KODlistData OBJECTList;

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
	_CIRCLE_ARC,
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
	_MESH
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
typedef struct{
	double zt;		
	Coord  cp[3];	
	double R;
	double t[2];
	Coord  U,V;
	int EntUseFlag;
	int pD;
	DispStat Dstat;
}CIRA;

// Structure: CONA
// 円錐曲線を表わす構造体
//
// Variables:
// double prop[6] - 係数
// double zt -		ZT平面の定義
// Coord  cp[2] -	始点、終点
// int pD -			ディレクトリ部への逆ポインタ
// DispStat Dstat - 表示属性（色r,g,b）
typedef struct{
	double prop[6];
	double zt;
	Coord  cp[2];
	int pD;
	DispStat Dstat;
}CONA;

// Structure: LINE_
// 線分を表わす構造体
//
// Variables:
// Coord cp[2] -	始点、終点
// int EntUseFlag - ディレクトリ部 Entity Use Flag の値(0:幾何要素 5:2Dパラメトリック要素)
// int pD -			ディレクトリ部への逆ポインタ
// DispStat Dstat - 表示属性（色r,g,b）
typedef struct{
	Coord cp[2];
	int EntUseFlag;
	int pD;
	DispStat Dstat;
}LINE_;

// Structure: TMAT
// 変換マトリックスを表わす構造体
//
// Variables:
// double R[3][3] - 回転行列
// double T[3] -	並進ベクトル
// int pD -			ディレクトリ部への逆ポインタ
typedef struct{
	double R[3][3];	// 回転行列
	double T[3];	// 並進ベクトル
	int pD;			// ディレクトリ部への逆ポインタ
}TMAT;

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
typedef struct{
	int K;
	int M;
	int N;
	int prop[4];
	double *T;
	double *W;
	Coord *cp;
	double V[2];
	Coord norm;
	int EntUseFlag;
	int pD;
	int OriginEnt;
	void *pOriginEnt;
	DispStat Dstat;
}NURBSC;

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
typedef struct{
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
}NURBSS;

// Structure: COMPELEM
// 複合曲線を構成できる曲線群を共用体で宣言
//
// Variables:
// CONA ConA -		円錐曲線
// LINE_ Line -		直線
// NURBSC NurbsC -	NURBS曲線
typedef union{
	CIRA CirA;
	CONA ConA;
	LINE_ Line;
	NURBSC NurbsC;
}COMPELEM;

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
typedef struct{
	int N;
	int *DEType;
	COMPELEM **pDE;
	int DegeFlag;
	NURBSC DegeNurbs;
	int pD;
}COMPC;

// Structure: CURVE
// 面上線を構成できる曲線群を共用体で宣言
//
// Variables:
// CIRA  CirA -		円・円弧
// COMPC CompC -	複合曲線
// CONA  ConA -		円錐曲線
// NURBSC NurbsC -	NURBS曲線
typedef union{
	CIRA  CirA;
	COMPC CompC;
	CONA  ConA;
	NURBSC NurbsC;
}CURVE;

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
typedef struct{
	int crtn;
	int SType;
	int BType;
	int CType;
	NURBSS *pS;
	CURVE *pB;
	CURVE *pC;
	int pref;
	int pD;
}CONPS;

// Class TRMS
// トリム面定義クラス
class TRMS{
public:
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
typedef struct{
	int Body;		// BODYオブジェクトの番号
	int Type;		// エンティティタイプのシンボル(NURBS曲線:126 , NURBS曲面:128 , トリム面:144)
	int Num;		// Typeにおける要素番号(NURBS曲線が4本あったら、その4本に割り当てられた0～3の番号)
	int CCount;		// 何番目にピックされた曲線かを表す
	int SCount;		// 何番目にピックされた曲面かを表す
}OBJECT;


#include "NURBS_Func.h"

// Class: BODY
// 全てのエンティティを統括するBODYクラス
class BODY
{
public:
	// Constructor: BODY
	// BODYクラスのコンストラクタ．各種初期化
	BODY();

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
	void RegistNurbsCtoBody(BODYList *,NURBSC,const char []);	

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

	// Variable: Name[FNAMEMAX]
	// BODY名
	char Name[FNAMEMAX];	

	// Variable: *Mom
	// 自分が属する親(BodyList)のアドレス
	Data *Mom;				
};

#endif
