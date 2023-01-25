#ifndef _STL_PARSER_MAIN_H_
#define _STL_PARSER_MAIN_H_

#include "BODY.h"

#define BUFSIZEMAX 256		// 文字列一時格納用バッファのサイズ
#define LABELSIZEMAX 10		// ラベル文字列一時格納用バッファのサイズ
#define LABEL_FASET_SIZE 5	// "facet"文字数
#define LABEL_OUTER_SIZE 5	// "outer"文字数
#define VERTEXNUM	3		// 1つのファセットに含まれる"vertex"の数
#define UVCPNUM	2			// 1つのNURBSファセットの1方向のコントロールポイント数

// STLパーサー用クラスを定義
class STL_PARSER
{
public:
	int STL_Parser_Main(BODY *,char []);	// STLファイルのパーサmain
};

#endif
