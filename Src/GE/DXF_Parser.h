#ifndef _DXF_PARSER_MAIN_H_
#define _DXF_PARSER_MAIN_H_

#include "BODY.h"

// Constants: General Defines
// SECTION_NAME -		セクション名を表すシンボル(2)
// HEADER_SECTIO -		ヘッダセクションのシンボル(1)
// CLASS_SECTION -		クラスセクションのシンボル(2)
// TABLE_SECTION -		テーブルセクションのシンボル(3)
// BLOCK_SECTION -		ブロックセクションのシンボル(4)
// ENTITIY_SECTION -	エンティティセクションのシンボル(5)
// OBJECT_SECTION -		オブジェクトセクションのシンボル(6)
// ENT_TYPE -			エンティティタイプを表すシンボル(0)
// COORD_X -			X座標を表すシンボル(10)
// COORD_Y -			Y座標を表すシンボル(20)
// COORD_Z -			Z座標を表すシンボル(30)
// COORD_X_ -			X座標を表すシンボル(11)
// COORD_Y_ -			Y座標を表すシンボル(21)
// COORD_Z_ -			Z座標を表すシンボル(31)
// RADIUS -				半径を表すシンボル(40)
// START_ANG -			円弧開始角度を表すシンボル(50)
// END_ANG -			円弧終了角度を表すシンボル(51)
// BUFSIZEMAX_DXF -		文字列一時格納用バッファのサイズ(256)
// LABELSIZEMAX_DXF -	ラベル文字列一時格納用バッファのサイズ(256)
// EVEN -				偶数のシンボル(0)
// ODD -				奇数のシンボル(1)
#define SECTION_NAME	2
#define HEADER_SECTION	1
#define CLASS_SECTION	2
#define TABLE_SECTION	3
#define BLOCK_SECTION	4
#define ENTITIY_SECTION	5
#define OBJECT_SECTION	6
#define ENT_TYPE		0
#define COORD_X			10
#define COORD_Y			20
#define COORD_Z			30
#define COORD_X_		11
#define COORD_Y_		21
#define COORD_Z_		31
#define RADIUS			40
#define START_ANG		50
#define END_ANG			51
#define BUFSIZEMAX_DXF		256
#define LABELSIZEMAX_DXF	256
#define EVEN			0
#define ODD				1

// Class: DXF_PARSER
// DXFパーサー用クラス
class DXF_PARSER
{
public:
	// Function: DXF_PARSER
	// コンストラクタ
	DXF_PARSER();

	// Function: DXF_Parser_Main
	// DXFファイルのパーサmain
	int DXF_Parser_Main(BODY *,const char *);	

private:
	// Function: ReadHeaderSection
	// (private)ヘッダセクションの読み込み
	int ReadHeaderSection(FILE *,int,BODY *);

	// Function: ReadClassesSection
	// (private)クラスセクションの読み込み
	int ReadClassesSection(FILE *,int,BODY *);
	
	// Function: ReadTablesSection
	// (private)テーブルセクションの読み込み
	int ReadTablesSection(FILE *,int,BODY *);
	
	// Function: ReadBlocksSection
	// (private)ブロックセクションの読み込み
	int ReadBlocksSection(FILE *,int,BODY *);
	
	// Function: ReadEntitiesSection
	// (private)エンティティセクションの読み込み
	int ReadEntitiesSection(FILE *,int,BODY *);
	
	// Function: ReadObjectsSection
	// (private)オブジェクトセクションの読み込み
	int ReadObjectsSection(FILE *,int,BODY *);
	
	// Function: ResearchEntNum
	// (private)各エンティティの数をリサーチ
	int ResearchEntNum(FILE *,BODY *);
	
	// Function: GetArcData
	// (private)円弧のデータを取得しBODYに格納
	int GetArcData(FILE *,BODY *);
	
	// Function: GetCircleData
	// (private)円のデータを取得しBODYに格納
	int GetCircleData(FILE *,BODY *);
	
	// Function: GetLineData
	// (private)線分のデータを取得しBODYに格納
	int GetLineData(FILE *,BODY *);
	
	// Function: SetStartEndPtArc
	// (private)円弧の始点，終点をセット
	int SetStartEndPtArc(CIRA *);
	
	// Function: CalcUVvec
	// (private)CIRAのUV直交座標を設定する
	int CalcUVvec(CIRA *);
	
	// Function: EvenOdd
	// (private)奇数偶数判別
	int EvenOdd(int);
	
	// Function: CheckSection
	// (private)セクション判別
	int CheckSection(char *);
	
	// Function: InitDisplayStat
	// (private)各エンティティの表示属性を設定
	void InitDisplayStat(DispStat *);
	
	// Function: ChangeEntityforNurbs
	// (private)NURBS曲線以外のエンティティをNURBS曲線に変換
	int ChangeEntityforNurbs(BODY *);


private:
	// Variable: Buf
	// (private)文字列一時格納用バッファ
	char Buf[BUFSIZEMAX_DXF];

	// Variable: Label
	// (private)ラベル文字列一時格納用バッファ
	char Label[LABELSIZEMAX_DXF];
	
	// Variable: Gcode
	// (private)グループコード格納バッファ
	int  Gcode;
	
	// Variable: Count
	// (private)エンティティカウンタ
	int  Count[ALL_ENTITY_TYPE_NUM];
};


#endif