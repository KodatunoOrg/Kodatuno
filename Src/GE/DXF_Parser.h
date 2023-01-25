#ifndef _DXF_PARSER_MAIN_H_
#define _DXF_PARSER_MAIN_H_

#include "BODY.h"

#define SECTION_NAME	2		// セクション名を表すシンボル
#define HEADER_SECTION	1		// ヘッダセクションのシンボル
#define CLASS_SECTION	2		// クラスセクションのシンボル
#define TABLE_SECTION	3		// テーブルセクションのシンボル
#define BLOCK_SECTION	4		// ブロックセクションのシンボル
#define ENTITIY_SECTION	5		// エンティティセクションのシンボル
#define OBJECT_SECTION	6		// オブジェクトセクションのシンボル
#define ENT_TYPE		0		// エンティティタイプを表すシンボル

#define COORD_X			10		// X座標を表すシンボル
#define COORD_Y			20		// Y座標を表すシンボル
#define COORD_Z			30		// Z座標を表すシンボル
#define COORD_X_		11		// X座標を表すシンボル
#define COORD_Y_		21		// Y座標を表すシンボル
#define COORD_Z_		31		// Z座標を表すシンボル
#define RADIUS			40		// 半径を表すシンボル
#define START_ANG		50		// 円弧開始角度を表すシンボル
#define END_ANG			51		// 円弧終了角度を表すシンボル

#define BUFSIZEMAX		256		// 文字列一時格納用バッファのサイズ
#define LABELSIZEMAX	256		// ラベル文字列一時格納用バッファのサイズ
#define EVEN			0		// 偶数のシンボル
#define ODD				1		// 奇数のシンボル

// DXFパーサー用クラスを定義
class DXF_PARSER
{
public:
	DXF_PARSER();
	int DXF_Parser_Main(BODY *,const char *);	// DXFファイルのパーサmain

private:
	int ReadHeaderSection(FILE *,int,BODY *);		// ヘッダセクションの読み込み
	int ReadClassesSection(FILE *,int,BODY *);		// クラスセクションの読み込み
	int ReadTablesSection(FILE *,int,BODY *);		// テーブルセクションの読み込み
	int ReadBlocksSection(FILE *,int,BODY *);		// ブロックセクションの読み込み
	int ReadEntitiesSection(FILE *,int,BODY *);		// エンティティセクションの読み込み
	int ReadObjectsSection(FILE *,int,BODY *);		// オブジェクトセクションの読み込み
	int ResearchEntNum(FILE *,BODY *);				// 各エンティティの数をリサーチ
	int GetArcData(FILE *,BODY *);					// 円弧のデータを取得しBODYに格納
	int GetCircleData(FILE *,BODY *);				// 円のデータを取得しBODYに格納
	int GetLineData(FILE *,BODY *);					// 線分のデータを取得しBODYに格納
	int SetStartEndPtArc(CIRA *);					// 円弧の始点，終点をセット
	int CalcUVvec(CIRA *);							// CIRAのUV直交座標を設定する
	int EvenOdd(int);								// 奇数偶数判別
	int CheckSection(char *);						// セクション判別
	void InitDisplayStat(DispStat *);				// 各エンティティの表示属性を設定
	int ChangeEntityforNurbs(BODY *);				// NURBS曲線以外のエンティティをNURBS曲線に変換


private:
	char Buf[BUFSIZEMAX];		// 文字列一時格納用バッファ
	char Label[LABELSIZEMAX];	// ラベル文字列一時格納用バッファ
	int  Gcode;					// グループコード格納バッファ
	int  Count[ALL_ENTITY_TYPE_NUM];	// エンティティカウンタ


};


#endif