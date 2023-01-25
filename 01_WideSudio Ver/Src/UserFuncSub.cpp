// UserSampleを記述
#include "UserFuncSub.h"

// Example 1 : NURBS曲面と、指定した平面の交線を求める(平面を逐次オフセットしていき、等高線を算出)
// 引数  
// *Body:BODYリストの先頭アドレス  
// *Obj:セレクションされたエンティティの情報  
// pickcount:セレクションされたエンティティ数
int ExecSampleFunc1(BODYList *BodyList,OBJECTList *ObjList,int pickcount,double prop[])
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
		if(obj->Type == _TRIMMED_SURFACE || obj->Type == _NURBSS){
				ContourLine(&body->NurbsS[obj->Num],prop[0],prop[1],prop[2],prop[3]);
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
//       upper_z：Nurbs曲面と交差するx-y平面のz高さの最大値
//       under_z：Nurbs曲面と交差するx-y平面のz高さの最小値
//       delta：Nurbs曲面と交差するx-y平面のz方向ピックフィード
//       roughness：交線の荒さ(0.1～1くら)
int ContourLine(NURBSS *NurbsS,double upper_z,double under_z,double delta,double roughness)
{
	NURBS_Func NFunc;				// NURBS関連の関数を集めたクラスのオブジェクトを生成
	Coord t[5000];					// 解
	int   num;						// 解の数
	Coord pt=SetCoord(0,0,10);		// 平面上の1点を指定
	Coord nvec=SetCoord(0,0,1);		// 平面の法線ベクトルを指定
	double pcolor[3] = {0,1,0};		// 表示の色
	double tcolor[3] = {1,0,0};

	int step = fabs(upper_z - under_z)/delta + 1;

	for(int i=0;i<step;i++){
		double z = under_z + delta*(double)i;
		pt = SetCoord(0,0,z);			// 平面をZ方向に徐々にシフトしていく
		char mes[256];
		sprintf(mes,"z=%.3lf  calculating...",z);
		SetMessage(mes);
		num = NFunc.CalcIntersecPtsPlaneSearch(NurbsS,pt,nvec,roughness,10,t,5000,BULIRSH_STOER);		// NURBS曲面と平面との交点群を交線追跡法で求める
		sprintf(mes,"num = %d",num);
		SetMessage(mes);

		for(int i=0;i<num;i++){		// 交点の数だけループ
			Coord p = NFunc.CalcNurbsSCoord(NurbsS,t[i].x,t[i].y);			// 交点をパラメータ値から座標値へ変換
			Coord nt = NFunc.CalcNormVecOnNurbsS(NurbsS,t[i].x,t[i].y);		// 交点上の法線ベクトルを計算
			nt = MulCoord(nt,-1);											// 外向き法線ベクトルへ変換
			DrawPoint(p,1,3,pcolor);			// 交点を描画
			DrawVector(p,nt,3,1,tcolor);		// 法線ベクトルを描画
		}
	}
	SetMessage("END");

	return KOD_TRUE;
}


// Example 2 : NURBS曲面同士の交線を算出する
// 引数  
// *Body:BODYリストの先頭アドレス  
// *Obj:セレクションされたエンティティの情報  
// pickcount:セレクションされたエンティティ数
// prop: prop[0]:初期点サーチ時の分割数   prop[1]:交線追跡幅
int ExecSampleFunc2(BODYList *BodyList,OBJECTList *ObjList,int pickcount,double prop[])
{
	if(!pickcount)	return KOD_ERR;

	BODY *body;
	OBJECT *obj;
	TRIMD_NURBSS *tnurbs[100];	// NURBS曲面へのポインタを格納
	int surf_body[100];
	int surf_type[100];			// 曲面のタイプを格納
	int surf_objn[100];
	int scount=0;				// 曲面数カウンタ
	NURBS_Func NFunc;			// NURBS関連の関数を集めたクラスのオブジェクトを生成
	Coord Rt[5000];				// NURBS曲面R(w,t)における解
	Coord St[5000];				// NURBS曲面S(u,v)における解
	double green[3] = {0,1,0};	// 表示の色
	int flag_r;					// トリム判別フラグ
	int flag_s;					// 

	SetMessage("Start calcu....");

	// セレクションされた数だけループ
	// 各曲面が"トリム面"なのか"トリムがないただのNURBS曲面"なのか調べる
	for(int i=0;i<pickcount;i++){
		obj = (OBJECT *)ObjList->getData(i);
		body = (BODY *)BodyList->getData(obj->Body);		// セレクションされたBODYデータを得る

		if(obj->Type != _TRIMMED_SURFACE && obj->Type != _NURBSS)	// セレクションされたエンティティがトリム面でもNURBS曲面でもない場合
			continue;

		else if(obj->Type == _TRIMMED_SURFACE)				// セレクションされたエンティティがトリム面
			tnurbs[scount] = &body->TrmS[obj->Num];			// セレクションされたNURBS曲面を一時登録

		else if(obj->Type == _NURBSS)						// セレクションされたエンティティがNURBS曲面
			tnurbs[scount]->pts = &body->NurbsS[obj->Num];	// セレクションされたNURBS曲面を一時登録
		surf_body[scount] = obj->Body;
		surf_type[scount] = obj->Type;						// 注目中の面のタイプを記憶
		surf_objn[scount] = obj->Num;
		scount++;											// 面をカウント
		if(scount == 100){
			SetMessage("USER FUNC ERROR: Too Many Surfaces");
			return KOD_ERR;
		}
	}

	// 選択された面全ての組み合わせで交線算出
	for(int i=0;i<scount-1;i++){
		for(int j=i+1;j<scount;j++){
			if(surf_body[i] == surf_body[j])	continue;
			fprintf(stderr,"%d-(%d,%d)\n",scount,i,j);
			int num = NFunc.CalcIntersecPtsNurbsSSearch(tnurbs[i]->pts,tnurbs[j]->pts,(int)prop[0],prop[1],Rt,St,5000);	// NURBS曲面同士の交線算出
			// 描画
			for(int k=0;k<num;k++){
				flag_r = flag_s = KOD_TRUE;
				if(surf_type[i] == _TRIMMED_SURFACE)								// NurbsRがトリム面
					flag_r = NFunc.DetermPtOnTRMSurf(tnurbs[i],Rt[k].x,Rt[k].y);	// 注目中の交点のトリム面内外判別
				if(surf_type[j] == _TRIMMED_SURFACE)								// NurbsSがトリム面
					flag_s = NFunc.DetermPtOnTRMSurf(tnurbs[j],St[k].x,St[k].y);	// 注目中の交点のトリム面内外判別
				if(flag_r == KOD_TRUE && flag_s == KOD_TRUE){						// NurbsR,NurbsS両方ともflag==TRUEのときのみ、交点として表示
					Coord p = NFunc.CalcNurbsSCoord(tnurbs[i]->pts,Rt[k].x,Rt[k].y);// uv座標上の点をxyz座標値に変換
					DrawPoint(p,1,6,green);											// 描画
				}
			}
			fprintf(stderr,"done\n");
		}
	}

	SetMessage("END");

	return KOD_TRUE;
}
