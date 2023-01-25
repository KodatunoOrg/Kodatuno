// UserSampleを記述
#include "SampleFunc.h"
#include <iostream>
#include <fstream>
#include <iomanip>

using namespace std;

// Function: SmpContourLine
// Sample6(等高線を生成する)
//
// Parameter:
// *BodyList - BODYの実体が登録順にリストされている
// *ObjList - セレクションされたエンティティ情報
// PickCount - セレクションされたエンティティの数
// Prop[] - ユーザー指定プロパティ値
//
// Return:
// 何も選択されていない：KOD_ERR,		正常終了：KOD_TRUE
int SmpContourLine(BODYList *BodyList,OBJECTList *ObjList, int PickCount, double Prop[])
{
	// NURBS曲面と平面との交線(交点群)をZ方向に順次求めていく
	// さらに，その交点群の法線ベクトルも算出する

	if(!PickCount)	return KOD_ERR;		// セレクションされていなかったら、何もしない

	NURBS_Func	nfunc;					// NURBSを扱う関数集を呼び出す

	OBJECT *obj = (OBJECT *)ObjList->getData(0);		// 一番最初にセレクションされたエンティティの情報を得る
	BODY *body = (BODY *)BodyList->getData(obj->Body);	// 一番最初にセレクションされたBODYの実体を得る
	if(obj->Type != _TRIMMED_SURFACE)	return KOD_ERR;	// セレクションされた曲面がトリム面でない場合は終了

	NURBSS *S = body->TrmS[obj->Num].pts;				// BODYからNURBS曲面を取り出す

	Coord t[5000];					// 解格用納
	double red[3] = {1,0,0};		// 法線ベクトル表示の色
	double blue[3] = {0,0,1};		// 点表示の色
	char mes[256];					// メッセージ出力用

	double feed = Prop[0];			// 交点群の点間隔
	double under = Prop[1];			// 等高線のZ_min
	double upper = Prop[2];			// 等高線のZ_max
	double delta = Prop[3];			// 等高線生成のZ間隔
	int step = fabs(upper - under)/delta + 1;	// 等高線の本数を算出

	Coord nvec = SetCoord(0,0,1);	// 平面の法線ベクトルを指定(X-Y平面とする)

	// 平面をZ方向にシフトしていきながら，等高線を算出する
	for(int i=0;i<step;i++){
		double z = under + delta*(double)i;	// 現在の平面のZ位置
		Coord pt = SetCoord(0,0,z);			// 現在の平面上の1点を指定

		sprintf(mes,"z=%.3lf  calculating...",z);
		GuiIF.SetMessage(mes);

		int num = nfunc.CalcIntersecPtsPlaneSearch(S,pt,nvec,feed,3,t,5000,RUNGE_KUTTA);		// NURBS曲面と平面との交点群を交線追跡法で求める

		for(int i=0;i<num;i++){		// 交点の数だけループ
			Coord p = nfunc.CalcNurbsSCoord(S,t[i].x,t[i].y);			// 交点をパラメータ値から座標値へ変換
			Coord nt = nfunc.CalcNormVecOnNurbsS(S,t[i].x,t[i].y);		// 交点上の法線ベクトルを計算
			nt = MulCoord(nt,-2);											// 外向き法線ベクトルへ変換し適当な長さにする
			DrawPoint(p,1,3,blue);			// 交点を描画
			DrawVector(p,nt,1,1,red);		// 法線ベクトルを描画
		}
	}
	GuiIF.SetMessage("END");


	return KOD_TRUE;
}

// Function: SmpIntersectSurfs
// Sample7(曲面同士の交線を算出する)
//
// Parameter:
// *BodyList - BODYの実体が登録順にリストされている
// *ObjList - セレクションされたエンティティ情報
// PickCount - セレクションされたエンティティの数
// Prop[] - ユーザー指定プロパティ値
//
// Return:
// 何も選択されていない：KOD_ERR,		セレクションされた2つのエンティティがトリム面でない：KOD_ERR,		正常終了：KOD_TRUE
int SmpIntersectSurfs(BODYList *BodyList,OBJECTList *ObjList, int PickCount, double Prop[])
{
	if(!PickCount)	return KOD_ERR;		// セレクションされていなかったら、何もしない

	NURBS_Func nfunc;				// NURBS関連の関数を集めたクラスのオブジェクトを生成
	Coord Rt[5000];					// NURBS曲面R(w,t)における解
	Coord St[5000];					// NURBS曲面S(u,v)における解
	double green[3] = {0,1,0};		// 点表示の色

	double feed = Prop[0];			// 交点群の点間隔

    GuiIF.SetMessage("Start calcu....");

	// 1番目にセレクションされたBODYデータを得る
	OBJECT *obj1 = (OBJECT *)ObjList->getData(0);
	BODY *body1 = (BODY *)BodyList->getData(obj1->Body);

	// 2番目にセレクションされたBODYデータを得る
	OBJECT *obj2 = (OBJECT *)ObjList->getData(1);
	BODY *body2 = (BODY *)BodyList->getData(obj2->Body);

	if(obj1->Type != _TRIMMED_SURFACE || obj2->Type != _TRIMMED_SURFACE)	// セレクションされた2つのエンティティがトリム面でない場合は終了
		return KOD_ERR;

	NURBSS *S1 = body1->TrmS[obj1->Num].pts;	// BODY1からNURBS曲面を取り出す
	NURBSS *S2 = body2->TrmS[obj2->Num].pts;	// BODY2からNURBS曲面を取り出す

	int num = nfunc.CalcIntersecPtsNurbsSSearch(S1,S2,10,feed,Rt,St,5000);	// NURBS曲面同士の交線算出

	for(int i=0;i<num;i++){
		Coord p = nfunc.CalcNurbsSCoord(S1,Rt[i].x,Rt[i].y);	// uv座標上の点をxyz座標値に変換
		DrawPoint(p,1,3,green);									// 描画
	}

    GuiIF.SetMessage("END");
	return KOD_TRUE;
}

// Function: SmpMeanCurvature
// Sample8(NURBS曲面の適当な(u,v)における平均曲率法線ベクトルを算出する)
//
// Parameter:
// *BodyList - BODYの実体が登録順にリストされている
// *ObjList - セレクションされたエンティティ情報
// PickCount - セレクションされたエンティティの数
// Prop[] - ユーザー指定プロパティ値
//
// Return:
// 何も選択されていない：KOD_ERR,		セレクションされた2つのエンティティがトリム面でない：KOD_ERR,		正常終了：KOD_TRUE
int SmpMeanCurvature(BODYList *BodyList,OBJECTList *ObjList, int PickCount, double Prop[])
{
	if(!PickCount)	return KOD_ERR;		// セレクションされていなかったら、何もしない

	NURBS_Func nfunc;
	double blue[3] = {0,0,1};

	OBJECT *obj = (OBJECT *)ObjList->getData(0);		// 一番最初にセレクションされたエンティティの情報を得る
	BODY *body = (BODY *)BodyList->getData(obj->Body);	// 一番最初にセレクションされたBODYの実体を得る
	if(obj->Type != _TRIMMED_SURFACE)	return KOD_ERR;	// セレクションされた曲面がトリム面でない場合は終了

	NURBSS *S = body->TrmS[obj->Num].pts;				// BODYからNURBS曲面を取り出す

	int udiv = (int)Prop[0];		// u方向分割数
	int vdiv = (int)Prop[1];		// v方向分割数

	for(int i=0;i<=udiv;i++){
		double u = S->U[0]+(double)i*(S->U[1] - S->U[0])/(udiv+1);
		for(int j=0;j<=vdiv;j++){
			double v = S->V[0]+(double)j*(S->V[1] - S->V[0])/(vdiv+1);
			Coord P = nfunc.CalcNurbsSCoord(S,u,v);				// 現在の(u,v)上の(x,y,z)を求める
			Coord Hn = nfunc.CalcMeanCurvatureNormVec(S,u,v);	// 平均曲率法線ベクトルを求める
			DrawVector(P,Hn,100,1,blue);						// 平均曲率法線ベクトルを表示
		}
	}

	return KOD_TRUE;
}
