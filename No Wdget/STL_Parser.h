#ifndef _STL_PARSER_MAIN_H_
#define _STL_PARSER_MAIN_H_

#include "BODY.h"

#define BUFSIZEMAX 256		// ������ꎞ�i�[�p�o�b�t�@�̃T�C�Y
#define LABELSIZEMAX 10		// ���x��������ꎞ�i�[�p�o�b�t�@�̃T�C�Y
#define LABEL_FASET_SIZE 5	// "facet"������
#define LABEL_OUTER_SIZE 5	// "outer"������
#define VERTEXNUM	3		// 1�̃t�@�Z�b�g�Ɋ܂܂��"vertex"�̐�
#define UVCPNUM	2			// 1��NURBS�t�@�Z�b�g��1�����̃R���g���[���|�C���g��

// STL�p�[�T�[�p�N���X���`
class STL_PARSER
{
public:
	int STL_Parser_Main(BODY *,char []);	// STL�t�@�C���̃p�[�Tmain
};

#endif
