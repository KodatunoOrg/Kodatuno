// Kodatuno�̃C���^�[�t�F�[�X�֌W���`

#ifndef _KODATUNO_H_
#define _KODATUNO_H_


#include "IGES_Parser.h"
#include "STL_Parser.h"
#include "StdAfx.h"
#include "Quaternion.h"
#include "Describe_BODY.h"
#include "UserFunc.h"
#include "Command.h"

#define EXT_IGES	1						// �g���q���ʗp(IGES)
#define EXT_STL		2						// �g���q���ʗp(STL)
#define CONSOLE_CHAR_NUMMAX	2048			// �R���\�[���ɓ��͂ł��镶�����̍ő�l
#define MAXSELECT	20000					// �Z���N�V�������̍ő�o�b�t�@�T�C�Y
#define BODYLISTMAX	8						// �ǂݍ��߂�BODY�̍ő吔
#define TOLERANCE 20						// NURBS�Ȗʂ̑e���̃f�t�H���g�l
#define HISTORYNUMMAX 10					// �R�}���h����

// NURBS�Ȗʂ̐������@��\���V���{��
#define ROTSURF	0							// ��]�T�[�t�F�X		
#define SWEEPSURF 1							// �X�C�[�v�T�[�t�F�X


// KODatUNO�N���X���`
class KODatUNO : public BODY_Handler		// BODY�֘A�֐����W�߂��N���X���p��
{
// public�֐��̒�`
public:
	void InitializeWin();					// Kodatuno�̏�����
	void DeleteWin();						// Kodatuno�̏I������
	int OpenFile();							// �t�@�C���I�[�v��
	int OpenFile(char *);					// �t�@�C���I�[�v��(�I�[�o�[���[�h)
	int OpenInpFile();						// �C���v�b�g�t�@�C���I�[�v��
	int BodyListSelect();					// �{�f�B���X�g�I��
	void ReDraw(int,int);					// �ĕ`��
	void ReDrawUserFunc();					// UserFunc���s���̍ĕ`��Ăяo��
	void ReDrawUserCommand();				// UserCommand���s���̍ĕ`��Ăяo��
	int GetConsoleCharNum();				// �R���\�[�����������o��
	void SetConsoleCharNum(int);			// �R���\�[����������ۑ�
	void ExecCommand(char *);				// �R�}���h���s
	int GetUpperConsoleHistory(WSCstring *);	// �R�}���h�������擾
	int GetLowerConsoleHistory(WSCstring *);	// �R�}���h�������擾
	void SetConsoleHistory(WSCstring);		// �R�}���h������o�^
	void GetResizeWindow();					// �E�B���h�E�����T�C�Y���ꂽ�Ƃ��ɁA���̃E�C���h�E�T�C�Y��ǂݍ���
	void MouseEvent(long,long,short,short);	// �}�E�X�C�x���g�̏���
	void MouseMotionEvent(long,short,short);// �}�E�X�ړ����̃C�x���g����
	void ChangeViewX();						// X����̎��_�ɐ؂�ւ���
	void ChangeViewY();						// Y����̎��_�ɐ؂�ւ���
	void ChangeViewZ();						// Z����̎��_�ɐ؂�ւ���
	void ChangeViewXYZ();					// ���Վ��_�ɐ؂�ւ���
	WSCbool ChangeViewUp();					// ����L�[�������ꂽ�ꍇ��BODY��]
	WSCbool ChangeViewDown();					// �����L�[�������ꂽ�ꍇ��BODY��]
	WSCbool ChangeViewLeft();					// �����L�[�������ꂽ�ꍇ��BODY��]
	WSCbool ChangeViewRight();					// �E���L�[�������ꂽ�ꍇ��BODY��]
	void FitView();							// �{�����f�t�H���g�ɖ߂�
	void SolidView();						// Solid�\��
	void NoEdgeView();						// Edge���\���ɂ���
	void WireFlameView();					// WireFlame�\��
	void VoidView();						// �\�����Ȃ�
	void SelectAll();						// �S�ăZ���N�g��Ԃɂ���
	void SelectionCancel();					// �Z���N�V�����L�����Z��
	void ViewBlending();					// BODY�𔼓����\��
	void DeleteBody();						// BODY����
	void OpenDelBtn();						// Open�CDelete�{�^���̕\�������̕ύX
	void ExecuteUserFuncMain(int);			// UserFunc�̎��s
	void AcceptUserStat(int,double []);		// UserStatus�̐ݒ�
	void UserViewCanncel();					// UserFunc�ɂ���ĕ`�悳�ꂽ�摜������
	void SetModelScale(double);				// ���f���X�P�[�����Z�b�g
	void GetModelScale();					// ���f���X�P�[���l���擾
	void SetTolerance(double);				// �g�������X�l���Z�b�g
	void GetTolerance();					// �g�������X�l���擾
	void GetShiftBody(Coord);				// BODY�𕽍s�ړ�
	void GetRotateBody(Coord,double);		// BODY����]
	void ExpandBody(Coord);					// BODY���g��
	int GenSurface(Coord,double,int);		// �e��T�[�t�F�X�𐶐�
	int GenNurbsCurve(int,char *,int);		// �w�肵���t�@�C����ǂݍ���,NURBS�Ȑ��𐶐�
	int GenNurbsSurface(int,char *,int);	// �w�肵���t�@�C����ǂݍ���,NURBS�Ȗʂ𐶐�
	void GetSurfInfo();						// �I������Ă���Ȗʂ̏����R���\�[���o�͂���
	void ChangeRank(int []);				// �I������Ă���Ȗʂ�Rank��ύX����

// private�֐��̒�`
private:
	void Describe_Main();				// BODY�`��
	void DrawBODY();					// BODY�`��̃��C����
	void DrawAxis(double,double);		// ���̕`��
	void DrawRubberband(double,double,double,double);	// ���o�[�o���h�̕`��
	void Draw_NurbsCurve(BODY *);		// NURBS�Ȑ���`��
	void Draw_NurbsSurface(BODY *);		// NURBS�Ȑ���`��
	void Draw_TrimSurfe(BODY *);		// �g�����ʂ�`��
	void UVWireView();					// UV�p�����[�^�ŕ������ꂽ���C���[�t���[����\��
	void SetOpenGLStat();				// OpenGL�̏�����
	void SetModelScale();				// �`�掞�̃X�P�[��������
	void SetMaxCoord();					// Body�̍ő咷��ݒ肷��
	void DoSelect(int,int);				// �Z���N�V����(�}�E�X�s�b�L���O)����
	void ClearSeldEntList();			// �Z���N�V�������X�g�y��OBJECT�̏�����
	void DeleteBodySub(BODY *,int);		// BODY����SUB
	void ClickPicking(GLuint SelectBuf[],int hits);	// �I�u�W�F�N�g�̃}�E�X�s�b�L���O(�N���b�N)
	void DragPicking(GLuint SelectBuf[],int hits);	// �I�u�W�F�N�g�̃}�E�X�s�b�L���O(�h���b�O)
	int  ObjSelect(GLuint [],int);					// �I�u�W�F�N�g�I�𔻕�
	int AddEntSymbolToObj();			// OBJECT�\���̂ɉ��Ԗڂ̋Ȑ�or�Ȗʂ��̏���t������
	void InitCmdFlags();				// Cmd�p�t���O��S��KOD_FALSE�ɏ�����
	void SetNewObject(int,int,int);		// �s�b�N���ꂽ�I�u�W�F�N�g��OBJECT���X�g�ɓo�^
	void DescribeCP();						// �R���g���[���|�C���g��`��


// pbulic�ϐ��̒�`
public:
	int  UVWireFlameViewFlag;	// UV���C���[�t���[���\���t���O
	int  CPViewFlag;			// �R���g���[���|�C���g�`��t���O

// private�ϐ��̒�`
private:
	USER User;					// ���[�U�[�N���X
	BODYList	BodyList;		// BODY���X�g�̑���p�C���X�^���X
	OBJECTList SeldEntList;		// �Z���N�V�������ꂽ�G���e�B�e�B�𒀎��i�[���Ă������X�g
	CommandMap CmdMap[COMMANDNUMMAX];		// �R�}���h�}�b�v
	int	ConsoleCharNum;			// �R���\�[���ɓ��͂���Ă���S�Ă̕�������ێ�
	int Focus_Body;				// �I�𒆂�Body��\���l
	QUATERNION QFunc;			// �N�H�[�^�j�I���̃C���X�^���X
	double RotMx[TMATELEMNUM];	// �N�H�[�^�j�I���������ϊ��}�g���b�N�X
	Quat StartQ;				// �E�N���b�N�����u�Ԃ̉�]�p���i�[����N�H�[�^�j�I��
	Quat TargetQ;				// ���肮�����Ă���r���̃N�H�[�^�j�I��
	double ModelScale;			// ���݂̃��f���X�P�[��
	double ModelScale1st;		// ���f���X�P�[���̏����l
	double Tolerance;			// NURBS��\�����鎞�̖ʂ̑e��
	double Trl[3];				// ���s�ړ���
	int  Vp[4];					// �r���[�|�C���g
	double AxisScale;			// ���`��̃X�P�[��
	int  OpenGLInitFlag;		// OpenGL�����ݒ�ς݃t���O
	int  ExecUserFuncFlag[USERFUNCNUMMAX];	// UserFunc�`��t���O
	int  ExecUserCommandFlag[USERCOMMANDNUM];	// UserCommand�`��t���O
	int  DoSelectFlag;			// �Z���N�V�������s�t���O
	int  SweepSelectFlag;		// �X�C�[�v�Z���N�V�������J�n�������Ƃ������t���O
	int  ScreenHeight;			// �f�t�H���g�X�N���[���̍���
	int  ScreenWidth;			// �f�t�H���g�X�N���[���̕�
	int  StartX;				// �h���b�O�J�n�ʒuX
	int  StartY;				// �h���b�O�J�n�ʒuY
	int  OldPosX;				// �E�B���h�E�ĕ`�撼�O��X���W
	int  OldPosY;				// �E�B���h�E�ĕ`�撼�O��Y���W
	int  CurrentX;				// �h���b�O���̈ʒuX
	int  CurrentY;				// �h���b�O���̈ʒuY
	int  LBtnFlag;				// ���N���b�N�������Ƃ������t���O
	int  RBtnFlag;				// �E�N���b�N�������Ƃ������t���O
	int  MBtnFlag;				// ���N���b�N�������Ƃ������t���O
	int  ShiftKeyFlag;			// Shift�L�[�����������Ƃ������t���O
	int  CtrlKeyFlag;			// Ctrl�L�[�����������Ƃ������t���O
	int  AltKeyFlag;			// Alt�L�[�����������Ƃ������t���O
	int  SolidViewFlag;			// �\���b�h�\���t���O
	int  EdgeViewFlag;			// �G�b�W�\���t���O
	int  WireFlameViewFlag;		// ���C���[�t���[���\���t���O
	int  BlendViewFlag;			// �������\���t���O
	int  DrawBODYFlag;			// BODY�`��t���O
	int  DrawSurfFlag;			// Surface�`��t���O
	int	 ReDrawBODYFlag;		// BODY�`��1���ڂ������t���O
	GLuint SelectBuf[MAXSELECT];	// �Z���N�g�o�b�t�@
	int argc;						// �R�}���h����
	char *argv[MAXCOMMANDOPNUM];	// �R�}���h����
	char CmdStack[HISTORYNUMMAX][256];	// �R�}���h����
	int CmdStackNum;				// �R�}���h����
	int CmdStackNow;

};

// Kodatuno�N���X�̃O���[�o���錾
extern KODatUNO	Kodatuno;

// Kodatuno�V�X�e���R�}���h��o�^
void CmdVerInf(int,char *[]);			// �R�}���h(�o�[�W�������o��)
void CmdFileOpen(int,char *[]);			// �R�}���h(�t�@�C���I�[�v��)
void CmdUVWire(int,char *[]);			// �R�}���h(UV���C���[�t���[���\��)
void CmdChScale(int,char *[]);			// �R�}���h(�X�P�[���ύX)
void CmdGenRotSurf(int,char *[]);		// �R�}���h(��]�T�[�t�F�X����)
void CmdGenSweepSurf(int,char *[]);		// �R�}���h(�X�C�[�v�T�[�t�F�X����)
void CmdGenNurbsCurve(int,char *[]);	// �R�}���h(Nurbs�Ȑ�����)
void CmdGenNurbsSurface(int,char *[]);	// �R�}���h(Nurbs�Ȗʐ���)
void CmdChTolerance(int,char *[]);		// �R�}���h(�g�������X�ύX)
void CmdMoveBody(int,char *[]);			// �R�}���h(BODY�̕��s�ړ�)
void CmdRotateBody(int ,char *[]);		// �R�}���h(BODY�̉�])
void CmdCPView(int ,char *[]);			// �R�}���h(�R���g���[���|�C���g�`��)
void CmdSurfInfo(int,char *[]);			// �R�}���h(�Ȗʏ����o��)
void CmdExpand(int,char *[]);			// �R�}���h(BODY�̊g��)
void CmdChRank(int,char *[]);			// �R�}���h(NURBS Rnak�̕ύX)
#endif
