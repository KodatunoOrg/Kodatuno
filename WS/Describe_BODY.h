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
	void DrawBody(BODY *);						// BODY��`��
	void DrawCompositeCurve(COMPC *);			// �����Ȑ���`��
	void DrawLine(LINE_);						// ������`��
	void DrawNurbsCurve(NURBSC);				// NURBS�Ȑ���`��
	void DrawTrimdNurbsSurfe(NURBSS *);			// NURBS�Ȗʂ̕`��(�g��������)
	void DrawNurbsSurfe(NURBSS);				// NURBS�Ȗʂ̕`��(�g�����Ȃ�)
	void DrawCurveOnParamSurfe(CONPS *);		// �ʏ����`��
	void DrawTrimdSurf(TRMS);					// �g�����ʂ�`��
	void DrawNurbsCurve_Param(NURBSC *);		// 2D�p�����g���b�NNURBS�Ȑ��v�f�̕`��
	void Draw_NurbsCurves(BODY *Body);			// BODY�Ɋ܂܂��NURBS�Ȑ���S�ĕ`��
	void Draw_NurbsSurfaces(BODY *Body);		// BODY�Ɋ܂܂��NURBS�Ȗʂ�S�ĕ`��
	void Draw_TrimSurfes(BODY *Body);			// BODY�Ɋ܂܂��g�����ʂ�S�ĕ`��
	void SetNurbsStat();						// NURBS�`�掞�̕`��X�e�[�^�X��ݒ�
	void SetNurbsSTolerance(GLfloat);			// NURBS�Ȗʂ̑e�����w��
	static void NURBS_Err(GLenum error_code);	// NURBS�`�掞�̃G���[�R�[���o�b�N
	void SetNurbsSProperty(GLenum,GLfloat);		// NURBS�Ȗʂ̕`��`����ύX����

	void DrawMesh(MESH *,int);					// ���b�V���̕`��

// ������
	void DrawCircleArc();						// �~�E�~�ʂ�`��
	void DrawConicArc();						// �~���Ȑ���`��
	void DrawCircleArc_Param(CIRA *);			// 2D�p�����g���b�N�~�v�f�̕`��
	void DrawConicArc_Param(CONA *);			// 2D�p�����g���b�N�~���Ȑ��v�f�̕`��
	void DrawLine_Param(LINE_ *);				// 2D�p�����g���b�N�����v�f�̕`��


protected:
	GLUnurbsObj *NurbsSurf;						// NURBS�Ȗʗp�I�u�W�F�N�g
	GLUnurbsObj *NurbsCurve;					// NURBS�Ȑ��p�I�u�W�F�N�g
};

#endif
