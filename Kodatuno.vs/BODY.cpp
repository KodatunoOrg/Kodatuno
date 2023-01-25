#include "stdafx.h"
#include "BODY.h"

#if defined(_DEBUG) && defined(_MSC_VER)
#define new DEBUG_NEW
#endif

// Function: BODY
// BODY�N���X�̃R���X�g���N�^�D�e�평����
BODY::BODY()
{
	// ������
	for(int i=0;i<ALL_ENTITY_TYPE_NUM;i++){
		TypeNum[i] = 0;
	}
	Mesh = NULL;

	MaxCoord = 1;

	// Add by K.Magara
	ClearMinMaxCoord();		// NCVC�p�̏�����
}

BODY::~BODY()
{
//	DelBodyElem();
}

// Add by K.Magara
void BODY::ClearMinMaxCoord(void)
{
	minmaxCoord[0] = SetCoord( DBL_MAX, DBL_MAX, DBL_MAX);		// �ŏ��ێ����ő�l�ŏ�����
	minmaxCoord[1] = SetCoord(-DBL_MAX,-DBL_MAX,-DBL_MAX);		// �ő�ێ����ŏ��l�ŏ�����
}
// --

// Function: NewBodyElem
// BODY�N���X�̃������[�m��
void BODY::NewBodyElem()
{
	int flag=0;

	// �G���e�B�e�B��V���ɒǉ�����ꍇ�͈ȉ��ɐV����malloc���L�q���Ă��������B
	// �G���e�B�e�B�^�C�v�̔ԍ����Ⴂ���ɋL�q
	if(TypeNum[_CIRCLE_ARC]){
		if(NewCirA(TypeNum[_CIRCLE_ARC]) == NULL)  goto EXIT;
		flag = _CIRCLE_ARC+1;
	}

	if(TypeNum[_COMPOSITE_CURVE]){
		if(NewCompC(TypeNum[_COMPOSITE_CURVE]) == NULL)  goto EXIT;
		flag = _COMPOSITE_CURVE+1;
	}

	if(TypeNum[_CONIC_ARC]){
		if(NewConA(TypeNum[_CONIC_ARC]) == NULL)  goto EXIT;
		flag = _CONIC_ARC+1;
	}

	if(TypeNum[_LINE]){
		if(NewLine(TypeNum[_LINE]) == NULL)  goto EXIT;
		flag = _LINE+1;
	}

	if(TypeNum[_TRANSFORMATION_MATRIX]){
		if(NewTMat(TypeNum[_TRANSFORMATION_MATRIX]) == NULL)  goto EXIT;
		flag = _TRANSFORMATION_MATRIX+1;
	}

	if(TypeNum[_NURBSC]){
		if(NewNurbsC(TypeNum[_NURBSC]) == NULL)  goto EXIT;
		flag = _NURBSC+1;
	}

	if(TypeNum[_NURBSS]){
		if(NewNurbsS(TypeNum[_NURBSS]) == NULL)  goto EXIT;
		flag = _NURBSS+1;
	}

	if(TypeNum[_CURVE_ON_PARAMETRIC_SURFACE]){
		if(NewConpS(TypeNum[_CURVE_ON_PARAMETRIC_SURFACE]) == NULL)  goto EXIT;
		flag = _CURVE_ON_PARAMETRIC_SURFACE+1;
	}

	if(TypeNum[_TRIMMED_SURFACE]){
		if(NewTrmS(TypeNum[_TRIMMED_SURFACE]) == NULL)  goto EXIT;
		flag = _TRIMMED_SURFACE+1;
	}

	Mesh = NULL;		// ���b�V����NULL�ɐݒ肵�Ă���

	return;		// �������[�𐳏�Ɋm��


EXIT:	// �������[�m�ۂɎ��s�����ꍇ�́A����܂Ŋm�ۂ�������������ďI��
	GuiIF.SetMessage("KOD_ERROR: malloc BODY");
	while(flag){
		if(flag == _CURVE_ON_PARAMETRIC_SURFACE+1 && TypeNum[_TRIMMED_SURFACE]){
//			free(ConpS);
			delete[]	ConpS;
			ConpS = NULL;
		}
		else if(flag == _NURBSS+1 && TypeNum[_NURBSS]){
//			free(NurbsS);
			delete[]	NurbsS;
			NurbsS = NULL;
		}
		else if(flag == _NURBSC+1 && TypeNum[_NURBSC]){
//			free(NurbsC);
			delete[]	NurbsC;
			NurbsC = NULL;
		}
		else if(flag == _TRANSFORMATION_MATRIX+1 && TypeNum[_TRANSFORMATION_MATRIX]){
//			free(TMat);
			delete[]	TMat;
			TMat = NULL;
		}
		else if(flag == _LINE+1 && TypeNum[_LINE]){
//			free(Line);
			delete[]	Line;
			Line = NULL;
		}
		else if(flag == _CONIC_ARC+1 && TypeNum[_CONIC_ARC]){
//			free(ConA);
			delete[]	ConA;
			ConA = NULL;
		}
		else if(flag == _COMPOSITE_CURVE+1 && TypeNum[_COMPOSITE_CURVE]){
//			free(CompC);
			delete[]	CompC;
			CompC = NULL;
		}
		else if(flag == _CIRCLE_ARC+1 && TypeNum[_CIRCLE_ARC]){
//			free(CirA);
			delete[]	CirA;
			CirA = NULL;
		}
		flag--;
	}
	exit(KOD_ERR);
}


// Function: DleBodyElem
// BODY���\������S�G���e�B�e�B�̃������J��
void BODY::DelBodyElem()
{
/*
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
*/
	DelBodyElem(TypeNum);
}


// Function: DelBodyElem
// BODY���\������G���e�B�e�B���w�肵���������������J��
//
// Parameters: 
//	TypeNum_[] - �e�G���e�B�e�B�ԍ����C���f�b�N�X�Ƃ����z��ɁC�m�ۂ���Ă���e�G���e�B�e�B������
void BODY::DelBodyElem(int TypeNum_[])
{
	NURBS_Func NFunc;

	// �G���e�B�e�B��V���ɒǉ�����ꍇ�͈ȉ��ɐV����free��ǉ�����
	if(TypeNum_[_TRIMMED_SURFACE]){
		NFunc.Free_TrmS_1DArray(TrmS,TypeNum_[_TRIMMED_SURFACE]);
//		free(TrmS);
		if ( TrmS ) {
			delete[]	TrmS;
			TrmS = NULL;
		}
		TypeNum_[_TRIMMED_SURFACE] = 0;
	}
	if(TypeNum_[_CURVE_ON_PARAMETRIC_SURFACE]){
//		free(ConpS);
		if ( ConpS ) {
			delete[]	ConpS;
			ConpS = NULL;
		}
		TypeNum_[_CURVE_ON_PARAMETRIC_SURFACE] = 0;
	}
	if(TypeNum_[_NURBSS]){
		NFunc.Free_NurbsS_1DArray(NurbsS,TypeNum_[_NURBSS]);
//		free(NurbsS);
		if ( NurbsS ) {
			delete[]	NurbsS;
			NurbsS = NULL;
		}
		TypeNum_[_NURBSS] = 0;
	}
	if(TypeNum_[_NURBSC]){
		NFunc.Free_NurbsC_1DArray(NurbsC,TypeNum_[_NURBSC]);
//		NFunc.Free_NurbsC_1DArray(NurbsC.get(),TypeNum_[_NURBSC]);
//		free(NurbsC);
		if ( NurbsC ) {
			delete[]	NurbsC;
			NurbsC = NULL;
		}
		TypeNum_[_NURBSC] = 0;
	}
	if(TypeNum_[_TRANSFORMATION_MATRIX]){
//		free(TMat);
		if ( TMat ) {
			delete[]	TMat;
			TMat = NULL;
		}
		TypeNum_[_TRANSFORMATION_MATRIX] = 0;
	}
	if(TypeNum_[_LINE]){
//		free(Line);
		if ( Line ) {
			delete[]	Line;
			Line = NULL;
		}
		TypeNum_[_LINE] = 0;
	}
	if(TypeNum_[_CONIC_ARC]){
//		free(ConA);
		if ( ConA ) {
			delete[]	ConA;
			ConA = NULL;
		}
		TypeNum_[_CONIC_ARC] = 0;
	}
	if(TypeNum_[_COMPOSITE_CURVE]){
		NFunc.Free_CompC_1DArray(CompC,TypeNum_[_COMPOSITE_CURVE]);
//		free(CompC);
		if ( CompC ) {
			delete[]	CompC;
			CompC = NULL;
		}
		TypeNum_[_COMPOSITE_CURVE] = 0;
	}
	if(TypeNum_[_CIRCLE_ARC]){
//		free(CirA);
		if ( CirA ) {
			delete[]	CirA;
			CirA = NULL;
		}
		TypeNum_[_CIRCLE_ARC] = 0;
	}
	if(Mesh != NULL){
		Mesh->clear();
		Mesh = NULL;
	}
}


// Function: RotBody
// BODY����]������

// Parameters:
//	Axis - ��]��
//	deg - ��]�p�x
void BODY::RotBody(Coord Axis,double deg)
{
//	NURBS_Func NFunc;

	for(int i=0;i<TypeNum[_NURBSS];i++)	{		// NURBS�Ȗʂ̉�]
//		NFunc.RotNurbsS(&NurbsS[i],Axis,deg);
		NurbsS[i].RotNurbsS(Axis,deg);
	}
	for(int i=0;i<TypeNum[_NURBSC];i++){		// NURBS�Ȑ��̉�]
		if(NurbsC[i].EntUseFlag == GEOMTRYELEM) {	// NURBS�Ȗʂ̃p�����g���b�N�v�f�Ƃ��Ă�NURBS�Ȑ��Ɋւ��Ă͉������Ȃ�
//			NFunc.RotNurbsC(&NurbsC[i],Axis,deg);
			NurbsC[i].RotNurbsC(Axis,deg);
		}
	}
}


// Function: ShiftBody
// BODY���V�t�g������
//
// Parameters:
//	d - �ړ���
void BODY::ShiftBody(Coord d)
{
//	NURBS_Func NFunc;

	// Add by K.Magara
	ClearMinMaxCoord();
	// --
	for(int i=0;i<TypeNum[_NURBSS];i++)	{		// NURBS�Ȗʂ̃V�t�g
//		NFunc.ShiftNurbsS(&NurbsS[i],d);
		NurbsS[i].ShiftNurbsS(d);
		// Add by K.Magara
		for ( int j=0; j<NurbsS[i].K[0]; j++ ) {
			for ( int k=0; k<NurbsS[i].K[1]; k++ ) {
				minmaxCoord[0].SetMinCoord(NurbsS[i].cp[j][k]);
				minmaxCoord[1].SetMaxCoord(NurbsS[i].cp[j][k]);
			}
		}
		// --
	}
	for(int i=0;i<TypeNum[_NURBSC];i++){		// NURBS�Ȑ��̃V�t�g
		if(NurbsC[i].EntUseFlag == GEOMTRYELEM) {	// NURBS�Ȗʂ̃p�����g���b�N�v�f�Ƃ��Ă�NURBS�Ȑ��Ɋւ��Ă͉������Ȃ�
//			NFunc.ShiftNurbsC(&NurbsC[i],d);
			NurbsC[i].ShiftNurbsC(d);
			// Add by K.Magara
			for ( int j=0; j<NurbsC[i].K; j++ ) {
				minmaxCoord[0].SetMinCoord(NurbsC[i].cp[j]);
				minmaxCoord[1].SetMaxCoord(NurbsC[i].cp[j]);
			}
			// --
		}
	}
}

// Function: ExpandBody
//			  BODY�̊g��k��
//
// Parameters:
//		  r - X, Y, Z�e�������ꂼ��̊g��(�k��)��(1���)
void BODY::ExpandBody(Coord r)
{
//	NURBS_Func NFunc;

	for(int i=0;i<TypeNum[_NURBSS];i++) {			// NURBS�Ȗʂ̃V�t�g
//		NFunc.ChRatioNurbsS(&NurbsS[i],r);
		NurbsS[i].ChRatioNurbsS(r);
	}
	for(int i=0;i<TypeNum[_NURBSC];i++){		// NURBS�Ȑ��̃V�t�g
		if(NurbsC[i].EntUseFlag == GEOMTRYELEM) {	// NURBS�Ȗʂ̃p�����g���b�N�v�f�Ƃ��Ă�NURBS�Ȑ��Ɋւ��Ă͉������Ȃ�
//			NFunc.ChRatioNurbsC(&NurbsC[i],r);		// NURBS�Ȑ��̊g��
			NurbsC[i].ChRatioNurbsC(r);		// NURBS�Ȑ��̊g��
		}
	}
}
// Function: RegistBody
//	������V����BODY�Ƃ��ēo�^����
// 
// Parameters:
//	*BodyList - �o�^�惊�X�g
//	BodyName[] - �o�^����BODY��
void BODY::RegistBody(BODYList *BodyList,const char BodyName[])
{
	Mom = BodyList->add(this);				// �ǂݍ���IGES�f�[�^��BODYList�ɓo�^����
	GuiIF.AddBodyNameToWin(BodyName);		// Body���X�g�E�B���h�E��BODY����o�^
//	strcpy(Name,BodyName);					// �t�@�C������body���Ƃ��ēo�^
	Name = BodyName;
}

// Function: RegistNurbsCtoBody
//	1��NURBS�Ȑ���V����BODY�Ƃ��ēo�^����
// 
// Parameters:
//	*BodyList - �o�^�惊�X�g
//	Nurb - �o�^����NURBS�Ȑ��̎���
//  BodyName[] - �o�^����BODY��
//void BODY::RegistNurbsCtoBody(BODYList *BodyList,NURBSC Nurb,const char BodyName[])
void BODY::RegistNurbsCtoBody(BODYList *BodyList,const NURBSC& Nurb,const char BodyName[])
{
//	NurbsC = (NURBSC *)malloc(sizeof(NURBSC));
	NurbsC = new NURBSC[1];
//	NurbsC = boost::shared_array<NURBSC>( new NURBSC );
	NurbsC[0] = Nurb;												// NURBS�Ȗʂ̎��̂���
	TypeNum[_NURBSC] = 1;											// NURBS�Ȗʂ̐�1�ɂ���
	ChangeStatColor(this->NurbsC[0].Dstat.Color,0.2,0.2,1.0,0.5);	// �F
	BodyList->add(this);											// ���X�g�ɐV����BODY��o�^
	GuiIF.AddBodyNameToWin(BodyName);								// Body���X�g�E�B���h�E��BODY����o�^
//	strcpy(Name,BodyName);											// �V����BODY����o�^
	Name = BodyName;											// �V����BODY����o�^
}

// Function: RegistNurbsCtoBodyN
// N��NURBS�Ȑ���V����BODY�Ƃ��ēo�^����
// 
// Parameters:
//	*BodyList - �o�^�惊�X�g
//	Nurb[] - �o�^����NURBS�Ȑ��̎���
//  BodyName[] - �o�^����BODY��
//	N - �o�^����NURBS�Ȑ��̐�
void BODY::RegistNurbsCtoBodyN(BODYList *BodyList,NURBSC Nurb[],const char BodyName[],int N)
{
//	NurbsC = (NURBSC *)malloc(sizeof(NURBSC)*N);
	NurbsC = new NURBSC[N];
//	NurbsC = boost::shared_array<NURBSC>( new NURBSC[N] );
	for(int i=0;i<N;i++){
		NurbsC[i] = Nurb[i];										// NURBS�Ȗʂ̎��̂���
		TypeNum[_NURBSC] = N;										// NURBS�Ȗʂ̐�1�ɂ���
		ChangeStatColor(this->NurbsC[i].Dstat.Color,0.2,0.2,1.0,0.5);	// �F
	}
	BodyList->add((void *)this);									// ���X�g�ɐV����BODY��o�^
	GuiIF.AddBodyNameToWin(BodyName);									// Body���X�g�E�B���h�E��BODY����o�^
//	strcpy(Name,BodyName);											// �V����BODY����o�^
	Name = BodyName;											// �V����BODY����o�^
}

// Function: RegistNurbsStoBody
// 1��NURBS�Ȗʂ�V����BODY�Ƃ��ēo�^����
//
// Parameters:
//	*BodyList - �o�^�惊�X�g
//	Nurb - �o�^����NURBS�Ȗʂ̎���
//  BodyName[] - �o�^����BODY��
void BODY::RegistNurbsStoBody(BODYList *BodyList,NURBSS Nurb,const char BodyName[])
{
//	NurbsS = (NURBSS *)malloc(sizeof(NURBSS));
	NurbsS = new NURBSS[1];
	NurbsS[0] = Nurb;												// NURBS�Ȗʂ̎��̂���
	NurbsS[0].TrmdSurfFlag = KOD_FALSE;								// �g�����̂Ȃ��P����NURBS�Ȗʂł��邱�Ƃ𖾎�
	TypeNum[_NURBSS] = 1;											// NURBS�Ȗʂ̐�1�ɂ���
	ChangeStatColor(this->NurbsS[0].Dstat.Color,0.2,0.2,1.0,0.5);	// �F
	BodyList->add((void *)this);									// ���X�g�ɐV����BODY��o�^
	GuiIF.AddBodyNameToWin(BodyName);									// Body���X�g�E�B���h�E��BODY����o�^
//	strcpy(Name,BodyName);											// �V����BODY����o�^
	Name = BodyName;											// �V����BODY����o�^
}

// Function: RegistNurbsStoBodyN
// N��NURBS�Ȗʂ�V����BODY�Ƃ��ēo�^����
// 
// Parameters:
//	*BodyList - �o�^�惊�X�g
//	Nurb[] - �o�^����NURBS�Ȗʂ̎���
//  BodyName[] - �o�^����BODY��
//	N - �o�^����NURBS�Ȗʂ̐�
void BODY::RegistNurbsStoBodyN(BODYList *BodyList,NURBSS Nurb[],const char BodyName[],int N)
{
//	NurbsS = (NURBSS *)malloc(sizeof(NURBSS)*N);
	NurbsS = new NURBSS[N];
	for(int i=0;i<N;i++){
		NurbsS[i] = Nurb[i];										// NURBS�Ȗʂ̎��̂���
		NurbsS[i].TrmdSurfFlag = KOD_FALSE;							// �g�����̂Ȃ��P����NURBS�Ȗʂł��邱�Ƃ𖾎�
		TypeNum[_NURBSS] = N;										// NURBS�Ȗʂ̐�1�ɂ���
		ChangeStatColor(this->NurbsS[i].Dstat.Color,0.2,0.2,1.0,0.5);	// �F
	}
	BodyList->add((void *)this);									// ���X�g�ɐV����BODY��o�^
	GuiIF.AddBodyNameToWin(BodyName);									// Body���X�g�E�B���h�E��BODY����o�^
//	strcpy(Name,BodyName);											// �V����BODY����o�^
	Name = BodyName;											// �V����BODY����o�^
}

// Function: ChangeStatColor
// �G���e�B�e�B�̃X�e�[�^�X�Œ�`����Ă���F��ύX
//
// Parameters:
// *col - �F��ύX�������G���e�B�e�B�̃����o�ϐ�Dstat�̃����o�ϐ�Color[4]�ւ̃|�C���^
// r,g,b,t - �F����(0.0 - 1.0)
void BODY::ChangeStatColor(float *col,float r,float g,float b,float t)
{
	col[0] = r;
	col[1] = g;
	col[2] = b;
	col[3] = t;
}

// Function: InitCurveColor
// ���̐F�̏����l��^����
//
// Parameters:
// *col - �F��ύX�������G���e�B�e�B�̃����o�ϐ�Dstat�̃����o�ϐ�Color[4]�ւ̃|�C���^
void BODY::InitCurveColor(float *col)
{
	col[0] = col[1] = col[2] = 1.0;
	col[3] = 0.5;
}

// Function: InitSurfaceColor
// �ʂ̐F�̏����l��^����
//
// Parameters:
// *col - �F��ύX�������G���e�B�e�B�̃����o�ϐ�Dstat�̃����o�ϐ�Color[4]�ւ̃|�C���^
void BODY::InitSurfaceColor(float *col)
{
	col[0] = col[1] = col[2] = 0.2;
	col[3] = 0.5;
}

// Function: NewCirA
// �~�E�~��CIRA���w�肵���������������[�m�ۂ��C����������
// 
// Parameters:
// N - �������[�m�ۂ���CIRA�̐�
CIRA* BODY::NewCirA(int N)
{
//	if((CirA = (CIRA *)malloc(sizeof(CIRA)*N/*TypeNum[_CIRCLE_ARC]*/)) == NULL)
	CirA = new CIRA[N];
	if( !CirA )
		return NULL;

/*	-- �������̓R���X�g���N�^�ɂ�
	for(int i=0;i<N;i++){
		CirA[i].zt = 0;
		CirA[i].cp[0] = CirA[i].cp[1] = CirA[i].cp[3] = SetCoord(0,0,0);
		CirA[i].R = 0;
		CirA[i].t[0] = CirA[i].t[1] = 0;
		CirA[i].U = CirA[i].V = SetCoord(0,0,0);
		CirA[i].EntUseFlag = 0;
		CirA[i].pD = 0;
		CirA[i].Dstat.Color[0] = CirA[i].Dstat.Color[1] = CirA[i].Dstat.Color[2] = CirA[i].Dstat.Color[3] = 0;
	}
*/
	TypeNum[_CIRCLE_ARC] = N;

	return CirA;
}

// Function: NewCompC
// �����Ȑ�COMPC���w�肵���������������[�m�ۂ��C����������
// 
// Parameters:
// N - �������[�m�ۂ���COMPC�̐�
COMPC* BODY::NewCompC(int N)
{
//	if((CompC = (COMPC *)malloc(sizeof(COMPC)*N/*TypeNum[_COMPOSITE_CURVE]*/)) == NULL)
	CompC = new COMPC[N];
	if ( !CompC )
		return NULL;

/*	-- �������̓R���X�g���N�^�ɂ�
	for(int i=0;i<N;i++){
		CompC[i].DegeFlag = 0;
		CompC[i].DEType = NULL;
		CompC[i].N = 0;
		CompC[i].pD = 0;
		CompC[i].pDE = NULL;
	}
*/
	TypeNum[_COMPOSITE_CURVE] = N;

	return CompC;
}

// Function: NewConA
// �~���Ȑ�CONA���w�肵���������������[�m�ۂ��C����������
// 
// Parameters:
// N - �������[�m�ۂ���CONA�̐�
CONA* BODY::NewConA(int N)
{
//	if((ConA = (CONA *)malloc(sizeof(CONA)*N/*TypeNum[_CONIC_ARC]*/)) == NULL)
	ConA = new CONA[N];
	if ( !ConA )
		return NULL;

/*	-- �������̓R���X�g���N�^�ɂ�
	for(int i=0;i<N;i++){
		ConA[i].cp[0] = ConA[i].cp[1] = SetCoord(0,0,0);
		ConA[i].Dstat.Color[0] = ConA[i].Dstat.Color[1] = ConA[i].Dstat.Color[2] = ConA[i].Dstat.Color[3] = 0;
		ConA[i].pD = 0;
		InitVector(ConA[i].prop,6);
		ConA[i].zt = 0;
	}
*/
	TypeNum[_CONIC_ARC] = N;

	return ConA;
}

// Function: NewLine
// ����LINE_���w�肵���������������[�m�ۂ��C����������
//
// Parameters:
// N - �������[�m�ۂ���LINE_�̐�
LINE_* BODY::NewLine(int N)
{
//	if((Line = (LINE_ *)malloc(sizeof(LINE_)*N/*TypeNum[_LINE]*/)) == NULL)
	Line = new LINE_[N];
	if ( !Line )
		return NULL;

/*	-- �������̓R���X�g���N�^�ɂ�
	for(int i=0;i<N;i++){
		Line[i].cp[0] = Line[i].cp[1] = SetCoord(0,0,0);
		Line[i].Dstat.Color[0] = Line[i].Dstat.Color[1] = Line[i].Dstat.Color[2] = Line[i].Dstat.Color[3] = 0;
		Line[i].EntUseFlag = 0;
		Line[i].pD = 0;
	}
*/
	TypeNum[_LINE] = N;

	return Line;
}

// Function: NewTMat
// �ϊ��}�g���b�N�XTMAT���w�肵���������������[�m�ۂ��C����������
//
// Parameters:
// N - �������[�m�ۂ���TMAT�̐�
TMAT* BODY::NewTMat(int N)
{
//	if((TMat = (TMAT *)malloc(sizeof(TMAT)*N/*TypeNum[_TRANSFORMATION_MATRIX]*/)) == NULL)
	TMat = new TMAT[N];
	if ( !TMat )
		return NULL;

/*	-- �������̓R���X�g���N�^�ɂ�
	for(int i=0;i<N;i++){
		for(int j=0;j<3;j++)
			InitVector(TMat[i].R[j],3);
		InitVector(TMat[i].T,3);
		TMat[i].pD = 0;
	}
*/
	TypeNum[_TRANSFORMATION_MATRIX] = N;

	return TMat;
}

// Function: NewNurbsC
// NURBS�Ȑ�NURBSC���w�肵���������������[�m�ۂ��C����������
//
// Parameters:
// N - �������[�m�ۂ���NURBSC�̐�
NURBSC* BODY::NewNurbsC(int N)
{
//	if((NurbsC = (NURBSC *)malloc(sizeof(NURBSC)*N/*TypeNum[_NURBSC]*/)) == NULL)
	NurbsC = new NURBSC[N];
//	NurbsC = boost::shared_array<NURBSC>( new NURBSC[N] );
	if ( !NurbsC )
		return NULL;

/*	-- �������̓R���X�g���N�^�ɂ�
	for(int i=0;i<N;i++){
		NurbsC[i].cp = NULL;
		NurbsC[i].Dstat.Color[0] = NurbsC[i].Dstat.Color[1] = NurbsC[i].Dstat.Color[2] = NurbsC[i].Dstat.Color[3] = 0;
		NurbsC[i].EntUseFlag = 0;
		NurbsC[i].K = 0;
		NurbsC[i].M = 0;
		NurbsC[i].N = 0;
		NurbsC[i].norm = SetCoord(0,0,0);
		NurbsC[i].OriginEnt = 0;
		NurbsC[i].pD = 0;
		NurbsC[i].pOriginEnt = NULL;
		NurbsC[i].prop[0] = NurbsC[i].prop[1] = NurbsC[i].prop[2] = NurbsC[i].prop[3] = 0;
		NurbsC[i].T = NULL;
		NurbsC[i].V[0] = NurbsC[i].V[1] = 0;
		NurbsC[i].W = NULL;
	}
*/
	TypeNum[_NURBSC] = N;

	return NurbsC;
//	return NurbsC.get();
}

// Function: NewNurbsS
// NURBS�Ȑ�NURBSS���w�肵���������������[�m�ۂ��C����������
//
// Parameters:
// N - �������[�m�ۂ���NURBSS�̐�
NURBSS* BODY::NewNurbsS(int N)
{
//	if((NurbsS = (NURBSS *)malloc(sizeof(NURBSS)*N/*TypeNum[_NURBSS]*/)) == NULL)
	NurbsS = new NURBSS[N];
	if ( !NurbsS )
		return NULL;

/*	-- �������̓R���X�g���N�^�ɂ�
	for(int i=0;i<N;i++){
		NurbsS[i].cp = NULL;
		NurbsS[i].Dstat.Color[0] = NurbsS[i].Dstat.Color[1] = NurbsS[i].Dstat.Color[2] = NurbsS[i].Dstat.Color[3] = 0;
		NurbsS[i].K[0] = NurbsS[i].K[1] = 0;
		NurbsS[i].M[0] = NurbsS[i].M[1] = 0;
		NurbsS[i].N[0] = NurbsS[i].N[0] = 0;
		NurbsS[i].pD = 0;
		NurbsS[i].prop[0] = NurbsS[i].prop[1] = NurbsS[i].prop[2] = NurbsS[i].prop[3] = NurbsS[i].prop[4] = 0;
		NurbsS[i].S = NULL;
		NurbsS[i].T = NULL;
		NurbsS[i].TrmdSurfFlag = 0;
		NurbsS[i].U[0] = NurbsS[i].U[1] = 0;
		NurbsS[i].V[0] = NurbsS[i].V[1] = 0;
		NurbsS[i].W = NULL;
	}
*/
	TypeNum[_NURBSS] = N;

	return NurbsS;
}

// Function: NewConpS
// �ʏ��CONPS���w�肵���������������[�m�ۂ��C����������
//
// Parameters:
// N - �������[�m�ۂ���CONPS�̐�
CONPS* BODY::NewConpS(int N)
{
//	if((ConpS = (CONPS *)malloc(sizeof(CONPS)*N/*TypeNum[_CURVE_ON_PARAMETRIC_SURFACE]*/)) == NULL)
	ConpS = new CONPS[N];
	if ( !ConpS )
		return NULL;

/*	-- �������̓R���X�g���N�^�ɂ�
	for(int i=0;i<N;i++){
		ConpS[i].BType = 0;
		ConpS[i].crtn = 0;
		ConpS[i].CType = 0;
		ConpS[i].pB = NULL;
		ConpS[i].pC = NULL;
		ConpS[i].pD = 0;
		ConpS[i].pref = 0;
		ConpS[i].pS = NULL;
		ConpS[i].SType = 0;
	}
*/
	TypeNum[_CURVE_ON_PARAMETRIC_SURFACE] = N;
	return ConpS;
}

// Function: NewTrmS
// �g������TRMS���w�肵���������������[�m�ۂ��C����������
//
// Parameters:
// N - �������[�m�ۂ���TRMS�̐�
TRMS* BODY::NewTrmS(int N)
{
//	if((TrmS = (TRMS *)malloc(sizeof(TRMS)*N/*TypeNum[_TRIMMED_SURFACE]*/)) == NULL)
	TrmS = new TRMS[N];
	if ( !TrmS )
		return NULL;

/*	-- �������̓R���X�g���N�^�ɂ�
	for(int i=0;i<N;i++){
		TrmS[i].n1 = 0;
		TrmS[i].n2 = 0;
		TrmS[i].pD = 0;
		TrmS[i].pTI = NULL;
		TrmS[i].pTO = NULL;
		TrmS[i].pts = NULL;
	}
*/
	TypeNum[_TRIMMED_SURFACE] = N;

	return TrmS;
}

// Function: GetNurbsCFromLine
// �����G���e�B�e�B��NURBS�Ȑ��G���e�B�e�B�ւƕϊ�����
//
// Parameters:
// NurbsCount - NURBS�Ȑ��ւ̕ϊ����NURBSC�̃C���f�b�N�X�ԍ�
// LineCount - �ϊ�������LINE�̃C���f�b�N�X�ԍ�
int BODY::GetNurbsCFromLine(int NurbsCount,int LineCount)	
{
	int i=0;
	int KOD_ERRflag=0;

	NurbsC[NurbsCount].K = 2;		// ���a�L���̏㑤�Y���i�R���g���[���|�C���g-1�j�̒l
	NurbsC[NurbsCount].M = 2;		// ���֐��̊K��
	NurbsC[NurbsCount].N = NurbsC[NurbsCount].K + NurbsC[NurbsCount].M;	// �m�b�g�x�N�g���̐�

	// �u�[���A���^�v���p�e�B4��
	NurbsC[NurbsCount].prop[0] = 0;
	NurbsC[NurbsCount].prop[1] = 0;
	NurbsC[NurbsCount].prop[2] = 1;
	NurbsC[NurbsCount].prop[3] = 0;

	// �������[�m��
	KOD_ERRflag++;	// 1
//	if((NurbsC[NurbsCount].T = (double *)malloc(sizeof(double)*NurbsC[NurbsCount].N)) == NULL){
	NurbsC[NurbsCount].T = new double[NurbsC[NurbsCount].N];
//	NurbsC[NurbsCount].T = boost::shared_array<double>( new double[NurbsC[NurbsCount].N] );
	if ( !NurbsC[NurbsCount].T ) {
		goto EXIT;
	}
	KOD_ERRflag++;	// 2
//	if((NurbsC[NurbsCount].W = (double *)malloc(sizeof(double)*NurbsC[NurbsCount].K)) == NULL){
	NurbsC[NurbsCount].W = new double[NurbsC[NurbsCount].K];
//	NurbsC[NurbsCount].W = boost::shared_array<double>( new double[NurbsC[NurbsCount].K] );
	if ( !NurbsC[NurbsCount].W ) {
		goto EXIT;
	}
	KOD_ERRflag++;	// 3
//	if((NurbsC[NurbsCount].cp = (Coord *)malloc(sizeof(Coord)*NurbsC[NurbsCount].K)) == NULL){
	NurbsC[NurbsCount].cp = new Coord[NurbsC[NurbsCount].K];
//	NurbsC[NurbsCount].cp = boost::shared_array<Coord>( new Coord[NurbsC[NurbsCount].K] );
	if ( !NurbsC[NurbsCount].cp ) {
		goto EXIT;
	}

	// �m�b�g�x�N�g���̒l	
	NurbsC[NurbsCount].T[0] = 0.;
	NurbsC[NurbsCount].T[1] = 0.;
	NurbsC[NurbsCount].T[2] = 1.;
	NurbsC[NurbsCount].T[3] = 1.;
	
	for(i=0;i<NurbsC[NurbsCount].K;i++){				// Weight�̒l
		NurbsC[NurbsCount].W[i] = 1.;
	}
	for(i=0;i<NurbsC[NurbsCount].K;i++){				// �R���g���[���|�C���g�̍��W�l
		NurbsC[NurbsCount].cp[i].x = Line[LineCount].cp[i].x;
		NurbsC[NurbsCount].cp[i].y = Line[LineCount].cp[i].y;
		NurbsC[NurbsCount].cp[i].z = Line[LineCount].cp[i].z;
	}
	
	// �p�����[�^�̒l
	NurbsC[NurbsCount].V[0] = 0.;
	NurbsC[NurbsCount].V[1] = 1.;

	NurbsC[NurbsCount].EntUseFlag = Line[LineCount].EntUseFlag;	// �f�B���N�g�����̏��"Entity Use Flag"�𓾂�(NURBSC)
	NurbsC[NurbsCount].OriginEnt = LINE;						// ���͐����v�f�ł��������Ƃ��L��
	NurbsC[NurbsCount].pOriginEnt = &Line[LineCount];			// ���͐����v�f�ł��������Ƃ��L��
	for(int i=0;i<4;i++)
		NurbsC[NurbsCount].Dstat.Color[i] = Line[LineCount].Dstat.Color[i];

	return KOD_TRUE;

	// �������[�m�ۂɎ��s�����ꍇ�͍��܂Ŋm�ۂ��������J������KOD_ERR��Ԃ�
EXIT:
		GuiIF.SetMessage("PARAMETER SECTION KOD_ERROR:fail to allocate memory");
	if(KOD_ERRflag == 3){
//		free(NurbsC[NurbsCount].cp);
		delete[]	NurbsC[NurbsCount].cp;
		NurbsC[NurbsCount].cp = NULL;
		KOD_ERRflag--;
	}
	if(KOD_ERRflag == 2){
//		free(NurbsC[NurbsCount].W);
		delete[]	NurbsC[NurbsCount].W;
		NurbsC[NurbsCount].W = NULL;
		KOD_ERRflag--;
	}
	if(KOD_ERRflag == 1){
//		free(NurbsC[NurbsCount].T);
		delete[]	NurbsC[NurbsCount].T;
		NurbsC[NurbsCount].T = NULL;
	}
	return KOD_ERR;
}

// Function: GetNurbsCFromCirA
// �~�E�~�ʃG���e�B�e�B��NURBS�Ȑ��G���e�B�e�B�ւƕϊ�����
//
// Parameters:
// NurbsCount - NURBS�Ȑ��ւ̕ϊ����NURBSC�̃C���f�b�N�X�ԍ�
// CirCount - �ϊ�������CIRA�̃C���f�b�N�X�ԍ�
int BODY::GetNurbsCFromCirA(int NurbsCount,int CirCount)	
{
	int	 flag=KOD_TRUE;
	double	angle_deg = 0.0,
			angle_rad = 0.0,
			radius = 0.0;
	Coord	vec[2];
	
	// �~/�~�ʂ̒��S�_O-�n�_Ps�x�N�g�������A���S�_-�I�_Pe�x�N�g�����������ꂼ�ꋁ�߂�
	vec[0] = SubCoord(CirA[CirCount].cp[1],CirA[CirCount].cp[0]);
	vec[1] = SubCoord(CirA[CirCount].cp[2],CirA[CirCount].cp[0]);	

	radius = CirA[CirCount].R;	// �~/�~�ʂ̒��S�_�Ǝn�_�̋���(���a)
	angle_rad = CalcVecAngle2D(vec[0],vec[1]);			// �~/�~�ʂ𐬂����S�p�̑傫��(degree)�����߂�
	angle_deg = RadToDeg(angle_rad);				// �~/�~�ʂ𐬂����S�p�̑傫��(radian)�����߂�

	// ���S�p(degree)�̑傫�����ƂɃZ�O�����g����ύX����
	if( angle_deg > 0 && angle_deg <= 90 ){								// 0��<��<=90��
		flag = CirAToNurbsC_seg1(NurbsCount ,CirCount ,vec, angle_rad);		// 1�Z�O�����g
	}
	else if( angle_deg > 90 && angle_deg <= 270 ){						// 90��<��<=270��
		flag = CirAToNurbsC_seg2(NurbsCount ,CirCount ,vec, angle_rad);		// 2�Z�O�����g
	}
	else if( angle_deg > 270 && angle_deg < 360 ){						// 270��<��<360��
		flag = CirAToNurbsC_seg3(NurbsCount ,CirCount ,vec, angle_rad);		// 3�Z�O�����g
	}
	else if( angle_deg == 0 ){											// ��=0��(360��)
		flag = CirAToNurbsC_seg4(NurbsCount ,CirCount ,vec, radius);			//�@4�Z�O�����g
	}
	else{
		GuiIF.SetMessage("Center angle of a circle or circular arc is not calculated normally");
		return KOD_ERR;
	}

	NurbsC[NurbsCount].EntUseFlag = CirA[CirCount].EntUseFlag;	// �f�B���N�g�����̏��"Entity Use Flag"�𓾂�(NURBSC)
	NurbsC[NurbsCount].OriginEnt = CIRCLE_ARC;					// ���͉~�E�~�ʗv�f�ł��������Ƃ��L��
	NurbsC[NurbsCount].pOriginEnt = &CirA[CirCount];		// ���̉~�E�~�ʗv�f�ւ̃|�C���^

	return KOD_TRUE;
}

// 1�Z�O�����g�̉~��(���S�p��0��<��<=90���̎�)
int BODY::CirAToNurbsC_seg1(int NurbsCount,int CirCount,Coord vec[], double angle_rad)
{
	int i=0;
	int KOD_ERRflag=0;
	
	Coord	vec_cp;
	
	NurbsC[NurbsCount].K = 3;		// ���a�L���̏㑤�Y���i�R���g���[���|�C���g-1�j�̒l
	NurbsC[NurbsCount].M = 3;		// ���֐��̊K��
	NurbsC[NurbsCount].N = NurbsC[NurbsCount].K + NurbsC[NurbsCount].M;	// �m�b�g�x�N�g���̐�

	// �u�[���A���^�v���p�e�B4��
	NurbsC[NurbsCount].prop[0] = 0;
	NurbsC[NurbsCount].prop[1] = 0;
	NurbsC[NurbsCount].prop[2] = 1;
	NurbsC[NurbsCount].prop[3] = 0;
	
	// �������[�m��
	KOD_ERRflag++;	// 1
//	if((NurbsC[NurbsCount].T = (double *)malloc(sizeof(double)*NurbsC[NurbsCount].N)) == NULL){
	NurbsC[NurbsCount].T = new double[NurbsC[NurbsCount].N];
//	NurbsC[NurbsCount].T = boost::shared_array<double>( new double[NurbsC[NurbsCount].N] );
	if ( !NurbsC[NurbsCount].T ) {
		goto EXIT;
	}
	KOD_ERRflag++;	// 2
//	if((NurbsC[NurbsCount].W = (double *)malloc(sizeof(double)*NurbsC[NurbsCount].K)) == NULL){
	NurbsC[NurbsCount].W = new double[NurbsC[NurbsCount].K];
//	NurbsC[NurbsCount].W = boost::shared_array<double>( new double[NurbsC[NurbsCount].K] );
	if ( !NurbsC[NurbsCount].W ) {
		goto EXIT;
	}
	KOD_ERRflag++;	// 3
//	if((NurbsC[NurbsCount].cp = (Coord *)malloc(sizeof(Coord)*NurbsC[NurbsCount].K)) == NULL){
	NurbsC[NurbsCount].cp = new Coord[NurbsC[NurbsCount].K];
//	NurbsC[NurbsCount].cp = boost::shared_array<Coord>( new Coord[NurbsC[NurbsCount].K] );
	if ( !NurbsC[NurbsCount].cp ) {
		goto EXIT;
	}
	
	// �m�b�g�x�N�g���̒l	
	NurbsC[NurbsCount].T[0] = 0.;
	NurbsC[NurbsCount].T[1] = 0.;
	NurbsC[NurbsCount].T[2] = 0.;
	NurbsC[NurbsCount].T[3] = 1.;
	NurbsC[NurbsCount].T[4] = 1.;
	NurbsC[NurbsCount].T[5] = 1.;
		
	// Weight�̒l
	for(i=0; i<3; i++){
		if(i % 2 == 0){
			NurbsC[NurbsCount].W[i] = 1.;
		}	
		else if(i % 2 == 1){	
			NurbsC[NurbsCount].W[i] = cos(angle_rad/2);
		}
	}
		
	vec_cp = Arc_CP(vec[0], vec[1], cos(angle_rad));	//�@�~�̒��S�_����R���g���[���|�C���gP1�ւ̃x�N�g�������߂�
	
	// �R���g���[���|�C���g�̍��W�l
	NurbsC[NurbsCount].cp[0].x = CirA[CirCount].cp[1].x;
	NurbsC[NurbsCount].cp[0].y = CirA[CirCount].cp[1].y;		
	NurbsC[NurbsCount].cp[1].x = vec_cp.x + CirA[CirCount].cp[0].x;
	NurbsC[NurbsCount].cp[1].y = vec_cp.y + CirA[CirCount].cp[0].y;
	NurbsC[NurbsCount].cp[2].x = CirA[CirCount].cp[2].x;
	NurbsC[NurbsCount].cp[2].y = CirA[CirCount].cp[2].y;

	for(i=0; i<3; i++){
		NurbsC[NurbsCount].cp[i].z = CirA[CirCount].zt;	// Z�����̑傫���͈��
	}
		
	NurbsC[NurbsCount].V[0] = 0.;		// �p�����[�^�̒l
	NurbsC[NurbsCount].V[1] = 1.;
		  
	return KOD_TRUE;

	// �������[�m�ۂɎ��s�����ꍇ�͍��܂Ŋm�ۂ��������J������KOD_ERR��Ԃ�
EXIT:
		GuiIF.SetMessage("PARAMETER SECTION KOD_ERROR:fail to allocate memory");
	if(KOD_ERRflag == 3){
//		free(NurbsC[NurbsCount].cp);
		delete[]	NurbsC[NurbsCount].cp;
		NurbsC[NurbsCount].cp = NULL;
		KOD_ERRflag--;
	}
	if(KOD_ERRflag == 2){
//		free(NurbsC[NurbsCount].W);
		delete[]	NurbsC[NurbsCount].W;
		NurbsC[NurbsCount].W = NULL;
		KOD_ERRflag--;
	}
	if(KOD_ERRflag == 1){
//		free(NurbsC[NurbsCount].T);
		delete[]	NurbsC[NurbsCount].T;
		NurbsC[NurbsCount].T = NULL;
	}
	return KOD_ERR;
}

// private
// 2�Z�O�����g�̉~��(���S�p��90��<��<=270���̎�)
int BODY::CirAToNurbsC_seg2(int NurbsCount,int CirCount,Coord vec[], double angle_rad)
{
	int	i = 0,
		KOD_ERRflag = 0;
	double	angle_rad2 = 0.0;
	
	Coord vec_cp[3];
	
	NurbsC[NurbsCount].K = 5;		// ���a�L���̏㑤�Y���i�R���g���[���|�C���g-1�j�̒l
	NurbsC[NurbsCount].M = 3;		// ���֐��̊K��
	NurbsC[NurbsCount].N = NurbsC[NurbsCount].K + NurbsC[NurbsCount].M;	// �m�b�g�x�N�g���̐�
	// �u�[���A���^�v���p�e�B4��
	NurbsC[NurbsCount].prop[0] = 0;
	NurbsC[NurbsCount].prop[1] = 0;
	NurbsC[NurbsCount].prop[2] = 1;
	NurbsC[NurbsCount].prop[3] = 0;
	
	// �������[�m��
	KOD_ERRflag++;	// 1
//	if((NurbsC[NurbsCount].T = (double *)malloc(sizeof(double)*NurbsC[NurbsCount].N)) == NULL){
	NurbsC[NurbsCount].T = new double[NurbsC[NurbsCount].N];
//	NurbsC[NurbsCount].T = boost::shared_array<double>( new double[NurbsC[NurbsCount].N] );
	if ( !NurbsC[NurbsCount].T ) {
		goto EXIT;
	}
	KOD_ERRflag++;	// 2
//	if((NurbsC[NurbsCount].W = (double *)malloc(sizeof(double)*NurbsC[NurbsCount].K)) == NULL){
	NurbsC[NurbsCount].W = new double[NurbsC[NurbsCount].K];
//	NurbsC[NurbsCount].W = boost::shared_array<double>( new double[NurbsC[NurbsCount].K] );
	if ( !NurbsC[NurbsCount].W ) {
		goto EXIT;
	}
	KOD_ERRflag++;	// 3
//	if((NurbsC[NurbsCount].cp = (Coord *)malloc(sizeof(Coord)*NurbsC[NurbsCount].K)) == NULL){
	NurbsC[NurbsCount].cp = new Coord[NurbsC[NurbsCount].K];
//	NurbsC[NurbsCount].cp = boost::shared_array<Coord>( new Coord[NurbsC[NurbsCount].K] );
	if ( !NurbsC[NurbsCount].cp ) {
		goto EXIT;
	}
	
	// �m�b�g�x�N�g���̒l	
	NurbsC[NurbsCount].T[0] = 0.;
	NurbsC[NurbsCount].T[1] = 0.;
	NurbsC[NurbsCount].T[2] = 0.;
	NurbsC[NurbsCount].T[3] = 2./4.;
	NurbsC[NurbsCount].T[4] = 2./4.;
	NurbsC[NurbsCount].T[5] = 1.;
	NurbsC[NurbsCount].T[6] = 1.;
	NurbsC[NurbsCount].T[7] = 1.;
		
	// Weight�̒l
	for(i=0; i<5; i++){
		if(i % 2 == 0){
			NurbsC[NurbsCount].W[i] = 1.;
		}	
		else if(i % 2 == 1){	
			NurbsC[NurbsCount].W[i] = cos(angle_rad/4);
		}
	}
		
	angle_rad2 = angle_rad/2;	// (���S�p)��2
	
	vec_cp[1] = CalcRotVec2D(vec[0], angle_rad2);		// �~�̒��S�_���璆�S�p�̔����̈ʒu(�R���g���[���|�C���gP2)�ւ̃x�N�g�������߂�
	vec_cp[0] = Arc_CP(vec[0], vec_cp[1], cos(angle_rad2));	// �~�̒��S�_����R���g���[���|�C���gP1�ւ̃x�N�g�������߂�
	vec_cp[2] = Arc_CP(vec_cp[1], vec[1], cos(angle_rad2));	// �~�̒��S�_����R���g���[���|�C���gP3�ւ̃x�N�g�������߂�
	
	// �R���g���[���|�C���g�̍��W�l
	NurbsC[NurbsCount].cp[0].x = CirA[CirCount].cp[1].x;
	NurbsC[NurbsCount].cp[0].y = CirA[CirCount].cp[1].y;		
 	NurbsC[NurbsCount].cp[1].x = vec_cp[0].x + CirA[CirCount].cp[0].x;
 	NurbsC[NurbsCount].cp[1].y = vec_cp[0].y + CirA[CirCount].cp[0].y;
 	NurbsC[NurbsCount].cp[2].x = vec_cp[1].x + CirA[CirCount].cp[0].x;
 	NurbsC[NurbsCount].cp[2].y = vec_cp[1].y + CirA[CirCount].cp[0].y;
 	NurbsC[NurbsCount].cp[3].x = vec_cp[2].x + CirA[CirCount].cp[0].x;
 	NurbsC[NurbsCount].cp[3].y = vec_cp[2].y + CirA[CirCount].cp[0].y;
 	NurbsC[NurbsCount].cp[4].x = CirA[CirCount].cp[2].x;
 	NurbsC[NurbsCount].cp[4].y = CirA[CirCount].cp[2].y;
	for(i=0; i<5; i++){
		NurbsC[NurbsCount].cp[i].z = CirA[CirCount].zt;	// Z�����̑傫���͈��
	}
	
	NurbsC[NurbsCount].V[0] = 0.;		// �p�����[�^�̒l
	NurbsC[NurbsCount].V[1] = 1.;
		  
	return KOD_TRUE;

	// �������[�m�ۂɎ��s�����ꍇ�͍��܂Ŋm�ۂ��������J������KOD_ERR��Ԃ�
EXIT:
		GuiIF.SetMessage("PARAMETER SECTION KOD_ERROR:fail to allocate memory");
	if(KOD_ERRflag == 3){
//		free(NurbsC[NurbsCount].cp);
		delete[]	NurbsC[NurbsCount].cp;
		NurbsC[NurbsCount].cp = NULL;
		KOD_ERRflag--;
	}
	if(KOD_ERRflag == 2){
//		free(NurbsC[NurbsCount].W);
		delete[]	NurbsC[NurbsCount].W;
		NurbsC[NurbsCount].W = NULL;
		KOD_ERRflag--;
	}
	if(KOD_ERRflag == 1){
//		free(NurbsC[NurbsCount].T);
		delete[]	NurbsC[NurbsCount].T;
		NurbsC[NurbsCount].T = NULL;
	}
	return KOD_ERR;
}

// private
// 3�Z�O�����g�̉~��(���S�p��270��<��<360���̎�)
int BODY::CirAToNurbsC_seg3(int NurbsCount,int CirCount,Coord vec[], double angle_rad)
{
	int	i=0,
		KOD_ERRflag=0;
	double	angle_rad3 = 0.0;
	
	Coord	vec_cp[5];
	
	NurbsC[NurbsCount].K = 7;		// ���a�L���̏㑤�Y���i�R���g���[���|�C���g-1�j�̒l
	NurbsC[NurbsCount].M = 3;		// ���֐��̊K��
	NurbsC[NurbsCount].N = NurbsC[NurbsCount].K + NurbsC[NurbsCount].M;	// �m�b�g�x�N�g���̐�
	
	// �u�[���A���^�v���p�e�B4��
	NurbsC[NurbsCount].prop[0] = 0;
	NurbsC[NurbsCount].prop[1] = 0;
	NurbsC[NurbsCount].prop[2] = 1;
	NurbsC[NurbsCount].prop[3] = 0;
	
	// �������[�m��
	KOD_ERRflag++;	// 1
//	if((NurbsC[NurbsCount].T = (double *)malloc(sizeof(double)*NurbsC[NurbsCount].N)) == NULL){
	NurbsC[NurbsCount].T = new double[NurbsC[NurbsCount].N];
//	NurbsC[NurbsCount].T = boost::shared_array<double>( new double[NurbsC[NurbsCount].N] );
	if ( !NurbsC[NurbsCount].T ) {
		goto EXIT;
	}
	KOD_ERRflag++;	// 2
//	if((NurbsC[NurbsCount].W = (double *)malloc(sizeof(double)*NurbsC[NurbsCount].K)) == NULL){
	NurbsC[NurbsCount].W = new double[NurbsC[NurbsCount].K];
//	NurbsC[NurbsCount].W = boost::shared_array<double>( new double[NurbsC[NurbsCount].K] );
	if ( !NurbsC[NurbsCount].W ) {
		goto EXIT;
	}
	KOD_ERRflag++;	// 3
//	if((NurbsC[NurbsCount].cp = (Coord *)malloc(sizeof(Coord)*NurbsC[NurbsCount].K)) == NULL){
	NurbsC[NurbsCount].cp = new Coord[NurbsC[NurbsCount].K];
//	NurbsC[NurbsCount].cp = boost::shared_array<Coord>( new Coord[NurbsC[NurbsCount].K] );
	if ( !NurbsC[NurbsCount].cp ) {
		goto EXIT;
	}
	
	// �m�b�g�x�N�g���̒l	
	NurbsC[NurbsCount].T[0] = 0.;
	NurbsC[NurbsCount].T[1] = 0.;
	NurbsC[NurbsCount].T[2] = 0.;
	NurbsC[NurbsCount].T[3] = 1./3.;
	NurbsC[NurbsCount].T[4] = 1./3.;
	NurbsC[NurbsCount].T[5] = 2./3.;
	NurbsC[NurbsCount].T[6] = 2./3.;
	NurbsC[NurbsCount].T[7] = 1.;
	NurbsC[NurbsCount].T[8] = 1.;
	NurbsC[NurbsCount].T[9] = 1.;
	
	// Weight�̒l
	for(i=0; i<7; i++){
		if(i % 2 == 0){
			NurbsC[NurbsCount].W[i] = 1.;
		}	
		else if(i % 2 == 1){	
			NurbsC[NurbsCount].W[i] = cos(angle_rad/6);
		}
	}

	angle_rad3 = angle_rad/3;	// (���S�p)��3
	
	vec_cp[1] = CalcRotVec2D(vec[0], angle_rad3);		// �~�̒��S�_���璆�S�p��1/3�̈ʒu(�R���g���[���|�C���gP2)�ւ̃x�N�g�������߂�
	vec_cp[0] = Arc_CP(vec[0], vec_cp[1], cos(angle_rad3));	// �~�̒��S�_����R���g���[���|�C���gP1�ւ̃x�N�g�������߂�
	vec_cp[3] = CalcRotVec2D(vec_cp[1], angle_rad3);		// �~�̒��S�_���璆�S�p��2/3�̈ʒu(�R���g���[���|�C���gP4)�ւ̃x�N�g�������߂�
	vec_cp[2] = Arc_CP(vec_cp[1], vec_cp[3], cos(angle_rad3));	// �~�̒��S�_����R���g���[���|�C���gP3�ւ̃x�N�g�������߂�
	vec_cp[4] = Arc_CP(vec_cp[3], vec[1], cos(angle_rad3));	// �~�̒��S�_����R���g���[���|�C���gP4�ւ̃x�N�g�������߂�
		
	// �R���g���[���|�C���g�̍��W�l
	NurbsC[NurbsCount].cp[0].x = CirA[CirCount].cp[1].x;
	NurbsC[NurbsCount].cp[0].y = CirA[CirCount].cp[1].y;		
	NurbsC[NurbsCount].cp[1].x = vec_cp[0].x + CirA[CirCount].cp[0].x;
	NurbsC[NurbsCount].cp[1].y = vec_cp[0].y + CirA[CirCount].cp[0].y;
	NurbsC[NurbsCount].cp[2].x = vec_cp[1].x + CirA[CirCount].cp[0].x;
	NurbsC[NurbsCount].cp[2].y = vec_cp[1].y + CirA[CirCount].cp[0].y;
	NurbsC[NurbsCount].cp[3].x = vec_cp[2].x + CirA[CirCount].cp[0].x;
	NurbsC[NurbsCount].cp[3].y = vec_cp[2].y + CirA[CirCount].cp[0].y;
	NurbsC[NurbsCount].cp[4].x = vec_cp[3].x + CirA[CirCount].cp[0].x;
	NurbsC[NurbsCount].cp[4].y = vec_cp[3].y + CirA[CirCount].cp[0].y;
	NurbsC[NurbsCount].cp[5].x = vec_cp[4].x + CirA[CirCount].cp[0].x;
	NurbsC[NurbsCount].cp[5].y = vec_cp[4].y + CirA[CirCount].cp[0].y;
	NurbsC[NurbsCount].cp[6].x = CirA[CirCount].cp[2].x;
	NurbsC[NurbsCount].cp[6].y = CirA[CirCount].cp[2].y;

	for(i=0; i<7; i++){
		NurbsC[NurbsCount].cp[i].z = CirA[CirCount].zt;	// Z�����̑傫���͈��
	}
		
	NurbsC[NurbsCount].V[0] = 0.;		// �p�����[�^�̒l
	NurbsC[NurbsCount].V[1] = 1.;
		  
	return KOD_TRUE;

	// �������[�m�ۂɎ��s�����ꍇ�͍��܂Ŋm�ۂ��������J������KOD_ERR��Ԃ�
EXIT:
		GuiIF.SetMessage("PARAMETER SECTION KOD_ERROR:fail to allocate memory");
	if(KOD_ERRflag == 3){
//		free(NurbsC[NurbsCount].cp);
		delete[]	NurbsC[NurbsCount].cp;
		NurbsC[NurbsCount].cp = NULL;
		KOD_ERRflag--;
	}
	if(KOD_ERRflag == 2){
//		free(NurbsC[NurbsCount].W);
		delete[]	NurbsC[NurbsCount].W;
		NurbsC[NurbsCount].W = NULL;
		KOD_ERRflag--;
	}
	if(KOD_ERRflag == 1){
//		free(NurbsC[NurbsCount].T);
		delete[]	NurbsC[NurbsCount].T;
		NurbsC[NurbsCount].T = NULL;
	}
	return KOD_ERR;
}

// private
// 4�Z�O�����g�̉~��(�~)
int BODY::CirAToNurbsC_seg4(int NurbsCount,int CirCount,Coord vec[], double radius)
{
	int i=0;
	int KOD_ERRflag=0;

	NurbsC[NurbsCount].K = 9;		// ���a�L���̏㑤�Y���i�R���g���[���|�C���g-1�j�̒l
	NurbsC[NurbsCount].M = 3;		// ���֐��̊K��
	NurbsC[NurbsCount].N = NurbsC[NurbsCount].K + NurbsC[NurbsCount].M;	// �m�b�g�x�N�g���̐�
	
	// �u�[���A���^�v���p�e�B4��
	NurbsC[NurbsCount].prop[0] = 0;
	NurbsC[NurbsCount].prop[1] = 0;
	NurbsC[NurbsCount].prop[2] = 1;
	NurbsC[NurbsCount].prop[3] = 0;
	
	// �������[�m��
	KOD_ERRflag++;	// 1
//	if((NurbsC[NurbsCount].T = (double *)malloc(sizeof(double)*NurbsC[NurbsCount].N)) == NULL){
	NurbsC[NurbsCount].T = new double[NurbsC[NurbsCount].N];
//	NurbsC[NurbsCount].T = boost::shared_array<double>( new double[NurbsC[NurbsCount].N] );
	if ( !NurbsC[NurbsCount].T ) {
		goto EXIT;
	}
	KOD_ERRflag++;	// 2
//	if((NurbsC[NurbsCount].W = (double *)malloc(sizeof(double)*NurbsC[NurbsCount].K)) == NULL){
	NurbsC[NurbsCount].W = new double[NurbsC[NurbsCount].K];
//	NurbsC[NurbsCount].W = boost::shared_array<double>( new double[NurbsC[NurbsCount].K] );
	if ( !NurbsC[NurbsCount].W ) {
		goto EXIT;
	}
	KOD_ERRflag++;	// 3
//	if((NurbsC[NurbsCount].cp = (Coord *)malloc(sizeof(Coord)*NurbsC[NurbsCount].K)) == NULL){
	NurbsC[NurbsCount].cp = new Coord[NurbsC[NurbsCount].K];
//	NurbsC[NurbsCount].cp = boost::shared_array<Coord>( new Coord[NurbsC[NurbsCount].K] );
	if ( !NurbsC[NurbsCount].cp ) {
		goto EXIT;
	}
	
	// �m�b�g�x�N�g���̒l	
	NurbsC[NurbsCount].T[0] = 0.;
	NurbsC[NurbsCount].T[1] = 0.;
	NurbsC[NurbsCount].T[2] = 0.;
	NurbsC[NurbsCount].T[3] = 1./4.;
	NurbsC[NurbsCount].T[4] = 1./4.;
	NurbsC[NurbsCount].T[5] = 2./4.;
	NurbsC[NurbsCount].T[6] = 2./4.;
	NurbsC[NurbsCount].T[7] = 3./4.;
	NurbsC[NurbsCount].T[8] = 3./4.;
	NurbsC[NurbsCount].T[9] = 1.;
	NurbsC[NurbsCount].T[10] = 1.;
	NurbsC[NurbsCount].T[11] = 1.;
		
	// Weight�̒l
	for(i=0; i<9; i++){
		if(i % 2 == 0){
			NurbsC[NurbsCount].W[i] = 1.;
		}	
		else if(i % 2 == 1){	
			NurbsC[NurbsCount].W[i] = sqrt(2.0)/2;
		}
	}

	// �R���g���[���|�C���g�̍��W�l
	NurbsC[NurbsCount].cp[0].x = CirA[CirCount].cp[0].x + radius;
	NurbsC[NurbsCount].cp[0].y = CirA[CirCount].cp[0].y;		
	NurbsC[NurbsCount].cp[1].x = CirA[CirCount].cp[0].x + radius;
	NurbsC[NurbsCount].cp[1].y = CirA[CirCount].cp[0].y + radius;
	NurbsC[NurbsCount].cp[2].x = CirA[CirCount].cp[0].x;
	NurbsC[NurbsCount].cp[2].y = CirA[CirCount].cp[0].y + radius;
	NurbsC[NurbsCount].cp[3].x = CirA[CirCount].cp[0].x - radius;
	NurbsC[NurbsCount].cp[3].y = CirA[CirCount].cp[0].y + radius;
	NurbsC[NurbsCount].cp[4].x = CirA[CirCount].cp[0].x - radius;
	NurbsC[NurbsCount].cp[4].y = CirA[CirCount].cp[0].y;
	NurbsC[NurbsCount].cp[5].x = CirA[CirCount].cp[0].x - radius;
	NurbsC[NurbsCount].cp[5].y = CirA[CirCount].cp[0].y - radius;
	NurbsC[NurbsCount].cp[6].x = CirA[CirCount].cp[0].x;
	NurbsC[NurbsCount].cp[6].y = CirA[CirCount].cp[0].y - radius;
	NurbsC[NurbsCount].cp[7].x = CirA[CirCount].cp[0].x + radius;
	NurbsC[NurbsCount].cp[7].y = CirA[CirCount].cp[0].y - radius;
	NurbsC[NurbsCount].cp[8].x = CirA[CirCount].cp[0].x + radius;
	NurbsC[NurbsCount].cp[8].y = CirA[CirCount].cp[0].y;

	for(i=0; i<9; i++){
		NurbsC[NurbsCount].cp[i].z = CirA[CirCount].zt;	// Z�����̑傫���͈��
	}
		
	NurbsC[NurbsCount].V[0] = 0.;		// �p�����[�^�̒l
	NurbsC[NurbsCount].V[1] = 1.;
	
	return KOD_TRUE;

	// �������[�m�ۂɎ��s�����ꍇ�͍��܂Ŋm�ۂ��������J������KOD_ERR��Ԃ�
EXIT:
		GuiIF.SetMessage("PARAMETER SECTION KOD_ERROR:fail to allocate memory");
	if(KOD_ERRflag == 3){
//		free(NurbsC[NurbsCount].cp);
		delete[]	NurbsC[NurbsCount].cp;
		NurbsC[NurbsCount].cp = NULL;
		KOD_ERRflag--;
	}
	if(KOD_ERRflag == 2){
//		free(NurbsC[NurbsCount].W);
		delete[]	NurbsC[NurbsCount].W;
		NurbsC[NurbsCount].W = NULL;
		KOD_ERRflag--;
	}
	if(KOD_ERRflag == 1){
//		free(NurbsC[NurbsCount].T);
		delete[]	NurbsC[NurbsCount].T;
		NurbsC[NurbsCount].T = NULL;
	}
	return KOD_ERR;
}

// Function: GetOuterEdgeNum
// �g�����ʂ��\������O���G�b�W�̐����擾����
//
// Return:
// �g�����ʂ��\������O���G�b�W�̐�
int TRMS::GetOuterEdgeNum()
{
//	COMPC *CompC = (COMPC *)pTO->pB;
	COMPC *CompC = pTO->pB.CompC;
    return CompC->N;
}

// Function: GetInnerTrmNum
// �g�����ʂ��\����������g�����̐����擾����
//
// Return:
// �g�����ʂ��\����������g�����̐�
int TRMS::GetInnerTrmNum()
{
    return n2;
}

// Function: GetInnerEdgeNum
// �g�����ʂ��\����������G�b�W�̐����擾����
//
// Parameters:
// N - �����g�����̃C���f�b�N�X�ԍ�
//
// Return:
// �g�����ʂ��\����������G�b�W�̐�
int TRMS::GetInnerEdgeNum(int N)
{
//	COMPC *CompC = (COMPC *)pTI[N]->pB;
	COMPC *CompC = pTI[N]->pB.CompC;
    return CompC->N;
}

// Function: GetOuterCompC
// �g�����ʂ��\������O���g�����Ȑ�(�����Ȑ�)�ւ̃|�C���^���擾����
//
// Return:
// �g�����ʂ��\������O���g�����Ȑ�(�����Ȑ�)�ւ̃|�C���^
COMPC *TRMS::GetOuterCompC()
{
//	return (COMPC *)pTO->pB;
	return pTO->pB.CompC;
}

// Function: GetInnerCompC
// �g�����ʂ��\������O���g�����Ȑ�(�����Ȑ�)�ւ̃|�C���^���擾����
//
// Parameters:
// N - �����g�����̃C���f�b�N�X�ԍ�
//
// Return:
// �g�����ʂ��\������O���g�����Ȑ�(�����Ȑ�)�ւ̃|�C���^
COMPC *TRMS::GetInnerCompC(int N)
{
//	return (COMPC *)pTI[N]->pB;
	return pTI[N]->pB.CompC;
}

// Funciton: GetNurbsS
// �g�����ʂ��\������NURBS�Ȗʂւ̃|�C���^�𓾂�
//
// Return:
// �g�����ʂ��\������NURBS�Ȗʂւ̃|�C���^
NURBSS *TRMS::GetNurbsS()
{
    return pts;
}
