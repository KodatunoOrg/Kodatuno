#include <stdlib.h>
#include "BODY.h"


// BODY�N���X�̃������[�m��
void New_body(BODY *Body,int TypeNum[])
{
	int flag=0;

	// �G���e�B�e�B��V���ɒǉ�����ꍇ�͈ȉ��ɐV����malloc���L�q���Ă��������B
	// �G���e�B�e�B�^�C�v�̔ԍ����Ⴂ���ɋL�q
	if(TypeNum[_CIRCLE_ARC]){
		if((Body->CirA = (CIRA *)malloc(sizeof(CIRA)*TypeNum[_CIRCLE_ARC])) == NULL)  goto EXIT;
		flag = _CIRCLE_ARC+1;
	}

	if(TypeNum[_COMPOSITE_CURVE]){
		if((Body->CompC = (COMPC *)malloc(sizeof(COMPC)*TypeNum[_COMPOSITE_CURVE])) == NULL)  goto EXIT;
		flag = _COMPOSITE_CURVE+1;
	}

	if(TypeNum[_CONIC_ARC]){
		if((Body->ConA = (CONA *)malloc(sizeof(CONA)*TypeNum[_CONIC_ARC])) == NULL)  goto EXIT;
		flag = _CONIC_ARC+1;
	}

	if(TypeNum[_LINE]){
		if((Body->Line = (LINE_ *)malloc(sizeof(LINE_)*TypeNum[_LINE])) == NULL)  goto EXIT;
		flag = _LINE+1;
	}

	if(TypeNum[_TRANSFORMATION_MATRIX]){
		if((Body->TMat = (TMAT *)malloc(sizeof(TMAT)*TypeNum[_TRANSFORMATION_MATRIX])) == NULL)  goto EXIT;
		flag = _TRANSFORMATION_MATRIX+1;
	}

	if(TypeNum[_NURBSC]){
		if((Body->NurbsC = (NURBSC *)malloc(sizeof(NURBSC)*TypeNum[_NURBSC])) == NULL)  goto EXIT;
		flag = _NURBSC+1;
	}

	if(TypeNum[_NURBSS]){
		if((Body->NurbsS = (NURBSS *)malloc(sizeof(NURBSS)*TypeNum[_NURBSS])) == NULL)  goto EXIT;
		flag = _NURBSS+1;
	}

	if(TypeNum[_CURVE_ON_PARAMETRIC_SURFACE]){
		if((Body->ConpS = (CONPS *)malloc(sizeof(CONPS)*TypeNum[_CURVE_ON_PARAMETRIC_SURFACE])) == NULL)  goto EXIT;
		flag = _CURVE_ON_PARAMETRIC_SURFACE+1;
	}

	if(TypeNum[_TRIMMED_SURFACE]){
		if((Body->TrmS = (TRMS *)malloc(sizeof(TRMS)*TypeNum[_TRIMMED_SURFACE])) == NULL)  goto EXIT;
		flag = _TRIMMED_SURFACE+1;
	}

	return;		// �������[�𐳏�Ɋm��


EXIT:	// �������[�m�ۂɎ��s�����ꍇ�́A����܂Ŋm�ۂ�������������ďI��
	fprintf(stderr,"ERROR: malloc BODY\n");
	while(flag){
		if(flag == _CURVE_ON_PARAMETRIC_SURFACE+1 && TypeNum[_TRIMMED_SURFACE]){
			free(Body->ConpS);
		}
		else if(flag == _NURBSS+1 && TypeNum[_NURBSS]){
			free(Body->NurbsS);
		}
		else if(flag == _NURBSC+1 && TypeNum[_NURBSC]){
			free(Body->NurbsC);
		}
		else if(flag == _TRANSFORMATION_MATRIX+1 && TypeNum[_TRANSFORMATION_MATRIX]){
			free(Body->TMat);
		}
		else if(flag == _LINE+1 && TypeNum[_LINE]){
			free(Body->Line);
		}
		else if(flag == _CONIC_ARC+1 && TypeNum[_CONIC_ARC]){
			free(Body->ConA);
		}
		else if(flag == _COMPOSITE_CURVE+1 && TypeNum[_COMPOSITE_CURVE]){
			free(Body->CompC);
		}
		else if(flag == _CIRCLE_ARC+1 && TypeNum[_CIRCLE_ARC]){
			free(Body->CirA);
		}
		flag--;
	}
	exit(ERR);
}

// BODY���\������S�G���e�B�e�B�̃������J��
// TypeCount[ALL_ENTITY_TYPE_NUM]:�e�G���e�B�e�B�̐����i�[���ꂽ����
void Free_body(BODY *Body,int TypeCount[])
{
	// �G���e�B�e�B��V���ɒǉ�����ꍇ�͈ȉ��ɐV����free��ǉ�����
	if(TypeCount[_TRIMMED_SURFACE]){
		Free_TrmS_1DArray(Body->TrmS,TypeCount[_TRIMMED_SURFACE]);
		free(Body->TrmS);
	}
	if(TypeCount[_CURVE_ON_PARAMETRIC_SURFACE]){
		free(Body->ConpS);
	}
	if(TypeCount[_NURBSS]){
		Free_NurbsS_1DArray(Body->NurbsS,TypeCount[_NURBSS]);
		free(Body->NurbsS);
	}
	if(TypeCount[_NURBSC]){
		Free_NurbsC_1DArray(Body->NurbsC,TypeCount[_NURBSC]);
		free(Body->NurbsC);
	}
	if(TypeCount[_TRANSFORMATION_MATRIX]){
		free(Body->TMat);
	}
	if(TypeCount[_LINE]){
		free(Body->Line);
	}
	if(TypeCount[_CONIC_ARC]){
		free(Body->ConA);
	}
	if(TypeCount[_COMPOSITE_CURVE]){
		Free_CompC_1DArray(Body->CompC,TypeCount[_COMPOSITE_CURVE]);
		free(Body->CompC);
	}
	if(TypeCount[_CIRCLE_ARC]){
		free(Body->CirA);
	}
}

// �����Ȑ��̃������[�m��
int New_CompC(COMPC *compc,int num)
{
	if((compc->DEType = (int *)malloc(sizeof(int)*num)) == NULL){
		return ERR;
	}
	
	if((compc->pDE = (COMPELEM **)malloc(sizeof(COMPELEM *)*num)) == NULL){
		free(compc->DEType);
		return ERR;
	}

	compc->N = num;

	return TRUE;
}

// �����Ȑ��z��̃������[���
void Free_CompC_1DArray(COMPC *a,int num)
{
	for(int i=0;i<num;i++)
		Free_CompC(&a[i]);
}

// �����Ȑ��̃������[���
void Free_CompC(COMPC *a)
{
	free(a->DEType);
	free(a->pDE);
}

// Nurbs�Ȑ��̃������[�m��
// ����  *nurb:�������[�m�ۂ���Nurbs�Ȑ��ւ̃|�C���^�AK:�R���g���[���|�C���g�̐��AN:�m�b�g�x�N�g���̐�
int New_NurbsC(NURBSC *nurb,int K, int N)
{
	if((nurb->T = (double *)malloc(sizeof(double)*N)) == NULL)
		return ERR;
	if((nurb->W = (double *)malloc(sizeof(double)*K)) == NULL){
		free(nurb->T);
		return ERR;
	}
	if((nurb->cp = (Coord *)malloc(sizeof(Coord)*K)) == NULL){
		free(nurb->T);
		free(nurb->W);
		return ERR;
	}

	return TRUE;
}

// NURBS�Ȑ��z��̃������[���
void Free_NurbsC_1DArray(NURBSC *a,int num)
{
	for(int i=0;i<num;i++)
		Free_NurbsC(&a[i]);
}

// 1�{��NURBS�Ȑ��̃������[���
void Free_NurbsC(NURBSC *a)
{
	free(a->T);
	free(a->W);
	free(a->cp);
}

// NURBS�Ȗʂ̃������[�m��
// ����  *nurb:�������[�m�ۂ���Nurbs�Ȗʂւ̃|�C���^�AK[2]:u,v�R���g���[���|�C���g�̐��AN[2]:u,v�m�b�g�x�N�g���̐�
int New_NurbsS(NURBSS *nurb,int K[2],int N[2])
{
	int errflag = 0;

	if((nurb->S = (double *)malloc(sizeof(double)*N[0])) == NULL)
		goto EXIT;
	errflag++;	// 1
	if((nurb->T = (double *)malloc(sizeof(double)*N[1])) == NULL)
		goto EXIT;
	errflag++;	// 2
	if((nurb->W = NewMatrix(K[0],K[1])) == NULL)
		goto EXIT;
	errflag++; // 3
	if((nurb->cp = NewCoord(K[0],K[1])) == NULL)
		goto EXIT;

	return TRUE;

EXIT:
	if(errflag == 3){
		FreeMatrix(nurb->W);
		errflag--;
	}
	if(errflag == 2){
		free(nurb->T);
		errflag--;
	}
	if(errflag == 1){
		free(nurb->S);
	}

	return ERR;
}

// NURBS�Ȗʔz��̃������[���
void Free_NurbsS_1DArray(NURBSS *a,int num)
{
	for(int i=0;i<num;i++)
		Free_NurbsS(&a[i]);
}

// NURBS�Ȗʂ̃������[���
void Free_NurbsS(NURBSS *a)
{
	free(a->S);
	free(a->T);
	FreeMatrix(a->W);
	FreeCoord(a->cp);
}

// �g�����ʂ̃������[�m��
int New_TrmS(TRMS *trms,int num)
{
	if((trms->pTI = (CONPS **)malloc(sizeof(CONPS *)*num)) == NULL){
		return ERR;
	}

	return TRUE;
}

// �g�����ʔz��̃������[���
void Free_TrmS_1DArray(TRMS *a,int num)
{
	for(int i=0;i<num;i++)
		Free_TrmS(&a[i]);
}

// �g�����ʂ̃������[���
void Free_TrmS(TRMS *a)
{
	free(a->pTI);
}

// �G���e�B�e�B�̃X�e�[�^�X�Œ�`����Ă���F��ύX
void ChangeStatColor(float *col,float r,float g,float b,float t)
{
	col[0] = r;
	col[1] = g;
	col[2] = b;
	col[3] = t;
}

// body���X�g�̎��̃f�[�^�ֈړ�����
BODY *ShiftBodyList(BODY *body)
{
	return(body->next);
}

// body���X�g��1�������[�m�ۂ���
BODY *AllocBodyList()
{
	return((BODY *)malloc(sizeof(BODY)));

}

// body���X�g�̎w�肵���ʒu�ɐV����body��}������
// ����   **body:BODY���X�g�̐擪�A�h���X  key:�V���ɑ}���������ꏊ��1��O��BODY�ԍ�  new_body:�V���ɑ}������BODY
void AddBodyList(BODY **body,int key,BODY *new_body)
{
	BODY *Curr;		// ���ڒ���BODY���w��

	// 1�ڂ�BODY�̏ꍇ
	if(key < 0){
		*body = new_body;		// BODY���X�g�̐擪�ɐV����BODY��ǉ�
		(*body)->next = NULL;	// ���̗v�f��NULL�ɂ���
		return;
	}
	// 2�ڈȍ~�̏ꍇ
	else{
		Curr = *body;							// BODY���X�g�̐擪�Ɉړ�
		while(Curr != NULL){					// ���X�g�̃G���h�܂Ń��[�v
			if(Curr->key == key){				// �V����BODY��}���������ꏊ�̒��O��BODY�𔭌�������
				new_body->next = Curr->next;	// ���X�g�̍����q��������
				Curr->next = new_body;			// ���X�g�̍����q��������
				return;
			}
			Curr = ShiftBodyList(Curr);			// ����BODY�ֈړ�
		}
	}
}

// �w�肵��body�ԍ���body�����X�g����폜����
// ����  **body:BODY���X�g�̐擪�A�h���X  key:�폜������BODY�v�f�ԍ�
void DelBodyList(BODY **body,int key)
{
	BODY *pre;
	BODY *Curr;
	
	pre = Curr = *body;

	while(Curr != NULL){
		if(Curr->key == key){
			Free_body(Curr,Curr->TypeNum);	// body�̃������[���
			if(Curr == *body)
				*body = Curr->next;
			else
				pre->next = Curr->next;
			free(Curr);						// �Ō�Ɏ���������
			return;
		}
		pre = Curr;
		Curr = ShiftBodyList(Curr);
	}
}

// �w�肵��Body�ԍ�(key)�̈ʒu��������. ������Ȃ��ꍇ��NULL��Ԃ�
BODY *SearchBodyList(BODY *body,int key)
{
	BODY *Curr;

	Curr = body;
	while(Curr != NULL){
		if(Curr->key == key){
			return Curr;
		}
		Curr = ShiftBodyList(Curr);
	}

	return NULL;
}
