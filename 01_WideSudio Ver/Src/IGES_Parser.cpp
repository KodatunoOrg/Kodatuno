/*************************
* IGESファイルを読み込む *
**************************/

#include "IGES_Parser.h"

// *body --- 立体を構成するエンティティの集合オブジェクトへのポインタ
// TypeNum[] --- 各エンティティの数が格納される
int IGES_PARSER::IGES_Parser_Main(BODY *body,char IGES_fname[])
{
	FILE *fp;
	GlobalParam gpara;				// グローバル部のパラメータを格納
	DirectoryParam *dpara;			// ディレクトリ部のパラメータを格納
	char mes[256];					// メッセージダンプ用string
	int  line[SECTION_NUM];			// 各セクション毎のライン数を格納
	int  flag = 0;
	int  i;

	// IGESファイルオープン
	if((fp = fopen(IGES_fname,"r")) == NULL){
		sprintf(mes,"KOD_ERROR: Cannot open %s",IGES_fname);
		SetMessage(mes);
		return(KOD_ERR);
	}
	sprintf(mes,"Open %s",IGES_fname);
	SetMessage(mes);

	// 各セクションの行数をあらかじめ取得
	GetSectionLine(fp,line);

	// DirectoryParamのメモリー確保
	line[SECTION_DIRECTORY] /= 2;		// ディレクトリ部は、2行で1つのシーケンスを構成するので2で割ったものをディレクトリ部のライン数とする
	dpara = (DirectoryParam *)malloc(sizeof(DirectoryParam)*line[SECTION_DIRECTORY]);
	if(dpara == NULL){
		SetMessage("KOD_ERROR: fail to allocate DirectoryParam");
		return(KOD_ERR);
	}

	// IGESファイル読み込み(各セクション毎に処理)
	for(i=0;i<SECTION_NUM;i++){
		if(i == SECTION_START){					// スタート部読み込み
			flag = GetStartSection(fp,line[i]);
		}
		else if(i == SECTION_GLOBAL){			// グローバル部読み込み
			flag = GetGlobalSection(fp,&gpara,line[i]);
		}
		else if(i == SECTION_DIRECTORY){		// ディレクトリ部読み込み
			flag = GetDirectorySection(fp,dpara,body->TypeNum,line[i]);
		}
		else if(i == SECTION_PARAMETER){			// パラメータ部読み込み
			hbody.New_body(body,body->TypeNum);		// BODY構造体内の各エンティティのメモリー確保
			flag = GetParameterSection(fp,dpara,*body,line[SECTION_DIRECTORY]);
		}
		else if(i == SECTION_TERMINATE){		// ターミネート部読み込み
			flag = GetTerminateSection(fp);
		}
		if(flag == KOD_ERR){
			hbody.Free_body(body,TypeCount);			// 今まで確保した分のメモリーを解放
			return(KOD_ERR);
		}
	}

	ChangeEntityforNurbs(dpara,*body,line[SECTION_DIRECTORY]);	// 内部表現を全てNURBSに変更する

	flag = SearchMaxCoord(body,body->TypeNum);		// 立体の最大座標値を探索(初期表示での表示倍率を決定するため)

	fclose(fp);

	free(dpara);

	return flag;
}

// 縮退チェック
int IGES_PARSER::CheckDegenracy(BODY *body)
{
	int flag;
	NURBS_Func NFunc;

	// 縮退用Nurbs曲線を複合曲線の数だけ生成
	if(body->TypeNum[_COMPOSITE_CURVE]){
		double T[4] = {0,0,NORM_KNOT_VAL,NORM_KNOT_VAL};
		double W[2] = {1,1};
		double V[2] = {0,NORM_KNOT_VAL};
		int prop[4] = {0,0,1,0};
		Coord cp[2];
		InitCoord(&cp[0]);
		InitCoord(&cp[1]);

		for(int i=0;i<body->TypeNum[_COMPOSITE_CURVE];i++){
			 NFunc.GenNurbsC(&body->CompC[i].DegeNurbs,2,2,4,T,W,cp,V,prop,1);	// 縮退用Nurbs曲線を複合曲線のエンティティ数だけ生成する

			// 各複合曲線がNURBS曲線のみで構成されておりかつ2Dパラメトリック要素であるかのチェック
			flag = 0;
			for(int j=0;j<body->CompC[i].N;j++){
				if(body->CompC[i].DEType[j] == NURBS_CURVE && body->CompC[i].pDE[j]->NurbsC.EntUseFlag == PARAMETRICELEM){
					flag++;				
				}
			}

			// NURBS曲線で構成されている複合曲線に対して、始点と終点の座標値を比較
			if(flag == body->CompC[i].N){
				Coord s,e;
				s = NFunc.CalcNurbsCCoord(&body->CompC[i].pDE[0]->NurbsC,body->CompC[i].pDE[0]->NurbsC.V[0]);					// 始点
				e = NFunc.CalcNurbsCCoord(&body->CompC[i].pDE[body->CompC[i].N-1]->NurbsC,body->CompC[i].pDE[body->CompC[i].N-1]->NurbsC.V[1]);	// 終点
				if(DiffCoord(s,e,1.0E-5) == KOD_FALSE){				// 始点≠終点
					body->CompC[i].DegeNurbs.cp[0] = e;
					body->CompC[i].DegeNurbs.cp[1] = s;
					body->CompC[i].DegeFlag = KOD_FALSE;			// 縮退ありのフラグを立てる
				}
				else{
					body->CompC[i].DegeFlag = KOD_TRUE;				// 縮退なしのフラグを立てる
				}
			}
			else{
				body->CompC[i].DegeFlag = KOD_TRUE;					// 複合曲線がNurbs曲線で構成されていない場合も縮退なしのフラグ
			}
		}
	}

	return KOD_TRUE;
}

// パラメトリック平面内のトリム曲線同士のつながりをチェック、修正する
int IGES_PARSER::ModifyParamConect(BODY *body)
{
	NURBSC *bc,*nc;

	// トリム曲面
	for(int i=0;i<body->TypeNum[_TRIMMED_SURFACE];i++){
		// 外側トリム
		for(int j=1;j<body->TrmS[i].pTO->pB->CompC.N;j++){
			bc = (NURBSC *)body->TrmS[i].pTO->pB->CompC.pDE[j-1];
			nc = (NURBSC *)body->TrmS[i].pTO->pB->CompC.pDE[j];
			if(DiffCoord2D(bc->cp[bc->K-1],nc->cp[0]) == KOD_FALSE)
				nc->cp[0] = SetCoord(bc->cp[bc->K-1]);
		}
		// 内側トリム
		for(int j=0;j<body->TrmS[i].n2;j++){
			for(int k=1;k<body->TrmS[i].pTI[j]->pB->CompC.N;k++){
				bc = (NURBSC *)body->TrmS[i].pTI[j]->pB->CompC.pDE[k-1];
				nc = (NURBSC *)body->TrmS[i].pTI[j]->pB->CompC.pDE[k];
				if(DiffCoord2D(bc->cp[bc->K-1],nc->cp[0]) == KOD_FALSE)
					nc->cp[0] = SetCoord(bc->cp[bc->K-1]);
			}
		}
	}

	return KOD_TRUE;
}

// ノットベクトルの正規化Sub1
int IGES_PARSER::ChangeKnotVecRange(double Range[],double Knot[],int N,int M,int K)
{
	double _t[KNOTNUMMAX];
	for(int i=0;i<N;i++){
		_t[i] = ChangeKnot(Knot[i],Knot[M-1],Knot[K]);
	}
	for(int i=0;i<N;i++){
		Knot[i] = _t[i];
	}
	Range[0] = 0;
	Range[1] = NORM_KNOT_VAL;

	return KOD_TRUE;
}

// ノットベクトルの正規化Sub2
double IGES_PARSER::ChangeKnot(double Knot,double M_,double K_)
{
	return NORM_KNOT_VAL*(Knot - M_)/(K_-M_);
}


// ノットの正規化
// 0.0001以下くらいの微小変化をOpenGLが認識しないため、ノット間隔を広く取り、0.0001以上の間隔
// になるようにする。ただし現時点では全てのノット間隔を一律0～NORM_KNOT_VALしており、無駄である。
// 今後ノット間隔が0.0001以下の場合のみノット間隔を広げるようにするべき。(2011/10)
int IGES_PARSER::NormalizeKnotRange(BODY *body)
{
	// トリム面
	for(int i=0;i<body->TypeNum[_TRIMMED_SURFACE];i++){
		int M0 = body->TrmS[i].pts->M[0];
		int M1 = body->TrmS[i].pts->M[1];
		int K0 = body->TrmS[i].pts->K[0];
		int K1 = body->TrmS[i].pts->K[1];
		// トリム面のパラメトリック平面における外側トリム曲線の正規化
		for(int j=0;j<body->TrmS[i].pTO->pB->CompC.N;j++){
			NURBSC *nc = (NURBSC *)body->TrmS[i].pTO->pB->CompC.pDE[j];
			for(int k=0;k<nc->K;k++){
				nc->cp[k].x = ChangeKnot(nc->cp[k].x,body->TrmS[i].pts->S[M0-1],body->TrmS[i].pts->S[K0]);
				nc->cp[k].y = ChangeKnot(nc->cp[k].y,body->TrmS[i].pts->T[M1-1],body->TrmS[i].pts->T[K1]);
			}
			ChangeKnotVecRange(nc->V,nc->T,nc->N,nc->M,nc->K);
		}
		// トリム面のパラメトリック平面における内側トリム曲線の正規化
		for(int j=0;j<body->TrmS[i].n2;j++){
			for(int k=0;k<body->TrmS[i].pTI[j]->pB->CompC.N;k++){
				NURBSC *nc = (NURBSC *)body->TrmS[i].pTI[j]->pB->CompC.pDE[k];
				for(int l=0;l<nc->K;l++){
					nc->cp[l].x = ChangeKnot(nc->cp[l].x,body->TrmS[i].pts->S[M0-1],body->TrmS[i].pts->S[K0]);
					nc->cp[l].y = ChangeKnot(nc->cp[l].y,body->TrmS[i].pts->T[M1-1],body->TrmS[i].pts->T[K1]);
				}
				ChangeKnotVecRange(nc->V,nc->T,nc->N,nc->M,nc->K);
			}
		}
		// ノットベクトルの範囲を正規化する
		ChangeKnotVecRange(body->TrmS[i].pts->U,body->TrmS[i].pts->S,body->TrmS[i].pts->N[0],M0,K0);
		ChangeKnotVecRange(body->TrmS[i].pts->V,body->TrmS[i].pts->T,body->TrmS[i].pts->N[1],M1,K1);
	}

	// NURBS曲線
	for(int i=0;i<body->TypeNum[_NURBSC];i++){
		if(body->NurbsC[i].EntUseFlag == 5) continue;	// 実空間上の曲線のみ変更
		ChangeKnotVecRange(body->NurbsC[i].V,body->NurbsC[i].T,body->NurbsC[i].N,body->NurbsC[i].M,body->NurbsC[i].K);
	}

	return KOD_TRUE;
}


// NURBS曲線以外のエンティティをNURBS曲線に変換し、変換行列があれば座標変換を施す
int IGES_PARSER::ChangeEntityforNurbs(DirectoryParam *dpara,BODY body,int dline)
{
	bool flag;

	for(int i=0;i<dline;i++){
		flag = KOD_FALSE;
		// 円/円弧->NURBS曲線
		if(dpara[i].entity_type == CIRCLE_ARC){
			body.NurbsC[TypeCount[_NURBSC]].EntUseFlag = body.CirA[dpara[i].entity_count].EntUseFlag;	// ディレクトリ部の情報"Entity Use Flag"を得る(NURBSC)
			if(GetNurbsCFromCirA(TypeCount[_NURBSC],dpara[i].entity_count,body) == KOD_ERR) return KOD_ERR;		// 円/円弧パラメータからNURBS曲線パラメータを得る
			flag = KOD_TRUE;
		}
		// 線分->NURBS曲線
		else if(dpara[i].entity_type == LINE){
			body.NurbsC[TypeCount[_NURBSC]].EntUseFlag = body.Line[dpara[i].entity_count].EntUseFlag;	// ディレクトリ部の情報"Entity Use Flag"を得る(NURBSC)
			if(GetNurbsCFromLine(TypeCount[_NURBSC],dpara[i].entity_count,body) == KOD_ERR) return KOD_ERR;		// 線分パラメータからNURBS曲線パラメータを得る
			flag = KOD_TRUE;
		}
		// 円/円弧、直線以外の曲線エンティティが存在する場合は、新規に処理を追加してください

		if(flag == KOD_TRUE){													// NURBS変換されたエンティティに対して
			if(dpara[i].p_tm){												// 変換行列が存在する場合
				for(int j=0;j<TypeCount[_TRANSFORMATION_MATRIX];j++){		// 全ての変換行列タイプを調べる
					if(body.TMat[j].pD == dpara[i].p_tm){					// 対象となる変換行列タイプへのポインタ
						if(TransformNurbsC(TypeCount[_NURBSC],j,body) == KOD_ERR) return KOD_ERR;	// NURBS曲線を座標変換する
					}
				}
			}
			TypeCount[_NURBSC]++;											// NURBSCの数をインクリメント
		}
	}

	return KOD_TRUE;
}

// パラメータ部読み込み
int IGES_PARSER::GetParameterSection(FILE *fp,DirectoryParam *dpara,BODY body,int dline)
{
	int i,j;
	char str[COLUMN_MAX*5000];	// 文字列バッファ(5000行分確保)
	char *p;					// 文字列先頭判別用ポインタ
	int  pD;					// ディレクトリ部への逆ポインタの値

	// TypeCountの初期化
	for(i=0;i<ALL_ENTITY_TYPE_NUM;i++)
		TypeCount[i] = 0;

	// 全エンティティのパラメータをそれぞれのエンティティ構造体に格納していく
	for(i=0;i<dline;i++){
		// ディレクトリ部14フィールドの情報を元に、strに各パラメータ部のライン数分を繋ぎ合わせる
		strcpy(str,"");
		for(j=0;j<dpara[i].param_line_count;j++){
			fgets(buf,COLUMN_MAX_,fp);
			if((p = strchr(buf,';')) == NULL){
				p = strchr(buf,' ');
			}
			else{
				buf[p-buf] = '\0';		// レコードデリミタ検出時は、レコードデリミタ部を終端文字にする
			}
			strncat(str,buf,p-buf+1);	// 文字列を各パラメータ部のライン数分繋ぎ合わせていく
		}
		p = &buf[COL_P_DIRECTORY];		// ディレクトリ部への逆ポインタの値をあらかじめ調べておく（便宜上）
		sscanf(p,"%d",&pD);

		// strを分解し各エンティティ構造体に情報を埋めていく
		// 他のエンティティを追加する場合は以下にコードを追加する

		// 円・円弧(NURBS曲線としてのエンティティ情報も同時に得る)
		if(dpara[i].entity_type == CIRCLE_ARC){							
			if(GetCirAPara(str,pD,dpara,body) == KOD_ERR)  return KOD_ERR;					// 円/円弧パラメータの取得
			body.CirA[TypeCount[_CIRCLE_ARC]].EntUseFlag = dpara[i].useflag_stat;	// ディレクトリ部の情報"Entity Use Flag"を得る
			dpara[i].entity_count = TypeCount[_CIRCLE_ARC];							// dparaとbodyを関連付ける
			TypeCount[_CIRCLE_ARC]++;					// 円・円弧タイプの数をインクリメント
		}
		// 複合曲線
		else if(dpara[i].entity_type == COMPOSITE_CURVE){					
			if(GetCompCPara(str,pD,dpara,dline,body) == KOD_ERR)  return KOD_ERR;
			dpara[i].entity_count = TypeCount[_COMPOSITE_CURVE];					// dparaとbodyを関連付ける
			TypeCount[_COMPOSITE_CURVE]++;				// 複合曲線タイプの数をインクリメント
		}
		// 円錐曲線
		else if(dpara[i].entity_type == CONIC_ARC){											
			if(GetConAPara(str,pD,dpara,body) == KOD_ERR)  return KOD_ERR;
			TypeCount[_CONIC_ARC]++;					// 円錐曲線タイプの数をインクリメント
		}
		// 線分(NURBS曲線としてのエンティティ情報も同時に得る)
		else if(dpara[i].entity_type == LINE){									
			if(GetLinePara(str,pD,dpara,body) == KOD_ERR)  return KOD_ERR;				// 線分パラメータの取得
			body.Line[TypeCount[_LINE]].EntUseFlag = dpara[i].useflag_stat;		// ディレクトリ部の情報"Entity Use Flag"を得る(LINE)
			dpara[i].entity_count = TypeCount[_LINE];							// dparaとbodyを関連付ける
			TypeCount[_LINE]++;							// 線分タイプの数をインクリメント
		}
		// 変換行列
		else if(dpara[i].entity_type == TRANSFORMATION_MATRIX){			
			if(GetTMatPara(str,pD,dpara,body) == KOD_ERR)  return KOD_ERR;
			dpara[i].entity_count = TypeCount[_TRANSFORMATION_MATRIX];			// dparaとbodyを関連付ける
			TypeCount[_TRANSFORMATION_MATRIX]++;		// 変換行列タイプの数をインクリメント
		}
		// NRBS曲線
		else if(dpara[i].entity_type == NURBS_CURVE){		
			if(GetNurbsCPara(str,pD,dpara,body) == KOD_ERR)  return KOD_ERR;
			body.NurbsC[TypeCount[_NURBSC]].EntUseFlag = dpara[i].useflag_stat;	// ディレクトリ部の情報"Entity Use Flag"を得る
			dpara[i].entity_count = TypeCount[_NURBSC];							// dparaとbodyを関連付ける
			TypeCount[_NURBSC]++;		// NRBS曲線タイプの数をインクリメント
		}
		// NRBS曲面
		else if(dpara[i].entity_type == NURBS_SURFACE){		
			if(GetNurbsSPara(str,pD,dpara,body) == KOD_ERR)  return KOD_ERR;
			dpara[i].entity_count = TypeCount[_NURBSS];							// dparaとbodyを関連付ける
			TypeCount[_NURBSS]++;		// NRBS曲面タイプの数をインクリメント
		}
		// 面上線
		else if(dpara[i].entity_type == CURVE_ON_PARAMETRIC_SURFACE){	
			if(GeConpSPara(str,pD,dpara,dline,body) == KOD_ERR)  return KOD_ERR;
			dpara[i].entity_count = TypeCount[_CURVE_ON_PARAMETRIC_SURFACE];	// dparaとbodyを関連付ける
			TypeCount[_CURVE_ON_PARAMETRIC_SURFACE]++;	// 面上線タイプの数をインクリメント
		}
		// トリム面	
		else if(dpara[i].entity_type == TRIMMED_SURFACE){				
			if(GetTrmSPara(str,pD,dpara,body) == KOD_ERR)  return KOD_ERR;
			dpara[i].entity_count = TypeCount[_TRIMMED_SURFACE];				// dparaとbodyを関連付ける
			TypeCount[_TRIMMED_SURFACE]++;				// トリム面タイプの数をインクリメント
		}
		// サポートしていないEntity Typeの場合
		else{
			char mes[256];
			sprintf(mes,"Entity Type #%d:Unsupported",dpara[i].entity_type);
			SetMessage(mes);
			continue;
		}
	}

	return KOD_TRUE;
}

// Type100 円・円弧の読み込み
int IGES_PARSER::GetCirAPara(char str[],int pD,DirectoryParam *dpara,BODY body)
{
//	int i;
	char *p;
	double x[3],y[3];

	p = str;

	body.CirA[TypeCount[_CIRCLE_ARC]].zt = CatchStringD(&p);		// Z軸方向の深さ
	x[0] = CatchStringD(&p);							// 中心座標X
	body.CirA[TypeCount[_CIRCLE_ARC]].cp[0].x = x[0];
	y[0] = CatchStringD(&p);							// 中心座標Y
	body.CirA[TypeCount[_CIRCLE_ARC]].cp[0].y = y[0];
	x[1] = CatchStringD(&p);							// 始点X
	body.CirA[TypeCount[_CIRCLE_ARC]].cp[1].x = x[1];
	y[1] = CatchStringD(&p);							// 始点Y
	body.CirA[TypeCount[_CIRCLE_ARC]].cp[1].y = y[1];
	x[2] = CatchStringD(&p);							// 終点X
	body.CirA[TypeCount[_CIRCLE_ARC]].cp[2].x = x[2];
	y[2] = CatchStringD(&p);							// 終点Y
	body.CirA[TypeCount[_CIRCLE_ARC]].cp[2].y = y[2];

	body.CirA[TypeCount[_CIRCLE_ARC]].R = sqrt((x[1]-x[0])*(x[1]-x[0])+(y[1]-y[0])*(y[1]-y[0]));	// 半径算出

	body.CirA[TypeCount[_CIRCLE_ARC]].pD = pD;		// ディレクトリ部への逆ポインタの値

	InitDisplayStat(&body.CirA[TypeCount[_CIRCLE_ARC]].Dstat);	// 表示属性の初期化

	return KOD_TRUE;
}

// Type104 円錐曲線の読み込み
int IGES_PARSER::GetConAPara(char str[],int pD,DirectoryParam *dpara,BODY body)
{
	SetMessage("Type104:Unmounted");
	return KOD_TRUE;
}

// Type110 線分の読み込み
int IGES_PARSER::GetLinePara(char str[],int pD,DirectoryParam *dpara,BODY body)
{
	char *p;

	p = str;

	body.Line[TypeCount[_LINE]].cp[0].x = CatchStringD(&p);		// 始点のX座標
	body.Line[TypeCount[_LINE]].cp[0].y = CatchStringD(&p);		// 始点のY座標
	body.Line[TypeCount[_LINE]].cp[0].z = CatchStringD(&p);		// 始点のZ座標
	body.Line[TypeCount[_LINE]].cp[1].x = CatchStringD(&p);		// 終点のX座標
	body.Line[TypeCount[_LINE]].cp[1].y = CatchStringD(&p);		// 終点のY座標
	body.Line[TypeCount[_LINE]].cp[1].z = CatchStringD(&p);		// 終点のZ座標

	body.Line[TypeCount[_LINE]].pD = pD;		// ディレクトリ部への逆ポインタの値

	InitDisplayStat(&body.Line[TypeCount[_LINE]].Dstat);	// 表示属性の初期化

	return KOD_TRUE;
}

// Type124 変換行列の読み込み
int IGES_PARSER::GetTMatPara(char str[],int pD,DirectoryParam *dpara,BODY body)
{
	int i,j;
	char *p;
	
	p = str;
	for(i=0;i<3;i++){
		for(j=0;j<4;j++){
			if(j != 3){
				body.TMat[TypeCount[_TRANSFORMATION_MATRIX]].R[i][j] = CatchStringD(&p);	// 3×3回転行列成分
			}
			else{
				body.TMat[TypeCount[_TRANSFORMATION_MATRIX]].T[i] = CatchStringD(&p);		// 並進ベクトル成分
			}
		}
	}
	
	body.TMat[TypeCount[_TRANSFORMATION_MATRIX]].pD = pD;		// DE部への逆ポインタの値
	
	return KOD_TRUE;
}

// Type126 NRBS曲線の読み込み
int IGES_PARSER::GetNurbsCPara(char str[],int pD,DirectoryParam *dpara,BODY body)
{
	char *p;
	int i=0;

	p = str;
	body.NurbsC[TypeCount[_NURBSC]].K = CatchStringI(&p) + 1;		// 総和記号の上側添字（コントロールポイント-1）の値
	body.NurbsC[TypeCount[_NURBSC]].M = CatchStringI(&p) + 1;		// 基底関数の階数
	body.NurbsC[TypeCount[_NURBSC]].N = body.NurbsC[TypeCount[_NURBSC]].K + body.NurbsC[TypeCount[_NURBSC]].M;	// ノットベクトルの数
	for(i=0;i<4;i++){	// ブーリアン型プロパティ4つ
		body.NurbsC[TypeCount[_NURBSC]].prop[i] = CatchStringI(&p);
	}

	// メモリー確保
	if(NFunc.New_NurbsC(&body.NurbsC[TypeCount[_NURBSC]],body.NurbsC[TypeCount[_NURBSC]].K,body.NurbsC[TypeCount[_NURBSC]].N) == KOD_ERR){
		SetMessage("PARAMETER SECTION KOD_ERROR:fail to allocate memory");
		return KOD_ERR;
	}

	for(i=0;i<body.NurbsC[TypeCount[_NURBSC]].N;i++){
		body.NurbsC[TypeCount[_NURBSC]].T[i] = CatchStringD(&p);	// ノットベクトルの値
	}
	for(i=0;i<body.NurbsC[TypeCount[_NURBSC]].K;i++){				// Weightの値
		body.NurbsC[TypeCount[_NURBSC]].W[i] = CatchStringD(&p);
	}
	for(i=0;i<body.NurbsC[TypeCount[_NURBSC]].K;i++){				// コントロールポイントの座標値
		body.NurbsC[TypeCount[_NURBSC]].cp[i].x = CatchStringD(&p);
		body.NurbsC[TypeCount[_NURBSC]].cp[i].y = CatchStringD(&p);
		body.NurbsC[TypeCount[_NURBSC]].cp[i].z = CatchStringD(&p);
	}
	body.NurbsC[TypeCount[_NURBSC]].V[0] = CatchStringD(&p);		// パラメータの範囲
	body.NurbsC[TypeCount[_NURBSC]].V[1] = CatchStringD(&p);

	// 法線ベクトルは記述されている場合とされていない場合があるようなので、記述されている場合のみ読み込む
	if(strchr(p,',') != NULL){
		body.NurbsC[TypeCount[_NURBSC]].norm.x = CatchStringD(&p);	// 法線ベクトル
		body.NurbsC[TypeCount[_NURBSC]].norm.y = CatchStringD(&p);
		body.NurbsC[TypeCount[_NURBSC]].norm.z = CatchStringD(&p);
	}

	body.NurbsC[TypeCount[_NURBSC]].pD = pD;		// DE部への逆ポインタの値

	InitDisplayStat(&body.NurbsC[TypeCount[_NURBSC]].Dstat);	// 表示属性の初期化

	return KOD_TRUE;
}

// Type128 NURBS曲面の読み込み
int IGES_PARSER::GetNurbsSPara(char str[],int pD,DirectoryParam *dpara,BODY body)
{
	char *p;
	int i=0,j=0;

	p = str;

	body.NurbsS[TypeCount[_NURBSS]].K[0] = CatchStringI(&p) + 1;	// u方向コントロールポイントの数
	body.NurbsS[TypeCount[_NURBSS]].K[1] = CatchStringI(&p) + 1;	// v方向コントロールポイントの数
	body.NurbsS[TypeCount[_NURBSS]].M[0] = CatchStringI(&p) + 1;	// 基底関数のu方向階数
	body.NurbsS[TypeCount[_NURBSS]].M[1] = CatchStringI(&p) + 1;	// 基底関数のv方向階数
	body.NurbsS[TypeCount[_NURBSS]].N[0] = body.NurbsS[TypeCount[_NURBSS]].K[0] + body.NurbsS[TypeCount[_NURBSS]].M[0];	// u方向ノットベクトルの数
	body.NurbsS[TypeCount[_NURBSS]].N[1] = body.NurbsS[TypeCount[_NURBSS]].K[1] + body.NurbsS[TypeCount[_NURBSS]].M[1];	// v方向ノットベクトルの数
	for(i=0;i<5;i++){
		body.NurbsS[TypeCount[_NURBSS]].prop[i] = CatchStringI(&p);	// ブーリアン型プロパティ5つ
	}

	// メモリー確保
	if(NFunc.New_NurbsS(&body.NurbsS[TypeCount[_NURBSS]],body.NurbsS[TypeCount[_NURBSS]].K,body.NurbsS[TypeCount[_NURBSS]].N) == KOD_ERR){
		SetMessage("PARAMETER SECTION KOD_ERROR:fail to allocate memory");
		return KOD_ERR;
	}
	
	for(i=0;i<body.NurbsS[TypeCount[_NURBSS]].N[0];i++){
		body.NurbsS[TypeCount[_NURBSS]].S[i] = CatchStringD(&p);	// u方向ノットベクトル
	}
	for(i=0;i<body.NurbsS[TypeCount[_NURBSS]].N[1];i++){
		body.NurbsS[TypeCount[_NURBSS]].T[i] = CatchStringD(&p);	// v方向ノットベクトル
	}
	for(i=0;i<body.NurbsS[TypeCount[_NURBSS]].K[1];i++){
		for(j=0;j<body.NurbsS[TypeCount[_NURBSS]].K[0];j++){
			body.NurbsS[TypeCount[_NURBSS]].W[j][i] = CatchStringD(&p);	//  u方向Weight
		}
	}
	for(i=0;i<body.NurbsS[TypeCount[_NURBSS]].K[1];i++){
		for(j=0;j<body.NurbsS[TypeCount[_NURBSS]].K[0];j++){
			body.NurbsS[TypeCount[_NURBSS]].cp[j][i].x = CatchStringD(&p);	// コントロールポイントX
			body.NurbsS[TypeCount[_NURBSS]].cp[j][i].y = CatchStringD(&p);	// コントロールポイントY
			body.NurbsS[TypeCount[_NURBSS]].cp[j][i].z = CatchStringD(&p);	// コントロールポイントZ
		}
	}
	body.NurbsS[TypeCount[_NURBSS]].U[0] = CatchStringD(&p);			// u方向の開始値
	body.NurbsS[TypeCount[_NURBSS]].U[1] = CatchStringD(&p);			// u方向の終了値
	body.NurbsS[TypeCount[_NURBSS]].V[0] = CatchStringD(&p);			// v方向の開始値
	body.NurbsS[TypeCount[_NURBSS]].V[1] = CatchStringD(&p);			// v方向の終了値

	body.NurbsS[TypeCount[_NURBSS]].pD = pD;		// DE部への逆ポインタの値

	body.NurbsS[TypeCount[_NURBSS]].TrmdSurfFlag = KOD_FALSE;	// とりあえずトリムされていない独立面としておく(Type144を読みに言ったときに変更される)

	hbody.ChangeStatColor(body.NurbsS[TypeCount[_NURBSS]].Dstat.Color,0.2,0.2,0.2,0.5);	// 曲面の色を設定

	return KOD_TRUE;
}

// Type102 複合曲線の読み込み
int IGES_PARSER::GetCompCPara(char str[],int pD,DirectoryParam *dpara,int dline,BODY body)
{
	char *p;
	int  pdnum;		// DE部のシーケンスナンバー取得用
	int  i;

	p = str;

	body.CompC[TypeCount[_COMPOSITE_CURVE]].N = CatchStringI(&p);	// 複合曲線の構成要素数

	// 複合曲線のメモリーを確保
	if(NFunc.New_CompC(&body.CompC[TypeCount[_COMPOSITE_CURVE]],body.CompC[TypeCount[_COMPOSITE_CURVE]].N) == KOD_ERR){
		SetMessage("PARAMETER SECTION KOD_ERROR:fail to allocate memory");
		return KOD_ERR;
	}

	for(i=0;i<body.CompC[TypeCount[_COMPOSITE_CURVE]].N;i++){		// 構成要素のDE部へのポインタ値
		pdnum = CatchStringI(&p);		// 各構成要素のDE部のシーケンスナンバーを得る
		body.CompC[TypeCount[_COMPOSITE_CURVE]].DEType[i] = SearchEntType(dpara,pdnum,dline);		// pdnumが示すエンティティタイプを判別
		body.CompC[TypeCount[_COMPOSITE_CURVE]].pDE[i] = (COMPELEM *)GetDEPointer(pdnum,body);		// pdnumが示す構造体のポインタを得る
	}

	body.CompC[TypeCount[_COMPOSITE_CURVE]].pD = pD;		// DE部への逆ポインタの値

	return KOD_TRUE;
}

// Type142 面上線の読み込み
int IGES_PARSER::GeConpSPara(char str[],int pD,DirectoryParam *dpara,int dline,BODY body)
{
	char *p;
	int pdnum;		// DE部のシーケンスナンバー取得用

	p = str;

	body.ConpS[TypeCount[_CURVE_ON_PARAMETRIC_SURFACE]].crtn = CatchStringI(&p);	// 面上線がどのように作られたかを表す

	pdnum = CatchStringI(&p);			// Curveが乗るSurfaceのDE部のシーケンスナンバーを得る
	body.ConpS[TypeCount[_CURVE_ON_PARAMETRIC_SURFACE]].SType = SearchEntType(dpara,pdnum,dline);	// pdnumが示すエンティティタイプを判別
	body.ConpS[TypeCount[_CURVE_ON_PARAMETRIC_SURFACE]].pS = (NURBSS *)GetDEPointer(pdnum,body);		// pdnumが示す構造体のポインタを得る

	pdnum = CatchStringI(&p);			// Surfaceのパラメータ空間におけるcurveを定義するEntityのDE部のシーケンスナンバーを得る
	body.ConpS[TypeCount[_CURVE_ON_PARAMETRIC_SURFACE]].BType = SearchEntType(dpara,pdnum,dline);	// pdnumが示すエンティティタイプを判別
	body.ConpS[TypeCount[_CURVE_ON_PARAMETRIC_SURFACE]].pB = (CURVE *)GetDEPointer(pdnum,body);		// pdnumが示す構造体のポインタを得る(共用体)

	pdnum = CatchStringI(&p);			// Curve CのDE部へのポインタ
	body.ConpS[TypeCount[_CURVE_ON_PARAMETRIC_SURFACE]].CType = SearchEntType(dpara,pdnum,dline);	// pdnumが示すエンティティタイプを判別
	body.ConpS[TypeCount[_CURVE_ON_PARAMETRIC_SURFACE]].pC = (CURVE *)GetDEPointer(pdnum,body);		// pdnumが示す構造体のポインタを得る(共用体)

	body.ConpS[TypeCount[_CURVE_ON_PARAMETRIC_SURFACE]].pref = CatchStringI(&p);	// 送り側システムで採られていた表現を表すフラグ

	body.ConpS[TypeCount[_CURVE_ON_PARAMETRIC_SURFACE]].pD = pD;	// DE部のシーケンスナンバーを得る

	return KOD_TRUE;
}

// Type144 トリム面の読み込み
int IGES_PARSER::GetTrmSPara(char str[],int pD,DirectoryParam *dpara,BODY body)
{
	char *p;
	int  i;
	int  pdnum;		// DE部のシーケンスナンバー取得用

	p = str;
	
	pdnum = CatchStringI(&p);		// トリムされるSurface EntityのDE部の値を取得
	body.TrmS[TypeCount[_TRIMMED_SURFACE]].pts = (NURBSS *)GetDEPointer(pdnum,body);		// トリムされるSurface Entityへのポインタを取得
	body.TrmS[TypeCount[_TRIMMED_SURFACE]].pts->TrmdSurfFlag = KOD_TRUE;		// トリム面としてのNURBS曲面であることを示す
	body.TrmS[TypeCount[_TRIMMED_SURFACE]].n1 = CatchStringI(&p);		// ０：外周がDの境界と一致している　１：それ以外
	body.TrmS[TypeCount[_TRIMMED_SURFACE]].n2 = CatchStringI(&p);		// Trimmed Surfaceの内周の単純閉曲線の数

	pdnum = CatchStringI(&p);		// Trimmed Surfaceの外周の単純閉曲線の数
	body.TrmS[TypeCount[_TRIMMED_SURFACE]].pTO = (CONPS *)GetDEPointer(pdnum,body); // 単純閉曲線構造体へのポインタを取得

	// 単純閉曲線N2の数だけメモリー確保
	if((NFunc.New_TrmS(&body.TrmS[TypeCount[_TRIMMED_SURFACE]],body.TrmS[TypeCount[_TRIMMED_SURFACE]].n2)) == KOD_ERR){
		SetMessage("PARAMETER SECTION KOD_ERROR:fail to allocate memory");
		return KOD_ERR;
	}

	for(i=0;i<body.TrmS[TypeCount[_TRIMMED_SURFACE]].n2;i++){
		pdnum = CatchStringI(&p);	// Trimmed Surfaceの内周の単純閉曲線のDE部の値を取得
		body.TrmS[TypeCount[_TRIMMED_SURFACE]].pTI[i] = (CONPS *)GetDEPointer(pdnum,body);	// 単純閉曲線構造体へのポインタを取得
	}

	body.TrmS[TypeCount[_TRIMMED_SURFACE]].pD = pD;		// DE部のシーケンスナンバーを得る

	return KOD_TRUE;
}


// ディレクトリ部読み込み
int IGES_PARSER::GetDirectorySection(FILE *fp,DirectoryParam *dpara,int TypeNum[],int dline)
{
	int i,j;
	char *p;						// 文字列先頭判別用ポインタ
	char str[COLUMN_MAX*2+1];		// 2行分（1エンティティ分）の文字列配列
	char field[FIELD_NUM+1];		// 8文字で1フィールド
	char dmy;

	for(i=0;i<ALL_ENTITY_TYPE_NUM;i++){
		TypeNum[i] = 0;			// 初期化
	}

	for(i=0;i<dline;i++){
		strcpy(str,"");				// str初期化

		if(fgets(buf,COLUMN_MAX_,fp) == NULL){		// i番目のエンティティの1行目を読み込み
			SetMessage("DIRECTORY SECTION KOD_ERROR: fail to read this file");
			exit(KOD_ERR);
		}
		strncat(str,buf,COLUMN_MAX);
		if(fgets(buf,COLUMN_MAX_,fp) == NULL){		// i番目のエンティティの2行目を読み込み
			SetMessage("DIRECTORY SECTION KOD_ERROR: fail to read this file");
			exit(KOD_ERR);
		}
		strncat(str,buf,COLUMN_MAX);				// 読み込んだ2行はstrに全て格納される

		p = str;									// pをまずstrの先頭にする
		for(j=0;j<DIRECTORYPARANUM;j++){			// ディレクトリ部のパラメータの数分だけループ
			strncpy(field,p,8);						// pの先頭8文字をfieldへ格納
			field[FIELD_NUM] = '\0';				// 一応、終端文字をfieldのお尻につけておく
			p += FIELD_NUM;							// pを次のフィールドの先頭へ移動
			// ディレクトリ部の情報が必要な場合は以下にコードを追加する
			if(j == ENTITY_TYPE_NUM){					// 要素番号を取得
				dpara[i].entity_type = atoi(field);
				GetType(dpara[i].entity_type,TypeNum);	// エンティティタイプの数を加算
			}
			else if(j == PARAM_DATA){					// パラメータ部へのポインタを取得
				dpara[i].p_param = atoi(field);
			}
			else if(j == TRAN_MATRIX){					// マトリックスへのポインタを取得
				dpara[i].p_tm = atoi(field);
			}
			else if(j == STATUS_NUM){					// ステータスを取得
				GetStatusNumber(field,&dpara[i]);
			}
			else if(j == SEQUENCE_NUM){					// シーケンス番号を取得
				sscanf(field,"%c %d",&dmy,&dpara[i].seq_num);
			}
			else if(j == PARAM_LINE_COUNT){				// パラメータ部のライン数
				dpara[i].param_line_count = atoi(field);
			}
		}
	}
	
	return KOD_TRUE;
}

// エンティティタイプの数を加算する
void IGES_PARSER::GetType(int type,int entitynum[])
{
	int i;

	// 直線または円・円弧エンティティの場合はNURBS曲線エンティティも同時にインクリメント
	if(type == LINE || type == CIRCLE_ARC){
		entitynum[_NURBSC]++;
	}

	for(i=0;i<ALL_ENTITY_TYPE_NUM;i++){
		if(type == entity[i]){
			entitynum[i]++;		// iはenum型EntityTypに対応 ex) entitynum[10]は_NURBSC（有理Bスプライン曲線）の数を表す
		}
	}
}

// DE#9(ステータス)部の読み込み
void IGES_PARSER::GetStatusNumber(char field[],DirectoryParam *dpara)
{
	char str[3]="";
	char *p;

	p = field;
	strncpy(str,p,2);
	sscanf(str,"%d",&dpara->blank_stat);
	p += 2;
	strncpy(str,p,2);
	sscanf(str,"%d",&dpara->subordinate_stat);
	p += 2;
	strncpy(str,p,2);
	sscanf(str,"%d",&dpara->useflag_stat);
	p += 2;
}

// グローバル部読み込み
int IGES_PARSER::GetGlobalSection(FILE *fp,GlobalParam *gpara,int gline)
{
	char *str;					// グローバル部文字列全てを格納する
	char para_delim = ',';		// パラメータデリミタ
	char record_delim = ';';	// レコードデリミタ
	int  para_length;			// 各パラメータの文字列の長さを格納
	char *p;
	int  i;

	// グローバル部のライン数*COL_CHAR分メモリー確保
	str = (char *)malloc(sizeof(char) * gline*COL_CHAR);
	if(str == NULL){
		SetMessage("GLOBAL SECTION KOD_ERROR: faile to allocate memory");
		exit(KOD_ERR);
	}

	strcpy(str,"");			// str初期化
	for(i=0;i<gline;i++){
		if(fgets(buf,COLUMN_MAX_,fp) == NULL){
			SetMessage("GLOBAL SECTION KOD_ERROR: faile to read this file");
			exit(KOD_ERR);
		}
		strncat(str,buf,COL_CHAR-1);	// strにセクション判別文字までの文字列をつけたしていく
	}

	// パラメータデリミタとレコードデリミタのチェック
	sscanf(str,"%dH%c",&para_length,&para_delim);		// パラメータデリミタを取得
	if(para_delim != ','){
		SetMessage("GLOBAL SECTION KOD_ERROR: The parameter delimiter is not governed by and construed for JAMA-IS");
		exit(KOD_ERR);
	}
	else{
		sscanf(str,"%dH[^,],[^,],%dH%c",&para_length,&para_length,&record_delim);		// レコードデリミタを取得
		if(record_delim != ';'){
			SetMessage("GLOBAL SECTION KOD_ERROR: The record delimiter is not governed by and construed for JAMA-IS");
			exit(KOD_ERR);
		}
	}

	p = str;
	p += 4;		// pをstrの5文字目（パラメータデリミタ定義部の次）まで移動
	p = strtok(p,",");
	for(i=3;i<=GLOBALPARAMNUM;i++){		// 2つのデリミタを抜かした残りのパラメータを逐次読み込む
		if((p = strtok(NULL,",")) == NULL && i<GLOBALPARAMNUM){
			SetMessage("GLOBAL SECTION KOD_ERROR: Low parameter count of global section");
			exit(KOD_ERR);
		}

		// グローバル部の情報が必要な場合は以下にコードを記入
		if(i == MODEL_SCALE){				// モデルスケール読み込み
			gpara->scale = atof(p);
		}
		else if(i == UNIT_FLAG){			// 単位フラグ読み込み
			gpara->unit_flag = atoi(p);
		}
		else if(i == MODEL_SPACE_SIZE){		// モデル空間の大きさ読み込み
			gpara->space_size = atof(p);
		}
	}

	free(str);		// メモリー開放

	return KOD_TRUE;
}

// スタート部読み込み
int IGES_PARSER::GetStartSection(FILE *fp,int sline)
{
	int i;

	for(i=0;i<sline;i++){
		if(fgets(buf,COLUMN_MAX_,fp) == NULL){
			SetMessage("START SECTION KOD_ERROR:fail to read this file");
			exit(KOD_ERR);
		}

		// スタート部の情報が必要な場合は以下にコードを追加する
	}

	return KOD_TRUE;
}

// ターミネート部読み込み
int IGES_PARSER::GetTerminateSection(FILE *fp)
{
	return KOD_TRUE;
}

// 各セクションのライン数を調べる
void IGES_PARSER::GetSectionLine(FILE *fp,int line[])
{
	line[0] = line[1] = line[2] = line[3] = line[4] = 0;	// 初期化

	while(fgets(buf,COLUMN_MAX_,fp)){
		if(buf[COL_CHAR-1] == 'S'){
			line[SECTION_START]++;
		}
		else if(buf[COL_CHAR-1] == 'G'){
			line[SECTION_GLOBAL]++;
		}
		else if(buf[COL_CHAR-1] == 'D'){
			line[SECTION_DIRECTORY]++;
		}
		else if(buf[COL_CHAR-1] == 'P'){
			line[SECTION_PARAMETER]++;
		}
		else if(buf[COL_CHAR-1] == 'T'){
			line[SECTION_TERMINATE]++;
		}
		else{							// どのセクションにも属さない文字を検出
			SetMessage("KOD_ERROR: IGES FORMAT");
			exit(KOD_ERR);
		}
	}
	fseek(fp,0L,SEEK_SET);				// ファイル先頭に戻る

}

// カンマまでの数値を読み込んで返す(int)
int IGES_PARSER::CatchStringI(char **p)
{
	int a;

	if((*p = strchr(*p,',')) == NULL){
		SetMessage("KOD_ERROR:No governed by and construed for JAMA-IS");
		exit(KOD_ERR);
	}

	(*p)++;
	sscanf(*p,"%d[^,],",&a);

	return a;
}

// カンマまでの数値を読み込んで返す(double)
double IGES_PARSER::CatchStringD(char **p)
{
	double a;

	if((*p = strchr(*p,',')) == NULL){
		SetMessage("KOD_ERROR:No governed by and construed for JAMA-IS");
		exit(KOD_ERR);
	}

	(*p)++;
	sscanf(*p,"%lf[^,],",&a);

	return a;
}

// DE部へのポインタが示す実際の構造体へのポインタを返す
void *IGES_PARSER::GetDEPointer(int TypeNum,BODY body)
{
	int i,j;

	for(i=0;i<ALL_ENTITY_TYPE_NUM;i++){
		for(j=0;j<TypeCount[i];j++){
			if(i==_CIRCLE_ARC && body.CirA[j].pD == TypeNum){
				return &body.CirA[j];
			}
			else if(i==_COMPOSITE_CURVE && body.CompC[j].pD == TypeNum){
				return &body.CompC[j];
			}
			else if(i==_CONIC_ARC && body.ConA[j].pD == TypeNum){
				return &body.ConA[j];
			}
			else if(i==_LINE && body.Line[j].pD == TypeNum){
				return &body.Line[j];
			}
			else if(i==_TRANSFORMATION_MATRIX && body.TMat[j].pD == TypeNum){
				return &body.TMat[j];
			}
			else if(i==_NURBSC && body.NurbsC[j].pD == TypeNum){
				return &body.NurbsC[j];
			}
			else if(i==_NURBSS && body.NurbsS[j].pD == TypeNum){
				return &body.NurbsS[j];
			}
			else if(i==_CURVE_ON_PARAMETRIC_SURFACE && body.ConpS[j].pD == TypeNum){
				return &body.ConpS[j];
			}
			else if(i==_TRIMMED_SURFACE && body.TrmS[j].pD == TypeNum){
				return &body.TrmS[j];
			}
		}
	}

	return NULL;
}

// DE部へのポインタの値からエンティティのタイプを調べて返す
int IGES_PARSER::SearchEntType(DirectoryParam *dpara,int pdnum,int dline)
{
	int i;

	for(i=0;i<dline;i++){
		if(dpara[i].seq_num == pdnum){
			return dpara[i].entity_type;
		}
	}

	return KOD_ERR;
}
// 全てのエンティティにおける座標値の最大値を調べる
int IGES_PARSER::SearchMaxCoord(BODY *body,int TypeNum[])
{
	int i,j;
	int temp=0;
	int bufnum=0;
	double *CoordBuf;
	
	// #100(円、円弧)、#110(線分)、#126(NURBS曲線)のコントロールポイントの座標値の個数を得る
	for(i=0;i<TypeNum[_NURBSC];i++){
		bufnum += 3*body->NurbsC[i].K;
	}
	
	// メモリ確保
	if((CoordBuf = (double*)malloc(sizeof(double)*bufnum)) == NULL){
		SetMessage("SEARCH MAXCOORD KOD_ERROR:fail to allocate memory");
		return KOD_ERR;
	}

	// #100(円、円弧)、#110(線分)、#126(NURBS曲線)のコントロールポイントの座標値を得る
	for(i=0;i<TypeNum[_NURBSC];i++){
		for(j=0;j<body->NurbsC[i].K;j++){
			CoordBuf[temp*3] = fabs(body->NurbsC[i].cp[j].x);	// コントロールポイントX
			CoordBuf[temp*3+1] = fabs(body->NurbsC[i].cp[j].y);	// コントロールポイントY
			CoordBuf[temp*3+2] = fabs(body->NurbsC[i].cp[j].z);	// コントロールポイントZ
			temp++;
		}
	}
	
	qsort(CoordBuf,bufnum,sizeof(double),QCmp);	// 全ての座標値をクイックソートにより降順にソート
	body->MaxCoord = CoordBuf[0];				// 最も大きい座標値を得る

	// メモリ解放
	free(CoordBuf);
	
	return KOD_TRUE;
}

// コンストラクタ
IGES_PARSER::IGES_PARSER()
{
		entity[0] = CIRCLE_ARC;							// 円/円弧
		entity[1] = COMPOSITE_CURVE;					// 複合曲線
		entity[2] = CONIC_ARC;							// 円錐曲線
		entity[3] = COPIOUS_DATA;						// 有意点列
		entity[4] = PLANE;								// 平面
		entity[5] = LINE;								// 線分
		entity[6] = PARAMETRIC_SPLINE_CURVE;			// パラメトリックスプライン曲線
		entity[7] = PARAMETRIC_SPLINE_SURFACE;			// パラメトリックスプライン曲面
		entity[8] = POINT;								// 点
		entity[9] = TRANSFORMATION_MATRIX;				// 変換行列
		entity[10] = NURBS_CURVE;						// 有理Bスプライン曲線
		entity[11] = NURBS_SURFACE;						// 有理Bスプライン曲面
		entity[12] = CURVE_ON_PARAMETRIC_SURFACE; 		// 面上線
		entity[13] = TRIMMED_SURFACE;					// トリム面
		entity[14] = SUBFIGURE_DEFINITION;				// 子図の定義
		entity[15] = ASSOCIATIVITY_INSTANCE;			// グループ
		entity[16] = DRAWING;							// 図面
		entity[17] = PROPERTY;							// 図面サイズ
		entity[18] = SINGULAR_SUBFIGURE_INSTANCE;		// 子図の参照
		entity[19] = VIEW;								// 投象面
}

// 各エンティティの表示属性を設定
void IGES_PARSER::InitDisplayStat(DispStat *Dstat)
{
	// 白色を設定
	Dstat->Color[0] = 1.0;
	Dstat->Color[1] = 1.0;
	Dstat->Color[2] = 1.0;
	Dstat->Color[3] = 0.5;

	// 表示属性を追加する場合は以下に追加のコードを記述
}

// Type110(直線)からType126(NURBS曲線)のパラメータを得る
int IGES_PARSER::GetNurbsCFromLine(int NurbsCount,int LineCount,BODY body)	
{
	int i=0;
	int KOD_ERRflag=0;

	body.NurbsC[NurbsCount].K = 2;		// 総和記号の上側添字（コントロールポイント-1）の値
	body.NurbsC[NurbsCount].M = 2;		// 基底関数の階数
	body.NurbsC[NurbsCount].N = body.NurbsC[NurbsCount].K + body.NurbsC[NurbsCount].M;	// ノットベクトルの数

	// ブーリアン型プロパティ4つ
	body.NurbsC[NurbsCount].prop[0] = 0;
	body.NurbsC[NurbsCount].prop[1] = 0;
	body.NurbsC[NurbsCount].prop[2] = 1;
	body.NurbsC[NurbsCount].prop[3] = 0;

	// メモリー確保
	KOD_ERRflag++;	// 1
	if((body.NurbsC[NurbsCount].T = (double *)malloc(sizeof(double)*body.NurbsC[NurbsCount].N)) == NULL){
		goto EXIT;
	}
	KOD_ERRflag++;	// 2
	if((body.NurbsC[NurbsCount].W = (double *)malloc(sizeof(double)*body.NurbsC[NurbsCount].K)) == NULL){
		goto EXIT;
	}
	KOD_ERRflag++;	// 3
	if((body.NurbsC[NurbsCount].cp = (Coord *)malloc(sizeof(Coord)*body.NurbsC[NurbsCount].K)) == NULL){
		goto EXIT;
	}

	// ノットベクトルの値	
	body.NurbsC[NurbsCount].T[0] = 0.;
	body.NurbsC[NurbsCount].T[1] = 0.;
	body.NurbsC[NurbsCount].T[2] = 1.;
	body.NurbsC[NurbsCount].T[3] = 1.;
	
	for(i=0;i<body.NurbsC[NurbsCount].K;i++){				// Weightの値
		body.NurbsC[NurbsCount].W[i] = 1.;
	}
	for(i=0;i<body.NurbsC[NurbsCount].K;i++){				// コントロールポイントの座標値
		body.NurbsC[NurbsCount].cp[i].x = body.Line[LineCount].cp[i].x;
		body.NurbsC[NurbsCount].cp[i].y = body.Line[LineCount].cp[i].y;
		body.NurbsC[NurbsCount].cp[i].z = body.Line[LineCount].cp[i].z;
	}
	
	// パラメータの値
	body.NurbsC[NurbsCount].V[0] = 0.;
	body.NurbsC[NurbsCount].V[1] = 1.;

	InitDisplayStat(&body.NurbsC[NurbsCount].Dstat);	// 表示属性の初期化

	return KOD_TRUE;

	// メモリー確保に失敗した場合は今まで確保した分を開放してKOD_ERRを返す
EXIT:
		SetMessage("PARAMETER SECTION KOD_ERROR:fail to allocate memory");
	if(KOD_ERRflag == 3){
		free(body.NurbsC[NurbsCount].cp);
		KOD_ERRflag--;
	}
	if(KOD_ERRflag == 2){
		free(body.NurbsC[NurbsCount].W);
		KOD_ERRflag--;
	}
	if(KOD_ERRflag == 1){
		free(body.NurbsC[NurbsCount].T);
	}
	return KOD_ERR;
}

// Type110(円・円弧)からType126(NURBS曲線)のパラメータデータを得る
int IGES_PARSER::GetNurbsCFromCirA(int NurbsCount,int CirCount,BODY body)	
{
	int	 flag=KOD_TRUE;
	double	angle_deg = 0.0,
			angle_rad = 0.0,
			radius = 0.0;
	Coord	vec[2];
	
	// 円/円弧の中心点O-始点Psベクトル成分、中心点-終点Peベクトル成分をそれぞれ求める
	vec[0] = SubCoord(body.CirA[CirCount].cp[1],body.CirA[CirCount].cp[0]);
	vec[1] = SubCoord(body.CirA[CirCount].cp[2],body.CirA[CirCount].cp[0]);	

	radius = body.CirA[CirCount].R;	// 円/円弧の中心点と始点の距離(半径)
	angle_rad = CalcVecAngle2D(vec[0],vec[1]);			// 円/円弧を成す中心角の大きさ(degree)を求める
	angle_deg = RadToDeg(angle_rad);				// 円/円弧を成す中心角の大きさ(radian)を求める

	// 中心角(degree)の大きさごとにセグメント数を変更する
	if( angle_deg > 0 && angle_deg <= 90 ){								// 0°<θ<=90°
		flag = CirAToNurbsC_seg1(NurbsCount ,CirCount , body, vec, angle_rad);		// 1セグメント
	}
	else if( angle_deg > 90 && angle_deg <= 270 ){						// 90°<θ<=270°
		flag = CirAToNurbsC_seg2(NurbsCount ,CirCount , body, vec, angle_rad);		// 2セグメント
	}
	else if( angle_deg > 270 && angle_deg < 360 ){						// 270°<θ<360°
		flag = CirAToNurbsC_seg3(NurbsCount ,CirCount , body, vec, angle_rad);		// 3セグメント
	}
	else if( angle_deg == 0 ){											// θ=0°(360°)
		flag = CirAToNurbsC_seg4(NurbsCount ,CirCount , body, vec, radius);			//　4セグメント
	}
	else{
		SetMessage("Center angle of a circle or circular arc is not calculated normally");
		return KOD_ERR;
	}

	return KOD_TRUE;
}

// 1セグメントの円弧(中心角が0°<θ<=90°の時)
int IGES_PARSER::CirAToNurbsC_seg1(int NurbsCount,int CirCount,BODY body,Coord vec[], double angle_rad)
{
	int i=0;
	int KOD_ERRflag=0;
	
	Coord	vec_cp;
	
	body.NurbsC[NurbsCount].K = 3;		// 総和記号の上側添字（コントロールポイント-1）の値
	body.NurbsC[NurbsCount].M = 3;		// 基底関数の階数
	body.NurbsC[NurbsCount].N = body.NurbsC[NurbsCount].K + body.NurbsC[NurbsCount].M;	// ノットベクトルの数

	// ブーリアン型プロパティ4つ
	body.NurbsC[NurbsCount].prop[0] = 0;
	body.NurbsC[NurbsCount].prop[1] = 0;
	body.NurbsC[NurbsCount].prop[2] = 1;
	body.NurbsC[NurbsCount].prop[3] = 0;
	
	// メモリー確保
	KOD_ERRflag++;	// 1
	if((body.NurbsC[NurbsCount].T = (double *)malloc(sizeof(double)*body.NurbsC[NurbsCount].N)) == NULL){
		goto EXIT;
	}
	KOD_ERRflag++;	// 2
	if((body.NurbsC[NurbsCount].W = (double *)malloc(sizeof(double)*body.NurbsC[NurbsCount].K)) == NULL){
		goto EXIT;
	}
	KOD_ERRflag++;	// 3
	if((body.NurbsC[NurbsCount].cp = (Coord *)malloc(sizeof(Coord)*body.NurbsC[NurbsCount].K)) == NULL){
		goto EXIT;
	}
	
	// ノットベクトルの値	
	body.NurbsC[NurbsCount].T[0] = 0.;
	body.NurbsC[NurbsCount].T[1] = 0.;
	body.NurbsC[NurbsCount].T[2] = 0.;
	body.NurbsC[NurbsCount].T[3] = 1.;
	body.NurbsC[NurbsCount].T[4] = 1.;
	body.NurbsC[NurbsCount].T[5] = 1.;
		
	// Weightの値
	for(i=0; i<3; i++){
		if(i % 2 == 0){
			body.NurbsC[NurbsCount].W[i] = 1.;
		}	
		else if(i % 2 == 1){	
			body.NurbsC[NurbsCount].W[i] = cos(angle_rad/2);
		}
	}
		
	vec_cp = Arc_CP(vec[0], vec[1], cos(angle_rad));	//　円の中心点からコントロールポイントP1へのベクトルを求める
	
	// コントロールポイントの座標値
	body.NurbsC[NurbsCount].cp[0].x = body.CirA[CirCount].cp[1].x;
	body.NurbsC[NurbsCount].cp[0].y = body.CirA[CirCount].cp[1].y;		
	body.NurbsC[NurbsCount].cp[1].x = vec_cp.x + body.CirA[CirCount].cp[0].x;
	body.NurbsC[NurbsCount].cp[1].y = vec_cp.y + body.CirA[CirCount].cp[0].y;
	body.NurbsC[NurbsCount].cp[2].x = body.CirA[CirCount].cp[2].x;
	body.NurbsC[NurbsCount].cp[2].y = body.CirA[CirCount].cp[2].y;

	for(i=0; i<3; i++){
		body.NurbsC[NurbsCount].cp[i].z = body.CirA[CirCount].zt;	// Z方向の大きさは一定
	}
		
	body.NurbsC[NurbsCount].V[0] = 0.;		// パラメータの値
	body.NurbsC[NurbsCount].V[1] = 1.;

	InitDisplayStat(&body.NurbsC[NurbsCount].Dstat);
		  
	return KOD_TRUE;

	// メモリー確保に失敗した場合は今まで確保した分を開放してKOD_ERRを返す
EXIT:
		SetMessage("PARAMETER SECTION KOD_ERROR:fail to allocate memory");
	if(KOD_ERRflag == 3){
		free(body.NurbsC[NurbsCount].cp);
		KOD_ERRflag--;
	}
	if(KOD_ERRflag == 2){
		free(body.NurbsC[NurbsCount].W);
		KOD_ERRflag--;
	}
	if(KOD_ERRflag == 1){
		free(body.NurbsC[NurbsCount].T);
	}
	return KOD_ERR;
}

// 2セグメントの円弧(中心角が90°<θ<=270°の時)
int IGES_PARSER::CirAToNurbsC_seg2(int NurbsCount,int CirCount,BODY body,Coord vec[], double angle_rad)
{
	int	i = 0,
		KOD_ERRflag = 0;
	double	angle_rad2 = 0.0;
	
	Coord vec_cp[3];
	
	body.NurbsC[NurbsCount].K = 5;		// 総和記号の上側添字（コントロールポイント-1）の値
	body.NurbsC[NurbsCount].M = 3;		// 基底関数の階数
	body.NurbsC[NurbsCount].N = body.NurbsC[NurbsCount].K + body.NurbsC[NurbsCount].M;	// ノットベクトルの数
	// ブーリアン型プロパティ4つ
	body.NurbsC[NurbsCount].prop[0] = 0;
	body.NurbsC[NurbsCount].prop[1] = 0;
	body.NurbsC[NurbsCount].prop[2] = 1;
	body.NurbsC[NurbsCount].prop[3] = 0;
	
	// メモリー確保
	KOD_ERRflag++;	// 1
	if((body.NurbsC[NurbsCount].T = (double *)malloc(sizeof(double)*body.NurbsC[NurbsCount].N)) == NULL){
		goto EXIT;
	}
	KOD_ERRflag++;	// 2
	if((body.NurbsC[NurbsCount].W = (double *)malloc(sizeof(double)*body.NurbsC[NurbsCount].K)) == NULL){
		goto EXIT;
	}
	KOD_ERRflag++;	// 3
	if((body.NurbsC[NurbsCount].cp = (Coord *)malloc(sizeof(Coord)*body.NurbsC[NurbsCount].K)) == NULL){
		goto EXIT;
	}
	
	// ノットベクトルの値	
	body.NurbsC[NurbsCount].T[0] = 0.;
	body.NurbsC[NurbsCount].T[1] = 0.;
	body.NurbsC[NurbsCount].T[2] = 0.;
	body.NurbsC[NurbsCount].T[3] = 2./4.;
	body.NurbsC[NurbsCount].T[4] = 2./4.;
	body.NurbsC[NurbsCount].T[5] = 1.;
	body.NurbsC[NurbsCount].T[6] = 1.;
	body.NurbsC[NurbsCount].T[7] = 1.;
		
	// Weightの値
	for(i=0; i<5; i++){
		if(i % 2 == 0){
			body.NurbsC[NurbsCount].W[i] = 1.;
		}	
		else if(i % 2 == 1){	
			body.NurbsC[NurbsCount].W[i] = cos(angle_rad/4);
		}
	}
		
	angle_rad2 = angle_rad/2;	// (中心角)÷2
	
	vec_cp[1] = CalcRotVec2D(vec[0], angle_rad2);		// 円の中心点から中心角の半分の位置(コントロールポイントP2)へのベクトルを求める
	vec_cp[0] = Arc_CP(vec[0], vec_cp[1], cos(angle_rad2));	// 円の中心点からコントロールポイントP1へのベクトルを求める
	vec_cp[2] = Arc_CP(vec_cp[1], vec[1], cos(angle_rad2));	// 円の中心点からコントロールポイントP3へのベクトルを求める
	
	// コントロールポイントの座標値
	body.NurbsC[NurbsCount].cp[0].x = body.CirA[CirCount].cp[1].x;
	body.NurbsC[NurbsCount].cp[0].y = body.CirA[CirCount].cp[1].y;		
 	body.NurbsC[NurbsCount].cp[1].x = vec_cp[0].x + body.CirA[CirCount].cp[0].x;
 	body.NurbsC[NurbsCount].cp[1].y = vec_cp[0].y + body.CirA[CirCount].cp[0].y;
 	body.NurbsC[NurbsCount].cp[2].x = vec_cp[1].x + body.CirA[CirCount].cp[0].x;
 	body.NurbsC[NurbsCount].cp[2].y = vec_cp[1].y + body.CirA[CirCount].cp[0].y;
 	body.NurbsC[NurbsCount].cp[3].x = vec_cp[2].x + body.CirA[CirCount].cp[0].x;
 	body.NurbsC[NurbsCount].cp[3].y = vec_cp[2].y + body.CirA[CirCount].cp[0].y;
 	body.NurbsC[NurbsCount].cp[4].x = body.CirA[CirCount].cp[2].x;
 	body.NurbsC[NurbsCount].cp[4].y = body.CirA[CirCount].cp[2].y;
	for(i=0; i<5; i++){
		body.NurbsC[NurbsCount].cp[i].z = body.CirA[CirCount].zt;	// Z方向の大きさは一定
	}
	
	body.NurbsC[NurbsCount].V[0] = 0.;		// パラメータの値
	body.NurbsC[NurbsCount].V[1] = 1.;

	InitDisplayStat(&body.NurbsC[NurbsCount].Dstat);
		  
	return KOD_TRUE;

	// メモリー確保に失敗した場合は今まで確保した分を開放してKOD_ERRを返す
EXIT:
		SetMessage("PARAMETER SECTION KOD_ERROR:fail to allocate memory");
	if(KOD_ERRflag == 3){
		free(body.NurbsC[NurbsCount].cp);
		KOD_ERRflag--;
	}
	if(KOD_ERRflag == 2){
		free(body.NurbsC[NurbsCount].W);
		KOD_ERRflag--;
	}
	if(KOD_ERRflag == 1){
		free(body.NurbsC[NurbsCount].T);
	}
	return KOD_ERR;
}

// 3セグメントの円弧(中心角が270°<θ<360°の時)
int IGES_PARSER::CirAToNurbsC_seg3(int NurbsCount,int CirCount,BODY body,Coord vec[], double angle_rad)
{
	int	i=0,
		KOD_ERRflag=0;
	double	angle_rad3 = 0.0;
	
	Coord	vec_cp[5];
	
	body.NurbsC[NurbsCount].K = 7;		// 総和記号の上側添字（コントロールポイント-1）の値
	body.NurbsC[NurbsCount].M = 3;		// 基底関数の階数
	body.NurbsC[NurbsCount].N = body.NurbsC[NurbsCount].K + body.NurbsC[NurbsCount].M;	// ノットベクトルの数
	
	// ブーリアン型プロパティ4つ
	body.NurbsC[NurbsCount].prop[0] = 0;
	body.NurbsC[NurbsCount].prop[1] = 0;
	body.NurbsC[NurbsCount].prop[2] = 1;
	body.NurbsC[NurbsCount].prop[3] = 0;
	
	// メモリー確保
	KOD_ERRflag++;	// 1
	if((body.NurbsC[NurbsCount].T = (double *)malloc(sizeof(double)*body.NurbsC[NurbsCount].N)) == NULL){
		goto EXIT;
	}
	KOD_ERRflag++;	// 2
	if((body.NurbsC[NurbsCount].W = (double *)malloc(sizeof(double)*body.NurbsC[NurbsCount].K)) == NULL){
		goto EXIT;
	}
	KOD_ERRflag++;	// 3
	if((body.NurbsC[NurbsCount].cp = (Coord *)malloc(sizeof(Coord)*body.NurbsC[NurbsCount].K)) == NULL){
		goto EXIT;
	}
	
	// ノットベクトルの値	
	body.NurbsC[NurbsCount].T[0] = 0.;
	body.NurbsC[NurbsCount].T[1] = 0.;
	body.NurbsC[NurbsCount].T[2] = 0.;
	body.NurbsC[NurbsCount].T[3] = 1./3.;
	body.NurbsC[NurbsCount].T[4] = 1./3.;
	body.NurbsC[NurbsCount].T[5] = 2./3.;
	body.NurbsC[NurbsCount].T[6] = 2./3.;
	body.NurbsC[NurbsCount].T[7] = 1.;
	body.NurbsC[NurbsCount].T[8] = 1.;
	body.NurbsC[NurbsCount].T[9] = 1.;
	
	// Weightの値
	for(i=0; i<7; i++){
		if(i % 2 == 0){
			body.NurbsC[NurbsCount].W[i] = 1.;
		}	
		else if(i % 2 == 1){	
			body.NurbsC[NurbsCount].W[i] = cos(angle_rad/6);
		}
	}

	angle_rad3 = angle_rad/3;	// (中心角)÷3
	
	vec_cp[1] = CalcRotVec2D(vec[0], angle_rad3);		// 円の中心点から中心角の1/3の位置(コントロールポイントP2)へのベクトルを求める
	vec_cp[0] = Arc_CP(vec[0], vec_cp[1], cos(angle_rad3));	// 円の中心点からコントロールポイントP1へのベクトルを求める
	vec_cp[3] = CalcRotVec2D(vec_cp[1], angle_rad3);		// 円の中心点から中心角の2/3の位置(コントロールポイントP4)へのベクトルを求める
	vec_cp[2] = Arc_CP(vec_cp[1], vec_cp[3], cos(angle_rad3));	// 円の中心点からコントロールポイントP3へのベクトルを求める
	vec_cp[4] = Arc_CP(vec_cp[3], vec[1], cos(angle_rad3));	// 円の中心点からコントロールポイントP4へのベクトルを求める
		
	// コントロールポイントの座標値
	body.NurbsC[NurbsCount].cp[0].x = body.CirA[CirCount].cp[1].x;
	body.NurbsC[NurbsCount].cp[0].y = body.CirA[CirCount].cp[1].y;		
	body.NurbsC[NurbsCount].cp[1].x = vec_cp[0].x + body.CirA[CirCount].cp[0].x;
	body.NurbsC[NurbsCount].cp[1].y = vec_cp[0].y + body.CirA[CirCount].cp[0].y;
	body.NurbsC[NurbsCount].cp[2].x = vec_cp[1].x + body.CirA[CirCount].cp[0].x;
	body.NurbsC[NurbsCount].cp[2].y = vec_cp[1].y + body.CirA[CirCount].cp[0].y;
	body.NurbsC[NurbsCount].cp[3].x = vec_cp[2].x + body.CirA[CirCount].cp[0].x;
	body.NurbsC[NurbsCount].cp[3].y = vec_cp[2].y + body.CirA[CirCount].cp[0].y;
	body.NurbsC[NurbsCount].cp[4].x = vec_cp[3].x + body.CirA[CirCount].cp[0].x;
	body.NurbsC[NurbsCount].cp[4].y = vec_cp[3].y + body.CirA[CirCount].cp[0].y;
	body.NurbsC[NurbsCount].cp[5].x = vec_cp[4].x + body.CirA[CirCount].cp[0].x;
	body.NurbsC[NurbsCount].cp[5].y = vec_cp[4].y + body.CirA[CirCount].cp[0].y;
	body.NurbsC[NurbsCount].cp[6].x = body.CirA[CirCount].cp[2].x;
	body.NurbsC[NurbsCount].cp[6].y = body.CirA[CirCount].cp[2].y;

	for(i=0; i<7; i++){
		body.NurbsC[NurbsCount].cp[i].z = body.CirA[CirCount].zt;	// Z方向の大きさは一定
	}
		
	body.NurbsC[NurbsCount].V[0] = 0.;		// パラメータの値
	body.NurbsC[NurbsCount].V[1] = 1.;

	InitDisplayStat(&body.NurbsC[NurbsCount].Dstat);
		  
	return KOD_TRUE;

	// メモリー確保に失敗した場合は今まで確保した分を開放してKOD_ERRを返す
EXIT:
		SetMessage("PARAMETER SECTION KOD_ERROR:fail to allocate memory");
	if(KOD_ERRflag == 3){
		free(body.NurbsC[NurbsCount].cp);
		KOD_ERRflag--;
	}
	if(KOD_ERRflag == 2){
		free(body.NurbsC[NurbsCount].W);
		KOD_ERRflag--;
	}
	if(KOD_ERRflag == 1){
		free(body.NurbsC[NurbsCount].T);
	}
	return KOD_ERR;
}

// 4セグメントの円弧(円)
int IGES_PARSER::CirAToNurbsC_seg4(int NurbsCount,int CirCount,BODY body,Coord vec[], double radius)
{
	int i=0;
	int KOD_ERRflag=0;

	body.NurbsC[NurbsCount].K = 9;		// 総和記号の上側添字（コントロールポイント-1）の値
	body.NurbsC[NurbsCount].M = 3;		// 基底関数の階数
	body.NurbsC[NurbsCount].N = body.NurbsC[NurbsCount].K + body.NurbsC[NurbsCount].M;	// ノットベクトルの数
	
	// ブーリアン型プロパティ4つ
	body.NurbsC[NurbsCount].prop[0] = 0;
	body.NurbsC[NurbsCount].prop[1] = 0;
	body.NurbsC[NurbsCount].prop[2] = 1;
	body.NurbsC[NurbsCount].prop[3] = 0;
	
	// メモリー確保
	KOD_ERRflag++;	// 1
	if((body.NurbsC[NurbsCount].T = (double *)malloc(sizeof(double)*body.NurbsC[NurbsCount].N)) == NULL){
		goto EXIT;
	}
	KOD_ERRflag++;	// 2
	if((body.NurbsC[NurbsCount].W = (double *)malloc(sizeof(double)*body.NurbsC[NurbsCount].K)) == NULL){
		goto EXIT;
	}
	KOD_ERRflag++;	// 3
	if((body.NurbsC[NurbsCount].cp = (Coord *)malloc(sizeof(Coord)*body.NurbsC[NurbsCount].K)) == NULL){
		goto EXIT;
	}
	
	// ノットベクトルの値	
	body.NurbsC[NurbsCount].T[0] = 0.;
	body.NurbsC[NurbsCount].T[1] = 0.;
	body.NurbsC[NurbsCount].T[2] = 0.;
	body.NurbsC[NurbsCount].T[3] = 1./4.;
	body.NurbsC[NurbsCount].T[4] = 1./4.;
	body.NurbsC[NurbsCount].T[5] = 2./4.;
	body.NurbsC[NurbsCount].T[6] = 2./4.;
	body.NurbsC[NurbsCount].T[7] = 3./4.;
	body.NurbsC[NurbsCount].T[8] = 3./4.;
	body.NurbsC[NurbsCount].T[9] = 1.;
	body.NurbsC[NurbsCount].T[10] = 1.;
	body.NurbsC[NurbsCount].T[11] = 1.;
		
	// Weightの値
	for(i=0; i<9; i++){
		if(i % 2 == 0){
			body.NurbsC[NurbsCount].W[i] = 1.;
		}	
		else if(i % 2 == 1){	
			body.NurbsC[NurbsCount].W[i] = sqrt(2.0)/2;
		}
	}

	// コントロールポイントの座標値
	body.NurbsC[NurbsCount].cp[0].x = body.CirA[CirCount].cp[0].x + radius;
	body.NurbsC[NurbsCount].cp[0].y = body.CirA[CirCount].cp[0].y;		
	body.NurbsC[NurbsCount].cp[1].x = body.CirA[CirCount].cp[0].x + radius;
	body.NurbsC[NurbsCount].cp[1].y = body.CirA[CirCount].cp[0].y + radius;
	body.NurbsC[NurbsCount].cp[2].x = body.CirA[CirCount].cp[0].x;
	body.NurbsC[NurbsCount].cp[2].y = body.CirA[CirCount].cp[0].y + radius;
	body.NurbsC[NurbsCount].cp[3].x = body.CirA[CirCount].cp[0].x - radius;
	body.NurbsC[NurbsCount].cp[3].y = body.CirA[CirCount].cp[0].y + radius;
	body.NurbsC[NurbsCount].cp[4].x = body.CirA[CirCount].cp[0].x - radius;
	body.NurbsC[NurbsCount].cp[4].y = body.CirA[CirCount].cp[0].y;
	body.NurbsC[NurbsCount].cp[5].x = body.CirA[CirCount].cp[0].x - radius;
	body.NurbsC[NurbsCount].cp[5].y = body.CirA[CirCount].cp[0].y - radius;
	body.NurbsC[NurbsCount].cp[6].x = body.CirA[CirCount].cp[0].x;
	body.NurbsC[NurbsCount].cp[6].y = body.CirA[CirCount].cp[0].y - radius;
	body.NurbsC[NurbsCount].cp[7].x = body.CirA[CirCount].cp[0].x + radius;
	body.NurbsC[NurbsCount].cp[7].y = body.CirA[CirCount].cp[0].y - radius;
	body.NurbsC[NurbsCount].cp[8].x = body.CirA[CirCount].cp[0].x + radius;
	body.NurbsC[NurbsCount].cp[8].y = body.CirA[CirCount].cp[0].y;

	for(i=0; i<9; i++){
		body.NurbsC[NurbsCount].cp[i].z = body.CirA[CirCount].zt;	// Z方向の大きさは一定
	}
		
	body.NurbsC[NurbsCount].V[0] = 0.;		// パラメータの値
	body.NurbsC[NurbsCount].V[1] = 1.;

	InitDisplayStat(&body.NurbsC[NurbsCount].Dstat);
	
	return KOD_TRUE;

	// メモリー確保に失敗した場合は今まで確保した分を開放してKOD_ERRを返す
EXIT:
		SetMessage("PARAMETER SECTION KOD_ERROR:fail to allocate memory");
	if(KOD_ERRflag == 3){
		free(body.NurbsC[NurbsCount].cp);
		KOD_ERRflag--;
	}
	if(KOD_ERRflag == 2){
		free(body.NurbsC[NurbsCount].W);
		KOD_ERRflag--;
	}
	if(KOD_ERRflag == 1){
		free(body.NurbsC[NurbsCount].T);
	}
	return KOD_ERR;
}

// Type124(変換行列)を用いてNURBS曲線を座標変換する
int IGES_PARSER::TransformNurbsC(int NurbsCount,int TMp,BODY body)	
{
	int i;

	for(i=0;i<body.NurbsC[NurbsCount].K;i++){
		body.NurbsC[NurbsCount].cp[i] = MulFrameCoord(body.TMat[TMp].R,body.TMat[TMp].T,body.NurbsC[NurbsCount].cp[i]);	
	}
	
	return KOD_TRUE;
}
