// Kodatunoのインターフェース関係を定義

#ifndef _KODATUNO_H_
#define _KODATUNO_H_


#include "IGES_Parser.h"
#include "STL_Parser.h"
#include "VRML_Parser.h"
#include "DXF_Parser.h"
#include "StdAfx.h"
#include "Quaternion.h"
#include "Describe_BODY.h"
#include "UserFunc.h"
#include "Command.h"

// Constants : General Defines
// KODATUNO_MAJOR_VER - 			kodatunoのメジャーバージョン(3)
// KODATUNO_MINOR_VER - 			kodatunoのマイナーバージョン(0)
// EXT_IGES -						拡張子判別用(IGES)(1)
// EXT_STL -						拡張子判別用(STL)(2)
// CONSOLE_CHAR_NUMMAX -			コンソールに出力可能な文字数(2048)
// MAXSELECT -						セレクション時の最大バッファサイズ(20000)
// BODYLISTMAX -					読み込めるBODYの最大数(8)
// TOLERANCE -						NURBS曲面の粗さのデフォルト値(20)
// HISTORYNUMMAX -					コマンド履歴の最大数(10)
// KDT_MOUSE_PRESS -				マウスボタンプレス(1000)
// KDT_MOUSE_RELEASE -				マウスボタンリリース(1001)
// KDT_MOUSE_LEFT -					マウス左ボタン(1002)
// KDT_MOUSE_MIDDLE -				マウス中ボタン(1003)
// KDT_MOUSE_RIGHT -				マウス右ボタン(1004)
// KDT_MOUSE_NONE -					どのボタンでもない(1005)
// KDT_MOD_CTRL -					Ctrl修飾(2001)
// KDT_MOD_SHIFT -					Shift修飾(2001)
// KDT_MOD_ALT -					Alt修飾(2002)
// KDT_MOD_NONE -					どの修飾もない(2003)
#define KODATUNO_MAJOR_VER  3  				
#define KODATUNO_MINOR_VER  0  				

#define EXT_IGES	1						
#define EXT_STL		2						
#define CONSOLE_CHAR_NUMMAX	2048			
#define MAXSELECT	20000					
#define BODYLISTMAX	8						
#define TOLERANCE 20						
#define HISTORYNUMMAX 10					

#define KDT_MOUSE_PRESS		1000	
#define KDT_MOUSE_RELEASE   1001	
#define KDT_MOUSE_LEFT      1002	
#define KDT_MOUSE_MIDDLE    1003	
#define KDT_MOUSE_RIGHT		1004	
#define KDT_MOUSE_NONE		1005	

#define KDT_MOD_CTRL	2001	
#define KDT_MOD_SHIFT	2001	
#define KDT_MOD_ALT		2002	
#define KDT_MOD_NONE	2003	

// Constants: General Defines
// ROTSURF							回転サーフェス(0)
// SWEEPSURF						スイープサーフェス(1)
#define ROTSURF	0							
#define SWEEPSURF 1							


// Class: KODatUNO
// BODY関連関数を集めたクラスを継承
class KODatUNO : public Describe_BODY		
{
public:
// Function: InitializeWin
// ユーザーが独自に作成した関数とUserボタンとの関連付け及び、Userステータスの初期化を行う
    void InitializeWin(int, int);			

// Function: DeleteWin
// Kodatunoの終了処理
	void DeleteWin();						

// Function: OpenFile
// 拡張子IGSまたはigsのファイルを読み込むダイアログボックスを表示し、IGSファイルを読み込む．
// 読み込まれた立体情報はBODYに登録され、BODY描画フラグを立てる．
// また、読み込んだNURBSデータに縮退がある場合は、その補間もここで行われる
	int OpenFile();							

// Function: OpenFile
// ファイルオープン(オーバーロード)
	int OpenFile(char *);					

// Function: OpenInpFile
// 拡張子INPまたはinpを持つインプットファイルのフルパス名を取得する
    std::string OpenInpFile();              

// Function: BodyListSelect
// BODYリストボックスにあるBODY名が選択された場合に、そのBODYの番号を得る
	int BodyListSelect();					

// Fuction: ReDraw
// OpenGLによるBODY描画を行う．再描画イベントが発生した時に呼ばれる．
// (注意)OpenGLが用意するAPIはこの関数にぶら下がっていなければならない
	void ReDraw(int,int);					

// Function: ReDrawUserFunc
// User関数によって実行されたOpenGLによる描画はこの関数で行う．
// (注意)OpenGLが用意するAPIはこの関数にぶら下がっていなければならない
	void ReDrawUserFunc();					

// Function: ReDrawUserCommand
// UserCommand実行時の再描画呼び出し
	void ReDrawUserCommand();				

// Function: GetConsoleCharNum
// コンソールの現在の文字数を得る
	int GetConsoleCharNum();				
	
// Function: SetConsoleCharNum
// コンソール文字数を保存する
	void SetConsoleCharNum(int);			

// Function: ExecCommand
// コンソールから入力されたコマンドを解析し実行する
	void ExecCommand(char *);				
	
// GetUpperConsoleHistory
// コンソール内で上矢印キーが押された場合、コマンド履歴を矢印に沿って取得していく
    int GetUpperConsoleHistory(std::string *);	
	
// GetLowerConsoleHistory
// コンソール内で下矢印キーが押された場合、コマンド履歴を矢印に沿って取得していく
    int GetLowerConsoleHistory(std::string *);	
	
// Function: SetConsoleHistory
// コマンド履歴を格納している配列に新たなコマンドを履歴として登録する
    void SetConsoleHistory(std::string);		

// Function: GetonsoleHistoryNum
// コマンド履歴数を取得 
    int GetConsoleHistoryNum();             	

// Function: GetResizeWindow
// ウィンドウがリサイズされたときに、そのウインドウサイズを読み込む
    void GetResizeWindow(int, int);					
	
// Function: MouseEvent
// マウスイベントが発生した場合に、押されたボタンに応じて処理を行う
	void MouseEvent(long,long,short,short);	
	
// Function: MouseMotionEvent
// マウス移動中のイベント処理
    void MouseMotionEvent(long,long,short,short);
	
// Function: MouseWheelEvent
// マウスホイールの処理
    void MouseWheelEvent(long);				
	
// Function: ChangeViewX
// X軸上の視点に切り替える
    void ChangeViewX();						

// Function: ChangeViewY
// Y軸上の視点に切り替える
	void ChangeViewY();						
	
// Function: ChangeViewZ
// Z軸上の視点に切り替える
	void ChangeViewZ();						
	
// Function: ChangeViewXYZ
// 鳥瞰視点に切り替える
	void ChangeViewXYZ();					
	
// Function: ChangeViewUp
// Ctrl+上矢印キーで15degづつBODYを回転させる
    bool ChangeViewUp();					

// Function: ChangeViewDown
// Ctrl+下矢印キーで15degづつBODYを回転させる
    bool ChangeViewDown();				
	
// Function: ChangeViewLeft
// Ctrl+左矢印キーで15degづつBODYを回転させる
    bool ChangeViewLeft();				
	
// Function: ChangeViewRight
// Ctrl+右矢印キーで15degづつBODYを回転させる
    bool ChangeViewRight();				
	
// Function: FitView
// 倍率をデフォルトに戻す
	void FitView();							

// Function: SolidView
// Solid表示
	void SolidView();						
	
// Function: NoEdgeView
// BODYをソリッド表示に切り替える
	void NoEdgeView();						
	
// Function: WireFlameView
// BODYをWireFlame表示に切り替える
	void WireFlameView();					
	
// Function: VoidView
// BODYを非表示に切り替える
	void VoidView();						
	
// Function: SelectAll
// 全てセレクト状態にする
	void SelectAll();						
	
// Function: SelectionCancel
// エンティティがセレクション状態にある場合は、セレクションを全て解除する
	void SelectionCancel();					
	
// Function: ViewBlending
// BODYを半透明にする
	void ViewBlending();					
	
// Function: DeleteBody
// 選択されているBODYを消去
	void DeleteBody();						
	
// Function: OpenDelBtn
// Open，Deleteボタンの表示属性の変更
	void OpenDelBtn();						
	
// Function: ExecuteUserFuncMain
// User関数を実行する
	void ExecuteUserFuncMain(int);			
	
// Functon: AcceptUserStat
// Userステータスダイアログの設定値を認識する
	void AcceptUserStat(int,double []);		
	
// Function: GetUserStat
// UserStatusを取得
    void GetUserStat(int *,double []);      
	
// Function: UserViewCanccel
// User関数の実行によって描画された画像を消去する
	void UserViewCanncel();					
	
// Function: SetModelScale
// 最適モデルスケールを取得する
	void SetModelScale(double);				
	
// Function: GetModelScale
// モデルスケールを取得する
	void GetModelScale();					

// Function: SetTolerance
// トレランス値をセットする
	void SetTolerance(double);				
	
// Function: GetTolerance
// トレランス値を取得する
	void GetTolerance();					
	
// Function: GetShiftBody
// BODYを平行移動
	void GetShiftBody(Coord);				
	
// Function: GetRotateBody
// BODYを回転
	void GetRotateBody(Coord,double);		
	
// Function: ExpandBody
// BODYを拡大
	void ExpandBody(Coord);					
	
// Function: GenSurface
// セレクションされているNUBS曲線に対して、任意軸周りに任意角度だけ回転/任意軸方向に任意長さだけスイープさせたNURBS曲面を生成する．
	int GenSurface(Coord,double,int);		
	
// Function: GenNurbsCurve
// Inputファイル(点列)を読み込み、指定した生成方法を用いてNURBS曲線を生成する
	int GenNurbsCurve(int,char *,int);		
	
// Function: GenNurbsSurface
// Inputファイル(点列)を読み込み、指定した生成方法を用いてNURBS曲面を生成する
	int GenNurbsSurface(int,char *,int);	
	
// Function: GetSurfInfo
// 選択されている曲面の情報をコンソール出力する
	void GetSurfInfo();						
	
// Function: changeRank
// 選択されている曲面のRankを変更する
	void ChangeRank(int []);				

// Function: ChangeBackColor
// 背景の色を変更する
	void ChangeBackColor(double []);		
	
// Function: *SearchBodyList
// セレクションされているBODYの実体を得る
	BODY *SearchBodyList(BODYList *,int);	
	
// Function: GetMeshInfo
// Mesh情報を出力
	void GetMeshInfo();						

// Function: DrawCharX
// 文字'X'を描画
	void DrawCharX(double,double);			
	
// Function: rawCharY
// 文字'Y'を描画
	void DrawCharY(double,double);			
	
// Function: DrawCharZ
// 文字'Z'を描画
	void DrawCharZ(double,double);			

// Function: DispUVdirection
// 選択されている曲面のU，V方向を表示する
	void DispUVdirection();					


private:
// Function: Describe_Main
// (Private)BODY描画メイン
	void Describe_Main();				
	
// Function: DrawBODY
// (Private)BODYの描画をする
	void DrawBODY();					
	
// Function: DrawAxis
// (Private)軸を描画する
	void DrawAxis(double,double);		
	
// Function: DrawRubberband
// (Private)ラバーバンドを描画する
	void DrawRubberband(double,double,double,double);	
	
// Function: Draw_NurbsCurve
// (Private)NURBS曲線を描画する
	void Draw_NurbsCurve(BODY *);		
	
// Function: Draw_NurbsSurface
// (Private)NURBS曲面を描画する
	void Draw_NurbsSurface(BODY *);		
	
// Function: Draw_TrimSurface
// (Private)トリム面を描画する
	void Draw_TrimSurfe(BODY *);		
	
// Function: Draw_Mesh
// (Private)メッシュを描画
	void Draw_Mesh(BODY *);				
	
// Function: UVWireView
// (Private)UVパラメータで分割されたワイヤーフレームを表示する
	void UVWireView();					
	
// Function: SetOpenGLStat
// (Private)OpenGLを初期化する
	void SetOpenGLStat();				
	
// Function: SetModelScale
// (Private)最適モデルスケールを取得する
	void SetModelScale();				
	
// Function: SetMaxCoord
// (Private)BODYの最大長を設定する
	void SetMaxCoord();					
	
// Function: DoSelect
// (Private)セレクション(マウスピッキング)を設定する
	void DoSelect(int,int);				

// Function: ClearSeldEntList
// (Private)セレクションリスト及びOBJECTを初期化する
	void ClearSeldEntList();			
	
// Function: DeleteBodySub
// (Private)選択されているBODYを消去する(Sub)
	void DeleteBodySub(BODY *,int);		
	
// Function: ClickPicking
// (Private)クリックによるマウスピッキングをする
	void ClickPicking(GLuint SelectBuf[],int hits);	
	
// Function: DragPicking
// (Private)ドラッグによるマウスピッキングをする
	void DragPicking(GLuint SelectBuf[],int hits);	
	
// Function: ObjSelect
// (Private)オブジェクトの選択判別する
	int  ObjSelect(GLuint [],int);					
	
// Function: AddEntSymbolToObj
// (Private)OBJECT構造体に何番目の曲線or曲面かの情報を付加する
	int AddEntSymbolToObj();			
	
// Function: InitCmdFlags
// (Private)Cmd用フラグを全てKOD_FALSEに初期化する
	void InitCmdFlags();				

// Function: SetNewObject
// (Private)ピックされたオブジェクトをOBJECTリストに登録する
	void SetNewObject(int,int,int);		
	
// Function: DescribeCP
// (Private)コントポイントを描画
	void DescribeCP();						



public:
// Function: UVWireFlameviewFlag
// UVワイヤーフレーム表示フラグ
	int  UVWireFlameViewFlag;	
	
// Function: CPViewFlag
// コントロールポイント描画フラグ
	int  CPViewFlag;			
	
// Function: UVDirFlag
// UV方向表示フラグ
	int  UVDirFlag;				


private:
// Function: User
// (Private)ユーザークラス
	USER User;					
	
// Function: BodyList
// (Private)BODYリストの操作用インスタンス
	BODYList	BodyList;		
	
// Function: SeldEntList
// (Private)セレクションされたエンティティを逐次格納していくリスト
	OBJECTList SeldEntList;		

// Function: CmdMap
// (Private)コマンドマップ
	CommandMap CmdMap[COMMANDNUMMAX];		
	
// Function: ConsoleCharNum
// (Private)コンソールに入力されている全ての文字数を保持
	int	ConsoleCharNum;			
	
// Function: Focus_Body
// (Private)選択中のBODYを表す値
	int Focus_Body;				
	
// Function: QFunc
// (Private)クォータニオンのインスタンス
	QUATERNION QFunc;			
	
// Function: RotMx
// (Private)クォータニオン→同時変換マトリックス
	double RotMx[TMATELEMNUM];	
	
// Function: StartQ
// (Private)右クリックした瞬間の回転角を格納するクォータニオン
	Quat StartQ;				
	
// Function: TargetQ
// (Private)ぐりぐりやっている途中のクォータニオン
	Quat TargetQ;				
	
// Function: ModelScale
// (Private)現在のモデルスケール
	double ModelScale;			
	
// Function: ModelScale1st
// (Private)モデルスケールの初期値
	double ModelScale1st;		
	
// Function: Tolerance
// (Private)NURBSを表現する時の面の粗さ
	double Tolerance;			
	
// Function; Trl
// (Private)平行移動量
	double Trl[3];				
	
// Function: Vp
// (Private)ビューポイント
	int  Vp[4];					
	
// Function: AxisScale
// (Private)軸描画のスケール
	double AxisScale;			
	
// Function: OpenGLInitFlag
// (Private)OpenGL初期設定済みフラグ
	int  OpenGLInitFlag;		
	
// Function: ExecUserFuncFlag
// (Private)UserFunc描画フラグ
	int  ExecUserFuncFlag[USERFUNCNUMMAX];	
	
// Function: ExecUserCommandFlag
// (Private)UserComand描画フラグ
	int  ExecUserCommandFlag[USERCOMMANDNUM];	
	
// Function: DoSelectFlag
// (Private)セレクション実行フラグ
	int  DoSelectFlag;			
	
// Function: SweepSelectFlag
// (Private)スイープセレクションを開始したことを示すフラグ
	int  SweepSelectFlag;		
	
// Function: ScreenHeight
// (Private)デフォルトスクリーンの高さ
	int  ScreenHeight;			
	
// Function: ScreenEidth
// (Private)デフォルトスクリーンの幅
	int  ScreenWidth;			
	
// Function: StartX
// (Private)ドラッグ開始位置X
	int  StartX;				

// Function: StartY
// (Private)ドラッグ開始位置Y
	int  StartY;				
	
// Function: OldPosX
// (Private)ウィンドウ再描画直前のX座標
	int  OldPosX;				
	
// Function: OldPosY
// (Private)ウィンドウ再描画直前のY座標
	int  OldPosY;				

// Function: CurrentX
// (Private)ドラッグ中の位置X
	int  CurrentX;				
	
// Function: CurrentY
// (Private)ドラッグ中の位置Y
	int  CurrentY;				
	
// Function: LBtnFlag
// (Private)右クリックしたことを示すフラグ
	int  LBtnFlag;				
	
// Function: RBtnFlag
// (Private)右クリックしたことを示すフラグ
	int  RBtnFlag;				
	
// Function: MBtnFlag
// (Private)中クリックしたことを示すフラグ
	int  MBtnFlag;				
	
// Function: ShiftKeyFlag
// (Private)Shiftキーを押したことを示すフラグ
	int  ShiftKeyFlag;			
	
// Function: CtrlKeyFlag
// (Private)Ctrlキーを押したことを示すフラグ
	int  CtrlKeyFlag;			
	
// Function: AltKeyFlag
// (Private)Altキーを押したことを示すフラグ
	int  AltKeyFlag;			
	
// Function:SolidViewFlag 
// (Private)ソリッド表示フラグ
	int  SolidViewFlag;			
	
// Function: EdgeViewFlag
// (Private)エッジ表示フラグ
	int  EdgeViewFlag;			
	
// Function: WireFlameViewFlag
// (Private)ワイヤーフレーム表示フラグ
	int  WireFlameViewFlag;		

// Function: BlendViewFlag
// (Private)半透明表示フラグ
	int  BlendViewFlag;			
	
// Function: DrawBODYFlag
// (Private)BODY描画フラグ
	int  DrawBODYFlag;			

// Function: DrawSurfFlag
// (Private)Surface描画フラグ
	int  DrawSurfFlag;			
	
// Function: ReDrawBODYFlag
// (Private)BODY描画1発目を示すフラグ
	int	 ReDrawBODYFlag;		
	
// Function: SelectBuf
// (Private)セレクトバッファ
	GLuint SelectBuf[MAXSELECT];	
	
// Function: argc
// (Private)コマンド引数
	int argc;						
	
// Function: argv
// (Private)コマンド引数
	char *argv[MAXCOMMANDOPNUM];	
	
// Function: CmdStack
// (Private)コマンド履歴
	char CmdStack[HISTORYNUMMAX][256];	
	
// Function: CmdStackNum
// (Private)コマンド履歴数
	int CmdStackNum;				

// Function: CmdStackNow
// (Private)背景色
	int CmdStackNow;
	double BkCol[3];			
};

// Variable: Kodatuno
// Kodatunoクラスのグローバル宣言
extern KODatUNO	Kodatuno;

// Function: CmdVerInf
// コマンド(バージョン情報出力)
void CmdVerInf(int,char *[]);			

// Function: CmdFileOpen
// コマンド(ファイルオープン)
void CmdFileOpen(int,char *[]);			

// Function: CmdUVWire
// コマンド(UVワイヤーフレーム表示)
void CmdUVWire(int,char *[]);			

// Function: CmdChScale
// コマンド(スケール変更)
void CmdChScale(int,char *[]);			

// Function: CmdGenRotSurf
// コマンド(回転サーフェス生成)
void CmdGenRotSurf(int,char *[]);		

// Function: CmdGenSweepSurf
// コマンド(スイープサーフェス生成) 
void CmdGenSweepSurf(int,char *[]);		

// Function: CmdGenNurbsCurve
// コマンド(Nurbs曲線生成)
void CmdGenNurbsCurve(int,char *[]);	

// Function: CmdGenNurbsSurface
// コマンド(トレランス変更)
void CmdGenNurbsSurface(int,char *[]);	
void CmdChTolerance(int,char *[]);		

// Function: CmdMoveBody
// コマンド(BODYの変更移動)
void CmdMoveBody(int,char *[]);			

// Function: CmdRotateBody
// コマンド(BODYの回転)
void CmdRotateBody(int ,char *[]);		

// Function: CmdCPView
// コマンド(コントロールポイント描画)
void CmdCPView(int ,char *[]);			

// Function: CmdSurfInfo
// コマンド(曲面情報を出力)
void CmdSurfInfo(int,char *[]);			

// Function: CmdExpand
// コマンド(BODYの拡大)
void CmdExpand(int,char *[]);			

// Function: CmdChRank
// コマンド(NURBS Rankの変更)
void CmdChRank(int,char *[]);			

// Function: CmdChBkCol
// コマンド(背景色変更)
void CmdChBkCol(int,char *[]);			

// Function: CmdMeshInf
// コマンド(Mesh情報出力)
void CmdMeshInf(int,char *[]);			

// Function: CmdUVdir
// コマンド(U,Vの方向を描画)
void CmdUVdir(int,char *[]);			

#endif
