#ifndef _STL_PARSER_MAIN_H_
#define _STL_PARSER_MAIN_H_

#include "BODY.h"

// Constants: General Defines
// BUFSIZEMAX_STL -		文字列一時格納用バッファのサイズ(256)
// LABELSIZEMAX_STL -	ラベル文字列一時格納用バッファのサイズ(10)
// LABEL_FASET_SIZE -	"facet"文字数(5)
// LABEL_OUTER_SIZE -	"outer"文字数(5)
// VERTEXNUM -			1つのファセットに含まれる"vertex"の数(3)
// UVCPNUM -			1つのNURBSファセットの1方向のコントロールポイント数(2)
#define BUFSIZEMAX_STL		256
#define LABELSIZEMAX_STL	10
#define LABEL_FASET_SIZE	5
#define LABEL_OUTER_SIZE	5
#define VERTEXNUM			3
#define UVCPNUM				2

// Class: STL_PARSER
// STLパーサー用クラス
class STL_PARSER
{
public:
	// Function: STL_Parser_Main
	// STLパーサメイン
    int STL_Parser_Main(BODY *, const char *);
};

#endif
