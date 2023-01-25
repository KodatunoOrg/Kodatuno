#ifndef _USER_FUNC_H_
#define _USER_FUNC_H_

#include "StdAfx.h"
#include "BODY.h"
#include "NURBS_Func.h"


#define LABELNAMEMAX 256	// メニューに登録するメニュー名バッファ
#define USERFUNCNUMMAX	5	// UserFuncの最大登録数
#define USERPROPNUM	15		// 利用できるUserプロパティ数

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
	int MasterUserFunc(BODYList *,OBJECTList *,int);			// 未使用UserFunc関数へのポインタは必ずこれにする
	void ExecdClear();											// Canncelボタンが押されるとこの関数が実行されるので、ここに初期化コードを挿入しておく

private:
	char MenuLabel1[LABELNAMEMAX];				// User関数1のメニュー名
	char MenuLabel2[LABELNAMEMAX];				// User関数2のメニュー名
	char MenuLabel3[LABELNAMEMAX];				// User関数3のメニュー名
	char MenuLabel4[LABELNAMEMAX];				// User関数4のメニュー名
	char MenuLabel5[LABELNAMEMAX];				// User関数5のメニュー名

///////////////////////////////////////////////////////////////////////////////////////////////////////


///////////新たにUser関数等を登録する場合は以下にprivate:で記述////////////////////////////////////////
private:
	int CalcIntersecPts(BODYList *,OBJECTList *,int);	// サンプル
	int ContourLine(NURBSS *);			// 等高線生成

};

#endif
