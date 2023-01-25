#ifndef _STL_PARSER_MAIN_H_
#define _STL_PARSER_MAIN_H_

#include "BODY.h"

// Constants: General Defines
// BUFSIZEMAX_STL -		������ꎞ�i�[�p�o�b�t�@�̃T�C�Y(256)
// LABELSIZEMAX_STL -	���x��������ꎞ�i�[�p�o�b�t�@�̃T�C�Y(10)
// LABEL_FASET_SIZE -	"facet"������(5)
// LABEL_OUTER_SIZE -	"outer"������(5)
// VERTEXNUM -			1�̃t�@�Z�b�g�Ɋ܂܂��"vertex"�̐�(3)
// UVCPNUM -			1��NURBS�t�@�Z�b�g��1�����̃R���g���[���|�C���g��(2)
#define BUFSIZEMAX_STL		256
#define LABELSIZEMAX_STL	10
#define LABEL_FASET_SIZE	5
#define LABEL_OUTER_SIZE	5
#define VERTEXNUM			3
#define UVCPNUM				2

// Class: STL_PARSER
// STL�p�[�T�[�p�N���X
class STL_PARSER
{
public:
	// Function: STL_Parser_Main
	// STL�p�[�T���C��
    int STL_Parser_Main(BODY *, const char *);
};

#endif
