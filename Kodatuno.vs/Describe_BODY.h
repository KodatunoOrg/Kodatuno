// BODY�`��p�N���X���`

#ifndef _DESCRIBE_BODY_H_
#define _DESCRIBE_BODY_H_

#include "BODY.h"
#include "NURBS_Func.h"

// Constants: General Defines
// COMMAND_DRAW_BOD					BODY�`��p�f�B�X�v���C���X�g�̓o�^�ԍ�(1)
// COMMAND_DRAW_USER				User���C���֐��ɂ���ăR�[�����ꂽOpenGL�`��֐��p�f�B�X�v���C���X�g�̓o�^�ԍ�(2)
// COMMAND_DRAW_USER_COMMAND		User Command�ɂ���ăR�[�����ꂽOpenGL�`��֐��p�f�B�X�v���C���X�g�̓o�^�ԍ�(100)
#define COMMAND_DRAW_BODY  1
#define COMMAND_DRAW_USER  2
#define COMMAND_DRAW_USER_COMMAND 100

// Class: Describe_BODY
// BODY�G���e�B�e�B��`�悷��֐����W�߂��N���X
class Describe_BODY
{
public:
	// Constructor: Describe_BODY
	// Describe_BODY�N���X�̃R���X�g���N�^�DNURBS�`��X�e�[�^�X�̐ݒ�
	Describe_BODY();

	// Destructor: ~Describe_BODY
	// Describe_BODY�N���X�̃f�X�g���N�^�D�X�P���g���ł��D
	~Describe_BODY();

	// Function: DrawBody
	// BODY��`��
	static void DrawBody(BODY *);					

	// Function: DrawCompositeCurve
	// �����Ȑ���`��
	static void DrawCompositeCurve(COMPC *);		

	// Function: DrawLine
	// ������`��
//	static void DrawLine(LINE_);					
	static void DrawLine(const LINE_&);					

	// Function: DrawCircleArc
	// �~�E�~�ʂ�`��
//	static void DrawCircleArc(CIRA);				
	static void DrawCircleArc(const CIRA&);				

	// Function: DrawNurbsCurve
	// NURBS�Ȑ���`��
//	static void DrawNurbsCurve(NURBSC);				
	static void DrawNurbsCurve(const NURBSC&);				

	// Function: DrawTrimdNurbsSurfe
	// NURBS�Ȗʂ̕`��(�g��������)
	static void DrawTrimdNurbsSurfe(NURBSS *);		

	// Function: DrawNurbsSurfe
	// NURBS�Ȗʂ̕`��(�g�����Ȃ�)
//	static void DrawNurbsSurfe(NURBSS);				
	static void DrawNurbsSurfe(const NURBSS&);				

	// Function: DrawCurveOnParamSurfe
	// �ʏ����`��
	static void DrawCurveOnParamSurfe(CONPS *);		

	// Function: DrawTrimdSurf
	// �g�����ʂ�`��
//	static void DrawTrimdSurf(TRMS);				
	static void DrawTrimdSurf(const TRMS&);				

	// Function: DrawNurbsCurve_Param
	// 2D�p�����g���b�NNURBS�Ȑ��v�f�̕`��
	static void DrawNurbsCurve_Param(NURBSC *);		

	// Function: Draw_Lines
	// BODY�Ɋ܂܂�������S�ĕ`��
	static void Draw_Lines(BODY *Body);				

	// Function: Draw_CircleArcs
	// BODY�Ɋ܂܂��~�C�~�ʂ�S�ĕ`��
	static void Draw_CircleArcs(BODY *Body);		

	// Function: Draw_NurbsCurves
	// BODY�Ɋ܂܂��NURBS�Ȑ���S�ĕ`��
	static void Draw_NurbsCurves(BODY *Body);		

	// Function: Draw_NurbsSurfaces
	// BODY�Ɋ܂܂��NURBS�Ȗʂ�S�ĕ`��
	static void Draw_NurbsSurfaces(BODY *Body);		

	// Function: Draw_TrimSurfes
	// BODY�Ɋ܂܂��g�����ʂ�S�ĕ`��
	static void Draw_TrimSurfes(BODY *Body);		

	// Function: 
	// NURBS�`�掞�̕`��X�e�[�^�X��ݒ�
	static void SetNurbsStat();						

	// Function: SetNurbsSTolerance
	// NURBS�Ȗʂ̑e�����w��
	static void SetNurbsSTolerance(GLfloat);		

	// Function: NURBS_Err
	// NURBS�`�掞�̃G���[�R�[���o�b�N
	static void NURBS_Err(GLenum error_code);		

	// Function: SetNurbsSProperty
	// NURBS�Ȗʂ̕`��`����ύX����
	static void SetNurbsSProperty(GLenum,GLfloat);	

	// Function: DrawMesh
	// ���b�V���̕`��
	static void DrawMesh(MESH *,int);				

	// ������
	static void DrawCircleArc();					// �~�E�~�ʂ�`��
	static void DrawConicArc();						// �~���Ȑ���`��
	static void DrawCircleArc_Param(CIRA *);		// 2D�p�����g���b�N�~�v�f�̕`��
	static void DrawConicArc_Param(CONA *);			// 2D�p�����g���b�N�~���Ȑ��v�f�̕`��
	static void DrawLine_Param(LINE_ *);			// 2D�p�����g���b�N�����v�f�̕`��


protected:
	// Variable:
	// NURBS�Ȗʗp�I�u�W�F�N�g
	static GLUnurbsObj *NurbsSurf;			

	// Variable:
	// NURBS�Ȑ��p�I�u�W�F�N�g
	static GLUnurbsObj *NurbsCurve;					
};

#endif
