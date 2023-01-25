// BODY�`��p�N���X���`

#ifndef _DESCRIBE_BODY_H_
#define _DESCRIBE_BODY_H_

#include "BODY.h"
#include "NURBS_Func.h"

#define COMMAND_DRAW_BODY  1			// BODY�`��p�f�B�X�v���C���X�g�̓o�^�ԍ�
#define COMMAND_DRAW_USER  2			// User���C���֐��ɂ���ăR�[�����ꂽOpenGL�`��֐��p�f�B�X�v���C���X�g�̓o�^�ԍ�
#define COMMAND_DRAW_USER_COMMAND 100	// User Command�ɂ���ăR�[�����ꂽOpenGL�`��֐��p�f�B�X�v���C���X�g�̓o�^�ԍ�


// BODY�G���e�B�e�B��`�悷��֐����W�߂��N���X
class Describe_BODY
{
public:
	Describe_BODY();
	~Describe_BODY();
	static void DrawBody(BODY *);						// BODY��`��
	static void DrawCompositeCurve(COMPC *);			// �����Ȑ���`��
	static void DrawLine(LINE_);						// ������`��
	static void DrawNurbsCurve(NURBSC);				// NURBS�Ȑ���`��
	static void DrawTrimdNurbsSurfe(NURBSS *);			// NURBS�Ȗʂ̕`��(�g��������)
	static void DrawNurbsSurfe(NURBSS);				// NURBS�Ȗʂ̕`��(�g�����Ȃ�)
	static void DrawCurveOnParamSurfe(CONPS *);		// �ʏ����`��
	static void DrawTrimdSurf(TRMS);					// �g�����ʂ�`��
	static void DrawNurbsCurve_Param(NURBSC *);		// 2D�p�����g���b�NNURBS�Ȑ��v�f�̕`��
	static void Draw_NurbsCurves(BODY *Body);			// BODY�Ɋ܂܂��NURBS�Ȑ���S�ĕ`��
	static void Draw_NurbsSurfaces(BODY *Body);		// BODY�Ɋ܂܂��NURBS�Ȗʂ�S�ĕ`��
	static void Draw_TrimSurfes(BODY *Body);			// BODY�Ɋ܂܂��g�����ʂ�S�ĕ`��
	static void SetNurbsStat();						// NURBS�`�掞�̕`��X�e�[�^�X��ݒ�
	static void SetNurbsSTolerance(GLfloat);			// NURBS�Ȗʂ̑e�����w��
	static void NURBS_Err(GLenum error_code);	// NURBS�`�掞�̃G���[�R�[���o�b�N
	static void SetNurbsSProperty(GLenum,GLfloat);		// NURBS�Ȗʂ̕`��`����ύX����

	static void DrawMesh(MESH *,int);					// ���b�V���̕`��

// ������
	static void DrawCircleArc();						// �~�E�~�ʂ�`��
	static void DrawConicArc();						// �~���Ȑ���`��
	static void DrawCircleArc_Param(CIRA *);			// 2D�p�����g���b�N�~�v�f�̕`��
	static void DrawConicArc_Param(CONA *);			// 2D�p�����g���b�N�~���Ȑ��v�f�̕`��
	static void DrawLine_Param(LINE_ *);				// 2D�p�����g���b�N�����v�f�̕`��


protected:
	static GLUnurbsObj *NurbsSurf;						// NURBS�Ȗʗp�I�u�W�F�N�g
	static GLUnurbsObj *NurbsCurve;					// NURBS�Ȑ��p�I�u�W�F�N�g
};

#endif
