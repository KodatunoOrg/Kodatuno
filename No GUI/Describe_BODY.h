// BODY描画用クラスを定義

#ifndef _DESCRIBE_BODY_H_
#define _DESCRIBE_BODY_H_

#include "BODY.h"
#include "NURBS_Func.h"

#define COMMAND_DRAW_BODY  1			// BODY描画用ディスプレイリストの登録番号
#define COMMAND_DRAW_USER  2			// Userメイン関数によってコールされたOpenGL描画関数用ディスプレイリストの登録番号
#define COMMAND_DRAW_USER_COMMAND 100	// User CommandによってコールされたOpenGL描画関数用ディスプレイリストの登録番号


// BODYエンティティを描画する関数を集めたクラス
class Describe_BODY
{
public:
	Describe_BODY();
	~Describe_BODY();
	static void DrawBody(BODY *);						// BODYを描画
	static void DrawCompositeCurve(COMPC *);			// 複合曲線を描画
	static void DrawLine(LINE_);						// 直線を描画
	static void DrawNurbsCurve(NURBSC);				// NURBS曲線を描画
	static void DrawTrimdNurbsSurfe(NURBSS *);			// NURBS曲面の描画(トリムあり)
	static void DrawNurbsSurfe(NURBSS);				// NURBS曲面の描画(トリムなし)
	static void DrawCurveOnParamSurfe(CONPS *);		// 面上線を描画
	static void DrawTrimdSurf(TRMS);					// トリム面を描画
	static void DrawNurbsCurve_Param(NURBSC *);		// 2DパラメトリックNURBS曲線要素の描画
	static void Draw_NurbsCurves(BODY *Body);			// BODYに含まれるNURBS曲線を全て描画
	static void Draw_NurbsSurfaces(BODY *Body);		// BODYに含まれるNURBS曲面を全て描画
	static void Draw_TrimSurfes(BODY *Body);			// BODYに含まれるトリム面を全て描画
	static void SetNurbsStat();						// NURBS描画時の描画ステータスを設定
	static void SetNurbsSTolerance(GLfloat);			// NURBS曲面の粗さを指定
	static void NURBS_Err(GLenum error_code);	// NURBS描画時のエラーコールバック
	static void SetNurbsSProperty(GLenum,GLfloat);		// NURBS曲面の描画形式を変更する

	static void DrawMesh(MESH *,int);					// メッシュの描画

// 未実装
	static void DrawCircleArc();						// 円・円弧を描画
	static void DrawConicArc();						// 円錐曲線を描画
	static void DrawCircleArc_Param(CIRA *);			// 2Dパラメトリック円要素の描画
	static void DrawConicArc_Param(CONA *);			// 2Dパラメトリック円錐曲線要素の描画
	static void DrawLine_Param(LINE_ *);				// 2Dパラメトリック直線要素の描画


protected:
	static GLUnurbsObj *NurbsSurf;						// NURBS曲面用オブジェクト
	static GLUnurbsObj *NurbsCurve;					// NURBS曲線用オブジェクト
};

#endif
