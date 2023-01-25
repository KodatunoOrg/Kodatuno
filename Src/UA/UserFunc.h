#ifndef _USER_FUNC_H_
#define _USER_FUNC_H_

#include "StdAfx.h"
#include "Command.h"
#include "BODY.h"
#include "SampleFunc.h"

// Constants: General Defines
// LABELNAMEMAX -		メニューに登録するメニュー名バッファ(256)
// USERFUNCNUMMAX -		UserFuncの最大登録数(8)
// USERPROPNUM -		利用できるUserプロパティ数(15)
#define LABELNAMEMAX	256
#define USERFUNCNUMMAX	8
#define USERPROPNUM		15


// コマンド登録番号(登録コマンドのシンボルを100～199の数値で指定する)

// Structure: UserStat_
// Userステータス値格納用構造体
// 
// Variables:
// Mode -				モード番号
// Prop[USERPROPNUM] -	プロパティ値
typedef struct{
	int Mode;
	double Prop[USERPROPNUM];
}UserStat_;

// Enum: Enum Symbol of Menu Label
// M_Label1 - 0
// M_Label2 - 1
// M_Label3 - 2
// M_Label4 - 3
// M_Label5 - 4
// M_Label6 - 5
// M_Label7 - 6
// M_Label8 - 7
enum Sub_Menu_Label{
	M_Label1,
	M_Label2,
	M_Label3,
	M_Label4,
    M_Label5,
    M_Label6,
    M_Label7,
    M_Label8
};

// Class: USER
// 描画セクションでのメニュー選択により発生するイベント内容を記述
class USER
{

///////////新たにUser関数等を登録する場合は以下にprivate:で記述////////////////////////////////////////

private:
	// Function:ExecSampleFunc0
	// サンプル関数1
	int ExecSampleFunc0(BODYList *,OBJECTList *,int);	
	
	// Function:ExecSampleFunc1
	// サンプル関数2
	int ExecSampleFunc1(BODYList *,OBJECTList *,int);
	
	// Function:ExecSampleFunc2
	// サンプル関数3
	int ExecSampleFunc2(BODYList *,OBJECTList *,int);
    
	// Function:ExecSampleFunc3
	// サンプル関数4
	int ExecSampleFunc3(BODYList *,OBJECTList *,int);

///////////////////////////////////////////////////////////////////////////////////////////////////////





// KODATUNOシステムとのブリッジ関数(開発者以外は変更しないこと) ///////////////////////////////////////
public:

	// Function: USER
	// コンストラクタ
	USER();

	// Function: USER::*UserFunc[USERFUNCNUMMAX]
	// UserFunc関数へのポインタ
	int (USER::*UserFunc[USERFUNCNUMMAX])(BODYList *,OBJECTList *,int);

	// Function: GetMenuLabelName
	// メニュー名を渡す	
	char *GetMenuLabelName(int);
	
	// Function: SetMenuLabelName
	// メニューに登録するメニュー名をセット
	void SetMenuLabelName();
	
	// Function: RegistUserCommand
	// Userコマンドの登録
	void RegistUserCommand(CommandMap *);
	
	// Function: Command
	// Userコマンドの実行
	void Command(BODYList *,OBJECTList *,int,int,int,char *[]);
	
	// Function: MasterUserFunc
	// 未使用UserFunc関数へのポインタは必ずこれにする
	int MasterUserFunc(BODYList *,OBJECTList *,int);
	
	// Function: SetUserStatus
	// UserステータスをUserStat構造体に格納
	void SetUserStatus(int,double []);
    
	// Function: GetUserStatus
	// UserStatステータスを取得
	UserStat_ *GetUserStatus();
	
	// Function: ExecdClear
	// Canncelボタンが押されるとこの関数が実行されるので、ここに初期化コードを挿入しておく
	void ExecdClear();
	
	// Function: ExecWidgetFunc
	// 各Userボタンに対してWS関数を使用したい場合はこの関数内で割り当てること
	int ExecWidgetFunc(int);

private:
	// Variable: MenuLabel1
	// User関数1のメニュー名
	char MenuLabel1[LABELNAMEMAX];				
	
	// Variable: MenuLabel2
	// User関数2のメニュー名
	char MenuLabel2[LABELNAMEMAX];				
	
	// Variable: MenuLabel3
	// User関数3のメニュー名
	char MenuLabel3[LABELNAMEMAX];				
	
	// Variable: MenuLabel4
	// User関数4のメニュー名
	char MenuLabel4[LABELNAMEMAX];				
	
	// Variable: MenuLabel5
	// User関数5のメニュー名
	char MenuLabel5[LABELNAMEMAX];				
    
	// Variable: MenuLabel6
	// User関数6のメニュー名
	char MenuLabel6[LABELNAMEMAX];				
    
	// Variable: MenuLabel7
	// User関数7のメニュー名
	char MenuLabel7[LABELNAMEMAX];				
    
	// Variable: MenuLabel8
	// User関数8のメニュー名
	char MenuLabel8[LABELNAMEMAX];				
	
	// Variable: UserStat
	// Userステータス
	UserStat_	UserStat;						

};

#endif
