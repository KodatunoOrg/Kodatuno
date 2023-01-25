#include "SampleFunc.h"

// Function: Smp3xCAM
// スキャンパスを生成するシンプルな3xCAM
//
// Parameter:
// *S - 加工面（一つの面のみ）
// *C - 生成するスキャンパスのガイドカーブ
// R - ボールエンドミル半径
// N - スキャニングライン分割数(N < 100)
// H - 素材上面のZ座標
// D - Z方向分割数（粗加工用）
void Smp3xCAM(NURBSS *S,NURBSC *C,double R,int N,double H,int D)
{
    NURBS_Func	nf;                 // NURBS_Funcへのインスタンス
    double green[3] = {0,1,0};      // 描画するパスの色（緑）
    Coord plane_pt;                 // 分割する平面上の1点
    Coord plane_n;                  // 分割する平面の法線ベクトル
    Coord path_[2000];              // 一時格納用バッファ
    Coord ***path = NewCoord3(D+1,N+1,2000);    // 生成されたパスを格納
    int ptnum[100];                 // スキャンライン1本ごとの加工点数を格納
    int flag = 0;                   // ジグザグパス生成時の方向転換用フラグ

    // ガイドカーブに沿って垂直平面をシフトしていき，加工面との交点群を求めていく
	for(int i=0;i<=N;i++){
        double t = (double)i/(double)N;
		if(i==0) t += 0.0001;		// 特異点回避
		else if(i==N) t-= 0.0001;	// 特異点回避
        plane_pt = nf.CalcNurbsCCoord(C,t);     // 注目中の垂直平面上の1点
        plane_n = nf.CalcTanVecOnNurbsC(C,t);   // 注目中の垂直平面の法線ベクトル
        ptnum[i] = nf.CalcIntersecPtsPlaneSearch(S,plane_pt,plane_n,0.5,3,path_,2000,RUNGE_KUTTA);  // 交点群算出
        // 得られた交点群を，加工面法線方向に工具半径分オフセットさせた点を得る
		for(int j=0;j<ptnum[i];j++){
            Coord pt = nf.CalcNurbsSCoord(S,path_[j].x,path_[j].y);     // 工具コンタクト点
            Coord n = nf.CalcNormVecOnNurbsS(S,path_[j].x,path_[j].y);  // 法線ベクトル
            if(n.z < 0)	n = n*(-1);         // 法線ベクトルの向き調整
			path[D][i][j] = pt + n*R;		// 工具半径オフセット
		}
	}

	// 粗加工パス生成
	for(int i=0;i<D;i++){
		for(int j=0;j<N+1;j++){
			for(int k=0;k<ptnum[j];k++){
				double del = (H - path[D][j][k].z)/(double)D;
				double Z = H - del*(double)i;
				path[i][j][k] = SetCoord(path[D][j][k].x,path[D][j][k].y,Z);
			}
		}
	}

	// path描画
	for(int i=0;i<D+1;i++){
		for(int j=0;j<N+1;j++){
			for(int k=0;k<ptnum[j];k++){
				DrawPoint(path[i][j][k],1,1,green);
			}
		}
	}

    // NCコード生成(各条件は各自で変更ください)
	double ret = H + 10;		// リトラクトZ
	FILE *fp = fopen("demo.nc","w");
	fprintf(fp,"G92X0.0Y0.0Z0.0\n");
	fprintf(fp,"S4000M03\n");
	fprintf(fp,"F500\n");
	for(int i=0;i<D+1;i++){
		fprintf(fp,"G00X%.2lfY%.2lfZ%.2lf\n",path[i][0][0].x,path[i][0][0].y,ret);
		for(int j=0;j<N+1;j++){
			if(flag == 0){
				for(int k=0;k<ptnum[j];k++){
					fprintf(fp,"G01X%.2lfY%.2lfZ%.2lf\n",path[i][j][k].x,path[i][j][k].y,path[i][j][k].z);
				}
			}
			else{
				for(int k=ptnum[j]-1;k>=0;k--){
					fprintf(fp,"G01X%.2lfY%.2lfZ%.2lf\n",path[i][j][k].x,path[i][j][k].y,path[i][j][k].z);
				}
			}
			flag = 1 - flag;
		}
		fprintf(fp,"G01Z%.2lf\n",ret);
	}
	fprintf(fp,"G01Z%.2lf\n",ret);
	fprintf(fp,"M05\n");
	fprintf(fp,"M30\n");

	fclose(fp);
}
