// IGES�p�[�T

#ifndef _IGES_PARSER_MAIN_H_
#define _IGES_PARSER_MAIN_H_

#include "BODY.h"

// Constants: General Defines
//#define SECTION_NUM -				�Z�N�V�����̐�(S,G,D,P,T)(5)
//#define COLUMN_MAX_ -				1�s�̃J������('\n'��'\0'���܂�)(82)
//#define COLUMN_MAX -				1�s�̃J������('\n'��'\0'�͊܂܂Ȃ�)(80)
//#define COL_CHAR -				�Z�N�V�������ʕ����̃J����(73)
//#define COL_P_DIRECTORY -			�p�����[�^���ł̃f�B���N�g�����ւ̋t�|�C���^������J����(65)
//#define GLOBALPARAMNUM -			�O���[�o�����̃p�����[�^��(25)
//#define FIELD_NUM -				�f�B���N�g������1�t�B�[���h�̕�����(8)
//#define DIRECTORYPARANUM -		�f�B���N�g�����̃p�����[�^��(20)
//#define SECTION_START -			�X�^�[�g���Z�N�V�������ʗp�V���{��(0)
//#define SECTION_GLOBAL -			�O���[�o���Z�N�V�������ʗp�V���{��(1)
//#define SECTION_DIRECTORY -		�f�B���N�g���Z�N�V�������ʗp�V���{��(2)
//#define SECTION_PARAMETER -		�p�����[�^�Z�N�V�������ʗp�V���{��(3)
//#define SECTION_TERMINATE -		�^�[�~�l�[�g�Z�N�V�������ʗp�V���{��(4)
//#define MAIN_ENTITY_TYPE_NUM -	�ǂݍ��ݑΏۂƂȂ�G���e�B�e�B�^�C�v�̐�(9)
//#define NRBS_PARAM_MAX -			NRBS�ȖʁE�Ȑ��Œ�`�����p�����[�^�̍ő�l(200)
#define SECTION_NUM		 5			
#define COLUMN_MAX_	     82			
#define COLUMN_MAX       80			
#define COL_CHAR		 73			
#define COL_P_DIRECTORY  65			
#define GLOBALPARAMNUM	 25			
#define FIELD_NUM		 8			
#define DIRECTORYPARANUM 20			
#define SECTION_START		0		
#define SECTION_GLOBAL		1		
#define SECTION_DIRECTORY	2		
#define SECTION_PARAMETER	3		
#define SECTION_TERMINATE	4		
#define MAIN_ENTITY_TYPE_NUM 9		
#define NRBS_PARAM_MAX  200			


// �O���[�o�����p�����[�^�̃V���{�����`
// Enum: Enum Symbol of Global Section's Parameter
//PARAM_DELIMITER=1 -		�p�����[�^�f���~�^
//RECORD_DELIMITER -		���R�[�h�f���~�^
//SEND_PRODUCT_ID -			���葤���i�h�c
//FILE_NAME -				�t�@�C����
//SEND_SYSTEM_ID -			���葤�V�X�e���h�c
//PRIRPO_VERSION -			�v���v���Z�b�T�o�[�W����
//INT_LENGTH -				�����l�̒���
//FLOAT_PNT_LENGTH -		�P���x���������_�w���̍ő�l
//FLOAT_PNT_FIGURE -		�P���x���������_�L������
//DOUBLE_PNT_LENGTH -		�{���x���������_�w���̍ő�l
//DOUBLE_PNT_FIGURE -		�{���x���������_�L������
//RECIVE_PRODUCT_ID -		�󂯎�葤���i�h�c
//MODEL_SCALE -				���f���X�P�[��
//UNIT_FLAG -				�P�ʃt���O
//UNIT -					�P��
//LINE_THICKNESS_NUMBER -	�����̍ő�ޕʐ�
//LINE_THICKNESS -			�����̍ő�l
//DATE_MAKE_FILE -			�t�@�C���쐬����
//MIN_ACCRACY -				�ŏ����x
//MODEL_SPACE_SIZE -		���f����Ԃ̑傫��
//FILE_MAKER_NAME -			�t�@�C���쐬��
//BELONG -					����
//IGES_VERSION -			�o�[�W����
//DRAFTING_STANDARD -		���}�K�i
//DATE_MAKE_MODEL -			���f���쐬��
enum GlobalParamType{
	PARAM_DELIMITER=1,
	RECORD_DELIMITER,
	SEND_PRODUCT_ID,
	FILE_NAME,		
	SEND_SYSTEM_ID,	
	PRIRPO_VERSION,	
	INT_LENGTH,		
	FLOAT_PNT_LENGTH,
	FLOAT_PNT_FIGURE,
	DOUBLE_PNT_LENGTH,
	DOUBLE_PNT_FIGURE,
	RECIVE_PRODUCT_ID,
	MODEL_SCALE,	
	UNIT_FLAG,		
	UNIT,			
	LINE_THICKNESS_NUMBER,
	LINE_THICKNESS,		
	DATE_MAKE_FILE,		
	MIN_ACCRACY,		
	MODEL_SPACE_SIZE,	
	FILE_MAKER_NAME,	
	BELONG,				
	IGES_VERSION,		
	DRAFTING_STANDARD,	
	DATE_MAKE_MODEL		
};

// �f�B���N�g�����p�����[�^�̃V���{�����`
// Enum: Enum Symbol of Directory Section's Parameter
	//ENTITY_TYPE_NUM -			�v�f�ԍ�
	//PARAM_DATA -				�p�����[�^���ւ̃|�C���^
	//STRUCTURE -				�X�g���N�`��
	//LINE_FONT_PATTERN -		����
	//LEVEL -					���x��
	//VIEW_ -					�r���[
	//TRAN_MATRIX -				�}�g���b�N�X�ւ̃|�C���^
	//LABEL_DISP_ASSOC -		���x���\��
	//STATUS_NUM -				�X�e�C�^�X
	//SEQUENCE_NUM -			�V�[�P���X�ԍ�
	//ENTITY_TYPE_NUM_ -		�v�f�ԍ�
	//LINE_WEIGHT_NUM -			����
	//COLOR_NUM -				�F
	//PARAM_LINE_COUNT -		�p�����[�^���̃��C����
	//FORM_NUM -				�`���ԍ�
	//RESERVED1 -				�\��
	//RESERVED2 -				�\��
	//ENTITY_LABEL -			�v�f�̃��x��
	//ENTITY_SUBSCRIPT_NUM -	�T�u�X�N���v�g
	//SEQUENCE_NUM_ -			�V�[�P���X�ԍ�
enum DirectoryParamType{
	ENTITY_TYPE_NUM,
	PARAM_DATA,		
	STRUCTURE,		
	LINE_FONT_PATTERN,
	LEVEL,			
	VIEW_,			
	TRAN_MATRIX,	
	LABEL_DISP_ASSOC,
	STATUS_NUM,		
	SEQUENCE_NUM,	
	ENTITY_TYPE_NUM_,
	LINE_WEIGHT_NUM,
	COLOR_NUM,		
	PARAM_LINE_COUNT,
	FORM_NUM,		
	RESERVED1,		
	RESERVED2,		
	ENTITY_LABEL,	
	ENTITY_SUBSCRIPT_NUM,
	SEQUENCE_NUM_		
};

// Structure: GlobalParam
// �O���[�o�����̕K�v�p�����[�^�\����
//
// Variables:
// double	scale -			���f���̃X�P�[��
// int		unit_flag -		�P�ʃt���O
// double	space_size -	���f����Ԃ̑傫��
typedef struct{
	double scale;
	int    unit_flag;
	double space_size;
}GlobalParam;

// Structure: DirectoryParam
// �f�B���N�g�����̕K�v�p�����[�^�\����
//
// Variables:
// int entity_type -		�v�f�ԍ�
// int entity_count -		���Ԗڂ�entity_type����\��
// int p_param -			�p�����[�^���ւ̃|�C���^
// int p_tm -				�}�g���b�N�X�ւ̃|�C���^
// int blank_stat -			�\������
// int subordinate_stat -	�]������
// int useflag_stat -		entity�g�p�Ӑ}
// int seq_num -			�V�[�P���X�ԍ�
// int param_line_count -	�p�����[�^���̃��C����
typedef struct{
	int entity_type;
	int entity_count;
	int p_param;	
	int p_tm;		
	int blank_stat;	
	int subordinate_stat;	
	int useflag_stat;		
	int seq_num;			
	int param_line_count;	
}DirectoryParam;

// Class: IGES_PARSER
// IGES�p�[�T�[�p�N���X
class IGES_PARSER
{
public:
	// Constructor: IGES_PARSER
	// IGES_PARSER�N���X�̃R���X�g���N�^
	IGES_PARSER();	

	//  Function: IGES_Parser_Main
	// IGES�t�@�C���̃p�[�Tmain
	int IGES_Parser_Main(BODY *, const char *);	

	//  Function: Optimize4OpenGL
	// �ǂݍ���IGES�t�@�C����OpenGL�p�ɍœK������(ExpandKnotRange(), ModifyParamConect(), CheckDegenracy(), CheckCWforTrim()�����s)
	int Optimize4OpenGL(BODY *);			

	//  Function: ExpandKnotRange
	// �ׂ荇���m�b�g�x�N�g���̍���MIN_KNOT_RANGE�ȏ�ɂȂ�悤�͈͂�ύX����
	int ExpandKnotRange(BODY *);			

	//  Function: ModifyParamConect
	// �p�����g���b�N���ʓ��̃g�����Ȑ����m�̂Ȃ�����`�F�b�N�A�C������
	int ModifyParamConect(BODY *);			

	//  Function: CheckDegenracy
	// �k��(2D�p�����g���b�N�Ȑ��̎n�_�ƏI�_����v���Ă��邩)�̃`�F�b�N
	int CheckDegenracy(BODY *);				

	//  Function: CheckCWforTrim
	// �g�����Ɏg���Ă��镡���Ȑ�����Ȃ鑽�p�`�����v��肩�����v��肩�𒲂ׁA�O���g�����͔����v���A�����g�����͎��v����ɂȂ�悤�ɕύX����
	int CheckCWforTrim(BODY *);				

	//  Function: NormalizeKnotRange
	// �m�b�g�x�N�g���͈̔͂�0�`val��(�m�b�g�̐��K��)
	int NormalizeKnotRange(BODY *,double);	


private:
	// Function: GetSectionLine
	// �e�Z�N�V�����̃��C�����𒲂ׂ�
	void GetSectionLine(FILE *,int []);								

	// Function: GetStartSection
	// �X�^�[�g���̓ǂݍ���
	int GetStartSection(FILE *,int);								

	// Function: GetGlobalSection
	// �O���[�o�����̓ǂݍ���
	int GetGlobalSection(FILE *,GlobalParam *,int);					

	// Function: GetDirectorySection
	// �f�B���N�g�����̓ǂݍ���
	int GetDirectorySection(FILE *,DirectoryParam *,int [],int);	

	// Function: GetStatusNumber
	// �f�B���N�g�����̏��#9���擾
	void GetStatusNumber(char [],DirectoryParam *);					

	// Function: GetParameterSection
	// �p�����[�^���̓ǂݍ���
	int GetParameterSection(FILE *,DirectoryParam *,BODY,int);		

	// Function: GetTerminateSection
	// �^�[�~�l�[�g���̓ǂݍ��݁i�X�P���g���j
	int GetTerminateSection(FILE *);								

	// Function: GetType
	// �e�G���e�B�e�B�^�C�v�̐����L������
	void GetType(int,int []);										

	// Function: GetCirAPara
	// Type100 �~�E�~�ʂ̓ǂݍ���
	int GetCirAPara(char [],int,DirectoryParam *,BODY);				

	// Function: GetCompCPara
	// Type102 �����Ȑ��̓ǂݍ���
	int GetCompCPara(char [],int,DirectoryParam *,int,BODY);		

	// Function: GetConAPara
	// Type104 �~���Ȑ��̓ǂݍ���
	int GetConAPara(char [],int,DirectoryParam *,BODY);				

	// Function: GetLinePara
	// Type110 �����̓ǂݍ���
	int GetLinePara(char [],int,DirectoryParam *,BODY);				

	// Function: GetTMatPara
	// Type124 �ϊ��s��̓ǂݍ���
	int GetTMatPara(char [],int,DirectoryParam *,BODY);				

	// Function: GetNurbsCPara
	// Type126 NRBS�Ȑ��̓ǂݍ���
	int GetNurbsCPara(char [],int,DirectoryParam *,BODY);			

	// Function: GetNurbsSPara
	// Type128 NRBS�Ȗʂ̓ǂݍ���
	int GetNurbsSPara(char [],int,DirectoryParam *,BODY);			

	// Function: GeConpSPara
	// Type142 �ʏ���̓ǂݍ���
	int GeConpSPara(char [],int,DirectoryParam *,int,BODY);			

	// Function: GetTrmSPara
	// Type144 �g�����ʂ̓ǂݍ���
	int GetTrmSPara(char [],int,DirectoryParam *,BODY);				

	// Function: CatchStringI
	// �J���}�܂ł̐��l��ǂݍ���ŕԂ�(int)
	int CatchStringI(char **);										

	// Function: CatchStringD
	// �J���}�܂ł̐��l��ǂݍ���ŕԂ�(double)
	double CatchStringD(char **);									

	// Function: ChangeEntityforNurbs
	// �G���e�B�e�B��S��NURBS�֕ύX����
	int ChangeEntityforNurbs(DirectoryParam *,BODY,int);			

	// Function: SearchMaxCoord
	// �S�ẴG���e�B�e�B�ɂ�������W�l�̍ő�l�𒲂ׂ�
	int SearchMaxCoord(BODY *,int []);	

	// Function: GetDEPointer
	// DE���ւ̃|�C���^���������ۂ̍\���̂ւ̃|�C���^��Ԃ�
	void *GetDEPointer(int ,BODY);		

	// Function: SearchEntType
	// DE���ւ̃|�C���^�̒l����G���e�B�e�B�̃^�C�v�𒲂ׂĕԂ�
	int SearchEntType(DirectoryParam *,int,int);	

	// Function: InitDisplayStat
	// �e�G���e�B�e�B�̕\��������ݒ�
	void InitDisplayStat(DispStat *);	

	// Function: TransformNurbsC
	// NURBS�Ȑ������W�ϊ�����
	int TransformNurbsC(int,int,BODY);

	// Function: ChangeKnotVecRange
	// �m�b�g�x�N�g���̐��K��sub1
	int ChangeKnotVecRange(double [],double [],int,int,int,double);

	// Function: ChangeKnot
	// �m�b�g�x�N�g���̐��K��sub2
	double ChangeKnot(double ,double ,double,double);	

	// Function: SearchMinVecRange
	// �m�b�g�x�N�g���̍ŏ��Ԋu��T��
	double SearchMinVecRange(double [],int,int);	

	// Function: ReverseCOMPELEM
	// COMPELEM�z��𔽓]
	void ReverseCOMPELEM(COMPC *);


private:
	// Variable: *body
	// (private)BODY�N���X�ւ̃|�C���^(IGES�f�[�^�͂���BODY�\���̂ɑS�Ċi�[�����)
	BODY *body;

	// Variable: NFunc
	// (private)NURBS_Func�N���X�̃C���X�^���X
	NURBS_Func NFunc;

	// Variable: *TypeNum
	// (private)�ǂ�BODY�I�u�W�F�N�g������邩������
	int *TypeNum;

	// Variable: buf[COLUMN_MAX_]
	// (private)�ėp������o�b�t�@
	char buf[COLUMN_MAX_];				 

	// Variable: entity[ALL_ENTITY_TYPE_NUM]
	// (private)�G���e�B�e�B�̔ԍ����i�[�����z��
	int  entity[ALL_ENTITY_TYPE_NUM];	 

	// Variable: TypeCount[ALL_ENTITY_TYPE_NUM]
	// (private)�e�G���e�B�e�B�^�C�v�̐��탁�����[�m�ې����i�[
	int  TypeCount[ALL_ENTITY_TYPE_NUM]; 
};

#endif
