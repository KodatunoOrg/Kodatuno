/*************************
* DXF�t�@�C����ǂݍ���  *
**************************/

#include "DXF_Parser.h"

// �R���X�g���N�^
DXF_PARSER::DXF_PARSER()
{
	for(int i=0;i<ALL_ENTITY_TYPE_NUM;i++){
		Count[i] = 0;
	}
}

// *body --- ���̂��\������G���e�B�e�B�̏W���I�u�W�F�N�g�ւ̃|�C���^
// TypeNum[] --- �e�G���e�B�e�B�̐����i�[�����
int DXF_PARSER::DXF_Parser_Main(BODY *body,const char *DXF_fname)
{
	FILE *fp;
	NURBS_Func nfunc;
	char mes[BUFSIZEMAX];		// �o�͗p���b�Z�[�W�i�[�o�b�t�@
	int line = 1;				// ���݂̍s�ԍ�
	int section = 0;			// �Z�N�V�����ԍ�

	// DXF�t�@�C���I�[�v��
	if((fp = fopen(DXF_fname,"r")) == NULL){
		sprintf(mes,"KOD_ERROR: Cannot open %s",DXF_fname);
		GuiIF.SetMessage(mes);
		return(KOD_ERR);
	}
	sprintf(mes,"Open %s",DXF_fname);
	GuiIF.SetMessage(mes);

	// �t�@�C���ǂݍ���
	while(fgets(Buf,sizeof(Buf),fp) != NULL){

		if(EvenOdd(line) == ODD)			// ��s�̂Ƃ��̓O���[�v�R�[�h��ǂݍ���
			sscanf(Buf,"%d",&Gcode);		

		// �O���[�v�R�[�h���Z�N�V��������\��"2"��������
		if(Gcode == SECTION_NAME){
			fgets(Buf,sizeof(Buf),fp);		// �����1�s�ǂݍ��݁C
			section = CheckSection(Buf);	// �Z�N�V�������𔻕�

			// �Z�N�V���������Ƃɓǂݍ���
			if(section == HEADER_SECTION)
				ReadHeaderSection(fp,line,body);		// �w�b�_�Z�N�V�����̓ǂݍ���
			else if(section == CLASS_SECTION)
				ReadClassesSection(fp,line,body);		// �N���X�Z�N�V�����̓ǂݍ���
			else if(section == TABLE_SECTION)
				ReadTablesSection(fp,line,body);		// �e�[�u���Z�N�V�����̓ǂݍ���
			else if(section == BLOCK_SECTION)
				ReadBlocksSection(fp,line,body);		// �u���b�N�Z�N�V�����̓ǂݍ���
			else if(section == ENTITIY_SECTION)
				ReadEntitiesSection(fp,line,body);		// �G���e�B�e�B�Z�N�V�����̓ǂݍ���
			else if(section == OBJECT_SECTION)
				ReadObjectsSection(fp,line,body);		// �I�u�W�F�N�g�Z�N�V�����̓ǂݍ���
			line++;
		}

		line++;
	}

	fclose(fp);

	body->MaxCoord = 200;

	return KOD_TRUE;
}

// �G���e�B�e�B�Z�N�V�����̓ǂݍ���
int DXF_PARSER::ReadEntitiesSection(FILE *fp,int Line,BODY *body)
{
	fpos_t pos;

	fgetpos(fp,&pos);			// ���݂�fp�̈ʒu���o���Ă���

	ResearchEntNum(fp,body);	// �e�G���e�B�e�B�̐������T�[�`
	body->NewBodyElem();		// BODY�\���̓��̊e�G���e�B�e�B�̃������[�m��
	fseek(fp,pos,SEEK_SET);		// �G���e�B�e�B�Z�N�V�����̍ŏ��̈ʒu�֖߂�
	fgets(Buf,sizeof(Buf),fp);	// 1�s�ǂݎ̂�

	// ���f�[�^�擾
	do{
		fgets(Buf,sizeof(Buf),fp);	// �����s
		sscanf(Buf,"%s",&Label);

		if(strcmp(Label,"ARC") == 0)			GetArcData(fp,body);		// �~��
		else if(strcmp(Label,"CIRCLE") == 0)	GetCircleData(fp,body);		// �~
		else if(strcmp(Label,"LINE") == 0)		GetLineData(fp,body);		// ����
		else	fgets(Buf,sizeof(Buf),fp);	// �O���[�v�R�[�h(��s)�ǂݎ̂�

	}while(strcmp(Label,"ENDSEC"));

	ChangeEntityforNurbs(body);		// �~�C�����G���e�B�e�B��NURBS�ɕϊ�

	return KOD_TRUE;
}

// �~�C�����G���e�B�e�B��NURBS�ɕϊ�
int DXF_PARSER::ChangeEntityforNurbs(BODY *body)
{
	int ncount = 0;
	for(int i=0;i<body->TypeNum[_LINE];i++){
		if(body->GetNurbsCFromLine(ncount,i) == KOD_ERR) return KOD_ERR;	// �~/�~�ʃp�����[�^����NURBS�Ȑ��p�����[�^�𓾂�
		InitDisplayStat(&body->NurbsC[ncount].Dstat);						// �\�������̏�����
		ncount++;
	}
	for(int i=0;i<body->TypeNum[_CIRCLE_ARC];i++){
		if(body->GetNurbsCFromCirA(ncount,i) == KOD_ERR) return KOD_ERR;	// �����p�����[�^����NURBS�Ȑ��p�����[�^�𓾂�
		InitDisplayStat(&body->NurbsC[ncount].Dstat);								// �\�������̏�����
		ncount++;
	}
}


// �~�ʂ̃f�[�^���擾��BODY�Ɋi�[
int DXF_PARSER::GetArcData(FILE *fp,BODY *body)
{
	while(1){
		fgets(Buf,sizeof(Buf),fp);		// ��s
		sscanf(Buf,"%d",&Gcode);

		if(Gcode == ENT_TYPE)	break;	// �O���[�v�R�[�h:0�Ȃ�while break

		else{
			fgets(Buf,sizeof(Buf),fp);	// �����s

			if(Gcode == COORD_X)
				sscanf(Buf,"%lf",&body->CirA[Count[_CIRCLE_ARC]].cp[0].x);		// �~�ʂ̒��S���WX
			else if(Gcode == COORD_Y)
				sscanf(Buf,"%lf",&body->CirA[Count[_CIRCLE_ARC]].cp[0].y);		// �~�ʂ̒��S���WY
			else if(Gcode == COORD_Z)
				sscanf(Buf,"%lf",&body->CirA[Count[_CIRCLE_ARC]].cp[0].z);		// �~�ʂ̒��S���WZ
			else if(Gcode == RADIUS)
				sscanf(Buf,"%lf",&body->CirA[Count[_CIRCLE_ARC]].R);			// �~�ʂ̔��a
			else if(Gcode == START_ANG)
				sscanf(Buf,"%lf",&body->CirA[Count[_CIRCLE_ARC]].t[0]);			// �~�ʂ̊J�n�p�x
			else if(Gcode == END_ANG)
				sscanf(Buf,"%lf",&body->CirA[Count[_CIRCLE_ARC]].t[1]);			// �~�ʂ̏I���p�x
		}
	}

	// debug
	//fprintf(stderr,"%d:%lf,%lf,%lf,%lf,%lf,%lf\n",Count[_CIRCLE_ARC],
	//	body->CirA[Count[_CIRCLE_ARC]].cp[0].x,
	//	body->CirA[Count[_CIRCLE_ARC]].cp[0].y,
	//	body->CirA[Count[_CIRCLE_ARC]].cp[0].z,
	//	body->CirA[Count[_CIRCLE_ARC]].R,
	//	body->CirA[Count[_CIRCLE_ARC]].t[0],
	//	body->CirA[Count[_CIRCLE_ARC]].t[1]);

	SetStartEndPtArc(&body->CirA[Count[_CIRCLE_ARC]]);			// �~�ʂ̎n�_�C�I�_���Z�b�g
	CalcUVvec(&body->CirA[Count[_CIRCLE_ARC]]);					// CIRA��UV�������W��ݒ肷��

	InitDisplayStat(&body->CirA[Count[_CIRCLE_ARC]].Dstat);		// �F�w��
	body->CirA[Count[_CIRCLE_ARC]].EntUseFlag = GEOMTRYELEM;	// �􉽗v�f�ł��邱�Ƃ�錾
	body->CirA[Count[_CIRCLE_ARC]].pD = NULL;					// IGES�łȂ��̂Ŋ֌W�Ȃ�

	Count[_CIRCLE_ARC]++;

	return KOD_TRUE;
}

// CIRA��UV�������W��ݒ肷��
int DXF_PARSER::CalcUVvec(CIRA *Cira)
{
	// X-Y����
	Cira->U = SetCoord(1,0,0);
	Cira->V = SetCoord(0,1,0);

	return KOD_TRUE;
}

// �~�ʂ̎n�_�C�I�_���Z�b�g
int DXF_PARSER::SetStartEndPtArc(CIRA *Cira)
{
	double sth = Cira->t[0]*PI/180;
	double eth = Cira->t[1]*PI/180;

	Cira->cp[1].x = Cira->R*cos(sth) + Cira->cp[0].x;
	Cira->cp[1].y = Cira->R*sin(sth) + Cira->cp[0].y;
	Cira->cp[1].z = 0;
	Cira->cp[2].x = Cira->R*cos(eth) + Cira->cp[0].x;
	Cira->cp[2].y = Cira->R*sin(eth) + Cira->cp[0].y;
	Cira->cp[2].z = 0;

	return KOD_TRUE;
}

// �~�̃f�[�^���擾��BODY�Ɋi�[
int DXF_PARSER::GetCircleData(FILE *fp,BODY *body)
{
	while(1){
		fgets(Buf,sizeof(Buf),fp);		// ��s
		sscanf(Buf,"%d",&Gcode);

		if(Gcode == ENT_TYPE)	break;	// �O���[�v�R�[�h:0�Ȃ�while break

		else{
			fgets(Buf,sizeof(Buf),fp);	// �����s

			if(Gcode == COORD_X)
				sscanf(Buf,"%lf",&body->CirA[Count[_CIRCLE_ARC]].cp[0].x);		// �~�̒��S���WX
			else if(Gcode == COORD_Y)
				sscanf(Buf,"%lf",&body->CirA[Count[_CIRCLE_ARC]].cp[0].y);		// �~�̒��S���WY
			else if(Gcode == COORD_Z)
				sscanf(Buf,"%lf",&body->CirA[Count[_CIRCLE_ARC]].cp[0].z);		// �~�̒��S���WZ
			else if(Gcode == RADIUS)
				sscanf(Buf,"%lf",&body->CirA[Count[_CIRCLE_ARC]].R);			// �~�̔��a
		}
	}

	// debug
	//fprintf(stderr,"%d:%lf,%lf,%lf,%lf\n",Count[_CIRCLE_ARC],
	//	body->CirA[Count[_CIRCLE_ARC]].cp[0].x,
	//	body->CirA[Count[_CIRCLE_ARC]].cp[0].y,
	//	body->CirA[Count[_CIRCLE_ARC]].cp[0].z,
	//	body->CirA[Count[_CIRCLE_ARC]].R);

	body->CirA[Count[_CIRCLE_ARC]].t[0] = 0;		// �~�̊J�n�p�x
	body->CirA[Count[_CIRCLE_ARC]].t[1] = 360;		// �~�̏I���p�x
	SetStartEndPtArc(&body->CirA[Count[_CIRCLE_ARC]]);			// �~�ʂ̎n�_�C�I�_���Z�b�g
	CalcUVvec(&body->CirA[Count[_CIRCLE_ARC]]);					// CIRA��UV�������W��ݒ肷��

	InitDisplayStat(&body->CirA[Count[_CIRCLE_ARC]].Dstat);
	body->CirA[Count[_CIRCLE_ARC]].EntUseFlag = GEOMTRYELEM;
	body->CirA[Count[_CIRCLE_ARC]].pD = NULL;

	Count[_CIRCLE_ARC]++;

	return KOD_TRUE;
}

// �����̃f�[�^���擾��BODY�Ɋi�[
int DXF_PARSER::GetLineData(FILE *fp,BODY *body)
{
	while(1){
		fgets(Buf,sizeof(Buf),fp);		// ��s
		sscanf(Buf,"%d",&Gcode);

		if(Gcode == ENT_TYPE)	break;	// �O���[�v�R�[�h:0�Ȃ�while break

		else{
			fgets(Buf,sizeof(Buf),fp);	// �����s

			if(Gcode == COORD_X)
				sscanf(Buf,"%lf",&body->Line[Count[_LINE]].cp[0].x);		// �����̎n�_X
			else if(Gcode == COORD_Y)
				sscanf(Buf,"%lf",&body->Line[Count[_LINE]].cp[0].y);		// �����̎n�_Y
			else if(Gcode == COORD_Z)
				sscanf(Buf,"%lf",&body->Line[Count[_LINE]].cp[0].z);		// �����̎n�_Z
			else if(Gcode == COORD_X_)
				sscanf(Buf,"%lf",&body->Line[Count[_LINE]].cp[1].x);		// �����̏I�_X
			else if(Gcode == COORD_Y_)
				sscanf(Buf,"%lf",&body->Line[Count[_LINE]].cp[1].y);		// �����̏I�_Y
			else if(Gcode == COORD_Z_)
				sscanf(Buf,"%lf",&body->Line[Count[_LINE]].cp[1].z);		// �����̏I�_Z
		}
	}

	// debug
	//fprintf(stderr,"%d:%lf,%lf,%lf,%lf,%lf,%lf\n",Count[_LINE],
	//	body->Line[Count[_LINE]].cp[0].x,
	//	body->Line[Count[_LINE]].cp[0].y,
	//	body->Line[Count[_LINE]].cp[0].z,
	//	body->Line[Count[_LINE]].cp[1].x,
	//	body->Line[Count[_LINE]].cp[1].y,
	//	body->Line[Count[_LINE]].cp[1].z);

	InitDisplayStat(&body->Line[Count[_LINE]].Dstat);
	body->Line[Count[_LINE]].EntUseFlag = GEOMTRYELEM;
	body->Line[Count[_LINE]].pD = NULL;

	Count[_LINE]++;

	return KOD_TRUE;
}

// �e�G���e�B�e�B�̐������T�[�`
int DXF_PARSER::ResearchEntNum(FILE *fp,BODY *body)
{
	do{
		fgets(Buf,sizeof(Buf),fp);	// ��s
		sscanf(Buf,"%d",&Gcode);
		fgets(Buf,sizeof(Buf),fp);	// �����s
		sscanf(Buf,"%s",&Label);

		if(Gcode == ENT_TYPE){
			if(strcmp(Label,"ARC") == 0)			body->TypeNum[_CIRCLE_ARC]++;
			else if(strcmp(Label,"CIRCLE") == 0)	body->TypeNum[_CIRCLE_ARC]++;
			else if(strcmp(Label,"LINE") == 0)		body->TypeNum[_LINE]++;
		}
	}while(strcmp(Label,"ENDSEC"));

	body->TypeNum[_NURBSC] = body->TypeNum[_CIRCLE_ARC] + body->TypeNum[_LINE];

	fprintf(stderr,"%d,%d\n",body->TypeNum[_CIRCLE_ARC],body->TypeNum[_LINE]);	// for debug

	return KOD_TRUE;
}

// �w�b�_�Z�N�V�����̓ǂݍ���
int DXF_PARSER::ReadHeaderSection(FILE *fp,int Line,BODY *body)
{
	return KOD_TRUE;
}

// �N���X�Z�N�V�����̓ǂݍ���
int DXF_PARSER::ReadClassesSection(FILE *fp,int Line,BODY *body)
{
	return KOD_TRUE;
}

// �e�[�u���Z�N�V�����̓ǂݍ���
int DXF_PARSER::ReadTablesSection(FILE *fp,int Line,BODY *body)
{
	return KOD_TRUE;
}

// �u���b�N�Z�N�V�����̓ǂݍ���
int DXF_PARSER::ReadBlocksSection(FILE *fp,int Line,BODY *body)
{
	return KOD_TRUE;
}

// �I�u�W�F�N�g�Z�N�V�����̓ǂݍ���
int DXF_PARSER::ReadObjectsSection(FILE *fp,int Line,BODY *body)
{
	return KOD_TRUE;
}

// ���������
int DXF_PARSER::EvenOdd(int val)
{
	if(val%2 == 0)
		return EVEN;
	else
		return ODD;
}

// �Z�N�V��������
int DXF_PARSER::CheckSection(char *str)
{
	char section[LABELSIZEMAX];
	sscanf(str,"%s",section);

	if(strcmp(section,"HEADER") == 0)
		return HEADER_SECTION;
	else if(strcmp(section,"CLASSES") == 0)
		return CLASS_SECTION;
	else if(strcmp(section,"TABLES") == 0)
		return TABLE_SECTION;
	else if(strcmp(section,"BLOCKS") == 0)
		return BLOCK_SECTION;
	else if(strcmp(section,"ENTITIES") == 0)
		return ENTITIY_SECTION;
	else if(strcmp(section,"OBJECTS") == 0)
		return OBJECT_SECTION;
	else
		return KOD_ERR;
}

// �e�G���e�B�e�B�̕\��������ݒ�
void DXF_PARSER::InitDisplayStat(DispStat *Dstat)
{
	// ���F��ݒ�
	Dstat->Color[0] = 1.0;
	Dstat->Color[1] = 1.0;
	Dstat->Color[2] = 1.0;
	Dstat->Color[3] = 0.5;

	// �\��������ǉ�����ꍇ�͈ȉ��ɒǉ��̃R�[�h���L�q
}