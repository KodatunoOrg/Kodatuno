/*************************
* DXFファイルを読み込む  *
**************************/

#include "DXF_Parser.h"

// Function: DXF_PARSER
// コンストラクタ
DXF_PARSER::DXF_PARSER()
{
	for(int i=0;i<ALL_ENTITY_TYPE_NUM;i++){
		Count[i] = 0;
	}
}

// Function: DXF_Parser_Main
// DXFパーサメイン
// 
// Parameter:
// *body - 立体を構成するエンティティの集合オブジェクトへのポインタ
// TypeNum[] - 各エンティティの数が格納される
//
// Return:
// 指定されたファイルが開けない：KOD_ERR, 読み込み成功：KOD_TRUE
int DXF_PARSER::DXF_Parser_Main(BODY *body,const char *DXF_fname)
{
	FILE *fp;
	NURBS_Func nfunc;
	char mes[BUFSIZEMAX_DXF];		// 出力用メッセージ格納バッファ
	int line = 1;				// 現在の行番号
	int section = 0;			// セクション番号

	// DXFファイルオープン
	if((fp = fopen(DXF_fname,"r")) == NULL){
		sprintf(mes,"KOD_ERROR: Cannot open %s",DXF_fname);
		GuiIF.SetMessage(mes);
		return(KOD_ERR);
	}
	sprintf(mes,"Open %s",DXF_fname);
	GuiIF.SetMessage(mes);

	// ファイル読み込み
	while(fgets(Buf,sizeof(Buf),fp) != NULL){

		if(EvenOdd(line) == ODD)			// 奇数行のときはグループコードを読み込む
			sscanf(Buf,"%d",&Gcode);		

		// グループコードがセクション名を表す"2"だったら
		if(Gcode == SECTION_NAME){
			fgets(Buf,sizeof(Buf),fp);		// さらに1行読み込み，
			section = CheckSection(Buf);	// セクション名を判別

			// セクション名ごとに読み込み
			if(section == HEADER_SECTION)
				ReadHeaderSection(fp,line,body);		// ヘッダセクションの読み込み
			else if(section == CLASS_SECTION)
				ReadClassesSection(fp,line,body);		// クラスセクションの読み込み
			else if(section == TABLE_SECTION)
				ReadTablesSection(fp,line,body);		// テーブルセクションの読み込み
			else if(section == BLOCK_SECTION)
				ReadBlocksSection(fp,line,body);		// ブロックセクションの読み込み
			else if(section == ENTITIY_SECTION)
				ReadEntitiesSection(fp,line,body);		// エンティティセクションの読み込み
			else if(section == OBJECT_SECTION)
				ReadObjectsSection(fp,line,body);		// オブジェクトセクションの読み込み
			line++;
		}

		line++;
	}

	fclose(fp);

	body->MaxCoord = 200;

	return KOD_TRUE;
}

// Function: ReadEntitiesSection
// エンティティセクションの読み込み
//
// Parameter:
// *fp - DXFファイルへのファイルポインタ
// Line - 現在の行番号
// *body - BODYへのポインタ
//
// Return:
// KOD_TRUE
int DXF_PARSER::ReadEntitiesSection(FILE *fp,int Line,BODY *body)
{
	fpos_t pos;

	fgetpos(fp,&pos);			// 現在のfpの位置を覚えておく

	ResearchEntNum(fp,body);	// 各エンティティの数をリサーチ
	body->NewBodyElem();		// BODY構造体内の各エンティティのメモリー確保
	fseek(fp,pos,SEEK_SET);		// エンティティセクションの最初の位置へ戻る
	fgets(Buf,sizeof(Buf),fp);	// 1行読み捨て

	// 実データ取得
	do{
		fgets(Buf,sizeof(Buf),fp);	// 偶数行
		sscanf(Buf,"%s",&Label);

		if(strcmp(Label,"ARC") == 0)			GetArcData(fp,body);		// 円弧
		else if(strcmp(Label,"CIRCLE") == 0)	GetCircleData(fp,body);		// 円
		else if(strcmp(Label,"LINE") == 0)		GetLineData(fp,body);		// 直線
		else	fgets(Buf,sizeof(Buf),fp);	// グループコード(奇数行)読み捨て

	}while(strcmp(Label,"ENDSEC"));

	ChangeEntityforNurbs(body);		// 円，直線エンティティをNURBSに変換

	return KOD_TRUE;
}

// Function: ChangeEntityforNurbs
// 円，直線エンティティをNURBSに変換
//
// Parameter:
// *body - BODYへのポインタ
//
// Return:
// メモリー確保に失敗：KOD_ERR
int DXF_PARSER::ChangeEntityforNurbs(BODY *body)
{
	int ncount = 0;
	for(int i=0;i<body->TypeNum[_LINE];i++){
		if(body->GetNurbsCFromLine(ncount,i) == KOD_ERR) return KOD_ERR;	// 円/円弧パラメータからNURBS曲線パラメータを得る
		InitDisplayStat(&body->NurbsC[ncount].Dstat);						// 表示属性の初期化
		ncount++;
	}
	for(int i=0;i<body->TypeNum[_CIRCLE_ARC];i++){
		if(body->GetNurbsCFromCirA(ncount,i) == KOD_ERR) return KOD_ERR;	// 線分パラメータからNURBS曲線パラメータを得る
		InitDisplayStat(&body->NurbsC[ncount].Dstat);								// 表示属性の初期化
		ncount++;
	}
}

// Function: GetArcData
// 円弧のデータを取得しBODYに格納
//
// Parameter:
// *fp - DXFファイルへのファイルポインタ
// *body - BODYへのポインタ
//
// Return:
// KOD_TRUE
int DXF_PARSER::GetArcData(FILE *fp,BODY *body)
{
	while(1){
		fgets(Buf,sizeof(Buf),fp);		// 奇数行
		sscanf(Buf,"%d",&Gcode);

		if(Gcode == ENT_TYPE)	break;	// グループコード:0ならwhile break

		else{
			fgets(Buf,sizeof(Buf),fp);	// 偶数行

			if(Gcode == COORD_X)
				sscanf(Buf,"%lf",&body->CirA[Count[_CIRCLE_ARC]].cp[0].x);		// 円弧の中心座標X
			else if(Gcode == COORD_Y)
				sscanf(Buf,"%lf",&body->CirA[Count[_CIRCLE_ARC]].cp[0].y);		// 円弧の中心座標Y
			else if(Gcode == COORD_Z)
				sscanf(Buf,"%lf",&body->CirA[Count[_CIRCLE_ARC]].cp[0].z);		// 円弧の中心座標Z
			else if(Gcode == RADIUS)
				sscanf(Buf,"%lf",&body->CirA[Count[_CIRCLE_ARC]].R);			// 円弧の半径
			else if(Gcode == START_ANG)
				sscanf(Buf,"%lf",&body->CirA[Count[_CIRCLE_ARC]].t[0]);			// 円弧の開始角度
			else if(Gcode == END_ANG)
				sscanf(Buf,"%lf",&body->CirA[Count[_CIRCLE_ARC]].t[1]);			// 円弧の終了角度
		}
	}

	// debug
	//fprintf(stderr,"%d:%lf,%lf,%lf,%lf,%lf,%lf\n",Count[_CIRCLE_ARC],
	//	body->CirA[Count[_CIRCLE_ARC]].cp[0].x,
	//	body->CirA[Count[_CIRCLE_ARC]].cp[0].y,
	//	body->CirA[Count[_CIRCLE_ARC]].cp[0].z,
	//	body->CirA[Count[_CIRCLE_ARC]].R,
	//	body->CirA[Count[_CIRCLE_ARC]].t[0],
	//	body->CirA[Count[_CIRCLE_ARC]].t[1]);

	SetStartEndPtArc(&body->CirA[Count[_CIRCLE_ARC]]);			// 円弧の始点，終点をセット
	CalcUVvec(&body->CirA[Count[_CIRCLE_ARC]]);					// CIRAのUV直交座標を設定する

	InitDisplayStat(&body->CirA[Count[_CIRCLE_ARC]].Dstat);		// 色指定
    body->CirA[Count[_CIRCLE_ARC]].BlankStat = DISPLAY;         // 描画対象であることを宣言
	body->CirA[Count[_CIRCLE_ARC]].EntUseFlag = GEOMTRYELEM;	// 幾何要素であることを宣言
	body->CirA[Count[_CIRCLE_ARC]].pD = NULL;					// IGESでないので関係なし

	Count[_CIRCLE_ARC]++;

	return KOD_TRUE;
}

// Function: CalcUVvec
// CIRAのUV直交座標を設定する
//
// Parameter:
// *Cira - 円/円弧へのポインタ
//
// Return:
// KOD_TRUE
int DXF_PARSER::CalcUVvec(CIRA *Cira)
{
	// X-Y平面
	Cira->U = SetCoord(1,0,0);
	Cira->V = SetCoord(0,1,0);

	return KOD_TRUE;
}

// Function: SetStartEndPtArc
// 円弧の始点，終点をセット
// Parameter:
// *Cira - 円/円弧へのポインタ
//
// Return:
// KOD_TRUE
int DXF_PARSER::SetStartEndPtArc(CIRA *Cira)
{
	double sth = Cira->t[0]*PI/180;
	double eth = Cira->t[1]*PI/180;

	Cira->cp[1].x = Cira->R*cos(sth) + Cira->cp[0].x;
	Cira->cp[1].y = Cira->R*sin(sth) + Cira->cp[0].y;
	Cira->cp[1].z = 0;
	Cira->cp[2].x = Cira->R*cos(eth) + Cira->cp[0].x;
	Cira->cp[2].y = Cira->R*sin(eth) + Cira->cp[0].y;
	Cira->cp[2].z = 0;

	return KOD_TRUE;
}

// Function: GetCircleData
// 円のデータを取得しBODYに格納
//
// Parameter:
// *fp - DXFファイルへのファイルポインタ
// *body - BODYへのポインタ
//
// Return:
// KOD_TRUE
int DXF_PARSER::GetCircleData(FILE *fp,BODY *body)
{
	while(1){
		fgets(Buf,sizeof(Buf),fp);		// 奇数行
		sscanf(Buf,"%d",&Gcode);

		if(Gcode == ENT_TYPE)	break;	// グループコード:0ならwhile break

		else{
			fgets(Buf,sizeof(Buf),fp);	// 偶数行

			if(Gcode == COORD_X)
				sscanf(Buf,"%lf",&body->CirA[Count[_CIRCLE_ARC]].cp[0].x);		// 円の中心座標X
			else if(Gcode == COORD_Y)
				sscanf(Buf,"%lf",&body->CirA[Count[_CIRCLE_ARC]].cp[0].y);		// 円の中心座標Y
			else if(Gcode == COORD_Z)
				sscanf(Buf,"%lf",&body->CirA[Count[_CIRCLE_ARC]].cp[0].z);		// 円の中心座標Z
			else if(Gcode == RADIUS)
				sscanf(Buf,"%lf",&body->CirA[Count[_CIRCLE_ARC]].R);			// 円の半径
		}
	}

	// debug
	//fprintf(stderr,"%d:%lf,%lf,%lf,%lf\n",Count[_CIRCLE_ARC],
	//	body->CirA[Count[_CIRCLE_ARC]].cp[0].x,
	//	body->CirA[Count[_CIRCLE_ARC]].cp[0].y,
	//	body->CirA[Count[_CIRCLE_ARC]].cp[0].z,
	//	body->CirA[Count[_CIRCLE_ARC]].R);

	body->CirA[Count[_CIRCLE_ARC]].t[0] = 0;		// 円の開始角度
	body->CirA[Count[_CIRCLE_ARC]].t[1] = 360;		// 円の終了角度
	SetStartEndPtArc(&body->CirA[Count[_CIRCLE_ARC]]);			// 円弧の始点，終点をセット
	CalcUVvec(&body->CirA[Count[_CIRCLE_ARC]]);					// CIRAのUV直交座標を設定する

	InitDisplayStat(&body->CirA[Count[_CIRCLE_ARC]].Dstat);
    body->CirA[Count[_CIRCLE_ARC]].BlankStat = DISPLAY;         // 描画対象であることを宣言
	body->CirA[Count[_CIRCLE_ARC]].EntUseFlag = GEOMTRYELEM;
	body->CirA[Count[_CIRCLE_ARC]].pD = NULL;

	Count[_CIRCLE_ARC]++;

	return KOD_TRUE;
}

// Function: GetLineData
// 線分のデータを取得しBODYに格納
//
// Parameter:
// *fp - DXFファイルへのファイルポインタ
// *body - BODYへのポインタ
//
// Return:
// KOD_TRUE
int DXF_PARSER::GetLineData(FILE *fp,BODY *body)
{
	while(1){
		fgets(Buf,sizeof(Buf),fp);		// 奇数行
		sscanf(Buf,"%d",&Gcode);

		if(Gcode == ENT_TYPE)	break;	// グループコード:0ならwhile break

		else{
			fgets(Buf,sizeof(Buf),fp);	// 偶数行

			if(Gcode == COORD_X)
				sscanf(Buf,"%lf",&body->Line[Count[_LINE]].cp[0].x);		// 線分の始点X
			else if(Gcode == COORD_Y)
				sscanf(Buf,"%lf",&body->Line[Count[_LINE]].cp[0].y);		// 線分の始点Y
			else if(Gcode == COORD_Z)
				sscanf(Buf,"%lf",&body->Line[Count[_LINE]].cp[0].z);		// 線分の始点Z
			else if(Gcode == COORD_X_)
				sscanf(Buf,"%lf",&body->Line[Count[_LINE]].cp[1].x);		// 線分の終点X
			else if(Gcode == COORD_Y_)
				sscanf(Buf,"%lf",&body->Line[Count[_LINE]].cp[1].y);		// 線分の終点Y
			else if(Gcode == COORD_Z_)
				sscanf(Buf,"%lf",&body->Line[Count[_LINE]].cp[1].z);		// 線分の終点Z
		}
	}

	// debug
	//fprintf(stderr,"%d:%lf,%lf,%lf,%lf,%lf,%lf\n",Count[_LINE],
	//	body->Line[Count[_LINE]].cp[0].x,
	//	body->Line[Count[_LINE]].cp[0].y,
	//	body->Line[Count[_LINE]].cp[0].z,
	//	body->Line[Count[_LINE]].cp[1].x,
	//	body->Line[Count[_LINE]].cp[1].y,
	//	body->Line[Count[_LINE]].cp[1].z);

	InitDisplayStat(&body->Line[Count[_LINE]].Dstat);
    body->Line[Count[_LINE]].BlankStat = DISPLAY;
	body->Line[Count[_LINE]].EntUseFlag = GEOMTRYELEM;
	body->Line[Count[_LINE]].pD = NULL;

	Count[_LINE]++;

	return KOD_TRUE;
}

// Function: ResearchEntNum
// 各エンティティの数をリサーチ
//
// Parameter:
// *fp - DXFファイルへのファイルポインタ
// *body - BODYへのポインタ
//
// Return:
// KOD_TRUE
int DXF_PARSER::ResearchEntNum(FILE *fp,BODY *body)
{
	do{
		fgets(Buf,sizeof(Buf),fp);	// 奇数行
		sscanf(Buf,"%d",&Gcode);
		fgets(Buf,sizeof(Buf),fp);	// 偶数行
		sscanf(Buf,"%s",&Label);

		if(Gcode == ENT_TYPE){
			if(strcmp(Label,"ARC") == 0)			body->TypeNum[_CIRCLE_ARC]++;
			else if(strcmp(Label,"CIRCLE") == 0)	body->TypeNum[_CIRCLE_ARC]++;
			else if(strcmp(Label,"LINE") == 0)		body->TypeNum[_LINE]++;
		}
	}while(strcmp(Label,"ENDSEC"));

	body->TypeNum[_NURBSC] = body->TypeNum[_CIRCLE_ARC] + body->TypeNum[_LINE];

	fprintf(stderr,"%d,%d\n",body->TypeNum[_CIRCLE_ARC],body->TypeNum[_LINE]);	// for debug

	return KOD_TRUE;
}

// Function:ReadHeaderSection
// ヘッダセクションの読み込み(スケルトン)
//
// Parameter:
// *fp - DXFファイルへのファイルポインタ
// *body - BODYへのポインタ
//
// Return:
// KOD_TRUE
int DXF_PARSER::ReadHeaderSection(FILE *fp,int Line,BODY *body)
{
	return KOD_TRUE;
}

// Function:ReadClassesSection
// クラスセクションの読み込み(スケルトン)
//
// Parameter:
// *fp - DXFファイルへのファイルポインタ
// *body - BODYへのポインタ
//
// Return:
// KOD_TRUE
int DXF_PARSER::ReadClassesSection(FILE *fp,int Line,BODY *body)
{
	return KOD_TRUE;
}

// Function: ReadTablesSection
// テーブルセクションの読み込み(スケルトン)
//
// Parameter:
// *fp - DXFファイルへのファイルポインタ
// *body - BODYへのポインタ
//
// Return:
// KOD_TRUE
int DXF_PARSER::ReadTablesSection(FILE *fp,int Line,BODY *body)
{
	return KOD_TRUE;
}

// Function: ReadBlocksSection
// ブロックセクションの読み込み(スケルトン)
//
// Parameter:
// *fp - DXFファイルへのファイルポインタ
// *body - BODYへのポインタ
//
// Return:
// KOD_TRUE
int DXF_PARSER::ReadBlocksSection(FILE *fp,int Line,BODY *body)
{
	return KOD_TRUE;
}

// Function: ReadObjectsSection
// オブジェクトセクションの読み込み(スケルトン)
//
// Parameter:
// *fp - DXFファイルへのファイルポインタ
// *body - BODYへのポインタ
//
// Return:
// KOD_TRUE
int DXF_PARSER::ReadObjectsSection(FILE *fp,int Line,BODY *body)
{
	return KOD_TRUE;
}

// Function: EvenOdd
// 奇数偶数判別
//
// Parameter:
// val - 整数値
//
// Return:
// 偶数：EVEN, 奇数：ODD
int DXF_PARSER::EvenOdd(int val)
{
	if(val%2 == 0)
		return EVEN;
	else
		return ODD;
}

// Function: CheckSection
// セクション判別
//
// Parameter:
// *str - 1行
//
// Return:
// 各セクションのシンボル
int DXF_PARSER::CheckSection(char *str)
{
	char section[LABELSIZEMAX_DXF];
	sscanf(str,"%s",section);

	if(strcmp(section,"HEADER") == 0)
		return HEADER_SECTION;
	else if(strcmp(section,"CLASSES") == 0)
		return CLASS_SECTION;
	else if(strcmp(section,"TABLES") == 0)
		return TABLE_SECTION;
	else if(strcmp(section,"BLOCKS") == 0)
		return BLOCK_SECTION;
	else if(strcmp(section,"ENTITIES") == 0)
		return ENTITIY_SECTION;
	else if(strcmp(section,"OBJECTS") == 0)
		return OBJECT_SECTION;
	else
		return KOD_ERR;
}

// Function: InitDisplayStat
// 各エンティティの表示属性を設定
//
// Parameter:
// *Dstat - 表示属性
void DXF_PARSER::InitDisplayStat(DispStat *Dstat)
{
	// 白色を設定
	Dstat->Color[0] = 1.0;
	Dstat->Color[1] = 1.0;
	Dstat->Color[2] = 1.0;
	Dstat->Color[3] = 0.5;

	// 表示属性を追加する場合は以下に追加のコードを記述
}
