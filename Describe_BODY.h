#ifndef _DESCRIBE_BODY_H_
#define _DESCRIBE_BODY_H_

#include "BODY.h"
#include "NURBS_Func.h"

#define COMMAND_DRAW_USER 2	// User���C���֐��ɂ���ăR�[�����ꂽOpenGL�`��֐��p�f�B�X�v���C���X�g�̓o�^�ԍ�
#define COMMAND_DRAW_BODY 1	// BODY�`��p�f�B�X�v���C���X�g�̓o�^�ԍ�

// BODY�G���e�B�e�B��`�悷��֐����W�߂��N���X
class Describe_BODY
{
public:
	Describe_BODY();
	~Describe_BODY();
	static void DrawCircleArc();					// �~�E�~�ʂ�`��
	static void DrawCompositeCurve(COMPC *);		// �����Ȑ���`��
	static void DrawConicArc();						// �~���Ȑ���`��
	static void DrawLine(LINE_);					// ������`��
	static void DrawNurbsCurve(NURBSC);				// NURBS�Ȑ���`��
	static void DrawTrimdNurbsSurfe(NURBSS *);		// NURBS�Ȗʂ̕`��(�g��������)
	static void DrawNurbsSurfe(NURBSS);				// NURBS�Ȗʂ̕`��(�g�����Ȃ�)
	static void DrawCurveOnParamSurfe(CONPS *);		// �ʏ����`��
	static void DrawTrimdSurf(TRMS);				// �g�����ʂ�`��
	static void DrawCircleArc_Param(CIRA *);		// 2D�p�����g���b�N�~�v�f�̕`��
	static void DrawConicArc_Param(CONA *);			// 2D�p�����g���b�N�~���Ȑ��v�f�̕`��
	static void DrawLine_Param(LINE_ *);			// 2D�p�����g���b�N�����v�f�̕`��
	static void DrawNurbsCurve_Param(NURBSC *);		// 2D�p�����g���b�NNURBS�Ȑ��v�f�̕`��
	static void CheckDegenracy(BODY *);				// �k��(2D�p�����g���b�N�Ȑ��̎n�_�ƏI�_����v���Ă��邩)�̃`�F�b�N
	static void SetNurbsStat();						// NURBS�`�掞�̕`��X�e�[�^�X��ݒ�
	static void NURBS_Err(GLenum error_code);		// NURBS�`�掞�̃G���[�R�[���o�b�N

protected:
	static GLUnurbsObj *NurbsSurf;					// NURBS�Ȗʗp�I�u�W�F�N�g
	static GLUnurbsObj *NurbsCurve;					// NURBS�Ȑ��p�I�u�W�F�N�g
	static GLfloat	uKnot[KNOTNUMMAX];				// NURBS�`��p�o�b�t�@
	static GLfloat	vKnot[KNOTNUMMAX];				// NURBS�`��p�o�b�t�@
	static GLfloat	CCtlp[CTLPNUMMAX][4];			// NURBS�`��p�o�b�t�@
	static GLfloat	SCtlp[CTLPNUMMAX][CTLPNUMMAX][4];	// NURBS�`��p�o�b�t�@
	static int  DrawBODYFlag;						// BODY�`��t���O
	static int  DrawSurfFlag;						// Surface�`��t���O
	static int	DrawBODY1st;						// BODY�`��1���ڂ������t���O

};

#endif