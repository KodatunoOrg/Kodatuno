#ifndef _KOD_LIST_FUNC_H_
#define _KOD_LIST_FUNC_H_

//#include "StdAfx.h"

// Constants: General Defines
// Non

// Class: Data (friend class KODlistData)
// ���X�gKODListadata�p�f�[�^�̎��̂��i�[���邽�߂̃N���X
class Data
{
	friend class KODlistData;

public:
	// Constructor: Data
	// �R���X�g���N�^(�f�[�^���Z�b�g����)
	//
	// Parameters:
	// *a - �f�[�^�̃A�h���X���i�[
	Data(void *a) { data = a; next = NULL; before = NULL;}

	// Constructor: Data
	// �R���X�g���N�^(�S�Ă�NULL�ŏ�����)
	Data() { data = NULL; next = NULL; before = NULL;}

	// Function: SetNextpData
	// ����Data�ւ̃|�C���^���Z�b�g����
	//
	// Parameters:
	// *d - ���̃f�[�^�ւ̃|�C���^
	void SetNextpData(Data *d) {next = d;}

	// Function: SetBeforepData
	// �O��Data�ւ̃|�C���^���Z�b�g����
	//
	// Parameters:
	// *d - �O�̃f�[�^�ւ̃|�C���^
	void SetBeforepData(Data *d) {before = d;}

	// Function: GetNextpData
	// ����Data�ւ̃|�C���^�𓾂�
	//
	// Return:
	// ���̃f�[�^�ւ̃|�C���^
	Data *GetNextpData() {return next;}	

	// Function: GetNextpData
	// �O��Data�ւ̃|�C���^�𓾂�
	//
	// Return:
	// �O�̃f�[�^�ւ̃|�C���^
	Data *GetBeforepData() {return before;}

	// Function: GetData
	// �f�[�^�̎��̂𓾂�
	//
	// Return:
	// �f�[�^�̎���
	void *GetData() {return data;}		


protected:
	// Variable: *data
	// (protected)�f�[�^�̎��̂͂��̃A�h���X�Ɋi�[�����
	void *data;		

	// Variable: *next
	// (protected)���X�g�̑O�����w���|�C���^
	Data *next;			

	// Variable: *before
	// (protected)���X�g�̌�����w���|�C���^
	Data *before;		
};

// Class: KODlistData
// ���X�g����N���X
class KODlistData
{
public:

	// Function: KODlistData
	// �R���X�g���N�^
	KODlistData();					
	
	// Function: getNum
	// ���X�g�ւ̓o�^���𓾂�
	int getNum();					
	
	// Function: getData
	// n�Ԃ̃f�[�^�𓾂�(�f�[�^�ԍ���0����X�^�[�g����j
	void *getData(int);				
	
	// Function: clear
	// ���g�̏���
	int clear();					
	
	// Function: add
	// �V���ȃf�[�^�����X�g�ɓo�^
	Data *add(void *);				

	// Function: addData
	// �V���ȃf�[�^���w�肵���f�[�^�̌��ɉ�����	
	Data *addData(Data *,void *);	

	// Function: addData
	// �V���ȃf�[�^��n�Ԃ̃f�[�^�̌��ɉ�����
	Data *addData(int n,void *);	

	// Function: delData
	// n�Ԃ̃f�[�^�����X�g�������
	int delData(int);				

	// Function: delData
	// �w�肵���f�[�^�����X�g�������
	int delData(Data *);			

	// Function: snipData
	// �w�肵���f�[�^�����X�g����͂����i�����͂��Ȃ��j
	Data *snipData(Data *);			

	// Function: insData
	// �w�肵���f�[�^A���w�肵���f�[�^B�̌��ɕύX����
	int insData(Data *,Data *);		

	// Function: ChangeHead
	// ���X�g�̐擪��ύX
	void ChangeHead(Data *);		

	// Function: ChangeEnd
	// ���X�g�̖�����ύX
	void ChangeEnd(Data *);			

	// Function: getpData
	// n�Ԃ�Data�I�u�W�F�N�g�ւ̃|�C���^�𓾂�(�f�[�^�ԍ���0����X�^�[�g����j
	Data *getpData(int);			

	// Function: getNextpData
	// �w�肵��Data�̎���Data�I�u�W�F�N�g�ւ̃|�C���^�𓾂�
	Data *getNextpData(Data *);		

	// Function: getPrevpData
	// �w�肵��Data�̑O��Data�I�u�W�F�N�g�ւ̃|�C���^�𓾂�
	Data *getPrevpData(Data *);		

	// Function: getEndpData
	// �Ō��Data�I�u�W�F�N�g�ւ̃|�C���^�𓾂�
	Data *getEndpData();			

	// �Z���`�l������

	// Function: setSentinel
	// �w�肵���ʒu�Ɋ��𗧂Ă�
	void *setSentinel(int);			
	
	// Function: getDataFromSentinel
	// ������n�Ԗڌ�or�O�̃f�[�^�𓾂�
	void *getDataFromSentinel(int);	
	
	// Function: shiftSentinel
	// �������݂̈ʒu����n�Ԗڌ�or�O�Ɉړ�������
	void *shiftSentinel(int);		
	
	// Function: getSentinelData
	// ���̂���ꏊ�̃f�[�^���擾����
	void *getSentinelData();		
	
	// Function: getNextSentData
	// ���̂���f�[�^�̎��̃f�[�^���擾����
	void *getNextSentData();		

private:

	// Variable: *head
	// (private)���X�g�̐擪
	Data *head;

	// Variable: *end
	// ���X�g�̖���
	Data *end;	

	// Variable: *sentinel
	// �f�[�^�T����Ɨp
	Data *sentinel;

	// Variable: *curr
	// �f�[�^�T����Ɨp
	Data *curr;	

	// Variable: *old
	// �f�[�^�T����Ɨp
	Data *old;

	// Variable: num
	// �f�[�^���i�ŏ��̃f�[�^(0��)��1�ځj
	int  num;				
};

// Typedefs: KODlistData
// BODYList - �ėp�f�[�^���X�g�̌^��BODYList�Ƃ��čēo�^
// OBJECTList - �ėp�f�[�^���X�g�̌^��OBJECTList�Ƃ��čēo�^
typedef KODlistData BODYList;
typedef KODlistData OBJECTList;

#endif