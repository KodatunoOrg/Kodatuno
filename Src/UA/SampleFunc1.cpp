// UserSampleを記述
#include "SampleFunc.h"

// Function: SmpRegistBody
// Sample1(IGESファイルを読み込み，Bodyとして登録する)
//
// Parameter:
// *BodyList - BODYの実体が登録順にリストされている
// *ObjList - セレクションされたエンティティ情報
//
// Return:
// ファイル読み込みに失敗：KOD_ERR,		正常終了：KOD_TRUE
int SmpRegistBody(BODYList *BodyList,OBJECTList *ObjList)
{
	char fname[256] = "../../../Model/IGES/FreeSurf.igs";	// ここではファイル名は決め打ちとする
	IGES_PARSER Iges;					// IGESファイルを読み込むためのクラスを呼び出す
	BODY *body = new BODY;				// 読み込んだIGESデータを格納するための箱(BODY)を用意

	int flag = Iges.IGES_Parser_Main(body,fname);	// IGESデータを読み込んで、bodyに格納
	if(flag == KOD_TRUE){							// 読み込み成功なら
		Iges.Optimize4OpenGL(body);					// 読み込んだIGESファイルをOpenGL用に最適化する(必ずこの関数を実行してください)
        GuiIF.SetMessage("Succeeded to read IGES file");
	}
	else{											// 読み込みに失敗したら
		delete body;								// 確保したメモリーを解放し
        GuiIF.SetMessage("Error: fail to read IGES file");
		return KOD_ERR;								// 終了
	}

	body->RegistBody(BodyList,fname);		// Bodyをリストに登録

	// 読み込んだBODYを消去する場合は以下を実行
	//body->DeleteBody(BodyList);			// Bodyをリストからはずす
	//delete body;							// Body自身のメモリー解放

	return KOD_TRUE;
}

// Function: SmpTransBody
// Sample2(指定したBodyに対しアフィン変換を行う)
//
// Parameter:
// *BodyList - BODYの実体が登録順にリストされている
// *ObjList - セレクションされたエンティティ情報
// PickCount - セレクションされたエンティティの数
// Prop[] - ユーザー指定プロパティ値
//
// Return:
// 何も選択されていない：KOD_ERR,		正常終了：KOD_TRUE
int SmpTransBody(BODYList *BodyList,OBJECTList *ObjList, int PickCount, double Prop[])
{
	if(!PickCount)	return KOD_ERR;		// セレクションされていなかったら、何もしない

	OBJECT *obj = (OBJECT *)ObjList->getData(0);			// 一番最初にセレクションされたエンティティの情報を得る
	BODY *body = (BODY *)BodyList->getData(obj->Body);		// 一番最初にセレクションされたBODYの実体を得る

	Coord rot_ax = SetCoord(0,0,1);		// 回転軸
	double deg = 90;					// 回転角度
	Coord shift = SetCoord(0,0,10);		// シフト量
	Coord expand = SetCoord(2,2,2);		// 拡大量

	body->RotBody(rot_ax,deg);			// Z軸周りに90度回転
	body->ShiftBody(shift);				// Z方向に10シフト
	body->ExpandBody(expand);			// BODYを2倍に拡大

	return KOD_TRUE;
}

// Function: SmpDivCurves
// Sample3(曲線を指定した分割数で分割した点を生成する)
//
// Parameter:
// *BodyList - BODYの実体が登録順にリストされている
// *ObjList - セレクションされたエンティティ情報
// PickCount - セレクションされたエンティティの数
// Prop[] - ユーザー指定プロパティ値
//
// Return:
// KOD_TRUE
int SmpDivCurves(BODYList *BodyList,OBJECTList *ObjList, int PickCount, double Prop[])
{
	if(!PickCount)	return KOD_ERR;		// セレクションされていなかったら、何もしない

	NURBS_Func	nfunc;					// NURBSを扱う関数集を呼び出す
	double green[3] = {0,1,0};			// 分割点表示の色(緑)

    int divnum = (int)Prop[0];          	// ユーザーステータスのprop1を分割数として読み込み
    Coord *div_pt = NewCoord1(divnum+2);	// 分割点の座標値を格納する変数の宣言とメモリー確保

	// セレクションした数だけループ
	for(int i=0;i<PickCount;i++){
		OBJECT *obj = (OBJECT *)ObjList->getData(i);			// i番目にセレクションされたエンティティの情報を得る
		BODY *body = (BODY *)BodyList->getData(obj->Body);		// i番目にセレクションされたBODYの実体を得る
		if(obj->Type == _NURBSC){								// i番目にセレクションされたエンティティがNURBS曲線なら
			int ptnum = nfunc.CalcDeltaPtsOnNurbsC(&body->NurbsC[obj->Num],divnum,div_pt);		// 分割点を求める
            for(int j=0;j<ptnum;j++){
                DrawPoint(div_pt[j],1,3,green);					// 分割点を表示
			}
		}
	}

    FreeCoord1(div_pt);     // 確保メモリーの解放

	return KOD_TRUE;
}

// Function: SmpDivCurves
// Sample4(任意の点からの曲面上の最近傍点を求める)
//
// Parameter:
// *BodyList - BODYの実体が登録順にリストされている
// *ObjList - セレクションされたエンティティ情報
// PickCount - セレクションされたエンティティの数
// Prop[] - ユーザー指定プロパティ値
//
// Return:
// 何も選択されていない：KOD_ERR,		正常終了：KOD_TRUE
int SmpNearestPt(BODYList *BodyList,OBJECTList *ObjList, int PickCount, double Prop[])
{
	if(!PickCount)	return KOD_ERR;		// セレクションされていなかったら、何もしない

	NURBS_Func	nfunc;					// NURBSを扱う関数集を呼び出す
	NURBSS *S;							// セレクションされた曲面へのポインタ
	Coord Q,Q_;							// 最近傍点格納用
	char mes[256];						// メッセージ出力用
	double blue[3] = {0,0,1};			// 最近傍点表示の色(青)
	double red[3] = {1,0,0};			// コントロールポイント表示の色(赤)
	double green[3] = {0,1,0};			// 最近傍点とコントロールポイントをつなぐ線の色(緑)

	OBJECT *obj = (OBJECT *)ObjList->getData(0);		// 一番最初にセレクションされたエンティティの情報を得る
	BODY *body = (BODY *)BodyList->getData(obj->Body);	// 一番最初にセレクションされたBODYの実体を得る

	// セレクションされた面がトリムされたNURBS曲面か，ただのNURBS曲面か調べ，NURBS曲面のみを取り出す
	// KODATUNOでは，IGESファイル読み込みの段階で全ての面/線がNURBS曲面/曲線へとされるため，
	// 次のような3つの条件分岐によって欲しい面を取り出すことができる
	if(obj->Type == _TRIMMED_SURFACE){
		S = body->TrmS[obj->Num].pts;	// トリム面の場合は，トリム前の元のNURBS曲面を取り出す
	}
	else if(obj->Type == _NURBSS){
		S = &body->NurbsS[obj->Num];	// ただのNURBS曲面の場合はそのままその曲面へのポインタを得る
	}
	else{
		return KOD_ERR;					// セレクションされた曲面がトリム面でもNURBS曲面でもでない場合は終了
	}

	// 任意の点を3つ用意
	Coord P[3];
	P[0] = SetCoord(0,0,100);
	P[1] = SetCoord(100,50,50);
	P[2] = SetCoord(0,100,-50);

	// 近傍点を得る
	for(int i=0;i<3;i++){
		DrawPoint(P[i],1,3,red);									// 任意点を描画
        int flag = nfunc.CalcIntersecPtNurbsPt(S,P[i],3,5,&Q);  	// 最近傍点算出
		if(flag == KOD_TRUE){										// 最近傍点が見つかったら
			Q_ = nfunc.CalcNurbsSCoord(S,Q.x,Q.y);					// 実空間座標へ変換し保存
			DrawPoint(Q_,1,3,blue);									// 最近傍点を描画
			DrawLine(Q_,P[i],1,green);								// 線分描画
			sprintf(mes,"%d:True(%lf,%lf,%lf)",i,Q_.x,Q_.y,Q_.z);
            GuiIF.SetMessage(mes);
		}
		else{									// 最近傍点が見つからなかったら
			sprintf(mes,"%d:False\n",i);		// エラーメッセージをコンソールに出力	
            GuiIF.SetMessage(mes);
		}
	}

	return KOD_TRUE;
}

// Function: SmpUVDivLine
// Sample5(曲面のUVパラメータで等間隔に分割した点を生成する)
//
// Parameter:
// *BodyList - BODYの実体が登録順にリストされている
// *ObjList - セレクションされたエンティティ情報
// PickCount - セレクションされたエンティティの数
// Prop[] - ユーザー指定プロパティ値
//
// Return:
// 何も選択されていない：KOD_ERR,		正常終了：KOD_TRUE
int SmpUVDivLine(BODYList *BodyList,OBJECTList *ObjList, int PickCount, double Prop[])
{
	if(!PickCount)	return KOD_ERR;		// セレクションされていなかったら、何もしない

	NURBS_Func	nfunc;					// NURBSを扱う関数集を呼び出す
	double green[3] = {0,1,0};			// 点表示の色(緑)

	OBJECT *obj = (OBJECT *)ObjList->getData(0);		// 一番最初にセレクションされたエンティティの情報を得る
	BODY *body = (BODY *)BodyList->getData(obj->Body);	// 一番最初にセレクションされたBODYの実体を得る

	// 本来であれば，Sample4に示した曲面のタイプによる条件分岐が必要であるが，これ以降のSampleではトリム面で決め打ちする
	if(obj->Type != _TRIMMED_SURFACE)	return KOD_ERR;	

	NURBSS *S = body->TrmS[obj->Num].pts;				// BODYからNURBS曲面を取り出す

	int u_divnum = (int)Prop[0];						// ユーザーステータスのprop1をu方向分割数として読み込み
	int v_divnum = (int)Prop[1];						// ユーザーステータスのprop2をv方向分割数として読み込み
	double u_val = (S->U[1] - S->U[0])/u_divnum;		// パラメトリック空間内でのu方向線分長を得る
	double v_val = (S->V[1] - S->V[0])/v_divnum;		// パラメトリック空間内でのv方向線分長を得る

	// u方向，v方向の各分割点における座標値を求める
	for(int i=0;i<=u_divnum;i++){
		for(int j=0;j<=v_divnum;j++){
			Coord P = nfunc.CalcNurbsSCoord(S,S->U[0]+u_val*i,S->V[0]+v_val*j);	// 指定した(u,v)の座標値を求める
			DrawPoint(P,1,3,green);						// 描画
		}
	}

	return KOD_TRUE;
}
