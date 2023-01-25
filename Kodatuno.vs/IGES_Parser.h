// IGESパーサ

#ifndef _IGES_PARSER_MAIN_H_
#define _IGES_PARSER_MAIN_H_

#include "BODY.h"

// Constants: General Defines
//#define SECTION_NUM -				セクションの数(S,G,D,P,T)(5)
//#define COLUMN_MAX_ -				1行のカラム数('\n'と'\0'を含む)(82)
//#define COLUMN_MAX -				1行のカラム数('\n'と'\0'は含まない)(80)
//#define COL_CHAR -				セクション判別文字のカラム(73)
//#define COL_P_DIRECTORY -			パラメータ部でのディレクトリ部への逆ポインタがあるカラム(65)
//#define GLOBALPARAMNUM -			グローバル部のパラメータ数(25)
//#define FIELD_NUM -				ディレクトリ部の1フィールドの文字数(8)
//#define DIRECTORYPARANUM -		ディレクトリ部のパラメータ数(20)
//#define SECTION_START -			スタートルセクション判別用シンボル(0)
//#define SECTION_GLOBAL -			グローバルセクション判別用シンボル(1)
//#define SECTION_DIRECTORY -		ディレクトリセクション判別用シンボル(2)
//#define SECTION_PARAMETER -		パラメータセクション判別用シンボル(3)
//#define SECTION_TERMINATE -		ターミネートセクション判別用シンボル(4)
//#define MAIN_ENTITY_TYPE_NUM -	読み込み対象となるエンティティタイプの数(9)
//#define NRBS_PARAM_MAX -			NRBS曲面・曲線で定義されるパラメータの最大値(200)
#define SECTION_NUM		 5			
#define COLUMN_MAX_	     82			
#define COLUMN_MAX       80			
#define COL_CHAR		 73			
#define COL_P_DIRECTORY  65			
#define GLOBALPARAMNUM	 25			
#define FIELD_NUM		 8			
#define DIRECTORYPARANUM 20			
#define SECTION_START		0		
#define SECTION_GLOBAL		1		
#define SECTION_DIRECTORY	2		
#define SECTION_PARAMETER	3		
#define SECTION_TERMINATE	4		
#define MAIN_ENTITY_TYPE_NUM 9		
#define NRBS_PARAM_MAX  200			


// グローバル部パラメータのシンボルを定義
// Enum: Enum Symbol of Global Section's Parameter
//PARAM_DELIMITER=1 -		パラメータデリミタ
//RECORD_DELIMITER -		レコードデリミタ
//SEND_PRODUCT_ID -			送り側製品ＩＤ
//FILE_NAME -				ファイル名
//SEND_SYSTEM_ID -			送り側システムＩＤ
//PRIRPO_VERSION -			プリプロセッサバージョン
//INT_LENGTH -				整数値の長さ
//FLOAT_PNT_LENGTH -		単精度浮動小数点指数の最大値
//FLOAT_PNT_FIGURE -		単精度浮動小数点有効桁数
//DOUBLE_PNT_LENGTH -		倍精度浮動小数点指数の最大値
//DOUBLE_PNT_FIGURE -		倍精度浮動小数点有効桁数
//RECIVE_PRODUCT_ID -		受け取り側製品ＩＤ
//MODEL_SCALE -				モデルスケール
//UNIT_FLAG -				単位フラグ
//UNIT -					単位
//LINE_THICKNESS_NUMBER -	線幅の最大類別数
//LINE_THICKNESS -			線幅の最大値
//DATE_MAKE_FILE -			ファイル作成日時
//MIN_ACCRACY -				最小精度
//MODEL_SPACE_SIZE -		モデル空間の大きさ
//FILE_MAKER_NAME -			ファイル作成者
//BELONG -					所属
//IGES_VERSION -			バージョン
//DRAFTING_STANDARD -		製図規格
//DATE_MAKE_MODEL -			モデル作成日
enum GlobalParamType{
	PARAM_DELIMITER=1,
	RECORD_DELIMITER,
	SEND_PRODUCT_ID,
	FILE_NAME,		
	SEND_SYSTEM_ID,	
	PRIRPO_VERSION,	
	INT_LENGTH,		
	FLOAT_PNT_LENGTH,
	FLOAT_PNT_FIGURE,
	DOUBLE_PNT_LENGTH,
	DOUBLE_PNT_FIGURE,
	RECIVE_PRODUCT_ID,
	MODEL_SCALE,	
	UNIT_FLAG,		
	UNIT,			
	LINE_THICKNESS_NUMBER,
	LINE_THICKNESS,		
	DATE_MAKE_FILE,		
	MIN_ACCRACY,		
	MODEL_SPACE_SIZE,	
	FILE_MAKER_NAME,	
	BELONG,				
	IGES_VERSION,		
	DRAFTING_STANDARD,	
	DATE_MAKE_MODEL		
};

// ディレクトリ部パラメータのシンボルを定義
// Enum: Enum Symbol of Directory Section's Parameter
	//ENTITY_TYPE_NUM -			要素番号
	//PARAM_DATA -				パラメータ部へのポインタ
	//STRUCTURE -				ストラクチャ
	//LINE_FONT_PATTERN -		線種
	//LEVEL -					レベル
	//VIEW_ -					ビュー
	//TRAN_MATRIX -				マトリックスへのポインタ
	//LABEL_DISP_ASSOC -		ラベル表示
	//STATUS_NUM -				ステイタス
	//SEQUENCE_NUM -			シーケンス番号
	//ENTITY_TYPE_NUM_ -		要素番号
	//LINE_WEIGHT_NUM -			線幅
	//COLOR_NUM -				色
	//PARAM_LINE_COUNT -		パラメータ部のライン数
	//FORM_NUM -				形式番号
	//RESERVED1 -				予備
	//RESERVED2 -				予備
	//ENTITY_LABEL -			要素のラベル
	//ENTITY_SUBSCRIPT_NUM -	サブスクリプト
	//SEQUENCE_NUM_ -			シーケンス番号
enum DirectoryParamType{
	ENTITY_TYPE_NUM,
	PARAM_DATA,		
	STRUCTURE,		
	LINE_FONT_PATTERN,
	LEVEL,			
	VIEW_,			
	TRAN_MATRIX,	
	LABEL_DISP_ASSOC,
	STATUS_NUM,		
	SEQUENCE_NUM,	
	ENTITY_TYPE_NUM_,
	LINE_WEIGHT_NUM,
	COLOR_NUM,		
	PARAM_LINE_COUNT,
	FORM_NUM,		
	RESERVED1,		
	RESERVED2,		
	ENTITY_LABEL,	
	ENTITY_SUBSCRIPT_NUM,
	SEQUENCE_NUM_		
};

// Structure: GlobalParam
// グローバル部の必要パラメータ構造体
//
// Variables:
// double	scale -			モデルのスケール
// int		unit_flag -		単位フラグ
// double	space_size -	モデル空間の大きさ
typedef struct{
	double scale;
	int    unit_flag;
	double space_size;
}GlobalParam;

// Structure: DirectoryParam
// ディレクトリ部の必要パラメータ構造体
//
// Variables:
// int entity_type -		要素番号
// int entity_count -		何番目のentity_typeかを表す
// int p_param -			パラメータ部へのポインタ
// int p_tm -				マトリックスへのポインタ
// int blank_stat -			表示属性
// int subordinate_stat -	従属属性
// int useflag_stat -		entity使用意図
// int seq_num -			シーケンス番号
// int param_line_count -	パラメータ部のライン数
typedef struct{
	int entity_type;
	int entity_count;
	int p_param;	
	int p_tm;		
	int blank_stat;	
	int subordinate_stat;	
	int useflag_stat;		
	int seq_num;			
	int param_line_count;	
}DirectoryParam;

// Class: IGES_PARSER
// IGESパーサー用クラス
class IGES_PARSER
{
public:
	// Constructor: IGES_PARSER
	// IGES_PARSERクラスのコンストラクタ
	IGES_PARSER();	

	//  Function: IGES_Parser_Main
	// IGESファイルのパーサmain
	int IGES_Parser_Main(BODY *, const char *);	

	//  Function: Optimize4OpenGL
	// 読み込んだIGESファイルをOpenGL用に最適化する(ExpandKnotRange(), ModifyParamConect(), CheckDegenracy(), CheckCWforTrim()を実行)
	int Optimize4OpenGL(BODY *);			

	//  Function: ExpandKnotRange
	// 隣り合うノットベクトルの差がMIN_KNOT_RANGE以上になるよう範囲を変更する
	int ExpandKnotRange(BODY *);			

	//  Function: ModifyParamConect
	// パラメトリック平面内のトリム曲線同士のつながりをチェック、修正する
	int ModifyParamConect(BODY *);			

	//  Function: CheckDegenracy
	// 縮退(2Dパラメトリック曲線の始点と終点が一致しているか)のチェック
	int CheckDegenracy(BODY *);				

	//  Function: CheckCWforTrim
	// トリムに使われている複合曲線からなる多角形が時計回りか反時計回りかを調べ、外周トリムは反時計回り、内周トリムは時計周りになるように変更する
	int CheckCWforTrim(BODY *);				

	//  Function: NormalizeKnotRange
	// ノットベクトルの範囲を0～valへ(ノットの正規化)
	int NormalizeKnotRange(BODY *,double);	


private:
	// Function: GetSectionLine
	// 各セクションのライン数を調べる
	void GetSectionLine(FILE *,int []);								

	// Function: GetStartSection
	// スタート部の読み込み
	int GetStartSection(FILE *,int);								

	// Function: GetGlobalSection
	// グローバル部の読み込み
	int GetGlobalSection(FILE *,GlobalParam *,int);					

	// Function: GetDirectorySection
	// ディレクトリ部の読み込み
	int GetDirectorySection(FILE *,DirectoryParam *,int [],int);	

	// Function: GetStatusNumber
	// ディレクトリ部の情報#9を取得
	void GetStatusNumber(char [],DirectoryParam *);					

	// Function: GetParameterSection
	// パラメータ部の読み込み
	int GetParameterSection(FILE *,DirectoryParam *,BODY,int);		

	// Function: GetTerminateSection
	// ターミネート部の読み込み（スケルトン）
	int GetTerminateSection(FILE *);								

	// Function: GetType
	// 各エンティティタイプの数を記憶する
	void GetType(int,int []);										

	// Function: GetCirAPara
	// Type100 円・円弧の読み込み
	int GetCirAPara(char [],int,DirectoryParam *,BODY);				

	// Function: GetCompCPara
	// Type102 複合曲線の読み込み
	int GetCompCPara(char [],int,DirectoryParam *,int,BODY);		

	// Function: GetConAPara
	// Type104 円錐曲線の読み込み
	int GetConAPara(char [],int,DirectoryParam *,BODY);				

	// Function: GetLinePara
	// Type110 線分の読み込み
	int GetLinePara(char [],int,DirectoryParam *,BODY);				

	// Function: GetTMatPara
	// Type124 変換行列の読み込み
	int GetTMatPara(char [],int,DirectoryParam *,BODY);				

	// Function: GetNurbsCPara
	// Type126 NRBS曲線の読み込み
	int GetNurbsCPara(char [],int,DirectoryParam *,BODY);			

	// Function: GetNurbsSPara
	// Type128 NRBS曲面の読み込み
	int GetNurbsSPara(char [],int,DirectoryParam *,BODY);			

	// Function: GeConpSPara
	// Type142 面上線の読み込み
	int GeConpSPara(char [],int,DirectoryParam *,int,BODY);			

	// Function: GetTrmSPara
	// Type144 トリム面の読み込み
	int GetTrmSPara(char [],int,DirectoryParam *,BODY);				

	// Function: CatchStringI
	// カンマまでの数値を読み込んで返す(int)
	int CatchStringI(char **);										

	// Function: CatchStringD
	// カンマまでの数値を読み込んで返す(double)
	double CatchStringD(char **);									

	// Function: ChangeEntityforNurbs
	// エンティティを全てNURBSへ変更する
	int ChangeEntityforNurbs(DirectoryParam *,BODY,int);			

	// Function: SearchMaxCoord
	// 全てのエンティティにおける座標値の最大値を調べる
	int SearchMaxCoord(BODY *,int []);	

	// Function: GetDEPointer
	// DE部へのポインタが示す実際の構造体へのポインタを返す
	void *GetDEPointer(int ,BODY);		

	// Function: SearchEntType
	// DE部へのポインタの値からエンティティのタイプを調べて返す
	int SearchEntType(DirectoryParam *,int,int);	

	// Function: InitDisplayStat
	// 各エンティティの表示属性を設定
	void InitDisplayStat(DispStat *);	

	// Function: TransformNurbsC
	// NURBS曲線を座標変換する
	int TransformNurbsC(int,int,BODY);

	// Function: ChangeKnotVecRange
	// ノットベクトルの正規化sub1
	int ChangeKnotVecRange(double [],double [],int,int,int,double);

	// Function: ChangeKnot
	// ノットベクトルの正規化sub2
	double ChangeKnot(double ,double ,double,double);	

	// Function: SearchMinVecRange
	// ノットベクトルの最小間隔を探索
	double SearchMinVecRange(double [],int,int);	

	// Function: ReverseCOMPELEM
	// COMPELEM配列を反転
	void ReverseCOMPELEM(COMPC *);


private:
	// Variable: *body
	// (private)BODYクラスへのポインタ(IGESデータはこのBODY構造体に全て格納される)
	BODY *body;

	// Variable: NFunc
	// (private)NURBS_Funcクラスのインスタンス
	NURBS_Func NFunc;

	// Variable: *TypeNum
	// (private)どのBODYオブジェクトが幾つあるかを示す
	int *TypeNum;

	// Variable: buf[COLUMN_MAX_]
	// (private)汎用文字列バッファ
	char buf[COLUMN_MAX_];				 

	// Variable: entity[ALL_ENTITY_TYPE_NUM]
	// (private)エンティティの番号を格納した配列
	int  entity[ALL_ENTITY_TYPE_NUM];	 

	// Variable: TypeCount[ALL_ENTITY_TYPE_NUM]
	// (private)各エンティティタイプの正常メモリー確保数を格納
	int  TypeCount[ALL_ENTITY_TYPE_NUM]; 
};

#endif
