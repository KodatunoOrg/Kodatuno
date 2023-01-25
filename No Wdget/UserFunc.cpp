#include "UserFunc.h"

// User用関数を5つまで登録することができます。
// 開発者はこれらに独自の処理を追加してください
// 各関数は以下の引数を持ちます  

// コンストラクタ
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
			UserFunc[i] = &USER::MasterUserFunc;		
		else if(i==4)
			UserFunc[i] = &USER::MasterUserFunc;					
		else
			UserFunc[i] = &USER::MasterUserFunc;		// 未使用のUserFunc[i]にはMasterUserFunc()を登録しておく
	}

}


// Userボタン登録関数0の実体を記述
// *BodyList:BODYの実体が登録順にリストされている
// *ObjList:セレクションされたエンティティ情報
// PickCount:セレクションされたエンティティの数
int USER::ExecSampleFunc0(BODYList *BodyList,OBJECTList *ObjList,int PickCount)
{
	char mes[256];

	// セレクション数をコンソール出力
	sprintf(mes,"All Pick Count : %d",PickCount);
	SetMessage(mes);

	for(int i=0;i<PickCount;i++){
		OBJECT *obj = (OBJECT *)ObjList->getData(i);			// i番目にセレクションされたエンティティの情報を得る
		sprintf(mes,"#%d",i+1);
		SetMessage(mes);
		sprintf(mes,"Body No. : %d",obj->Body);
		SetMessage(mes);
		sprintf(mes,"Selected Object Type : %d",obj->Type);
		SetMessage(mes);
		sprintf(mes,"Selected Object No. : %d",obj->Num);
		SetMessage(mes);
	}

	return KOD_TRUE;
}
	

// Userボタン登録関数1の実体を記述
// *BodyList:BODYの実体が登録順にリストされている
// *ObjList:セレクションされたエンティティ情報
// PickCount:セレクションされたエンティティの数
int USER::ExecSampleFunc1(BODYList *BodyList,OBJECTList *ObjList,int PickCount)
{
	if(!PickCount)	return KOD_ERR;		// セレクションされていなかったら、何もしない

	NURBS_Func	nfunc;					// NURBSを扱う関数集を呼び出す
	double green[3] = {0,1,0};			// 点表示の色(緑)

	OBJECT *obj = (OBJECT *)ObjList->getData(0);		// 一番最初にセレクションされたエンティティの情報を得る
	BODY *body = (BODY *)BodyList->getData(obj->Body);	// 一番最初にセレクションされたBODYの実体を得る

	// 本来であれば，Sample4に示した曲面のタイプによる条件分岐が必要であるが，これ以降のSampleではトリム面で決め打ちする
	if(obj->Type != _TRIMMED_SURFACE)	return KOD_ERR;	

	NURBSS *S = body->TrmS[obj->Num].pts;				// BODYからNURBS曲面を取り出す

	int u_divnum = 50;						// ユーザーステータスのprop1をu方向分割数として読み込み
	int v_divnum = 50;						// ユーザーステータスのprop2をv方向分割数として読み込み
	double u_val = (S->U[1] - S->U[0])/u_divnum;		// パラメトリック空間内でのu方向線分長を得る
	double v_val = (S->V[1] - S->V[0])/v_divnum;		// パラメトリック空間内でのv方向線分長を得る

	// u方向，v方向の各分割点における座標値を求める
	for(int i=0;i<=u_divnum;i++){
		for(int j=0;j<=v_divnum;j++){
			Coord P = nfunc.CalcNurbsSCoord(S,S->U[0]+u_val*i,S->V[0]+v_val*j);	// 指定した(u,v)の座標値を求める
			DrawPoint(P,1,3,green);						// 描画
		}
	}

	return KOD_TRUE;}

// Userボタン登録関数2の実体を記述
int USER::ExecSampleFunc2(BODYList *BodyList,OBJECTList *ObjList,int PickCount)
{
	if(!PickCount)	return KOD_ERR;		// セレクションされていなかったら、何もしない

	NURBS_Func	nfunc;					// NURBSを扱う関数集を呼び出す

	OBJECT *obj = (OBJECT *)ObjList->getData(0);		// 一番最初にセレクションされたエンティティの情報を得る
	BODY *body = (BODY *)BodyList->getData(obj->Body);	// 一番最初にセレクションされたBODYの実体を得る
	if(obj->Type != _TRIMMED_SURFACE)	return KOD_ERR;	// セレクションされた曲面がトリム面でない場合は終了

	NURBSS *S = body->TrmS[obj->Num].pts;				// BODYからNURBS曲面を取り出す

	Coord t[5000];					// 解格用納
	double red[3] = {1,0,0};		// 法線ベクトル表示の色
	double blue[3] = {0,0,1};		// 点表示の色
	char mes[256];					// メッセージ出力用

	double feed = 1;			// 交点群の点間隔
	double under = -20;			// 等高線のZ_min
	double upper = 40;			// 等高線のZ_max
	double delta = 5;			// 等高線生成のZ間隔
	int step = fabs(upper - under)/delta + 1;	// 等高線の本数を算出

	Coord nvec = SetCoord(0,0,1);	// 平面の法線ベクトルを指定(X-Y平面とする)

	// 平面をZ方向にシフトしていきながら，等高線を算出する
	for(int i=0;i<step;i++){
		double z = under + delta*(double)i;	// 現在の平面のZ位置
		Coord pt = SetCoord(0,0,z);			// 現在の平面上の1点を指定

		sprintf(mes,"z=%.3lf  calculating...",z);
		SetMessage(mes);

		int num = nfunc.CalcIntersecPtsPlaneSearch(S,pt,nvec,feed,10,t,5000,RUNGE_KUTTA);		// NURBS曲面と平面との交点群を交線追跡法で求める

		for(int i=0;i<num;i++){		// 交点の数だけループ
			Coord p = nfunc.CalcNurbsSCoord(S,t[i].x,t[i].y);			// 交点をパラメータ値から座標値へ変換
			Coord nt = nfunc.CalcNormVecOnNurbsS(S,t[i].x,t[i].y);		// 交点上の法線ベクトルを計算
			nt = MulCoord(nt,-2);											// 外向き法線ベクトルへ変換し適当な長さにする
			DrawPoint(p,1,3,blue);			// 交点を描画
			DrawVector(p,nt,1,1,red);		// 法線ベクトルを描画
		}
	}
	SetMessage("END");


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
	strcpy(MenuLabel1,"Sample Func 0");
	strcpy(MenuLabel2,"Sample Func 1");
	strcpy(MenuLabel3,"Sample Func 2");
	strcpy(MenuLabel4,"User Func 4");
	strcpy(MenuLabel5,"User Func 5");
}







///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// これ以降はKodatuno開発者以外は変更しないこと
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////



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