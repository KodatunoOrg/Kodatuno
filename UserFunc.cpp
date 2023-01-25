#include"UserFunc.h"

// User用メイン関数1個、サブ関数5つまで登録することができます。
// 開発者はこれらに独自の処理を追加してください
// 各関数は以下の引数を持ちます  
// *Body:BODYリストの先頭ポインタ
// *Obj:セレクションされたエンティティ情報
// PickCount:セレクションされたエンティティの数

// Userメイン関数
void USER::UserFuncMain(BODY *Body,OBJECT *Obj,int PickCount)
{
	SampleMain(Body,Obj,PickCount);		// サンプル
}

// Userサブ関数1
void USER::UserFunc1(BODY *Body,OBJECT *Obj,int PickCount)
{
	// ここに新たな処理を追加できます
}

// Userサブ関数2
void USER::UserFunc2(BODY *Body,OBJECT *Obj,int PickCount)
{
	// ここに新たな処理を追加できます
}

// Userサブ関数3
void USER::UserFunc3(BODY *Body,OBJECT *Obj,int PickCount)
{
	// ここに新たな処理を追加できます
}

// Userサブ関数4
void USER::UserFunc4(BODY *Body,OBJECT *Obj,int PickCount)
{
	// ここに新たな処理を追加できます
}

// Userサブ関数5
void USER::UserFunc5(BODY *Body,OBJECT *Obj,int PickCount)
{
	// ここに新たな処理を追加できます
}

// Userメニュー名を登録
void USER::SetMenuLabelName()
{
	// 独自のメニュー名をここに記述してください
	strcpy(MenuLabelMain,"Exec Sample Func");
	strcpy(MenuLabel1,"Exec Func1");
	strcpy(MenuLabel2,"Exec Func2");
	strcpy(MenuLabel3,"Exec Func3");
	strcpy(MenuLabel4,"Exec Func4");
	strcpy(MenuLabel5,"Exec Func5");
}

// コンストラクタ
USER::USER()
{
	SetMenuLabelName();		// Userメニュー名を登録
}