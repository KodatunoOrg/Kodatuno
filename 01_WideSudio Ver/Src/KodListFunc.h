#ifndef _KOD_LIST_FUNC_H_
#define _KOD_LIST_FUNC_H_

#include "StdAfx.h"

// ���X�gKODListadata�p�f�[�^�̎��̂��i�[���邽�߂̃N���X
class Data
{
	friend class KODlistData;

private:
	void *data;
	Data *next;

public:
	Data(void *a) { data = a; next = NULL; };
};

// ���X�g����N���X
class KODlistData
{
public:
	KODlistData();			// �R���X�g���N�^
	int getNum();			// ���X�g�ւ̓o�^���𓾂�
	void *getData(int);		// i�Ԗڂ̃f�[�^�𓾂�
	int clear();			// ���g�̏���
	int add(void *);		// �V���ȃf�[�^�����X�g�ɓo�^
	int delPos(int);		// i�Ԗڂ̃f�[�^�����X�g����͂���

private:
	Data *head;				// �f�[�^�T���p
	Data *curr;				// �f�[�^�T���p
	Data *old;				// �f�[�^�T���p
	int  num;				// �f�[�^��
};

#endif