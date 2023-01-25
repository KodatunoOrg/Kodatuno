#ifndef _KOD_LIST_FUNC_H_
#define _KOD_LIST_FUNC_H_

#include "StdAfx.h"

// ���X�gKODListadata�p�f�[�^�̎��̂��i�[���邽�߂̃N���X
class Data
{
	friend class KODlistData;

public:
	Data(void *a) { data = a; next = NULL; before = NULL;}		// �R���X�g���N�^
	Data()		  { data = NULL; next = NULL; before = NULL;}	// �R���X�g���N�^
	void SetNextpData(Data *d)		{next = d;}					// ����Data�I�u�W�F�N�g�ւ̃|�C���^���Z�b�g����
	void SetBeforepData(Data *d)	{before = d;}				// �O��Data�I�u�W�F�N�g�ւ̃|�C���^���Z�b�g����
	Data *GetNextpData()			{return next;}				// ����Data�I�u�W�F�N�g�ւ̃|�C���^�𓾂�
	Data *GetBeforepData()			{return before;}			// �O��Data�I�u�W�F�N�g�ւ̃|�C���^�𓾂�
	void *GetData()					{return data;}				// �f�[�^�̎��̂𓾂�

protected:
	void *data;			// �f�[�^�̎��̂͂��̃A�h���X�Ɋi�[�����
	Data *next;			// ���X�g�̑O�����w���|�C���^
	Data *before;		// ���X�g�̌�����w���|�C���^
};

// ���X�g����N���X
class KODlistData
{
public:
	KODlistData();					// �R���X�g���N�^
	int getNum();					// ���X�g�ւ̓o�^���𓾂�
	void *getData(int);				// n�Ԃ̃f�[�^�𓾂�(�f�[�^�ԍ���0����X�^�[�g����j
	int clear();					// ���g�̏���
	Data *add(void *);				// �V���ȃf�[�^�����X�g�ɓo�^
	Data *addData(Data *,void *);	// �V���ȃf�[�^���w�肵���f�[�^�̌��ɉ�����	
	Data *addData(int n,void *);	// �V���ȃf�[�^��n�Ԃ̃f�[�^�̌��ɉ�����
	int delData(int);				// n�Ԃ̃f�[�^�����X�g�������
	int delData(Data *);			// �w�肵���f�[�^�����X�g�������
	Data *snipData(Data *);			// �w�肵���f�[�^�����X�g����͂����i�����͂��Ȃ��j
	int insData(Data *,Data *);		// �w�肵���f�[�^A���w�肵���f�[�^B�̌��ɕύX����
	void ChangeHead(Data *);		// ���X�g�̐擪��ύX
	void ChangeEnd(Data *);			// ���X�g�̖�����ύX

	Data *getpData(int);			// n�Ԃ�Data�I�u�W�F�N�g�ւ̃|�C���^�𓾂�(�f�[�^�ԍ���0����X�^�[�g����j
	Data *getNextpData(Data *);		// �w�肵��Data�̎���Data�I�u�W�F�N�g�ւ̃|�C���^�𓾂�
	Data *getPrevpData(Data *);		// �w�肵��Data�̑O��Data�I�u�W�F�N�g�ւ̃|�C���^�𓾂�
	Data *getEndpData();			// �Ō��Data�I�u�W�F�N�g�ւ̃|�C���^�𓾂�

	// �Z���`�l������
	void *setSentinel(int);			// �w�肵���ʒu�Ɋ��𗧂Ă�
	void *getDataFromSentinel(int);	// ������n�Ԗڌ�or�O�̃f�[�^�𓾂�
	void *shiftSentinel(int);		// �������݂̈ʒu����n�Ԗڌ�or�O�Ɉړ�������
	void *getSentinelData();		// ���̂���ꏊ�̃f�[�^���擾����
	void *getNextSentData();		// ���̂���f�[�^�̎��̃f�[�^���擾����

private:
	Data *head;				// ���X�g�̐擪
	Data *end;				// ���X�g�̖���
	Data *sentinel;			// �f�[�^�T���p
	Data *curr;				// ��Ɨp
	Data *old;				// ��Ɨp
	int  num;				// �f�[�^���i�ŏ��̃f�[�^(0��)��1�ځj
};

#endif