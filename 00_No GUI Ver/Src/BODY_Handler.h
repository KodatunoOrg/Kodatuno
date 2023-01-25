// BODY�̊e�푀��N���X

#ifndef _BODY_HANDLER_H_
#define _BODY_HANDLER_H_

#include "NURBS_Func.h"
#include "Describe_BODY.h"


class BODY_Handler : public Describe_BODY
{
public:
	void InitCurveColor(float *);							// ���̐F�̏����l��^����
	void InitSurfaceColor(float *);							// �ʂ̐F�̏����l��^����
	void ChangeStatColor(float *,float,float,float,float);	// �G���e�B�e�B�̃X�e�[�^�X�Œ�`����Ă���F��ύX
	BODY *SearchBodyList(BODYList *,int);					// �Z���N�V��������Ă���BODY�ԍ��𓾂�
	void Init_body(BODY *);									// BODY������
	void RegistNurbsCtoBody(BODYList *,BODY *,NURBSC,char []);			// 1��NURBC�Ȑ���V����BODY�Ƃ��ēo�^����
	void RegistNurbsCtoBodyN(BODYList *,BODY *,NURBSC [],char [],int);	// N��NURBC�Ȑ���V����BODY�Ƃ��ēo�^����
	void RegistNurbsStoBody(BODYList *,BODY *,NURBSS,char []);			// 1��NURBS�Ȗʂ�V����BODY�Ƃ��ēo�^����
	void RegistNurbsStoBodyN(BODYList *,BODY *,NURBSS [],char [],int);	// N��NURBS�Ȗʂ�V����BODY�Ƃ��ēo�^����
	void RotBody(BODY *,Coord,double);						// BODY�̉�]
	void ShiftBody(BODY *, Coord);							// BODY�̃V�t�g


	// �e�G���e�B�e�B�̃������[����֐�
	void New_body(BODY *,int []);				// �S�G���e�B�e�B�̃������[�m��
	void Free_body(BODY *,int []);				// �S�G���e�B�e�B�̃������[���
	
private:
	NURBS_Func NFunc;
};

#endif
