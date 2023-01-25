#include "UserFunc.h"

// User用関数を5つまで登録することができます。
// 開発者はこれらに独自の処理を追加してください
// 各関数は以下の引数を持ちます  

// Function: USER
// コンストラクタ(ここでUserFunc関数ポインタの指す関数を指定する)
USER::USER()
{
	// ここでUserFunc関数ポインタの指す関数を指定する
	for(int i=0;i<USERFUNCNUMMAX;i++){
		if(i==0)
			UserFunc[i] = &USER::ExecSampleFunc0;		// サンプル
		else if(i==1)
			UserFunc[i] = &USER::ExecSampleFunc1;		// サンプル
		else if(i==2)
			UserFunc[i] = &USER::ExecSampleFunc2;		// サンプル
		else if(i==3)
            UserFunc[i] = &USER::ExecSampleFunc3;		// サンプル
		else if(i==4)
            UserFunc[i] = &USER::MasterUserFunc;		// 未使用のUserFunc[i]にはMasterUserFunc()を登録しておく
        else if(i==5)
            UserFunc[i] = &USER::MasterUserFunc;		// 未使用のUserFunc[i]にはMasterUserFunc()を登録しておく
        else if(i==6)
            UserFunc[i] = &USER::MasterUserFunc;		// 未使用のUserFunc[i]にはMasterUserFunc()を登録しておく
        else if(i==7)
            UserFunc[i] = &USER::MasterUserFunc;		// 未使用のUserFunc[i]にはMasterUserFunc()を登録しておく
        else
            UserFunc[i] = &USER::MasterUserFunc;		// 未使用のUserFunc[i]にはMasterUserFunc()を登録しておく
	}

	// Userステータスの初期化
	UserStat.Mode = 0;
	for(int i=0;i<USERPROPNUM;i++)
		UserStat.Prop[i] = 0;
}

// Function: ExecSampleFunc0
// Userボタン登録関数0の実体を記述
//
// Parameter:
// *BodyList - BODYの実体が登録順にリストされている
// *ObjList - セレクションされたエンティティ情報
// PickCount - セレクションされたエンティティの数
//
// Return:
// KOD_TRUE
int USER::ExecSampleFunc0(BODYList *BodyList,OBJECTList *ObjList,int PickCount)
{
	char mes[256];

	// ユーザーステータスでの選択されたモード番号をコンソール出力
	sprintf(mes,"Selected Mode : %d",UserStat.Mode);
    GuiIF.SetMessage(mes);

	// ユーザーステータスでのプロパティ値(最初の2つ)をコンソール出力
	sprintf(mes,"User Property 1: %lf",UserStat.Prop[0]);
    GuiIF.SetMessage(mes);
	sprintf(mes,"User Property 2: %lf",UserStat.Prop[1]);
    GuiIF.SetMessage(mes);

    GuiIF.SetMessage("....");

	// セレクション数をコンソール出力
	sprintf(mes,"All Pick Count : %d",PickCount);
    GuiIF.SetMessage(mes);

	for(int i=0;i<PickCount;i++){
		OBJECT *obj = (OBJECT *)ObjList->getData(i);			// i番目にセレクションされたエンティティの情報を得る
		sprintf(mes,"#%d",i+1);
        GuiIF.SetMessage(mes);
		sprintf(mes,"Body No. : %d",obj->Body);
        GuiIF.SetMessage(mes);
		sprintf(mes,"Selected Object Type : %d",obj->Type);
        GuiIF.SetMessage(mes);
		sprintf(mes,"Selected Object No. : %d",obj->Num);
        GuiIF.SetMessage(mes);
	}

	return KOD_TRUE;
}
	

// Function: ExecSampleFunc1
// Userボタン登録関数1の実体を記述
//
// Parameter:
// *BodyList - BODYの実体が登録順にリストされている
// *ObjList - セレクションされたエンティティ情報
// PickCount - セレクションされたエンティティの数
//
// Return:
// ユーザー指定のモード番号が範囲外：KOD_FALSE,		正常終了：KOD_TRUE
int USER::ExecSampleFunc1(BODYList *BodyList,OBJECTList *ObjList,int pickcount)
{
	// User StatusのModeによって処理を分ける
	if(UserStat.Mode == 0){
		SmpRegistBody(BodyList,ObjList);	// IGESファイルを読み込み，Bodyとして登録する
		return KOD_TRUE;
	}
	else if(UserStat.Mode == 1){
		SmpTransBody(BodyList,ObjList,pickcount,UserStat.Prop);		// 指定したBodyに対しアフィン変換を行う
		return KOD_TRUE;
	}
	else if(UserStat.Mode == 2){
		SmpDivCurves(BodyList,ObjList,pickcount,UserStat.Prop);		// 曲線を指定した分割数で分割した点を生成する
		return KOD_TRUE;
	}
	else if(UserStat.Mode == 3){
		SmpNearestPt(BodyList,ObjList,pickcount,UserStat.Prop);		// 任意の点からの曲面上の最近傍点を求める
		return KOD_TRUE;
	}
	else if(UserStat.Mode == 4){
		SmpUVDivLine(BodyList,ObjList,pickcount,UserStat.Prop);		// 曲面のUVパラメータで等間隔に分割した点を生成する
		return KOD_TRUE;
	}
	else{
		// void
		return KOD_FALSE;
	}
}

// Function: ExecSampleFunc2
// Userボタン登録関数2の実体を記述
//
// Parameter:
// *BodyList - BODYの実体が登録順にリストされている
// *ObjList - セレクションされたエンティティ情報
// PickCount - セレクションされたエンティティの数
//
// Return:
// ユーザー指定のモード番号が範囲外：KOD_FALSE,		正常終了：KOD_TRUE
int USER::ExecSampleFunc2(BODYList *BodyList,OBJECTList *ObjList,int pickcount)
{
	// User StatusのModeによって処理を分ける
	if(UserStat.Mode == 0){
		SmpContourLine(BodyList,ObjList,pickcount,UserStat.Prop);		// 等高線を生成する
        return KOD_TRUE;
	}
	else if(UserStat.Mode == 1){
		SmpIntersectSurfs(BodyList,ObjList,pickcount,UserStat.Prop);	// 曲面同士の交線を算出する
		return KOD_TRUE;
	}
	else if(UserStat.Mode == 2){
		SmpMeanCurvature(BodyList,ObjList,pickcount,UserStat.Prop);		// 平均曲率法線ベクトルを算出する
		return KOD_TRUE;
	}
	else{
		// void
		return KOD_FALSE;
	}
}

// Function: ExecSampleFunc3
// Userボタン登録関数3の実体を記述
//
// Parameter:
// *BodyList - BODYの実体が登録順にリストされている
// *ObjList - セレクションされたエンティティ情報
// PickCount - セレクションされたエンティティの数
//
// Return:
// KOD_TRUE
int USER::ExecSampleFunc3(BODYList *BodyList,OBJECTList *ObjList,int pickcount)
{
    GuiIF.SetMessage("aaaaaaa");
    return KOD_TRUE;
}

// Function: ExecWidgetFunc
// UserがWidgetの機能を使い新たなDialogをポップアップさせたい場合は、ここにそのWS処理を記述
//
// Parameter:
// val - Userボタンのシンボル(0～5)
//
// Return:
// KOD_TRUE
int USER::ExecWidgetFunc(int val)
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
    else if(val == 5){
        // 処理を追加できます
    }

    else if(val == 6){
        // 処理を追加できます
    }

    else if(val == 7){
        // 処理を追加できます
    }

	return KOD_TRUE;
}

// Function: ExecdClear
// KODATUNOのViewメニューのCanncelボタン(消しゴムマーク)が押されるとこの関数が実行される(ここにCanncelボタン押下時のUser初期化コードを記述)
void USER::ExecdClear()
{
	// 処理を追加できます
}

// Function: SetMenuLabelName
// Userメニュー名を登録(Userボタンの表示名)
void USER::SetMenuLabelName()
{
	// 独自のメニュー名をここに記述してください
	strcpy(MenuLabel1,"Sample Func 0");
	strcpy(MenuLabel2,"Sample Func 1");
	strcpy(MenuLabel3,"Sample Func 2");
	strcpy(MenuLabel4,"Sample Func 3");
	strcpy(MenuLabel5,"User Func 5");
    strcpy(MenuLabel6,"User Func 6");
    strcpy(MenuLabel7,"User Func 7");
    strcpy(MenuLabel8,"User Func 8");
}

// Function: RegistUserCommand
// Userコマンドを登録
// 
// Parameter:
// *CmdMap - コマンド用構造体
void USER::RegistUserCommand(CommandMap *CmdMap)
{
	// 独自のコマンド情報をここに記述してください
	// 第5引数はNULLを指定

}

// Function: Command
// Userコマンドを実行
//
// Parameter:
// *BodList - BODYリストの先頭アドレス
// *ObjList - セレクションされたエンティティの情報(セレクションの数だけリスト化)
// pickcount - セレクション数
// CmdNo - 実行されたコマンド番号
// argc - いつものアレ
// *argv[] - いつものアレ
void USER::Command(BODYList *BodyList,OBJECTList *ObjList,int pickcount,int CmdNo,int argc,char *argv[])
{
	// 登録したコマンドごとに処理を記述してください

}








///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// これ以降はKodatuno開発者以外は変更しないこと
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Function: SetUserStatus
// UserステータスをUserStat構造体に格納(User StatusダイアログOKボタン押下時に実行される)(開発者以外は変更しない)
// 
// Parameter:
// mode - ユーザー指定モード番号
// prop[] - ユーザー指定プロパティ値
void USER::SetUserStatus(int mode,double prop[])
{
	UserStat.Mode = mode;
	for(int i=0;i<USERPROPNUM;i++){
		UserStat.Prop[i] = prop[i];
	}
}

// Function: GetUserStatus
// Userステータスを返す
//
// Return:
// Userステータス
UserStat_ *USER::GetUserStatus()
{
    return &UserStat;
}

// Function: GetMenuLabelName
// Userボタンラベル名を取得(開発者以外は変更しない)
//
// Parameter:
// val - メニューラベルのインデックス
//
// Return:
// メニューラベル名
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
        case M_Label6:
            return MenuLabel6;
        case M_Label7:
            return MenuLabel7;
        case M_Label8:
            return MenuLabel8;
	}

	return NULL;
}

// Function: MasterUserFunc
// UserFunc関数へのポインタの初期値として使用(開発者以外は変更しない)
//
// Parameter:
// *BodyList - BODYの実体が登録順にリストされている
// *ObjList - セレクションされたエンティティ情報
// PickCount - セレクションされたエンティティの数
//
// Return:
// KOD_TRUE
int USER::MasterUserFunc(BODYList *BodyList,OBJECTList *Obj,int PickCount)
{
    GuiIF.SetMessage("User Func is not registered.");
	return KOD_TRUE;
}
