#ifndef _IGES_PARSER_MAIN_H_
#define _IGES_PARSER_MAIN_H_

#include "BODY.h"

#define SECTION_NUM		 5			// �Z�N�V�����̐�(S,G,D,P,T)
#define COLUMN_MAX_	     82			// 1�s�̃J������('\n'��'\0'���܂�)
#define COLUMN_MAX       80			// 1�s�̃J������('\n'��'\0'�͊܂܂Ȃ�)
#define COL_CHAR		 73			// �Z�N�V�������ʕ����̃J����
#define COL_P_DIRECTORY  65			// �p�����[�^���ł̃f�B���N�g�����ւ̋t�|�C���^������J����
#define GLOBALPARAMNUM	 25			// �O���[�o�����̃p�����[�^��
#define FIELD_NUM		 8			// �f�B���N�g������1�t�B�[���h�̕�����
#define DIRECTORYPARANUM 20			// �f�B���N�g�����̃p�����[�^��
#define SECTION_START		0		// �X�^�[�g���Z�N�V�������ʗp�V���{��
#define SECTION_GLOBAL		1		// �O���[�o���Z�N�V�������ʗp�V���{��
#define SECTION_DIRECTORY	2		// �f�B���N�g���Z�N�V�������ʗp�V���{��
#define SECTION_PARAMETER	3		// �p�����[�^�Z�N�V�������ʗp�V���{��
#define SECTION_TERMINATE	4		// �^�[�~�l�[�g�Z�N�V�������ʗp�V���{��
#define MAIN_ENTITY_TYPE_NUM 9		// �ǂݍ��ݑΏۂƂȂ�G���e�B�e�B�^�C�v�̐�
#define NRBS_PARAM_MAX  200			// NRBS�ȖʁE�Ȑ��Œ�`�����p�����[�^�̍ő�l


// �O���[�o�����p�����[�^�̃V���{�����`
enum GlobalParamType{
	PARAM_DELIMITER=1,			// �p�����[�^�f���~�^
	RECORD_DELIMITER,			// ���R�[�h�f���~�^
	SEND_PRODUCT_ID,			// ���葤���i�h�c
	FILE_NAME,					// �t�@�C����
	SEND_SYSTEM_ID,				// ���葤�V�X�e���h�c
	PRIRPO_VERSION,				// �v���v���Z�b�T�o�[�W����
	INT_LENGTH,					// �����l�̒���
	FLOAT_PNT_LENGTH,			// �P���x���������_�w���̍ő�l
	FLOAT_PNT_FIGURE,			// �P���x���������_�L������
	DOUBLE_PNT_LENGTH,			// �{���x���������_�w���̍ő�l
	DOUBLE_PNT_FIGURE,			// �{���x���������_�L������
	RECIVE_PRODUCT_ID,			// �󂯎�葤���i�h�c
	MODEL_SCALE,				// ���f���X�P�[��
	UNIT_FLAG,					// �P�ʃt���O
	UNIT,						// �P��
	LINE_THICKNESS_NUMBER,		// �����̍ő�ޕʐ�
	LINE_THICKNESS,				// �����̍ő�l
	DATE_MAKE_FILE,				// �t�@�C���쐬����
	MIN_ACCRACY,				// �ŏ����x
	MODEL_SPACE_SIZE,			// ���f����Ԃ̑傫��
	FILE_MAKER_NAME,			// �t�@�C���쐬��
	BELONG,						// ����
	IGES_VERSION,				// �o�[�W����
	DRAFTING_STANDARD,			// ���}�K�i
	DATE_MAKE_MODEL				// ���f���쐬��
};

// �f�B���N�g�����p�����[�^�̃V���{�����`
enum DirectoryParamType{
	ENTITY_TYPE_NUM,			// �v�f�ԍ�
	PARAM_DATA,					// �p�����[�^���ւ̃|�C���^
	STRUCTURE,					// �X�g���N�`��
	LINE_FONT_PATTERN,			// ����
	LEVEL,						// ���x��
	VIEW_,						// �r���[
	TRAN_MATRIX,				// �}�g���b�N�X�ւ̃|�C���^
	LABEL_DISP_ASSOC,			// ���x���\��
	STATUS_NUM,					// �X�e�C�^�X
	SEQUENCE_NUM,				// �V�[�P���X�ԍ�
	ENTITY_TYPE_NUM_,			// �v�f�ԍ�
	LINE_WEIGHT_NUM,			// ����
	COLOR_NUM,					// �F
	PARAM_LINE_COUNT,			// �p�����[�^���̃��C����
	FORM_NUM,					// �`���ԍ�
	RESERVED1,					// �\��
	RESERVED2,					// �\��
	ENTITY_LABEL,				// �v�f�̃��x��
	ENTITY_SUBSCRIPT_NUM,		// �T�u�X�N���v�g
	SEQUENCE_NUM_				// �V�[�P���X�ԍ�
};

// �O���[�o�����̕K�v�p�����[�^�\����
typedef struct{
	double scale;			// ���f���̃X�P�[��
	int    unit_flag;		// �P�ʃt���O
	double space_size;		// ���f����Ԃ̑傫��
}GlobalParam;

// �f�B���N�g�����̕K�v�p�����[�^�\����
typedef struct{
	int entity_type;		// �v�f�ԍ�
	int entity_count;		// ���Ԗڂ�entity_type����\��
	int p_param;			// �p�����[�^���ւ̃|�C���^
	int p_tm;				// �}�g���b�N�X�ւ̃|�C���^
	int blank_stat;			// �\������
	int subordinate_stat;	// �]������
	int useflag_stat;		// entity�g�p�Ӑ}
	int seq_num;			// �V�[�P���X�ԍ�
	int param_line_count;	// �p�����[�^���̃��C����
}DirectoryParam;


// IGES�p�[�T�[�p�N���X���`
class IGES_PARSER
{
public:
	int IGES_Parser_Main(BODY *,char []);	// IGES�t�@�C���̃p�[�Tmain
	int Optimize4OpenGL(BODY *);			// �ǂݍ���IGES�t�@�C����OpenGL�p�ɍœK������(ExpandKnotRange(), ModifyParamConect(), CheckDegenracy(), CheckCWforTrim()�����s)
	int ExpandKnotRange(BODY *);			// �ׂ荇���m�b�g�x�N�g���̍���MIN_KNOT_RANGE�ȏ�ɂȂ�悤�͈͂�ύX����
	int ModifyParamConect(BODY *);			// �p�����g���b�N���ʓ��̃g�����Ȑ����m�̂Ȃ�����`�F�b�N�A�C������
	int CheckDegenracy(BODY *);				// �k��(2D�p�����g���b�N�Ȑ��̎n�_�ƏI�_����v���Ă��邩)�̃`�F�b�N
	int CheckCWforTrim(BODY *);				// �g�����Ɏg���Ă��镡���Ȑ�����Ȃ鑽�p�`�����v��肩�����v��肩�𒲂ׁA�O���g�����͔����v���A�����g�����͎��v����ɂȂ�悤�ɕύX����
	int NormalizeKnotRange(BODY *,double);	// �m�b�g�x�N�g���͈̔͂�0�`val��(�m�b�g�̐��K��)
	IGES_PARSER();					// �R���X�g���N�^

private:
	void GetSectionLine(FILE *,int []);								// �e�Z�N�V�����̃��C�����𒲂ׂ�
	int GetStartSection(FILE *,int);								// �X�^�[�g���̓ǂݍ���
	int GetGlobalSection(FILE *,GlobalParam *,int);					// �O���[�o�����̓ǂݍ���
	int GetDirectorySection(FILE *,DirectoryParam *,int [],int);	// �f�B���N�g�����̓ǂݍ���
	void GetStatusNumber(char [],DirectoryParam *);					// �f�B���N�g�����̏��#9���擾
	int GetParameterSection(FILE *,DirectoryParam *,BODY,int);		// �p�����[�^���̓ǂݍ���
	int GetTerminateSection(FILE *);								// �^�[�~�l�[�g���̓ǂݍ��݁i�X�P���g���j
	void GetType(int,int []);										// �e�G���e�B�e�B�^�C�v�̐����L������
	int GetCirAPara(char [],int,DirectoryParam *,BODY);				// Type100 �~�E�~�ʂ̓ǂݍ���
	int GetCompCPara(char [],int,DirectoryParam *,int,BODY);		// Type102 �����Ȑ��̓ǂݍ���
	int GetConAPara(char [],int,DirectoryParam *,BODY);				// Type104 �~���Ȑ��̓ǂݍ���
	int GetLinePara(char [],int,DirectoryParam *,BODY);				// Type110 �����̓ǂݍ���
	int GetTMatPara(char [],int,DirectoryParam *,BODY);				// Type124 �ϊ��s��̓ǂݍ���
	int GetNurbsCPara(char [],int,DirectoryParam *,BODY);			// Type126 NRBS�Ȑ��̓ǂݍ���
	int GetNurbsSPara(char [],int,DirectoryParam *,BODY);			// Type128 NRBS�Ȗʂ̓ǂݍ���
	int GeConpSPara(char [],int,DirectoryParam *,int,BODY);			// Type142 �ʏ���̓ǂݍ���
	int GetTrmSPara(char [],int,DirectoryParam *,BODY);				// Type144 �g�����ʂ̓ǂݍ���
	int CatchStringI(char **);										// �J���}�܂ł̐��l��ǂݍ���ŕԂ�(int)
	double CatchStringD(char **);									// �J���}�܂ł̐��l��ǂݍ���ŕԂ�(double)
	int ChangeEntityforNurbs(DirectoryParam *,BODY,int);			// �G���e�B�e�B��S��NURBS�֕ύX����
	int SearchMaxCoord(BODY *,int []);								// �S�ẴG���e�B�e�B�ɂ�������W�l�̍ő�l�𒲂ׂ�
	void *GetDEPointer(int ,BODY);									// DE���ւ̃|�C���^���������ۂ̍\���̂ւ̃|�C���^��Ԃ�
	int SearchEntType(DirectoryParam *,int,int);					// DE���ւ̃|�C���^�̒l����G���e�B�e�B�̃^�C�v�𒲂ׂĕԂ�
	void InitDisplayStat(DispStat *);								// �e�G���e�B�e�B�̕\��������ݒ�
	int GetNurbsCFromLine(int,int,BODY);							// Type110����Type126�̃p�����[�^�f�[�^�𓾂�
	int GetNurbsCFromCirA(int,int,BODY);					// Type100����Type126�̃p�����[�^�f�[�^�𓾂�
	int CirAToNurbsC_seg1(int,int,BODY,Coord [],double);	// Type100��1�Z�O�����g�̏ꍇ
	int CirAToNurbsC_seg2(int,int,BODY,Coord [],double);	// Type100��2�Z�O�����g�̏ꍇ
	int CirAToNurbsC_seg3(int,int,BODY,Coord [],double);	// Type100��3�Z�O�����g�̏ꍇ
	int CirAToNurbsC_seg4(int,int,BODY,Coord [],double);	// Type100��4�Z�O�����g�̏ꍇ
	int TransformNurbsC(int,int,BODY);						// NURBS�Ȑ������W�ϊ�����
	int ChangeKnotVecRange(double [],double [],int,int,int,double);	// �m�b�g�x�N�g���̐��K��sub1
	double ChangeKnot(double ,double ,double,double);				// �m�b�g�x�N�g���̐��K��sub2
	double SearchMinVecRange(double [],int,int);			// �m�b�g�x�N�g���̍ŏ��Ԋu��T��
	void ReverseCOMPELEM(COMPC *);							// COMPELEM�z��𔽓]

private:
	BODY *body;
	NURBS_Func NFunc;
	int *TypeNum;
	char buf[COLUMN_MAX_];				 // �ėp������o�b�t�@
	int  entity[ALL_ENTITY_TYPE_NUM];	 // �G���e�B�e�B�̔ԍ����i�[�����z��
	int  TypeCount[ALL_ENTITY_TYPE_NUM]; // �e�G���e�B�e�B�^�C�v�̐��탁�����[�m�ې����i�[
};

#endif
