// KODATUNOサンプルワーク

#include"Sample.h"

// 引数  
// *Body:BODYリストの先頭アドレス  
// *Obj:セレクションされたエンティティの情報  
// pickcount:セレクションされたエンティティ数
void SampleMain(BODY *Body,OBJECT *Obj,int pickcount)
{
	BODY *curr;		// 作業用BODY
	int i;

	// セレクションされた数だけループ
	for(i=0;i<pickcount;i++){
		curr = Body;
		curr = SearchBodyList(curr,Obj[i].Body);		// セレクションされたBODYが見つかるまでBODYリストをたどる

		if(Obj[i].Type == _TRIMMED_SURFACE){			// セレクションされたエンティティがトリム面
			ContourLine(&curr->NurbsS[Obj[i].Num]);		// 等高線を算出
		}
		
		else if(Obj[i].Type == _NURBSC){				// セレクションされたエンティティがNURBS曲線
			// 処理を追加
		}
	}
}

// サンプルとして平面とNURBS曲線の交線(交点群)を算出するプログラムを示す
// 平面をZ方向にシフトさせていき、各Z位置での交線を逐次求める
void ContourLine(NURBSS *NurbsS)
{
	NURBS_Func NFunc;				// NURBS関連の関数を集めたクラスのオブジェクトを生成
	Coord t[5000];					// 解
	int   num;						// 解の数
	int   v_divnum=10;				// NURBS曲面の交点群を求める際のvパラメータ分割数
	Coord pt=SetCoord(0,0,10);		// 平面上の1点を指定
	Coord nvec=SetCoord(0,0,1);		// 平面の法線ベクトルを指定
	double pcolor[3] = {0,1,0};		// 表示の色
	double tcolor[3] = {1,0,0};

	for(int i=-10;i<20;i++){
		if(i==0) continue;
		pt = SetCoord(0,0,3*(double)i);		// 平面をZ方向に徐々にシフトしていく
		fprintf(stderr,"%.3lf計算中...",pt.z);
		num = NFunc.CalcIntersecPtsPlaneSearch3(NurbsS,pt,nvec,0.5,t,5000);		// NURBS曲面と平面との交点群を交線追跡法で求める
		fprintf(stderr,"END\n");

		for(int i=0;i<num;i++){		// 交点の数だけループ
			if(i%5 == 0){
				Coord p = NFunc.CalcNurbsSCoord(NurbsS,t[i].x,t[i].y);			// 交点をパラメータ値から座標値へ変換
				Coord nt = NFunc.CalcNormVecOnNurbsS(NurbsS,t[i].x,t[i].y);		// 交点上の法線ベクトルを計算
				nt = MulCoord(nt,-1);											// 外向き法線ベクトルへ変換
				DrawPoint(p,1,3,pcolor);			// 交点を描画
				DrawVector(p,nt,3,1,tcolor);		// 法線ベクトルを描画
			}
		}
	}

}