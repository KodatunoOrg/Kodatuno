#ifndef _BODY_DESCRIBE_H_
#define _BODY_DESCRIBE_H_

#include "BODY.h"
#include "Describe_BODY.h"
#include "IGES_Parser.h"
#include "Quaternion.h"
#include "NURBS_Func.h"
#include "UserFunc.h"
#include "StdAfx.h"

#define CIRCLE_DIV_NUM 100	// �~�`�掞�̉~������
#define MAXSELECT	2000	// �Z���N�V�������̍ő�o�b�t�@�T�C�Y

// ���j���[�penum
enum Menu{
	M_Version,
	M_Draw_Body,
	M_Draw_Surf,
	M_UnSelect,
	M_View_Angle,
};

enum Sub_Menu_View{
	S_M_XView,
	S_M_YView,
	S_M_ZView
};

// IGES�`��N���X���`
class DESCRIBE : public Describe_BODY			// BODY�`��֐����W�߂��N���X���p��
{
public:
	DESCRIBE();		// �R���X�g���N�^
	~DESCRIBE();	// �f�X�g���N�^
	int BODY_Describe_Main(int,char **);		// �`��main
	
// �֐�
private:
	// OpenGL�p�`��֐��̐錾��static���K�v
	static void Keyboard(unsigned char key, int x, int y);	// �L�[�{�[�h�C�x���g
	static void Display(void);								// �`��C�x���g
	static void Motion(int,int);							// �}�E�X����C�x���g
	static void Reshape(int,int);							// �ĕ`��C�x���g
	static void Mouse(int,int,int,int);						// �}�E�X�C�x���g
	static void Menu(int);									// ���j���[�����C�x���g
	static void Sub_Menu_View(int);							// �T�u���j���[�i���_�؊��j�����C�x���g
	static void Sub_Menu_User(int);							// �T�u���j���[�iUser�o�^�֐��j�����C�x���g
	static void DoSelect(int,int);							// �Z���N�V�����̐ݒ�
	static int  ObjSelect(GLuint [],int);					// �I�u�W�F�N�g�I�𔻕�
	static void Init(char *progname);						// �`�揉����
	static void Idle();										// �`��A�C�h�����O��Ԃł̏����C�x���g
	static void Special(int,int,int);						// ����L�[�C�x���g
	static void glDrawBODY();								// IGES�f�[�^�̕`��
	static void glDrawAxis(double,double);					// ���̕`��
	static void glDrawRubberband(double,double,double,double);	// ���o�[�o���h�̕`��
	static void glDraw_Line();								// ������`��
	static void glDraw_NurbsCurve();						// NURBS�Ȑ���`��
	static void glDraw_SurfLine();							// NURBS�Ȗʂ̃p�����[�^���C����`��
	static void glDraw_TrimSurfe();							// 
	static int GenBodyList();								// body��ǂݍ��݃��X�g�ɓo�^
	static int NewPath();									// SPath,CPath�̃������[���Z���N�g���ꂽ�G���e�B�e�B���m�ۂ���
	static void AddEntSymbolToObj();						// OBJECT�\���̂ɉ��Ԗڂ̋Ȑ�or�Ȗʂ��̏���t������
	static int DelPath();									// SPath,CPath�̃������[���
	static void UpDateDisplayList(int);						// �f�B�X�v���C���X�g�̍X�V
	static void ClickPicking(GLuint SelectBuf[],int hits);	// �I�u�W�F�N�g�̃}�E�X�s�b�L���O(�N���b�N)
	static void DragPicking(GLuint SelectBuf[],int hits);	// �I�u�W�F�N�g�̃}�E�X�s�b�L���O(�h���b�O)
	static void VersionInfo();								// �o�[�W�������o��

// �ϐ�
private:
	static BODY *glbody;			// �ǂݍ���BODY���X�g�̐擪�|�C���^
	static BODY *Curr_body;			// BODY���X�g�ɂ����钍�ڒ���BODY�I�u�W�F�N�g���w��
	static int  ScreenHeight;		// �f�t�H���g�X�N���[���̍���
	static int  ScreenWidth;		// �f�t�H���g�X�N���[���̕�
	static int  StartX;				// �h���b�O�J�n�ʒuX
	static int  StartY;				// �h���b�O�J�n�ʒuY
	static int  OldPosX;			// �E�B���h�E�ĕ`�撼�O��X���W
	static int  OldPosY;			// �E�B���h�E�ĕ`�撼�O��Y���W
	static int  CurrentX;			// �h���b�O���̈ʒuX
	static int  CurrentY;			// �h���b�O���̈ʒuY
	static int  SweepSelectFlag;	// �X�C�[�v�Z���N�V�������J�n�������Ƃ������t���O
	static int  Vp[4];				// �r���[�|�C���g
	static GLuint  SelectBuf[MAXSELECT];	// �Z���N�g�o�b�t�@
	static int  LBtnFlag;			// ���N���b�N�������Ƃ������t���O
	static int  RBtnFlag;			// �E�N���b�N�������Ƃ������t���O
	static int  MBtnFlag;			// ���N���b�N�������Ƃ������t���O
	static int  ShiftKeyFlag;		// Shift�L�[�����������Ƃ������t���O
	static int  CtrlKeyFlag;		// Ctrl�L�[�����������Ƃ������t���O
	static int  AltKeyFlag;			// Alt�L�[�����������Ƃ������t���O
	static double Trl[3];			// ���s�ړ���
	static double AxisScale;		// ���`��̃X�P�[��
	static double ModelScale;		// ���f���̃X�P�[��
	static double ModelScale1st;	// ���f���X�P�[���̏����l
	static double IdentX;			// �}�E�X�̐�ΈʒuX���E�B���h�E���ł̑��Έʒu�̊��Z�W��X
	static double IdentY;			// �}�E�X�̐�ΈʒuY���E�B���h�E���ł̑��Έʒu�̊��Z�W��Y
	static Quat StartQ;				// �E�N���b�N�����u�Ԃ̉�]�p���i�[����N�H�[�^�j�I��
	static Quat TargetQ;			// ���肮�����Ă���r���̃N�H�[�^�j�I��
	static double RotMx[TMATELEMNUM];	// �N�H�[�^�j�I���������ϊ��}�g���b�N�X
	static int  DrawSurfFlagCount;	// Surface�`��t���O���j���[������I�����ꂽ���𐔂���J�E���^
	static int  DrawBODYFlagCount;	// BODY�`��t���O���j���[������I�����ꂽ���𐔂���J�E���^
	static int DrawUserFuncFlag;			// PDT�`��t���O
	static OBJECT SelectedEntList[MAXSELECT];	// �Z���N�V�������ꂽ�G���e�B�e�B�𒀎��i�[���Ă���
	static int PickCount;			// �s�b�N�񐔂��J�E���g
	static int BodyCount;			// body�̐����J�E���g
	static int CPickCount;			// NURBS�Ȑ����s�b�N���ꂽ�����J�E���g
	static int SPickCount;			// NURBS�Ȗʂ��s�b�N���ꂽ�����J�E���g

private:
	static IGES_PARSER Parser;	// IGES�p�[�T�[�p�I�u�W�F�N�g�𐶐�
	static QUATERNION QFunc;	// �N�H�[�^�j�I���N���X�̃I�u�W�F�N�g�𐶐�
	static NURBS_Func NFunc;	// NURBS_Func�N���X�̃I�u�W�F�N�g�𐶐�
	static USER User;			// ���[�U�[�N���X�̃I�u�W�F�N�g�𐶐�

};


#endif
