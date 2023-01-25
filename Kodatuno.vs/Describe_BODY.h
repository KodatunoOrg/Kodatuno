// BODY描画用クラスを定義

#ifndef _DESCRIBE_BODY_H_
#define _DESCRIBE_BODY_H_

#include "BODY.h"
#include "NURBS_Func.h"

// Constants: General Defines
// COMMAND_DRAW_BOD					BODY描画用ディスプレイリストの登録番号(1)
// COMMAND_DRAW_USER				Userメイン関数によってコールされたOpenGL描画関数用ディスプレイリストの登録番号(2)
// COMMAND_DRAW_USER_COMMAND		User CommandによってコールされたOpenGL描画関数用ディスプレイリストの登録番号(100)
#define COMMAND_DRAW_BODY  1
#define COMMAND_DRAW_USER  2
#define COMMAND_DRAW_USER_COMMAND 100

// Class: Describe_BODY
// BODYエンティティを描画する関数を集めたクラス
class Describe_BODY
{
public:
	// Constructor: Describe_BODY
	// Describe_BODYクラスのコンストラクタ．NURBS描画ステータスの設定
	Describe_BODY();

	// Destructor: ~Describe_BODY
	// Describe_BODYクラスのデストラクタ．スケルトンです．
	~Describe_BODY();

	// Function: DrawBody
	// BODYを描画
	static void DrawBody(BODY *);					

	// Function: DrawCompositeCurve
	// 複合曲線を描画
	static void DrawCompositeCurve(COMPC *);		

	// Function: DrawLine
	// 直線を描画
//	static void DrawLine(LINE_);					
	static void DrawLine(const LINE_&);					

	// Function: DrawCircleArc
	// 円・円弧を描画
//	static void DrawCircleArc(CIRA);				
	static void DrawCircleArc(const CIRA&);				

	// Function: DrawNurbsCurve
	// NURBS曲線を描画
//	static void DrawNurbsCurve(NURBSC);				
	static void DrawNurbsCurve(const NURBSC&);				

	// Function: DrawTrimdNurbsSurfe
	// NURBS曲面の描画(トリムあり)
	static void DrawTrimdNurbsSurfe(NURBSS *);		

	// Function: DrawNurbsSurfe
	// NURBS曲面の描画(トリムなし)
//	static void DrawNurbsSurfe(NURBSS);				
	static void DrawNurbsSurfe(const NURBSS&);				

	// Function: DrawCurveOnParamSurfe
	// 面上線を描画
	static void DrawCurveOnParamSurfe(CONPS *);		

	// Function: DrawTrimdSurf
	// トリム面を描画
//	static void DrawTrimdSurf(TRMS);				
	static void DrawTrimdSurf(const TRMS&);				

	// Function: DrawNurbsCurve_Param
	// 2DパラメトリックNURBS曲線要素の描画
	static void DrawNurbsCurve_Param(NURBSC *);		

	// Function: Draw_Lines
	// BODYに含まれる線分を全て描画
	static void Draw_Lines(BODY *Body);				

	// Function: Draw_CircleArcs
	// BODYに含まれる円，円弧を全て描画
	static void Draw_CircleArcs(BODY *Body);		

	// Function: Draw_NurbsCurves
	// BODYに含まれるNURBS曲線を全て描画
	static void Draw_NurbsCurves(BODY *Body);		

	// Function: Draw_NurbsSurfaces
	// BODYに含まれるNURBS曲面を全て描画
	static void Draw_NurbsSurfaces(BODY *Body);		

	// Function: Draw_TrimSurfes
	// BODYに含まれるトリム面を全て描画
	static void Draw_TrimSurfes(BODY *Body);		

	// Function: 
	// NURBS描画時の描画ステータスを設定
	static void SetNurbsStat();						

	// Function: SetNurbsSTolerance
	// NURBS曲面の粗さを指定
	static void SetNurbsSTolerance(GLfloat);		

	// Function: NURBS_Err
	// NURBS描画時のエラーコールバック
	static void NURBS_Err(GLenum error_code);		

	// Function: SetNurbsSProperty
	// NURBS曲面の描画形式を変更する
	static void SetNurbsSProperty(GLenum,GLfloat);	

	// Function: DrawMesh
	// メッシュの描画
	static void DrawMesh(MESH *,int);				

	// 未実装
	static void DrawCircleArc();					// 円・円弧を描画
	static void DrawConicArc();						// 円錐曲線を描画
	static void DrawCircleArc_Param(CIRA *);		// 2Dパラメトリック円要素の描画
	static void DrawConicArc_Param(CONA *);			// 2Dパラメトリック円錐曲線要素の描画
	static void DrawLine_Param(LINE_ *);			// 2Dパラメトリック直線要素の描画


protected:
	// Variable:
	// NURBS曲面用オブジェクト
	static GLUnurbsObj *NurbsSurf;			

	// Variable:
	// NURBS曲線用オブジェクト
	static GLUnurbsObj *NurbsCurve;					
};

#endif
