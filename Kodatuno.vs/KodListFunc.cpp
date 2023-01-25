#include "stdafx.h"
#include "KodListFunc.h"

#if defined(_DEBUG) && defined(_MSC_VER)
#define new DEBUG_NEW
#endif

// Function: KODlistData
// �R���X�g���N�^(������)
KODlistData::KODlistData()
{
	num	 = 0;
	head = NULL;
	end  = NULL;
	sentinel = NULL;
}

// Function: getNum
// ���X�g�ւ̓o�^���𓾂�
//
// Return:
// ���X�g�ւ̓o�^��
int KODlistData::getNum()
{
	return num;
}

// Function: add
// �V���ȃf�[�^�����X�g�Ō���ɓo�^
//
// Parameters:
// *idata - �V���ɓo�^����f�[�^�ւ̃|�C���^
//
// Return:
// �f�[�^����ꂽ���̃A�h���X
Data *KODlistData::add(void *idata)
{
	if(idata == NULL) return NULL;

	Data *newdata = new Data(idata);
	if(newdata == NULL)	return NULL;

	if(head == NULL){
		head = newdata;
		head->next = NULL;
		head->before = NULL;
		end = head;
	}
	else{
		curr = end;
		curr->next = newdata;
		end = curr->next;
		end->next = NULL;
		end->before = curr;
	}
	num++;

	return newdata;			
}

// Function: addData
// �V���ȃf�[�^���w�肵���f�[�^�̌��ɉ�����
//
// Parameters:
// *tag - �w��f�[�^ (NULL���w�肵���ꍇ�͐擪�ɉ�����)    
// *ins - ������f�[�^
//
// Return:
// �f�[�^����ꂽ���̃A�h���X
Data *KODlistData::addData(Data *tag,void *ins)
{
	if(ins == NULL) return NULL;
		
	Data *nd = new Data(ins);
	if(nd == NULL)	return NULL;

	if(tag == NULL){
		nd->next = head;
		head->before = nd;
		head = nd;
	}
	else{
		nd->before = tag;
		nd->next = tag->next;
		tag->next = nd;
		tag->next->before = nd;
		if(nd->next == NULL) end = nd;
	}

	num++;

	return nd;
}

// Function: addData
// �V���ȃf�[�^��n�Ԃ̃f�[�^�̌��ɉ�����(�I�[�o�[���[�h)
//
// Parameters:
// n - n�Ԃ̃f�[�^    
// *ins - ������f�[�^
//
// Return:
// �f�[�^����ꂽ���̃A�h���X
Data *KODlistData::addData(int n,void *ins)
{
	if(n >= num) return NULL;
	if(ins == NULL) return NULL;

	if(n < int(num/2)){
		curr = head;
		for(int i=0;i<n;i++){
			curr = curr->next;
		}
	}
	else{
		curr = end;
		for(int i=num-1;i>n;i--){
			curr = curr->before;
		}
	}

	return (addData(curr,ins));
}

// Function: insData
// �w�肵���f�[�^A���w�肵���f�[�^B�̌��ɕύX����
//
// Parameters:
// *A - �ύX����f�[�^�|�C���^
// *B - A�̑O�̃f�[�^�������|�C���^ (NULL�̏ꍇ�͐擪�ɑ}������)
//
// Return:
// KOD_TRUE
int KODlistData::insData(Data *A,Data *B)
{
	if(A == NULL) return KOD_ERR;

	//snipData(A);
	//fprintf(stderr,"Out:%p-%p-%p\n",A->before,A,A->next);

	
	if(B == NULL){
		A->next = head;
		head->before = A;
		head = A;
		A->before = NULL;
	}
	else{
		A->before = B;
		A->next = B->next;
		if(B->next != NULL)
			B->next->before = A;
		B->next = A;
		if(A->next == NULL) end = A;
	}
	//fprintf(stderr,"In:%p-%p-%p\n",A->before,A,A->next);
	num++;

	return KOD_TRUE;
}

// Function: getData
// n�Ԃ̃f�[�^�𓾂�
//
// Parameters:
// n - �f�[�^�ԍ�
//
// Return:
// n�Ԃ̃f�[�^�ւ̃|�C���^
void *KODlistData::getData(int n)
{
	if(n >= num) return NULL;
	if(n == 0)	return head->data;
	if(n == num-1) return end->data;

	if(n < int(num/2)){
		curr = head;
		for(int i=0;i<n;i++){
			curr = curr->next;
		}
	}
	else{
		curr = end;
		for(int i=num-1;i>n;i--){
			curr = curr->before;
		}
	}
	return curr->data;
}

// Function: clear
// ���g�̏���
//
// Return:
// KOD_TRUE (���������f�[�^���Ȃ�������KOD_FALSE�����^�[��)
int KODlistData::clear()
{
	if(head == NULL)	return KOD_FALSE;	// �������������Ȃ�������FALSE�����^�[��

	curr = old = head;
	while(curr != NULL){
		curr = old->next;
		delete old;
		old = curr;
	}
	head = NULL;
	end = NULL;
	num = 0;

	return KOD_TRUE;
}

// Function: delData
// n�Ԃ̃f�[�^�����X�g�������
//
// Parameters:
// n - ��������f�[�^�̔ԍ�
//
// Return:
// KOD_TRUE (n�����X�g�ɓo�^����Ă���f�[�^����葽���ꍇ�FKOD_ERR)
int KODlistData::delData(int n)
{
	if(n >= num) return KOD_ERR;

	if(head->next == NULL){
		delete head;
		head = NULL;
		num=0;
		return KOD_TRUE;
	}

	curr = head;
	if(n==0){
		head = head->next;
		head->before = NULL;
	}
	else{
		for(int i=0;i<n;i++){
			old = curr;
			curr = curr->next;
		}
		old->next = curr->next;
		if(curr != end)
			curr->next->before = old;
		else
			end = old;
	}
	delete curr;
	num--;

	return KOD_TRUE;
}

// Function: delData
// �w�肵���f�[�^�����X�g������� (�I�[�o�[���[�h)
//
// Parameters:
// *d - ��������f�[�^�̃|�C���^
//
// Return:
// KOD_TRUE (d��NULL�FKOD_ERR)
int KODlistData::delData(Data *d)
{
	if(d == NULL)	return KOD_ERR;

	if(head->next == NULL){
		delete head;
		head = NULL;
	}
	else if(d->before == NULL){
		curr = d->next;
		curr->before = NULL;
		head = curr;
	}
	else{
		curr = d->before;
		curr->next = d->next;
		if(d->next != NULL){
			curr = d->next;
			curr->before = d->before;
		}
		else
			end = curr;
	}

	delete d;
	num--;

	return KOD_TRUE;
}

// Function: snipData
// �w�肵���f�[�^�����X�g����͂���
//
// ���ӁF�����͂��Ȃ��̂ŁA�͂����ꂽ�f�[�^�͎����ŏ������邱��
//
// Parameters:
// *d - �͂����f�[�^�̃|�C���^
//
// Return:
// �͂����ꂽ�f�[�^�ւ̃|�C���^ (d��NULL�������ꍇ�CNULL�����^�[��)
Data *KODlistData::snipData(Data *d)
{
	if(d == NULL)	return NULL;

	if(d->before == NULL){
		curr = d->next;
		curr->before = NULL;
		head = curr;
	}
	else{
		curr = d->before;
		curr->next = d->next;
		if(d->next != NULL){
			curr = d->next;
			curr->before = d->before;
		}
		else
			end = curr;
	}
	d->before = NULL;
	d->next = NULL;

	num--;

	return d;
}

// Function: ChangeHead
// ���X�g�̐擪���w�肵���f�[�^�ɕύX����
// 
// Parameters:
// *d - �ύX����f�[�^�̃|�C���^
void KODlistData::ChangeHead(Data *d)
{
	head = d;
}

// Function: ChangeEnd
// ���X�g�̖������w�肵���f�[�^�ɕύX����
// 
// Parameters:
// *d - �ύX����f�[�^�̃|�C���^
void KODlistData::ChangeEnd(Data *d)
{
	end = d;
}

// Function: getpData
// n�Ԃ�Data�N���X�ւ̃|�C���^�𓾂�(�f�[�^�ԍ���0����X�^�[�g����j
//
// Parameters:
// n - �f�[�^�ԍ�
//
// Return:
// �w�肳�ꂽn�Ԃ̃f�[�^�̃|�C���^
Data *KODlistData::getpData(int n)
{
	if(n >= num) return NULL;

	if(n < int(num/2)){
		curr = head;
		for(int i=0;i<n;i++){
			curr = curr->next;
		}
	}
	else{
		curr = end;
		for(int i=num-1;i>n;i--){
			curr = curr->before;
		}
	}

	return curr;
}

// Function: getNextpData
// �w�肵��Data�̎���Data�ւ̃|�C���^�𓾂�
// 
// Parameters:
// *data - �w�肷��Data�ւ̃|�C���^
//
// Return:
// �w�肵��Data�̎���Data�ւ̃|�C���^
Data *KODlistData::getNextpData(Data *data)
{
	return data->next;
}

// Function: getPrevpData
// �w�肵��Data�̑O��Data�ւ̃|�C���^�𓾂�
// 
// Parameters:
// *data - �w�肷��Data�ւ̃|�C���^
//
// Return:
// �w�肵��Data�̑O��Data�ւ̃|�C���^
Data *KODlistData::getPrevpData(Data *data)
{
	return data->before;
}

// Function: getEndpData
// �Ō��Data�ւ̃|�C���^�𓾂�
//
// Return:
// �Ō��Data�ւ̃|�C���^
Data *KODlistData::getEndpData()
{
	return end;
}

// Function: setSentinel
// �w�肵���f�[�^�Ɋ��𗧂Ă�
//
// Parameters:
// n - ���𗧂Ă�f�[�^�̔ԍ�
// 
// Return:
// ���𗧂Ă��f�[�^�̃A�h���X (n�����X�g�������傫�ꍇ��NULL�����^�[��)
void *KODlistData::setSentinel(int n)
{
	if(n >= num) return NULL;

	sentinel = head;
	for(int i=0;i<n;i++){
		sentinel = sentinel->next;
	}

	return sentinel->data;
}

// Function: getDataFromSentinel
// ������dn�Ԗڌ�or�O�̃f�[�^�𓾂� (dn���}�C�i�X�̏ꍇ�͌������̒T��)
// 
// Parameters:
// dn - �����牽�Ԗڂ���\�킷
//
// Return:
// �w��f�[�^�ւ̃|�C���^ (�����ݒ肳��Ă��Ȃ������ꍇ�����,�f�[�^��������Ȃ������ꍇ��NULL�����^�[��)
void *KODlistData::getDataFromSentinel(int dn)
{
	if(sentinel == NULL)		// �����ݒ肳��Ă��Ȃ�������NULL��Ԃ�
		return NULL;

	bool flag;
	if(dn < 0){
		flag = false;
		dn *= -1;
	}
	else{
		flag = true;
	}

	// �O����
	if(flag == true){
		curr = sentinel;
		for(int i=0;i<dn;i++){
			curr = curr->next;
			if(curr == NULL){		// �f�[�^���I�[�֗��Ă��܂�����
				return NULL;		// NULL��Ԃ�
			}
		}
		return curr->data;
	}
	// �����
	else if(flag == false){
		curr = sentinel;
		for(int i=0;i<dn;i++){
			curr = curr->before;	
			if(curr == NULL){		// �f�[�^���擪�֗��Ă��܂�����
				return NULL;		// NULL��Ԃ�
			}
		}
		return curr->data;
	}

	return NULL;
}

// Function: shiftSentinel
// �������݂̈ʒu����dn�Ԗڌ�or�O�Ɉړ������� (dn���}�C�i�X�̏ꍇ�͌������Ɉړ�)
//
// Parameters:
// ���̈ړ���
//
// Return:
// �ړ���̊��̃A�h���X (�����ݒ肳��Ă��Ȃ�������NULL��Ԃ�)
void *KODlistData::shiftSentinel(int dn)
{
	if(sentinel == NULL)		// �����ݒ肳��Ă��Ȃ�������NULL��Ԃ�
		return NULL;

	bool flag;
	if(dn < 0){
		flag = false;
		dn *= -1;
	}
	else{
		flag = true;
	}

	// �O����
	if(flag == true){
		for(int i=0;i<dn;i++){
			sentinel = sentinel->next;
			if(sentinel == NULL) return NULL;
		}
		return sentinel->data;
	}
	// �����
	else if(flag == false){
		for(int i=0;i<dn;i++){
			sentinel = sentinel->before;
			if(sentinel == NULL) return NULL;
		}
		return sentinel->data;
	}

	return NULL;
}

// Function: getSentinelData
// ���̂���ꏊ�̃f�[�^���擾����
//
// Return:
// ���̂���ꏊ�̃f�[�^�ւ̃|�C���^
void *KODlistData::getSentinelData()
{
	if(sentinel == NULL)		// �����ݒ肳��Ă��Ȃ�������NULL��Ԃ�
		return NULL;

	return sentinel->data;
}

// Function: getNextSentData
// ���̂���f�[�^�̎��̃f�[�^���擾����
//
// Return:
// ���̂���ꏊ�̎��̃f�[�^�ւ̃|�C���^
void *KODlistData::getNextSentData()
{
	if(sentinel == NULL)		// �����ݒ肳��Ă��Ȃ�������NULL��Ԃ�
		return NULL;

	return sentinel->next->data;
}
