// Kodatunoのインターフェース関係を定義

#ifndef _KODATUNO_H_
#define _KODATUNO_H_


#include "IGES_Parser.h"
#include "STL_Parser.h"
#include "StdAfx.h"
#include "Quaternion.h"
#include "Describe_BODY.h"
#include "UserFunc.h"
#include "Command.h"

#define EXT_IGES	1						// 拡張子判別用(IGES)
#define EXT_STL		2						// 拡張子判別用(STL)
#define CONSOLE_CHAR_NUMMAX	2048			// コンソールに入力できる文字数の最大値
#define MAXSELECT	20000					// セレクション時の最大バッファサイズ
#define BODYLISTMAX	8						// 読み込めるBODYの最大数
#define TOLERANCE 20						// NURBS曲面の粗さのデフォルト値
#define HISTORYNUMMAX 10					// コマンド履歴数

// NURBS曲面の生成方法を表すシンボル
#define ROTSURF	0							// 回転サーフェス		
#define SWEEPSURF 1							// スイープサーフェス


// KODatUNOクラスを定義
class KODatUNO : public BODY_Handler		// BODY関連関数を集めたクラスを継承
{
// public関数の定義
public:
	void InitializeWin();					// Kodatunoの初期化
	void DeleteWin();						// Kodatunoの終了処理
	int OpenFile();							// ファイルオープン
	int OpenFile(char *);					// ファイルオープン(オーバーロード)
	int OpenInpFile();						// インプットファイルオープン
	int BodyListSelect();					// ボディリスト選択
	void ReDraw(int,int);					// 再描画
	void ReDrawUserFunc();					// UserFunc実行時の再描画呼び出し
	void ReDrawUserCommand();				// UserCommand実行時の再描画呼び出し
	int GetConsoleCharNum();				// コンソール文字数を出力
	void SetConsoleCharNum(int);			// コンソール文字数を保存
	void ExecCommand(char *);				// コマンド実行
	int GetUpperConsoleHistory(WSCstring *);	// コマンド履歴を取得
	int GetLowerConsoleHistory(WSCstring *);	// コマンド履歴を取得
	void SetConsoleHistory(WSCstring);		// コマンド履歴を登録
	void GetResizeWindow();					// ウィンドウがリサイズされたときに、そのウインドウサイズを読み込む
	void MouseEvent(long,long,short,short);	// マウスイベントの処理
	void MouseMotionEvent(long,short,short);// マウス移動中のイベント処理
	void ChangeViewX();						// X軸上の視点に切り替える
	void ChangeViewY();						// Y軸上の視点に切り替える
	void ChangeViewZ();						// Z軸上の視点に切り替える
	void ChangeViewXYZ();					// 鳥瞰視点に切り替える
	WSCbool ChangeViewUp();					// 上矢印キーが押された場合のBODY回転
	WSCbool ChangeViewDown();					// 下矢印キーが押された場合のBODY回転
	WSCbool ChangeViewLeft();					// 左矢印キーが押された場合のBODY回転
	WSCbool ChangeViewRight();					// 右矢印キーが押された場合のBODY回転
	void FitView();							// 倍率をデフォルトに戻す
	void SolidView();						// Solid表示
	void NoEdgeView();						// Edgeを非表示にする
	void WireFlameView();					// WireFlame表示
	void VoidView();						// 表示しない
	void SelectAll();						// 全てセレクト状態にする
	void SelectionCancel();					// セレクションキャンセル
	void ViewBlending();					// BODYを半透明表示
	void DeleteBody();						// BODY消去
	void OpenDelBtn();						// Open，Deleteボタンの表示属性の変更
	void ExecuteUserFuncMain(int);			// UserFuncの実行
	void AcceptUserStat(int,double []);		// UserStatusの設定
	void UserViewCanncel();					// UserFuncによって描画された画像を消す
	void SetModelScale(double);				// モデルスケールをセット
	void GetModelScale();					// モデルスケール値を取得
	void SetTolerance(double);				// トレランス値をセット
	void GetTolerance();					// トレランス値を取得
	void GetShiftBody(Coord);				// BODYを平行移動
	void GetRotateBody(Coord,double);		// BODYを回転
	void ExpandBody(Coord);					// BODYを拡大
	int GenSurface(Coord,double,int);		// 各種サーフェスを生成
	int GenNurbsCurve(int,char *,int);		// 指定したファイルを読み込み,NURBS曲線を生成
	int GenNurbsSurface(int,char *,int);	// 指定したファイルを読み込み,NURBS曲面を生成
	void GetSurfInfo();						// 選択されている曲面の情報をコンソール出力する
	void ChangeRank(int []);				// 選択されている曲面のRankを変更する

// private関数の定義
private:
	void Describe_Main();				// BODY描画
	void DrawBODY();					// BODY描画のメイン部
	void DrawAxis(double,double);		// 軸の描画
	void DrawRubberband(double,double,double,double);	// ラバーバンドの描画
	void Draw_NurbsCurve(BODY *);		// NURBS曲線を描画
	void Draw_NurbsSurface(BODY *);		// NURBS曲線を描画
	void Draw_TrimSurfe(BODY *);		// トリム面を描画
	void UVWireView();					// UVパラメータで分割されたワイヤーフレームを表示
	void SetOpenGLStat();				// OpenGLの初期化
	void SetModelScale();				// 描画時のスケールを決定
	void SetMaxCoord();					// Bodyの最大長を設定する
	void DoSelect(int,int);				// セレクション(マウスピッキング)処理
	void ClearSeldEntList();			// セレクションリスト及びOBJECTの初期化
	void DeleteBodySub(BODY *,int);		// BODY消去SUB
	void ClickPicking(GLuint SelectBuf[],int hits);	// オブジェクトのマウスピッキング(クリック)
	void DragPicking(GLuint SelectBuf[],int hits);	// オブジェクトのマウスピッキング(ドラッグ)
	int  ObjSelect(GLuint [],int);					// オブジェクト選択判別
	int AddEntSymbolToObj();			// OBJECT構造体に何番目の曲線or曲面かの情報を付加する
	void InitCmdFlags();				// Cmd用フラグを全てKOD_FALSEに初期化
	void SetNewObject(int,int,int);		// ピックされたオブジェクトをOBJECTリストに登録
	void DescribeCP();						// コントロールポイントを描画


// pbulic変数の定義
public:
	int  UVWireFlameViewFlag;	// UVワイヤーフレーム表示フラグ
	int  CPViewFlag;			// コントロールポイント描画フラグ

// private変数の定義
private:
	USER User;					// ユーザークラス
	BODYList	BodyList;		// BODYリストの操作用インスタンス
	OBJECTList SeldEntList;		// セレクションされたエンティティを逐次格納していくリスト
	CommandMap CmdMap[COMMANDNUMMAX];		// コマンドマップ
	int	ConsoleCharNum;			// コンソールに入力されている全ての文字数を保持
	int Focus_Body;				// 選択中のBodyを表す値
	QUATERNION QFunc;			// クォータニオンのインスタンス
	double RotMx[TMATELEMNUM];	// クォータニオン→同時変換マトリックス
	Quat StartQ;				// 右クリックした瞬間の回転角を格納するクォータニオン
	Quat TargetQ;				// ぐりぐりやっている途中のクォータニオン
	double ModelScale;			// 現在のモデルスケール
	double ModelScale1st;		// モデルスケールの初期値
	double Tolerance;			// NURBSを表現する時の面の粗さ
	double Trl[3];				// 平行移動量
	int  Vp[4];					// ビューポイント
	double AxisScale;			// 軸描画のスケール
	int  OpenGLInitFlag;		// OpenGL初期設定済みフラグ
	int  ExecUserFuncFlag[USERFUNCNUMMAX];	// UserFunc描画フラグ
	int  ExecUserCommandFlag[USERCOMMANDNUM];	// UserCommand描画フラグ
	int  DoSelectFlag;			// セレクション実行フラグ
	int  SweepSelectFlag;		// スイープセレクションを開始したことを示すフラグ
	int  ScreenHeight;			// デフォルトスクリーンの高さ
	int  ScreenWidth;			// デフォルトスクリーンの幅
	int  StartX;				// ドラッグ開始位置X
	int  StartY;				// ドラッグ開始位置Y
	int  OldPosX;				// ウィンドウ再描画直前のX座標
	int  OldPosY;				// ウィンドウ再描画直前のY座標
	int  CurrentX;				// ドラッグ中の位置X
	int  CurrentY;				// ドラッグ中の位置Y
	int  LBtnFlag;				// 左クリックしたことを示すフラグ
	int  RBtnFlag;				// 右クリックしたことを示すフラグ
	int  MBtnFlag;				// 中クリックしたことを示すフラグ
	int  ShiftKeyFlag;			// Shiftキーを押したことを示すフラグ
	int  CtrlKeyFlag;			// Ctrlキーを押したことを示すフラグ
	int  AltKeyFlag;			// Altキーを押したことを示すフラグ
	int  SolidViewFlag;			// ソリッド表示フラグ
	int  EdgeViewFlag;			// エッジ表示フラグ
	int  WireFlameViewFlag;		// ワイヤーフレーム表示フラグ
	int  BlendViewFlag;			// 半透明表示フラグ
	int  DrawBODYFlag;			// BODY描画フラグ
	int  DrawSurfFlag;			// Surface描画フラグ
	int	 ReDrawBODYFlag;		// BODY描画1発目を示すフラグ
	GLuint SelectBuf[MAXSELECT];	// セレクトバッファ
	int argc;						// コマンド引数
	char *argv[MAXCOMMANDOPNUM];	// コマンド引数
	char CmdStack[HISTORYNUMMAX][256];	// コマンド履歴
	int CmdStackNum;				// コマンド履歴数
	int CmdStackNow;

};

// Kodatunoクラスのグローバル宣言
extern KODatUNO	Kodatuno;

// Kodatunoシステムコマンドを登録
void CmdVerInf(int,char *[]);			// コマンド(バージョン情報出力)
void CmdFileOpen(int,char *[]);			// コマンド(ファイルオープン)
void CmdUVWire(int,char *[]);			// コマンド(UVワイヤーフレーム表示)
void CmdChScale(int,char *[]);			// コマンド(スケール変更)
void CmdGenRotSurf(int,char *[]);		// コマンド(回転サーフェス生成)
void CmdGenSweepSurf(int,char *[]);		// コマンド(スイープサーフェス生成)
void CmdGenNurbsCurve(int,char *[]);	// コマンド(Nurbs曲線生成)
void CmdGenNurbsSurface(int,char *[]);	// コマンド(Nurbs曲面生成)
void CmdChTolerance(int,char *[]);		// コマンド(トレランス変更)
void CmdMoveBody(int,char *[]);			// コマンド(BODYの平行移動)
void CmdRotateBody(int ,char *[]);		// コマンド(BODYの回転)
void CmdCPView(int ,char *[]);			// コマンド(コントロールポイント描画)
void CmdSurfInfo(int,char *[]);			// コマンド(曲面情報を出力)
void CmdExpand(int,char *[]);			// コマンド(BODYの拡大)
void CmdChRank(int,char *[]);			// コマンド(NURBS Rnakの変更)
#endif
