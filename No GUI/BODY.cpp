#include "BODY.h"

// �R���X�g���N�^
BODY::BODY()
{
	// ������
	for(int i=0;i<ALL_ENTITY_TYPE_NUM;i++){
		TypeNum[i] = 0;
	}
	Mesh = NULL;
	Mom = NULL;

	MaxCoord = 1;
}

// BODY�N���X�̃������[�m��
void BODY::NewBodyElem()
{
	int flag=0;

	// �G���e�B�e�B��V���ɒǉ�����ꍇ�͈ȉ��ɐV����malloc���L�q���Ă��������B
	// �G���e�B�e�B�^�C�v�̔ԍ����Ⴂ���ɋL�q
	if(TypeNum[_CIRCLE_ARC]){
		if((CirA = (CIRA *)malloc(sizeof(CIRA)*TypeNum[_CIRCLE_ARC])) == NULL)  goto EXIT;
		flag = _CIRCLE_ARC+1;
	}

	if(TypeNum[_COMPOSITE_CURVE]){
		if((CompC = (COMPC *)malloc(sizeof(COMPC)*TypeNum[_COMPOSITE_CURVE])) == NULL)  goto EXIT;
		flag = _COMPOSITE_CURVE+1;
	}

	if(TypeNum[_CONIC_ARC]){
		if((ConA = (CONA *)malloc(sizeof(CONA)*TypeNum[_CONIC_ARC])) == NULL)  goto EXIT;
		flag = _CONIC_ARC+1;
	}

	if(TypeNum[_LINE]){
		if((Line = (LINE_ *)malloc(sizeof(LINE_)*TypeNum[_LINE])) == NULL)  goto EXIT;
		flag = _LINE+1;
	}

	if(TypeNum[_TRANSFORMATION_MATRIX]){
		if((TMat = (TMAT *)malloc(sizeof(TMAT)*TypeNum[_TRANSFORMATION_MATRIX])) == NULL)  goto EXIT;
		flag = _TRANSFORMATION_MATRIX+1;
	}

	if(TypeNum[_NURBSC]){
		if((NurbsC = (NURBSC *)malloc(sizeof(NURBSC)*TypeNum[_NURBSC])) == NULL)  goto EXIT;
		flag = _NURBSC+1;
	}

	if(TypeNum[_NURBSS]){
		if((NurbsS = (NURBSS *)malloc(sizeof(NURBSS)*TypeNum[_NURBSS])) == NULL)  goto EXIT;
		flag = _NURBSS+1;
	}

	if(TypeNum[_CURVE_ON_PARAMETRIC_SURFACE]){
		if((ConpS = (CONPS *)malloc(sizeof(CONPS)*TypeNum[_CURVE_ON_PARAMETRIC_SURFACE])) == NULL)  goto EXIT;
		flag = _CURVE_ON_PARAMETRIC_SURFACE+1;
	}

	if(TypeNum[_TRIMMED_SURFACE]){
		if((TrmS = (TRMS *)malloc(sizeof(TRMS)*TypeNum[_TRIMMED_SURFACE])) == NULL)  goto EXIT;
		flag = _TRIMMED_SURFACE+1;
	}

	Mesh = NULL;		// ���b�V����NULL�ɐݒ肵�Ă���

	return;		// �������[�𐳏�Ɋm��


EXIT:	// �������[�m�ۂɎ��s�����ꍇ�́A����܂Ŋm�ۂ�������������ďI��
	SetMessage("KOD_ERROR: malloc BODY");
	while(flag){
		if(flag == _CURVE_ON_PARAMETRIC_SURFACE+1 && TypeNum[_TRIMMED_SURFACE]){
			free(ConpS);
		}
		else if(flag == _NURBSS+1 && TypeNum[_NURBSS]){
			free(NurbsS);
		}
		else if(flag == _NURBSC+1 && TypeNum[_NURBSC]){
			free(NurbsC);
		}
		else if(flag == _TRANSFORMATION_MATRIX+1 && TypeNum[_TRANSFORMATION_MATRIX]){
			free(TMat);
		}
		else if(flag == _LINE+1 && TypeNum[_LINE]){
			free(Line);
		}
		else if(flag == _CONIC_ARC+1 && TypeNum[_CONIC_ARC]){
			free(ConA);
		}
		else if(flag == _COMPOSITE_CURVE+1 && TypeNum[_COMPOSITE_CURVE]){
			free(CompC);
		}
		else if(flag == _CIRCLE_ARC+1 && TypeNum[_CIRCLE_ARC]){
			free(CirA);
		}
		flag--;
	}
	exit(KOD_ERR);
}

// BODY���\������S�G���e�B�e�B�̃������J��
void BODY::DelBodyElem()
{
	NURBS_Func NFunc;

	// �G���e�B�e�B��V���ɒǉ�����ꍇ�͈ȉ��ɐV����free��ǉ�����
	if(TypeNum[_TRIMMED_SURFACE]){
		NFunc.Free_TrmS_1DArray(TrmS,TypeNum[_TRIMMED_SURFACE]);
		free(TrmS);
	}
	if(TypeNum[_CURVE_ON_PARAMETRIC_SURFACE]){
		free(ConpS);
	}
	if(TypeNum[_NURBSS]){
		NFunc.Free_NurbsS_1DArray(NurbsS,TypeNum[_NURBSS]);
		free(NurbsS);
	}
	if(TypeNum[_NURBSC]){
		NFunc.Free_NurbsC_1DArray(NurbsC,TypeNum[_NURBSC]);
		free(NurbsC);
	}
	if(TypeNum[_TRANSFORMATION_MATRIX]){
		free(TMat);
	}
	if(TypeNum[_LINE]){
		free(Line);
	}
	if(TypeNum[_CONIC_ARC]){
		free(ConA);
	}
	if(TypeNum[_COMPOSITE_CURVE]){
		NFunc.Free_CompC_1DArray(CompC,TypeNum[_COMPOSITE_CURVE]);
		free(CompC);
	}
	if(TypeNum[_CIRCLE_ARC]){
		free(CirA);
	}
	if(Mesh != NULL){
		Mesh->clear();
	}
}
// BODY���\������G���e�B�e�B���w�肵���������������J��
void BODY::DelBodyElem(int TypeNum_[])
{
	NURBS_Func NFunc;

	// �G���e�B�e�B��V���ɒǉ�����ꍇ�͈ȉ��ɐV����free��ǉ�����
	if(TypeNum_[_TRIMMED_SURFACE]){
		NFunc.Free_TrmS_1DArray(TrmS,TypeNum_[_TRIMMED_SURFACE]);
		free(TrmS);
	}
	if(TypeNum_[_CURVE_ON_PARAMETRIC_SURFACE]){
		free(ConpS);
	}
	if(TypeNum_[_NURBSS]){
		NFunc.Free_NurbsS_1DArray(NurbsS,TypeNum_[_NURBSS]);
		free(NurbsS);
	}
	if(TypeNum_[_NURBSC]){
		NFunc.Free_NurbsC_1DArray(NurbsC,TypeNum_[_NURBSC]);
		free(NurbsC);
	}
	if(TypeNum_[_TRANSFORMATION_MATRIX]){
		free(TMat);
	}
	if(TypeNum_[_LINE]){
		free(Line);
	}
	if(TypeNum_[_CONIC_ARC]){
		free(ConA);
	}
	if(TypeNum_[_COMPOSITE_CURVE]){
		NFunc.Free_CompC_1DArray(CompC,TypeNum_[_COMPOSITE_CURVE]);
		free(CompC);
	}
	if(TypeNum_[_CIRCLE_ARC]){
		free(CirA);
	}
	if(Mesh != NULL){
		Mesh->clear();
	}
}

// BODY�̉�]
void BODY::RotBody(Coord Axis,double deg)
{
	NURBS_Func NFunc;

	for(int i=0;i<TypeNum[_NURBSS];i++)			// NURBS�Ȗʂ̃V�t�g
		NFunc.RotNurbsS(&NurbsS[i],Axis,deg);
	for(int i=0;i<TypeNum[_NURBSC];i++){			// NURBS�Ȑ��̃V�t�g
		if(NurbsC[i].EntUseFlag == GEOMTRYELEM)	// NURBS�Ȗʂ̃p�����g���b�N�v�f�Ƃ��Ă�NURBS�Ȑ��Ɋւ��Ă͉������Ȃ�
			NFunc.RotNurbsC(&NurbsC[i],Axis,deg);
	}
}

// BODY�̃V�t�g
void BODY::ShiftBody(Coord d)
{
	NURBS_Func NFunc;

	for(int i=0;i<TypeNum[_NURBSS];i++)			// NURBS�Ȗʂ̃V�t�g
		NFunc.ShiftNurbsS(&NurbsS[i],d);
	for(int i=0;i<TypeNum[_NURBSC];i++){		// NURBS�Ȑ��̃V�t�g
		if(NurbsC[i].EntUseFlag == GEOMTRYELEM)	// NURBS�Ȗʂ̃p�����g���b�N�v�f�Ƃ��Ă�NURBS�Ȑ��Ɋւ��Ă͉������Ȃ�
			NFunc.ShiftNurbsC(&NurbsC[i],d);
	}
}

// BODY�̊g��k��
void BODY::ExpandBody(Coord r)
{
	NURBS_Func NFunc;

	for(int i=0;i<TypeNum[_NURBSS];i++)			// NURBS�Ȗʂ̃V�t�g
		NFunc.ChRatioNurbsS(&NurbsS[i],r);
	for(int i=0;i<TypeNum[_NURBSC];i++){		// NURBS�Ȑ��̃V�t�g
		if(NurbsC[i].EntUseFlag == GEOMTRYELEM)	// NURBS�Ȗʂ̃p�����g���b�N�v�f�Ƃ��Ă�NURBS�Ȑ��Ɋւ��Ă͉������Ȃ�
		NFunc.ChRatioNurbsC(&NurbsC[i],r);		// NURBS�Ȑ��̊g��
	}
}

// ������V����BODY�Ƃ��ēo�^����
void BODY::RegistBody(BODYList *BodyList, char BodyName[])
{
	Mom = BodyList->add(this);				// �ǂݍ���IGES�f�[�^��BODYList�ɓo�^����
	strcpy(Name,BodyName);					// �t�@�C������body���Ƃ��ēo�^
}

// �������g����������
void BODY::DeleteBody(BODYList *BodyList)
{
	BodyList->delData(Mom);			// ���X�g����ǂݍ���BODY���͂���
	DelBodyElem();					// BODY���Ŋm�ۂ��Ă��郁�����[�̉��
}

// 1��NURBS�Ȑ���V����BODY�Ƃ��ēo�^����
void BODY::RegistNurbsCtoBody(BODYList *BodyList,NURBSC Nurb,char BodyName[])
{
	NurbsC = (NURBSC *)malloc(sizeof(NURBSC));
	NurbsC[0] = Nurb;												// NURBS�Ȗʂ̎��̂���
	TypeNum[_NURBSC] = 1;											// NURBS�Ȗʂ̐�1�ɂ���
	ChangeStatColor(this->NurbsC[0].Dstat.Color,0.2,0.2,1.0,0.5);		// �F
	BodyList->add(this);										// ���X�g�ɐV����BODY��o�^
	//SetBodyNameToWin(BodyName);											// Body���X�g�E�B���h�E��BODY����o�^
	strcpy(Name,BodyName);										// �V����BODY����o�^
}

// N��NURBS�Ȑ���V����BODY�Ƃ��ēo�^����
void BODY::RegistNurbsCtoBodyN(BODYList *BodyList,NURBSC Nurb[],char BodyName[],int N)
{
	NurbsC = (NURBSC *)malloc(sizeof(NURBSC)*N);
	for(int i=0;i<N;i++){
		NurbsC[i] = Nurb[i];										// NURBS�Ȗʂ̎��̂���
		TypeNum[_NURBSC] = N;										// NURBS�Ȗʂ̐�1�ɂ���
		ChangeStatColor(this->NurbsC[i].Dstat.Color,0.2,0.2,1.0,0.5);	// �F
	}
	BodyList->add((void *)this);										// ���X�g�ɐV����BODY��o�^
	//SetBodyNameToWin(BodyName);											// Body���X�g�E�B���h�E��BODY����o�^
	strcpy(Name,BodyName);										// �V����BODY����o�^
}


// 1��NURBS�Ȗʂ�V����BODY�Ƃ��ēo�^����
void BODY::RegistNurbsStoBody(BODYList *BodyList,NURBSS Nurb,char BodyName[])
{
	NurbsS = (NURBSS *)malloc(sizeof(NURBSS));
	NurbsS[0] = Nurb;												// NURBS�Ȗʂ̎��̂���
	NurbsS[0].TrmdSurfFlag = KOD_FALSE;								// �g�����̂Ȃ��P����NURBS�Ȗʂł��邱�Ƃ𖾎�
	TypeNum[_NURBSS] = 1;											// NURBS�Ȗʂ̐�1�ɂ���
	ChangeStatColor(this->NurbsS[0].Dstat.Color,0.2,0.2,1.0,0.5);	// �F
	this->Mom = BodyList->add((void *)this);						// ���X�g�ɐV����BODY��o�^
	//SetBodyNameToWin(BodyName);										// Body���X�g�E�B���h�E��BODY����o�^
	strcpy(Name,BodyName);											// �V����BODY����o�^
}

// N��NURBS�Ȗʂ�V����BODY�Ƃ��ēo�^����
void BODY::RegistNurbsStoBodyN(BODYList *BodyList,NURBSS Nurb[],char BodyName[],int N)
{
	NurbsS = (NURBSS *)malloc(sizeof(NURBSS)*N);
	for(int i=0;i<N;i++){
		NurbsS[i] = Nurb[i];										// NURBS�Ȗʂ̎��̂���
		NurbsS[i].TrmdSurfFlag = KOD_FALSE;							// �g�����̂Ȃ��P����NURBS�Ȗʂł��邱�Ƃ𖾎�
		TypeNum[_NURBSS] = N;										// NURBS�Ȗʂ̐�1�ɂ���
		ChangeStatColor(this->NurbsS[i].Dstat.Color,0.2,0.2,1.0,0.5);	// �F
	}
	this->Mom = BodyList->add((void *)this);						// ���X�g�ɐV����BODY��o�^
	//SetBodyNameToWin(BodyName);										// Body���X�g�E�B���h�E��BODY����o�^
	strcpy(Name,BodyName);											// �V����BODY����o�^
}

// �G���e�B�e�B�̃X�e�[�^�X�Œ�`����Ă���F��ύX
void BODY::ChangeStatColor(float *col,float r,float g,float b,float t)
{
	col[0] = r;
	col[1] = g;
	col[2] = b;
	col[3] = t;
}

// ���̐F�̏����l��^����
void BODY::InitCurveColor(float *col)
{
	col[0] = col[1] = col[2] = 1.0;
	col[3] = 0.5;
}

// �ʂ̐F�̏����l��^����
void BODY::InitSurfaceColor(float *col)
{
	col[0] = col[1] = col[2] = 0.2;
	col[3] = 0.5;
}
