/*************************
* IGES�t�@�C����ǂݍ��� *
**************************/

#include "IGES_Parser.h"

// *body --- ���̂��\������G���e�B�e�B�̏W���I�u�W�F�N�g�ւ̃|�C���^
// TypeNum[] --- �e�G���e�B�e�B�̐����i�[�����
int IGES_PARSER::IGES_Parser_Main(BODY *body)
{
	FILE *fp;
	GlobalParam gpara;				// �O���[�o�����̃p�����[�^���i�[
	DirectoryParam *dpara;			// �f�B���N�g�����̃p�����[�^���i�[
	char IGES_fname[COLUMN_MAX];	// �ǂݍ���IGES�t�@�C����
	int  line[SECTION_NUM];			// �e�Z�N�V�������̃��C�������i�[
	int  flag = 0;
	int  i;


	// IGES�t�@�C�����̓ǂݍ���
	fprintf(stderr,"Input IGES File Name:");	
	fgets(buf,COLUMN_MAX,stdin);
	sscanf(buf,"%s",IGES_fname);

	//strcpy(IGES_fname,"Surf2.IGS");

	// IGES�t�@�C���I�[�v��
	if((fp = fopen(IGES_fname,"r")) == NULL){
		fprintf(stderr,"ERROR: OPEN %s\n",IGES_fname);
		return(ERR);
	}

	// �e�Z�N�V�����̍s�������炩���ߎ擾
	GetSectionLine(fp,line);

	// DirectoryParam�̃������[�m��
	line[SECTION_DIRECTORY] /= 2;		// �f�B���N�g�����́A2�s��1�̃V�[�P���X���\������̂�2�Ŋ��������̂��f�B���N�g�����̃��C�����Ƃ���
	dpara = (DirectoryParam *)malloc(sizeof(DirectoryParam)*line[SECTION_DIRECTORY]);
	if(dpara == NULL){
		fprintf(stderr,"ERROR: DirectoryParam�̃������[�m�ۂɎ��s���܂���\n");
		return(ERR);
	}

	// IGES�t�@�C���ǂݍ���(�e�Z�N�V�������ɏ���)
	for(i=0;i<SECTION_NUM;i++){
		if(i == SECTION_START){					// �X�^�[�g���ǂݍ���
			flag = GetStartSection(fp,line[i]);
		}
		else if(i == SECTION_GLOBAL){			// �O���[�o�����ǂݍ���
			flag = GetGlobalSection(fp,&gpara,line[i]);
		}
		else if(i == SECTION_DIRECTORY){		// �f�B���N�g�����ǂݍ���
			flag = GetDirectorySection(fp,dpara,body->TypeNum,line[i]);
		}
		else if(i == SECTION_PARAMETER){		// �p�����[�^���ǂݍ���
			New_body(body,body->TypeNum);		// BODY�\���̓��̊e�G���e�B�e�B�̃������[�m��
			flag = GetParameterSection(fp,dpara,*body,line[SECTION_DIRECTORY]);
		}
		else if(i == SECTION_TERMINATE){		// �^�[�~�l�[�g���ǂݍ���
			flag = GetTerminateSection(fp);
		}
		if(flag == ERR){
			Free_body(body,TypeCount);			// ���܂Ŋm�ۂ������̃������[�����
			return(ERR);
		}
	}

	ChangeEntityforNurbs(dpara,*body,line[SECTION_DIRECTORY]);	// �����\����S��NURBS�ɕύX����

	flag = SearchMaxCoord(body,body->TypeNum);		// ���̂̍ő���W�l��T��(�����\���ł̕\���{�������肷�邽��)

	fclose(fp);

	free(dpara);

	return flag;
}

// NURBS�Ȑ��ȊO�̃G���e�B�e�B��NURBS�Ȑ��ɕϊ����A�ϊ��s�񂪂���΍��W�ϊ����{��
int IGES_PARSER::ChangeEntityforNurbs(DirectoryParam *dpara,BODY body,int dline)
{
	bool flag;

	for(int i=0;i<dline;i++){
		flag = false;
		// �~/�~��->NURBS�Ȑ�
		if(dpara[i].entity_type == CIRCLE_ARC){
			body.NurbsC[TypeCount[_NURBSC]].EntUseFlag = body.CirA[dpara[i].entity_count].EntUseFlag;	// �f�B���N�g�����̏��"Entity Use Flag"�𓾂�(NURBSC)
			if(GetNurbsCFromCirA(TypeCount[_NURBSC],dpara[i].entity_count,body) == ERR) return ERR;		// �~/�~�ʃp�����[�^����NURBS�Ȑ��p�����[�^�𓾂�
			flag = true;
		}
		// ����->NURBS�Ȑ�
		else if(dpara[i].entity_type == LINE){
			body.NurbsC[TypeCount[_NURBSC]].EntUseFlag = body.Line[dpara[i].entity_count].EntUseFlag;	// �f�B���N�g�����̏��"Entity Use Flag"�𓾂�(NURBSC)
			if(GetNurbsCFromLine(TypeCount[_NURBSC],dpara[i].entity_count,body) == ERR) return ERR;		// �����p�����[�^����NURBS�Ȑ��p�����[�^�𓾂�
			flag = true;
		}
		// �~/�~�ʁA�����ȊO�̋Ȑ��G���e�B�e�B�����݂���ꍇ�́A�V�K�ɏ�����ǉ����Ă�������

		if(flag == true){													// NURBS�ϊ����ꂽ�G���e�B�e�B�ɑ΂���
			if(dpara[i].p_tm){												// �ϊ��s�񂪑��݂���ꍇ
				for(int j=0;j<TypeCount[_TRANSFORMATION_MATRIX];j++){		// �S�Ă̕ϊ��s��^�C�v�𒲂ׂ�
					if(body.TMat[j].pD == dpara[i].p_tm){					// �ΏۂƂȂ�ϊ��s��^�C�v�ւ̃|�C���^
						if(TransformNurbsC(TypeCount[_NURBSC],j,body) == ERR) return ERR;	// NURBS�Ȑ������W�ϊ�����
					}
				}
			}
			TypeCount[_NURBSC]++;											// NURBSC�̐����C���N�������g
		}
	}

	return TRUE;
}

// �p�����[�^���ǂݍ���
int IGES_PARSER::GetParameterSection(FILE *fp,DirectoryParam *dpara,BODY body,int dline)
{
	int i,j;
	char str[COLUMN_MAX*2000];	// ������o�b�t�@(2000�s���m��)
	char *p;					// ������擪���ʗp�|�C���^
	int  pD;					// �f�B���N�g�����ւ̋t�|�C���^�̒l

	// TypeCount�̏�����
	for(i=0;i<ALL_ENTITY_TYPE_NUM;i++)
		TypeCount[i] = 0;

	// �S�G���e�B�e�B�̃p�����[�^�����ꂼ��̃G���e�B�e�B�\���̂Ɋi�[���Ă���
	for(i=0;i<dline;i++){
		// �f�B���N�g����14�t�B�[���h�̏������ɁAstr�Ɋe�p�����[�^���̃��C���������q�����킹��
		strcpy(str,"");
		for(j=0;j<dpara[i].param_line_count;j++){
			fgets(buf,COLUMN_MAX_,fp);
			if((p = strchr(buf,';')) == NULL){
				p = strchr(buf,' ');
			}
			else{
				buf[p-buf] = '\0';		// ���R�[�h�f���~�^���o���́A���R�[�h�f���~�^�����I�[�����ɂ���
			}
			strncat(str,buf,p-buf+1);	// ��������e�p�����[�^���̃��C�������q�����킹�Ă���
		}
		p = &buf[COL_P_DIRECTORY];		// �f�B���N�g�����ւ̋t�|�C���^�̒l�����炩���ߒ��ׂĂ����i�֋X��j
		sscanf(p,"%d",&pD);

		// str�𕪉����e�G���e�B�e�B�\���̂ɏ��𖄂߂Ă���
		// ���̃G���e�B�e�B��ǉ�����ꍇ�͈ȉ��ɃR�[�h��ǉ�����

		// �~�E�~��(NURBS�Ȑ��Ƃ��ẴG���e�B�e�B���������ɓ���)
		if(dpara[i].entity_type == CIRCLE_ARC){							
			if(GetCirAPara(str,pD,dpara,body) == ERR)  return ERR;					// �~/�~�ʃp�����[�^�̎擾
			body.CirA[TypeCount[_CIRCLE_ARC]].EntUseFlag = dpara[i].useflag_stat;	// �f�B���N�g�����̏��"Entity Use Flag"�𓾂�
			dpara[i].entity_count = TypeCount[_CIRCLE_ARC];							// dpara��body���֘A�t����
			TypeCount[_CIRCLE_ARC]++;					// �~�E�~�ʃ^�C�v�̐����C���N�������g
		}
		// �����Ȑ�
		else if(dpara[i].entity_type == COMPOSITE_CURVE){					
			if(GetCompCPara(str,pD,dpara,dline,body) == ERR)  return ERR;
			dpara[i].entity_count = TypeCount[_COMPOSITE_CURVE];					// dpara��body���֘A�t����
			TypeCount[_COMPOSITE_CURVE]++;				// �����Ȑ��^�C�v�̐����C���N�������g
		}
		// �~���Ȑ�
		else if(dpara[i].entity_type == CONIC_ARC){											
			if(GetConAPara(str,pD,dpara,body) == ERR)  return ERR;
			TypeCount[_CONIC_ARC]++;					// �~���Ȑ��^�C�v�̐����C���N�������g
		}
		// ����(NURBS�Ȑ��Ƃ��ẴG���e�B�e�B���������ɓ���)
		else if(dpara[i].entity_type == LINE){									
			if(GetLinePara(str,pD,dpara,body) == ERR)  return ERR;				// �����p�����[�^�̎擾
			body.Line[TypeCount[_LINE]].EntUseFlag = dpara[i].useflag_stat;		// �f�B���N�g�����̏��"Entity Use Flag"�𓾂�(LINE)
			dpara[i].entity_count = TypeCount[_LINE];							// dpara��body���֘A�t����
			TypeCount[_LINE]++;							// �����^�C�v�̐����C���N�������g
		}
		// �ϊ��s��
		else if(dpara[i].entity_type == TRANSFORMATION_MATRIX){			
			if(GetTMatPara(str,pD,dpara,body) == ERR)  return ERR;
			dpara[i].entity_count = TypeCount[_TRANSFORMATION_MATRIX];			// dpara��body���֘A�t����
			TypeCount[_TRANSFORMATION_MATRIX]++;		// �ϊ��s��^�C�v�̐����C���N�������g
		}
		// NRBS�Ȑ�
		else if(dpara[i].entity_type == NURBS_CURVE){		
			if(GetNurbsCPara(str,pD,dpara,body) == ERR)  return ERR;
			body.NurbsC[TypeCount[_NURBSC]].EntUseFlag = dpara[i].useflag_stat;	// �f�B���N�g�����̏��"Entity Use Flag"�𓾂�
			dpara[i].entity_count = TypeCount[_NURBSC];							// dpara��body���֘A�t����
			TypeCount[_NURBSC]++;		// NRBS�Ȑ��^�C�v�̐����C���N�������g
		}
		// NRBS�Ȗ�
		else if(dpara[i].entity_type == NURBS_SURFACE){		
			if(GetNurbsSPara(str,pD,dpara,body) == ERR)  return ERR;
			dpara[i].entity_count = TypeCount[_NURBSS];							// dpara��body���֘A�t����
			TypeCount[_NURBSS]++;		// NRBS�Ȗʃ^�C�v�̐����C���N�������g
		}
		// �ʏ��
		else if(dpara[i].entity_type == CURVE_ON_PARAMETRIC_SURFACE){	
			if(GeConpSPara(str,pD,dpara,dline,body) == ERR)  return ERR;
			dpara[i].entity_count = TypeCount[_CURVE_ON_PARAMETRIC_SURFACE];	// dpara��body���֘A�t����
			TypeCount[_CURVE_ON_PARAMETRIC_SURFACE]++;	// �ʏ���^�C�v�̐����C���N�������g
		}
		// �g������	
		else if(dpara[i].entity_type == TRIMMED_SURFACE){				
			if(GetTrmSPara(str,pD,dpara,body) == ERR)  return ERR;
			dpara[i].entity_count = TypeCount[_TRIMMED_SURFACE];				// dpara��body���֘A�t����
			TypeCount[_TRIMMED_SURFACE]++;				// �g�����ʃ^�C�v�̐����C���N�������g
		}
		// �T�|�[�g���Ă��Ȃ�Entity Type�̏ꍇ
		else{
			fprintf(stderr,"Entity Type #%d:�T�|�[�g���Ă��܂���\n",dpara[i].entity_type);
			continue;
		}
	}

	return TRUE;
}

// Type100 �~�E�~�ʂ̓ǂݍ���
int IGES_PARSER::GetCirAPara(char str[],int pD,DirectoryParam *dpara,BODY body)
{
//	int i;
	char *p;
	double x[3],y[3];

	p = str;

	body.CirA[TypeCount[_CIRCLE_ARC]].zt = CatchStringD(&p);		// Z�������̐[��
	x[0] = CatchStringD(&p);							// ���S���WX
	body.CirA[TypeCount[_CIRCLE_ARC]].cp[0].x = x[0];
	y[0] = CatchStringD(&p);							// ���S���WY
	body.CirA[TypeCount[_CIRCLE_ARC]].cp[0].y = y[0];
	x[1] = CatchStringD(&p);							// �n�_X
	body.CirA[TypeCount[_CIRCLE_ARC]].cp[1].x = x[1];
	y[1] = CatchStringD(&p);							// �n�_Y
	body.CirA[TypeCount[_CIRCLE_ARC]].cp[1].y = y[1];
	x[2] = CatchStringD(&p);							// �I�_X
	body.CirA[TypeCount[_CIRCLE_ARC]].cp[2].x = x[2];
	y[2] = CatchStringD(&p);							// �I�_Y
	body.CirA[TypeCount[_CIRCLE_ARC]].cp[2].y = y[2];

	body.CirA[TypeCount[_CIRCLE_ARC]].R = sqrt((x[1]-x[0])*(x[1]-x[0])+(y[1]-y[0])*(y[1]-y[0]));	// ���a�Z�o

	body.CirA[TypeCount[_CIRCLE_ARC]].pD = pD;		// �f�B���N�g�����ւ̋t�|�C���^�̒l

	InitDisplayStat(&body.CirA[TypeCount[_CIRCLE_ARC]].Dstat);	// �\�������̏�����

	return TRUE;
}

// Type104 �~���Ȑ��̓ǂݍ���
int IGES_PARSER::GetConAPara(char str[],int pD,DirectoryParam *dpara,BODY body)
{
	fprintf(stderr,"Type104:������\n");
	return TRUE;
}

// Type110 �����̓ǂݍ���
int IGES_PARSER::GetLinePara(char str[],int pD,DirectoryParam *dpara,BODY body)
{
	char *p;

	p = str;

	body.Line[TypeCount[_LINE]].cp[0].x = CatchStringD(&p);		// �n�_��X���W
	body.Line[TypeCount[_LINE]].cp[0].y = CatchStringD(&p);		// �n�_��Y���W
	body.Line[TypeCount[_LINE]].cp[0].z = CatchStringD(&p);		// �n�_��Z���W
	body.Line[TypeCount[_LINE]].cp[1].x = CatchStringD(&p);		// �I�_��X���W
	body.Line[TypeCount[_LINE]].cp[1].y = CatchStringD(&p);		// �I�_��Y���W
	body.Line[TypeCount[_LINE]].cp[1].z = CatchStringD(&p);		// �I�_��Z���W

	body.Line[TypeCount[_LINE]].pD = pD;		// �f�B���N�g�����ւ̋t�|�C���^�̒l

	InitDisplayStat(&body.Line[TypeCount[_LINE]].Dstat);	// �\�������̏�����

	return TRUE;
}

// Type124 �ϊ��s��̓ǂݍ���
int IGES_PARSER::GetTMatPara(char str[],int pD,DirectoryParam *dpara,BODY body)
{
	int i,j;
	char *p;
	
	p = str;
	for(i=0;i<3;i++){
		for(j=0;j<4;j++){
			if(j != 3){
				body.TMat[TypeCount[_TRANSFORMATION_MATRIX]].R[i][j] = CatchStringD(&p);	// 3�~3��]�s�񐬕�
			}
			else{
				body.TMat[TypeCount[_TRANSFORMATION_MATRIX]].T[i] = CatchStringD(&p);		// ���i�x�N�g������
			}
		}
	}
	
	body.TMat[TypeCount[_TRANSFORMATION_MATRIX]].pD = pD;		// DE���ւ̋t�|�C���^�̒l
	
	return TRUE;
}

// Type126 NRBS�Ȑ��̓ǂݍ���
int IGES_PARSER::GetNurbsCPara(char str[],int pD,DirectoryParam *dpara,BODY body)
{
	char *p;
	int i=0;
	int errflag=0;

	p = str;
	body.NurbsC[TypeCount[_NURBSC]].K = CatchStringI(&p) + 1;		// ���a�L���̏㑤�Y���i�R���g���[���|�C���g-1�j�̒l
	body.NurbsC[TypeCount[_NURBSC]].M = CatchStringI(&p) + 1;		// ���֐��̊K��
	body.NurbsC[TypeCount[_NURBSC]].N = body.NurbsC[TypeCount[_NURBSC]].K + body.NurbsC[TypeCount[_NURBSC]].M;	// �m�b�g�x�N�g���̐�
	for(i=0;i<4;i++){	// �u�[���A���^�v���p�e�B4��
		body.NurbsC[TypeCount[_NURBSC]].prop[i] = CatchStringI(&p);
	}

	// �������[�m��
	if(New_NurbsC(&body.NurbsC[TypeCount[_NURBSC]],body.NurbsC[TypeCount[_NURBSC]].K,body.NurbsC[TypeCount[_NURBSC]].N) == ERR){
		fprintf(stderr,"PARAMETER SECTION ERROR:�������[�̊m�ۂɎ��s���܂���\n");
		return ERR;
	}

	for(i=0;i<body.NurbsC[TypeCount[_NURBSC]].N;i++){
		body.NurbsC[TypeCount[_NURBSC]].T[i] = CatchStringD(&p);	// �m�b�g�x�N�g���̒l
	}
	for(i=0;i<body.NurbsC[TypeCount[_NURBSC]].K;i++){				// Weight�̒l
		body.NurbsC[TypeCount[_NURBSC]].W[i] = CatchStringD(&p);
	}
	for(i=0;i<body.NurbsC[TypeCount[_NURBSC]].K;i++){				// �R���g���[���|�C���g�̍��W�l
		body.NurbsC[TypeCount[_NURBSC]].cp[i].x = CatchStringD(&p);
		body.NurbsC[TypeCount[_NURBSC]].cp[i].y = CatchStringD(&p);
		body.NurbsC[TypeCount[_NURBSC]].cp[i].z = CatchStringD(&p);
	}
	body.NurbsC[TypeCount[_NURBSC]].V[0] = CatchStringD(&p);		// �p�����[�^�̒l
	body.NurbsC[TypeCount[_NURBSC]].V[1] = CatchStringD(&p);

	// �@���x�N�g���͋L�q����Ă���ꍇ�Ƃ���Ă��Ȃ��ꍇ������悤�Ȃ̂ŁA�L�q����Ă���ꍇ�̂ݓǂݍ���
	if(strchr(p,',') != NULL){
		body.NurbsC[TypeCount[_NURBSC]].norm.x = CatchStringD(&p);	// �@���x�N�g��
		body.NurbsC[TypeCount[_NURBSC]].norm.y = CatchStringD(&p);
		body.NurbsC[TypeCount[_NURBSC]].norm.z = CatchStringD(&p);
	}

	body.NurbsC[TypeCount[_NURBSC]].pD = pD;		// DE���ւ̋t�|�C���^�̒l

	InitDisplayStat(&body.NurbsC[TypeCount[_NURBSC]].Dstat);	// �\�������̏�����

	return TRUE;
}

// Type128 NRBS�Ȗʂ̓ǂݍ���
int IGES_PARSER::GetNurbsSPara(char str[],int pD,DirectoryParam *dpara,BODY body)
{
	char *p;
	int i=0,j=0;
	int errflag=0;

	p = str;

	body.NurbsS[TypeCount[_NURBSS]].K[0] = CatchStringI(&p) + 1;	// u�����R���g���[���|�C���g�̐�
	body.NurbsS[TypeCount[_NURBSS]].K[1] = CatchStringI(&p) + 1;	// v�����R���g���[���|�C���g�̐�
	body.NurbsS[TypeCount[_NURBSS]].M[0] = CatchStringI(&p) + 1;	// ���֐���u�����K��
	body.NurbsS[TypeCount[_NURBSS]].M[1] = CatchStringI(&p) + 1;	// ���֐���v�����K��
	body.NurbsS[TypeCount[_NURBSS]].N[0] = body.NurbsS[TypeCount[_NURBSS]].K[0] + body.NurbsS[TypeCount[_NURBSS]].M[0];	// u�����m�b�g�x�N�g���̐�
	body.NurbsS[TypeCount[_NURBSS]].N[1] = body.NurbsS[TypeCount[_NURBSS]].K[1] + body.NurbsS[TypeCount[_NURBSS]].M[1];	// v�����m�b�g�x�N�g���̐�
	for(i=0;i<5;i++){
		body.NurbsS[TypeCount[_NURBSS]].prop[i] = CatchStringI(&p);	// �u�[���A���^�v���p�e�B5��
	}

	// �������[�m��
	if(New_NurbsS(&body.NurbsS[TypeCount[_NURBSS]],body.NurbsS[TypeCount[_NURBSS]].K,body.NurbsS[TypeCount[_NURBSS]].N) == ERR){
		fprintf(stderr,"PARAMETER SECTION ERROR:�������[�̊m�ۂɎ��s���܂���\n");
		return ERR;
	}
	
	for(i=0;i<body.NurbsS[TypeCount[_NURBSS]].N[0];i++){
		body.NurbsS[TypeCount[_NURBSS]].S[i] = CatchStringD(&p);	// u�����m�b�g�x�N�g��
	}
	for(i=0;i<body.NurbsS[TypeCount[_NURBSS]].N[1];i++){
		body.NurbsS[TypeCount[_NURBSS]].T[i] = CatchStringD(&p);	// v�����m�b�g�x�N�g��
	}
	for(i=0;i<body.NurbsS[TypeCount[_NURBSS]].K[1];i++){
		for(j=0;j<body.NurbsS[TypeCount[_NURBSS]].K[0];j++){
			body.NurbsS[TypeCount[_NURBSS]].W[j][i] = CatchStringD(&p);	//  u����Weight
		}
	}
	for(i=0;i<body.NurbsS[TypeCount[_NURBSS]].K[1];i++){
		for(j=0;j<body.NurbsS[TypeCount[_NURBSS]].K[0];j++){
			body.NurbsS[TypeCount[_NURBSS]].cp[j][i].x = CatchStringD(&p);	// �R���g���[���|�C���gX
			body.NurbsS[TypeCount[_NURBSS]].cp[j][i].y = CatchStringD(&p);	// �R���g���[���|�C���gY
			body.NurbsS[TypeCount[_NURBSS]].cp[j][i].z = CatchStringD(&p);	// �R���g���[���|�C���gZ
		}
	}
	body.NurbsS[TypeCount[_NURBSS]].U[0] = CatchStringD(&p);			// u�����̊J�n�l
	body.NurbsS[TypeCount[_NURBSS]].U[1] = CatchStringD(&p);			// u�����̏I���l
	body.NurbsS[TypeCount[_NURBSS]].V[0] = CatchStringD(&p);			// v�����̊J�n�l
	body.NurbsS[TypeCount[_NURBSS]].V[1] = CatchStringD(&p);			// v�����̏I���l

	body.NurbsS[TypeCount[_NURBSS]].pD = pD;		// DE���ւ̋t�|�C���^�̒l

	ChangeStatColor(body.NurbsS[TypeCount[_NURBSS]].Dstat.Color,0.2,0.2,0.2,1);	// �Ȗʂ̐F��ݒ�

	return TRUE;
}

// Type102 �����Ȑ��̓ǂݍ���
int IGES_PARSER::GetCompCPara(char str[],int pD,DirectoryParam *dpara,int dline,BODY body)
{
	char *p;
	int  pdnum;		// DE���̃V�[�P���X�i���o�[�擾�p
	int  i;

	p = str;

	body.CompC[TypeCount[_COMPOSITE_CURVE]].N = CatchStringI(&p);	// �����Ȑ��̍\���v�f��

	// �����Ȑ��̃������[���m��
	if(New_CompC(&body.CompC[TypeCount[_COMPOSITE_CURVE]],body.CompC[TypeCount[_COMPOSITE_CURVE]].N) == ERR){
		fprintf(stderr,"PARAMETER SECTION ERROR:�������[�̊m�ۂɎ��s���܂���\n");
		return ERR;
	}

	for(i=0;i<body.CompC[TypeCount[_COMPOSITE_CURVE]].N;i++){		// �\���v�f��DE���ւ̃|�C���^�l
		pdnum = CatchStringI(&p);		// �e�\���v�f��DE���̃V�[�P���X�i���o�[�𓾂�
		body.CompC[TypeCount[_COMPOSITE_CURVE]].DEType[i] = SearchEntType(dpara,pdnum,dline);		// pdnum�������G���e�B�e�B�^�C�v�𔻕�
		body.CompC[TypeCount[_COMPOSITE_CURVE]].pDE[i] = (COMPELEM *)GetDEPointer(pdnum,body);		// pdnum�������\���̂̃|�C���^�𓾂�
	}

	body.CompC[TypeCount[_COMPOSITE_CURVE]].pD = pD;		// DE���ւ̋t�|�C���^�̒l

	return TRUE;
}

// Type142 �ʏ���̓ǂݍ���
int IGES_PARSER::GeConpSPara(char str[],int pD,DirectoryParam *dpara,int dline,BODY body)
{
	char *p;
	int pdnum;		// DE���̃V�[�P���X�i���o�[�擾�p

	p = str;

	body.ConpS[TypeCount[_CURVE_ON_PARAMETRIC_SURFACE]].crtn = CatchStringI(&p);	// �ʏ�����ǂ̂悤�ɍ��ꂽ����\��

	pdnum = CatchStringI(&p);			// Curve�����Surface��DE���̃V�[�P���X�i���o�[�𓾂�
	body.ConpS[TypeCount[_CURVE_ON_PARAMETRIC_SURFACE]].SType = SearchEntType(dpara,pdnum,dline);	// pdnum�������G���e�B�e�B�^�C�v�𔻕�
	body.ConpS[TypeCount[_CURVE_ON_PARAMETRIC_SURFACE]].pS = (NURBSS *)GetDEPointer(pdnum,body);		// pdnum�������\���̂̃|�C���^�𓾂�

	pdnum = CatchStringI(&p);			// Surface�̃p�����[�^��Ԃɂ�����curve���`����Entity��DE���̃V�[�P���X�i���o�[�𓾂�
	body.ConpS[TypeCount[_CURVE_ON_PARAMETRIC_SURFACE]].BType = SearchEntType(dpara,pdnum,dline);	// pdnum�������G���e�B�e�B�^�C�v�𔻕�
	body.ConpS[TypeCount[_CURVE_ON_PARAMETRIC_SURFACE]].pB = (CURVE *)GetDEPointer(pdnum,body);		// pdnum�������\���̂̃|�C���^�𓾂�(���p��)

	pdnum = CatchStringI(&p);			// Curve C��DE���ւ̃|�C���^
	body.ConpS[TypeCount[_CURVE_ON_PARAMETRIC_SURFACE]].CType = SearchEntType(dpara,pdnum,dline);	// pdnum�������G���e�B�e�B�^�C�v�𔻕�
	body.ConpS[TypeCount[_CURVE_ON_PARAMETRIC_SURFACE]].pC = (CURVE *)GetDEPointer(pdnum,body);		// pdnum�������\���̂̃|�C���^�𓾂�(���p��)

	body.ConpS[TypeCount[_CURVE_ON_PARAMETRIC_SURFACE]].pref = CatchStringI(&p);	// ���葤�V�X�e���ō̂��Ă����\����\���t���O

	body.ConpS[TypeCount[_CURVE_ON_PARAMETRIC_SURFACE]].pD = pD;	// DE���̃V�[�P���X�i���o�[�𓾂�

	return TRUE;
}

// Type144 �g�����ʂ̓ǂݍ���
int IGES_PARSER::GetTrmSPara(char str[],int pD,DirectoryParam *dpara,BODY body)
{
	char *p;
	int  i;
	int  pdnum;		// DE���̃V�[�P���X�i���o�[�擾�p

	p = str;
	
	pdnum = CatchStringI(&p);		// �g���������Surface Entity��DE���̒l���擾
	body.TrmS[TypeCount[_TRIMMED_SURFACE]].pts = (NURBSS *)GetDEPointer(pdnum,body);		// �g���������Surface Entity�ւ̃|�C���^���擾
	
	body.TrmS[TypeCount[_TRIMMED_SURFACE]].n1 = CatchStringI(&p);		// �O�F�O����D�̋��E�ƈ�v���Ă���@�P�F����ȊO
	body.TrmS[TypeCount[_TRIMMED_SURFACE]].n2 = CatchStringI(&p);		// Trimmed Surface�̓����̒P���Ȑ��̐�

	pdnum = CatchStringI(&p);		// Trimmed Surface�̊O���̒P���Ȑ��̐�
	body.TrmS[TypeCount[_TRIMMED_SURFACE]].pTO = (CONPS *)GetDEPointer(pdnum,body); // �P���Ȑ��\���̂ւ̃|�C���^���擾

	// �P���Ȑ�N2�̐������������[�m��
	if((New_TrmS(&body.TrmS[TypeCount[_TRIMMED_SURFACE]],body.TrmS[TypeCount[_TRIMMED_SURFACE]].n2)) == ERR){
		fprintf(stderr,"PARAMETER SECTION ERROR:�������[�̊m�ۂɎ��s���܂���\n");
		return ERR;
	}

	for(i=0;i<body.TrmS[TypeCount[_TRIMMED_SURFACE]].n2;i++){
		pdnum = CatchStringI(&p);	// Trimmed Surface�̓����̒P���Ȑ���DE���̒l���擾
		body.TrmS[TypeCount[_TRIMMED_SURFACE]].pTI[i] = (CONPS *)GetDEPointer(pdnum,body);	// �P���Ȑ��\���̂ւ̃|�C���^���擾
	}

	body.TrmS[TypeCount[_TRIMMED_SURFACE]].pD = pD;		// DE���̃V�[�P���X�i���o�[�𓾂�

	return TRUE;
}


// �f�B���N�g�����ǂݍ���
int IGES_PARSER::GetDirectorySection(FILE *fp,DirectoryParam *dpara,int TypeNum[],int dline)
{
	int i,j;
	char *p;						// ������擪���ʗp�|�C���^
	char str[COLUMN_MAX*2+1];		// 2�s���i1�G���e�B�e�B���j�̕�����z��
	char field[FIELD_NUM+1];		// 8������1�t�B�[���h
	char dmy;

	for(i=0;i<ALL_ENTITY_TYPE_NUM;i++){
		TypeNum[i] = 0;			// ������
	}

	for(i=0;i<dline;i++){
		strcpy(str,"");				// str������

		if(fgets(buf,COLUMN_MAX_,fp) == NULL){		// i�Ԗڂ̃G���e�B�e�B��1�s�ڂ�ǂݍ���
			fprintf(stderr,"DIRECTORY SECTION ERROR: �t�@�C���ǂݍ��݃G���[\n");
			exit(ERR);
		}
		strncat(str,buf,COLUMN_MAX);
		if(fgets(buf,COLUMN_MAX_,fp) == NULL){		// i�Ԗڂ̃G���e�B�e�B��2�s�ڂ�ǂݍ���
			fprintf(stderr,"DIRECTORY SECTION ERROR: �t�@�C���ǂݍ��݃G���[\n");
			exit(ERR);
		}
		strncat(str,buf,COLUMN_MAX);				// �ǂݍ���2�s��str�ɑS�Ċi�[�����

		p = str;									// p���܂�str�̐擪�ɂ���
		for(j=0;j<DIRECTORYPARANUM;j++){			// �f�B���N�g�����̃p�����[�^�̐����������[�v
			strncpy(field,p,8);						// p�̐擪8������field�֊i�[
			field[FIELD_NUM] = '\0';				// �ꉞ�A�I�[������field�̂��K�ɂ��Ă���
			p += FIELD_NUM;							// p�����̃t�B�[���h�̐擪�ֈړ�
			// �f�B���N�g�����̏�񂪕K�v�ȏꍇ�͈ȉ��ɃR�[�h��ǉ�����
			if(j == ENTITY_TYPE_NUM){					// �v�f�ԍ����擾
				dpara[i].entity_type = atoi(field);
				GetType(dpara[i].entity_type,TypeNum);	// �G���e�B�e�B�^�C�v�̐������Z
			}
			else if(j == PARAM_DATA){					// �p�����[�^���ւ̃|�C���^���擾
				dpara[i].p_param = atoi(field);
			}
			else if(j == TRAN_MATRIX){					// �}�g���b�N�X�ւ̃|�C���^���擾
				dpara[i].p_tm = atoi(field);
			}
			else if(j == STATUS_NUM){					// �X�e�[�^�X���擾
				GetStatusNumber(field,&dpara[i]);
			}
			else if(j == SEQUENCE_NUM){					// �V�[�P���X�ԍ����擾
				sscanf(field,"%c %d",&dmy,&dpara[i].seq_num);
			}
			else if(j == PARAM_LINE_COUNT){				// �p�����[�^���̃��C����
				dpara[i].param_line_count = atoi(field);
			}
		}
	}
	
	return TRUE;
}

// �G���e�B�e�B�^�C�v�̐������Z����
void IGES_PARSER::GetType(int type,int entitynum[])
{
	int i;

	// �����܂��͉~�E�~�ʃG���e�B�e�B�̏ꍇ��NURBS�Ȑ��G���e�B�e�B�������ɃC���N�������g
	if(type == LINE || type == CIRCLE_ARC){
		entitynum[_NURBSC]++;
	}

	for(i=0;i<ALL_ENTITY_TYPE_NUM;i++){
		if(type == entity[i]){
			entitynum[i]++;		// i��enum�^EntityTyp�ɑΉ� ex) entitynum[10]��_NURBSC�i�L��B�X�v���C���Ȑ��j�̐���\��
		}
	}
}

// DE#9(�X�e�[�^�X)���̓ǂݍ���
void IGES_PARSER::GetStatusNumber(char field[],DirectoryParam *dpara)
{
	char str[3]="";
	char *p;

	p = field;
	strncpy(str,p,2);
	sscanf(str,"%d",&dpara->blank_stat);
	p += 2;
	strncpy(str,p,2);
	sscanf(str,"%d",&dpara->subordinate_stat);
	p += 2;
	strncpy(str,p,2);
	sscanf(str,"%d",&dpara->useflag_stat);
	p += 2;
}

// �O���[�o�����ǂݍ���
int IGES_PARSER::GetGlobalSection(FILE *fp,GlobalParam *gpara,int gline)
{
	char *str;					// �O���[�o����������S�Ă��i�[����
	char para_delim = ',';		// �p�����[�^�f���~�^
	char record_delim = ';';	// ���R�[�h�f���~�^
	int  para_length;			// �e�p�����[�^�̕�����̒������i�[
	char *p;
	int  i;

	// �O���[�o�����̃��C����*COL_CHAR���������[�m��
	str = (char *)malloc(sizeof(char) * gline*COL_CHAR);
	if(str == NULL){
		fprintf(stderr,"GLOBAL SECTION ERROR: �������[�̊m�ۂɎ��s���܂���\n");
		exit(ERR);
	}

	strcpy(str,"");			// str������
	for(i=0;i<gline;i++){
		if(fgets(buf,COLUMN_MAX_,fp) == NULL){
			fprintf(stderr,"GLOBAL SECTION ERROR: �t�@�C���ǂݍ��݃G���[\n");
			exit(ERR);
		}
		strncat(str,buf,COL_CHAR-1);	// str�ɃZ�N�V�������ʕ����܂ł̕�������������Ă���
	}

	// �p�����[�^�f���~�^�ƃ��R�[�h�f���~�^�̃`�F�b�N
	sscanf(str,"%dH%c",&para_length,&para_delim);		// �p�����[�^�f���~�^���擾
	if(para_delim != ','){
		fprintf(stderr,"GLOBAL SECTION ERROR: �p�����[�^�f���~�^��JAMA-IS�ɏ������Ă��܂���\n");
		exit(ERR);
	}
	else{
		sscanf(str,"%dH[^,],[^,],%dH%c",&para_length,&para_length,&record_delim);		// ���R�[�h�f���~�^���擾
		if(record_delim != ';'){
			fprintf(stderr,"GLOBAL SECTION ERROR: ���R�[�h�f���~�^��JAMA-IS�ɏ������Ă��܂���\n");
			exit(ERR);
		}
	}

	p = str;
	p += 4;		// p��str��5�����ځi�p�����[�^�f���~�^��`���̎��j�܂ňړ�
	p = strtok(p,",");
	for(i=3;i<=GLOBALPARAMNUM;i++){		// 2�̃f���~�^�𔲂������c��̃p�����[�^�𒀎��ǂݍ���
		if((p = strtok(NULL,",")) == NULL && i<GLOBALPARAMNUM){
			fprintf(stderr,"GLOBAL SECTION ERROR: �O���[�o�����̃p�����[�^��������܂���\n");
			exit(ERR);
		}

		// �O���[�o�����̏�񂪕K�v�ȏꍇ�͈ȉ��ɃR�[�h���L��
		if(i == MODEL_SCALE){				// ���f���X�P�[���ǂݍ���
			gpara->scale = atof(p);
		}
		else if(i == UNIT_FLAG){			// �P�ʃt���O�ǂݍ���
			gpara->unit_flag = atoi(p);
		}
		else if(i == MODEL_SPACE_SIZE){		// ���f����Ԃ̑傫���ǂݍ���
			gpara->space_size = atof(p);
		}
	}

	free(str);		// �������[�J��

	return TRUE;
}

// �X�^�[�g���ǂݍ���
int IGES_PARSER::GetStartSection(FILE *fp,int sline)
{
	int i;

	for(i=0;i<sline;i++){
		if(fgets(buf,COLUMN_MAX_,fp) == NULL){
			fprintf(stderr,"START SECTION ERROR: �t�@�C���ǂݍ��݃G���[\n");
			exit(ERR);
		}

		// �X�^�[�g���̏�񂪕K�v�ȏꍇ�͈ȉ��ɃR�[�h��ǉ�����
	}

	return TRUE;
}

// �^�[�~�l�[�g���ǂݍ���
int IGES_PARSER::GetTerminateSection(FILE *fp)
{
	return TRUE;
}

// �e�Z�N�V�����̃��C�����𒲂ׂ�
void IGES_PARSER::GetSectionLine(FILE *fp,int line[])
{
	line[0] = line[1] = line[2] = line[3] = line[4] = 0;	// ������

	while(fgets(buf,COLUMN_MAX_,fp)){
		if(buf[COL_CHAR-1] == 'S'){
			line[SECTION_START]++;
		}
		else if(buf[COL_CHAR-1] == 'G'){
			line[SECTION_GLOBAL]++;
		}
		else if(buf[COL_CHAR-1] == 'D'){
			line[SECTION_DIRECTORY]++;
		}
		else if(buf[COL_CHAR-1] == 'P'){
			line[SECTION_PARAMETER]++;
		}
		else if(buf[COL_CHAR-1] == 'T'){
			line[SECTION_TERMINATE]++;
		}
		else{							// �ǂ̃Z�N�V�����ɂ������Ȃ����������o
			fprintf(stderr,"ERROR: IGES FORMAT\n");
			exit(ERR);
		}
	}
	fseek(fp,0L,SEEK_SET);				// �t�@�C���擪�ɖ߂�

}

// �J���}�܂ł̐��l��ǂݍ���ŕԂ�(int)
int IGES_PARSER::CatchStringI(char **p)
{
	int a;

	if((*p = strchr(*p,',')) == NULL){
		fprintf(stderr,"ERROR: IGES�t�H�[�}�b�g�ɏ������Ă��܂���\n");
		exit(ERR);
	}

	(*p)++;
	sscanf(*p,"%d[^,],",&a);

	return a;
}

// �J���}�܂ł̐��l��ǂݍ���ŕԂ�(double)
double IGES_PARSER::CatchStringD(char **p)
{
	double a;

	if((*p = strchr(*p,',')) == NULL){
		fprintf(stderr,"ERROR: IGES�t�H�[�}�b�g�ɏ������Ă��܂���\n");
		exit(ERR);
	}

	(*p)++;
	sscanf(*p,"%lf[^,],",&a);

	return a;
}

// DE���ւ̃|�C���^���������ۂ̍\���̂ւ̃|�C���^��Ԃ�
void *IGES_PARSER::GetDEPointer(int TypeNum,BODY body)
{
	int i,j;

	for(i=0;i<ALL_ENTITY_TYPE_NUM;i++){
		for(j=0;j<TypeCount[i];j++){
			if(i==_CIRCLE_ARC && body.CirA[j].pD == TypeNum){
				return &body.CirA[j];
			}
			else if(i==_COMPOSITE_CURVE && body.CompC[j].pD == TypeNum){
				return &body.CompC[j];
			}
			else if(i==_CONIC_ARC && body.ConA[j].pD == TypeNum){
				return &body.ConA[j];
			}
			else if(i==_LINE && body.Line[j].pD == TypeNum){
				return &body.Line[j];
			}
			else if(i==_TRANSFORMATION_MATRIX && body.TMat[j].pD == TypeNum){
				return &body.TMat[j];
			}
			else if(i==_NURBSC && body.NurbsC[j].pD == TypeNum){
				return &body.NurbsC[j];
			}
			else if(i==_NURBSS && body.NurbsS[j].pD == TypeNum){
				return &body.NurbsS[j];
			}
			else if(i==_CURVE_ON_PARAMETRIC_SURFACE && body.ConpS[j].pD == TypeNum){
				return &body.ConpS[j];
			}
			else if(i==_TRIMMED_SURFACE && body.TrmS[j].pD == TypeNum){
				return &body.TrmS[j];
			}
		}
	}

	return NULL;
}

// DE���ւ̃|�C���^�̒l����G���e�B�e�B�̃^�C�v�𒲂ׂĕԂ�
int IGES_PARSER::SearchEntType(DirectoryParam *dpara,int pdnum,int dline)
{
	int i;

	for(i=0;i<dline;i++){
		if(dpara[i].seq_num == pdnum){
			return dpara[i].entity_type;
		}
	}

	return ERR;
}
// �S�ẴG���e�B�e�B�ɂ�������W�l�̍ő�l�𒲂ׂ�
int IGES_PARSER::SearchMaxCoord(BODY *body,int TypeNum[])
{
	int i,j;
	int temp=0;
	int bufnum=0;
	double *CoordBuf;
	
	// #100(�~�A�~��)�A#110(����)�A#126(NURBS�Ȑ�)�̃R���g���[���|�C���g�̍��W�l�̌��𓾂�
	for(i=0;i<TypeNum[_NURBSC];i++){
		bufnum += 3*body->NurbsC[i].K;
	}
	
	// �������m��
	if((CoordBuf = (double*)malloc(sizeof(double)*bufnum)) == NULL){
		fprintf(stderr,"SEARCH MAXCOORD ERROR:�������[�̊m�ۂɎ��s���܂���\n");
		return ERR;
	}

	// #100(�~�A�~��)�A#110(����)�A#126(NURBS�Ȑ�)�̃R���g���[���|�C���g�̍��W�l�𓾂�
	for(i=0;i<TypeNum[_NURBSC];i++){
		for(j=0;j<body->NurbsC[i].K;j++){
			CoordBuf[temp*3] = body->NurbsC[i].cp[j].x;	// �R���g���[���|�C���gX
			CoordBuf[temp*3+1] = body->NurbsC[i].cp[j].y;	// �R���g���[���|�C���gY
			CoordBuf[temp*3+2] = body->NurbsC[i].cp[j].z;	// �R���g���[���|�C���gZ
			temp++;
		}
	}
	
	qsort(CoordBuf,bufnum,sizeof(double),QCmp);	// �S�Ă̍��W�l���N�C�b�N�\�[�g�ɂ��~���Ƀ\�[�g
	body->MaxCoord = CoordBuf[0];				// �ł��傫�����W�l�𓾂�
	
	// ���������
	free(CoordBuf);
	
	return TRUE;
}

// �R���X�g���N�^
IGES_PARSER::IGES_PARSER()
{
		entity[0] = CIRCLE_ARC;							// �~/�~��
		entity[1] = COMPOSITE_CURVE;					// �����Ȑ�
		entity[2] = CONIC_ARC;							// �~���Ȑ�
		entity[3] = COPIOUS_DATA;						// �L�ӓ_��
		entity[4] = PLANE;								// ����
		entity[5] = LINE;								// ����
		entity[6] = PARAMETRIC_SPLINE_CURVE;			// �p�����g���b�N�X�v���C���Ȑ�
		entity[7] = PARAMETRIC_SPLINE_SURFACE;			// �p�����g���b�N�X�v���C���Ȗ�
		entity[8] = POINT;								// �_
		entity[9] = TRANSFORMATION_MATRIX;				// �ϊ��s��
		entity[10] = NURBS_CURVE;						// �L��B�X�v���C���Ȑ�
		entity[11] = NURBS_SURFACE;						// �L��B�X�v���C���Ȗ�
		entity[12] = CURVE_ON_PARAMETRIC_SURFACE; 		// �ʏ��
		entity[13] = TRIMMED_SURFACE;					// �g������
		entity[14] = SUBFIGURE_DEFINITION;				// �q�}�̒�`
		entity[15] = ASSOCIATIVITY_INSTANCE;			// �O���[�v
		entity[16] = DRAWING;							// �}��
		entity[17] = PROPERTY;							// �}�ʃT�C�Y
		entity[18] = SINGULAR_SUBFIGURE_INSTANCE;		// �q�}�̎Q��
		entity[19] = VIEW;								// ���ۖ�
}

// �e�G���e�B�e�B�̕\��������ݒ�
void IGES_PARSER::InitDisplayStat(DispStat *Dstat)
{
	// ���F��ݒ�
	Dstat->Color[0] = 1.0;
	Dstat->Color[1] = 1.0;
	Dstat->Color[2] = 1.0;
	Dstat->Color[3] = 1.0;

	// �\��������ǉ�����ꍇ�͈ȉ��ɒǉ��̃R�[�h���L�q
}

// Type110(����)����Type126(NURBS�Ȑ�)�̃p�����[�^�𓾂�
int IGES_PARSER::GetNurbsCFromLine(int NurbsCount,int LineCount,BODY body)	
{
	int i=0;
	int errflag=0;

	body.NurbsC[NurbsCount].K = 2;		// ���a�L���̏㑤�Y���i�R���g���[���|�C���g-1�j�̒l
	body.NurbsC[NurbsCount].M = 2;		// ���֐��̊K��
	body.NurbsC[NurbsCount].N = body.NurbsC[NurbsCount].K + body.NurbsC[NurbsCount].M;	// �m�b�g�x�N�g���̐�

	// �u�[���A���^�v���p�e�B4��
	body.NurbsC[NurbsCount].prop[0] = 0;
	body.NurbsC[NurbsCount].prop[1] = 0;
	body.NurbsC[NurbsCount].prop[2] = 1;
	body.NurbsC[NurbsCount].prop[3] = 0;

	// �������[�m��
	errflag++;	// 1
	if((body.NurbsC[NurbsCount].T = (double *)malloc(sizeof(double)*body.NurbsC[NurbsCount].N)) == NULL){
		goto EXIT;
	}
	errflag++;	// 2
	if((body.NurbsC[NurbsCount].W = (double *)malloc(sizeof(double)*body.NurbsC[NurbsCount].K)) == NULL){
		goto EXIT;
	}
	errflag++;	// 3
	if((body.NurbsC[NurbsCount].cp = (Coord *)malloc(sizeof(Coord)*body.NurbsC[NurbsCount].K)) == NULL){
		goto EXIT;
	}

	// �m�b�g�x�N�g���̒l	
	body.NurbsC[NurbsCount].T[0] = 0.;
	body.NurbsC[NurbsCount].T[1] = 0.;
	body.NurbsC[NurbsCount].T[2] = 1.;
	body.NurbsC[NurbsCount].T[3] = 1.;
	
	for(i=0;i<body.NurbsC[NurbsCount].K;i++){				// Weight�̒l
		body.NurbsC[NurbsCount].W[i] = 1.;
	}
	for(i=0;i<body.NurbsC[NurbsCount].K;i++){				// �R���g���[���|�C���g�̍��W�l
		body.NurbsC[NurbsCount].cp[i].x = body.Line[LineCount].cp[i].x;
		body.NurbsC[NurbsCount].cp[i].y = body.Line[LineCount].cp[i].y;
		body.NurbsC[NurbsCount].cp[i].z = body.Line[LineCount].cp[i].z;
	}
	
	// �p�����[�^�̒l
	body.NurbsC[NurbsCount].V[0] = 0.;
	body.NurbsC[NurbsCount].V[1] = 1.;

	InitDisplayStat(&body.NurbsC[NurbsCount].Dstat);	// �\�������̏�����

	return TRUE;

	// �������[�m�ۂɎ��s�����ꍇ�͍��܂Ŋm�ۂ��������J������ERR��Ԃ�
EXIT:
		fprintf(stderr,"PARAMETER SECTION ERROR:�������[�̊m�ۂɎ��s���܂���\n");
	if(errflag == 3){
		free(body.NurbsC[NurbsCount].cp);
		errflag--;
	}
	if(errflag == 2){
		free(body.NurbsC[NurbsCount].W);
		errflag--;
	}
	if(errflag == 1){
		free(body.NurbsC[NurbsCount].T);
	}
	return ERR;
}

// Type110(�~�E�~��)����Type126(NURBS�Ȑ�)�̃p�����[�^�f�[�^�𓾂�
int IGES_PARSER::GetNurbsCFromCirA(int NurbsCount,int CirCount,BODY body)	
{
	int	 flag=TRUE;
	double	angle_deg = 0.0,
			angle_rad = 0.0,
			radius = 0.0;
	Coord	vec[2];
	
	// �~/�~�ʂ̒��S�_O-�n�_Ps�x�N�g�������A���S�_-�I�_Pe�x�N�g�����������ꂼ�ꋁ�߂�
	vec[0] = SubCoord(body.CirA[CirCount].cp[1],body.CirA[CirCount].cp[0]);
	vec[1] = SubCoord(body.CirA[CirCount].cp[2],body.CirA[CirCount].cp[0]);	

	radius = body.CirA[CirCount].R;	// �~/�~�ʂ̒��S�_�Ǝn�_�̋���(���a)
	angle_rad = Rot_Vec_Ang(vec[0],vec[1]);			// �~/�~�ʂ𐬂����S�p�̑傫��(degree)�����߂�
	angle_deg = RadToDeg(angle_rad);				// �~/�~�ʂ𐬂����S�p�̑傫��(radian)�����߂�

	// ���S�p(degree)�̑傫�����ƂɃZ�O�����g����ύX����
	if( angle_deg > 0 && angle_deg <= 90 ){								// 0��<��<=90��
		flag = CirAToNurbsC_seg1(NurbsCount ,CirCount , body, vec, angle_rad);		// 1�Z�O�����g
	}
	else if( angle_deg > 90 && angle_deg <= 270 ){						// 90��<��<=270��
		flag = CirAToNurbsC_seg2(NurbsCount ,CirCount , body, vec, angle_rad);		// 2�Z�O�����g
	}
	else if( angle_deg > 270 && angle_deg < 360 ){						// 270��<��<360��
		flag = CirAToNurbsC_seg3(NurbsCount ,CirCount , body, vec, angle_rad);		// 3�Z�O�����g
	}
	else if( angle_deg == 0 ){											// ��=0��(360��)
		flag = CirAToNurbsC_seg4(NurbsCount ,CirCount , body, vec, radius);			//�@4�Z�O�����g
	}
	else{
		fprintf(stderr,"�~/�~�ʂ̒��S�p������ɎZ�o����Ă��܂���\n");
		return ERR;
	}

	return TRUE;
}

// 1�Z�O�����g�̉~��(���S�p��0��<��<=90���̎�)
int IGES_PARSER::CirAToNurbsC_seg1(int NurbsCount,int CirCount,BODY body,Coord vec[], double angle_rad)
{
	int i=0;
	int errflag=0;
	
	Coord	vec_cp;
	
	body.NurbsC[NurbsCount].K = 3;		// ���a�L���̏㑤�Y���i�R���g���[���|�C���g-1�j�̒l
	body.NurbsC[NurbsCount].M = 3;		// ���֐��̊K��
	body.NurbsC[NurbsCount].N = body.NurbsC[NurbsCount].K + body.NurbsC[NurbsCount].M;	// �m�b�g�x�N�g���̐�

	// �u�[���A���^�v���p�e�B4��
	body.NurbsC[NurbsCount].prop[0] = 0;
	body.NurbsC[NurbsCount].prop[1] = 0;
	body.NurbsC[NurbsCount].prop[2] = 1;
	body.NurbsC[NurbsCount].prop[3] = 0;
	
	// �������[�m��
	errflag++;	// 1
	if((body.NurbsC[NurbsCount].T = (double *)malloc(sizeof(double)*body.NurbsC[NurbsCount].N)) == NULL){
		goto EXIT;
	}
	errflag++;	// 2
	if((body.NurbsC[NurbsCount].W = (double *)malloc(sizeof(double)*body.NurbsC[NurbsCount].K)) == NULL){
		goto EXIT;
	}
	errflag++;	// 3
	if((body.NurbsC[NurbsCount].cp = (Coord *)malloc(sizeof(Coord)*body.NurbsC[NurbsCount].K)) == NULL){
		goto EXIT;
	}
	
	// �m�b�g�x�N�g���̒l	
	body.NurbsC[NurbsCount].T[0] = 0.;
	body.NurbsC[NurbsCount].T[1] = 0.;
	body.NurbsC[NurbsCount].T[2] = 0.;
	body.NurbsC[NurbsCount].T[3] = 1.;
	body.NurbsC[NurbsCount].T[4] = 1.;
	body.NurbsC[NurbsCount].T[5] = 1.;
		
	// Weight�̒l
	for(i=0; i<3; i++){
		if(i % 2 == 0){
			body.NurbsC[NurbsCount].W[i] = 1.;
		}	
		else if(i % 2 == 1){	
			body.NurbsC[NurbsCount].W[i] = cos(angle_rad/2);
		}
	}
		
	vec_cp = Arc_CP(vec[0], vec[1], cos(angle_rad));	//�@�~�̒��S�_����R���g���[���|�C���gP1�ւ̃x�N�g�������߂�
	
	// �R���g���[���|�C���g�̍��W�l
	body.NurbsC[NurbsCount].cp[0].x = body.CirA[CirCount].cp[1].x;
	body.NurbsC[NurbsCount].cp[0].y = body.CirA[CirCount].cp[1].y;		
	body.NurbsC[NurbsCount].cp[1].x = vec_cp.x + body.CirA[CirCount].cp[0].x;
	body.NurbsC[NurbsCount].cp[1].y = vec_cp.y + body.CirA[CirCount].cp[0].y;
	body.NurbsC[NurbsCount].cp[2].x = body.CirA[CirCount].cp[2].x;
	body.NurbsC[NurbsCount].cp[2].y = body.CirA[CirCount].cp[2].y;

	for(i=0; i<3; i++){
		body.NurbsC[NurbsCount].cp[i].z = body.CirA[CirCount].zt;	// Z�����̑傫���͈��
	}
		
	body.NurbsC[NurbsCount].V[0] = 0.;		// �p�����[�^�̒l
	body.NurbsC[NurbsCount].V[1] = 1.;

	InitDisplayStat(&body.NurbsC[NurbsCount].Dstat);
		  
	return TRUE;

	// �������[�m�ۂɎ��s�����ꍇ�͍��܂Ŋm�ۂ��������J������ERR��Ԃ�
EXIT:
		fprintf(stderr,"PARAMETER SECTION ERROR:�������[�̊m�ۂɎ��s���܂���\n");
	if(errflag == 3){
		free(body.NurbsC[NurbsCount].cp);
		errflag--;
	}
	if(errflag == 2){
		free(body.NurbsC[NurbsCount].W);
		errflag--;
	}
	if(errflag == 1){
		free(body.NurbsC[NurbsCount].T);
	}
	return ERR;
}

// 2�Z�O�����g�̉~��(���S�p��90��<��<=270���̎�)
int IGES_PARSER::CirAToNurbsC_seg2(int NurbsCount,int CirCount,BODY body,Coord vec[], double angle_rad)
{
	int	i = 0,
		errflag = 0;
	double	angle_rad2 = 0.0;
	
	Coord vec_cp[3];
	
	body.NurbsC[NurbsCount].K = 5;		// ���a�L���̏㑤�Y���i�R���g���[���|�C���g-1�j�̒l
	body.NurbsC[NurbsCount].M = 3;		// ���֐��̊K��
	body.NurbsC[NurbsCount].N = body.NurbsC[NurbsCount].K + body.NurbsC[NurbsCount].M;	// �m�b�g�x�N�g���̐�
	// �u�[���A���^�v���p�e�B4��
	body.NurbsC[NurbsCount].prop[0] = 0;
	body.NurbsC[NurbsCount].prop[1] = 0;
	body.NurbsC[NurbsCount].prop[2] = 1;
	body.NurbsC[NurbsCount].prop[3] = 0;
	
	// �������[�m��
	errflag++;	// 1
	if((body.NurbsC[NurbsCount].T = (double *)malloc(sizeof(double)*body.NurbsC[NurbsCount].N)) == NULL){
		goto EXIT;
	}
	errflag++;	// 2
	if((body.NurbsC[NurbsCount].W = (double *)malloc(sizeof(double)*body.NurbsC[NurbsCount].K)) == NULL){
		goto EXIT;
	}
	errflag++;	// 3
	if((body.NurbsC[NurbsCount].cp = (Coord *)malloc(sizeof(Coord)*body.NurbsC[NurbsCount].K)) == NULL){
		goto EXIT;
	}
	
	// �m�b�g�x�N�g���̒l	
	body.NurbsC[NurbsCount].T[0] = 0.;
	body.NurbsC[NurbsCount].T[1] = 0.;
	body.NurbsC[NurbsCount].T[2] = 0.;
	body.NurbsC[NurbsCount].T[3] = 2./4.;
	body.NurbsC[NurbsCount].T[4] = 2./4.;
	body.NurbsC[NurbsCount].T[5] = 1.;
	body.NurbsC[NurbsCount].T[6] = 1.;
	body.NurbsC[NurbsCount].T[7] = 1.;
		
	// Weight�̒l
	for(i=0; i<5; i++){
		if(i % 2 == 0){
			body.NurbsC[NurbsCount].W[i] = 1.;
		}	
		else if(i % 2 == 1){	
			body.NurbsC[NurbsCount].W[i] = cos(angle_rad/4);
		}
	}
		
	angle_rad2 = angle_rad/2;	// (���S�p)��2
	
	vec_cp[1] = Rot_Vec(vec[0], angle_rad2);		// �~�̒��S�_���璆�S�p�̔����̈ʒu(�R���g���[���|�C���gP2)�ւ̃x�N�g�������߂�
	vec_cp[0] = Arc_CP(vec[0], vec_cp[1], cos(angle_rad2));	// �~�̒��S�_����R���g���[���|�C���gP1�ւ̃x�N�g�������߂�
	vec_cp[2] = Arc_CP(vec_cp[1], vec[1], cos(angle_rad2));	// �~�̒��S�_����R���g���[���|�C���gP3�ւ̃x�N�g�������߂�
	
	// �R���g���[���|�C���g�̍��W�l
	body.NurbsC[NurbsCount].cp[0].x = body.CirA[CirCount].cp[1].x;
	body.NurbsC[NurbsCount].cp[0].y = body.CirA[CirCount].cp[1].y;		
 	body.NurbsC[NurbsCount].cp[1].x = vec_cp[0].x + body.CirA[CirCount].cp[0].x;
 	body.NurbsC[NurbsCount].cp[1].y = vec_cp[0].y + body.CirA[CirCount].cp[0].y;
 	body.NurbsC[NurbsCount].cp[2].x = vec_cp[1].x + body.CirA[CirCount].cp[0].x;
 	body.NurbsC[NurbsCount].cp[2].y = vec_cp[1].y + body.CirA[CirCount].cp[0].y;
 	body.NurbsC[NurbsCount].cp[3].x = vec_cp[2].x + body.CirA[CirCount].cp[0].x;
 	body.NurbsC[NurbsCount].cp[3].y = vec_cp[2].y + body.CirA[CirCount].cp[0].y;
 	body.NurbsC[NurbsCount].cp[4].x = body.CirA[CirCount].cp[2].x;
 	body.NurbsC[NurbsCount].cp[4].y = body.CirA[CirCount].cp[2].y;
	for(i=0; i<5; i++){
		body.NurbsC[NurbsCount].cp[i].z = body.CirA[CirCount].zt;	// Z�����̑傫���͈��
	}
	
	body.NurbsC[NurbsCount].V[0] = 0.;		// �p�����[�^�̒l
	body.NurbsC[NurbsCount].V[1] = 1.;

	InitDisplayStat(&body.NurbsC[NurbsCount].Dstat);
		  
	return TRUE;

	// �������[�m�ۂɎ��s�����ꍇ�͍��܂Ŋm�ۂ��������J������ERR��Ԃ�
EXIT:
		fprintf(stderr,"PARAMETER SECTION ERROR:�������[�̊m�ۂɎ��s���܂���\n");
	if(errflag == 3){
		free(body.NurbsC[NurbsCount].cp);
		errflag--;
	}
	if(errflag == 2){
		free(body.NurbsC[NurbsCount].W);
		errflag--;
	}
	if(errflag == 1){
		free(body.NurbsC[NurbsCount].T);
	}
	return ERR;
}

// 3�Z�O�����g�̉~��(���S�p��270��<��<360���̎�)
int IGES_PARSER::CirAToNurbsC_seg3(int NurbsCount,int CirCount,BODY body,Coord vec[], double angle_rad)
{
	int	i=0,
		errflag=0;
	double	angle_rad3 = 0.0;
	
	Coord	vec_cp[5];
	
	body.NurbsC[NurbsCount].K = 7;		// ���a�L���̏㑤�Y���i�R���g���[���|�C���g-1�j�̒l
	body.NurbsC[NurbsCount].M = 3;		// ���֐��̊K��
	body.NurbsC[NurbsCount].N = body.NurbsC[NurbsCount].K + body.NurbsC[NurbsCount].M;	// �m�b�g�x�N�g���̐�
	
	// �u�[���A���^�v���p�e�B4��
	body.NurbsC[NurbsCount].prop[0] = 0;
	body.NurbsC[NurbsCount].prop[1] = 0;
	body.NurbsC[NurbsCount].prop[2] = 1;
	body.NurbsC[NurbsCount].prop[3] = 0;
	
	// �������[�m��
	errflag++;	// 1
	if((body.NurbsC[NurbsCount].T = (double *)malloc(sizeof(double)*body.NurbsC[NurbsCount].N)) == NULL){
		goto EXIT;
	}
	errflag++;	// 2
	if((body.NurbsC[NurbsCount].W = (double *)malloc(sizeof(double)*body.NurbsC[NurbsCount].K)) == NULL){
		goto EXIT;
	}
	errflag++;	// 3
	if((body.NurbsC[NurbsCount].cp = (Coord *)malloc(sizeof(Coord)*body.NurbsC[NurbsCount].K)) == NULL){
		goto EXIT;
	}
	
	// �m�b�g�x�N�g���̒l	
	body.NurbsC[NurbsCount].T[0] = 0.;
	body.NurbsC[NurbsCount].T[1] = 0.;
	body.NurbsC[NurbsCount].T[2] = 0.;
	body.NurbsC[NurbsCount].T[3] = 1./3.;
	body.NurbsC[NurbsCount].T[4] = 1./3.;
	body.NurbsC[NurbsCount].T[5] = 2./3.;
	body.NurbsC[NurbsCount].T[6] = 2./3.;
	body.NurbsC[NurbsCount].T[7] = 1.;
	body.NurbsC[NurbsCount].T[8] = 1.;
	body.NurbsC[NurbsCount].T[9] = 1.;
	
	// Weight�̒l
	for(i=0; i<7; i++){
		if(i % 2 == 0){
			body.NurbsC[NurbsCount].W[i] = 1.;
		}	
		else if(i % 2 == 1){	
			body.NurbsC[NurbsCount].W[i] = cos(angle_rad/6);
		}
	}

	angle_rad3 = angle_rad/3;	// (���S�p)��3
	
	vec_cp[1] = Rot_Vec(vec[0], angle_rad3);		// �~�̒��S�_���璆�S�p��1/3�̈ʒu(�R���g���[���|�C���gP2)�ւ̃x�N�g�������߂�
	vec_cp[0] = Arc_CP(vec[0], vec_cp[1], cos(angle_rad3));	// �~�̒��S�_����R���g���[���|�C���gP1�ւ̃x�N�g�������߂�
	vec_cp[3] = Rot_Vec(vec_cp[1], angle_rad3);		// �~�̒��S�_���璆�S�p��2/3�̈ʒu(�R���g���[���|�C���gP4)�ւ̃x�N�g�������߂�
	vec_cp[2] = Arc_CP(vec_cp[1], vec_cp[3], cos(angle_rad3));	// �~�̒��S�_����R���g���[���|�C���gP3�ւ̃x�N�g�������߂�
	vec_cp[4] = Arc_CP(vec_cp[3], vec[1], cos(angle_rad3));	// �~�̒��S�_����R���g���[���|�C���gP4�ւ̃x�N�g�������߂�
		
	// �R���g���[���|�C���g�̍��W�l
	body.NurbsC[NurbsCount].cp[0].x = body.CirA[CirCount].cp[1].x;
	body.NurbsC[NurbsCount].cp[0].y = body.CirA[CirCount].cp[1].y;		
	body.NurbsC[NurbsCount].cp[1].x = vec_cp[0].x + body.CirA[CirCount].cp[0].x;
	body.NurbsC[NurbsCount].cp[1].y = vec_cp[0].y + body.CirA[CirCount].cp[0].y;
	body.NurbsC[NurbsCount].cp[2].x = vec_cp[1].x + body.CirA[CirCount].cp[0].x;
	body.NurbsC[NurbsCount].cp[2].y = vec_cp[1].y + body.CirA[CirCount].cp[0].y;
	body.NurbsC[NurbsCount].cp[3].x = vec_cp[2].x + body.CirA[CirCount].cp[0].x;
	body.NurbsC[NurbsCount].cp[3].y = vec_cp[2].y + body.CirA[CirCount].cp[0].y;
	body.NurbsC[NurbsCount].cp[4].x = vec_cp[3].x + body.CirA[CirCount].cp[0].x;
	body.NurbsC[NurbsCount].cp[4].y = vec_cp[3].y + body.CirA[CirCount].cp[0].y;
	body.NurbsC[NurbsCount].cp[5].x = vec_cp[4].x + body.CirA[CirCount].cp[0].x;
	body.NurbsC[NurbsCount].cp[5].y = vec_cp[4].y + body.CirA[CirCount].cp[0].y;
	body.NurbsC[NurbsCount].cp[6].x = body.CirA[CirCount].cp[2].x;
	body.NurbsC[NurbsCount].cp[6].y = body.CirA[CirCount].cp[2].y;

	for(i=0; i<7; i++){
		body.NurbsC[NurbsCount].cp[i].z = body.CirA[CirCount].zt;	// Z�����̑傫���͈��
	}
		
	body.NurbsC[NurbsCount].V[0] = 0.;		// �p�����[�^�̒l
	body.NurbsC[NurbsCount].V[1] = 1.;

	InitDisplayStat(&body.NurbsC[NurbsCount].Dstat);
		  
	return TRUE;

	// �������[�m�ۂɎ��s�����ꍇ�͍��܂Ŋm�ۂ��������J������ERR��Ԃ�
EXIT:
		fprintf(stderr,"PARAMETER SECTION ERROR:�������[�̊m�ۂɎ��s���܂���\n");
	if(errflag == 3){
		free(body.NurbsC[NurbsCount].cp);
		errflag--;
	}
	if(errflag == 2){
		free(body.NurbsC[NurbsCount].W);
		errflag--;
	}
	if(errflag == 1){
		free(body.NurbsC[NurbsCount].T);
	}
	return ERR;
}

// 4�Z�O�����g�̉~��(�~)
int IGES_PARSER::CirAToNurbsC_seg4(int NurbsCount,int CirCount,BODY body,Coord vec[], double radius)
{
	int i=0;
	int errflag=0;

	body.NurbsC[NurbsCount].K = 9;		// ���a�L���̏㑤�Y���i�R���g���[���|�C���g-1�j�̒l
	body.NurbsC[NurbsCount].M = 3;		// ���֐��̊K��
	body.NurbsC[NurbsCount].N = body.NurbsC[NurbsCount].K + body.NurbsC[NurbsCount].M;	// �m�b�g�x�N�g���̐�
	
	// �u�[���A���^�v���p�e�B4��
	body.NurbsC[NurbsCount].prop[0] = 0;
	body.NurbsC[NurbsCount].prop[1] = 0;
	body.NurbsC[NurbsCount].prop[2] = 1;
	body.NurbsC[NurbsCount].prop[3] = 0;
	
	// �������[�m��
	errflag++;	// 1
	if((body.NurbsC[NurbsCount].T = (double *)malloc(sizeof(double)*body.NurbsC[NurbsCount].N)) == NULL){
		goto EXIT;
	}
	errflag++;	// 2
	if((body.NurbsC[NurbsCount].W = (double *)malloc(sizeof(double)*body.NurbsC[NurbsCount].K)) == NULL){
		goto EXIT;
	}
	errflag++;	// 3
	if((body.NurbsC[NurbsCount].cp = (Coord *)malloc(sizeof(Coord)*body.NurbsC[NurbsCount].K)) == NULL){
		goto EXIT;
	}
	
	// �m�b�g�x�N�g���̒l	
	body.NurbsC[NurbsCount].T[0] = 0.;
	body.NurbsC[NurbsCount].T[1] = 0.;
	body.NurbsC[NurbsCount].T[2] = 0.;
	body.NurbsC[NurbsCount].T[3] = 1./4.;
	body.NurbsC[NurbsCount].T[4] = 1./4.;
	body.NurbsC[NurbsCount].T[5] = 2./4.;
	body.NurbsC[NurbsCount].T[6] = 2./4.;
	body.NurbsC[NurbsCount].T[7] = 3./4.;
	body.NurbsC[NurbsCount].T[8] = 3./4.;
	body.NurbsC[NurbsCount].T[9] = 1.;
	body.NurbsC[NurbsCount].T[10] = 1.;
	body.NurbsC[NurbsCount].T[11] = 1.;
		
	// Weight�̒l
	for(i=0; i<9; i++){
		if(i % 2 == 0){
			body.NurbsC[NurbsCount].W[i] = 1.;
		}	
		else if(i % 2 == 1){	
			body.NurbsC[NurbsCount].W[i] = sqrt(2.0)/2;
		}
	}

	// �R���g���[���|�C���g�̍��W�l
	body.NurbsC[NurbsCount].cp[0].x = body.CirA[CirCount].cp[0].x + radius;
	body.NurbsC[NurbsCount].cp[0].y = body.CirA[CirCount].cp[0].y;		
	body.NurbsC[NurbsCount].cp[1].x = body.CirA[CirCount].cp[0].x + radius;
	body.NurbsC[NurbsCount].cp[1].y = body.CirA[CirCount].cp[0].y + radius;
	body.NurbsC[NurbsCount].cp[2].x = body.CirA[CirCount].cp[0].x;
	body.NurbsC[NurbsCount].cp[2].y = body.CirA[CirCount].cp[0].y + radius;
	body.NurbsC[NurbsCount].cp[3].x = body.CirA[CirCount].cp[0].x - radius;
	body.NurbsC[NurbsCount].cp[3].y = body.CirA[CirCount].cp[0].y + radius;
	body.NurbsC[NurbsCount].cp[4].x = body.CirA[CirCount].cp[0].x - radius;
	body.NurbsC[NurbsCount].cp[4].y = body.CirA[CirCount].cp[0].y;
	body.NurbsC[NurbsCount].cp[5].x = body.CirA[CirCount].cp[0].x - radius;
	body.NurbsC[NurbsCount].cp[5].y = body.CirA[CirCount].cp[0].y - radius;
	body.NurbsC[NurbsCount].cp[6].x = body.CirA[CirCount].cp[0].x;
	body.NurbsC[NurbsCount].cp[6].y = body.CirA[CirCount].cp[0].y - radius;
	body.NurbsC[NurbsCount].cp[7].x = body.CirA[CirCount].cp[0].x + radius;
	body.NurbsC[NurbsCount].cp[7].y = body.CirA[CirCount].cp[0].y - radius;
	body.NurbsC[NurbsCount].cp[8].x = body.CirA[CirCount].cp[0].x + radius;
	body.NurbsC[NurbsCount].cp[8].y = body.CirA[CirCount].cp[0].y;

	for(i=0; i<9; i++){
		body.NurbsC[NurbsCount].cp[i].z = body.CirA[CirCount].zt;	// Z�����̑傫���͈��
	}
		
	body.NurbsC[NurbsCount].V[0] = 0.;		// �p�����[�^�̒l
	body.NurbsC[NurbsCount].V[1] = 1.;

	InitDisplayStat(&body.NurbsC[NurbsCount].Dstat);
	
	return TRUE;

	// �������[�m�ۂɎ��s�����ꍇ�͍��܂Ŋm�ۂ��������J������ERR��Ԃ�
EXIT:
		fprintf(stderr,"PARAMETER SECTION ERROR:�������[�̊m�ۂɎ��s���܂���\n");
	if(errflag == 3){
		free(body.NurbsC[NurbsCount].cp);
		errflag--;
	}
	if(errflag == 2){
		free(body.NurbsC[NurbsCount].W);
		errflag--;
	}
	if(errflag == 1){
		free(body.NurbsC[NurbsCount].T);
	}
	return ERR;
}

// Type124(�ϊ��s��)��p����NURBS�Ȑ������W�ϊ�����
int IGES_PARSER::TransformNurbsC(int NurbsCount,int TMp,BODY body)	
{
	int i;

	for(i=0;i<body.NurbsC[NurbsCount].K;i++){
		body.NurbsC[NurbsCount].cp[i] = HomoTransform(body.TMat[TMp].R,body.TMat[TMp].T,body.NurbsC[NurbsCount].cp[i]);	
	}
	
	return TRUE;
}