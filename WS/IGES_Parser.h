#ifndef _IGES_PARSER_MAIN_H_
#define _IGES_PARSER_MAIN_H_

#include "BODY.h"

#define SECTION_NUM		 5			// セクションの数(S,G,D,P,T)
#define COLUMN_MAX_	     82			// 1行のカラム数('\n'と'\0'を含む)
#define COLUMN_MAX       80			// 1行のカラム数('\n'と'\0'は含まない)
#define COL_CHAR		 73			// セクション判別文字のカラム
#define COL_P_DIRECTORY  65			// パラメータ部でのディレクトリ部への逆ポインタがあるカラム
#define GLOBALPARAMNUM	 25			// グローバル部のパラメータ数
#define FIELD_NUM		 8			// ディレクトリ部の1フィールドの文字数
#define DIRECTORYPARANUM 20			// ディレクトリ部のパラメータ数
#define SECTION_START		0		// スタートルセクション判別用シンボル
#define SECTION_GLOBAL		1		// グローバルセクション判別用シンボル
#define SECTION_DIRECTORY	2		// ディレクトリセクション判別用シンボル
#define SECTION_PARAMETER	3		// パラメータセクション判別用シンボル
#define SECTION_TERMINATE	4		// ターミネートセクション判別用シンボル
#define MAIN_ENTITY_TYPE_NUM 9		// 読み込み対象となるエンティティタイプの数
#define NRBS_PARAM_MAX  200			// NRBS曲面・曲線で定義されるパラメータの最大値


// グローバル部パラメータのシンボルを定義
enum GlobalParamType{
	PARAM_DELIMITER=1,			// パラメータデリミタ
	RECORD_DELIMITER,			// レコードデリミタ
	SEND_PRODUCT_ID,			// 送り側製品ＩＤ
	FILE_NAME,					// ファイル名
	SEND_SYSTEM_ID,				// 送り側システムＩＤ
	PRIRPO_VERSION,				// プリプロセッサバージョン
	INT_LENGTH,					// 整数値の長さ
	FLOAT_PNT_LENGTH,			// 単精度浮動小数点指数の最大値
	FLOAT_PNT_FIGURE,			// 単精度浮動小数点有効桁数
	DOUBLE_PNT_LENGTH,			// 倍精度浮動小数点指数の最大値
	DOUBLE_PNT_FIGURE,			// 倍精度浮動小数点有効桁数
	RECIVE_PRODUCT_ID,			// 受け取り側製品ＩＤ
	MODEL_SCALE,				// モデルスケール
	UNIT_FLAG,					// 単位フラグ
	UNIT,						// 単位
	LINE_THICKNESS_NUMBER,		// 線幅の最大類別数
	LINE_THICKNESS,				// 線幅の最大値
	DATE_MAKE_FILE,				// ファイル作成日時
	MIN_ACCRACY,				// 最小精度
	MODEL_SPACE_SIZE,			// モデル空間の大きさ
	FILE_MAKER_NAME,			// ファイル作成者
	BELONG,						// 所属
	IGES_VERSION,				// バージョン
	DRAFTING_STANDARD,			// 製図規格
	DATE_MAKE_MODEL				// モデル作成日
};

// ディレクトリ部パラメータのシンボルを定義
enum DirectoryParamType{
	ENTITY_TYPE_NUM,			// 要素番号
	PARAM_DATA,					// パラメータ部へのポインタ
	STRUCTURE,					// ストラクチャ
	LINE_FONT_PATTERN,			// 線種
	LEVEL,						// レベル
	VIEW_,						// ビュー
	TRAN_MATRIX,				// マトリックスへのポインタ
	LABEL_DISP_ASSOC,			// ラベル表示
	STATUS_NUM,					// ステイタス
	SEQUENCE_NUM,				// シーケンス番号
	ENTITY_TYPE_NUM_,			// 要素番号
	LINE_WEIGHT_NUM,			// 線幅
	COLOR_NUM,					// 色
	PARAM_LINE_COUNT,			// パラメータ部のライン数
	FORM_NUM,					// 形式番号
	RESERVED1,					// 予備
	RESERVED2,					// 予備
	ENTITY_LABEL,				// 要素のラベル
	ENTITY_SUBSCRIPT_NUM,		// サブスクリプト
	SEQUENCE_NUM_				// シーケンス番号
};

// グローバル部の必要パラメータ構造体
typedef struct{
	double scale;			// モデルのスケール
	int    unit_flag;		// 単位フラグ
	double space_size;		// モデル空間の大きさ
}GlobalParam;

// ディレクトリ部の必要パラメータ構造体
typedef struct{
	int entity_type;		// 要素番号
	int entity_count;		// 何番目のentity_typeかを表す
	int p_param;			// パラメータ部へのポインタ
	int p_tm;				// マトリックスへのポインタ
	int blank_stat;			// 表示属性
	int subordinate_stat;	// 従属属性
	int useflag_stat;		// entity使用意図
	int seq_num;			// シーケンス番号
	int param_line_count;	// パラメータ部のライン数
}DirectoryParam;


// IGESパーサー用クラスを定義
class IGES_PARSER
{
public:
	int IGES_Parser_Main(BODY *,char []);	// IGESファイルのパーサmain
	int Optimize4OpenGL(BODY *);			// 読み込んだIGESファイルをOpenGL用に最適化する(ExpandKnotRange(), ModifyParamConect(), CheckDegenracy(), CheckCWforTrim()を実行)
	int ExpandKnotRange(BODY *);			// 隣り合うノットベクトルの差がMIN_KNOT_RANGE以上になるよう範囲を変更する
	int ModifyParamConect(BODY *);			// パラメトリック平面内のトリム曲線同士のつながりをチェック、修正する
	int CheckDegenracy(BODY *);				// 縮退(2Dパラメトリック曲線の始点と終点が一致しているか)のチェック
	int CheckCWforTrim(BODY *);				// トリムに使われている複合曲線からなる多角形が時計回りか反時計回りかを調べ、外周トリムは反時計回り、内周トリムは時計周りになるように変更する
	int NormalizeKnotRange(BODY *,double);	// ノットベクトルの範囲を0～valへ(ノットの正規化)
	IGES_PARSER();					// コンストラクタ

private:
	void GetSectionLine(FILE *,int []);								// 各セクションのライン数を調べる
	int GetStartSection(FILE *,int);								// スタート部の読み込み
	int GetGlobalSection(FILE *,GlobalParam *,int);					// グローバル部の読み込み
	int GetDirectorySection(FILE *,DirectoryParam *,int [],int);	// ディレクトリ部の読み込み
	void GetStatusNumber(char [],DirectoryParam *);					// ディレクトリ部の情報#9を取得
	int GetParameterSection(FILE *,DirectoryParam *,BODY,int);		// パラメータ部の読み込み
	int GetTerminateSection(FILE *);								// ターミネート部の読み込み（スケルトン）
	void GetType(int,int []);										// 各エンティティタイプの数を記憶する
	int GetCirAPara(char [],int,DirectoryParam *,BODY);				// Type100 円・円弧の読み込み
	int GetCompCPara(char [],int,DirectoryParam *,int,BODY);		// Type102 複合曲線の読み込み
	int GetConAPara(char [],int,DirectoryParam *,BODY);				// Type104 円錐曲線の読み込み
	int GetLinePara(char [],int,DirectoryParam *,BODY);				// Type110 線分の読み込み
	int GetTMatPara(char [],int,DirectoryParam *,BODY);				// Type124 変換行列の読み込み
	int GetNurbsCPara(char [],int,DirectoryParam *,BODY);			// Type126 NRBS曲線の読み込み
	int GetNurbsSPara(char [],int,DirectoryParam *,BODY);			// Type128 NRBS曲面の読み込み
	int GeConpSPara(char [],int,DirectoryParam *,int,BODY);			// Type142 面上線の読み込み
	int GetTrmSPara(char [],int,DirectoryParam *,BODY);				// Type144 トリム面の読み込み
	int CatchStringI(char **);										// カンマまでの数値を読み込んで返す(int)
	double CatchStringD(char **);									// カンマまでの数値を読み込んで返す(double)
	int ChangeEntityforNurbs(DirectoryParam *,BODY,int);			// エンティティを全てNURBSへ変更する
	int SearchMaxCoord(BODY *,int []);								// 全てのエンティティにおける座標値の最大値を調べる
	void *GetDEPointer(int ,BODY);									// DE部へのポインタが示す実際の構造体へのポインタを返す
	int SearchEntType(DirectoryParam *,int,int);					// DE部へのポインタの値からエンティティのタイプを調べて返す
	void InitDisplayStat(DispStat *);								// 各エンティティの表示属性を設定
	int GetNurbsCFromLine(int,int,BODY);							// Type110からType126のパラメータデータを得る
	int GetNurbsCFromCirA(int,int,BODY);					// Type100からType126のパラメータデータを得る
	int CirAToNurbsC_seg1(int,int,BODY,Coord [],double);	// Type100が1セグメントの場合
	int CirAToNurbsC_seg2(int,int,BODY,Coord [],double);	// Type100が2セグメントの場合
	int CirAToNurbsC_seg3(int,int,BODY,Coord [],double);	// Type100が3セグメントの場合
	int CirAToNurbsC_seg4(int,int,BODY,Coord [],double);	// Type100が4セグメントの場合
	int TransformNurbsC(int,int,BODY);						// NURBS曲線を座標変換する
	int ChangeKnotVecRange(double [],double [],int,int,int,double);	// ノットベクトルの正規化sub1
	double ChangeKnot(double ,double ,double,double);				// ノットベクトルの正規化sub2
	double SearchMinVecRange(double [],int,int);			// ノットベクトルの最小間隔を探索
	void ReverseCOMPELEM(COMPC *);							// COMPELEM配列を反転

private:
	BODY *body;
	NURBS_Func NFunc;
	int *TypeNum;
	char buf[COLUMN_MAX_];				 // 汎用文字列バッファ
	int  entity[ALL_ENTITY_TYPE_NUM];	 // エンティティの番号を格納した配列
	int  TypeCount[ALL_ENTITY_TYPE_NUM]; // 各エンティティタイプの正常メモリー確保数を格納
};

#endif
