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
	void DrawCompositeCurve(COMPC *);			// 複合曲線を描画
	void DrawLine(LINE_);						// 直線を描画
	void DrawNurbsCurve(NURBSC);				// NURBS曲線を描画
	void DrawTrimdNurbsSurfe(NURBSS *);			// NURBS曲面の描画(トリムあり)
	void DrawNurbsSurfe(NURBSS);				// NURBS曲面の描画(トリムなし)
	void DrawCurveOnParamSurfe(CONPS *);		// 面上線を描画
	void DrawTrimdSurf(TRMS);					// トリム面を描画
	void DrawNurbsCurve_Param(NURBSC *);		// 2DパラメトリックNURBS曲線要素の描画
	void CheckDegenracy(BODY *);				// 縮退(2Dパラメトリック曲線の始点と終点が一致しているか)のチェック
	void SetNurbsStat();						// NURBS描画時の描画ステータスを設定
	void SetNurbsSTolerance(GLfloat);			// NURBS曲面の粗さを指定
	static void NURBS_Err(GLenum error_code);	// NURBS描画時のエラーコールバック
	void SetNurbsSProperty(GLenum,GLfloat);		// NURBS曲面の描画形式を変更する

// 未実装
	void DrawCircleArc();						// 円・円弧を描画
	void DrawConicArc();						// 円錐曲線を描画
	void DrawCircleArc_Param(CIRA *);			// 2Dパラメトリック円要素の描画
	void DrawConicArc_Param(CONA *);			// 2Dパラメトリック円錐曲線要素の描画
	void DrawLine_Param(LINE_ *);				// 2Dパラメトリック直線要素の描画


protected:
	GLUnurbsObj *NurbsSurf;						// NURBS曲面用オブジェクト
	GLUnurbsObj *NurbsCurve;					// NURBS曲線用オブジェクト
	GLfloat	uKnot[KNOTNUMMAX];					// NURBS描画用バッファ
	GLfloat	vKnot[KNOTNUMMAX];					// NURBS描画用バッファ
	GLfloat	CCtlp[CTLPNUMMAX][4];				// NURBS描画用バッファ
	GLfloat	SCtlp[CTLPNUMMAX][CTLPNUMMAX][4];	// NURBS描画用バッファ

};

#endif
