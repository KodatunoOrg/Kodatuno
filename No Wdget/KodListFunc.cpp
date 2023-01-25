#include "KodListFunc.h"

// �R���X�g���N�^
KODlistData::KODlistData()
{
	num	 = 0;
	head = NULL;
	end  = NULL;
	sentinel = NULL;
}

// ���X�g�ւ̓o�^���𓾂�
int KODlistData::getNum()
{
	return num;
}

// �V���ȃf�[�^�����X�g�Ō���ɓo�^
// �Ԓl�̓f�[�^����ꂽ���̃A�h���X
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

// �V���ȃf�[�^���w�肵���f�[�^�̌��ɉ�����	
// �����@�@tag:�w��f�[�^    ins�F������f�[�^
// tag��NULL���w�肵���ꍇ�͐擪�ɉ�����
// �Ԓl�̓f�[�^����ꂽ���̃A�h���X
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

// �V���ȃf�[�^��n�Ԃ̃f�[�^�̌��ɉ�����	
// �����@�@n:n�Ԃ̃f�[�^    ins�F������f�[�^
// �Ԓl�̓f�[�^����ꂽ���̃A�h���X
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

// �w�肵���f�[�^A���w�肵���f�[�^B�̌��ɕύX����
// B��NULL�̏ꍇ�͐擪�ɑ}������
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


// n�Ԃ̃f�[�^�𓾂�
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

// ���g�̏���
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

// n�Ԃ̃f�[�^�����X�g�������
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

// �w�肵���f�[�^�����X�g�������
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

// �w�肵���f�[�^�����X�g����͂���
// ���ӁF�����͂��Ȃ��̂ŁA�͂����ꂽ�f�[�^�͎����ŏ������邱��
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


// ���X�g�̓���ύX
void KODlistData::ChangeHead(Data *d)
{
	head = d;
}

// ���X�g�̖�����ύX
void KODlistData::ChangeEnd(Data *d)
{
	end = d;
}


// n�Ԃ�Data�N���X�ւ̃|�C���^�𓾂�(�f�[�^�ԍ���0����X�^�[�g����j
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

// �w�肵��Data�̎���Data�ւ̃|�C���^�𓾂�
Data *KODlistData::getNextpData(Data *data)
{
	return data->next;
}

// �w�肵��Data�̎���Data�ւ̃|�C���^�𓾂�
Data *KODlistData::getPrevpData(Data *data)
{
	return data->before;
}

// �Ō��Data�I�u�W�F�N�g�ւ̃|�C���^�𓾂�
Data *KODlistData::getEndpData()
{
	return end;
}


// �w�肵���f�[�^�Ɋ��𗧂Ă�
// �Ԓl�͊��𗧂Ă��f�[�^�̃A�h���X
void *KODlistData::setSentinel(int n)
{
	if(n >= num) return NULL;

	sentinel = head;
	for(int i=0;i<n;i++){
		sentinel = sentinel->next;
	}

	return sentinel->data;
}

// ������n�Ԗڌ�or�O�̃f�[�^�𓾂�
// dn���}�C�i�X�̏ꍇ�͌������̒T��
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

// �������݂̈ʒu����n�Ԗڌ�or�O�Ɉړ�������
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

// ���̂���ꏊ�̃f�[�^���擾����
void *KODlistData::getSentinelData()
{
	if(sentinel == NULL)		// �����ݒ肳��Ă��Ȃ�������NULL��Ԃ�
		return NULL;

	return sentinel->data;
}

// ���̂���f�[�^�̎��̃f�[�^���擾����
void *KODlistData::getNextSentData()
{
	if(sentinel == NULL)		// �����ݒ肳��Ă��Ȃ�������NULL��Ԃ�
		return NULL;

	return sentinel->next->data;
}
