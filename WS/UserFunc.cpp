#include"UserFunc.h"

// User用関数を5つまで登録することができます。
// 開発者はこれらに独自の処理を追加してください
// 各関数は以下の引数を持ちます  
// *Body:BODYリストの先頭ポインタ
// *Obj:セレクションされたエンティティ情報
// PickCount:セレクションされたエンティティの数

// コンストラクタ
USER::USER()
{
	// ここでUserFunc関数ポインタの指す関数を指定する
	for(int i=0;i<USERFUNCNUMMAX;i++){
		if(i==0)
			UserFunc[i] = &USER::MasterUserFunc;		
		else if(i==1)
			UserFunc[i] = &USER::MasterUserFunc;		
		else if(i==2)
			UserFunc[i] = &USER::MasterUserFunc;		
		else if(i==3)
			UserFunc[i] = &USER::CalcIntersecPts;		// サンプル
		else if(i==4)
			UserFunc[i] = &USER::MasterUserFunc;						
		else
			UserFunc[i] = &USER::MasterUserFunc;		// 未使用のUserFunc[i]にはMasterUserFunc()を登録しておく
	}

	// Userステータスの初期化
	UserStat.Mode = 0;
	for(int i=0;i<USERPROPNUM;i++)
		UserStat.Prop[i] = 0;
}

// Userボタン登録関数4の実体を記述
int USER::CalcIntersecPts(BODYList *BodyList,OBJECTList *ObjList,int pickcount)
{
	if(UserStat.Mode == 0){
		ExecSampleFunc1(BodyList,ObjList,pickcount,UserStat.Prop);		// サンプル関数1を実行する
		return KOD_TRUE;
	}
	else if(UserStat.Mode == 1){
		ExecSampleFunc2(BodyList,ObjList,pickcount,UserStat.Prop);		// サンプル関数2を実行する
		return KOD_TRUE;
	}
	else{
		// void
		return KOD_FALSE;
	}
}

// UserがWSの機能を使い新たなDialogをポップアップさせたい場合は、ここにそのWS処理を記述
// 引数   val:Userボタンのシンボル(0〜5)
int USER::ExecWSFunc(int val)
{
	// Userボタン1が押された
	if(val == 0){
		// 処理を追加できます
	}

	// Userボタン2が押された
	else if(val == 1){
		// 処理を追加できます
	}

	// Userボタン3が押された
	else if(val == 2){
		// 処理を追加できます
	}

	// Userボタン4が押された
	else if(val == 3){
		// 処理を追加できます
	}

	// Userボタン5が押された
	else if(val == 4){
		// 処理を追加できます
	}

	return KOD_TRUE;
}

// KODATUNOのViewメニューのCanncelボタン(消しゴムマーク)が押されるとこの関数が実行される
// ここにCanncelボタン押下時のUser初期化コードを記述
void USER::ExecdClear()
{
	// 処理を追加できます
}

// Userメニュー名を登録(Userボタンの表示名)
void USER::SetMenuLabelName()
{
	// 独自のメニュー名をここに記述してください
	strcpy(MenuLabel1,"User Func 1");
	strcpy(MenuLabel2,"User Func 2");
	strcpy(MenuLabel3,"User Func 3");
	strcpy(MenuLabel4,"CalcIntersecLine");
	strcpy(MenuLabel5,"User Func 5");
}

// Userコマンドを登録
void USER::RegistUserCommand(CommandMap *CmdMap)
{
	// 独自のコマンド情報をここに記述してください
	// 第5引数はNULLを指定

}

// Userコマンドを実行
// 引数   *BodList:BODYリストの先頭アドレス    *ObjList:セレクションされたエンティティの情報(セレクションの数だけリスト化)
//        pickcount:セレクション数    CmdNo:実行されたコマンド番号      argc,*argv[]:いつものアレ
void USER::Command(BODYList *BodyList,OBJECTList *ObjList,int pickcount,int CmdNo,int argc,char *argv[])
{
	// 登録したコマンドごとに処理を記述してください

}











///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// これ以降はKodatuno開発者以外は変更しないこと
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// UserステータスをUserStat構造体に格納(User StatusダイアログOKボタン押下時に実行される)(ここには何も記述しない)
void USER::SetUserStatus(int mode,double prop[])
{
	UserStat.Mode = mode;
	for(int i=0;i<USERPROPNUM;i++){
		UserStat.Prop[i] = prop[i];
	}
}


// Userボタンラベル名を取得(ここには何も記述しない)
char *USER::GetMenuLabelName(int val)
{
	switch(val){
		case M_Label1:
			return MenuLabel1;
		case M_Label2:
			return MenuLabel2;
		case M_Label3:
			return MenuLabel3;
		case M_Label4:
			return MenuLabel4;
		case M_Label5:
			return MenuLabel5;
	}

	return NULL;
}

// UserFunc関数へのポインタの初期値として使用(ここには何も記述しない)
int USER::MasterUserFunc(BODYList *BodyList,OBJECTList *Obj,int PickCount)
{
	// void
	return KOD_TRUE;
}