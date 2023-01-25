#ifndef _NURBS_FUNC_H_
#define _NURBS_FUNC_H_

#include "BODY.h"
#include "Describe_BODY.h"

#define INTERSECPTNUMMAX	1000			// 交点格納配列長
#define NEAREST_GAP			0.01			// 2点が同一点とみなす距離
#define CONVERG_GAP			0.000001		// ニュートン法の収束を判別する閾値
#define LOOPCOUNTMAX		100				// 収束計算回数の条件

// NURBSの操作を集めたクラスを定義
class NURBS_Func
{
public:
	Coord CalcNurbsCCoord(NURBSC *,double);						// 指定したノットtでのNURBS曲線の座標値を求める
	Coord CalcNurbsSCoord(NURBSS *,double,double);				// 指定したノットu,vでのNURBS曲面の座標点を求める
	Coord CalcDiffuNurbsS(NURBSS *,double,double);				// 指定したノットu,vでのNURBS曲線のu方向1階微分係数を求める
	Coord CalcDiffvNurbsS(NURBSS *,double,double);				// NURBS曲線のv方向1階微分係数を求める
	int GenNurbsC(NURBSC *,int,int,int,double [],double [],Coord []);	// 1つのNURBS曲線を生成する
	int GenNurbsC(NURBSC *,NURBSC);								// 1つのNURBS曲線を生成する(NURBS曲線のコピー)(オーバーロード)
	void DelNurbsC(NURBSC *);									// NURBS曲線を削除する
	int GenNurbsS(NURBSS *,NURBSS);								// 1つのNURBS曲面を生成する(NURBS曲面のコピー)
	void DelNurbsS(NURBSS *);									// NURBS曲面を削除する
	int GenTrimdNurbsS(TRIMD_NURBSS *,TRIMD_NURBSS);			// トリム面を生成する
	int DelTrimdNurbsS(TRIMD_NURBSS *);							// トリム面を削除(メモリー解放)する
	double CalcBSbasis(double,double [],int,int);				// Bスプライン基底関数を計算し、計算結果を返す
	double CalcDiffBSbasis(double,double [],int,int);			// Bスプライン基底関数の1階微分係数を求める
	Coord CalcDiffNurbsC(NURBSC *,double);						// NURBS曲線の1階微分係数を求める
	Coord CalcNormVecOnNurbsS(NURBSS *,double,double);			// NURBS曲面上の(u,v)における法線ベクトルをもとめる
	int CalcIntersecCurve3(NURBSC *,Coord,Coord,double *,int);	// 3次以下のNURBS曲線と平面との交点を求める
	int CalcIntersecPtsPlaneV3(NURBSS *,Coord,Coord,int,Coord *,int);	// 3次以下のNURBS曲面と平面との交点群を代数計算で求める(vパラメータ分割)
	int CalcIntersecPtsPlaneU3(NURBSS *,Coord,Coord,int,Coord *,int);	// 3次以下のNURBS曲面と平面との交点群を代数計算で求める(uパラメータ分割)
	int CalcIntersecPtsPlaneSearch3(NURBSS *,Coord,Coord,double,Coord *,int);	// 3次以下のNURBS曲面と平面との交点群を交線追跡法で求める
	int GetBSplCoef3(int,int,int,double *,double **);			// 3次のBスプライン曲線の各係数を求める　(at^3 + bt^2 + ct + dの係数a,b,c,dを返す)
	int GetBSplCoef2(int,int,int,double *,double **);			// 2次のBスプライン曲線の各係数を求める　(at^2 + bt + cの係数a,b,cを返す)
	int GetBSplCoef1(int,int,int,double *,double **);			// 1次のBスプライン曲線の各係数を求める　(at + bの係数a,bを返す)
	void ShiftNurbsS(NURBSS *,Coord);							// NURBS曲面のシフト
	void ShiftNurbsC(NURBSC *,Coord);							// NURBS曲線のシフト
	void ChRatioNurbsS(NURBSS *,Coord);							// NURBS曲面の倍率を変更する
	void ChRatioNurbsC(NURBSC *,Coord);							// NURBS曲線の倍率を変更する
	int SetCPNurbsS(NURBSS *,NURBSS);							// コントロールポイントを代入する

private:
	int GetNurbsCCoef(NURBSC *,double **,int,Coord *,double *);	// NURBS曲線の係数を求める(最高3次)
	int CalcEquation(double *,double *,int);					// 3次方程式までを判別して解く
	void GetNurbsSCoef(int,double **,double *,Coord *,int,Coord *,double *);	// NURBS曲面においてuまたはvを固定した場合に得られるNURBS曲線C(u) or C(v)の分母分子の係数を求める
	void GetIntersecEquation(int,Coord *,double *,Coord,Coord,double *);		// NURBS曲線と平面の交線導出用3次方程式を得る
};

#endif
