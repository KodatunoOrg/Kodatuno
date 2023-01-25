#ifndef _USER_FUNC_H_
#define _USER_FUNC_H_

#include"BODY.h"
#include"Sample.h"

#define LABELNAMEMAX 256	// メニューに登録するメニュー名バッファ

enum Sub_Menu_Label{
	M_Label_Main,
	M_Label1,
	M_Label2,
	M_Label3,
	M_Label4,
	M_Label5
};

// Userクラス：描画セクションでのメニュー選択により発生するイベント内容を記述
class USER
{
public:
	USER();										// コンストラクタ
	void UserFuncMain(BODY *,OBJECT *,int);		// Userメイン関数
	void UserFunc1(BODY *,OBJECT *,int);		// Userサブ関数1
	void UserFunc2(BODY *,OBJECT *,int);		// Userサブ関数2
	void UserFunc3(BODY *,OBJECT *,int);		// Userサブ関数3
	void UserFunc4(BODY *,OBJECT *,int);		// Userサブ関数4
	void UserFunc5(BODY *,OBJECT *,int);		// Userサブ関数5

private:
	void SetMenuLabelName();					// メニューに登録するメニュー名をセット

public:
	char MenuLabelMain[LABELNAMEMAX];			// Userメイン関数のメニュー名
	char MenuLabel1[LABELNAMEMAX];				// Userサブ関数1のメニュー名
	char MenuLabel2[LABELNAMEMAX];				// Userサブ関数2のメニュー名
	char MenuLabel3[LABELNAMEMAX];				// Userサブ関数3のメニュー名
	char MenuLabel4[LABELNAMEMAX];				// Userサブ関数4のメニュー名
	char MenuLabel5[LABELNAMEMAX];				// Userサブ関数5のメニュー名


///////////新たに追加する場合は以下に記述//////////////////////////////////////////////////////////

};

#endif