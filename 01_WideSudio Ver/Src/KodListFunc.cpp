#include "KodListFunc.h"

// �R���X�g���N�^
KODlistData::KODlistData()
{
	num = 0;
	head = NULL;
}

// ���X�g�ւ̓o�^���𓾂�
int KODlistData::getNum()
{
	return num;
}

// �V���ȃf�[�^�����X�g�Ō���ɓo�^
int KODlistData::add(void *idata)
{
	if(idata == NULL) return KOD_ERR;

	Data *newdata = new Data(idata);
	if(newdata == NULL)	return KOD_ERR;
	if(head == NULL){
		head = newdata;
		head->next = NULL;
	}
	else{
		curr = old = head;
		while(curr != NULL){
			old = curr;
			curr = curr->next;
		}
		curr = newdata;
		old->next = curr;
		curr->next = NULL;
	}
	num++;

	return KOD_TRUE;			
}

// i�Ԗڂ̃f�[�^�𓾂�
void *KODlistData::getData(int n)
{
	if(n >= num) return NULL;

	curr = head;
	for(int i=0;i<n;i++){
		curr = curr->next;
	}

	return curr->data;
}

// ���g�̏���
int KODlistData::clear()
{
	head = NULL;
	num = 0;

	return KOD_TRUE;
}

// i�Ԗڂ̃f�[�^�����X�g����͂���
int KODlistData::delPos(int n)
{
	if(n >= num) return KOD_ERR;

	if(head->next == NULL){
		//delete head->data;
		head = NULL;
		num=0;
		return KOD_TRUE;
	}

	if(n==0){
		head = head->next;
	}
	else{
		curr = head;
		for(int i=0;i<n;i++){
			old = curr;
			curr = curr->next;
		}
		old->next = curr->next;
	}
	//delete curr->data;
	num--;

	return KOD_TRUE;
}
	

