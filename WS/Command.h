#ifndef _COMMAND_H_
#define _COMMAND_H_

#include<WSClistData.h>
#include"StdAfx.h"

#define MAXCOMMANDLEN	16			// �R�}���h�������̍ő�l
#define MAXCOMMANDOPNUM 5			// �o�^�ł���R�}���h�I�v�V������
#define COMMANDNUMMAX   200			// �o�^�ł���R�}���h��
#define SYSCOMMANDNUM	100			// Kodatuno�����炩���ߗp�ӂ���R�}���h�̍ő�l(0-99)
#define USERCOMMANDNUM	100			// ���[�U�[���Ǝ��ɓo�^�ł���R�}���h�̍ő�l(100-199)

// �R�}���h�p�\����
typedef struct{
	char Name[MAXCOMMANDLEN];	// �R�}���h��
	char Op[MAXCOMMANDOPNUM];	// �I�v�V������
	int  CmdNo;					// �L�[
	void (*Command)(int,char *[]);			// �R�}���h���s�֐��ւ̃|�C���^
}CommandMap;

void SetCmdList(CommandMap *,char *,char *,int,void (*Command)(int,char *[]));	// �R�}���h��o�^
int AnalCommandLine(char *,char *[]);				// �R�}���h���
int SearchRegdCmdNum(CommandMap *,char *);			// �����Ŏw�肵���R�}���h�̓o�^No�𒲂ׂ�




#endif
