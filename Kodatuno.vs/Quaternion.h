#ifndef _QUATERNION_H_
#define _QUATERNION_H_

//#include "StdAfx.h"

// Constants: General Defines
// TMATELEMNUM - ��]�s��z��̗v�f��(16)
#define TMATELEMNUM 16

// Structure: Quat
//
// Variables:
// t -		����
// x,y,z -	����
typedef struct{
	double t;
	double x,y,z;
}Quat;

// Class: QUATERNION
// �N�H�[�^�j�I�����Z�p���W���[��
class QUATERNION
{
public:
	// Function: QInit
	// �N�H�[�^�j�I���̏�����
	Quat QInit(double,double,double,double);	
	
	// Function: QGenRot
	// ��]�N�H�[�^�j�I������
	Quat QGenRot(double,double,double,double);	

	// Function: QRot
	// �N�H�[�^�j�I���ɂ��x�N�g���̉�]
	Quat QRot(Quat,Quat,Quat);					

	// Function: QConjugation
	// �����N�H�[�^�j�I���𐶐�
	Quat QConjugation(Quat);					

	// Function: QMult
	// �N�H�[�^�j�I���̊|���Z
	Quat QMult(Quat,Quat);						

	// Function: QtoR
	// �N�H�[�^�j�I��q����]�s��r(OpenGL�Ή��̂��߁A1����16�v�f�z��)�֕ϊ� (�K�v�ł���΁A2�����z��Ver���I�[�o�[���[�h���Ă�������)
	void QtoR(double [],Quat);					

	// Function: QtoR
	// �I�[�o�[���[�h(�����Fr,t,x,y,z)
	void QtoR(double [],double,double,double,double);	

	// Function: QCopy
	// �N�H�[�^�j�I���̃R�s�[ p <- q
	Quat QCopy(Quat);							

	// Function: QtoC
	// �N�H�[�^�j�I���̋�������Coord�Ƃ��ĕԂ�
	Coord QtoC(Quat);							

	// Function: CtoQ
	// �N�H�[�^�j�I��������0�A������Coord�Ƃ��ăN�H�[�^�j�I���𐶐�
	Quat CtoQ(Coord);							

	// Function: QSlerp
	// �N�H�[�^�j�I���ɂ�鋅�ʐ��`���
	Quat QSlerp(Quat,Quat,double);				
};

#endif
