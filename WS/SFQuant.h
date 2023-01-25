#ifndef _SFQUANT_H_
#define _SFQUANT_H_

#include "BODY.h"

// �Ȗʂ̑��C����{�ʂ��i�[����N���X���`
class SFQuant
{
public:
	SFQuant();									// �R���X�g���N�^
	SFQuant(NURBSS *S,double u, double v);		// S(u,v)�ł̊�{�ʂ𓾂�(�R���X�g���N�^)
	int SetSFQ(NURBSS *S,double u, double v);	// S(u,v)�ł̊�{�ʂ𓾂�

public:
	double U,V;			// �Ȗʃp�����[�^
	Coord  n;			// �@���x�N�g��
	double E,F,G;		// ����{��
	double L,M,N;		// ����{��
};


#endif