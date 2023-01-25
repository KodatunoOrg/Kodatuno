#ifndef _SFQUANT_H_
#define _SFQUANT_H_

//#include "BODY.h"
#include "NURBS_Func.h"

// Constants: General Defines
// Non

// Class: SFQuant
// �Ȗʂ̑��C����{�ʂ��i�[����N���X���`
class SFQuant
{
public:
	// Constructor: SFQuant
	// �ϐ�������
	SFQuant();

	// Constructor: SFQuant
	// S(u,v)�ł̊�{�ʂ𓾂�
	SFQuant(NURBSS *S,double u, double v);

	// Function: SetSFQ
	// S(u,v)�ł̊�{�ʂ𓾂�
	int SetSFQ(NURBSS *S,double u, double v);

	// Function: CalcMeanCurvature
	// �I�[�o�[���[�h
	double CalcMeanCurvature(void);							

	// Function: CalcGaussCurvature
	// �I�[�o�[���[�h
	double CalcGaussCurvature(void);							

public:

	// Variables: U,V
	// �Ȗʃp�����[�^
	double U,V;			

	// Variables: n
	// �@���x�N�g��
	Coord  n;		

	// Variables:  E,F,G
	// ����{��
	double E,F,G;		

	// Variables: L,M,N
	// ����{��
	double L,M,N;		
};


#endif