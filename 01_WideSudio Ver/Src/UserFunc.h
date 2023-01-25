#ifndef _USER_FUNC_H_
#define _USER_FUNC_H_

#include "StdAfx.h"
#include "Command.h"
#include "BODY.h"
#include "UserFuncSub.h"


#define LABELNAMEMAX 256	// メニューに登録するメニュー名バッファ
#define USERFUNCNUMMAX	5	// UserFuncの最大登録数
#define USERPROPNUM	15		// 利用できるUserプロパティ数


// コマンド登録番号(登録コマンドのシンボルを100～199の数値で指定する)

// Userステータス値格納用構造体
typedef struct{
	int Mode;
	double Prop[USERPROPNUM];
}UserStat_;

enum Sub_Menu_Label{
	M_Label1,
	M_Label2,
	M_Label3,
	M_Label4,
	M_Label5
};

// Userクラス：描画セクションでのメニュー選択により発生するイベント内容を記述
class USER
{
// KODATUNOシステムとのブリッジ関数(開発者以外は変更しないこと) ///////////////////////////////////////
public:
	USER();														// コンストラクタ
	int (USER::*UserFunc[USERFUNCNUMMAX])(BODYList *,OBJECTList *,int);	// UserFunc関数へのポインタ
	char *GetMenuLabelName(int);								// メニュー名を渡す	
	void SetMenuLabelName();									// メニューに登録するメニュー名をセット
	void RegistUserCommand(CommandMap *);						// Userコマンドの登録
	void Command(BODYList *,OBJECTList *,int,int,int,char *[]);	// Userコマンドの実行
	int MasterUserFunc(BODYList *,OBJECTList *,int);			// 未使用UserFunc関数へのポインタは必ずこれにする
	void SetUserStatus(int,double []);							// UserステータスをUserStat構造体に格納
	void ExecdClear();											// Canncelボタンが押されるとこの関数が実行されるので、ここに初期化コードを挿入しておく
	int ExecWSFunc(int);										// 各Userボタンに対してWS関数を使用したい場合はこの関数内で割り当てること

private:
	char MenuLabel1[LABELNAMEMAX];				// User関数1のメニュー名
	char MenuLabel2[LABELNAMEMAX];				// User関数2のメニュー名
	char MenuLabel3[LABELNAMEMAX];				// User関数3のメニュー名
	char MenuLabel4[LABELNAMEMAX];				// User関数4のメニュー名
	char MenuLabel5[LABELNAMEMAX];				// User関数5のメニュー名
	UserStat_	UserStat;						// Userステータス

///////////////////////////////////////////////////////////////////////////////////////////////////////


///////////新たにUser関数等を登録する場合は以下にprivate:で記述////////////////////////////////////////
private:
	int CalcIntersecPts(BODYList *,OBJECTList *,int);	// サンプル

};

#endif
