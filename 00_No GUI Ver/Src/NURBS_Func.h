#ifndef _NURBS_FUNC_H_
#define _NURBS_FUNC_H_

#include "BODY.h"
#include "Quaternion.h"

#define PTNUMMAX			10000			// NURBSの点列の最大数
#define RANKMAX				9				// NURBSの階数の最大値
#define INTERSECPTNUMMAX	1000			// 交点格納配列長
#define NEAREST_GAP			0.01			// 2点が同一点とみなす距離
#define CONVERG_GAP			0.00001			// ニュートン法の収束を判別する閾値
#define CONVDIVNUM			100				// 収束計算用のパラメータ分割数
#define TRM_BORDERDIVNUM	100				// トリム境界線上に生成する点の数
#define FORWARD				1				// 交線追跡の方向(順)
#define INVERSE				-1				// 交線追跡の方向(逆)
#define PARAM_U				0				// u方向を表すシンボル
#define PARAM_V				1				// v方向を表すシンボル
#define OUTTER_TRIM			0				// 外周トリミング領域
#define INNER_TRIM			1				// 内周トリミング領域
#define PARAMDIVNUM			10				// 初期値探索用のパラメータ分割数


// NURBSの操作を集めたクラスを定義
class NURBS_Func
{
public:
	Coord CalcNurbsCCoord(NURBSC *,double);						// 指定したノットtでのNURBS曲線の座標値を求める
	void CalcNurbsCCoords(NURBSC *,int,double *,Coord *);		// 指定したノットt群でのNURBS曲線の座標値群を求める
	Coord CalcNurbsSCoord(NURBSS *,double,double);				// 指定したノットu,vでのNURBS曲面の座標点を求める
	void CalcNurbsSCoords(NURBSS *,int,Coord *,Coord *);		// 指定したノットu,v群でのNURBS曲面の座標値群を求める
	int GenNurbsC(NURBSC *,int,int,int,double [],double [],Coord [],double [],int[],int);	// 1つのNURBS曲線を生成する
	int GenNurbsC(NURBSC *,NURBSC);								// 1つのNURBS曲線を生成する(NURBS曲線のコピー)(オーバーロード)
	int GenNurbsS(NURBSS *,int,int,int,int,double *,double *,double **,Coord **,double,double,double,double);	// 1つのNURBS曲面を生成する
	int GenNurbsS(NURBSS *,NURBSS);								// 1つのNURBS曲面を生成する(NURBS曲面のコピー)(オーバーロード)
	int GenRotNurbsS(NURBSS *,NURBSC,Coord,double);				// 1つのNURBS曲線をある軸回りにある角度だけ回転させた回転サーフェスを生成する
	int GenSweepNurbsS(NURBSS *,NURBSC,Coord,double);			// 1つのNURBS曲線からある軸方向にある距離だけスイープさせたスイープサーフェスを生成する
	int GenIsoparamCurveU(NURBSS *,double,NURBSC *);			// NURBS曲面上のu方向パラメータ値を固定したときのアイソパラメトリックNURBS曲線を生成
	int GenIsoparamCurveV(NURBSS *,double,NURBSC *);			// NURBS曲面上のv方向パラメータ値を固定したときのアイソパラメトリックNURBS曲線を生成
	int GenTrimdNurbsS(TRIMD_NURBSS *,TRIMD_NURBSS);			// トリム面を生成する
	int DelTrimdNurbsS(TRIMD_NURBSS *);							// トリム面を削除(メモリー解放)する
	void DelNurbsC(NURBSC *);									// NURBS曲線を削除する
	void DelNurbsS(NURBSS *);									// NURBS曲面を削除する
	double CalcBSbasis(double,double [],int,int,int);			// Bスプライン基底関数を計算し、計算結果を返す
	double CalcDiffBSbasis(double,double [],int,int,int);		// Bスプライン基底関数の1階微分係数を求める
	double CalcDiffBSbasisN(double,double [],int,int,int,int);	// Bスプライン基底関数のN階微分係数を求める
	Coord CalcDiffNurbsC(NURBSC *,double);						// NURBS曲線の1階微分係数を求める
	Coord CalcDiff2NurbsC(NURBSC *,double);						// NURBS曲線の2階微分係数を求める
	Coord CalcDiffNNurbsC(NURBSC *,int,double);					// NURBS曲線のr階微分係数を求める
	Coord CalcDiffuNurbsS(NURBSS *,double,double);				// NURBS曲面のu方向1階微分係数を求める
	Coord CalcDiffvNurbsS(NURBSS *,double,double);				// NURBS曲面のv方向1階微分係数を求める
	Coord CalcDiffNNurbsS(NURBSS *,int,int,double,double);		// NURBS曲面の各方向を任意階微分したときの微分係数を求める
	Coord CalcNormVecOnNurbsS(NURBSS *,double,double);			// NURBS曲面上の(u,v)における法線ベクトルをもとめる
	Coord CalcDiffuNormVecOnNurbsS(NURBSS *,double,double);		// NURBS曲面上の(u,v)における法線ベクトルのu方向1階微分をもとめる
	Coord CalcDiffvNormVecOnNurbsS(NURBSS *,double,double);		// NURBS曲面上の(u,v)における法線ベクトルのv方向1階微分をもとめる
	double CalcMeanCurvature(NURBSS *,double,double);			// NURBS曲面上の(u,v)における平均曲率を求める
	Coord CalcMeanCurvatureNormVec(NURBSS *,double,double);		// NURBS曲面上の(u,v)における平均曲率法線ベクトルを求める
	double CalcGaussCurvature(NURBSS *,double,double);			// NURBS曲面上の(u,v)におけるガウス曲率を求める
	Coord CalcGaussCurvatureNormVec(NURBSS *,double,double);	// NURBS曲面上の(u,v)におけるガウス曲率法線ベクトルを求める
	int CalcuIntersecPtNurbsLine(NURBSS *,Coord,Coord,int,Coord *,int);	// NURBS曲面と直線の交点を算出
	int CalcIntersecPtNurbsPt(NURBSS *,Coord,int,Coord *);		// NURBS曲面上における空間上の1点からの最近傍点を求める(ニュートン法)
	int CalcIntersecCurve(NURBSC *,Coord,Coord,int,double *,int);	// NURBS曲線と平面との交点を求める(ニュートン法)
	int CalcIntersecCurve3(NURBSC *,Coord,Coord,double *,int);	// 3次以下のNURBS曲線と平面との交点を求める
	int CalcIntersecPtsPlaneV3(NURBSS *,Coord,Coord,int,Coord *,int);	// V方向のアイソパラ曲線を指定した分割数で生成し，各3次以下の曲線とNURBS曲面との交点を代数計算で算出する
	int CalcIntersecPtsPlaneU3(NURBSS *,Coord,Coord,int,Coord *,int);	// V方向のアイソパラ曲線を指定した分割数で生成し，各3次以下の曲線とNURBS曲面との交点を代数計算で算出する
	int CalcIntersecPtsPlaneV(NURBSS *,Coord,Coord,int,Coord *,int);	// V方向のアイソパラ曲線を指定した分割数で生成し，各曲線とNURBS曲面との交点を算出する
	int CalcIntersecPtsPlaneU(NURBSS *,Coord,Coord,int,Coord *,int);	// U方向のアイソパラ曲線を指定した分割数で生成し，各曲線とNURBS曲面との交点を算出する
	int CalcIntersecPtsPlaneSearch(NURBSS *,Coord,Coord,double,int,Coord *,int);	// NURBS曲面と平面との交点群を交線追跡法で求める
	int CalcIntersecPtsNurbsSNurbsC(NURBSS *,NURBSC *,int,Coord *,int);	// NURBS曲面とNURBS曲線との交点を求める(ニュートン法)
	int CalcIntersecPtsNurbsSGeom(NURBSS *,NURBSS *,int,int,Coord *,Coord *,int);		// NURBS曲面同士の交線上の点を幾何学的にいくつか求める
	int CalcIntersecPtsNurbsSSearch(NURBSS *,NURBSS *,int,double,Coord *,Coord *,int);	// NURBS曲面同士の交線(交点群)を交線追跡法で求める
	int CalcIntersecPtsNurbsCNurbsCParam(NURBSC *,NURBSC *,int,Coord *,int);	// 2DパラメトリックNURBS曲線同士の交点を求める
	int GetBSplCoef3(int,int,int,double *,double **);			// 3次のBスプライン曲線の各係数を求める　(at^3 + bt^2 + ct + dの係数a,b,c,dを返す)
	int GetBSplCoef2(int,int,int,double *,double **);			// 2次のBスプライン曲線の各係数を求める　(at^2 + bt + cの係数a,b,cを返す)
	int GetBSplCoef1(int,int,int,double *,double **);			// 1次のBスプライン曲線の各係数を求める　(at + bの係数a,bを返す)
	void ShiftNurbsS(NURBSS *,Coord);							// NURBS曲面のシフト
	void ShiftNurbsC(NURBSC *,Coord);							// NURBS曲線のシフト
	void ChRatioNurbsS(NURBSS *,Coord);							// NURBS曲面の倍率を変更する
	void ChRatioNurbsC(NURBSC *,Coord);							// NURBS曲線の倍率を変更する
	int SetCPNurbsS(NURBSS *,NURBSS);							// コントロールポイントを代入する
	int GenInterpolatedNurbsC1(NURBSC *,Coord *,int,int);		// 与えられた点列を補間するn階のNURBS曲線を生成する
	int GenInterpolatedNurbsC2(NURBSC *,Coord *,int,int);		// 与えられた点列を補間するn階のNURBS曲線を生成する(閉じた曲線)
	int GenApproximationNurbsC(NURBSC *,Coord *,int,int);		// 与えられた点列を近似するn階のNURBS曲線を生成する
	int GenNurbsCfromCP(NURBSC *,Coord *,int,int);				// コントロールポイントからNURBS曲線を生成する
	int GenPolygonalLine(NURBSC *,Coord *,int);					// 折れ線を生成する
	int GenInterpolatedNurbsS1(NURBSS *,Coord **,int,int,int,int);	// 与えられた点列を補間するn階NURBS曲面を生成する
	int GenPolygonalSurface(NURBSS *,Coord **,int,int);				// 折れ面を生成する
	int GenApproximationNurbsS(NURBSS *,Coord **,int,int,int,int);	// 与えられた点列を近似するn階のNURBS曲面を生成する
	int GenNurbsSfromCP(NURBSS *,Coord **,int,int,int,int);			// 与えられたコントロールポイントからn階のNURBS曲面を生成する
	int DetermPtOnTRMSurf(TRMS *,double,double);					// 注目中のNURBS曲面上の1点(u,v)がトリミング領域内にあるのかを判定する
	void RotNurbsS(NURBSS *,Coord,double);							// NURBS曲面を回転
	void RotNurbsC(NURBSC *,Coord,double);							// NURBS曲線を回転
	int DetectInterfereNurbsS(NURBSS *,NURBSS *,int);				// NURBS曲面(トリム無)同士の干渉検出
	int DetectInterfereTrmS(TRIMD_NURBSS *,TRIMD_NURBSS *,int);		// NURBS曲面(トリム有)同士の干渉検出
	int CalcIntersecPtsPlaneGeom(NURBSS *,Coord,Coord,int,int,Coord *,int);			// NURBS曲面と平面との交点群を幾何学的に求める
	double CalcNurbsCLength(NURBSC *);							// NURBS曲線の線分長を求める
	double CalcNurbsCLength(NURBSC *,double,double);			// NURBS曲線の指定区間の線分長を求める
	int CalcDeltaPtsOnNurbsC(NURBSC *,double,Coord *);			// 指定した間隔でNURBS曲線上の座標値を出力する
	int CalcExtremumNurbsC(NURBSC *,Coord,double *,int);		// NURBS曲線の指定した方向における極値の座標値を得る
	void GetEqIntervalKont(int,int,Vector);						// 曲線/曲面パラメータから等間隔なノットベクトルを算出

	int CalcExtSearchCurve(NURBSS *,Coord,Coord,double,NURBSC *,NURBSC *);			// 極地探索線を得る(準備中)
	int CalcExtGradCurve(NURBSS *,Coord,Coord,double,NURBSC *,NURBSC *);			// 極地傾斜線を得る(準備中)
	int TrimNurbsSPlane(TRMS *,Coord,Coord);					// NURBS曲面を平面でトリムする(準備中)

	int New_NurbsC(NURBSC *,int,int);			// NURBS曲線のメモリー確保
	void Free_NurbsC_1DArray(NURBSC *,int);		// NURBS曲線配列のメモリー解放
	void Free_NurbsC(NURBSC *);					// NURBS曲線のメモリー解放
	int New_NurbsS(NURBSS *,int [],int []);		// NURBS曲面のメモリー確保
	void Free_NurbsS_1DArray(NURBSS *,int);		// NURBS曲面配列のメモリー解放
	void Free_NurbsS(NURBSS *);					// NURBS曲面のメモリー解放
	int New_TrmS(TRMS *,int);					// トリム面のメモリー確保
	void Free_TrmS_1DArray(TRMS *,int);			// トリム面配列のメモリー解放
	void Free_TrmS(TRMS *);						// トリム面のメモリー解放
	int New_CompC(COMPC *,int);					// 複合曲線のメモリー確保
	void Free_CompC_1DArray(COMPC *,int);		// 複合曲線配列のメモリー解放
	void Free_CompC(COMPC *);					// 複合曲線のメモリー解放


private:
	int GetNurbsCCoef(NURBSC *,double **,int,Coord *,double *);	// NURBS曲線の係数を求める(最高3次)
	int CalcEquation(double *,double *,int);					// 3次方程式までを判別して解く
	void GetNurbsSCoef(int,double **,double *,Coord *,int,Coord *,double *);	// NURBS曲面においてuまたはvを固定した場合に得られるNURBS曲線C(u) or C(v)の分母分子の係数を求める
	void GetIntersecEquation(int,Coord *,double *,Coord,Coord,double *);		// NURBS曲線と平面の交線導出用3次方程式を得る
	int SearchIntersectPt(NURBSS *,Coord,Coord,double,double *,double *,int);	// ニュートン法により交点を収束させる(NURBS曲面と平面)
	int SearchIntersectPt_RKM(NURBSS *,Coord,Coord,double,double *,double *,int);	// 4次のルンゲクッタ法により交点を収束させる(NURBS曲面と平面)
	int SearchIntersectPt_BS(NURBSS *,Coord,Coord,double,double *,double *,int);	// Bulirsch-Stoer法により交点を収束させる(NURBS曲面と平面)
	int GetSIPParam1(NURBSS *,double ,double ,Coord ,Coord ,int ,Coord *);		// NURBS曲面と平面の交点を表す微分方程式の右辺の値を得る
	int SearchIntersectPt(NURBSS *,NURBSS *,double,double *,double *,double *,double *,int);		// ニュートン法により交点を収束させる(NURBS曲面同士)
	int DetermPtOnTRMSurf_sub(CONPS *,double,double);				// トリム境界線が複合曲線の場合のトリミング領域内外判定
	void GetCurveKnotParam1(Coord *,int,Vector);					// 各通過点の曲線パラメータを算出(コード長の比から算出)
	void GetCurveKnotParam2(Coord *,int,Vector);					// 各通過点の曲線パラメータを算出(コード長の平方根の比から算出)
	void GetSurfaceKnotParam(Vector,Vector,Coord **,int,int);		// 各通過点の曲面パラメータを算出
	void GetInterpolatedKnot(Vector,int,int,int,Vector);			// 曲線/曲面パラメータから補間用ノットベクトルを算出
	void GetApproximatedKnot(Vector,int,int,int,Vector);			// 曲線/曲面パラメータから近似用ノットベクトルを算出
	int SetApproximationCPnum(int);									// 点列数から生成するコントロールポイント数を算定する
	void CalcApproximationCP_LSM(Coord *,Vector,Vector,int,int,int,int,Coord *);	// 最小2乗法で近似コントロールポイントを求める
	int CheckTheSamePoints(Coord *,int);							// 同一点を除去する
	int CheckTheSamePoints(double *,int);							// 同一点を除去する
	int CheckTheSamePoints2D(Coord *,int);							// 2D平面内の同一点を除去する
	double CalcDiffNurbsSDenom(NURBSS *,int,int,double,double);		// NURBS曲面分母の各方向を任意階微分したときの微分係数を求める
	Coord CalcDiffNurbsSNumer(NURBSS *,int,int,double,double);		// NURBS曲面分子の各方向を任意階微分したときの微分係数を求める
	Coord TrimNurbsSPlaneSub1(double,double,double,double,double,double); // TrimNurbsSPlaneのサブ関数(2直線の交点をもとめる)

};

#endif
