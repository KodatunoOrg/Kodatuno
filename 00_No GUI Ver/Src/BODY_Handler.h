// BODYの各種操作クラス

#ifndef _BODY_HANDLER_H_
#define _BODY_HANDLER_H_

#include "NURBS_Func.h"
#include "Describe_BODY.h"


class BODY_Handler : public Describe_BODY
{
public:
	void InitCurveColor(float *);							// 線の色の初期値を与える
	void InitSurfaceColor(float *);							// 面の色の初期値を与える
	void ChangeStatColor(float *,float,float,float,float);	// エンティティのステータスで定義されている色を変更
	BODY *SearchBodyList(BODYList *,int);					// セレクションされているBODY番号を得る
	void Init_body(BODY *);									// BODY初期化
	void RegistNurbsCtoBody(BODYList *,BODY *,NURBSC,char []);			// 1つのNURBC曲線を新たなBODYとして登録する
	void RegistNurbsCtoBodyN(BODYList *,BODY *,NURBSC [],char [],int);	// N個のNURBC曲線を新たなBODYとして登録する
	void RegistNurbsStoBody(BODYList *,BODY *,NURBSS,char []);			// 1つのNURBS曲面を新たなBODYとして登録する
	void RegistNurbsStoBodyN(BODYList *,BODY *,NURBSS [],char [],int);	// N個のNURBS曲面を新たなBODYとして登録する
	void RotBody(BODY *,Coord,double);						// BODYの回転
	void ShiftBody(BODY *, Coord);							// BODYのシフト


	// 各エンティティのメモリー操作関数
	void New_body(BODY *,int []);				// 全エンティティのメモリー確保
	void Free_body(BODY *,int []);				// 全エンティティのメモリー解放
	
private:
	NURBS_Func NFunc;
};

#endif
