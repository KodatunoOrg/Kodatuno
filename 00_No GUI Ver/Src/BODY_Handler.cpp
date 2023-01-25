// BODY�̃X�e�[�^�X����A�������[�֘A�̒�`

#include <stdlib.h>
#include "BODY_Handler.h"

// BODY�N���X�̃������[�m��
void BODY_Handler::New_body(BODY *Body,int TypeNum[])
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
	SetMessage("KOD_ERROR: malloc BODY");
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
	exit(KOD_ERR);
}

// BODY���\������S�G���e�B�e�B�̃������J��
// TypeCount[ALL_ENTITY_TYPE_NUM]:�e�G���e�B�e�B�̐����i�[���ꂽ����
void BODY_Handler::Free_body(BODY *Body,int TypeCount[])
{
	// �G���e�B�e�B��V���ɒǉ�����ꍇ�͈ȉ��ɐV����free��ǉ�����
	if(TypeCount[_TRIMMED_SURFACE]){
		NFunc.Free_TrmS_1DArray(Body->TrmS,TypeCount[_TRIMMED_SURFACE]);
		free(Body->TrmS);
	}
	if(TypeCount[_CURVE_ON_PARAMETRIC_SURFACE]){
		free(Body->ConpS);
	}
	if(TypeCount[_NURBSS]){
		NFunc.Free_NurbsS_1DArray(Body->NurbsS,TypeCount[_NURBSS]);
		free(Body->NurbsS);
	}
	if(TypeCount[_NURBSC]){
		NFunc.Free_NurbsC_1DArray(Body->NurbsC,TypeCount[_NURBSC]);
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
		NFunc.Free_CompC_1DArray(Body->CompC,TypeCount[_COMPOSITE_CURVE]);
		free(Body->CompC);
	}
	if(TypeCount[_CIRCLE_ARC]){
		free(Body->CirA);
	}
}



// ���̐F�̏����l��^����
void BODY_Handler::InitCurveColor(float *col)
{
	col[0] = col[1] = col[2] = 1.0;
	col[3] = 0.5;
}

// �ʂ̐F�̏����l��^����
void BODY_Handler::InitSurfaceColor(float *col)
{
	col[0] = col[1] = col[2] = 0.2;
	col[3] = 0.5;
}


// �G���e�B�e�B�̃X�e�[�^�X�Œ�`����Ă���F��ύX
void BODY_Handler::ChangeStatColor(float *col,float r,float g,float b,float t)
{
	col[0] = r;
	col[1] = g;
	col[2] = b;
	col[3] = t;
}

// �Z���N�V��������Ă���BODY�ԍ��𓾂�
BODY *BODY_Handler::SearchBodyList(BODYList *BodyList,int key)
{
	return (BODY *)BodyList->getData(key);
}

// BODY������
void BODY_Handler::Init_body(BODY *body)
{
	body->MaxCoord = 1.0;

	for(int i=0;i<ALL_ENTITY_TYPE_NUM;i++){
		body->TypeNum[i] = 0;
	}
}

// 1��NURBS�Ȑ���V����BODY�Ƃ��ēo�^����
void BODY_Handler::RegistNurbsCtoBody(BODYList *BodyList,BODY *Body,NURBSC Nurb,char BodyName[])
{
	Init_body(Body);	
	Body->NurbsC = (NURBSC *)malloc(sizeof(NURBSC));
	Body->NurbsC[0] = Nurb;												// NURBS�Ȗʂ̎��̂���
	Body->TypeNum[_NURBSC] = 1;											// NURBS�Ȗʂ̐�1�ɂ���
	ChangeStatColor(Body->NurbsC[0].Dstat.Color,0.2,0.2,1.0,0.5);		// �F
	BodyList->add((void *)Body);										// ���X�g�ɐV����BODY��o�^
	SetBodyNameToWin(BodyName);											// Body���X�g�E�B���h�E��BODY����o�^
	strcpy(Body->Name,BodyName);										// �V����BODY����o�^
}

// N��NURBS�Ȑ���V����BODY�Ƃ��ēo�^����
void BODY_Handler::RegistNurbsCtoBodyN(BODYList *BodyList,BODY *Body,NURBSC Nurb[],char BodyName[],int N)
{
	Init_body(Body);	
	Body->NurbsC = (NURBSC *)malloc(sizeof(NURBSC)*N);
	for(int i=0;i<N;i++){
		Body->NurbsC[i] = Nurb[i];										// NURBS�Ȗʂ̎��̂���
		Body->TypeNum[_NURBSC] = N;										// NURBS�Ȗʂ̐�1�ɂ���
		ChangeStatColor(Body->NurbsC[i].Dstat.Color,0.2,0.2,1.0,0.5);	// �F
	}
	BodyList->add((void *)Body);										// ���X�g�ɐV����BODY��o�^
	SetBodyNameToWin(BodyName);											// Body���X�g�E�B���h�E��BODY����o�^
	strcpy(Body->Name,BodyName);										// �V����BODY����o�^
}


// 1��NURBS�Ȗʂ�V����BODY�Ƃ��ēo�^����
void BODY_Handler::RegistNurbsStoBody(BODYList *BodyList,BODY *Body,NURBSS Nurb,char BodyName[])
{
	Init_body(Body);	
	Body->NurbsS = (NURBSS *)malloc(sizeof(NURBSS));
	Body->NurbsS[0] = Nurb;												// NURBS�Ȗʂ̎��̂���
	Body->NurbsS[0].TrmdSurfFlag = KOD_FALSE;							// �g�����̂Ȃ��P����NURBS�Ȗʂł��邱�Ƃ𖾎�
	Body->TypeNum[_NURBSS] = 1;											// NURBS�Ȗʂ̐�1�ɂ���
	ChangeStatColor(Body->NurbsS[0].Dstat.Color,0.2,0.2,1.0,0.5);		// �F
	BodyList->add((void *)Body);										// ���X�g�ɐV����BODY��o�^
	SetBodyNameToWin(BodyName);											// Body���X�g�E�B���h�E��BODY����o�^
	strcpy(Body->Name,BodyName);										// �V����BODY����o�^
}

// N��NURBS�Ȗʂ�V����BODY�Ƃ��ēo�^����
void BODY_Handler::RegistNurbsStoBodyN(BODYList *BodyList,BODY *Body,NURBSS Nurb[],char BodyName[],int N)
{
	Init_body(Body);	
	Body->NurbsS = (NURBSS *)malloc(sizeof(NURBSS)*N);
	for(int i=0;i<N;i++){
		Body->NurbsS[i] = Nurb[i];										// NURBS�Ȗʂ̎��̂���
		Body->NurbsS[i].TrmdSurfFlag = KOD_FALSE;						// �g�����̂Ȃ��P����NURBS�Ȗʂł��邱�Ƃ𖾎�
		Body->TypeNum[_NURBSS] = N;										// NURBS�Ȗʂ̐�1�ɂ���
		ChangeStatColor(Body->NurbsS[i].Dstat.Color,0.2,0.2,1.0,0.5);	// �F
	}
	BodyList->add((void *)Body);										// ���X�g�ɐV����BODY��o�^
	SetBodyNameToWin(BodyName);											// Body���X�g�E�B���h�E��BODY����o�^
	strcpy(Body->Name,BodyName);										// �V����BODY����o�^
}

// BODY�̉�]
void BODY_Handler::RotBody(BODY *Body,Coord Axis,double deg)
{
	for(int i=0;i<Body->TypeNum[_NURBSS];i++)			// NURBS�Ȗʂ̃V�t�g
		NFunc.RotNurbsS(&Body->NurbsS[i],Axis,deg);
	for(int i=0;i<Body->TypeNum[_NURBSC];i++){			// NURBS�Ȑ��̃V�t�g
		if(Body->NurbsC[i].EntUseFlag == GEOMTRYELEM)	// NURBS�Ȗʂ̃p�����g���b�N�v�f�Ƃ��Ă�NURBS�Ȑ��Ɋւ��Ă͉������Ȃ�
			NFunc.RotNurbsC(&Body->NurbsC[i],Axis,deg);
	}
}

// BODY�̃V�t�g
void BODY_Handler::ShiftBody(BODY *Body,Coord d)
{
	for(int i=0;i<Body->TypeNum[_NURBSS];i++)			// NURBS�Ȗʂ̃V�t�g
		NFunc.ShiftNurbsS(&Body->NurbsS[i],d);
	for(int i=0;i<Body->TypeNum[_NURBSC];i++){			// NURBS�Ȑ��̃V�t�g
		if(Body->NurbsC[i].EntUseFlag == GEOMTRYELEM)	// NURBS�Ȗʂ̃p�����g���b�N�v�f�Ƃ��Ă�NURBS�Ȑ��Ɋւ��Ă͉������Ȃ�
			NFunc.ShiftNurbsC(&Body->NurbsC[i],d);
	}
}

