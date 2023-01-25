#ifndef _NURBS_FUNC_H_
#define _NURBS_FUNC_H_

#include "BODY.h"
#include "Quaternion.h"
#include "SFQuant.h"

// Constants: General Defines
// PTNUMMAX -			NURBSの点列の最大数(10000)
// RANKMAX -			NURBSの階数の最大値(9)
// INTERSECPTNUMMAX -	交点格納配列長(1000)
// NEAREST_GAP -		2点が同一点とみなす距離(0.01)
// CONVERG_GAP -		ニュートン法の収束を判別する閾値(0.00001)
// CONVDIVNUM -			収束計算用のパラメータ分割数(100)
// TRM_BORDERDIVNUM -	トリム境界線上に生成する点の数(100)
// FORWARD -			交線追跡の方向(順)(1)
// INVERSE -			交線追跡の方向(逆)(-1)
// PARAM_U -			u方向を表すシンボル(0)
// PARAM_V -			v方向を表すシンボル(1)
// OUTTER_TRIM -		外周トリミング領域(0)
// INNER_TRIM -			内周トリミング領域(1)
// PARAMDIVNUM -		初期値探索用のパラメータ分割数(10)
// RUNGE_KUTTA -		Runge-Kutta法のシンボル(0)
// BULIRSH_STOER -		Bulirsch-Stoer法のシンボル(1)
// CALC_OFFSET -		オフセット曲面計算のシンボル(2)
// BS_DIV -				Bulirsch-Stoer法の刻み数(11)
#define PTNUMMAX			10000
#define RANKMAX				9
#define INTERSECPTNUMMAX	1000
#define NEAREST_GAP			0.01
#define CONVERG_GAP			0.00001
#define CONVDIVNUM			100
#define TRM_BORDERDIVNUM	100
#define FORWARD				1
#define INVERSE				-1
#define PARAM_U				0
#define PARAM_V				1
#define OUTTER_TRIM			0
#define INNER_TRIM			1
#define PARAMDIVNUM			10
#define RUNGE_KUTTA			0
#define BULIRSH_STOER		1
#define CALC_OFFSET			2
#define BS_DIV				11

// Class: NURBS_Func
// NURBS曲線/曲面の操作を集めたクラス
class NURBS_Func
{
public:
	// Function: CalcNurbsCCoord
	// 指定したtでのNURBS曲線の座標値を求める
	Coord CalcNurbsCCoord(NURBSC *,double);

	// Function: CalcNurbsCCoords
	// 指定したt群でのNURBS曲線の座標値群を求める
	void CalcNurbsCCoords(NURBSC *,int,double *,Coord *);		

	// Function: CalcNurbsSCoord
	// 指定したu,vでのNURBS曲面の座標点を求める
	Coord CalcNurbsSCoord(NURBSS *,double,double);				

	// Function: CalcNurbsSCoords
	// 指定したu,v群でのNURBS曲面の座標値群を求める
	void CalcNurbsSCoords(NURBSS *,int,Coord *,Coord *);		

	// Function: GenNurbsC
	// 1つのNURBS曲線を生成する
	int GenNurbsC(NURBSC *,int,int,int,double [],double [],Coord [],double [],int[],int);	

	// Function: GenNurbsC
	// 1つのNURBS曲線を生成する(NURBS曲線のコピー)(オーバーロード)
	int GenNurbsC(NURBSC *,NURBSC);								

	// Function: GenNurbsS
	// 1つのNURBS曲面を生成する
	int GenNurbsS(NURBSS *,int,int,int,int,double *,double *,double **,Coord **,double,double,double,double);	

	// Function: GenNurbsS
	// 1つのNURBS曲面を生成する(NURBS曲面のコピー)(オーバーロード)
	int GenNurbsS(NURBSS *,NURBSS);								

	// Function: GenRotNurbsS
	// 1つのNURBS曲線をある軸回りにある角度だけ回転させた回転サーフェスを生成する
	int GenRotNurbsS(NURBSS *,NURBSC,Coord,double);				

	// Function: GenSweepNurbsS
	// 1つのNURBS曲線からある軸方向にある距離だけスイープさせたスイープサーフェスを生成する
	int GenSweepNurbsS(NURBSS *,NURBSC,Coord,double);			

	// Function: GenIsoparamCurveU
	// NURBS曲面上のu方向パラメータ値を固定したときのアイソパラメトリックNURBS曲線を生成
	int GenIsoparamCurveU(NURBSS *,double,NURBSC *);			

	// Function: GenIsoparamCurveV
	// NURBS曲面上のv方向パラメータ値を固定したときのアイソパラメトリックNURBS曲線を生成
	int GenIsoparamCurveV(NURBSS *,double,NURBSC *);			

	// Function: GenTrimdNurbsS
	// トリム面を生成する
	int GenTrimdNurbsS(TRIMD_NURBSS *,TRIMD_NURBSS);			

	// Function: DelTrimdNurbsS
	// トリム面を削除(メモリー解放)する
	int DelTrimdNurbsS(TRIMD_NURBSS *);							

	// Function: DelNurbsC
	// NURBS曲線を削除する
	void DelNurbsC(NURBSC *);									

	// Function: DelNurbsS
	// NURBS曲面を削除する
	void DelNurbsS(NURBSS *);									

	// Function: CalcBSbasis
	// Bスプライン基底関数を計算し、計算結果を返す
	double CalcBSbasis(double,double [],int,int,int);			

	// Function: CalcDiffBSbasis
	// Bスプライン基底関数の1階微分係数を求める
	double CalcDiffBSbasis(double,double [],int,int,int);		

	// Function: CalcDiffBSbasisN
	// Bスプライン基底関数のN階微分係数を求める
	double CalcDiffBSbasisN(double,double [],int,int,int,int);	

	// Function: CalcDiffNurbsC
	// NURBS曲線の1階微分係数を求める
	Coord CalcDiffNurbsC(NURBSC *,double);						

	// Function: CalcDiff2NurbsC
	// NURBS曲線の2階微分係数を求める
	Coord CalcDiff2NurbsC(NURBSC *,double);						

	// Function: CalcDiffNNurbsC
	// NURBS曲線のr階微分係数を求める
	Coord CalcDiffNNurbsC(NURBSC *,int,double);					

	// Function: CalcDiffuNurbsS
	// NURBS曲面のu方向1階微分係数を求める
	Coord CalcDiffuNurbsS(NURBSS *,double,double);				

	// Function: CalcDiffvNurbsS
	// NURBS曲面のv方向1階微分係数を求める
	Coord CalcDiffvNurbsS(NURBSS *,double,double);				

	// Function: CalcDiffNNurbsS
	// NURBS曲面の各方向を任意階微分したときの微分係数を求める
	Coord CalcDiffNNurbsS(NURBSS *,int,int,double,double);		

	// Function: CalcNormVecOnNurbsS
	// NURBS曲面上の(u,v)における法線ベクトルをもとめる
	Coord CalcNormVecOnNurbsS(NURBSS *,double,double);			

	// Function: CalcDiffuNormVecOnNurbsS
	// NURBS曲面上の(u,v)における法線ベクトルのu方向1階微分をもとめる
	Coord CalcDiffuNormVecOnNurbsS(NURBSS *,double,double);		

	// Function: CalcDiffvNormVecOnNurbsS
	// NURBS曲面上の(u,v)における法線ベクトルのv方向1階微分をもとめる
	Coord CalcDiffvNormVecOnNurbsS(NURBSS *,double,double);		

	// Function: CalcMeanCurvature
	// NURBS曲面上の(u,v)における平均曲率を求める
	double CalcMeanCurvature(NURBSS *,double,double);			

	// Function: CalcMeanCurvature
	// オーバーロード
	double CalcMeanCurvature(SFQuant);							

	// Function: CalcMeanCurvatureNormVec
	// NURBS曲面上の(u,v)における平均曲率法線ベクトルを求める
	Coord CalcMeanCurvatureNormVec(NURBSS *,double,double);		

	// Function: CalcGaussCurvature
	// NURBS曲面上の(u,v)におけるガウス曲率を求める
	double CalcGaussCurvature(NURBSS *,double,double);			

	// Function: CalcGaussCurvature
	// オーバーロード
	double CalcGaussCurvature(SFQuant);							

	// Function: CalcGaussCurvatureNormVec
	// NURBS曲面上の(u,v)におけるガウス曲率法線ベクトルを求める
	Coord CalcGaussCurvatureNormVec(NURBSS *,double,double);	

	// Function: CalcuIntersecPtNurbsLine
	// NURBS曲面と直線の交点を算出
	int CalcuIntersecPtNurbsLine(NURBSS *,Coord,Coord,int,Coord *,int,int);	

	// Function: CalcIntersecPtNurbsPt
	// 空間上の1点からNURBS曲面上の最近傍点を求める(ニュートン法)
	int CalcIntersecPtNurbsPt(NURBSS *,Coord,int,int,Coord *);		

	// Function: CalcIntersecPtNurbsPt
	// 空間上の1点からNURBS曲線上の最近傍点を求める(ニュートン法)(オーバーロード)
	int CalcIntersecPtNurbsPt(NURBSC *,Coord,int,int,double *);			

    // Function: CalcIntersecPtNurbsPtDescrete
    // 空間上の1点からNURBS曲面上の最近傍点を求める(離散的)
    void CalcIntersecPtNurbsPtDescrete(NURBSS *,Coord ,int ,int ,double ,double ,double ,double , Coord *);

	// Function: CalcIntersecIsparaCurveU
	// u方向アイソパラ曲線と平面との交点を求める(ニュートン法)
	int CalcIntersecIsparaCurveU(NURBSS *,double,Coord,Coord,int,double *,int);	

	// Function: CalcIntersecIsparaCurveV
	// v方向アイソパラ曲線と平面との交点を求める(ニュートン法)
	int CalcIntersecIsparaCurveV(NURBSS *,double,Coord,Coord,int,double *,int); 

	// Function: CalcIntersecCurve
	// NURBS曲線と平面との交点を求める(ニュートン法)
	int CalcIntersecCurve(NURBSC *,Coord,Coord,int,double *,int,int);	

	// Function: CalcIntersecCurve3
	// 3次以下のNURBS曲線と平面との交点を求める
	int CalcIntersecCurve3(NURBSC *,Coord,Coord,double *,int);	

	// Function: CalcIntersecPtsPlaneV3
	// V方向のアイソパラ曲線を指定した分割数で生成し，各3次以下の曲線とNURBS曲面との交点を代数計算で算出する
	int CalcIntersecPtsPlaneV3(NURBSS *,Coord,Coord,int,Coord *,int);	

	// Function: CalcIntersecPtsPlaneU3
	// V方向のアイソパラ曲線を指定した分割数で生成し，各3次以下の曲線とNURBS曲面との交点を代数計算で算出する
	int CalcIntersecPtsPlaneU3(NURBSS *,Coord,Coord,int,Coord *,int);	

	// Function: CalcIntersecPtsPlaneV
	// V方向のアイソパラ曲線を指定した分割数で生成し，各曲線とNURBS曲面との交点を算出する
	int CalcIntersecPtsPlaneV(NURBSS *,Coord,Coord,int,Coord *,int);	

	// Function: CalcIntersecPtsPlaneU
	// U方向のアイソパラ曲線を指定した分割数で生成し，各曲線とNURBS曲面との交点を算出する
	int CalcIntersecPtsPlaneU(NURBSS *,Coord,Coord,int,Coord *,int);	

	// Function: CalcIntersecPtsPlaneSearch
	// NURBS曲面と平面との交点群を交線追跡法で求める
	int CalcIntersecPtsPlaneSearch(NURBSS *,Coord,Coord,double,int,Coord *,int,int);	

	// Function: CalcIntersecPtsOffsetPlaneSearch
	// オフセットNURBS曲面と平面との交点群を交線追跡法で求める(準備中)
	int CalcIntersecPtsOffsetPlaneSearch(NURBSS *,double,Coord,Coord,double,int,Coord *,int);

	// Function: CalcIntersecPtsNurbsSNurbsC
	// NURBS曲面とNURBS曲線との交点を求める(ニュートン法)
	int CalcIntersecPtsNurbsSNurbsC(NURBSS *,NURBSC *,int,Coord *,int);	

	// Function: CalcIntersecPtsNurbsSGeom
	// NURBS曲面同士の交線上の点を幾何学的にいくつか求める
	int CalcIntersecPtsNurbsSGeom(NURBSS *,NURBSS *,int,int,Coord *,Coord *,int);		

	// Function: CalcIntersecPtsNurbsSSearch
	// NURBS曲面同士の交線(交点群)を交線追跡法で求める
	int CalcIntersecPtsNurbsSSearch(NURBSS *,NURBSS *,int,double,Coord *,Coord *,int);	

	// Function: CalcIntersecPtsNurbsCNurbsCParam
    // 2次元NURBS曲線同士の交点を求める
	int CalcIntersecPtsNurbsCNurbsCParam(NURBSC *,NURBSC *,int,Coord *,int);	

    // Function: CalcIntersecPtsNurbsCLine
    // 2次元NURBS曲線と直線との交点を求める
    int ClacIntersecPtsNurbsCLine(NURBSC *, Coord, Coord, double *, double *);

    // Function: CalcIntersecPtsNurbsCLineSeg
    // 2次元NURBS曲線と線分との交点を求める
    int ClacIntersecPtsNurbsCLineSeg(NURBSC *, Coord, Coord, double, double, double *, double *);

	// Function: SearchExtremum_BS
	// Bulirsch-Stoer法により極地探索を行う
	int SearchExtremum_BS(NURBSS *,Coord,double,double,double,int,int,Coord *);	

	// Function: GetBSplCoef3
	// 3次のBスプライン曲線の各係数を求める　(at^3 + bt^2 + ct + dの係数a,b,c,dを返す)
	int GetBSplCoef3(int,int,int,double *,double **);			

	// Function: GetBSplCoef2
	// 2次のBスプライン曲線の各係数を求める　(at^2 + bt + cの係数a,b,cを返す)
	int GetBSplCoef2(int,int,int,double *,double **);			

	// Function: GetBSplCoef1
	// 1次のBスプライン曲線の各係数を求める　(at + bの係数a,bを返す)
	int GetBSplCoef1(int,int,int,double *,double **);			

	// Function: ShiftNurbsS
	// NURBS曲面のシフト
	void ShiftNurbsS(NURBSS *,Coord);							

	// Function: ShiftNurbsC
	// NURBS曲線のシフト
	void ShiftNurbsC(NURBSC *,Coord);							

	// Function: ChRatioNurbsS
	// NURBS曲面の倍率を変更する
	void ChRatioNurbsS(NURBSS *,Coord);							

	// Function: ChRatioNurbsC
	// NURBS曲線の倍率を変更する
	void ChRatioNurbsC(NURBSC *,Coord);							

	// Function: RotNurbsS
	// NURBS曲面を回転
	void RotNurbsS(NURBSS *,Coord,double);						

	// Function: RotNurbsC
	// NURBS曲線を回転
	void RotNurbsC(NURBSC *,Coord,double);						

	// Function: SetCPNurbsS
	// コントロールポイントを代入する
	int SetCPNurbsS(NURBSS *,NURBSS);							

	// Function: GenInterpolatedNurbsC1
	// 与えられた点列を補間するn階のNURBS曲線を生成する
	int GenInterpolatedNurbsC1(NURBSC *,Coord *,int,int);		

	// Function: GenInterpolatedNurbsC2
	// 与えられた点列を補間するn階のNURBS曲線を生成する(閉じた曲線)
	int GenInterpolatedNurbsC2(NURBSC *,Coord *,int,int);		

	// Function: GenApproximationNurbsC
	// 与えられた点列を近似するn階のNURBS曲線を生成する
	int GenApproximationNurbsC(NURBSC *,Coord *,int,int);		

	// Function: GenNurbsCfromCP
	// コントロールポイントからNURBS曲線を生成する
	int GenNurbsCfromCP(NURBSC *,Coord *,int,int);				

	// Function: GenPolygonalLine
	// 折れ線を生成する
	int GenPolygonalLine(NURBSC *,Coord *,int);					

	// Function: GenInterpolatedNurbsS1
	// 与えられた点列を補間するn階NURBS曲面を生成する
	int GenInterpolatedNurbsS1(NURBSS *,Coord **,int,int,int,int);	

	// Function: GenPolygonalSurface
	// 折れ面を生成する
	int GenPolygonalSurface(NURBSS *,Coord **,int,int);				

	// Function: GenApproximationNurbsS
	// 与えられた点列を近似するn階のNURBS曲面を生成する
	int GenApproximationNurbsS(NURBSS *,Coord **,int,int,int,int);	

	// Function: GenNurbsSfromCP
	// 与えられたコントロールポイントからn階のNURBS曲面を生成する
	int GenNurbsSfromCP(NURBSS *,Coord **,int,int,int,int);			

	// Function: DetermPtOnTRMSurf
	// 注目中のNURBS曲面上の1点(u,v)がトリミング領域内にあるのかを判定する
	int DetermPtOnTRMSurf(TRMS *,double,double);					

	// Function: GetPtsOnOuterTRMSurf
	// 外周トリム面内の点のみ残す
	int GetPtsOnOuterTRMSurf(TRMS *,Coord *,int);					

	// Function: GetPtsOnInnerTRMSurf
	// 内周トリム面外の点のみ残す
	int GetPtsOnInnerTRMSurf(TRMS *,Coord *,int);					 

	// Function: GetPtsOnInnerOuterTRMSurf
	// 内外周トリム面内の点のみ残す
	int GetPtsOnInnerOuterTRMSurf(TRMS *,Coord *,int);				

	// Function: DetectInterfereNurbsS
	// NURBS曲面(トリム無)同士の干渉検出
	int DetectInterfereNurbsS(NURBSS *,NURBSS *,int);				

	// Function: DetectInterfereTrmS
	// NURBS曲面(トリム有)同士の干渉検出
	int DetectInterfereTrmS(TRIMD_NURBSS *,TRIMD_NURBSS *,int);		

	// Function: CalcIntersecPtsPlaneGeom
	// NURBS曲面と平面と交点追跡用初期点を得る(補助平面を用いた方法)
	int CalcIntersecPtsPlaneGeom(NURBSS *,Coord,Coord,int,int,Coord *,int);			

	// Function: CalcNurbsCLength
	// NURBS曲線の線分長を求める
	double CalcNurbsCLength(NURBSC *);							

	// Function: CalcNurbsCLength
	// NURBS曲線の指定区間の線分長を求める
	double CalcNurbsCLength(NURBSC *,double,double);			

	// Function: CalcDeltaPtsOnNurbsC
	// 指定した分割数でNURBS曲線上の座標値を求める
	int CalcDeltaPtsOnNurbsC(NURBSC *,int,Coord *);				

	// Function: CalcDeltaPtsOnNurbsS
	// 指定した分割数でNURBS曲面上の座標値を求める
	int CalcDeltaPtsOnNurbsS(NURBSS *,int,int,Coord **);		

	// Function: CalcExtremumNurbsC
	// NURBS曲線の指定した方向における極値の座標値を得る
	int CalcExtremumNurbsC(NURBSC *,Coord,double *,int);		

	// Function: GetEqIntervalKont
	// 曲線/曲面パラメータから等間隔なノットベクトルを算出
	void GetEqIntervalKont(int,int,Vector);						

	// Function: ChangeKnotVecRange
	// ノットベクトルのパラメータ定義域を変更する
	void ChangeKnotVecRange(Vector,int,int,int,double,double);	


	int CalcExtSearchCurve(NURBSS *,Coord,Coord,double,NURBSC *,NURBSC *);			// 極地探索線を得る(準備中)
	int CalcExtGradCurve(NURBSS *,Coord,Coord,double,NURBSC *,NURBSC *);			// 極地傾斜線を得る(準備中)
	int TrimNurbsSPlane(TRMS *,Coord,Coord);										// NURBS曲面を平面でトリムする(準備中)

	// Function: New_NurbsC
	// NURBS曲線のメモリー確保
	int New_NurbsC(NURBSC *,int,int);			

	// Function: Free_NurbsC_1DArray
	// NURBS曲線配列のメモリー解放
	void Free_NurbsC_1DArray(NURBSC *,int);		

	// Function: Free_NurbsC
	// NURBS曲線のメモリー解放
	void Free_NurbsC(NURBSC *);					

	// Function: New_NurbsS
	// NURBS曲面のメモリー確保
	int New_NurbsS(NURBSS *,int [],int []);		

	// Function: Free_NurbsS_1DArray
	// NURBS曲面配列のメモリー解放
	void Free_NurbsS_1DArray(NURBSS *,int);		

	// Function: Free_NurbsS
	// NURBS曲面のメモリー解放
	void Free_NurbsS(NURBSS *);					

	// Function: New_TrmS
	// トリム面のメモリー確保
	int New_TrmS(TRMS *,int);					

	// Function: Free_TrmS_1DArray
	// トリム面配列のメモリー解放
	void Free_TrmS_1DArray(TRMS *,int);			

	// Function: Free_TrmS
	// トリム面のメモリー解放
	void Free_TrmS(TRMS *);						

	// Function: New_CompC
	// 複合曲線のメモリー確保
	int New_CompC(COMPC *,int);					

	// Function: Free_CompC_1DArray
	// 複合曲線配列のメモリー解放
	void Free_CompC_1DArray(COMPC *,int);		

	// Function: Free_CompC
	// 複合曲線のメモリー解放
	void Free_CompC(COMPC *);					

	// Function: DebugForNurbsC
	// NURBS曲線情報をデバッグプリント
	void DebugForNurbsC(NURBSC *);				

	// Function: DebugForNurbsS
	// NURBS曲面情報をデバッグプリント
	void DebugForNurbsS(NURBSS *);				

	// Function: CalcIntersecPtsOffsetPlaneGeom
	// オフセットNURBS曲面と平面と交点追跡用初期点を得る(補助平面を用いた方法)(準備中)
	int CalcIntersecPtsOffsetPlaneGeom(NURBSS *,double,Coord,Coord,int,Coord *,int);	

	// Function: CalcTanVecOnNurbsC
	// NURBS曲線上のtにおける単位接ベクトルをもとめる
	Coord CalcTanVecOnNurbsC(NURBSC *,double);					

	// Function: ConnectNurbsSU
	// 2枚のNURBS曲面を連結する(U方向に長くなる)(S1_U1とS2_U0を連結)
	int ConnectNurbsSU(NURBSS *,NURBSS *,NURBSS *);				

	// Function: ConnectNurbsSV
	// 2枚のNURBS曲面を連結する(V方向に長くなる)(S1_V1とS2_V0を連結)
	int ConnectNurbsSV(NURBSS *,NURBSS *,NURBSS *);				

	// Function: CalcCurvatureNurbsC
	// NURBS曲線の曲率を求める
	double CalcCurvatureNurbsC(NURBSC *,double);				


//	double CalcTorsionNurbsC(NURBSC *,double);					// NURBS曲線の捩率を求める

	// Function: DivNurbsCParam
	// NURBS曲線を指定したパラメータ値で分割する
	int DivNurbsCParam(NURBSC *, NURBSC *, NURBSC *, double);	

	// Function: DivNurbsC
	// NURBS曲線を指定した位置（端からの距離）で分割する
	int DivNurbsC(NURBSC *, NURBSC *, NURBSC *, double);		

	// Function: ConnectNurbsC
	// NURBS曲線の連結
	int ConnectNurbsC(NURBSC *, NURBSC *, NURBSC *);			

	// Function: ReverseNurbsC
	// NURBS曲線のノットベクトル向きを反転する
	void ReverseNurbsC(NURBSC *);								

	// Function: CalcParamLengthOnNurbsC
	// NURBS曲線において一端からの指定距離におけるパラメータ値を返す
	double CalcParamLengthOnNurbsC(NURBSC *,double,double);		


	//int CalcDeltaParamsOnNurbsC(NURBSC *,double,Coord *);		// 指定したパラメータの間隔でNURBS曲線上の座標値を出力する

	// Function: CalcDeltaPtsOnNurbsC
	// 指定した間隔でNURBS曲線上の座標値を求める
	int CalcDeltaPtsOnNurbsC(NURBSC *,double,Coord *);			

private:


	// Function: GetNurbsCCoef
	// (private)NURBS曲線の係数を求める(最高3次)
	int GetNurbsCCoef(NURBSC *,double **,int,Coord *,double *);	

	// Function: CalcEquation
	// (private)3次方程式までを判別して解く
	int CalcEquation(double *,double *,int);					

	// Function: GetNurbsSCoef
	// (private)NURBS曲面においてuまたはvを固定した場合に得られるNURBS曲線C(u) or C(v)の分母分子の係数を求める
	void GetNurbsSCoef(int,double **,double *,Coord *,int,Coord *,double *);	

	// Function: GetIntersecEquation
	// (private)NURBS曲線と平面の交線導出用3次方程式を得る
	void GetIntersecEquation(int,Coord *,double *,Coord,Coord,double *);		

	// Function: SearchIntersectPt
	// (private)ニュートン法により交点を収束させる(NURBS曲面と平面)
	int SearchIntersectPt(NURBSS *,Coord,Coord,double,double *,double *,int);	

	// Function: SearchIntersectPt_RKM
	// (private)4次のルンゲクッタ法により交点を収束させる(NURBS曲面と平面)
	int SearchIntersectPt_RKM(NURBSS *,Coord,Coord,double,double *,double *,int);	

	// Function: SearchIntersectPt_BS
	// (private)Bulirsch-Stoer法により交点を収束させる(NURBS曲面と平面)
	int SearchIntersectPt_BS(NURBSS *,Coord,Coord,double,double *,double *,int);	

	// Function: SearchIntersectPt_OS
	// (private)4次のルンゲクッタ法により交点を収束させる(オフセットNURBS曲面と平面)
	int SearchIntersectPt_OS(NURBSS *,Coord,Coord,double,double *,double *,int);		

	// Function: GetSIPParam1
	// (private)NURBS曲面と平面の交点を表す微分方程式の右辺の値を得る
	int GetSIPParam1(NURBSS *,double ,double ,Coord ,Coord ,int ,Coord *);		

	// Function: SearchIntersectPt
	// (private)ニュートン法により交点を収束させる(NURBS曲面同士)
	int SearchIntersectPt(NURBSS *,NURBSS *,double,double *,double *,double *,double *,int);		

	// Function: DetermPtOnTRMSurf_sub
	// (private)トリム境界線が複合曲線の場合のトリミング領域内外判定
	int DetermPtOnTRMSurf_sub(CONPS *,double,double);				

	// Function: ApproxTrimBorder
	// (private)トリム境界線を点群で近似する
	int ApproxTrimBorder(COMPC *,Coord *);							

	// Function: GetCurveKnotParam1
	// (private)各通過点の曲線パラメータを算出(コード長の比から算出)
	void GetCurveKnotParam1(Coord *,int,Vector);					

	// Function: GetCurveKnotParam2
	// (private)各通過点の曲線パラメータを算出(コード長の平方根の比から算出)
	void GetCurveKnotParam2(Coord *,int,Vector);					

	// Function: GetSurfaceKnotParam
	// (private)各通過点の曲面パラメータを算出
	void GetSurfaceKnotParam(Vector,Vector,Coord **,int,int);		

	// Function: GetInterpolatedKnot
	// (private)曲線/曲面パラメータから補間用ノットベクトルを算出
	void GetInterpolatedKnot(Vector,int,int,int,Vector);			

	// Function: GetApproximatedKnot
	// (private)曲線/曲面パラメータから近似用ノットベクトルを算出
	void GetApproximatedKnot(Vector,int,int,int,Vector);			

	// Function: SetApproximationCPnum
	// (private)点列数から生成するコントロールポイント数を算定する
	int SetApproximationCPnum(int);									

	// Function: CalcApproximationCP_LSM
	// (private)最小2乗法で近似コントロールポイントを求める
	void CalcApproximationCP_LSM(Coord *,Vector,Vector,int,int,int,int,Coord *);	

	// Function: RemoveTheSamePoints
	// (private)NURBS曲面上の同一点を除去する
	int RemoveTheSamePoints(NURBSS *,Coord *,int);					

	// Function: CalcDiffNurbsSDenom
	// (private)NURBS曲面分母の各方向を任意階微分したときの微分係数を求める
	double CalcDiffNurbsSDenom(NURBSS *,int,int,double,double);		

	// Function: CalcDiffNurbsSNumer
	// (private)NURBS曲面分子の各方向を任意階微分したときの微分係数を求める
	Coord CalcDiffNurbsSNumer(NURBSS *,int,int,double,double);		

	// Function: TrimNurbsSPlaneSub1
	// (private)TrimNurbsSPlaneのサブ関数(2直線の交点をもとめる)
	Coord TrimNurbsSPlaneSub1(double,double,double,double,double,double); 

	// Function: CalcIntersecPtsPlaneSearch_Sub
	// (private)面から飛び出した(u,v)を参考に面のエッジ部(new_u,new_v)を得る
	Coord CalcIntersecPtsPlaneSearch_Sub(NURBSS *,double,double,Coord,Coord);	

	// Function: GetMinDistance
	// (private)最小距離を持つ座標値を返す
	Coord GetMinDistance(Coord,Coord *,int);						

	// Function: CheckClossedPoints
	// (private)指定した点が他の2点を対角とする立方体の中に存在するかを調べる
	int CheckClossedPoints(Coord,Coord,Coord);				

	// Function: GetSECParam1
	// (private)極値探索線Sub関数1
	int GetSECParam1(NURBSS *,double,double,Coord,int,int,Coord *);	

	// Function: GetMinDist
	// (private)最小距離を調べる
	int GetMinDist(NURBSS *,Coord ,Coord *,int ,Coord *);			

	// Function: SetKnotVecSU_ConnectS
	// (private)NURBS曲面連結用SUB関数(連結後の曲面のU方向ノット定義域を設定する)
	void SetKnotVecSU_ConnectS(NURBSS *,NURBSS *,NURBSS *);			

	// Function: SetKnotVecSV_ConnectS
	// (private)NURBS曲面連結用SUB関数(連結後の曲面のV方向ノット定義域を設定する)
	void SetKnotVecSV_ConnectS(NURBSS *,NURBSS *,NURBSS *);			

	// Function: SetCPSU_ConnectS
	// (private)NURBS曲面連結用SUB関数(連結後の曲面のU方向コントロールポイントとウェイトを設定する)
	void SetCPSU_ConnectS(NURBSS *,NURBSS *,NURBSS *);				

	// Function: SetCPSV_ConnectS
	// (private)NURBS曲面連結用SUB関数(連結後の曲面のV方向コントロールポイントとウェイトを設定する)
	void SetCPSV_ConnectS(NURBSS *,NURBSS *,NURBSS *);				

	// Function: InsertNewKnotOnNurbsC
	// (private)NURBS曲線に新たなノットを挿入する
	int InsertNewKnotOnNurbsC(NURBSC *,NURBSC *,double,int);		

	// Function: SetKnotVecC_ConnectC
	// (private)NURBS曲線連結用SUB関数(連結後の曲線のノット定義域を設定する)
	void SetKnotVecC_ConnectC(NURBSC *,NURBSC *,NURBSC *);			

	// Function: SetCPC_ConnectC
	// (private)NURBS曲線連結用SUB関数(連結後の曲線のコントロールポイントとウェイトを設定する)
	void SetCPC_ConnectC(NURBSC *,NURBSC *,NURBSC *);				

};

#endif
