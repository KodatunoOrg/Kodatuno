#ifndef _DESCRIBE_BODY_H_
#define _DESCRIBE_BODY_H_

#include "BODY.h"
#include "NURBS_Func.h"

#define COMMAND_DRAW_USER 2	// Userメイン関数によってコールされたOpenGL描画関数用ディスプレイリストの登録番号
#define COMMAND_DRAW_BODY 1	// BODY描画用ディスプレイリストの登録番号

// BODYエンティティを描画する関数を集めたクラス
class Describe_BODY
{
public:
	Describe_BODY();
	~Describe_BODY();
	static void DrawCircleArc();					// 円・円弧を描画
	static void DrawCompositeCurve(COMPC *);		// 複合曲線を描画
	static void DrawConicArc();						// 円錐曲線を描画
	static void DrawLine(LINE_);					// 直線を描画
	static void DrawNurbsCurve(NURBSC);				// NURBS曲線を描画
	static void DrawTrimdNurbsSurfe(NURBSS *);		// NURBS曲面の描画(トリムあり)
	static void DrawNurbsSurfe(NURBSS);				// NURBS曲面の描画(トリムなし)
	static void DrawCurveOnParamSurfe(CONPS *);		// 面上線を描画
	static void DrawTrimdSurf(TRMS);				// トリム面を描画
	static void DrawCircleArc_Param(CIRA *);		// 2Dパラメトリック円要素の描画
	static void DrawConicArc_Param(CONA *);			// 2Dパラメトリック円錐曲線要素の描画
	static void DrawLine_Param(LINE_ *);			// 2Dパラメトリック直線要素の描画
	static void DrawNurbsCurve_Param(NURBSC *);		// 2DパラメトリックNURBS曲線要素の描画
	static void CheckDegenracy(BODY *);				// 縮退(2Dパラメトリック曲線の始点と終点が一致しているか)のチェック
	static void SetNurbsStat();						// NURBS描画時の描画ステータスを設定
	static void NURBS_Err(GLenum error_code);		// NURBS描画時のエラーコールバック

protected:
	static GLUnurbsObj *NurbsSurf;					// NURBS曲面用オブジェクト
	static GLUnurbsObj *NurbsCurve;					// NURBS曲線用オブジェクト
	static GLfloat	uKnot[KNOTNUMMAX];				// NURBS描画用バッファ
	static GLfloat	vKnot[KNOTNUMMAX];				// NURBS描画用バッファ
	static GLfloat	CCtlp[CTLPNUMMAX][4];			// NURBS描画用バッファ
	static GLfloat	SCtlp[CTLPNUMMAX][CTLPNUMMAX][4];	// NURBS描画用バッファ
	static int  DrawBODYFlag;						// BODY描画フラグ
	static int  DrawSurfFlag;						// Surface描画フラグ
	static int	DrawBODY1st;						// BODY描画1発目を示すフラグ

};

#endif