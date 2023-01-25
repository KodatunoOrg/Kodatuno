#ifndef _QUATERNION_H_
#define _QUATERNION_H_

#include "StdAfx.h"

#define TMATELEMNUM 16		// ��]�s��z��̗v�f��

typedef struct{
	double t;		// ����
	double x,y,z;	// ����
}Quat;

// �N�H�[�^�j�I�����Z�p���W���[��
class QUATERNION
{
public:
	Quat QInit(double,double,double,double);	// �N�H�[�^�j�I���̏�����
	Quat QGenRot(double,double,double,double);	// ��]�N�H�[�^�j�I������
	Quat QRot(Quat,Quat,Quat);					// �N�H�[�^�j�I���ɂ��x�N�g���̉�]
	Quat QConjugation(Quat);					// �����N�H�[�^�j�I���𐶐�
	Quat QMult(Quat,Quat);						// �N�H�[�^�j�I���̊|���Z
	void QtoR(double [],Quat);					// �N�H�[�^�j�I��q����]�s��r(OpenGL�Ή��̂��߁A1����16�v�f�z��)�֕ϊ� (�K�v�ł���΁A2�����z��Ver���I�[�o�[���[�h���Ă�������)
	void QtoR(double [],double,double,double,double);	// �I�[�o�[���[�h(�����Fr,t,x,y,z)
	Quat QCopy(Quat);							// �N�H�[�^�j�I���̃R�s�[ p <- q
	Coord QtoC(Quat);							// �N�H�[�^�j�I���̋�������Coord�Ƃ��ĕԂ�
};

#endif
