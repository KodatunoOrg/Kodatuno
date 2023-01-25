#ifndef _BODY_DESCRIBE_H_
#define _BODY_DESCRIBE_H_

#include "BODY.h"
#include "Describe_BODY.h"
#include "IGES_Parser.h"
#include "STL_Parser.h"
#include "VRML_Parser.h"
#include "Quaternion.h"
#include "NURBS_Func.h"
#include "UserFunc.h"
#include "StdAfx.h"

#define EXT_IGES	1						// 拡張子判別用(IGES)
#define EXT_STL		2						// 拡張子判別用(STL)
#define EXT_VRML	3						// 拡張子判別用(VRML)
#define CIRCLE_DIV_NUM 100					// 円描画時の円分割数
#define MAXSELECT	20000					// セレクション時の最大バッファサイズ
#define TOLERANCE   20						// NURBS曲面の粗さのデフォルト値

// メニュー用enum
enum Menu{
	M_Version,
	M_Draw_Body,
	M_Draw_Surf,
	M_UnSelect,
	M_View_Angle,
	M_UserFunc_Canncel
};

enum Sub_Menu_View{
	S_M_XView,
	S_M_YView,
	S_M_ZView
};

// IGES描画クラスを定義
class DESCRIBE : public Describe_BODY			// BODY描画関数を集めたクラスを継承
{
public:
	DESCRIBE();		// コンストラクタ
	~DESCRIBE();	// デストラクタ
	int BODY_Describe_Main(int,char **);		// 描画main
	
// 関数
private:
	// OpenGL用描画関数の宣言はstaticが必要
	static void Keyboard(unsigned char key, int x, int y);	// キーボードイベント
	static void Display(void);								// 描画イベント
	static void Motion(int,int);							// マウス動作イベント
	static void Reshape(int,int);							// 再描画イベント
	static void Mouse(int,int,int,int);						// マウスイベント
	static void Menu(int);									// メニュー処理イベント
	static void Sub_Menu_View(int);							// サブメニュー（視点切換）処理イベント
	static void Sub_Menu_User(int);							// サブメニュー（User登録関数）処理イベント
	static void DoSelect(int,int);							// セレクションの設定
	static int  ObjSelect(GLuint [],int);					// オブジェクト選択判別
	static void Init(char *progname);						// 描画初期化
	static void Idle();										// 描画アイドリング状態での処理イベント
	static void Special(int,int,int);						// 特殊キーイベント
	static void glDrawBODY();								// IGESデータの描画
	static void glDrawAxis(double,double);					// 軸の描画
	static void glDrawRubberband(double,double,double,double);	// ラバーバンドの描画
	static void glDraw_Line();								// 直線を描画
	static void Draw_NurbsSurface(BODY *);					// NURBS曲線を描画
	static void Draw_NurbsCurve(BODY *);					// NURBS曲線を描画
	static void glDraw_SurfLine();							// NURBS曲面のパラメータラインを描画
	static void Draw_TrimSurfe(BODY *);						// トリム面の描画
	static void Draw_Mesh(BODY *);							// メッシュを描画
	static int GenBodyList();								// bodyを読み込みリストに登録
	static int NewPath();									// SPath,CPathのメモリーをセレクトされたエンティティ分確保する
	static int AddEntSymbolToObj();							// OBJECT構造体に何番目の曲線or曲面かの情報を付加する
	static int DelPath();									// SPath,CPathのメモリー解放
	static void UpDateDisplayList(int);						// ディスプレイリストの更新
	static void ClickPicking(GLuint SelectBuf[],int hits);	// オブジェクトのマウスピッキング(クリック)
	static void DragPicking(GLuint SelectBuf[],int hits);	// オブジェクトのマウスピッキング(ドラッグ)
	static int OpenFile();									// CADデータ読み込み
	static int CheckExtention(char *);						// 読み込まれたデータの拡張子をチェック
	static void ReDrawUserFunc();							// UserFunc実行時の再描画呼び出し
	static void SetNewObject(int,int,int);					// ピックされたオブジェクトをOBJECTリストに登録
	static void SetModelScale();							// モデルスケールをセット
	static void ClearSeldEntList();							// セレクションリスト及びOBJECTの初期化
	static void UserViewCanncel();							// UserFuncによって描画された画像を消す
	static void SelectionCanncel();							// セレクションをキャンセルする
	static void VersionInfo();								// バージョン情報出力
	static BODY *SearchBodyList(BODYList *,int);			// セレクションされているBODY番号を得る

// 変数
private:
	static BODY *glbody;			// 読み込んだBODYリストの先頭ポインタ
	static BODY *Curr_body;			// BODYリストにおける注目中のBODYオブジェクトを指す
	static int  ScreenHeight;		// デフォルトスクリーンの高さ
	static int  ScreenWidth;		// デフォルトスクリーンの幅
	static int  StartX;				// ドラッグ開始位置X
	static int  StartY;				// ドラッグ開始位置Y
	static int  OldPosX;			// ウィンドウ再描画直前のX座標
	static int  OldPosY;			// ウィンドウ再描画直前のY座標
	static int  CurrentX;			// ドラッグ中の位置X
	static int  CurrentY;			// ドラッグ中の位置Y
	static int  SweepSelectFlag;	// スイープセレクションを開始したことを示すフラグ
	static int  Vp[4];				// ビューポイント
	static GLuint  SelectBuf[MAXSELECT];	// セレクトバッファ
	static int  LBtnFlag;			// 左クリックしたことを示すフラグ
	static int  RBtnFlag;			// 右クリックしたことを示すフラグ
	static int  MBtnFlag;			// 中クリックしたことを示すフラグ
	static int  ShiftKeyFlag;		// Shiftキーを押したことを示すフラグ
	static int  CtrlKeyFlag;		// Ctrlキーを押したことを示すフラグ
	static int  AltKeyFlag;			// Altキーを押したことを示すフラグ
	static double Trl[3];			// 平行移動量
	static double AxisScale;		// 軸描画のスケール
	static double ModelScale;		// モデルのスケール
	static double ModelScale1st;	// モデルスケールの初期値
	static double IdentX;			// マウスの絶対位置X→ウィンドウ内での相対位置の換算係数X
	static double IdentY;			// マウスの絶対位置Y→ウィンドウ内での相対位置の換算係数Y
	static Quat StartQ;				// 右クリックした瞬間の回転角を格納するクォータニオン
	static Quat TargetQ;			// ぐりぐりやっている途中のクォータニオン
	static double RotMx[TMATELEMNUM];	// クォータニオン→同時変換マトリックス
	static int  DrawSurfFlagCount;	// Surface描画フラグメニューが何回選択されたかを数えるカウンタ
	static int  DrawBODYFlagCount;	// BODY描画フラグメニューが何回選択されたかを数えるカウンタ
	static int DrawUserFuncFlag;			// PDT描画フラグ
	static OBJECT SelectedEntList[MAXSELECT];	// セレクションされたエンティティを逐次格納していく
	static int PickCount;			// ピック回数をカウント
	static int BodyCount;			// bodyの数をカウント
	static int CPickCount;			// NURBS曲線がピックされた数をカウント
	static int SPickCount;			// NURBS曲面がピックされた数をカウント
	static int  DrawBODYFlag;			// BODY描画フラグ
	static int  DrawSurfFlag;			// Surface描画フラグ
	static int  DrawBODY1st;
	static int  ExecUserFuncFlag[USERFUNCNUMMAX];	// UserFunc描画フラグ
	static int	 ReDrawBODYFlag;		// BODY描画1発目を示すフラグ
	static double Tolerance;			// NURBSを表現する時の面の粗さ



private:
	static BODYList	BodyList;		// BODYリストの操作用インスタンス
	static OBJECTList SeldEntList;	// セレクションされたエンティティを逐次格納していくリスト
	static IGES_PARSER Parser;		// IGESパーサー用オブジェクトを生成
	static QUATERNION QFunc;		// クォータニオンクラスのオブジェクトを生成
	static NURBS_Func NFunc;		// NURBS_Funcクラスのオブジェクトを生成
	static USER User;				// ユーザークラスのオブジェクトを生成
};


#endif
