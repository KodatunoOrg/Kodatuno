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

// Userボタン登録関数4の実体を記述
int USER::CalcIntersecPts(BODYList *BodyList,OBJECTList *ObjList,int pickcount)
{
	if(!pickcount)	return KOD_ERR;		// セレクションされていなかったら、何もしない

	BODY *body;
	OBJECT *obj;
	int i;

	// セレクションされた数だけループ
	for(i=0;i<pickcount;i++){
		obj = (OBJECT *)ObjList->getData(i);				// セレクションされたエンティティの情報を得る
		body = (BODY *)BodyList->getData(obj->Body);		// セレクションされたBODYの実体を得る

		// セレクションされたエンティティがトリム面かNURBS曲面
		if(obj->Type == _NURBSS){
				ContourLine(&body->NurbsS[obj->Num]);
		}		
		else if(obj->Type == _TRIMMED_SURFACE){
			ContourLine(body->TrmS[obj->Num].pts);
		}


		// セレクションされたエンティティがNURBS曲線
		else if(obj->Type == _NURBSC){						
			// 処理を追加してください
		}
	}

	return KOD_TRUE;
}


// サンプルとして平面とNURBS曲線の交線(交点群)を算出するプログラムを示す
// 平面をZ方向にシフトさせていき、各Z位置での交線を逐次求める
// 引数  *NurbsS：交線算出対象となるNurbs曲面へのポインタ
int USER::ContourLine(NURBSS *NurbsS)
{
	NURBS_Func NFunc;				// NURBS関連の関数を集めたクラスのオブジェクトを生成
	Coord t[5000];					// 解
	int   num;						// 解の数
	Coord pt=SetCoord(0,0,10);		// 平面上の1点を指定
	Coord nvec=SetCoord(0,0,1);		// 平面の法線ベクトルを指定
	double pcolor[3] = {0,1,0};		// 表示の色
	double tcolor[3] = {1,0,0};
	int allnum = 0;
	double under_z = -20;
	double upper_z = 40;
	double delta = 4;
	int step = fabs(upper_z - under_z)/delta + 1;

	for(int i=0;i<step;i++){
		double z = under_z + delta*(double)i;
		if(z == 0.0) continue;			// z=0の時は特異点なので計算しない
		pt = SetCoord(0,0,z);			// 平面をZ方向に徐々にシフトしていく
		char mes[256];
		sprintf(mes,"z=%.3lf  calculating...",z);
		SetMessage(mes);
		num = NFunc.CalcIntersecPtsPlaneSearch(NurbsS,pt,nvec,0.5,10,t,5000);		// NURBS曲面と平面との交点群を交線追跡法で求める
		sprintf(mes,"num = %d",num);
		SetMessage(mes);
		allnum += num;
		for(int i=0;i<num;i++){		// 交点の数だけループ
			//if(i%5 == 0){
				Coord p = NFunc.CalcNurbsSCoord(NurbsS,t[i].x,t[i].y);			// 交点をパラメータ値から座標値へ変換
				Coord nt = NFunc.CalcNormVecOnNurbsS(NurbsS,t[i].x,t[i].y);		// 交点上の法線ベクトルを計算
				nt = MulCoord(nt,-1);											// 外向き法線ベクトルへ変換
				DrawPoint(p,1,3,pcolor);			// 交点を描画
				//DrawVector(p,nt,3,1,tcolor);		// 法線ベクトルを描画
			//}
		}
	}
	SetMessage("END");

	return KOD_TRUE;
}


