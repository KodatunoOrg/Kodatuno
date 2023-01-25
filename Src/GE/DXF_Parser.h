#ifndef _DXF_PARSER_MAIN_H_
#define _DXF_PARSER_MAIN_H_

#include "BODY.h"

#define SECTION_NAME	2		// �Z�N�V��������\���V���{��
#define HEADER_SECTION	1		// �w�b�_�Z�N�V�����̃V���{��
#define CLASS_SECTION	2		// �N���X�Z�N�V�����̃V���{��
#define TABLE_SECTION	3		// �e�[�u���Z�N�V�����̃V���{��
#define BLOCK_SECTION	4		// �u���b�N�Z�N�V�����̃V���{��
#define ENTITIY_SECTION	5		// �G���e�B�e�B�Z�N�V�����̃V���{��
#define OBJECT_SECTION	6		// �I�u�W�F�N�g�Z�N�V�����̃V���{��
#define ENT_TYPE		0		// �G���e�B�e�B�^�C�v��\���V���{��

#define COORD_X			10		// X���W��\���V���{��
#define COORD_Y			20		// Y���W��\���V���{��
#define COORD_Z			30		// Z���W��\���V���{��
#define COORD_X_		11		// X���W��\���V���{��
#define COORD_Y_		21		// Y���W��\���V���{��
#define COORD_Z_		31		// Z���W��\���V���{��
#define RADIUS			40		// ���a��\���V���{��
#define START_ANG		50		// �~�ʊJ�n�p�x��\���V���{��
#define END_ANG			51		// �~�ʏI���p�x��\���V���{��

#define BUFSIZEMAX		256		// ������ꎞ�i�[�p�o�b�t�@�̃T�C�Y
#define LABELSIZEMAX	256		// ���x��������ꎞ�i�[�p�o�b�t�@�̃T�C�Y
#define EVEN			0		// �����̃V���{��
#define ODD				1		// ��̃V���{��

// DXF�p�[�T�[�p�N���X���`
class DXF_PARSER
{
public:
	DXF_PARSER();
	int DXF_Parser_Main(BODY *,const char *);	// DXF�t�@�C���̃p�[�Tmain

private:
	int ReadHeaderSection(FILE *,int,BODY *);		// �w�b�_�Z�N�V�����̓ǂݍ���
	int ReadClassesSection(FILE *,int,BODY *);		// �N���X�Z�N�V�����̓ǂݍ���
	int ReadTablesSection(FILE *,int,BODY *);		// �e�[�u���Z�N�V�����̓ǂݍ���
	int ReadBlocksSection(FILE *,int,BODY *);		// �u���b�N�Z�N�V�����̓ǂݍ���
	int ReadEntitiesSection(FILE *,int,BODY *);		// �G���e�B�e�B�Z�N�V�����̓ǂݍ���
	int ReadObjectsSection(FILE *,int,BODY *);		// �I�u�W�F�N�g�Z�N�V�����̓ǂݍ���
	int ResearchEntNum(FILE *,BODY *);				// �e�G���e�B�e�B�̐������T�[�`
	int GetArcData(FILE *,BODY *);					// �~�ʂ̃f�[�^���擾��BODY�Ɋi�[
	int GetCircleData(FILE *,BODY *);				// �~�̃f�[�^���擾��BODY�Ɋi�[
	int GetLineData(FILE *,BODY *);					// �����̃f�[�^���擾��BODY�Ɋi�[
	int SetStartEndPtArc(CIRA *);					// �~�ʂ̎n�_�C�I�_���Z�b�g
	int CalcUVvec(CIRA *);							// CIRA��UV�������W��ݒ肷��
	int EvenOdd(int);								// ���������
	int CheckSection(char *);						// �Z�N�V��������
	void InitDisplayStat(DispStat *);				// �e�G���e�B�e�B�̕\��������ݒ�
	int ChangeEntityforNurbs(BODY *);				// NURBS�Ȑ��ȊO�̃G���e�B�e�B��NURBS�Ȑ��ɕϊ�


private:
	char Buf[BUFSIZEMAX];		// ������ꎞ�i�[�p�o�b�t�@
	char Label[LABELSIZEMAX];	// ���x��������ꎞ�i�[�p�o�b�t�@
	int  Gcode;					// �O���[�v�R�[�h�i�[�o�b�t�@
	int  Count[ALL_ENTITY_TYPE_NUM];	// �G���e�B�e�B�J�E���^


};


#endif